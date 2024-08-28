//
// Created by kamin.deng on 2024/8/23.
//
#include "osal_memory_manager.h"
#include "osal_mutex.h"
#include "test_framework.h"

using namespace osal;

TEST_CASE(TestOSALMemoryManagerAllocate) {
#if (TestOSALMemoryManagerAllocateEnabled)
    osal::OSALMemoryManager memoryManager(128, 10);
    void *ptr = memoryManager.allocate(50);
    OSAL_ASSERT_TRUE(ptr != nullptr);
    memoryManager.deallocate(ptr);
#endif
    return 0;  // 表示测试通过
}

TEST_CASE(TestOSALMemoryManagerDeallocate) {
#if (TestOSALMemoryManagerDeallocateEnabled)
    osal::OSALMemoryManager memoryManager(128, 10);
    void *ptr = memoryManager.allocate(50);
    OSAL_ASSERT_TRUE(ptr != nullptr);
    memoryManager.deallocate(ptr);
    // 这里没有直接的方法来验证内存是否被释放，但可以通过工具来检测内存泄漏
#endif
    return 0;  // 表示测试通过
}

TEST_CASE(TestOSALMemoryManagerReallocate) {
#if (TestOSALMemoryManagerReallocateEnabled)
    osal::OSALMemoryManager memoryManager(128, 10);
    void *ptr = memoryManager.allocate(50);
    OSAL_ASSERT_TRUE(ptr != nullptr);
    void *newPtr = memoryManager.reallocate(ptr, 100);
    OSAL_ASSERT_TRUE(newPtr != nullptr);
    memoryManager.deallocate(newPtr);
#endif
    return 0;  // 表示测试通过
}

TEST_CASE(TestOSALMemoryManagerAllocateAligned) {
#if (TestOSALMemoryManagerAllocateAlignedEnabled)
    osal::OSALMemoryManager memoryManager(128, 10);
    void *ptr = memoryManager.allocateAligned(50, 64);
    OSAL_ASSERT_TRUE(ptr != nullptr);
    OSAL_ASSERT_TRUE(reinterpret_cast<uintptr_t>(ptr) % 64 == 0);
    memoryManager.deallocate(ptr);
#endif
    return 0;  // 表示测试通过
}

TEST_CASE(TestOSALMemoryManagerGetAllocatedSize) {
#if (TestOSALMemoryManagerGetAllocatedSizeEnabled)
    osal::OSALMemoryManager memoryManager;
    void *ptr = memoryManager.allocate(100);
    OSAL_ASSERT_TRUE(ptr != nullptr);
    size_t size = memoryManager.getAllocatedSize(ptr);
    OSAL_ASSERT_EQ(size, 100);
    memoryManager.deallocate(ptr);
#endif
    return 0;  // 表示测试通过
}
