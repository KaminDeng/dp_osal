//
// Created by kamin.deng on 2024/8/22.
//
#ifndef __OSAL_MEMORY_MANAGER_H__
#define __OSAL_MEMORY_MANAGER_H__

#include <string.h>

#include "cmsis_os.h"
#include "interface_memory_manager.h"
#include "osal_debug.h"

namespace osal {

class OSALMemoryManager : public IMemoryManager {
public:
    OSALMemoryManager() {
        // 初始化内存池
        memPoolId = osMemoryPoolNew(POOL_BLOCK_COUNT, POOL_BLOCK_SIZE, NULL);
        if (memPoolId == NULL) {
            OSAL_LOGE("Failed to create memory pool\n");
            // 处理内存池创建失败的情况
        }
    }

    ~OSALMemoryManager() override {
        if (memPoolId != NULL) {
            osMemoryPoolDelete(memPoolId);
        }
    }

    void *allocate(size_t size) override {
        if (size > POOL_BLOCK_SIZE) {
            OSAL_LOGE("Requested size exceeds pool block size\n");
            return nullptr;
        }
        void *ptr = osMemoryPoolAlloc(memPoolId, 0);
        if (!ptr) {
            OSAL_LOGE("Failed to allocate memory from pool\n");
            return nullptr;
        }
        OSAL_LOGD("Allocated %zu bytes from pool\n", size);
        return ptr;
    }

    void deallocate(void *ptr) override {
        if (osMemoryPoolFree(memPoolId, ptr) != osOK) {
            OSAL_LOGE("Failed to deallocate memory to pool\n");
        } else {
            OSAL_LOGD("Deallocated memory to pool\n");
        }
    }

    void *reallocate(void *ptr, size_t newSize) override {
        // CMSIS-RTOS2 不支持直接重新分配内存池中的内存块
        // 这里我们手动实现重新分配
        if (newSize > POOL_BLOCK_SIZE) {
            OSAL_LOGE("Requested size exceeds pool block size\n");
            return nullptr;
        }
        void *newPtr = allocate(newSize);
        if (!newPtr) {
            return nullptr;
        }
        if (ptr) {
            memcpy(newPtr, ptr, newSize);
            deallocate(ptr);
        }
        OSAL_LOGD("Reallocated memory to %zu bytes from pool\n", newSize);
        return newPtr;
    }

    void *allocateAligned(size_t size, size_t alignment) override {
        if (alignment < sizeof(void *)) {
            alignment = sizeof(void *);
        }

        // 计算实际需要的内存大小，包括对齐修正和存储原始指针的空间
        // size_t totalSize = size + alignment - 1 + sizeof(void*);

        // 分配内存池块
        void *original = osMemoryPoolAlloc(memPoolId, 0);
        if (!original) {
            OSAL_LOGE("Failed to allocate aligned memory\n");
            return nullptr;
        }

        // 计算对齐地址
        uintptr_t aligned = (reinterpret_cast<uintptr_t>(original) + sizeof(void *) + alignment - 1) & ~(alignment - 1);

        // 存储原始指针用于释放时使用
        reinterpret_cast<void **>(aligned)[-1] = original;

        OSAL_LOGD("Allocated %zu bytes with alignment %zu\n", size, alignment);
        return reinterpret_cast<void *>(aligned);
    }

    size_t getAllocatedSize(void *ptr) const override {
        (void)ptr;
        // 这个功能在标准库中没有直接支持，通常需要自定义内存分配器来实现。
        // 这里仅作为示例，返回0。
        OSAL_LOGD("Requested size of allocated memory\n");
        return 0;
    }

private:
    osMemoryPoolId_t memPoolId;
    static const size_t POOL_BLOCK_SIZE = 128;  // 每个块的大小
    static const size_t POOL_BLOCK_COUNT = 10;  // 块的数量
};

}  // namespace osal

#endif  // __OSAL_MEMORY_MANAGER_H__