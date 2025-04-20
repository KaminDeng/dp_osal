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

#include "osal_spin_lock.h"
#include "osal_system.h"
#include "osal_test_framework_config.h"
#include "osal_thread.h"

using namespace osal;

TEST(TestOSALSpinLock, Lock) {
#if (TestOSALSpinLockLockEnabled)
    osal::OSALSpinLock spinlock;
    spinlock.lock();
    EXPECT_TRUE(spinlock.isLocked());
    spinlock.unlock();

    // Multi-threaded test
    OSALThread thread1, thread2;
    std::atomic<bool> task1Executed(false);
    std::atomic<bool> task2Executed(false);

    thread1.start(
        "Thread1",
        [&](void *) {
            spinlock.lock();
            OSALSystem::getInstance().sleep_ms(1000);  // Keep locked
            EXPECT_TRUE(spinlock.isLocked());
            spinlock.unlock();
            task1Executed = true;
        },
        nullptr, 0, 1024);

    OSALSystem::getInstance().sleep_ms(100);  // Ensure thread1 has the lock

    thread2.start(
        "Thread2",
        [&](void *) {
            EXPECT_FALSE(spinlock.tryLock());  // thread1 holds the lock, thread2 should not acquire it
            task2Executed = true;
        },
        nullptr, 0, 1024);

    thread1.join();
    thread2.join();

    EXPECT_TRUE(task1Executed);
    EXPECT_TRUE(task2Executed);
#else
    GTEST_SKIP();
#endif
}

TEST(TestOSALSpinLock, TryLock) {
#if (TestOSALSpinLockTryLockEnabled)
    osal::OSALSpinLock spinlock;
    EXPECT_TRUE(spinlock.tryLock());
    EXPECT_TRUE(spinlock.isLocked());
    spinlock.unlock();

    // Multi-threaded test
    OSALThread thread1, thread2;
    std::atomic<bool> task1Executed(false);
    std::atomic<bool> task2Executed(false);

    thread1.start(
        "Thread1",
        [&](void *) {
            spinlock.lock();
            OSALSystem::getInstance().sleep_ms(1000);  // Keep locked
            EXPECT_TRUE(spinlock.isLocked());
            spinlock.unlock();
            task1Executed = true;
        },
        nullptr, 0, 1024);

    OSALSystem::getInstance().sleep_ms(100);  // Ensure thread1 has the lock

    thread2.start(
        "Thread2",
        [&](void *) {
            EXPECT_FALSE(spinlock.tryLock());  // thread1 holds the lock, thread2 should not acquire it
            task2Executed = true;
        },
        nullptr, 0, 1024);

    thread1.join();
    thread2.join();

    EXPECT_TRUE(task1Executed);
    EXPECT_TRUE(task2Executed);
#else
    GTEST_SKIP();
#endif
}

TEST(TestOSALSpinLock, LockFor) {
#if (TestOSALSpinLockLockForEnabled)
    osal::OSALSpinLock spinlock;
    EXPECT_TRUE(spinlock.lockFor(500));
    EXPECT_TRUE(spinlock.isLocked());
    spinlock.unlock();

    // Multi-threaded test
    OSALThread thread1, thread2;
    std::atomic<bool> task1Executed(false);
    std::atomic<bool> task2Executed(false);

    thread1.start(
        "Thread1",
        [&](void *) {
            spinlock.lock();
            OSALSystem::getInstance().sleep_ms(1000);  // Keep locked
            EXPECT_TRUE(spinlock.isLocked());
            spinlock.unlock();
            task1Executed = true;
        },
        nullptr, 0, 1024);

    OSALSystem::getInstance().sleep_ms(100);  // Ensure thread1 has the lock

    thread2.start(
        "Thread2",
        [&](void *) {
            EXPECT_FALSE(spinlock.lockFor(500));  // thread1 holds the lock, thread2 should not acquire it
            task2Executed = true;
        },
        nullptr, 0, 1024);

    thread1.join();
    thread2.join();

    EXPECT_TRUE(task1Executed);
    EXPECT_TRUE(task2Executed);
#else
    GTEST_SKIP();
#endif
}

TEST(TestOSALSpinLock, Unlock) {
#if (TestOSALSpinLockUnlockEnabled)
    osal::OSALSpinLock spinlock;
    spinlock.lock();
    EXPECT_TRUE(spinlock.isLocked());
    spinlock.unlock();
    EXPECT_FALSE(spinlock.isLocked());

    // Multi-threaded test
    OSALThread thread1, thread2;
    std::atomic<bool> task1Executed(false);
    std::atomic<bool> task2Executed(false);

    thread1.start(
        "Thread1",
        [&](void *) {
            spinlock.lock();
            OSALSystem::getInstance().sleep_ms(1000);  // Keep locked
            EXPECT_TRUE(spinlock.isLocked());
            spinlock.unlock();
            task1Executed = true;
        },
        nullptr, 0, 1024);

    OSALSystem::getInstance().sleep_ms(100);  // Ensure thread1 has the lock

    thread2.start(
        "Thread2",
        [&](void *) {
            EXPECT_FALSE(spinlock.tryLock());  // thread1 holds the lock, thread2 should not acquire it
            task2Executed = true;
        },
        nullptr, 0, 1024);

    thread1.join();
    thread2.join();

    EXPECT_TRUE(task1Executed);
    EXPECT_TRUE(task2Executed);
#else
    GTEST_SKIP();
#endif
}

TEST(TestOSALSpinLock, IsLocked) {
#if (TestOSALSpinLockIsLockedEnabled)
    osal::OSALSpinLock spinlock;
    EXPECT_FALSE(spinlock.isLocked());
    spinlock.lock();
    EXPECT_TRUE(spinlock.isLocked());
    spinlock.unlock();
    EXPECT_FALSE(spinlock.isLocked());

    // Multi-threaded test
    OSALThread thread1, thread2;
    std::atomic<bool> task1Executed(false);
    std::atomic<bool> task2Executed(false);

    thread1.start(
        "Thread1",
        [&](void *) {
            spinlock.lock();
            OSALSystem::getInstance().sleep_ms(1000);  // Keep locked
            EXPECT_TRUE(spinlock.isLocked());
            spinlock.unlock();
            task1Executed = true;
        },
        nullptr, 0, 1024);

    OSALSystem::getInstance().sleep_ms(100);  // Ensure thread1 has the lock

    thread2.start(
        "Thread2",
        [&](void *) {
            EXPECT_FALSE(spinlock.tryLock());  // thread1 holds the lock, thread2 should not acquire it
            task2Executed = true;
        },
        nullptr, 0, 1024);

    thread1.join();
    thread2.join();

    EXPECT_TRUE(task1Executed);
    EXPECT_TRUE(task2Executed);
#else
    GTEST_SKIP();
#endif
}