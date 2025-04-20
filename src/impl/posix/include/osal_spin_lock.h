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