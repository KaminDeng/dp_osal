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
    OSALMemoryManager(size_t block_size, size_t block_count)
        : memPoolId(nullptr), _block_size(block_size), _block_count(block_count) {
        //        initialize(block_size, block_count);
    }

    ~OSALMemoryManager() override {
        if (memPoolId != NULL) {
            osMemoryPoolDelete(memPoolId);
        }
    }

    bool initialize(size_t block_size, size_t block_count) override {
        if (block_size == 0 || block_count == 0) {
            OSAL_LOGE("Invalid memory pool configuration\n");
            return false;
        }
        if (memPoolId == NULL) {
            _block_count = block_count;
            _block_size = block_size;
            // 初始化内存池
            memPoolId = osMemoryPoolNew(_block_count, _block_size, NULL);
            if (memPoolId == NULL) {
                OSAL_LOGE("Failed to create memory pool\n");
                // 处理内存池创建失败的情况
                return false;
            }
        }
        return true;
    }

    void *allocate(size_t size) override {
        if (memPoolId == NULL) {
            is_inited = initialize(_block_size, _block_count);
            if (!is_inited) return nullptr;
        }

        if (size > _block_size) {
            OSAL_LOGE("Requested size exceeds pool block size\n");
            return nullptr;
        }
        void *ptr = osMemoryPoolAlloc(memPoolId, 0);
        if (!ptr) {
            OSAL_LOGE("Failed to allocate memory from pool\n");
            return nullptr;
        }
        OSAL_LOGD("Allocated %d bytes from pool\n", size);
        return ptr;
    }

    void deallocate(void *ptr) override {
        if (memPoolId == NULL) {
            is_inited = initialize(_block_size, _block_count);
        }
        if (osMemoryPoolFree(memPoolId, ptr) != osOK) {
            OSAL_LOGE("Failed to deallocate memory to pool\n");
        } else {
            OSAL_LOGD("Deallocated memory to pool\n");
        }
    }

    void *reallocate(void *ptr, size_t newSize) override {
        if (memPoolId == NULL) {
            is_inited = initialize(_block_size, _block_count);
            if (!is_inited) return nullptr;
        }
        // CMSIS-RTOS2 不支持直接重新分配内存池中的内存块
        // 这里我们手动实现重新分配
        if (newSize > _block_size) {
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
        OSAL_LOGD("Reallocated memory to %d bytes from pool\n", newSize);
        return newPtr;
    }

    void *allocateAligned(size_t size, size_t alignment) override {
        if (memPoolId == NULL) {
            is_inited = initialize(_block_size, _block_count);
            if (!is_inited) return nullptr;
        }

        if (alignment < sizeof(void *)) {
            alignment = sizeof(void *);
        }

        // 确保分配的大小不会超过内存池单个块的大小
        size_t totalSize = size + alignment - 1 + sizeof(void *);
        if (totalSize > _block_size) {
            OSAL_LOGE("Requested size exceeds the block size of the memory pool.\n");
            return nullptr;
        }

        // 分配内存池块
        void *original = osMemoryPoolAlloc(memPoolId, 0);
        if (!original) {
            OSAL_LOGE("Failed to allocate aligned memory\n");
            return nullptr;
        }

        // 计算对齐后的地址
        uintptr_t aligned = (reinterpret_cast<uintptr_t>(original) + sizeof(void *) + alignment - 1) & ~(alignment - 1);

        // 存储原始指针用于释放时使用
        reinterpret_cast<void **>(aligned)[-1] = original;

        OSAL_LOGD("Allocated %d bytes with alignment %d\n", size, alignment);
        return reinterpret_cast<void *>(aligned);
    }

    [[nodiscard]] size_t getAllocatedSize() const override { return _block_size; }

private:
    osMemoryPoolId_t memPoolId = nullptr;
    size_t _block_size = 0;   // 每个块的大小
    size_t _block_count = 0;  // 块的数量
    volatile bool is_inited = false;
};

}  // namespace osal

#endif  // __OSAL_MEMORY_MANAGER_H__