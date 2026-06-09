// ============================================================
// QQSG-Toolkit - DLL Main Entry Point
//
// 注入方式：
//   方法一：DLL劫持 — 将编译好的 dll 重命名为 msacm32.drv，
//           放入 QQ三国根目录，启动游戏自动加载
//   方法二：直接注入 — 使用任意注入器 LoadLibrary 注入
//
// 整合自：QQSG-Hacker + qqsg_struct
// ============================================================

#include <Windows.h>
#include <cstdio>
#include <thread>
#include <chrono>
#include "d3d9_manager.h"
#include "game_data.h"

DWORD WINAPI InitializeThread(LPVOID mod) {
    const auto current_module = static_cast<HMODULE>(mod);
    const auto d3d9_module = GetModuleHandle(TEXT("d3d9.dll"));

    // 分配控制台（用于调试输出）
    bool allocate_console = (GetConsoleWindow() == nullptr);
    if (allocate_console) {
        AllocConsole();
        freopen_s(reinterpret_cast<FILE**>(stdout), "CONOUT$", "w", stdout);
        SetConsoleTitleA("QQSG-Toolkit Debug");
    }

    printf("[QQSG-Toolkit] Initializing...\n");
    printf("[QQSG-Toolkit] LOCALPLAYER = 0x%08X\n", LOCALPLAYER);
    printf("[QQSG-Toolkit] INTYPE = 0x%08X\n", INTYPE);

    if (d3d9_module) {
        if (FindAndHookD3D9(d3d9_module))
            printf("[QQSG-Toolkit] D3D9 hook successful!\n");
        else
            printf("[QQSG-Toolkit] D3D9 hook failed!\n");
    } else {
        printf("[QQSG-Toolkit] d3d9.dll not loaded, waiting...\n");
        // 等待 d3d9.dll 加载
        for (int i = 0; i < 100; i++) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            HMODULE d3d = GetModuleHandle(TEXT("d3d9.dll"));
            if (d3d) {
                printf("[QQSG-Toolkit] d3d9.dll loaded, hooking...\n");
                FindAndHookD3D9(d3d);
                break;
            }
        }
    }

    // 主循环，等待 Delete 键卸载
    printf("[QQSG-Toolkit] Running. Press DELETE to unload.\n");
    while (!g_D3D9.PressedKeys[VK_DELETE]) {
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }

    printf("[QQSG-Toolkit] Unloading...\n");
    g_D3D9.TryingToUnload = true;

    while (!g_D3D9.ReadyToUnload) {
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }

    // 还原钩子
    UnhookD3D9();

    if (allocate_console) {
        fclose(stdout);
        FreeConsole();
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    FreeLibraryAndExitThread(current_module, 0);
    return 0;
}

// DLL 入口
BOOL WINAPI DllMain(HMODULE mod, DWORD reason, PVOID) {
    if (reason == DLL_PROCESS_ATTACH) {
        // 为防止死锁，创建独立线程初始化
        HANDLE thread = CreateThread(nullptr, 0, InitializeThread, mod, 0, nullptr);
        if (thread) {
            // 确保 d3d9.dll 被加载
            LoadLibraryA("d3d9.dll");
            CloseHandle(thread);
        }
        return TRUE;
    }
    return TRUE;
}
