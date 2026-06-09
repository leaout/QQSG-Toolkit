#pragma once
// ============================================================
// QQ三国 内置寻路系统 - 接口模块
//
// 目标：直接调用游戏自身的寻路函数，实现任意地图任意点自动行走
// 不再依赖 if-else 坐标查表
// ============================================================

#include <Windows.h>
#include <cstdint>
#include <cstdio>

// -------------------------------------------------------
// 方式一：直接调用游戏寻路函数
//
// 原理：游戏内部有一个 Client_SendMoveTo(x, y, mapId) 函数，
//       在点击小地图/任务自动寻路时被调用。
//       通过特征码扫描找到该函数，直接调用即可。
// -------------------------------------------------------

// 寻路函数指针类型（根据逆向结果填写）
// 常见调用约定：thiscall, stdcall, fastcall
// 参数通常为：目标X, 目标Y, 地图ID
typedef int(__thiscall* PathFindTo_t)(void* worldPtr, int targetX, int targetY, int mapId);

// 寻路函数地址（由 FindPathFindFunction 填充）
extern PathFindTo_t g_PathFindTo;

// 特征码搜索寻路函数
// 需要先用 CE/IDA 定位函数，提取特征码填入此处
bool FindPathFindFunction();

// 调用寻路（封装）
bool MoveTo(int targetX, int targetY, int mapId);

// -------------------------------------------------------
// 方式二：注入寻路目标坐标（更稳定，不依赖函数地址）
//
// 原理：游戏内存中保存了"自动寻路目标坐标"，
//       直接改写这个坐标，再调用游戏处理寻路的函数。
// -------------------------------------------------------

// 寻路目标坐标偏移（需用 CE 扫描）
// 找到方式：CE 搜索当前自动寻路目标点的坐标写操作
#define PATH_TARGET_X_OFFSET 0x??  // 请用 CE 扫描填充
#define PATH_TARGET_Y_OFFSET 0x??  // 请用 CE 扫描填充
#define PATH_TARGET_MAP_OFFSET 0x?? // 请用 CE 扫描填充

// 触发寻路的虚函数表索引
// EndScene 钩子索引: 42
// Reset 钩子索引: 16
// 寻路处理函数可能在虚表某个位置
#define PATHFIND_VTABLE_INDEX 0 // 需逆向确定

// 注入目标坐标
void SetPathTarget(int x, int y, int mapId);

// 触发寻路（让游戏开始沿着路径走）
void TriggerPathfinding();

// -------------------------------------------------------
// 方式三：发送坐标封包（最稳定，走服务端校验）
//
// 原理：游戏移动由服务端控制，直接 Send 移动封包可以让
//       服务器驱动客户端角色移动到目标点。
//       需要抓包分析移动协议格式。
// -------------------------------------------------------

// 发送移动到指定坐标的封包
bool SendMovePacket(int targetX, int targetY);

// 辅助：在屏幕上显示当前位置给 CE 搜索用
void DebugShowCoords();
