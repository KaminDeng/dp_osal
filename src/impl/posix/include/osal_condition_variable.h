/*
 * Copyright (c) 2024 kamin.deng
 * Email: kamin.deng@gmail.com
 * Created on 2024/8/23.
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

#include <pthread.h>

#include <chrono>

#include "interface_condition_variable.h"
#include "osal_debug.h"
#include "osal_mutex.h"

namespace osal {

class OSALConditionVariable : public IConditionVariable {
public:
    OSALConditionVariable() : waitCount(0) {
        if (pthread_cond_init(&cond_, nullptr) != 0) {
            OSAL_LOGE("Failed to initialize condition variable\n");
        } else {
            OSAL_LOGD("Condition variable initialized\n");
        }
    }

    ~OSALConditionVariable() {
        if (pthread_cond_destroy(&cond_) != 0) {
            OSAL_LOGE("Failed to destroy condition variable\n");
        } else {
            OSAL_LOGD("Condition variable destroyed\n");
        }
    }

    //        void wail(IMutex& mutex) = delete;
    void wait(OSALMutex &mutex) override {
        waitCount++;
        if (pthread_cond_wait(&cond_, &mutex.getNativeHandle()) != 0) {
            OSAL_LOGE("Failed to wait on condition variable\n");
        } else {
            OSAL_LOGD("Condition variable wait succeeded\n");
        }
        waitCount--;
    }

    bool waitFor(OSALMutex &mutex, uint32_t timeout) override {
        waitCount++;

        // 获取当前时间
        auto now = std::chrono::system_clock::now();

        // 计算超时时间
        auto timeoutTime = now + std::chrono::milliseconds(timeout);

        // 将超时时间转换为 timespec 结构体
        timespec ts;
        ts.tv_sec = std::chrono::duration_cast<std::chrono::seconds>(timeoutTime.time_since_epoch()).count();
        ts.tv_nsec =
            std::chrono::duration_cast<std::chrono::nanoseconds>(timeoutTime.time_since_epoch()).count() % 1000000000;

        // 等待条件变量
        int result = pthread_cond_timedwait(&cond_, &mutex.getNativeHandle(), &ts);
        waitCount--;

        if (result == 0) {
            OSAL_LOGD("Condition variable waitFor succeeded\n");
            return true;
        } else if (result == ETIMEDOUT) {
            OSAL_LOGD("Condition variable waitFor timed out\n");
            return false;
        } else {
            OSAL_LOGE("Failed to wait on condition variable with timeout\n");
            return false;
        }
    }

    void notifyOne() override {
        if (pthread_cond_signal(&cond_) != 0) {
            OSAL_LOGE("Failed to notify one on condition variable\n");
        } else {
            OSAL_LOGD("Condition variable notifyOne succeeded\n");
        }
    }

    void notifyAll() override {
        if (pthread_cond_broadcast(&cond_) != 0) {
            OSAL_LOGE("Failed to notify all on condition variable\n");
        } else {
            OSAL_LOGD("Condition variable notifyAll succeeded\n");
        }
    }

    int getWaitCount() const override { return waitCount; }

private:
    pthread_cond_t cond_;
    mutable int waitCount;  // 当前等待线程数
};

}  // namespace osal

#endif  // __OSAL_CONDITION_VARIABLE_H__