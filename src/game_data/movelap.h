#pragma once

// ============================================================
// 道路结构
// 来自 qqsg_struct MoveLap.md
// ============================================================

#include <Windows.h>

// 坐标
struct CoordinateInfo {
    int x;
    int y;
};

// 道路元数据
struct MoveLapMeta {
    unsigned __int16 LapId;    // 道路ID
    int Type;                  // 类型: 2=水平, 4=斜坡, 8=垂直
    float Angle;               // 角度

    CoordinateInfo StartPoint; // 起点
    CoordinateInfo EndPoint;   // 终点

    unsigned __int16 prevLapId; // 上一条接壤道路ID
    unsigned __int16 nextLapId; // 下一条接壤道路ID

    int unknown_1;
    int DynamicLapId;          // 关联的传送带编号
};

// 传送带（如十里坡的水流滑动效果）
struct MoveDynamicLapMeta {
    int pad[9];
    int add_x;                 // 每帧滑动X距离
    int add_y;                 // 每帧滑动Y距离
    int pad1[200];
};
