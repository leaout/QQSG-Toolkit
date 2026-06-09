#pragma once
// ============================================================
// 内存读写工具
// ============================================================

#include <Windows.h>
#include <cstdint>
#include <string>
#include <vector>

namespace Memory {

    // 读取指定类型的值
    template<typename T>
    inline T Read(uintptr_t address) {
        if (!address) return T{};
        return *reinterpret_cast<T*>(address);
    }

    // 写入指定类型的值
    template<typename T>
    inline void Write(uintptr_t address, T value) {
        if (!address) return;
        DWORD oldProtect;
        VirtualProtect(reinterpret_cast<void*>(address), sizeof(T), PAGE_EXECUTE_READWRITE, &oldProtect);
        *reinterpret_cast<T*>(address) = value;
        VirtualProtect(reinterpret_cast<void*>(address), sizeof(T), oldProtect, &oldProtect);
    }

    // 读取指针链
    inline uintptr_t ReadPointerChain(uintptr_t baseAddress, const std::vector<uintptr_t>& offsets) {
        uintptr_t addr = baseAddress;
        for (size_t i = 0; i < offsets.size(); i++) {
            if (!addr) return 0;
            addr = *reinterpret_cast<uintptr_t*>(addr);
            if (i < offsets.size() - 1) {
                addr += offsets[i];
            }
        }
        return addr;
    }

    // 读取字节数组
    inline bool ReadBytes(uintptr_t address, void* buffer, size_t size) {
        if (!address || !buffer || !size) return false;
        memcpy(buffer, reinterpret_cast<void*>(address), size);
        return true;
    }

    // 写入字节数组
    inline bool WriteBytes(uintptr_t address, const void* buffer, size_t size) {
        if (!address || !buffer || !size) return false;
        DWORD oldProtect;
        VirtualProtect(reinterpret_cast<void*>(address), size, PAGE_EXECUTE_READWRITE, &oldProtect);
        memcpy(reinterpret_cast<void*>(address), buffer, size);
        VirtualProtect(reinterpret_cast<void*>(address), size, oldProtect, &oldProtect);
        return true;
    }

    // 初始化（设置基址）
    inline uintptr_t BaseAddress = 0;

    // 模块基址
    inline uintptr_t GetModuleBase(const wchar_t* moduleName) {
        return reinterpret_cast<uintptr_t>(GetModuleHandleW(moduleName));
    }
}
