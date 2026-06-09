#pragma once
// ============================================================
// 特征码扫描
// 修改自 QQSG-Hacker pattern.h (learn_more 版)
// ============================================================

#include <cstdio>
#include <cstdint>
#include <Windows.h>
#include <DbgHelp.h>
#include <Psapi.h>

#pragma comment(lib, "dbghelp.lib")
#pragma comment(lib, "psapi.lib")

#define INRANGE(x, a, b) (x >= a && x <= b)
#define getBits(x) (INRANGE((x & (~0x20)), 'A', 'F') ? ((x & (~0x20)) - 'A' + 0xA) : (INRANGE(x, '0', '9') ? x - '0' : 0))
#define getByte(x) (getBits(x[0]) << 4 | getBits(x[1]))

// 在指定内存范围搜索特征码
inline uintptr_t FindPattern(const uintptr_t& start_address, const uintptr_t& end_address, const char* target_pattern) {
    const char* pattern = target_pattern;
    uintptr_t first_match = 0;

    for (uintptr_t position = start_address; position < end_address; position++) {
        if (!*pattern)
            return first_match;

        const uint8_t pattern_current = *reinterpret_cast<const uint8_t*>(pattern);
        const uint8_t memory_current = *reinterpret_cast<const uint8_t*>(position);

        if (pattern_current == '\?' || memory_current == getByte(pattern)) {
            if (!first_match)
                first_match = position;

            if (!pattern[2])
                return first_match;

            pattern += pattern_current != '\?' ? 3 : 2;
        } else {
            pattern = target_pattern;
            first_match = 0;
        }
    }

    return NULL;
}

// 在指定模块搜索特征码
inline uintptr_t FindPattern(const char* module, const char* target_pattern) {
    MODULEINFO module_info = { 0 };
    if (!GetModuleInformation(GetCurrentProcess(), GetModuleHandleA(module), &module_info, sizeof(MODULEINFO)))
        return NULL;

    const uintptr_t start_address = uintptr_t(module_info.lpBaseOfDll);
    const uintptr_t end_address = start_address + module_info.SizeOfImage;
    return FindPattern(start_address, end_address, target_pattern);
}

// 从基址+DLL大小搜索特征码
inline uintptr_t FindPattern(PBYTE lpBase, uintptr_t SizeOfImage, const char* target_pattern) {
    const uintptr_t start_address = uintptr_t(lpBase);
    const uintptr_t end_address = start_address + SizeOfImage;
    return FindPattern(start_address, end_address, target_pattern);
}
