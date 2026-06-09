#pragma once

// ============================================================
// 世界/地图结构
// 来自 qqsg_struct OverWorld.md
// ============================================================

#include <Windows.h>
#include "character.h"
#include "camera.h"

// 当前地图
struct CurMap {
    DWORD Unknown[0x98 / 4];  // 地图ID、特性(是否能PK)等
    GameEntity* CurCharactor;  // 当前玩家实体
};

// 游戏世界
// auto constexpr BaseAddress = 0x??????;  // 需要用CE扫描基址
struct GameWorld {
    DWORD UnknownField3[0xC / 4];
    Camera* Camera;
    DWORD UnknownFileds[(0x14 - 0xC - sizeof(Camera*)) / 4];
    CurMap* CurMap;
};

// 获取游戏世界指针（基址需自行CE扫描）
inline GameWorld* GetGameWorld(DWORD_PTR baseAddress) {
    return reinterpret_cast<GameWorld*>(baseAddress);
}
