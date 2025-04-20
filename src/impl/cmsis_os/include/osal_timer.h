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

#ifndef __OSAL_TIMER_H__
#define __OSAL_TIMER_H__

#include <functional>

#include "osal.h"
#include "interface_timer.h"
#include "osal_chrono.h"
#include "osal_debug.h"
#include "osal_lockguard.h"
#include "osal_mutex.h"

namespace osal {

class OSALTimer : public ITimer {
public:
    OSALTimer() : timerId_(nullptr), running_(false), periodic_(false), interval_(0) {}

    ~OSALTimer() { stop(); }

    void start(uint32_t interval, bool periodic, std::function<void()> callback) override {
        stop();  // 停止任何现有的定时器
        interval_ = interval;
        periodic_ = periodic;
        callback_ = callback;

        osTimerType_t timerType = periodic ? osTimerPeriodic : osTimerOnce;
        osTimerAttr_t timerAttr = {};
        timerAttr.name = "OSALTimer";

        timerId_ = osTimerNew(&OSALTimer::timerCallback, timerType, this, &timerAttr);
        if (timerId_ == nullptr) {
            OSAL_LOGE("Failed to create timer\n");
            return;
        }

        endTime_ = OSALChrono::getInstance().now() + interval_;
        if (osTimerStart(timerId_, interval) != osOK) {
            OSAL_LOGE("Failed to start timer\n");
            osTimerDelete(timerId_);
            timerId_ = nullptr;
            return;
        }

        running_ = true;
        OSAL_LOGD("Timer started\n");
    }

    void stop() override {
        if (timerId_ != nullptr) {
            osTimerStop(timerId_);
            osTimerDelete(timerId_);
            timerId_ = nullptr;
            running_ = false;
            OSAL_LOGD("Timer stopped\n");
        }
    }

    bool isRunning() const override { return running_; }

    uint32_t getRemainingTime() const override {
        // CMSIS-RTOS2 并没有直接提供查询剩余时间的接口
        // 这里可以通过记录开始时间和间隔时间来计算剩余时间
        OSALLockGuard lockGuard(mutex_);
        if (!running_) {
            return 0;
        }

        auto now = OSALChrono::getInstance().now();
        auto remaining = (endTime_ - now);
        return remaining > 0 ? static_cast<uint32_t>(remaining) : 0;
    }

    void reset() override {
        OSALLockGuard lockGuard(mutex_);
        if (running_) {
            osTimerStop(timerId_);
            osTimerStart(timerId_, interval_);
            endTime_ = OSALChrono::getInstance().now() + interval_;
            OSAL_LOGD("Timer reset\n");
        }
    }

private:
    static void timerCallback(void *arg) {
        OSALTimer *timer = static_cast<OSALTimer *>(arg);
        if (timer->callback_) {
            timer->callback_();
        }
        if (!timer->periodic_) {
            timer->running_ = false;
        }
    }

    osTimerId_t timerId_;
    std::atomic<bool> running_;
    bool periodic_;
    unsigned int interval_;
    std::function<void()> callback_;
    mutable OSALMutex mutex_;
    OSALChrono::TimePoint endTime_;
};

}  // namespace osal

#endif  // __OSAL_TIMER_H__