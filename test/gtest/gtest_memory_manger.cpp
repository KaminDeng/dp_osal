#include "gtest/gtest.h"
#include "osal_memory_manager.h"
#include "osal_mutex.h"
#include "osal_test_framework_config.h"

using namespace osal;

TEST(OSALMemoryManagerTest, TestOSALMemoryManagerAllocate) {
#if (TestOSALMemoryManagerAllocateEnabled)
    osal::OSALMemoryManager memoryManager(128, 10);
    void *ptr = memoryManager.allocate(50);
    EXPECT_TRUE(ptr != nullptr);
    memoryManager.deallocate(ptr);
#else
    GTEST_SKIP();
#endif
}

TEST(OSALMemoryManagerTest, TestOSALMemoryManagerDeallocate) {
#if (TestOSALMemoryManagerDeallocateEnabled)
    osal::OSALMemoryManager memoryManager(128, 10);
    void *ptr = memoryManager.allocate(50);
    EXPECT_TRUE(ptr != nullptr);
    memoryManager.deallocate(ptr);
    // 这里没有直接的方法来验证内存是否被释放，但可以通过工具来检测内存泄漏
#else
    GTEST_SKIP();
#endif
}

TEST(OSALMemoryManagerTest, TestOSALMemoryManagerReallocate) {
#if (TestOSALMemoryManagerReallocateEnabled)
    osal::OSALMemoryManager memoryManager(128, 10);
    void *ptr = memoryManager.allocate(50);
    EXPECT_TRUE(ptr != nullptr);
    void *newPtr = memoryManager.reallocate(ptr, 100);
    EXPECT_TRUE(newPtr != nullptr);
    memoryManager.deallocate(newPtr);
#else
    GTEST_SKIP();
#endif
}

TEST(OSALMemoryManagerTest, TestOSALMemoryManagerAllocateAligned) {
#if (TestOSALMemoryManagerAllocateAlignedEnabled)
    osal::OSALMemoryManager memoryManager(128, 10);
    void *ptr = memoryManager.allocateAligned(30, 64);
    EXPECT_TRUE(ptr != nullptr);
    EXPECT_TRUE(reinterpret_cast<uintptr_t>(ptr) % 64 == 0);
    memoryManager.deallocate(ptr);
#else
    GTEST_SKIP();
#endif
}

TEST(OSALMemoryManagerTest, TestOSALMemoryManagerGetAllocatedSize) {
#if (TestOSALMemoryManagerGetAllocatedSizeEnabled)
    osal::OSALMemoryManager memoryManager;
    void *ptr = memoryManager.allocate(100);
    EXPECT_TRUE(ptr != nullptr);
    size_t size = memoryManager.getAllocatedSize(ptr);
    EXPECT_EQ(size, 100);
    memoryManager.deallocate(ptr);
#else
    GTEST_SKIP();
#endif
}