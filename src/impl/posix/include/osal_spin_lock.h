//
// Created by kamin.deng on 2024/8/23.
//
#ifndef __OSAL_SPINLOCK_H__
#define __OSAL_SPINLOCK_H__

#include <atomic>
#include <chrono>

#include "interface_spin_lock.h"
#include "osal_debug.h"

namespace osal {

class OSALSpinLock : public ISpinLock {
public:
    OSALSpinLock() : flag_{} {}

    ~OSALSpinLock() = default;

    void lock() override {
        while (flag_.test_and_set(std::memory_order_acquire)) {
            // 自旋等待
        }
        OSAL_LOGD("Lock acquired\n");
    }

    bool tryLock() override {
        bool result = !flag_.test_and_set(std::memory_order_acquire);
        OSAL_LOGD("Try lock %s\n", result ? "succeeded" : "failed");
        return result;
    }

    bool lockFor(uint32_t timeout) override {
        auto start = std::chrono::steady_clock::now();
        std::chrono::milliseconds timeout_duration(timeout);

        while (flag_.test_and_set(std::memory_order_acquire)) {
            if (std::chrono::steady_clock::now() - start >= timeout_duration) {
                OSAL_LOGD("Lock with timeout failed\n");
                return false;
            }
            // 自旋等待
        }
        OSAL_LOGD("Lock with timeout succeeded\n");
        return true;
    }

    void unlock() override {
        flag_.clear(std::memory_order_release);
        OSAL_LOGD("Lock released\n");
    }

    bool isLocked() const override {
        bool result = flag_.test(std::memory_order_relaxed);
        OSAL_LOGD("Requested lock status: %s\n", result ? "locked" : "unlocked");
        return result;
    }

private:
    std::atomic_flag flag_ = ATOMIC_FLAG_INIT;
};

}  // namespace osal

#endif  // __OSAL_SPINLOCK_H__