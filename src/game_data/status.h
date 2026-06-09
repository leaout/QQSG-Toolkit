#pragma once

// ============================================================
// 实体状态树结构
// 来自 qqsg_struct status.md
// ============================================================

#include <Windows.h>

// 状态树节点（二叉树）
template<typename T>
struct GameTreeNode {
    GameTreeNode<T>* LeftChild;
    GameTreeNode<T>* Father;
    GameTreeNode<T>* RightChild;
    int ValueId;
    T* Value;
};

// 状态树
template<typename T>
struct GameTree {
    int unknown_0;
    GameTreeNode<T>* Summary;
    int unknown_1;
    int Count;
};

// 状态元数据（数值表）
struct StatusMetadata {
    DWORD TotalTime;
    DWORD unknown_1;
    DWORD unknown_2;
    DWORD unknown_3;
    DWORD RecordTimeFlag;
    DWORD unknown_5;
    DWORD unknown_6;
    DWORD unknown_7;
    DWORD unknown_8;
    char Desc[32];
    DWORD unknown_17;
    DWORD unknown_18;
    DWORD unknown_19;
    DWORD unknown_20;
    DWORD unknown_21;
    DWORD unknown_22;
    DWORD unknown_23;
    DWORD unknown_24;
    DWORD unknown_25;
    DWORD unknown_26;
    DWORD unknown_27;
    DWORD unknown_28;
    DWORD unknown_29;
    DWORD unknown_30;
    DWORD unknown_31;
    DWORD unknown_32;
    DWORD unknown_33;
    DWORD unknown_34;
    DWORD PicCode;           // 图标
    DWORD unknown_36;
    DWORD LeftTime;          // 剩余时间
    DWORD Type;              // 状态类型
    DWORD Values[15];        // 状态数值
    DWORD SomeFlag;
    DWORD Effect;
    DWORD MaxImposeTimes;
    DWORD Todo[4];
    DWORD Todo1;
    DWORD Todo2[30];
};

// 状态实例
struct Status {
    DWORD CurrentTime;
    StatusMetadata* Metadata;
    DWORD unknown_0;
    DWORD unknown_1;
    DWORD unknown_2;
    DWORD CurrentTime2;
    DWORD unknown_4;
    DWORD unknown_5;
    DWORD unknown_6;
    DWORD ShowType;
};

// 类型别名
typedef GameTree<Status>   StatusTree;
typedef GameTreeNode<Status> StatusTreeNode;
typedef StatusMetadata*    PStatusMetadata;
typedef Status*            PStatus;

// 玩家状态树节点（在 GameEntity 中使用）
typedef GameTreeNode<Status> PlayerStatusNode;
typedef PlayerStatusNode* PPlayerStatusNode;
