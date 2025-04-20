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

#ifndef __OSAL_THREAD_H__
#define __OSAL_THREAD_H__

#include <atomic>
#include <functional>

#include "interface_thread.h"
#include "osal.h"
#include "osal_debug.h"

namespace osal {
class OSALThread : public IThread {
public:
    OSALThread() : threadHandle(nullptr), running(false), suspended(false) {
        OSAL_LOGD("OSALThread default constructor called\n");
    }

    OSALThread(const char *name, std::function<void(void *)> taskFunction, void *taskArgument = nullptr,
               int priority = OSAL_CONFIG_THREAD_DEFAULT_PRIORITY, int stack_size = 0, void *pstack = nullptr)
        : threadHandle(nullptr), running(false), suspended(false) {
        OSAL_LOGD("OSALThread parameterized constructor called\n");
        start(name, taskFunction, taskArgument, priority, stack_size, pstack);
    }

    ~OSALThread() override { stop(); }

    int start(const char *name, std::function<void(void *)> taskFunction, void *taskArgument = nullptr,
              int priority = OSAL_CONFIG_THREAD_DEFAULT_PRIORITY, int stack_size = 0, void *pstack = nullptr) override {
        if (!isRunning()) {
            this->_taskFunction = taskFunction;
            this->_taskArgument = taskArgument;

            osThreadAttr_t attr = {};
            attr.name = name;
            attr.priority = (osPriority_t)priority;
            attr.stack_size = (stack_size > OSAL_CONFIG_THREAD_MINIMAL_STACK_SIZE)
                                  ? stack_size
                                  : OSAL_CONFIG_THREAD_MINIMAL_STACK_SIZE;

            if (pstack != nullptr) {
                attr.stack_mem = pstack;
            }
            exitSemaphore = osSemaphoreNew(1, 0, nullptr);
            if (exitSemaphore == nullptr) {
                OSAL_LOGE("Failed to create semaphore\n");
                return -1;  // 失败
            }

            threadHandle = osThreadNew(reinterpret_cast<osThreadFunc_t>(taskRunner), this, &attr);
            if (threadHandle != nullptr) {
                running = true;
                OSAL_LOGD("Thread started successfully\n");
                return 0;  // 成功
            } else {
                OSAL_LOGE("Failed to create thread\n");
                osSemaphoreDelete(exitSemaphore);
                exitSemaphore = nullptr;
                return -1;  // 失败
            }
        }
        return -1;  // 线程已经在运行
    }

    void stop() override {
        if (threadHandle) {
            // 先终止线程
            osThreadTerminate(threadHandle);

            // 确保线程已终止
            osSemaphoreRelease(exitSemaphore);
            osSemaphoreDelete(exitSemaphore);
            exitSemaphore = nullptr;

            threadHandle = nullptr;
            running = false;
            OSAL_LOGD("Thread stopped\n");
        }
    }

    int suspend() override {
        if (isRunning()) {
            osThreadSuspend(threadHandle);
            suspended = true;
            OSAL_LOGD("Thread suspended\n");
            return 0;  // 成功
        }
        return -1;  // 线程未运行
    }

    int resume() override {
        if (isRunning() && suspended) {
            osThreadResume(threadHandle);
            suspended = false;
            OSAL_LOGD("Thread resumed\n");
            return 0;  // 成功
        }
        return -1;  // 线程未运行或未挂起
    }

    void join() override {
        if (isRunning()) {
            osSemaphoreAcquire(exitSemaphore, osWaitForever);
            running = false;
            OSAL_LOGD("Thread joined\n");
        }
    }

    void detach() override {
        // Threre are inherently detached and will clean up after completion.
        OSAL_LOGD("Thread detached\n");
    }

    bool isRunning() const override { return running; }

    void setPriority(int priority) override {
        if (threadHandle) {
            osThreadSetPriority(threadHandle, (osPriority_t)priority);
            OSAL_LOGD("Thread priority set to %d\n", priority);
        }
    }

    int getPriority() const override {
        if (threadHandle) {
            osPriority_t priority = osThreadGetPriority(threadHandle);
            OSAL_LOGD("Thread priority retrieved: %d\n", priority);
            return (int)priority;
        }
        return -1;  // 返回一个无效的优先级表示错误
    }

private:
    static void taskRunner(void *parameters) {
        auto *thread = static_cast<OSALThread *>(parameters);
        if (thread->_taskFunction) {
            thread->_taskFunction(thread->_taskArgument);
        }
        thread->running = false;
        osSemaphoreRelease(thread->exitSemaphore);
        osThreadExit();
    }

    osThreadId_t threadHandle;
    std::function<void(void *)> _taskFunction;
    void *_taskArgument{};
    std::atomic<bool> running;
    std::atomic<bool> suspended;
    osSemaphoreId_t exitSemaphore{};
};
}  // namespace osal

#endif  // __OSAL_THREAD_H__