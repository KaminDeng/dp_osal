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