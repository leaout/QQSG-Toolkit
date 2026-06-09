#pragma once
// ============================================================
// 基础工具
// 来自 QQSG-Hacker base.h
// ============================================================

#include <atomic>
#include <cassert>
#include <mutex>
#include <intrin.h>  // for _mm_pause

namespace base {
    namespace sync {
        class recursive_spinlock_t {
        public:
            ~recursive_spinlock_t() { wait(); }

            void lock()   { ++event_; }
            void wait() const {
                assert(event_ >= 0);
                while (event_ != 0)
                    _mm_pause();
            }
            void unlock() { --event_; }

        private:
            std::atomic<int32_t> event_ = 0;
        };
    }
}
