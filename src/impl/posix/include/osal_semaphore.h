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

#ifndef __OSAL_SEMAPHORE_H__
#define __OSAL_SEMAPHORE_H__

#include <chrono>
#include <condition_variable>
#include <mutex>

#include "interface_semaphore.h"
#include "osal_debug.h"

namespace osal {

class OSALSemaphore : public ISemaphore {
public:
    OSALSemaphore() : count_(0) { OSAL_LOGD("Semaphore initialized\n"); }

    ~OSALSemaphore() { OSAL_LOGD("Semaphore destroyed\n"); }

    void wait() override {
        std::unique_lock<std::mutex> lock(mutex_);
        cond_.wait(lock, [this]() { return count_ > 0; });
        --count_;
        OSAL_LOGD("Semaphore wait succeeded\n");
    }

    void signal() override {
        std::lock_guard<std::mutex> lock(mutex_);
        ++count_;
        cond_.notify_one();
        OSAL_LOGD("Semaphore signal succeeded\n");
    }

    bool tryWait() override {
        std::lock_guard<std::mutex> lock(mutex_);
        if (count_ > 0) {
            --count_;
            OSAL_LOGD("Semaphore tryWait succeeded\n");
            return true;
        }
        OSAL_LOGD("Semaphore tryWait failed\n");
        return false;
    }

    bool tryWaitFor(uint32_t timestamp) override {
        std::unique_lock<std::mutex> lock(mutex_);
        auto now = std::chrono::system_clock::now();
        if (cond_.wait_until(lock, now + std::chrono::milliseconds(timestamp), [this] { return count_ > 0; })) {
            --count_;
            OSAL_LOGD("Semaphore tryWaitFor succeeded\n");
            return true;
        }
        OSAL_LOGD("Semaphore tryWaitFor failed\n");
        return false;
    }

    int getValue() const override {
        std::lock_guard<std::mutex> lock(mutex_);
        OSAL_LOGD("Semaphore value: %d\n", count_);
        return count_;
    }

    void init(int initialValue) override {
        std::lock_guard<std::mutex> lock(mutex_);
        count_ = initialValue;
        OSAL_LOGD("Semaphore initialized with value %d\n", initialValue);
    }

private:
    mutable std::mutex mutex_;
    std::condition_variable cond_;
    int count_;
};

}  // namespace osal

#endif  // __OSAL_SEMAPHORE_H__