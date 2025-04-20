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

#include "osal_mutex.h"
#include "osal_system.h"
#include "osal_thread.h"
#include "test_framework.h"

using namespace osal;

TEST_CASE(TestOSALMutexLock) {
#if (TestOSALMutexLockEnabled)
    osal::OSALMutex mutex;
    OSAL_ASSERT_TRUE(mutex.lock());
    OSAL_ASSERT_TRUE(mutex.unlock());

    // 尝试再次锁定和解锁
    OSAL_ASSERT_TRUE(mutex.lock());
    OSAL_ASSERT_TRUE(mutex.unlock());
#endif
    return 0;  // 表示测试通过
}

TEST_CASE(TestOSALMutexUnlock) {
#if (TestOSALMutexUnlockEnabled)
    osal::OSALMutex mutex;
    OSAL_ASSERT_TRUE(mutex.lock());
    OSAL_ASSERT_TRUE(mutex.unlock());

    // 尝试解锁未锁定的互斥锁（假设此操作是安全的）
//    OSAL_ASSERT_FALSE(mutex.unlock());
#endif
    return 0;  // 表示测试通过
}

TEST_CASE(TestOSALMutexTryLock) {
#if (TestOSALMutexTryLockEnabled)
    osal::OSALMutex mutex;

    // 在同一个线程中测试递归锁的行为
    OSAL_ASSERT_TRUE(mutex.tryLock());
    OSAL_ASSERT_TRUE(mutex.tryLock());  // 递归锁应该允许同一个线程再次获取锁
    OSAL_ASSERT_TRUE(mutex.unlock());
    OSAL_ASSERT_TRUE(mutex.unlock());

    // 在不同线程中测试非递归锁的行为
    OSALThread thread1, thread2;
    std::atomic<bool> task1Executed(false);
    std::atomic<bool> task2Executed(false);

    thread1.start(
        "Thread1",
        [&](void *) {
            TestOSALPackFunc([&](void *) {
                OSAL_ASSERT_TRUE(mutex.lock());
                OSALSystem::getInstance().sleep_ms(1000);  // 保持锁定状态
                OSAL_ASSERT_TRUE(mutex.unlock());
                task1Executed = true;
                return 0;
            });
        },
        nullptr, 0, 1024);

    OSALSystem::getInstance().sleep_ms(100);  // 确保thread1已经获取锁

    thread2.start(
        "Thread2",
        [&](void *) {
            TestOSALPackFunc([&](void *) {
                OSAL_ASSERT_FALSE(mutex.tryLock());  // thread1持有锁，thread2应该无法获取锁
                task2Executed = true;
                return 0;
            });
        },
        nullptr, 0, 1024);

    thread1.join();
    thread2.join();

    OSAL_ASSERT_TRUE(task1Executed);
    OSAL_ASSERT_TRUE(task2Executed);
#endif
    return 0;  // 表示测试通过
}

TEST_CASE(TestOSALMutexTryLockFor) {
#if (TestOSALMutexTryLockForEnabled)
    osal::OSALMutex mutex;

    // 在同一个线程中测试递归锁的行为
    OSAL_ASSERT_TRUE(mutex.tryLockFor(500));
    OSAL_ASSERT_TRUE(mutex.tryLockFor(500));  // 递归锁应该允许同一个线程再次获取锁
    OSAL_ASSERT_TRUE(mutex.unlock());
    OSAL_ASSERT_TRUE(mutex.unlock());

    // 在不同线程中测试非递归锁的行为
    OSALThread thread1, thread2;
    std::atomic<bool> task1Executed(false);
    std::atomic<bool> task2Executed(false);

    thread1.start(
        "Thread1",
        [&](void *) {
            TestOSALPackFunc([&](void *) {
                OSAL_ASSERT_TRUE(mutex.lock());
                OSALSystem::getInstance().sleep_ms(1000);  // 保持锁定状态
                OSAL_ASSERT_TRUE(mutex.unlock());
                task1Executed = true;
                return 0;
            });
        },
        nullptr, 0, 1024);

    OSALSystem::getInstance().sleep_ms(100);  // 确保thread1已经获取锁

    thread2.start(
        "Thread2",
        [&](void *) {
            TestOSALPackFunc([&](void *) {
                OSAL_ASSERT_FALSE(mutex.tryLockFor(500));  // thread1持有锁，thread2应该无法获取锁
                task2Executed = true;
                return 0;
            });
        },
        nullptr, 0, 1024);

    thread1.join();
    thread2.join();

    OSAL_ASSERT_TRUE(task1Executed);
    OSAL_ASSERT_TRUE(task2Executed);
#endif
    return 0;  // 表示测试通过
}

TEST_CASE(TestOSALMutexLockAndUnlock) {
#if (TestOSALMutexLockAndUnlockEnabled)
    osal::OSALMutex mutex;
    std::atomic<bool> taskExecuted(false);

    auto taskFunction = [&](void *) {
        mutex.lock();
        OSALSystem::getInstance().sleep_ms(500);
        taskExecuted = true;
        mutex.unlock();
    };

    OSALThread thread;
    thread.start("TestThread", taskFunction, nullptr, 0, 1024);

    // 尝试锁定互斥锁
    OSALSystem::getInstance().sleep_ms(100);
    OSAL_ASSERT_FALSE(mutex.tryLock());

    // 等待线程执行完毕
    thread.join();

    // 检查任务是否执行
    OSAL_ASSERT_TRUE(taskExecuted.load());

    // 尝试再次锁定互斥锁
    OSAL_ASSERT_TRUE(mutex.tryLock());
    mutex.unlock();
#endif
    return 0;  // 表示测试通过
}