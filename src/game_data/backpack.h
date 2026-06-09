#pragma once

// ============================================================
// 背包结构
// 来自 qqsg_struct Backpack
// ============================================================

#include <Windows.h>

struct BackpackItem; // forward declaration

// 背包物品虚函数表
struct BackpackItemBehavior {
    int unknown[2];
    int (__thiscall* baseId)(BackpackItem* item);
    int unknown1[4];
    int (__thiscall* itemType)(BackpackItem* item);     // 物品类型 装备/道具
    int padding[1];
    int (__thiscall* serverPos)(BackpackItem* item);    // 服务端位置(唯一ID)
    int padding_0[4];
    char* (__thiscall* name)(BackpackItem* item);        // 道具名
    char* (__thiscall* content)(BackpackItem* item);     // 道具描述
    int (__thiscall* price)(BackpackItem* item);         // 价格
    int (__thiscall* soldPrice)(BackpackItem* item);     // 出售价
    int (__thiscall* attr)(BackpackItem* item);          // 属性(数值表H列)
    int (__thiscall* maxCount)(BackpackItem* item);      // 最大携带数量
    int (__thiscall* maxStoreCount)(BackpackItem* item); // 最大存仓数量
    int (__thiscall* sexLimit)(BackpackItem* item);      // 性别限制
    int padding1[1];
    int (__thiscall* Rare)(BackpackItem* item);          // 品质(颜色)
    int (__thiscall* target)(BackpackItem* item);        // 可使用对象
    int (__thiscall* unkown_fuck)(BackpackItem* item);
    int (__thiscall* canUseTo)(BackpackItem* item);      // 是否可使用
    int padding2[8];
    int (__thiscall* count)(BackpackItem* item);          // 当前拥有数量
    int padding3[7];
    int (__thiscall* LevelLimitVal)(BackpackItem* item); // 等级限制
    int (__thiscall* jobLimit)(BackpackItem* item);      // 职业限制
    int padding3_1[1];
    int (__thiscall* auctionsort)(BackpackItem* item);   // 寄售排序
    int (__thiscall* canMulUse)(BackpackItem* item);     // 可批量使用
    int padding4[5];
    int (__thiscall* laywrapCount)(BackpackItem* item);  // 摆摊数量
    int (__thiscall* metaValueType)(BackpackItem* item); // 数值类型
    int (__thiscall* ExtParam1)(BackpackItem* item);     // 主要数值1
    int (__thiscall* metaValue)(BackpackItem* item);     // 主要数值2
    int padding4_0[1];
    int (__thiscall* EnchaseWeizhi)(BackpackItem* item); // 强化位置
    int (__thiscall* EnchaseId)(BackpackItem* item);     // 强化ID
    int (__thiscall* EnchasePoint)(BackpackItem* item);  // 强化点数
    int (__thiscall* EnchaseWeaponLmt)(BackpackItem* item);
    int (__thiscall* Repute)(BackpackItem* item);        // 声望
    int padding4_02[1];
    int (__thiscall* HiLvlLimitVal)(BackpackItem* item);
    int (__thiscall* SoulLvLimit)(BackpackItem* item);   // 元神等级限制
    int padding4_1[2];
    int (__thiscall* timeLeft)(BackpackItem* item);      // CD剩余时间
    int padding5[2];
    int (__thiscall* attr2)(BackpackItem* item);
    int (__thiscall* fuck2)(BackpackItem* item);
    int padding6[1];
    int (__thiscall* ContentBG)(BackpackItem* item);
    int (__thiscall* ContentOrigin)(BackpackItem* item);
    int (__thiscall* ContentFunction)(BackpackItem* item);
    int (__thiscall* ContentUseWay)(BackpackItem* item);
    int padding6_0[2];
    int (__thiscall* bottleCurPoint)(BackpackItem* item); // 魔能丹属性
    int padding6_1;
    int (__thiscall* bottleState)(BackpackItem* item);
    int padding7[1];
    int (__thiscall* picCode)(BackpackItem* item);        // 物品图片
    int padding8[402];
    int (__thiscall* attr3)(BackpackItem* item);
    int padding9[24];
    int (__thiscall* MasterLvl)(BackpackItem* item);     // 玩家等级(过时)
};

// 背包项
struct BackpackItem {
    BackpackItemBehavior* Method;
    // 其他字段（如摆摊数量 laycount）
};

// 背包信息
// auto backpack = (Backpack*)(gameworld + 0x34);
struct Backpack {
    int pad[0xC / 4];
    int maxValue;
    int pad2[0xC / 4];
    BackpackItem* Items[165];  // 约165格
};
