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

#include "osal_semaphore.h"
#include "test_framework.h"

using namespace osal;

TEST_CASE(TestOSALSemaphoreInit) {
#if (TestOSALSemaphoreInitEnabled)
    osal::OSALSemaphore semaphore;
    semaphore.init(5);
    OSAL_ASSERT_EQ(semaphore.getValue(), 5);
#endif
    return 0;  // 表示测试通过
}

TEST_CASE(TestOSALSemaphoreWaitSignal) {
#if (TestOSALSemaphoreWaitSignalEnabled)
    osal::OSALSemaphore semaphore;
    semaphore.init(1);
    semaphore.wait();
    OSAL_ASSERT_EQ(semaphore.getValue(), 0);
    semaphore.signal();
    OSAL_ASSERT_EQ(semaphore.getValue(), 1);
#endif
    return 0;  // 表示测试通过
}

TEST_CASE(TestOSALSemaphoreTryWait) {
#if (TestOSALSemaphoreTryWaitEnabled)
    osal::OSALSemaphore semaphore;
    semaphore.init(1);
    OSAL_ASSERT_TRUE(semaphore.tryWait());
    OSAL_ASSERT_FALSE(semaphore.tryWait());
    semaphore.signal();
    OSAL_ASSERT_TRUE(semaphore.tryWait());
#endif
    return 0;  // 表示测试通过
}

TEST_CASE(TestOSALSemaphoreTryWaitFor) {
#if (TestOSALSemaphoreTryWaitForEnabled)
    osal::OSALSemaphore semaphore;
    semaphore.init(1);
    OSAL_ASSERT_TRUE(semaphore.tryWaitFor(500));
    OSAL_ASSERT_FALSE(semaphore.tryWaitFor(500));
    semaphore.signal();
    OSAL_ASSERT_TRUE(semaphore.tryWaitFor(500));
#endif
    return 0;  // 表示测试通过
}

TEST_CASE(TestOSALSemaphoreGetValue) {
#if (TestOSALSemaphoreGetValueEnabled)
    osal::OSALSemaphore semaphore;
    semaphore.init(3);
    OSAL_ASSERT_EQ(semaphore.getValue(), 3);
    semaphore.wait();
    OSAL_ASSERT_EQ(semaphore.getValue(), 2);
    semaphore.signal();
    OSAL_ASSERT_EQ(semaphore.getValue(), 3);
#endif
    return 0;  // 表示测试通过
}