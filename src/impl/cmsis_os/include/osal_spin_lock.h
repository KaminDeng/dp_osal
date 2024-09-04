//
// Created by kamin.deng on 2024/8/22.
//
#ifndef __OSAL_SPINLOCK_H__
#define __OSAL_SPINLOCK_H__

#include "osal.h"
#include "interface_spin_lock.h"
#include "osal_debug.h"
#include <atomic>

namespace osal {

class OSALSpinLock : public ISpinLock {
public:
    OSALSpinLock() : lockCount(0) {
        osMutexAttr_t mutexAttr = {};
        mutexAttr.name = "OSALSpinLock";
        mutexAttr.attr_bits = osMutexRecursive | osMutexPrioInherit;
        mutex_ = osMutexNew(&mutexAttr);
        if (mutex_ == nullptr) {
            OSAL_LOGE("Failed to create mutex\n");
            // 处理创建失败的情况
        } else {
            OSAL_LOGD("SpinLock initialized\n");
        }
    }

    ~OSALSpinLock() override {
        if (mutex_ != nullptr) {
            osMutexDelete(mutex_);
            OSAL_LOGD("SpinLock destroyed\n");
        }
    }

    void lock() override {
        if (osMutexAcquire(mutex_, osWaitForever) == osOK) {
            lockCount++;
            OSAL_LOGD("Lock acquired\n");
        } else {
            OSAL_LOGE("Lock acquisition failed\n");
        }
    }

    bool tryLock() override {
        if (osMutexAcquire(mutex_, 0) == osOK) {
            lockCount++;
            OSAL_LOGD("Try lock succeeded\n");
            return true;
        }
        OSAL_LOGD("Try lock failed\n");
        return false;
    }

    bool lockFor(uint32_t timeout) override {
        if (osMutexAcquire(mutex_, timeout) == osOK) {
            lockCount++;
            OSAL_LOGD("Lock with timeout succeeded\n");
            return true;
        }
        OSAL_LOGD("Lock with timeout failed\n");
        return false;
    }

    void unlock() override {
        if (osMutexRelease(mutex_) == osOK) {
            lockCount--;
            OSAL_LOGD("Lock released\n");
        } else {
            OSAL_LOGE("Lock release failed\n");
        }
    }

    [[nodiscard]] bool isLocked() const override {
        bool result = (lockCount > 0);
        OSAL_LOGD("Requested lock status: %s\n", result ? "locked" : "unlocked");
        return result;
    }

private:
    osMutexId_t mutex_;
    mutable std::atomic<int> lockCount;  // 使用原子计数器来跟踪锁的状态
};

}  // namespace osal

#endif  // __OSAL_SPINLOCK_H__