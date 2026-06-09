#pragma once

// ============================================================
// QQ三国 枚举定义
// 来自 qqsg_struct CtrlState.md
// ============================================================

// ---------- CtrlState4 (低位) ----------
#define CTRL4_DISABLE_ITEM        0x2       // 禁止使用道具
#define CTRL4_DISABLE_PICKUP      0x4       // 禁止拾取
#define CTRL4_SOUL_STATE          0x8       // 灵魂状态/剑状态
#define CTRL4_HEAL_MARK           0x10      // 治疗增益/减益标志
#define CTRL4_GREEN_PLUM          0x20      // 获得青梅状态
#define CTRL4_INVISIBLE_TARGET    0x40      // 隐身（主体）
#define CTRL4_INVISIBLE_RELATED   0x80      // 隐身（关联）
#define CTRL4_NO_SUMMON_CHILD     0x200     // 不可召唤子女
#define CTRL4_RED_NAME            0x800     // 红名状态

// ---------- CtrlState ----------
#define CTRL_POSSESSED            0x2       // 被附体（元神）
#define CTRL_STICKY               0x4       // 粽子状态（方向键混乱）
#define CTRL_FAST_CHARGE          0x20      // 速战/减蓄气
#define CTRL_STUN                 0x40      // 眩晕/石化
#define CTRL_BLIND                0x80      // 失明
#define CTRL_FEAR                 0x100     // 恐惧
#define CTRL_NO_SPEEDUP           0x200     // 禁止加速
#define CTRL_ANTI_ADDICTION       0x800     // 防沉迷
#define CTRL_ZHISHEN              0x1000    // 置身
#define CTRL_NO_CLIMB             0x2000    // 禁止攀爬
#define CTRL_DISARM               0x4000    // 缴械
#define CTRL_REVERSED             0x8000    // 左右相反
#define CTRL_POSSESS_ATTR_CHANGE  0x20000   // 附体属性改变
#define CTRL_TARGETED_HEAL        0x40000   // 指定加血
#define CTRL_NO_POSSESS           0x80000   // 不可附体
#define CTRL_UNSELECTABLE         0x100000  // 不可被选中
#define CTRL_INVINCIBLE           0x200000  // 无敌
#define CTRL_NO_ATTACK            0x400000  // 不可被攻击
#define CTRL_KNOCKUP              0x800000  // 击飞
#define CTRL_CHANGE_OUTFIT        0x1000000 // 换装
#define CTRL_LOW_MOBILITY         0x2000000 // 行动力降低
#define CTRL_DOT                  0x4000000 // 持续掉血
#define CTRL_ACTIVITY_ABNORMAL    0x8000000 // 活动异常
#define CTRL_JS_HJ_110            0x20000000 // JS/HJ 110技能
#define CTRL_PHYSICAL_110         0x40000000 // 物理系110技能

// ---------- LifeState ----------
enum LifeState {
    LIFE_NORMAL   = 1,   // 普通
    LIFE_FIGHT    = 2,   // 战斗
    LIFE_DEAD     = 8,   // 死亡
    LIFE_EXCHANGE = 16,  // 交易/交换
    LIFE_DREAM    = 32,  // 梦境
    LIFE_ERROR    = 64,
};

// ---------- FightState ----------
enum FightState {
    FIGHT_INVALID = 0,  // 无效
    FIGHT_CALM    = 1,  // 未进行战斗
    FIGHT_GATHER  = 2,  // 蓄气中
    FIGHT_COMBAT  = 3,  // 进行中
    FIGHT_FREEZE  = 4,  // 硬直中
    FIGHT_INJURE  = 5,  // 受伤中
    FIGHT_STATUS  = 6,  // 异常状态中
    FIGHT_DEAD    = 7,  // 死亡
};

// ---------- 移动方向 ----------
enum MoveDirection {
    MOVE_RIGHT = 0,
    MOVE_LEFT  = 1,
    MOVE_UP    = 2,
    MOVE_DOWN  = 3,
};

// ---------- 移动状态 ----------
enum MoveState {
    MOVE_HORIZONTAL_STILL = 2,
    MOVE_HORIZONTAL_MOVE  = 4,
    MOVE_VERTICAL_MOVE    = 8,
    MOVE_JUMPING          = 16,
    MOVE_VERTICAL_STILL   = 32,
};

// ---------- 按键标志 ----------
enum PressedKey {
    KEY_LEFT  = 4,
    KEY_RIGHT = 2,
    KEY_UP    = 8,
    KEY_DOWN  = 16,
};

// ---------- 实体类型 ----------
enum EntityType {
    ENTITY_PLAYER        = 1,
    ENTITY_MONSTER       = 2,
    ENTITY_NPC           = 3,
    ENTITY_SPIRIT        = 4,
    ENTITY_ITEM          = 5,
    ENTITY_ACCOUNT       = 6,
    ENTITY_SKILL         = 7,
    ENTITY_EQUIP         = 8,
    ENTITY_TEAM          = 9,
    ENTITY_SHOWITEM      = 10,
    ENTITY_DROP          = 11,
    ENTITY_STALL         = 12,
    ENTITY_SPAWN         = 13,
    ENTITY_ACTIVITY_AREA = 14,
    ENTITY_SOUL_BAG      = 21,
    ENTITY_BUILDING      = 26,
    ENTITY_LINGPO        = 63,
    ENTITY_AOYI          = 67,
    ENTITY_BEAST         = 77,
    ENTITY_INNER_PILL    = 78,
};

// ---------- 技能标志 ----------
#define SKILL_PHYSICAL_DAMAGE   0x1       // 物理伤害
#define SKILL_MAGIC_DAMAGE      0x2       // 魔法伤害
#define SKILL_HEAL              0x4       // 治疗
#define SKILL_TAUNT             0x8       // 挑衅
#define SKILL_FEAR              0x10      // 恐惧
#define SKILL_BUFF              0x20      // 增益
#define SKILL_DEBUFF            0x40      // 减益
#define SKILL_MONSTER_CONTROL   0x1000    // 怪物控制
#define SKILL_TELEPORT          0x4000    // 瞬移
#define SKILL_MASK_ANIM         0x8000    // 遮罩动画
#define SKILL_FLY_ANIM          0x10000   // 飞行
#define SKILL_CHARGE            0x100000  // 冲锋
#define SKILL_CHARGE_WEAK       0x800000  // 小冲锋
#define SKILL_SPECIAL_ANIM      0x4000000 // 特殊动画
