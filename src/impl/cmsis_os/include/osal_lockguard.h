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

#ifndef __OSAL_LOCK_GUARD_H__
#define __OSAL_LOCK_GUARD_H__

#include "interface_lockguard.h"
#include "osal_debug.h"
#include "osal_mutex.h"

namespace osal {

class OSALLockGuard : public ILockGuard {
public:
    explicit OSALLockGuard(OSALMutex &mutex) : mutex_(mutex), locked_(false) {
        locked_ = mutex_.lock();
        if (locked_) {
            OSAL_LOGD("Mutex locked successfully in OSALLockGuard\n");
        } else {
            OSAL_LOGE("Failed to lock mutex in OSALLockGuard\n");
        }
    }

    ~OSALLockGuard() override {
        if (locked_) {
            if (mutex_.unlock()) {
                OSAL_LOGD("Mutex unlocked successfully in OSALLockGuard\n");
            } else {
                OSAL_LOGE("Failed to unlock mutex in OSALLockGuard\n");
            }
        }
    }

    [[nodiscard]] bool isLocked() override { return locked_; }

private:
    OSALMutex &mutex_;
    bool locked_;
};

}  // namespace osal

#endif  // __OSAL_LOCK_GUARD_H__