#pragma once
// ============================================================
// Lua 钩子工具
// 来自 qqsg_struct lua_hook.md
//
// 注意：这是一个参考实现。要使用此功能，你需要：
// 1. 获取 QQ三国客户端中的 lua_State 指针
// 2. 包含 Lua 开发头文件（lua.hpp 等）
// 3. 定义 USE_LUA 宏
// ============================================================

#include <string>
#include <Windows.h>

// 前向声明，避免直接依赖 Lua SDK
struct lua_State;

typedef const char* (__stdcall* LuaHookProvider)(std::string func);

// 字符串替换工具（不依赖 Lua，可直接使用）
std::string StringReplaceAll(std::string src, const std::string& old_value, const std::string& new_value);

#ifdef USE_LUA
// Lua 函数钩子安装
// 原理：备份原函数 → 调用 provider 生成 hook 代码 → 替换原函数
void LuaInstallHook(lua_State* L, std::string func, LuaHookProvider hook_provider);
#endif
