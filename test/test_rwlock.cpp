//
// Created by kamin.deng on 2024/8/23.
//

#include "osal_rwlock.h"
#include "test_framework.h"

using namespace osal;

TEST_CASE(TestOSALRWLockReadLock) {
#if (TestOSALRWLockReadLockEnabled)
    osal::OSALRWLock rwlock;
    rwlock.readLock();
    OSAL_ASSERT_TRUE(rwlock.tryReadLock());
    rwlock.readUnlock();
#endif
    return 0;  // 表示测试通过
}

TEST_CASE(TestOSALRWLockTryReadLock) {
#if (TestOSALRWLockTryReadLockEnabled)
    osal::OSALRWLock rwlock;
    OSAL_ASSERT_TRUE(rwlock.tryReadLock());
    rwlock.readUnlock();
#endif
    return 0;  // 表示测试通过
}

TEST_CASE(TestOSALRWLockReadLockFor) {
#if (TestOSALRWLockReadLockForEnabled)
    osal::OSALRWLock rwlock;
    OSAL_ASSERT_TRUE(rwlock.readLockFor(500));
    rwlock.readUnlock();
#endif
    return 0;  // 表示测试通过
}

TEST_CASE(TestOSALRWLockWriteLock) {
#if (TestOSALRWLockWriteLockEnabled)
    osal::OSALRWLock rwlock;
    rwlock.writeLock();
    OSAL_ASSERT_FALSE(rwlock.tryWriteLock());
    rwlock.writeUnlock();
#endif
    return 0;  // 表示测试通过
}

TEST_CASE(TestOSALRWLockTryWriteLock) {
#if (TestOSALRWLockTryWriteLockEnabled)
    osal::OSALRWLock rwlock;
    OSAL_ASSERT_TRUE(rwlock.tryWriteLock());
    rwlock.writeUnlock();
#endif
    return 0;  // 表示测试通过
}

TEST_CASE(TestOSALRWLockWriteLockFor) {
#if (TestOSALRWLockWriteLockForEnabled)
    osal::OSALRWLock rwlock;
    OSAL_ASSERT_TRUE(rwlock.writeLockFor(500));
    rwlock.writeUnlock();
#endif
    return 0;  // 表示测试通过
}

TEST_CASE(TestOSALRWLockGetReadLockCount) {
#if (TestOSALRWLockGetReadLockCountEnabled)
    osal::OSALRWLock rwlock;
    OSAL_ASSERT_TRUE(rwlock.getReadLockCount() == 0);
#endif
    return 0;  // 表示测试通过
}

TEST_CASE(TestOSALRWLockIsWriteLocked) {
#if (TestOSALRWLockIsWriteLockedEnabled)
    osal::OSALRWLock rwlock;
    OSAL_ASSERT_FALSE(rwlock.isWriteLocked());
#endif
    return 0;  // 表示测试通过
}