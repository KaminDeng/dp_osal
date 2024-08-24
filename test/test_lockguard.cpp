//
// Created by kamin.deng on 2024/8/23.
//
#include <atomic>

#include "osal_lockguard.h"
#include "osal_thread.h"
#include "test_framework.h"

using namespace osal;

TEST_CASE(TestOSALLockGuardConstructor) {
#if (TestOSALLockGuardConstructorEnabled)
    osal::OSALMutex mutex;
    {
        osal::OSALLockGuard lockGuard(mutex);
        OSAL_ASSERT_TRUE(lockGuard.isLocked());
    }  // lockGuard超出作用域，应该自动解锁
#endif
    return 0;  // 表示测试通过
}

TEST_CASE(TestOSALLockGuardDestructor) {
#if (TestOSALLockGuardDestructorEnabled)
    osal::OSALMutex mutex;
    {
        osal::OSALLockGuard lockGuard(mutex);
        OSAL_ASSERT_TRUE(lockGuard.isLocked());
    }  // lockGuard超出作用域，应该自动解锁

    // 再次尝试锁定，确保之前的锁已解锁
    OSAL_ASSERT_TRUE(mutex.lock());
    OSAL_ASSERT_TRUE(mutex.unlock());
#endif
    return 0;  // 表示测试通过
}

TEST_CASE(TestOSALLockGuardIsLocked) {
#if (TestOSALLockGuardIsLockedEnabled)
    osal::OSALMutex mutex;
    osal::OSALLockGuard lockGuard(mutex);
    OSAL_ASSERT_TRUE(lockGuard.isLocked());
#endif
    return 0;  // 表示测试通过
}

TEST_CASE(TestOSALLockGuardMultiThread) {
#if (TestOSALLockGuardMultiThreadEnabled)
    OSALThread thread;
    osal::OSALMutex mutex;
    std::atomic<bool> taskExecuted(false);

    auto workerTask = [&](void *) {
        osal::OSALLockGuard lockGuard(mutex);

        // 封装成函数，避免在无返回值的函数中该宏return非零值，导致编译warning
        auto wrapper_func = [&]() {
            OSAL_ASSERT_TRUE(lockGuard.isLocked());
            return 0;
        };
        wrapper_func();
        taskExecuted = true;
    };

    thread.start("TestThread", workerTask);

    thread.join();
    OSAL_ASSERT_TRUE(taskExecuted.load());
#endif
    return 0;  // 表示测试通过
}