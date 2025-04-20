/*
 * Copyright (c) 2024 kamin.deng
 * Email: kamin.deng@gmail.com
 * Created on 2024/8/22.
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

#ifndef __OSAL_MEMORY_MANAGER_H__
#define __OSAL_MEMORY_MANAGER_H__

#include <string.h>

#include "osal.h"
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

        // 检查指针是否为空
        if (ptr == nullptr) {
            OSAL_LOGE("Attempted to deallocate a null pointer\n");
            return;
        }

        // 提取元数据
        uintptr_t *metadata = reinterpret_cast<uintptr_t *>(ptr) - 2;
        void *original = reinterpret_cast<void *>(metadata[0]);
        uintptr_t magic = metadata[1];

        // 检查是否是对齐分配的内存块
        if (magic == MAGIC_NUMBER) {
            // 对齐分配的内存块
            if (osMemoryPoolFree(memPoolId, original) != osOK) {
                OSAL_LOGE("Failed to deallocate memory to pool\n");
            } else {
                OSAL_LOGD("Deallocated memory to pool\n");
            }
        } else {
            // 普通分配的内存块
            if (osMemoryPoolFree(memPoolId, ptr) != osOK) {
                OSAL_LOGE("Failed to deallocate memory to pool\n");
            } else {
                OSAL_LOGD("Deallocated memory to pool\n");
            }
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
        size_t totalSize = size + alignment - 1 + 2 * sizeof(uintptr_t);
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

        // 检查原始地址是否已经对齐
        if (reinterpret_cast<uintptr_t>(original) % alignment == 0) {
            OSAL_LOGD("Allocated %d bytes with alignment %d (already aligned)\n", size, alignment);
            return original;
        }

        // 计算对齐后的地址
        uintptr_t aligned = (reinterpret_cast<uintptr_t>(original) + 2 * sizeof(uintptr_t) + alignment - 1) & ~(alignment - 1);

        // 存储元数据用于释放时使用
        uintptr_t *metadata = reinterpret_cast<uintptr_t *>(aligned) - 2;
        metadata[0] = reinterpret_cast<uintptr_t>(original);
        metadata[1] = MAGIC_NUMBER; // 使用魔数标识对齐分配的内存块

        OSAL_LOGD("Allocated %d bytes with alignment %d\n", size, alignment);
        return reinterpret_cast<void *>(aligned);
    }

    [[nodiscard]] size_t getAllocatedSize() const override { return _block_size; }

private:
    osMemoryPoolId_t memPoolId = nullptr;
    size_t _block_size = 0;   // 每个块的大小
    size_t _block_count = 0;  // 块的数量
    volatile bool is_inited = false;
    static constexpr uintptr_t MAGIC_NUMBER = 0xDEADBEEF; // 魔数，用于标识对齐分配的内存块
};

}  // namespace osal

#endif  // __OSAL_MEMORY_MANAGER_H__