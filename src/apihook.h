#pragma once
// ============================================================
// API 钩子工具
// 来自 QQSG-Hacker apihook.h
// 使用 JMP 重定位实现 API 函数拦截
// ============================================================

#include <Windows.h>

struct hook_t {
    bool isHooked;
    void* FunctionAddress;
    void* Hook;
    char  Jmp[6];
    char  APIBytes[6];
    void* APIFunction;
};

namespace hook {
    inline bool InitializeHook(hook_t* Hook, const char* ModuleName, const char* FunctionName, void* HookFunction) {
        if (Hook->isHooked)
            return false;

        HMODULE hModule = GetModuleHandleA(ModuleName);
        if (!hModule) {
            Hook->isHooked = false;
            return false;
        }

        Hook->FunctionAddress = GetProcAddress(hModule, FunctionName);
        if (!Hook->FunctionAddress) {
            Hook->isHooked = false;
            return false;
        }

        Hook->Jmp[0] = 0xe9;
        *(PULONG)&Hook->Jmp[1] = (ULONG)HookFunction - (ULONG)Hook->FunctionAddress - 5;
        memcpy(Hook->APIBytes, Hook->FunctionAddress, 5);

        Hook->APIFunction = VirtualAlloc(NULL, 4096, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
        if (!Hook->APIFunction)
            return false;

        memcpy(Hook->APIFunction, Hook->APIBytes, 5);
        ULONG OrigFunc = (ULONG)Hook->APIFunction + 5;
        ULONG FuncAddr = (ULONG)Hook->FunctionAddress + 5;
        *(LPBYTE)((LPBYTE)Hook->APIFunction + 5) = 0xe9;
        *(PULONG)((LPBYTE)Hook->APIFunction + 6) = (ULONG)FuncAddr - (ULONG)OrigFunc - 5;
        Hook->isHooked = true;
        return true;
    }

    inline bool InitializeByAddress(hook_t* Hook, void* Address, void* HookFunction) {
        if (Hook->isHooked)
            return false;

        Hook->FunctionAddress = Address;
        Hook->Jmp[0] = 0xe9;
        *(PULONG)&Hook->Jmp[1] = (ULONG)HookFunction - (ULONG)Hook->FunctionAddress - 5;
        memcpy(Hook->APIBytes, Hook->FunctionAddress, 5);

        Hook->APIFunction = VirtualAlloc(NULL, 4096, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
        if (!Hook->APIFunction)
            return false;

        memcpy(Hook->APIFunction, Hook->APIBytes, 5);
        ULONG OrigFunction = (ULONG)Hook->APIFunction + 5;
        ULONG FunctionAddress = (ULONG)Hook->FunctionAddress + 5;
        *(LPBYTE)((LPBYTE)Hook->APIFunction + 5) = 0xe9;
        *(PULONG)((LPBYTE)Hook->APIFunction + 6) = (ULONG)FunctionAddress - (ULONG)OrigFunction - 5;
        Hook->isHooked = true;
        return true;
    }

    inline bool InsertHook(hook_t* Hook) {
        if (!Hook->isHooked) return false;
        DWORD op;
        VirtualProtect(Hook->FunctionAddress, 5, PAGE_EXECUTE_READWRITE, &op);
        memcpy(Hook->FunctionAddress, Hook->Jmp, 5);
        VirtualProtect(Hook->FunctionAddress, 5, op, &op);
        return true;
    }

    inline bool Unhook(hook_t* Hook) {
        if (!Hook->isHooked) return false;
        DWORD op;
        VirtualProtect(Hook->FunctionAddress, 5, PAGE_EXECUTE_READWRITE, &op);
        memcpy(Hook->FunctionAddress, Hook->APIBytes, 5);
        VirtualProtect(Hook->FunctionAddress, 5, op, &op);
        Hook->isHooked = false;
        return true;
    }

    inline bool FreeHook(hook_t* Hook) {
        if (Hook->isHooked) return false;
        VirtualFree(Hook->APIFunction, 0, MEM_RELEASE);
        memset(Hook, 0, sizeof(hook_t));
        return true;
    }
};
