//
// Created by kamin.deng on 2024/8/23.
//

#include "osal_spin_lock.h"
#include "test_framework.h"

using namespace osal;

TEST_CASE(TestOSALSpinLockLock) {
#if (TestOSALSpinLockLockEnabled)
    osal::OSALSpinLock spinlock;
    spinlock.lock();
    OSAL_ASSERT_TRUE(spinlock.isLocked());
    spinlock.unlock();
#endif
    return 0;  // 表示测试通过
}

TEST_CASE(TestOSALSpinLockTryLock) {
#if (TestOSALSpinLockTryLockEnabled)
    osal::OSALSpinLock spinlock;
    OSAL_ASSERT_TRUE(spinlock.tryLock());
    OSAL_ASSERT_TRUE(spinlock.isLocked());
    spinlock.unlock();
#endif
    return 0;  // 表示测试通过
}

TEST_CASE(TestOSALSpinLockLockFor) {
#if (TestOSALSpinLockLockForEnabled)
    osal::OSALSpinLock spinlock;
    OSAL_ASSERT_TRUE(spinlock.lockFor(500));
    OSAL_ASSERT_TRUE(spinlock.isLocked());
    spinlock.unlock();
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
#endif
    return 0;  // 表示测试通过
}