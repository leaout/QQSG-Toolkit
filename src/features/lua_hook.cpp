#include "lua_hook.h"

std::string StringReplaceAll(std::string src, const std::string& old_value, const std::string& new_value) {
    for (std::string::size_type pos(0); pos != std::string::npos; pos += new_value.length()) {
        if ((pos = src.find(old_value, pos)) != std::string::npos) {
            src.replace(pos, old_value.length(), new_value);
        } else break;
    }
    return src;
}

#ifdef USE_LUA
// 需要 Lua 开发头文件支持
#include <lua.hpp>

void LuaInstallHook(lua_State* L, std::string func, LuaHookProvider hook_provider) {
    auto temp = "hook_" + StringReplaceAll(func, ".", "_");
    auto declare = temp + "=" + func;
    luaL_dostring(L, declare.c_str());

    auto hook = hook_provider(temp);
    auto installHook = func + "=" + hook;
    luaL_dostring(L, installHook.c_str());
}
#endif
