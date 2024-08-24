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