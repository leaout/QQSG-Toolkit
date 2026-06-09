#pragma once

// ============================================================
// UI控件结构
// 来自 qqsg_struct UIControl.cpp
// ============================================================

#include <Windows.h>
#include <string>

struct UIControl;

// 虚函数表
struct UIControlVftable {
    int pad[3];
    bool (__thiscall* IsEnabled)(UIControl*);
    int pad1[2];
    char* (__thiscall* id)(UIControl*);
    int pad2[23];
    int (__thiscall* type)(UIControl*);
    int pad6;
    int (__thiscall* style)(UIControl*);
    int pad3[13];
    UIControl* (__thiscall* GetElement)(UIControl*, int*, int, int);
    int pad4[10];
    void (__thiscall* setcaption)(UIControl*, const char*);
    char* (__thiscall* caption)(UIControl*);
    int pad5[52];
    void (__thiscall* show)(UIControl*, bool);
    int pad7[7];
    void (__thiscall* click)(UIControl*);
};

// 控件结构
struct UIControl {
    UIControlVftable* vftable;
    int pad0[8];
    UIControl* father;
    int pad1;
    UIControl** begin;
    UIControl** end;
    int pad2[3];
    char* name;
    int pad3[11];
    char* uiname;
    int pad4[73];
    int* bkimage;
    int* coverpic;
    int pad4_1[4];
    int bkimagex;
    int bkimagey;
    int coverpicx;
    int coverpicy;
    int pad5[33];
    unsigned int State;  // bit7 表示可见
    int pad6[11];
    int initlayer;
    int plusplayer;
    int prelayer;
    int pad7[62];
    int textstyle;
    int pad8;
    int textsize;
    int pad9[4];
    unsigned int Style;
    int pad10[130];
    int* name_ref;
    int pad11[5];
    UIControl* Related;
    int pad12[20];
    int luaEnv;
};

// 判断窗口是否可见（如自动切仓时需要判断私仓窗口是否展示）
// 比LUA实现性能更高，且可在非UI线程执行
inline bool WinIsVisible(UIControl* root, const char* window) {
    auto lstr = std::string(window);
    auto index = lstr.find_first_of(".");

    if (strcmp(root->name, lstr.substr(0, index).c_str()) != 0)
        return false;

    if (index > lstr.size())
        return ((root->State >> 7) & 1) != 0;

    auto size = root->end - root->begin;

    for (auto i = 0; i < size; i++) {
        auto child = lstr.substr(index + 1);
        if (WinIsVisible(root->begin[i], child.c_str()))
            return true;
    }

    return false;
}
