#pragma once
#include "offsets.h"
#include "status.h"

// ============================================================
// 角色/实体结构
// 来自 qqsg_struct Charactor.md
// ============================================================

#pragma pack(push, 1)

// 速度结构
// playerinfo + 0x304 (X轴)
// playerinfo + 0x330 (Y轴)
struct Speed {
    DWORD Unknown;
    DWORD SpeedVal;      // 速度值
    DWORD Unknown2;
    DWORD SpeedVector1;  // 速度向量
    DWORD SpeedVector2;
    DWORD SpeedVector3;
    DWORD SpeedVector4;
    DWORD UnknownFields[0x2C / 4];
};

// 游戏实体（玩家、怪物、NPC等）
// 包含了当前实体的坐标、名字、职业、国家、五铢、属性等
struct GameEntity {
    DWORD UnknownBlock[0x304 / 4];
    Speed* SpeedX;                           // X轴移速
    Speed* SpeedY;                           // Y轴跳跃高度
    Speed* SpeedUnknown;                     // Y轴移速(固定2000)
    DWORD UnknownBlock2[(0x8658 - 0x304 - sizeof(Speed*) * 3) / 4];
    PlayerStatusNode* Root;                  // 状态树根节点
    PlayerStatusNode* TopRoot;               // 最高根节点(Root==TopRoot则无状态)
    DWORD Count;                             // 状态节点数量
};

#pragma pack(pop)

// ---------- 装备工具函数 ----------
namespace Equipment {

    inline DWORD_PTR GetEquipmentSlot(DWORD_PTR localPlayer, DWORD slotOffset) {
        if (!localPlayer) return 0;
        DWORD_PTR equipPtr = *reinterpret_cast<DWORD_PTR*>(localPlayer + EQUIPMENT1);
        if (!equipPtr) return 0;
        return *reinterpret_cast<DWORD_PTR*>(equipPtr + slotOffset);
    }

    inline DWORD GetCurDurability(DWORD_PTR equipAddr) {
        if (!equipAddr) return (DWORD)-1;
        return *reinterpret_cast<DWORD*>(equipAddr + EQUIPMENTCUR);
    }

    inline DWORD GetMaxDurability(DWORD_PTR equipAddr) {
        if (!equipAddr) return (DWORD)-1;
        return *reinterpret_cast<DWORD*>(equipAddr + EQUIPMENTMAX);
    }

    // 获取各部位耐久
    inline DWORD GetHeadCur()  { DWORD_PTR e = GetEquipmentSlot(LOCALPLAYER, HAT);   return GetCurDurability(e); }
    inline DWORD GetHeadMax()  { DWORD_PTR e = GetEquipmentSlot(LOCALPLAYER, HAT);   return GetMaxDurability(e); }
    inline DWORD GetGlassCur() { DWORD_PTR e = GetEquipmentSlot(LOCALPLAYER, GLASS); return GetCurDurability(e); }
    inline DWORD GetGlassMax() { DWORD_PTR e = GetEquipmentSlot(LOCALPLAYER, GLASS); return GetMaxDurability(e); }
    inline DWORD GetNeckCur()  { DWORD_PTR e = GetEquipmentSlot(LOCALPLAYER, NECK);  return GetCurDurability(e); }
    inline DWORD GetNeckMax()  { DWORD_PTR e = GetEquipmentSlot(LOCALPLAYER, NECK);  return GetMaxDurability(e); }
    inline DWORD GetWingCur()  { DWORD_PTR e = GetEquipmentSlot(LOCALPLAYER, WING);  return GetCurDurability(e); }
    inline DWORD GetWingMax()  { DWORD_PTR e = GetEquipmentSlot(LOCALPLAYER, WING);  return GetMaxDurability(e); }
    inline DWORD GetUpperCur() { DWORD_PTR e = GetEquipmentSlot(LOCALPLAYER, UPPER); return GetCurDurability(e); }
    inline DWORD GetUpperMax() { DWORD_PTR e = GetEquipmentSlot(LOCALPLAYER, UPPER); return GetMaxDurability(e); }
    inline DWORD GetLowerCur() { DWORD_PTR e = GetEquipmentSlot(LOCALPLAYER, LOWER); return GetCurDurability(e); }
    inline DWORD GetLowerMax() { DWORD_PTR e = GetEquipmentSlot(LOCALPLAYER, LOWER); return GetMaxDurability(e); }
    inline DWORD GetArmCur()   { DWORD_PTR e = GetEquipmentSlot(LOCALPLAYER, ARM);   return GetCurDurability(e); }
    inline DWORD GetArmMax()   { DWORD_PTR e = GetEquipmentSlot(LOCALPLAYER, ARM);   return GetMaxDurability(e); }
    inline DWORD GetShoeCur()  { DWORD_PTR e = GetEquipmentSlot(LOCALPLAYER, SHOE);  return GetCurDurability(e); }
    inline DWORD GetShoeMax()  { DWORD_PTR e = GetEquipmentSlot(LOCALPLAYER, SHOE);  return GetMaxDurability(e); }
    inline DWORD GetWeaponCur() { DWORD_PTR e = GetEquipmentSlot(LOCALPLAYER, RIGHTWEAPON); return GetCurDurability(e); }
    inline DWORD GetWeaponMax() { DWORD_PTR e = GetEquipmentSlot(LOCALPLAYER, RIGHTWEAPON); return GetMaxDurability(e); }
    inline DWORD GetRingCur()  { DWORD_PTR e = GetEquipmentSlot(LOCALPLAYER, RING);    return GetCurDurability(e); }
    inline DWORD GetRingMax()  { DWORD_PTR e = GetEquipmentSlot(LOCALPLAYER, RING);    return GetMaxDurability(e); }
    inline DWORD GetFuZhouCur() { DWORD_PTR e = GetEquipmentSlot(LOCALPLAYER, FUZHOU); return GetCurDurability(e); }
    inline DWORD GetFuZhouMax() { DWORD_PTR e = GetEquipmentSlot(LOCALPLAYER, FUZHOU); return GetMaxDurability(e); }
}

// ---------- 世界/坐标工具函数 ----------
namespace Overworld {

    inline DWORD_PTR GetLocalPlayer() {
        return *reinterpret_cast<DWORD_PTR*>(LOCALPLAYER);
    }

    inline DWORD_PTR GetCurrentMapPtr() {
        DWORD_PTR lp = GetLocalPlayer();
        if (!lp) return 0;
        DWORD_PTR mapPtr = *reinterpret_cast<DWORD_PTR*>(lp + MAP);
        if (!mapPtr) return 0;
        return *reinterpret_cast<DWORD_PTR*>(mapPtr + CURRENTMAP);
    }

    inline DWORD GetMapIndex() {
        DWORD_PTR mp = GetCurrentMapPtr();
        if (!mp) return (DWORD)-1;
        return *reinterpret_cast<DWORD*>(mp + CURRENTMAPID);
    }

    inline DWORD GetPosX() {
        DWORD_PTR mp = GetCurrentMapPtr();
        if (!mp) return (DWORD)-1;
        return *reinterpret_cast<DWORD*>(mp + XPOS);
    }

    inline DWORD GetPosY() {
        DWORD_PTR mp = GetCurrentMapPtr();
        if (!mp) return (DWORD)-1;
        return *reinterpret_cast<DWORD*>(mp + YPOS);
    }

    inline DWORD GetSpeedHor() {
        DWORD_PTR mp = GetCurrentMapPtr();
        if (!mp) return (DWORD)-1;
        return *reinterpret_cast<DWORD*>(mp + RUN);
    }

    inline DWORD GetConversationState() {
        DWORD_PTR lp = GetLocalPlayer();
        if (!lp) return 0;
        return *reinterpret_cast<DWORD*>(*reinterpret_cast<DWORD_PTR*>(lp + CONVERSATIONSTATE1) + CONVERSATIONSTATE2);
    }
}

// ---------- 社交状态 ----------
namespace Social {
    inline BYTE GetTypingState() {
        return *reinterpret_cast<BYTE*>(INTYPE);
    }
}
