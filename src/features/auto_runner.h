#pragma once
// ============================================================
// 自动跑路系统
// 基于 QQSG-Hacker Runner.h 重构
// ============================================================

#include <Windows.h>

class AutoRunner {
public:
    static HWND GameWindow;

    // 按键工具
    static void KeyDown(int key);
    static void KeyUp(int key);
    static void KeyPress(int key);
    static void KeyPressDelayed(int key, int delayMs);

    // 掉坑导航（各洞窟路径）
    static bool RunHole1(int x, int y, int speedX);
    static bool RunHole2(int x, int y, int speedX);
    static bool RunHole3(int x, int y, int speedX);
    static bool RunHole4(int x, int y, int speedX);
    static bool RunHole5(int x, int y, int speedX);
    static bool RunHole6(int x, int y, int speedX);

    // 通用导航
    static bool RunGeneralPos(int x, int y, int speedX);

    // 洞窟出口处理
    static bool RunHoleExit(DWORD mapid, DWORD currentstate, DWORD x, DWORD y);

    // 雁鸟渡出口
    static bool RunOutside(DWORD mapid, DWORD currentstate, DWORD x, DWORD y);

    // 清除输入状态
    static bool ClearType(BOOL intype);

    // 总入口
    static void Execute(DWORD worldIndex, DWORD x, DWORD y, DWORD speed);
};
