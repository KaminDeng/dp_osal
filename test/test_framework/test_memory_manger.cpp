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
    void *ptr = memoryManager.allocateAligned(30, 64);
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
