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

#include "osal_queue.h"
#include "test_framework.h"

using namespace osal;

TEST_CASE(TestOSALMessageQueueSendReceive) {
#if (TestOSALMessageQueueSendReceiveEnabled)
    osal::OSALMessageQueue<int> queue;
    queue.send(42);
    int message = queue.receive();
    OSAL_ASSERT_EQ(message, 42);
#endif
    return 0;  // 表示测试通过
}

TEST_CASE(TestOSALMessageQueueTryReceive) {
#if (TestOSALMessageQueueTryReceiveEnabled)
    osal::OSALMessageQueue<int> queue;
    int message;
    OSAL_ASSERT_FALSE(queue.tryReceive(message));
    queue.send(42);
    OSAL_ASSERT_TRUE(queue.tryReceive(message));
    OSAL_ASSERT_EQ(message, 42);
#endif
    return 0;  // 表示测试通过
}

TEST_CASE(TestOSALMessageQueueReceiveFor) {
#if (TestOSALMessageQueueReceiveForEnabled)
    osal::OSALMessageQueue<int> queue;
    int message;
    OSAL_ASSERT_FALSE(queue.receiveFor(message, (100)));
    queue.send(42);
    OSAL_ASSERT_TRUE(queue.receiveFor(message, (100)));
    OSAL_ASSERT_EQ(message, 42);
#endif
    return 0;  // 表示测试通过
}

TEST_CASE(TestOSALMessageQueueSize) {
#if (TestOSALMessageQueueSizeEnabled)
    osal::OSALMessageQueue<int> queue;
    OSAL_ASSERT_EQ(queue.size(), 0);
    queue.send(42);
    OSAL_ASSERT_EQ(queue.size(), 1);
    queue.receive();
    OSAL_ASSERT_EQ(queue.size(), 0);
#endif
    return 0;  // 表示测试通过
}

TEST_CASE(TestOSALMessageQueueClear) {
#if (TestOSALMessageQueueClearEnabled)
    osal::OSALMessageQueue<int> queue;
    queue.send(42);
    queue.send(43);
    OSAL_ASSERT_EQ(queue.size(), 2);
    queue.clear();
    OSAL_ASSERT_EQ(queue.size(), 0);
#endif
    return 0;  // 表示测试通过
}