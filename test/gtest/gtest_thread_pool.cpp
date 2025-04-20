/*
 * Copyright (c) 2024 kamin.deng
 * Email: kamin.deng@gmail.com
 * Created on 2025/1/3.
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

#include "gtest/gtest.h"
#include "osal_chrono.h"
#include "osal_system.h"
#include "osal_test_framework_config.h"
#include "osal_thread_pool.h"

using namespace osal;

TEST(OSALThreadPoolTests, TestOSALThreadPoolStartStop) {
#if (TestOSALThreadPoolStartStopEnabled)
    osal::OSALThreadPool threadPool;
    threadPool.start(4, 0, 1024);
    ASSERT_TRUE(threadPool.isStarted());
    auto task = [](void *arg) {
        (void)arg;
        OSALSystem::getInstance().sleep_ms(10000);  // Simulate task execution time
    };
    threadPool.submit(task, nullptr, 0);
    auto timestamp_now = OSALChrono::getInstance().now();
    OSALSystem::getInstance().sleep_ms(100);
    threadPool.stop();
    auto interval = OSALChrono::getInstance().now() - timestamp_now;
    ASSERT_TRUE(interval > 50 && interval < 500);
    ASSERT_FALSE(threadPool.isStarted());
#else
    GTEST_SKIP();
#endif
}

TEST(OSALThreadPoolTests, TestOSALThreadPoolSuspendResume) {
#if (TestOSALThreadPoolSuspendResumeEnabled)
    osal::OSALThreadPool threadPool;
    threadPool.start(4, 0, 1024);
    ASSERT_TRUE(threadPool.isStarted());
    ASSERT_EQ(threadPool.suspend(), 0);
    ASSERT_TRUE(threadPool.isSuspended());
    ASSERT_EQ(threadPool.resume(), 0);
    ASSERT_FALSE(threadPool.isSuspended());
    threadPool.stop();
#else
    GTEST_SKIP();
#endif
}

auto task = [](void *arg) {
    bool *executed = static_cast<bool *>(arg);
    *executed = true;
};

TEST(OSALThreadPoolTests, TestOSALThreadPoolSubmitTask) {
#if (TestOSALThreadPoolSubmitTaskEnabled)
    osal::OSALThreadPool threadPool;
    threadPool.start(4, 0, 1024);

    bool taskExecuted = false;
    threadPool.submit(task, &taskExecuted, 0);
    OSALSystem::getInstance().sleep_ms(500);  // Wait for task execution
    ASSERT_TRUE(taskExecuted);

    threadPool.stop();
#else
    GTEST_SKIP();
#endif
}

TEST(OSALThreadPoolTests, TestOSALThreadPoolSetPriority) {
#if (TestOSALThreadPoolSetPriorityEnabled)
    osal::OSALThreadPool threadPool;
    threadPool.setPriority(5);
    ASSERT_EQ(threadPool.getPriority(), 5);
#else
    GTEST_SKIP();
#endif
}

TEST(OSALThreadPoolTests, TestOSALThreadPoolGetTaskQueueSize) {
#if (TestOSALThreadPoolGetTaskQueueSizeEnabled)
    osal::OSALThreadPool threadPool;
    threadPool.start(4, 0, 1024);

    auto task = [](void *arg) {
        (void)arg;
        OSALSystem::getInstance().sleep_ms(1000);  // Simulate task execution time
    };

    threadPool.suspend();
    threadPool.submit(task, nullptr, 0);
    ASSERT_EQ(threadPool.getTaskQueueSize(), 1);

    threadPool.stop();
#else
    GTEST_SKIP();
#endif
}

TEST(OSALThreadPoolTests, TestOSALThreadPoolGetActiveThreadCount) {
#if (TestOSALThreadPoolGetActiveThreadCountEnabled)
    osal::OSALThreadPool threadPool;
    threadPool.start(4, 0, 1024);
    ASSERT_EQ(threadPool.getActiveThreadCount(), 0);

    bool taskExecuted = false;
    auto task = [](void *arg) {
        (void)arg;
        OSALSystem::getInstance().sleep_ms(500);
    };
    for (int i = 0; i < 4; i++) {
        threadPool.submit(task, &taskExecuted, 0);
    }
    OSALSystem::getInstance().sleep_ms(200);
    ASSERT_EQ(threadPool.getActiveThreadCount(), 4);
    threadPool.stop();
#else
    GTEST_SKIP();
#endif
}

TEST(OSALThreadPoolTests, TestOSALThreadPoolCancelTask) {
#if (TestOSALThreadPoolCancelTaskEnabled)
    osal::OSALThreadPool threadPool;
    threadPool.start(4, 0, 1024);

    bool taskExecuted = false;
    std::function<void(void *)> task = [](void *arg) {
        bool *executed = static_cast<bool *>(arg);
        *executed = true;
    };

    threadPool.submit(task, &taskExecuted, 0);
    OSALSystem::getInstance().sleep_ms(500);  // Wait for task execution
    ASSERT_FALSE(threadPool.cancelTask(task));
    ASSERT_TRUE(taskExecuted);

    taskExecuted = false;
    threadPool.suspend();
    threadPool.submit(task, &taskExecuted, 0);
    ASSERT_TRUE(threadPool.cancelTask(task));
    ASSERT_FALSE(taskExecuted);

    threadPool.stop();
#else
    GTEST_SKIP();
#endif
}

TEST(OSALThreadPoolTests, TestOSALThreadPoolSetTaskFailureCallback) {
#if (TestOSALThreadPoolSetTaskFailureCallbackEnabled)
    osal::OSALThreadPool threadPool;
    threadPool.start(4, 0, 1024);

    bool callbackCalled = false;
    auto callback = [](void *arg) {
        bool *called = static_cast<bool *>(arg);
        *called = true;
    };

    threadPool.setTaskFailureCallback(callback);
    threadPool.submit(nullptr, &callbackCalled, 0);  // Submit an invalid task to trigger callback
    OSALSystem::getInstance().sleep_ms(500);         // Wait for callback execution
    ASSERT_TRUE(callbackCalled);
#else
    GTEST_SKIP();
#endif
}

TEST(OSALThreadPoolTests, TestOSALThreadPoolSetMaxThreads) {
#if (TestOSALThreadPoolSetMaxThreadsEnabled)
    osal::OSALThreadPool threadPool;
    threadPool.start(2, 0, 1024);
    ASSERT_EQ(threadPool.getMinThreads(), 2);

    threadPool.setMaxThreads(4);
    ASSERT_EQ(threadPool.getMaxThreads(), 4);

    bool taskExecuted = false;
    auto task = [](void *arg) {
        (void)arg;
        OSALSystem::getInstance().sleep_ms(600);
    };
    for (int i = 0; i < 4; i++) {
        threadPool.submit(task, &taskExecuted, 0);
        OSALSystem::getInstance().sleep_ms(100);
    }
    ASSERT_EQ(threadPool.getActiveThreadCount(), 4);
    threadPool.stop();
#else
    GTEST_SKIP();
#endif
}

TEST(OSALThreadPoolTests, TestOSALThreadPoolSetMinThreads) {
#if (TestOSALThreadPoolSetMinThreadsEnabled)
    osal::OSALThreadPool threadPool;
    threadPool.setMinThreads(2);
    ASSERT_EQ(threadPool.getMinThreads(), 2);
#else
    GTEST_SKIP();
#endif
}