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

#ifndef _OSAL_MUTEX_H__
#define _OSAL_MUTEX_H__

#include <pthread.h>

#include <cerrno>
#include <cstring>
#include <ctime>

#include "interface_mutex.h"
#include "osal_debug.h"

namespace osal {

class OSALMutex : public IMutex {
public:
    OSALMutex() {
        pthread_mutexattr_t attr;
        pthread_mutexattr_init(&attr);
        pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
        pthread_mutex_init(&mutex_, &attr);
        pthread_mutexattr_destroy(&attr);
    }

    ~OSALMutex() { pthread_mutex_destroy(&mutex_); }

    bool lock() override {
        int ret = pthread_mutex_lock(&mutex_);
        if (ret != 0) {
            OSAL_LOGE("Failed to lock mutex, return code %d\n", ret);
            return false;
        }
        OSAL_LOGD("Mutex locked successfully\n");
        return true;
    }

    bool unlock() override {
        int ret = pthread_mutex_unlock(&mutex_);
        if (ret != 0) {
            OSAL_LOGE("Failed to unlock mutex, return code %d\n", ret);
            return false;
        }
        OSAL_LOGD("Mutex unlocked successfully\n");
        return true;
    }

    bool tryLock() override {
        int ret = pthread_mutex_trylock(&mutex_);
        if (ret != 0) {
            OSAL_LOGD("Failed to try lock mutex, return code %d\n", ret);
            return false;
        }
        OSAL_LOGD("Mutex try lock successfully\n");
        return true;
    }

    bool tryLockFor(const uint32_t timeout) override {
        struct timespec ts;
        clock_gettime(CLOCK_REALTIME, &ts);
        ts.tv_sec += timeout / 1000;
        ts.tv_nsec += (timeout % 1000) * 1000000;
        if (ts.tv_nsec >= 1000000000) {
            ts.tv_sec++;
            ts.tv_nsec -= 1000000000;
        }

        int ret = timed_mutex_lock(&mutex_, &ts);
        if (ret != 0) {
            OSAL_LOGD("Failed to timed lock mutex, return code %d\n", ret);
            return false;
        }
        OSAL_LOGD("Mutex timed lock successfully\n");
        return true;
    }

    pthread_mutex_t &getNativeHandle() { return mutex_; }

private:
    pthread_mutex_t mutex_;

    int timed_mutex_lock(pthread_mutex_t *mutex, const struct timespec *timeout) {
        struct timespec now;
        while (clock_gettime(CLOCK_REALTIME, &now) == 0) {
            if (now.tv_sec > timeout->tv_sec || (now.tv_sec == timeout->tv_sec && now.tv_nsec >= timeout->tv_nsec)) {
                return ETIMEDOUT;
            }
            int ret = pthread_mutex_trylock(mutex);
            if (ret == 0) {
                return 0;
            } else if (ret != EBUSY) {
                return ret;
            }
            // 等待一段时间再重试
            struct timespec wait_time = {0, 1000000};  // 1毫秒
            nanosleep(&wait_time, NULL);
        }
        return errno;
    }
};

}  // namespace osal

#endif  // _OSAL_MUTEX_H__