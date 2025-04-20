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

#ifndef __OSAL_CONDITION_VARIABLE_H__
#define __OSAL_CONDITION_VARIABLE_H__

#include "osal.h"
#include "interface_condition_variable.h"
#include "osal_debug.h"
#include "osal_mutex.h"

namespace osal {

class OSALConditionVariable : public IConditionVariable {
public:
    OSALConditionVariable() : waitCount(0) {
        cond_ = osSemaphoreNew(16, 0, nullptr);
        if (cond_ == nullptr) {
            OSAL_LOGE("Failed to initialize condition variable\n");
        } else {
            OSAL_LOGD("Condition variable initialized\n");
        }
    }

    ~OSALConditionVariable() override {
        if (osSemaphoreDelete(cond_) != osOK) {
            OSAL_LOGE("Failed to destroy condition variable\n");
        } else {
            OSAL_LOGD("Condition variable destroyed\n");
        }
    }

    void wait(OSALMutex &mutex) override {
        waitCount++;
        mutex.unlock();  // Release the mutex while waiting
        if (osSemaphoreAcquire(cond_, osWaitForever) != osOK) {
            OSAL_LOGE("Failed to wait on condition variable\n");
        } else {
            OSAL_LOGD("Condition variable wait succeeded\n");
        }
        mutex.lock();  // Reacquire the mutex after waiting
        waitCount--;
    }

    bool waitFor(OSALMutex &mutex, uint32_t timeout) override {
        waitCount++;
        mutex.unlock();  // Release the mutex while waiting

        osStatus_t result = osSemaphoreAcquire(cond_, timeout);
        waitCount--;

        mutex.lock();  // Reacquire the mutex after waiting

        if (result == osOK) {
            OSAL_LOGD("Condition variable waitFor succeeded\n");
            return true;
        } else if (result == osErrorTimeout) {
            OSAL_LOGD("Condition variable waitFor timed out\n");
            return false;
        } else {
            OSAL_LOGE("Failed to wait on condition variable with timeout\n");
            return false;
        }
    }

    void notifyOne() override {
        if (osSemaphoreRelease(cond_) != osOK) {
            OSAL_LOGE("Failed to notify one on condition variable\n");
        } else {
            OSAL_LOGD("Condition variable notifyOne succeeded\n");
        }
    }

    void notifyAll() override {
        // In CMSIS-RTOS2, notifying all would require releasing the semaphore as many times as there are waiting
        // threads.
        for (int i = 0; i < waitCount; ++i) {
            if (osSemaphoreRelease(cond_) != osOK) {
                OSAL_LOGE("Failed to notify all on condition variable\n");
                break;
            }
        }
        OSAL_LOGD("Condition variable notifyAll succeeded\n");
    }

    int getWaitCount() const override { return waitCount; }

private:
    osSemaphoreId_t cond_;  // Condition variable semaphore
    mutable int waitCount;  // Number of waiting threads
};

}  // namespace osal

#endif  // __OSAL_CONDITION_VARIABLE_H__