#include "d3d9_manager.h"
#include "pattern.h"
#include "base.h"
#include "overlay.h"
#include <detours.h>

D3D9Manager g_D3D9;

// 钩子工具变量
using hook_fn = void(*)();

uintptr_t g_d3d9_device_new = 0;
uintptr_t g_return_address = 0;

base::sync::recursive_spinlock_t g_endscene_spinlock_;
base::sync::recursive_spinlock_t g_wndproc_spinlock_;

// ----- Detours Hook 辅助 -----
void HookFunction(PVOID* oFunction, PVOID pDetour) {
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourAttach(oFunction, pDetour);
    DetourTransactionCommit();
}

void UnhookFunction(PVOID* oFunction, PVOID pDetour) {
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourDetach(oFunction, pDetour);
    DetourTransactionCommit();
}

// ----- CFG 钩子（用于捕获 D3D9 设备指针） -----
__declspec(naked) void cfg_hook() {
    __asm {
        push eax
        mov eax, [esp + 4]
        cmp eax, [g_return_address]
        jne not_endscene
        mov [g_d3d9_device_new], ebx
        sub [g_d3d9_device_new], 4
        not_endscene :
        pop eax
            ret
    }
}

hook_fn patch(const uintptr_t address, hook_fn function) {
    DWORD protection = 0;
    const auto original = hook_fn(*reinterpret_cast<uintptr_t*>(address));
    if (VirtualProtect(LPVOID(address), sizeof(uintptr_t), PAGE_READWRITE, &protection) == FALSE)
        return nullptr;
    *reinterpret_cast<uintptr_t*>(address) = uintptr_t(function);
    VirtualProtect(LPVOID(address), sizeof(uintptr_t), protection, &protection);
    return original;
}

uintptr_t get_return_address(const HMODULE d3d_module) {
    constexpr auto return_addr_pattern = "8B CF FF D6 8B 4D 08 8B 41 3C FF 40 08 83 4D FC FF";

    MODULEINFO moduleinfo = { nullptr };
    if (GetModuleInformation(GetCurrentProcess(), d3d_module, &moduleinfo, sizeof(moduleinfo)) == FALSE)
        return 0;

    return uintptr_t(FindPattern(PBYTE(moduleinfo.lpBaseOfDll), moduleinfo.SizeOfImage, return_addr_pattern));
}

// ----- WndProc 钩子 -----
LRESULT __stdcall WndProc_h(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    std::unique_lock<base::sync::recursive_spinlock_t> l{ g_wndproc_spinlock_ };

    switch (uMsg) {
    case WM_LBUTTONDOWN:  g_D3D9.PressedKeys[VK_LBUTTON] = true; break;
    case WM_LBUTTONUP:    g_D3D9.PressedKeys[VK_LBUTTON] = false; break;
    case WM_RBUTTONDOWN:  g_D3D9.PressedKeys[VK_RBUTTON] = true; break;
    case WM_RBUTTONUP:    g_D3D9.PressedKeys[VK_RBUTTON] = false; break;
    case WM_KEYDOWN:      g_D3D9.PressedKeys[wParam] = true; break;
    case WM_KEYUP:        g_D3D9.PressedKeys[wParam] = false; break;
    default: break;
    }

    return CallWindowProcW((WNDPROC)g_D3D9.OriginalWndProc, hWnd, uMsg, wParam, lParam);
}

// ----- D3D9 钩子回调 -----
static bool renderer_inited = false;

HRESULT __stdcall EndScene_h(IDirect3DDevice9* pDevice) {
    std::unique_lock<base::sync::recursive_spinlock_t> l{ g_endscene_spinlock_ };

    // 延迟一帧初始化渲染器
    if (!renderer_inited) {
        if (g_Renderer.Init(pDevice)) {
            renderer_inited = true;
            printf("[QQSG-Toolkit] Renderer initialized\n");
        }
    }

    if (renderer_inited) {
        // 读取游戏内存并绘制 Overlay
        DrawOverlay();
    }

    g_D3D9.ReadyToUnload = true;
    return g_D3D9.OriginalEndScene(pDevice);
}

HRESULT __stdcall BeginScene_h(IDirect3DDevice9* pDevice) {
    g_D3D9.ReadyToUnload = false;

    if (!g_D3D9.Initialized) {
        D3DDEVICE_CREATION_PARAMETERS pPara;
        pDevice->GetCreationParameters(&pPara);
        g_D3D9.GameWindow = pPara.hFocusWindow;
        g_D3D9.Device = pDevice;

        // Hook WndProc
        g_D3D9.OriginalWndProc = SetWindowLongPtr(g_D3D9.GameWindow, GWLP_WNDPROC, (LONG_PTR)WndProc_h);

        g_D3D9.Initialized = true;
        printf("[QQSG-Toolkit] Initialized - Window: 0x%p\n", g_D3D9.GameWindow);
    }

    return g_D3D9.OriginalBeginScene(pDevice);
}

HRESULT __stdcall Reset_h(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters) {
    renderer_inited = false;
    g_Renderer.Release();
    return g_D3D9.OriginalReset(pDevice, pPresentationParameters);
}

HRESULT __stdcall Present_h(IDirect3DDevice9* pDevice, RECT* pSourceRect, RECT* pDestRect,
    HWND hDestWindowOverride, RGNDATA* pDirtyRegion) {
    return g_D3D9.OriginalPresent(pDevice, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
}

// ----- 查找并 Hook D3D9 -----
hook_fn g_original_fn = nullptr;
uintptr_t g_guard_function = 0;

bool FindAndHookD3D9(const HMODULE d3d9_module) {
    g_return_address = get_return_address(d3d9_module);
    if (g_return_address == 0) {
        printf("[QQSG-Toolkit] Could not find return address\n");
        return false;
    }
    printf("[QQSG-Toolkit] Return address: 0x%08X\n", g_return_address);

    if (ImageNtHeader(d3d9_module) == nullptr) {
        printf("[QQSG-Toolkit] d3d9 module is not a valid PE image!\n");
        return false;
    }

    // 获取 Load Config 目录项
    ULONG directory_size = 0;
    const auto load_config = PIMAGE_LOAD_CONFIG_DIRECTORY(
        ImageDirectoryEntryToData(d3d9_module, TRUE, IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG, &directory_size));
    if (load_config == nullptr) {
        printf("[QQSG-Toolkit] Could not find IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG\n");
        return false;
    }

    g_guard_function = load_config->GuardCFCheckFunctionPointer;
    printf("[QQSG-Toolkit] Guard function: 0x%08X\n", g_guard_function);

    g_original_fn = patch(g_guard_function, cfg_hook);
    if (g_original_fn == nullptr) {
        printf("[QQSG-Toolkit] Could not patch GuardCFCheckFunctionPointer\n");
        return false;
    }

    printf("[QQSG-Toolkit] Original guard function: 0x%p\n", g_original_fn);

    // 等待 D3D9 设备被捕获
    while (true) {
        if (HIBYTE(GetAsyncKeyState(VK_DELETE)))
            break;

        if (g_d3d9_device_new != 0) {
            printf("[QQSG-Toolkit] D3D9 device: 0x%I32X\n", g_d3d9_device_new);

            DWORD_PTR* pDeviceVT = reinterpret_cast<DWORD_PTR*>(g_d3d9_device_new);
            pDeviceVT = reinterpret_cast<DWORD_PTR*>(pDeviceVT[0]);

            g_D3D9.OriginalEndScene   = reinterpret_cast<D3D9Manager::EndScene_t>(pDeviceVT[42]);
            g_D3D9.OriginalBeginScene = reinterpret_cast<D3D9Manager::BeginScene_t>(pDeviceVT[41]);
            g_D3D9.OriginalReset      = reinterpret_cast<D3D9Manager::Reset_t>(pDeviceVT[16]);
            g_D3D9.OriginalPresent    = reinterpret_cast<D3D9Manager::Present_t>(pDeviceVT[17]);

            printf("[QQSG-Toolkit] EndScene: 0x%I32X\n", pDeviceVT[42]);
            printf("[QQSG-Toolkit] BeginScene: 0x%I32X\n", pDeviceVT[41]);

            // Hook D3D9 函数
            HookFunction(reinterpret_cast<PVOID*>(&g_D3D9.OriginalBeginScene), BeginScene_h);
            HookFunction(reinterpret_cast<PVOID*>(&g_D3D9.OriginalReset), Reset_h);
            HookFunction(reinterpret_cast<PVOID*>(&g_D3D9.OriginalPresent), Present_h);
            HookFunction(reinterpret_cast<PVOID*>(&g_D3D9.OriginalEndScene), EndScene_h);

            printf("[QQSG-Toolkit] D3D9 Hooking Successful\n");
            Beep(650, 50);
            break;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    return true;
}

void UnhookD3D9() {
    patch(g_guard_function, g_original_fn);
    printf("[QQSG-Toolkit] Unhooked guard function\n");

    UnhookFunction(reinterpret_cast<PVOID*>(&g_D3D9.OriginalPresent), Present_h);
    UnhookFunction(reinterpret_cast<PVOID*>(&g_D3D9.OriginalBeginScene), BeginScene_h);
    UnhookFunction(reinterpret_cast<PVOID*>(&g_D3D9.OriginalEndScene), EndScene_h);
    UnhookFunction(reinterpret_cast<PVOID*>(&g_D3D9.OriginalReset), Reset_h);

    if (g_D3D9.OriginalWndProc)
        SetWindowLongPtr(g_D3D9.GameWindow, GWL_WNDPROC, g_D3D9.OriginalWndProc);
    if (g_D3D9.LowLevelMouseHook)
        UnhookWindowsHookEx(g_D3D9.LowLevelMouseHook);

    g_Renderer.Release();

    printf("[QQSG-Toolkit] All hooks removed\n");
}
