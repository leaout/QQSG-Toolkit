#pragma once
// ============================================================
// D3D9 设备管理与 Hook
// 不依赖 d3dx9
// ============================================================

#include <d3d9.h>
#include <cstdint>
#include <cstdio>
#include <thread>
#include <chrono>
#include "renderer.h"

struct D3D9Manager {
    // D3D9 设备
    IDirect3DDevice9* Device = nullptr;

    // 窗口
    HWND GameWindow = nullptr;
    int Width = 1030;
    int Height = 797;

    // 按键状态
    bool PressedKeys[256] = { false };

    // 状态
    bool Initialized = false;
    bool TryingToUnload = false;
    bool ReadyToUnload = false;

    // 函数指针
    using EndScene_t = long(__stdcall*)(IDirect3DDevice9*);
    using BeginScene_t = long(__stdcall*)(IDirect3DDevice9*);
    using Present_t = long(__stdcall*)(IDirect3DDevice9*, RECT*, RECT*, HWND, RGNDATA*);
    using Reset_t = long(__stdcall*)(IDirect3DDevice9*, D3DPRESENT_PARAMETERS*);

    EndScene_t OriginalEndScene = nullptr;
    BeginScene_t OriginalBeginScene = nullptr;
    Present_t OriginalPresent = nullptr;
    Reset_t OriginalReset = nullptr;

    LONG_PTR OriginalWndProc = NULL;
    HHOOK LowLevelMouseHook = NULL;

    bool Sleeper = 0;
};

// 全局单例
extern D3D9Manager g_D3D9;

// D3D9 相关函数
bool FindAndHookD3D9(const HMODULE d3d9_module);
void UnhookD3D9();
