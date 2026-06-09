#pragma once

// ============================================================
// QQ三国 偏移量定义
// 整合自 QQSG-Hacker 和 qqsg_struct
// ============================================================

// ---------- 基址（需用 CE 更新） ----------
#define LOCALPLAYER      0x10F9134  // 玩家对象基址
#define INTYPE           0x10FB3DC  // 输入状态地址

// ============================================================
// 装备偏移
// LOCALPLAYER + EQUIPMENT1 + 部位 + EQUIPMENTCUR/MAX
// ============================================================
#define EQUIPMENT1        0x38

#define HAT               0x348   // 头盔
#define GLASS             0x34C   // 眼镜
#define NECK              0x350   // 项链
#define WING              0x354   // 翅膀
#define UPPER             0x358   // 上装
#define LOWER             0x35C   // 下装
#define ARM               0x360   // 护腕
#define SHOE              0x364   // 鞋子
#define LEFTWEAPON        0x368   // 左武器
#define RIGHTWEAPON       0x36C   // 右武器
#define RING              0x370   // 戒指
#define FUZHOU            0x374   // 符咒

#define EQUIPMENTCUR      0x28    // 当前耐久
#define EQUIPMENTMAX      0x2C    // 最大耐久

// ============================================================
// 世界/地图偏移
// LOCALPLAYER + MAP + CURRENTMAP + 字段
// ============================================================
#define MAP               0x0C
#define CURRENTMAP        0x334
#define CURRENTSTATE      0x338

#define CURRENTMAPID      0x18C   // 当前地图ID
#define XPOS              0x54    // X坐标
#define YPOS              0x58    // Y坐标
#define RUN               0x354   // 水平速度

// ============================================================
// 对话状态
// ============================================================
#define CONVERSATIONSTATE1  0x20
#define CONVERSATIONSTATE2  0x0

// ============================================================
// 地图ID常量
// ============================================================
#define HOLE6             456
#define HOLE3             457
#define HOLE2             458
#define HOLE5             459
#define HOLE4             460
#define HOLE1             461
#define HOLEEND           462
#define ENTRYYANNIAODU    28

// ============================================================
// 角色结构偏移（来自 qqsg_struct）
// playerinfo + 0x304  = 速度结构X
// playerinfo + 0x330  = 速度结构Y
// playerinfo + 0x8658 = 状态树根节点
// ============================================================
#define PLAYER_SPEED_X         0x304
#define PLAYER_SPEED_Y         0x330
#define PLAYER_SPEED_UNKNOWN   0x35C
#define PLAYER_STATUS_ROOT     0x8658

// 背包地址 = GameWorld + 0x34
#define BACKPACK_OFFSET        0x34
