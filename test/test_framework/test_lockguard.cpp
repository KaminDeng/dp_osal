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

    thread.start("TestThread", workerTask, nullptr, 0, 1024);

    thread.join();
    OSAL_ASSERT_TRUE(taskExecuted.load());
#endif
    return 0;  // 表示测试通过
}