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