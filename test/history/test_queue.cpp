//
// Created by kamin.deng on 2024/8/23.
//
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