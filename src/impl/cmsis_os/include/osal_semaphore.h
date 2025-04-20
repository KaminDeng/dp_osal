/*
 * Copyright (c) 2024 kamin.deng
 * Email: kamin.deng@gmail.com
 * Created on 2024/8/22.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

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