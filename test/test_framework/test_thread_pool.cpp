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

#include <atomic>

#include "osal_chrono.h"
#include "osal_system.h"
#include "osal_thread_pool.h"
#include "test_framework.h"

using namespace osal;

TEST_CASE(TestOSALThreadPoolStartStop) {
#if (TestOSALThreadPoolStartStopEnabled)
    osal::OSALThreadPool threadPool;
    threadPool.start(4, 0, 1024);
    OSAL_ASSERT_TRUE(threadPool.isStarted());
    auto task = [](void *arg) {
        (void)arg;
        OSALSystem::getInstance().sleep_ms(10000);  // 模拟任务执行时间
    };
    threadPool.submit(task, nullptr, 0);
    auto timestamp_now = OSALChrono::getInstance().now();
    OSALSystem::getInstance().sleep_ms(100);
    threadPool.stop();
    auto interval = OSALChrono::getInstance().now() - timestamp_now;
    OSAL_ASSERT_TRUE(interval > 50 && interval < 500);
    OSAL_ASSERT_FALSE(threadPool.isStarted());
#endif
    return 0;  // 表示测试通过
}

TEST_CASE(TestOSALThreadPoolSuspendResume) {
#if (TestOSALThreadPoolSuspendResumeEnabled)
    osal::OSALThreadPool threadPool;
    threadPool.start(4, 0, 1024);
    OSAL_ASSERT_TRUE(threadPool.isStarted());
    OSAL_ASSERT_EQ(threadPool.suspend(), 0);
    OSAL_ASSERT_TRUE(threadPool.isSuspended());
    OSAL_ASSERT_EQ(threadPool.resume(), 0);
    OSAL_ASSERT_FALSE(threadPool.isSuspended());
    threadPool.stop();
#endif
    return 0;  // 表示测试通过
}

auto task = [](void *arg) {
    bool *executed = static_cast<bool *>(arg);
    *executed = true;
};

TEST_CASE(TestOSALThreadPoolSubmitTask) {
#if (TestOSALThreadPoolSubmitTaskEnabled)
    osal::OSALThreadPool threadPool;
    threadPool.start(4, 0, 1024);

    bool taskExecuted = false;
    threadPool.submit(task, &taskExecuted, 0);
    OSALSystem::getInstance().sleep_ms(500);  // 等待任务执行
    OSAL_ASSERT_TRUE(taskExecuted);

    threadPool.stop();
#endif
    return 0;  // 表示测试通过
}

TEST_CASE(TestOSALThreadPoolSetPriority) {
#if (TestOSALThreadPoolSetPriorityEnabled)
    osal::OSALThreadPool threadPool;
    threadPool.setPriority(5);
    OSAL_ASSERT_EQ(threadPool.getPriority(), 5);
#endif
    return 0;  // 表示测试通过
}

TEST_CASE(TestOSALThreadPoolGetTaskQueueSize) {
#if (TestOSALThreadPoolGetTaskQueueSizeEnabled)
    osal::OSALThreadPool threadPool;
    threadPool.start(4, 0, 1024);

    auto task = [](void *arg) {
        (void)arg;
        OSALSystem::getInstance().sleep_ms(1000);  // 模拟任务执行时间
    };

    threadPool.suspend();
    threadPool.submit(task, nullptr, 0);
    OSAL_ASSERT_EQ(threadPool.getTaskQueueSize(), 1);

    threadPool.stop();
#endif
    return 0;  // 表示测试通过
}

TEST_CASE(TestOSALThreadPoolGetActiveThreadCount) {
#if (TestOSALThreadPoolGetActiveThreadCountEnabled)
    osal::OSALThreadPool threadPool;
    threadPool.start(4, 0, 1024);
    OSAL_ASSERT_EQ(threadPool.getActiveThreadCount(), 0);

    bool taskExecuted = false;
    auto task = [](void *arg) {
        (void)arg;
        OSALSystem::getInstance().sleep_ms(500);
    };
    for (int i = 0; i < 4; i++) {
        threadPool.submit(task, &taskExecuted, 0);
    }
    OSALSystem::getInstance().sleep_ms(200);
    OSAL_ASSERT_EQ(threadPool.getActiveThreadCount(), 4);
    threadPool.stop();
#endif
    return 0;  // 表示测试通过
}

TEST_CASE(TestOSALThreadPoolCancelTask) {
#if (TestOSALThreadPoolCancelTaskEnabled)
    osal::OSALThreadPool threadPool;
    threadPool.start(4, 0, 1024);

    bool taskExecuted = false;
    std::function<void(void *)> task = [](void *arg) {
        bool *executed = static_cast<bool *>(arg);
        *executed = true;
    };

    threadPool.submit(task, &taskExecuted, 0);
    OSALSystem::getInstance().sleep_ms(500);  // 等待任务执行
    OSAL_ASSERT_FALSE(threadPool.cancelTask(task));
    OSAL_ASSERT_TRUE(taskExecuted)

    taskExecuted = false;
    threadPool.suspend();
    threadPool.submit(task, &taskExecuted, 0);
    OSAL_ASSERT_TRUE(threadPool.cancelTask(task));
    OSAL_ASSERT_FALSE(taskExecuted)

    threadPool.stop();
#endif
    return 0;  // 表示测试通过
}

TEST_CASE(TestOSALThreadPoolSetTaskFailureCallback) {
#if (TestOSALThreadPoolSetTaskFailureCallbackEnabled)
    osal::OSALThreadPool threadPool;
    threadPool.start(4, 0, 1024);

    bool callbackCalled = false;
    auto callback = [](void *arg) {
        bool *called = static_cast<bool *>(arg);
        *called = true;
    };

    threadPool.setTaskFailureCallback(callback);
    threadPool.submit(nullptr, &callbackCalled, 0);  // 提交一个无效任务以触发回调
    OSALSystem::getInstance().sleep_ms(500);         // 等待回调执行
    OSAL_ASSERT_TRUE(callbackCalled);
#endif
    return 0;  // 表示测试通过
}

TEST_CASE(TestOSALThreadPoolSetMaxThreads) {
#if (TestOSALThreadPoolSetMaxThreadsEnabled)
    osal::OSALThreadPool threadPool;
    threadPool.start(2, 0, 1024);
    OSAL_ASSERT_EQ(threadPool.getMinThreads(), 2);

    threadPool.setMaxThreads(4);
    OSAL_ASSERT_EQ(threadPool.getMaxThreads(), 4);

    bool taskExecuted = false;
    auto task = [](void *arg) {
        (void)arg;
        OSALSystem::getInstance().sleep_ms(600);
    };
    for (int i = 0; i < 4; i++) {
        threadPool.submit(task, &taskExecuted, 0);
        OSALSystem::getInstance().sleep_ms(100);
    }
    OSAL_ASSERT_EQ(threadPool.getActiveThreadCount(), 4);
    threadPool.stop();
#endif
    return 0;  // 表示测试通过
}

TEST_CASE(TestOSALThreadPoolSetMinThreads) {
#if (TestOSALThreadPoolSetMinThreadsEnabled)
    osal::OSALThreadPool threadPool;
    threadPool.setMinThreads(2);
    OSAL_ASSERT_EQ(threadPool.getMinThreads(), 2);
#endif
    return 0;  // 表示测试通过
}
