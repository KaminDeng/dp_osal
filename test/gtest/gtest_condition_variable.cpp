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
#include "osal_condition_variable.h"
#include "osal_mutex.h"
#include "osal_system.h"
#include "osal_test_framework_config.h"
#include "osal_thread.h"

using namespace osal;

TEST(OSALConditionVariableTest, TestOSALConditionVariableWaitAndNotifyOne) {
#if (TestOSALConditionVariableWaitAndNotifyOneEnabled)
    osal::OSALMutex mutex;
    osal::OSALConditionVariable condVar;

    EXPECT_TRUE(mutex.lock());

    // Start a thread to notify the condition variable after a delay
    OSALThread notifyThread;
    notifyThread.start(
        "NotifyThread",
        [](void *arg) {
            OSALSystem::getInstance().sleep_ms(200);  // Simulate a delay
            static_cast<osal::OSALConditionVariable *>(arg)->notifyOne();
        },
        &condVar, 0, 1024);

    // Test waiting on the condition variable
    condVar.wait(mutex);
    EXPECT_TRUE(mutex.unlock());

    notifyThread.join();  // Ensure the notify thread completes
#else
    GTEST_SKIP();
#endif
}

TEST(OSALConditionVariableTest, TestOSALConditionVariableWaitForTimeout) {
#if (TestOSALConditionVariableWaitForTimeoutEnabled)
    osal::OSALMutex mutex;
    osal::OSALConditionVariable condVar;

    EXPECT_TRUE(mutex.lock());

    // Start a thread to notify the condition variable after a longer delay
    OSALThread timeoutThread;
    timeoutThread.start(
        "TimeoutThread",
        [](void *arg) {
            OSALSystem::getInstance().sleep_ms(1000);  // Simulate a delay longer than the timeout
            static_cast<osal::OSALConditionVariable *>(arg)->notifyOne();
        },
        &condVar, 0, 1024);

    // Should return false as the wait should time out before notify
    EXPECT_FALSE(condVar.waitFor(mutex, 500));
    EXPECT_TRUE(mutex.unlock());

    timeoutThread.join();  // Ensure the timeout thread completes
#else
    GTEST_SKIP();
#endif
}

TEST(OSALConditionVariableTest, TestOSALConditionVariableNotifyAll) {
#if (TestOSALConditionVariableNotifyAllEnabled)
    osal::OSALMutex mutex;
    osal::OSALConditionVariable condVar;
    std::atomic<int> taskExecutedCount(0);

    // Start multiple threads to wait on the condition variable
    auto worker = [&](void *arg) {
        (void)arg;
        mutex.lock();
        condVar.wait(mutex);
        mutex.unlock();
        taskExecutedCount++;
    };

    OSALThread thread1("test_thread1", worker, nullptr, 0, 1024);
    OSALThread thread2("test_thread2", worker, nullptr, 0, 1024);
    OSALThread thread3("test_thread3", worker, nullptr, 0, 1024);

    OSALSystem::getInstance().sleep_ms(100);  // Ensure all threads are waiting
    EXPECT_EQ(taskExecutedCount.load(), 0);
    {
        mutex.lock();
        condVar.notifyAll();
        mutex.unlock();
    }

    thread1.join();
    thread2.join();
    thread3.join();
    EXPECT_EQ(taskExecutedCount.load(), 3);
#else
    GTEST_SKIP();
#endif
}

TEST(OSALConditionVariableTest, TestOSALConditionVariableWaitCount) {
#if (TestOSALConditionVariableWaitCountEnabled)
    OSALConditionVariable condVar;
    OSALMutex mutex;

    auto workerTask = [&](void *) {
        mutex.lock();
        condVar.wait(mutex);
        mutex.unlock();
    };

    OSALThread thread1, thread2;
    thread1.start("TestThread1", workerTask, nullptr, 0, 1024);
    thread2.start("TestThread2", workerTask, nullptr, 0, 1024);

    OSALSystem::getInstance().sleep_ms(100);  // 确保线程进入等待状态
    EXPECT_EQ(condVar.getWaitCount(), 2);

    {
        mutex.lock();
        condVar.notifyAll();
        mutex.unlock();
    }

    thread1.join();
    thread2.join();
    EXPECT_EQ(condVar.getWaitCount(), 0);
#else
    GTEST_SKIP();
#endif
}