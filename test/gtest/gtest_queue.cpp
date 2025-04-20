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
#include "osal_queue.h"
#include "osal_test_framework_config.h"

using namespace osal;

TEST(OSALMessageQueueTest, TestOSALMessageQueueSendReceive) {
#if (TestOSALMessageQueueSendReceiveEnabled)
    osal::OSALMessageQueue<int> queue;
    queue.send(42);
    int message = queue.receive();
    EXPECT_EQ(message, 42);
#else
    GTEST_SKIP();
#endif
}

TEST(OSALMessageQueueTest, TestOSALMessageQueueTryReceive) {
#if (TestOSALMessageQueueTryReceiveEnabled)
    osal::OSALMessageQueue<int> queue;
    int message;
    EXPECT_FALSE(queue.tryReceive(message));
    queue.send(42);
    EXPECT_TRUE(queue.tryReceive(message));
    EXPECT_EQ(message, 42);
#else
    GTEST_SKIP();
#endif
}

TEST(OSALMessageQueueTest, TestOSALMessageQueueReceiveFor) {
#if (TestOSALMessageQueueReceiveForEnabled)
    osal::OSALMessageQueue<int> queue;
    int message;
    EXPECT_FALSE(queue.receiveFor(message, 100));
    queue.send(42);
    EXPECT_TRUE(queue.receiveFor(message, 100));
    EXPECT_EQ(message, 42);
#else
    GTEST_SKIP();
#endif
}

TEST(OSALMessageQueueTest, TestOSALMessageQueueSize) {
#if (TestOSALMessageQueueSizeEnabled)
    osal::OSALMessageQueue<int> queue;
    EXPECT_EQ(queue.size(), 0);
    queue.send(42);
    EXPECT_EQ(queue.size(), 1);
    queue.receive();
    EXPECT_EQ(queue.size(), 0);
#else
    GTEST_SKIP();
#endif
}

TEST(OSALMessageQueueTest, TestOSALMessageQueueClear) {
#if (TestOSALMessageQueueClearEnabled)
    osal::OSALMessageQueue<int> queue;
    queue.send(42);
    queue.send(43);
    EXPECT_EQ(queue.size(), 2);
    queue.clear();
    EXPECT_EQ(queue.size(), 0);
#else
    GTEST_SKIP();
#endif
}