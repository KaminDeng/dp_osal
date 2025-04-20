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
#include "osal_semaphore.h"
#include "osal_test_framework_config.h"

using namespace osal;

TEST(OSALSemaphoreTest, TestOSALSemaphoreInit) {
#if (TestOSALSemaphoreInitEnabled)
    osal::OSALSemaphore semaphore;
    semaphore.init(5);
    EXPECT_EQ(semaphore.getValue(), 5);
#else
    GTEST_SKIP();
#endif
}

TEST(OSALSemaphoreTest, TestOSALSemaphoreWaitSignal) {
#if (TestOSALSemaphoreWaitSignalEnabled)
    osal::OSALSemaphore semaphore;
    semaphore.init(1);
    semaphore.wait();
    EXPECT_EQ(semaphore.getValue(), 0);
    semaphore.signal();
    EXPECT_EQ(semaphore.getValue(), 1);
#else
    GTEST_SKIP();
#endif
}

TEST(OSALSemaphoreTest, TestOSALSemaphoreTryWait) {
#if (TestOSALSemaphoreTryWaitEnabled)
    osal::OSALSemaphore semaphore;
    semaphore.init(1);
    EXPECT_TRUE(semaphore.tryWait());
    EXPECT_FALSE(semaphore.tryWait());
    semaphore.signal();
    EXPECT_TRUE(semaphore.tryWait());
#else
    GTEST_SKIP();
#endif
}

TEST(OSALSemaphoreTest, TestOSALSemaphoreTryWaitFor) {
#if (TestOSALSemaphoreTryWaitForEnabled)
    osal::OSALSemaphore semaphore;
    semaphore.init(1);
    EXPECT_TRUE(semaphore.tryWaitFor(500));
    EXPECT_FALSE(semaphore.tryWaitFor(500));
    semaphore.signal();
    EXPECT_TRUE(semaphore.tryWaitFor(500));
#else
    GTEST_SKIP();
#endif
}

TEST(OSALSemaphoreTest, TestOSALSemaphoreGetValue) {
#if (TestOSALSemaphoreGetValueEnabled)
    osal::OSALSemaphore semaphore;
    semaphore.init(3);
    EXPECT_EQ(semaphore.getValue(), 3);
    semaphore.wait();
    EXPECT_EQ(semaphore.getValue(), 2);
    semaphore.signal();
    EXPECT_EQ(semaphore.getValue(), 3);
#else
    GTEST_SKIP();
#endif
}