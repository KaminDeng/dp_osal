#include <atomic>

#include "gtest/gtest.h"
#include "osal_lockguard.h"
#include "osal_test_framework_config.h"
#include "osal_thread.h"

using namespace osal;

TEST(OSALLockGuardTest, TestOSALLockGuardConstructor) {
#if (TestOSALLockGuardConstructorEnabled)
    osal::OSALMutex mutex;
    {
        osal::OSALLockGuard lockGuard(mutex);
        EXPECT_TRUE(lockGuard.isLocked());
    }  // lockGuard超出作用域，应该自动解锁
#else
    GTEST_SKIP();
#endif
}

TEST(OSALLockGuardTest, TestOSALLockGuardDestructor) {
#if (TestOSALLockGuardDestructorEnabled)
    osal::OSALMutex mutex;
    {
        osal::OSALLockGuard lockGuard(mutex);
        EXPECT_TRUE(lockGuard.isLocked());
    }  // lockGuard超出作用域，应该自动解锁

    // 再次尝试锁定，确保之前的锁已解锁
    EXPECT_TRUE(mutex.lock());
    EXPECT_TRUE(mutex.unlock());
#else
    GTEST_SKIP();
#endif
}

TEST(OSALLockGuardTest, TestOSALLockGuardIsLocked) {
#if (TestOSALLockGuardIsLockedEnabled)
    osal::OSALMutex mutex;
    osal::OSALLockGuard lockGuard(mutex);
    EXPECT_TRUE(lockGuard.isLocked());
#else
    GTEST_SKIP();
#endif
}

TEST(OSALLockGuardTest, TestOSALLockGuardMultiThread) {
#if (TestOSALLockGuardMultiThreadEnabled)
    OSALThread thread;
    osal::OSALMutex mutex;
    std::atomic<bool> taskExecuted(false);

    auto workerTask = [&](void *) {
        osal::OSALLockGuard lockGuard(mutex);

        // 封装成函数，避免在无返回值的函数中该宏return非零值，导致编译warning
        auto wrapper_func = [&]() {
            EXPECT_TRUE(lockGuard.isLocked());
            return 0;
        };
        wrapper_func();
        taskExecuted = true;
    };

    thread.start("TestThread", workerTask, nullptr, 0, 1024);

    thread.join();
    EXPECT_TRUE(taskExecuted.load());
#else
    GTEST_SKIP();
#endif
}