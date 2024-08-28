//
// Created by kamin.deng on 2024/8/23.
//

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
    OSAL_ASSERT_TRUE(mutex.tryLock());
    OSAL_ASSERT_TRUE(mutex.unlock());

    // 尝试在锁定状态下再次锁定
    OSAL_ASSERT_TRUE(mutex.lock());
    OSAL_ASSERT_FALSE(mutex.tryLock());
    OSAL_ASSERT_TRUE(mutex.unlock());
#endif
    return 0;  // 表示测试通过
}

TEST_CASE(TestOSALMutexTryLockFor) {
#if (TestOSALMutexTryLockForEnabled)
    osal::OSALMutex mutex;
    OSAL_ASSERT_TRUE(mutex.tryLockFor(500));
    OSAL_ASSERT_TRUE(mutex.unlock());

    // 尝试在锁定状态下再次锁定
    OSAL_ASSERT_TRUE(mutex.lock());
    OSAL_ASSERT_FALSE(mutex.tryLockFor(500));
    OSAL_ASSERT_TRUE(mutex.unlock());
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