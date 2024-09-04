//
// Created by kamin.deng on 2024/8/22.
//
#ifndef __OSAL_SEMAPHORE_H__
#define __OSAL_SEMAPHORE_H__

#include "osal.h"
#include "interface_semaphore.h"
#include "osal_debug.h"

namespace osal {

class OSALSemaphore : public ISemaphore {
public:
    OSALSemaphore() : semaphore_(nullptr) {
        osSemaphoreAttr_t semAttr = {};
        semAttr.name = "OSALSemaphore";
        semaphore_ = osSemaphoreNew(1, 0, &semAttr);  // 初始计数为0
        if (semaphore_ == nullptr) {
            OSAL_LOGE("Failed to create semaphore\n");
            // 处理创建失败的情况
        } else {
            OSAL_LOGD("Semaphore initialized\n");
        }
    }

    ~OSALSemaphore() override {
        if (semaphore_ != nullptr) {
            osSemaphoreDelete(semaphore_);
            OSAL_LOGD("Semaphore destroyed\n");
        }
    }

    void wait() override {
        if (osSemaphoreAcquire(semaphore_, osWaitForever) == osOK) {
            OSAL_LOGD("Semaphore wait succeeded\n");
        } else {
            OSAL_LOGE("Semaphore wait failed\n");
        }
    }

    void signal() override {
        if (osSemaphoreRelease(semaphore_) == osOK) {
            OSAL_LOGD("Semaphore signal succeeded\n");
        } else {
            OSAL_LOGE("Semaphore signal failed\n");
        }
    }

    bool tryWait() override { return tryWaitFor(0); }

    bool tryWaitFor(const uint32_t timestamp) override {
        if (osSemaphoreAcquire(semaphore_, timestamp) == osOK) {
            OSAL_LOGD("Semaphore tryWait succeeded\n");
            return true;
        }
        OSAL_LOGD("Semaphore tryWait failed\n");
        return false;
    }

    [[nodiscard]] int getValue() const override {
        int count = (int)osSemaphoreGetCount(semaphore_);
        OSAL_LOGD("Semaphore value: %d\n", count);
        return count;
    }

    void init(int initialValue) override {
        if (semaphore_ != nullptr) {
            osSemaphoreDelete(semaphore_);
        }
        osSemaphoreAttr_t semAttr = {};
        semAttr.name = "OSALSemaphore";
        semaphore_ = osSemaphoreNew(16, initialValue, &semAttr);  // 设置初始计数
        if (semaphore_ == nullptr) {
            OSAL_LOGE("Failed to initialize semaphore with value %d\n", initialValue);
            // 处理初始化失败的情况
        } else {
            OSAL_LOGD("Semaphore initialized with value %d\n", initialValue);
        }
    }

private:
    osSemaphoreId_t semaphore_;
};

}  // namespace osal

#endif  // __OSAL_SEMAPHORE_H__