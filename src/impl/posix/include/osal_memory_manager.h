//
// Created by kamin.deng on 2024/8/23.
//
#ifndef __OSAL_MEMORY_MANAGER_H__
#define __OSAL_MEMORY_MANAGER_H__

#include <cstdlib>
#include <new>

#include "interface_memory_manager.h"
#include "osal_debug.h"

namespace osal {

class OSALMemoryManager : public IMemoryManager {
public:
    void *allocate(size_t size) override {
        void *ptr = std::malloc(size);
        if (!ptr) {
            OSAL_LOGE("Failed to allocate memory\n");
            throw std::bad_alloc();
        }
        OSAL_LOGD("Allocated %zu bytes\n", size);
        return ptr;
    }

    void deallocate(void *ptr) override {
        std::free(ptr);
        OSAL_LOGD("Deallocated memory\n");
    }

    void *reallocate(void *ptr, size_t newSize) override {
        void *newPtr = std::realloc(ptr, newSize);
        if (!newPtr) {
            OSAL_LOGE("Failed to reallocate memory\n");
            throw std::bad_alloc();
        }
        OSAL_LOGD("Reallocated memory to %zu bytes\n", newSize);
        return newPtr;
    }

    void *allocateAligned(size_t size, size_t alignment) override {
        void *ptr = nullptr;
#if defined(_ISOC11_SOURCE)
        ptr = std::aligned_alloc(alignment, size);
#else
        if (posix_memalign(&ptr, alignment, size) != 0) {
            ptr = nullptr;
        }
#endif
        if (!ptr) {
            OSAL_LOGE("Failed to allocate aligned memory\n");
            throw std::bad_alloc();
        }
        OSAL_LOGD("Allocated %zu bytes with alignment %zu\n", size, alignment);
        return ptr;
    }

    size_t getAllocatedSize(void *ptr) const override {
        (void)ptr;
        // 这个功能在标准库中没有直接支持，通常需要自定义内存分配器来实现。
        // 这里仅作为示例，返回0。
        OSAL_LOGD("Requested size of allocated memory\n");
        return 0;
    }
};

}  // namespace osal

#endif  // __OSAL_MEMORY_MANAGER_H__