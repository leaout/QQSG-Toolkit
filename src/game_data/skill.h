#pragma once

// ============================================================
// 技能结构
// 来自 qqsg_struct Skill.md
// ============================================================

#include <Windows.h>

struct SkillMeta;
struct SkillLvlMeta;

// 技能项虚函数表
struct ComboItemVftable {
    int pad;
    int (__thiscall* serverPos)(void* combo);
    int pad1[68];
    int (__thiscall* level)(void* combo);
};

// 技能项
struct ComboItem {
    ComboItemVftable* vftable;
    int pad[9];
    SkillMeta* Meta;
};

// 技能数值表
struct SkillMeta {
    int SkillId;
    char name[32];
    char desc[304];
    int values[6];
    int Flag;                  // 技能标志位 (见 enums.h SKILL_xxx)
    int values2[29];
    int yinzhiTime;            // 僵直时间（修改为0可无视僵直，仅客户端）
    int values3[3];
    int MixStatusId;           // 混合异常状态ID
    int values4[7];
    int LianJiCount;           // 连击数（修改可实现连击伤害增幅）
    int values5[2];
    SkillLvlMeta* LvlMeta;     // 等级元数据
    int values6[20];
};

// 技能等级元数据
struct SkillLvlMeta {
    int id;
    int maxLvl;
    int TimeAddon;             // 持续时间增益
    int pad1[178];
    int lvlMixStatuId;         // 等级混合状态ID
    int pad2[20];
};
