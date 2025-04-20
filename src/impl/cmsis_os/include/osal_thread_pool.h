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

#ifndef __OSAL_THREADPOOL_H__
#define __OSAL_THREADPOOL_H__

#include <atomic>
#include <functional>
#include <memory>
#include <queue>
#include <vector>

#include "osal.h"
#include "interface_thread_pool.h"
#include "osal_condition_variable.h"
#include "osal_debug.h"
#include "osal_lockguard.h"
#include "osal_mutex.h"
#include "osal_thread.h"

namespace osal {

class OSALThreadPool : public IThreadPool {
public:
    OSALThreadPool();

    ~OSALThreadPool() override;

    void start(uint32_t numThreads, int priority = 0, int stack_size = 0) override;

    void stop() override;

    int suspend() override;

    int resume() override;

    bool isStarted() const override;

    bool isSuspended() const override;

    void submit(std::function<void(void *)> taskFunction, void *taskArgument, int priority) override;

    void setPriority(int priority) override;

    int getPriority() const override;

    size_t getTaskQueueSize() override;

    uint32_t getActiveThreadCount() const override;

    bool cancelTask(std::function<void(void *)> &taskFunction) override;

    void setTaskFailureCallback(std::function<void(void *)> callback) override;

    void setMaxThreads(uint32_t maxThreads) override;

    uint32_t getMaxThreads() const override;

    void setMinThreads(uint32_t minThreads) override;

    uint32_t getMinThreads() const override;

private:
    struct Task {
        std::function<void(void *)> function;
        void *argument;
        int priority;
    };

    static void threadEntry(void *arg);

    bool OSALAddTread();

    bool OSALDelTread();

    void threadLoop();

    std::vector<std::unique_ptr<OSALThread>> threads_;
    std::queue<Task> taskQueue_;
    OSALMutex queueMutex_;
    OSALConditionVariable condition_;
    std::atomic<bool> isstarted_;
    std::atomic<bool> suspended_;
    std::atomic<int> priority_;
    std::atomic<int> stack_size_;
    std::atomic<uint32_t> activeThreads_;
    std::atomic<uint32_t> maxThreads_;
    std::atomic<uint32_t> minThreads_;
    std::function<void(void *)> taskFailureCallback_;
};

}  // namespace osal

#endif  // __OSAL_THREADPOOL_H__