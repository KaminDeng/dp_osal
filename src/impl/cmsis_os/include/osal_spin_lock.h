//
// Created by kamin.deng on 2024/8/22.
//
#ifndef __OSAL_SPINLOCK_H__
#define __OSAL_SPINLOCK_H__

#include "cmsis_os.h"
#include "interface_spin_lock.h"
#include "osal_debug.h"

namespace osal {

class OSALSpinLock : public ISpinLock {
public:
    OSALSpinLock() {
        osMutexAttr_t mutexAttr = {};
        mutexAttr.name = "OSALSpinLock";
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
            OSAL_LOGD("Lock acquired\n");
        } else {
            OSAL_LOGE("Lock acquisition failed\n");
        }
    }

    bool tryLock() override {
        if (osMutexAcquire(mutex_, 0) == osOK) {
            OSAL_LOGD("Try lock succeeded\n");
            return true;
        }
        OSAL_LOGD("Try lock failed\n");
        return false;
    }

    bool lockFor(uint32_t timeout) override {
        if (osMutexAcquire(mutex_, timeout) == osOK) {
            OSAL_LOGD("Lock with timeout succeeded\n");
            return true;
        }
        OSAL_LOGD("Lock with timeout failed\n");
        return false;
    }

    void unlock() override {
        if (osMutexRelease(mutex_) == osOK) {
            OSAL_LOGD("Lock released\n");
        } else {
            OSAL_LOGE("Lock release failed\n");
        }
    }

    [[nodiscard]] bool isLocked() const override {
        // CMSIS-RTOS2 并没有直接提供查询互斥锁状态的接口
        // 这里可以通过尝试获取锁来间接判断锁的状态
        bool result = (osMutexAcquire(mutex_, 0) != osOK);
        if (!result) {
            osMutexRelease(mutex_);
        }
        OSAL_LOGD("Requested lock status: %s\n", result ? "locked" : "unlocked");
        return result;
    }

private:
    osMutexId_t mutex_;
};

}  // namespace osal

#endif  // __OSAL_SPINLOCK_H__