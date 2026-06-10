#pragma once
// ============================================================
// 基础工具
// 来自 QQSG-Hacker base.h
// ============================================================

#include <atomic>
#include <cassert>
#include <mutex>
#include <intrin.h>  // for _mm_pause
#include <processthreadsapi.h>  // for GetCurrentThreadId

namespace base {
    namespace sync {
        class recursive_spinlock_t {
        public:
            ~recursive_spinlock_t() { }

            void lock() {
                DWORD currentThread = GetCurrentThreadId();
                // 如果是同一线程递归进入，计数+1即可
                if (owner_.load(std::memory_order_relaxed) == currentThread) {
                    recursionCount_++;
                    return;
                }
                // 忙等直到锁可用
                int expected = 0;
                while (!locked_.compare_exchange_weak(expected, 1, std::memory_order_acquire)) {
                    _mm_pause();
                    expected = 0;
                }
                owner_.store(currentThread, std::memory_order_relaxed);
                recursionCount_ = 1;
            }

            void unlock() {
                // 递归解锁，减到0才真正释放
                if (--recursionCount_ == 0) {
                    owner_.store(0, std::memory_order_relaxed);
                    locked_.store(0, std::memory_order_release);
                }
            }

            // 等待锁完全空闲（兼容旧接口）
            void wait() const {
                while (locked_.load(std::memory_order_acquire) != 0) {
                    _mm_pause();
                }
            }

        private:
            std::atomic<int32_t> locked_ = 0;
            std::atomic<DWORD>   owner_ = 0;
            int recursionCount_ = 0;
        };
    }
}
