//
// Created by kamin.deng on 2024/8/23.
//

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