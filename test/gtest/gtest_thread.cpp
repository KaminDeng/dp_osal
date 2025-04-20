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

#include <gtest/gtest.h>

#include <atomic>

#include "osal_chrono.h"
#include "osal_system.h"
#include "osal_test_framework_config.h"
#include "osal_thread.h"

using namespace osal;

TEST(OSALThreadTest, TestOSALThreadStart) {
#if (TestOSALThreadStartEnabled)
    OSALThread thread;
    std::atomic<int> taskExecuted(1);

    thread.start(
        "TestThread",
        [&](void *) {
            taskExecuted = 2;
            OSALSystem::getInstance().sleep_ms(10000);
            taskExecuted = 3;
        },
        nullptr, 0, 1024);

    auto timestamp_now = OSALChrono::getInstance().now();
    OSALSystem::getInstance().sleep_ms(100);
    EXPECT_EQ(taskExecuted, 2);
    thread.stop();
    auto interval = OSALChrono::getInstance().now() - timestamp_now;
    EXPECT_EQ(taskExecuted, 2);
    EXPECT_TRUE(interval > 50 && interval < 500);

    // 尝试再次启动同一个线程，应该失败
    // EXPECT_TRUE(0 != thread.start("TestThread", [&](void*) {}));
#else
    GTEST_SKIP();
#endif
}

TEST(OSALThreadTest, TestOSALThreadStop) {
#if (TestOSALThreadStopEnabled)
    OSALThread thread;
    std::atomic<bool> taskExecuted(false);

    thread.start("TestThread", [&](void *) { taskExecuted = true; }, nullptr, 0, 1024);

    thread.join();
    EXPECT_TRUE(taskExecuted);

    // 尝试再次启动同一个线程，应该失败
    // EXPECT_TRUE(0 != thread.start("TestThread", [&](void*) {}));
#else
    GTEST_SKIP();
#endif
}

TEST(OSALThreadTest, TestOSALThreadJoin) {
#if (TestOSALThreadJoinEnabled)
    OSALThread thread;
    std::atomic<bool> taskExecuted(false);

    thread.start("TestThread", [&](void *) { taskExecuted = true; }, nullptr, 0, 1024);

    thread.join();
    EXPECT_TRUE(taskExecuted);

    // 尝试再次join同一个线程，应该安全
    thread.join();
#else
    GTEST_SKIP();
#endif
}

TEST(OSALThreadTest, TestOSALThreadDetach) {
#if (TestOSALThreadDetachEnabled)
    OSALThread thread;
    std::atomic<bool> taskExecuted(false);

    thread.start(
        "TestThread",
        [&](void *) {
            OSALSystem::getInstance().sleep_ms(50);
            taskExecuted = true;
        },
        nullptr, 0, 1024);

    thread.detach();
    OSALSystem::getInstance().sleep_ms(100);
    EXPECT_TRUE(taskExecuted);

    // 尝试再次detach同一个线程，应该安全
    thread.detach();
#else
    GTEST_SKIP();
#endif
}

TEST(OSALThreadTest, TestOSALThreadIsRunning) {
#if (TestOSALThreadIsRunningEnabled)
    OSALThread thread;
    std::atomic<bool> taskExecuted(false);

    thread.start(
        "TestThread",
        [&](void *) {
            OSALSystem::getInstance().sleep_ms(100);
            taskExecuted = true;
        },
        nullptr, 0, 1024);

    EXPECT_TRUE(thread.isRunning());
    thread.join();
    EXPECT_FALSE(thread.isRunning());

    // 尝试在线程结束后检查状态
    EXPECT_FALSE(thread.isRunning());
#else
    GTEST_SKIP();
#endif
}

TEST(OSALThreadTest, TestOSALThreadSetAndGetPriority) {
#if (TestOSALThreadSetAndGetPriorityEnabled)
    OSALThread thread;
    std::atomic<bool> taskExecuted(false);

    thread.start("TestThread", [&](void *) { taskExecuted = true; }, nullptr, 0, 1024);

    // 设置线程优先级
    int priority = 10;
    thread.setPriority(priority);

    // 获取线程优先级
    int retrievedPriority = thread.getPriority();

    EXPECT_EQ(priority, retrievedPriority);

    priority = 5;
    // 尝试在线程结束后设置和获取优先级
    thread.setPriority(priority);
    EXPECT_EQ(priority, thread.getPriority());

    thread.join();

    EXPECT_TRUE(taskExecuted);

    // priority = 2;
    // 尝试在线程结束后设置和获取优先级
    // thread.setPriority(priority);
    // EXPECT_EQ(priority, thread.getPriority());
#else
    GTEST_SKIP();
#endif
}

TEST(OSALThreadTest, TestOSALThreadSuspendAndResume) {
#if (TestOSALThreadSuspendAndResumeEnabled)
    OSALThread thread;
    std::atomic<bool> taskExecuted(false);

    std::function<void(void *)> taskFunction = [&](void *) {
        OSALSystem::getInstance().sleep_ms(100);
        taskExecuted = true;
    };

    thread.start("TestThread", taskFunction, nullptr, 0, 1024);

    // 暂停线程
    thread.suspend();
    OSALSystem::getInstance().sleep_ms(200);
    EXPECT_FALSE(taskExecuted.load());

    // 恢复线程
    thread.resume();
    thread.join();
    EXPECT_TRUE(taskExecuted.load());

    // 尝试在线程结束后暂停和恢复
    thread.suspend();
    thread.resume();
#else
    GTEST_SKIP();
#endif
}