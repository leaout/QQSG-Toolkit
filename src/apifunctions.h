#pragma once
// ============================================================
// API 钩子回调函数
// 来自 QQSG-Hacker apifunctions.h
// ============================================================

#include <Windows.h>

// ============================================================
// API 钩子回调函数（需要定义 extern 变量才可使用）
// 用法: 
//   1. 在 .cpp 中定义: fGetCursorInfo hookCursorInfo;
//   2. hook::InitializeHook(&hook, "user32.dll", "GetCursorInfo", HonkGetCursorInfo);
// ============================================================

//// GetCursorInfo 钩子
//typedef BOOL(WINAPI* fGetCursorInfo)(PCURSORINFO pci);
//extern fGetCursorInfo hookCursorInfo;
//
//inline BOOL HonkGetCursorInfo(PCURSORINFO pci) {
//    return hookCursorInfo(pci);
//}
//
//// GetCursorPos 钩子
//typedef BOOL(WINAPI* fGetCursorPos)(LPPOINT lpPoint);
//extern fGetCursorPos HookCursorPos;
//
//inline BOOL HonkGetCursorPos(LPPOINT lpPoint) {
//    return HookCursorPos(lpPoint);
//}
//
//// GetCursor 钩子
//typedef HCURSOR(WINAPI* fGetCursor)();
//extern fGetCursor HookCursor;
//
//inline HCURSOR HonkGetCursor() {
//    return HookCursor();
//}
