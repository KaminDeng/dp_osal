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

#ifndef _OSAL_MUTEX_H__
#define _OSAL_MUTEX_H__

#include "osal.h"
#include "interface_mutex.h"
#include "osal_debug.h"

namespace osal {

class OSALMutex : public IMutex {
public:
    OSALMutex() {
        osMutexAttr_t mutexAttr = {};
        mutexAttr.name = "OSALMutex";
        mutexAttr.attr_bits = osMutexRecursive | osMutexPrioInherit;
        mutex_ = osMutexNew(&mutexAttr);
        if (mutex_ == nullptr) {
            OSAL_LOGE("Failed to initialize mutex\n");
        } else {
            OSAL_LOGD("Mutex initialized\n");
        }
    }

    ~OSALMutex() {
        if (osMutexDelete(mutex_) != osOK) {
            OSAL_LOGE("Failed to destroy mutex\n");
        } else {
            OSAL_LOGD("Mutex destroyed\n");
        }
    }

    bool lock() override {
        osStatus_t status = osMutexAcquire(mutex_, osWaitForever);
        if (status != osOK) {
            OSAL_LOGE("Failed to lock mutex, status code %d\n", status);
            return false;
        }
        OSAL_LOGD("Mutex locked successfully\n");
        return true;
    }

    bool unlock() override {
        osStatus_t status = osMutexRelease(mutex_);
        if (status != osOK) {
            OSAL_LOGE("Failed to unlock mutex, status code %d\n", status);
            return false;
        }
        OSAL_LOGD("Mutex unlocked successfully\n");
        return true;
    }

    bool tryLock() override {
        osStatus_t status = osMutexAcquire(mutex_, 0);
        if (status != osOK) {
            OSAL_LOGD("Failed to try lock mutex, status code %d\n", status);
            return false;
        }
        OSAL_LOGD("Mutex try lock successful\n");
        return true;
    }

    bool tryLockFor(const uint32_t timeout) override {
        osStatus_t status = osMutexAcquire(mutex_, timeout);
        if (status != osOK) {
            OSAL_LOGD("Failed to timed lock mutex, status code %d\n", status);
            return false;
        }
        OSAL_LOGD("Mutex timed lock successful\n");
        return true;
    }

    osMutexId_t getNativeHandle() { return mutex_; }

private:
    osMutexId_t mutex_;
};

}  // namespace osal

#endif  // _OSAL_MUTEX_H__