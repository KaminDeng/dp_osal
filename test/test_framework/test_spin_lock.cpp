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

#include "osal_spin_lock.h"
#include "osal_system.h"
#include "osal_thread.h"
#include "test_framework.h"

using namespace osal;

TEST_CASE(TestOSALSpinLockLock) {
#if (TestOSALSpinLockLockEnabled)
    osal::OSALSpinLock spinlock;
    spinlock.lock();
    OSAL_ASSERT_TRUE(spinlock.isLocked());
    spinlock.unlock();

    // 多线程测试
    OSALThread thread1, thread2;
    std::atomic<bool> task1Executed(false);
    std::atomic<bool> task2Executed(false);

    thread1.start(
        "Thread1",
        [&](void *) {
            TestOSALPackFunc([&](void *) {
                spinlock.lock();
                OSALSystem::getInstance().sleep_ms(1000);  // 保持锁定状态
                OSAL_ASSERT_TRUE(spinlock.isLocked());
                spinlock.unlock();
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
                OSAL_ASSERT_FALSE(spinlock.tryLock());  // thread1持有锁，thread2应该无法获取锁
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

TEST_CASE(TestOSALSpinLockTryLock) {
#if (TestOSALSpinLockTryLockEnabled)
    osal::OSALSpinLock spinlock;
    OSAL_ASSERT_TRUE(spinlock.tryLock());
    OSAL_ASSERT_TRUE(spinlock.isLocked());
    spinlock.unlock();

    // 多线程测试
    OSALThread thread1, thread2;
    std::atomic<bool> task1Executed(false);
    std::atomic<bool> task2Executed(false);

    thread1.start(
        "Thread1",
        [&](void *) {
            TestOSALPackFunc([&](void *) {
                spinlock.lock();
                OSALSystem::getInstance().sleep_ms(1000);  // 保持锁定状态
                OSAL_ASSERT_TRUE(spinlock.isLocked());
                spinlock.unlock();
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
                OSAL_ASSERT_FALSE(spinlock.tryLock());  // thread1持有锁，thread2应该无法获取锁
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

TEST_CASE(TestOSALSpinLockLockFor) {
#if (TestOSALSpinLockLockForEnabled)
    osal::OSALSpinLock spinlock;
    OSAL_ASSERT_TRUE(spinlock.lockFor(500));
    OSAL_ASSERT_TRUE(spinlock.isLocked());
    spinlock.unlock();

    // 多线程测试
    OSALThread thread1, thread2;
    std::atomic<bool> task1Executed(false);
    std::atomic<bool> task2Executed(false);

    thread1.start(
        "Thread1",
        [&](void *) {
            TestOSALPackFunc([&](void *) {
                spinlock.lock();
                OSALSystem::getInstance().sleep_ms(1000);  // 保持锁定状态
                OSAL_ASSERT_TRUE(spinlock.isLocked());
                spinlock.unlock();
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
                OSAL_ASSERT_FALSE(spinlock.lockFor(500));  // thread1持有锁，thread2应该无法获取锁
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

TEST_CASE(TestOSALSpinLockUnlock) {
#if (TestOSALSpinLockUnlockEnabled)
    osal::OSALSpinLock spinlock;
    spinlock.lock();
    OSAL_ASSERT_TRUE(spinlock.isLocked());
    spinlock.unlock();
    OSAL_ASSERT_FALSE(spinlock.isLocked());

    // 多线程测试
    OSALThread thread1, thread2;
    std::atomic<bool> task1Executed(false);
    std::atomic<bool> task2Executed(false);

    thread1.start(
        "Thread1",
        [&](void *) {
            TestOSALPackFunc([&](void *) {
                spinlock.lock();
                OSALSystem::getInstance().sleep_ms(1000);  // 保持锁定状态
                OSAL_ASSERT_TRUE(spinlock.isLocked());
                spinlock.unlock();
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
                OSAL_ASSERT_FALSE(spinlock.tryLock());  // thread1持有锁，thread2应该无法获取锁
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

TEST_CASE(TestOSALSpinLockIsLocked) {
#if (TestOSALSpinLockIsLockedEnabled)
    osal::OSALSpinLock spinlock;
    OSAL_ASSERT_FALSE(spinlock.isLocked());
    spinlock.lock();
    OSAL_ASSERT_TRUE(spinlock.isLocked());
    spinlock.unlock();
    OSAL_ASSERT_FALSE(spinlock.isLocked());

    // 多线程测试
    OSALThread thread1, thread2;
    std::atomic<bool> task1Executed(false);
    std::atomic<bool> task2Executed(false);

    thread1.start(
        "Thread1",
        [&](void *) {
            TestOSALPackFunc([&](void *) {
                spinlock.lock();
                OSALSystem::getInstance().sleep_ms(1000);  // 保持锁定状态
                OSAL_ASSERT_TRUE(spinlock.isLocked());
                spinlock.unlock();
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
                OSAL_ASSERT_FALSE(spinlock.tryLock());  // thread1持有锁，thread2应该无法获取锁
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