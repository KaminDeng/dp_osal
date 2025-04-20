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

#ifndef __OSAL_THREAD_H__
#define __OSAL_THREAD_H__

#include <pthread.h>

#include <atomic>
#include <condition_variable>
#include <cstring>
#include <functional>
#include <iostream>
#include <mutex>

#include "interface_thread.h"
#include "osal_debug.h"

namespace osal {
class OSALThread : public IThread {
public:
    OSALThread() : threadHandle(0), running(false), suspended(false) {
        OSAL_LOGD("OSALThread default constructor called\n");
    }

    OSALThread(const char *name, std::function<void(void *)> taskFunction, void *taskArgument = nullptr,
               int priority = 0, int stack_size = 0, void *pstack = nullptr)
        : threadHandle(0), running(false), suspended(false) {
        OSAL_LOGD("OSALThread parameterized constructor called\n");
        start(name, taskFunction, taskArgument, priority, stack_size, pstack);
    }

    virtual ~OSALThread() { stop(); }

    int start(const char *name, std::function<void(void *)> taskFunction, void *taskArgument = nullptr,
              int priority = 0, int stack_size = 0, void *pstack = nullptr) override {
        (void)name;
        int result = 0;
        if (!isRunning()) {
            this->taskFunction = taskFunction;
            this->taskArgument = taskArgument;

            pthread_attr_t attr;
            pthread_attr_init(&attr);

            // 设置线程堆栈大小
            if (stack_size > 0) {
                int size = (stack_size > OSAL_CONFIG_THREAD_MINIMAL_STACK_SIZE) ? stack_size
                                                                                : OSAL_CONFIG_THREAD_MINIMAL_STACK_SIZE;
                pthread_attr_setstacksize(&attr, size);
            }

            // 设置线程堆栈内存
            if (pstack != nullptr) {
                pthread_attr_setstack(&attr, pstack, stack_size);
            }

            // 设置线程优先级
            if (priority > 0) {
                sched_param schedParam;
                pthread_attr_getschedparam(&attr, &schedParam);
                schedParam.sched_priority = priority;
                pthread_attr_setschedparam(&attr, &schedParam);
            }

            // 设置线程分离状态
            pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

            result = pthread_create(&threadHandle, &attr, taskRunner, this);
            if (result == 0) {
                running = true;
                OSAL_LOGD("Thread started successfully\n");
            } else {
                OSAL_LOGE("Failed to create thread: %s\n", strerror(result));
            }

            pthread_attr_destroy(&attr);
        }
        return result;
    }

    void stop() override {
        if (threadHandle) {
            {
                std::unique_lock<std::mutex> lock(mutex_);
                cv_.notify_all();
            }
            pthread_cancel(threadHandle);
            pthread_join(threadHandle, nullptr);
            OSAL_LOGD("OSALThread destructor called, canceling thread\n");
        }
    }

    int suspend() override {
        std::unique_lock<std::mutex> lock(mutex_);
        suspended = true;
        OSAL_LOGD("Thread suspended\n");
        return 0;  // 成功
    }

    int resume() override {
        std::unique_lock<std::mutex> lock(mutex_);
        suspended = false;
        cv_.notify_all();
        OSAL_LOGD("Thread resumed\n");
        return 0;  // 成功
    }

    void join() override {
        if (threadHandle) {
            pthread_join(threadHandle, nullptr);
            OSAL_LOGD("Thread joined\n");
        }
    }

    void detach() override {
        if (threadHandle) {
            pthread_detach(threadHandle);
            OSAL_LOGD("Thread detached\n");
        }
    }

    bool isRunning() const override { return running; }

    void setPriority(int priority) override {
        if (threadHandle) {
            struct sched_param schedParam;
            schedParam.sched_priority = priority;
            pthread_setschedparam(threadHandle, SCHED_FIFO, &schedParam);
            OSAL_LOGD("Thread priority set to %d\n", priority);
        }
    }

    int getPriority() const override {
        if (threadHandle) {
            int policy;
            struct sched_param schedParam;
            pthread_getschedparam(threadHandle, &policy, &schedParam);
            OSAL_LOGD("Thread priority retrieved: %d\n", schedParam.sched_priority);
            return schedParam.sched_priority;
        }
        return -1;  // 返回一个无效的优先级表示错误
    }

private:
    static void *taskRunner(void *parameters) {
        // 设置取消类型为异步, 可以使线程在接收到取消请求时立即响应而无需等待取消点。
        pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, nullptr);

        OSALThread *thread = static_cast<OSALThread *>(parameters);
        if (thread->taskFunction) {
            thread->waitIfSuspended();
            thread->taskFunction(thread->taskArgument);
        }
        thread->running = false;
        pthread_exit(nullptr);
        return nullptr;
    }

    void waitIfSuspended() {
        std::unique_lock<std::mutex> lock(mutex_);
        while (suspended) {
            cv_.wait(lock);
        }
    }

    pthread_t threadHandle;
    std::function<void(void *)> taskFunction;
    void *taskArgument;
    std::atomic<bool> running;
    std::atomic<bool> suspended;
    std::mutex mutex_;
    std::condition_variable cv_;
};
}  // namespace osal

#endif  // __OSAL_THREAD_H__