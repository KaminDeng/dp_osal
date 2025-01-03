#include "gtest/gtest.h"
#include "osal_rwlock.h"
#include "osal_test_framework_config.h"

using namespace osal;

TEST(OSALRWLockTest, TestOSALRWLockReadLock) {
#if (TestOSALRWLockReadLockEnabled)
    osal::OSALRWLock rwlock;
    rwlock.readLock();
    EXPECT_TRUE(rwlock.tryReadLock());
    rwlock.readUnlock();
#else
    GTEST_SKIP();
#endif
}

TEST(OSALRWLockTest, TestOSALRWLockTryReadLock) {
#if (TestOSALRWLockTryReadLockEnabled)
    osal::OSALRWLock rwlock;
    EXPECT_TRUE(rwlock.tryReadLock());
    rwlock.readUnlock();
#else
    GTEST_SKIP();
#endif
}

TEST(OSALRWLockTest, TestOSALRWLockReadLockFor) {
#if (TestOSALRWLockReadLockForEnabled)
    osal::OSALRWLock rwlock;
    EXPECT_TRUE(rwlock.readLockFor(500));
    rwlock.readUnlock();
#else
    GTEST_SKIP();
#endif
}

TEST(OSALRWLockTest, TestOSALRWLockWriteLock) {
#if (TestOSALRWLockWriteLockEnabled)
    osal::OSALRWLock rwlock;
    rwlock.writeLock();
    EXPECT_FALSE(rwlock.tryWriteLock());
    rwlock.writeUnlock();
#else
    GTEST_SKIP();
#endif
}

TEST(OSALRWLockTest, TestOSALRWLockTryWriteLock) {
#if (TestOSALRWLockTryWriteLockEnabled)
    osal::OSALRWLock rwlock;
    EXPECT_TRUE(rwlock.tryWriteLock());
    rwlock.writeUnlock();
#else
    GTEST_SKIP();
#endif
}

TEST(OSALRWLockTest, TestOSALRWLockWriteLockFor) {
#if (TestOSALRWLockWriteLockForEnabled)
    osal::OSALRWLock rwlock;
    EXPECT_TRUE(rwlock.writeLockFor(500));
    rwlock.writeUnlock();
#else
    GTEST_SKIP();
#endif
}

TEST(OSALRWLockTest, TestOSALRWLockGetReadLockCount) {
#if (TestOSALRWLockGetReadLockCountEnabled)
    osal::OSALRWLock rwlock;
    EXPECT_EQ(rwlock.getReadLockCount(), 0);
#else
    GTEST_SKIP();
#endif
}

TEST(OSALRWLockTest, TestOSALRWLockIsWriteLocked) {
#if (TestOSALRWLockIsWriteLockedEnabled)
    osal::OSALRWLock rwlock;
    EXPECT_FALSE(rwlock.isWriteLocked());
#else
    GTEST_SKIP();
#endif
}