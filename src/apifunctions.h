#pragma once
// ============================================================
// API 钩子回调函数
// 来自 QQSG-Hacker apifunctions.h
// ============================================================

#include <Windows.h>

// GetCursorInfo 钩子
typedef BOOL(WINAPI* fGetCursorInfo)(PCURSORINFO pci);
extern fGetCursorInfo hookCursorInfo;

inline BOOL HonkGetCursorInfo(PCURSORINFO pci) {
    // printf("CursorInfo is called\n");
    return hookCursorInfo(pci);
}

// GetCursorPos 钩子
typedef BOOL(WINAPI* fGetCursorPos)(LPPOINT lpPoint);
extern fGetCursorPos HookCursorPos;

inline BOOL HonkGetCursorPos(LPPOINT lpPoint) {
    // printf("CursorPos is called\n");
    return HookCursorPos(lpPoint);
}

// GetCursor 钩子
typedef HCURSOR(WINAPI* fGetCursor)();
extern fGetCursor HookCursor;

inline HCURSOR HonkGetCursor() {
    // printf("Cursor is called\n");
    return HookCursor();
}
