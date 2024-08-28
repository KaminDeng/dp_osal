//
// Created by kamin.deng on 2024/8/23.
//
#ifndef __OSAL_MEMORY_MANAGER_H__
#define __OSAL_MEMORY_MANAGER_H__

#include <cstdlib>
#include <cstring>
#include <new>

#include "interface_memory_manager.h"
#include "osal_debug.h"

namespace osal {

class OSALMemoryManager : public IMemoryManager {
public:
    OSALMemoryManager(size_t block_size, size_t block_count)
        : pool_(nullptr), freeList_(nullptr), blockSize_(block_size), blockCount_(block_count) {
        initialize(block_size, block_count);
    }

    ~OSALMemoryManager() override {
        if (pool_ != nullptr) {
            std::free(pool_);
            pool_ = nullptr;
        }
    }

    bool initialize(size_t block_size, size_t block_count) override {
        blockSize_ = block_size;
        blockCount_ = block_count;
        size_t poolSize = blockSize_ * blockCount_;

        pool_ = std::malloc(poolSize);
        if (pool_ == nullptr) {
            OSAL_LOGE("MemoryPool initialization failed: unable to allocate memory.");
            return false;
        }

        freeList_ = reinterpret_cast<void **>(pool_);
        for (size_t i = 0; i < blockCount_ - 1; ++i) {
            freeList_[i] = reinterpret_cast<void *>(reinterpret_cast<uint8_t *>(pool_) + (i + 1) * blockSize_);
        }
        freeList_[blockCount_ - 1] = nullptr;

        OSAL_LOGD("MemoryPool initialized with block size: %zu, block count: %zu.", blockSize_, blockCount_);
        return true;
    }

    void *allocate(size_t size) override {
        if (size > blockSize_) {
            OSAL_LOGE("Allocation failed: requested size %zu exceeds block size %zu.", size, blockSize_);
            return nullptr;
        }

        if (freeList_ == nullptr) {
            OSAL_LOGE("Allocation failed: no free blocks available.");
            return nullptr;
        }

        void *block = freeList_;
        freeList_ = reinterpret_cast<void **>(*freeList_);
        OSAL_LOGD("Allocated block at address: %p.", block);
        return block;
    }

    void deallocate(void *ptr) override {
        if (ptr == nullptr) {
            OSAL_LOGE("Deallocate failed: pointer is null.");
            return;
        }

        *reinterpret_cast<void **>(ptr) = freeList_;
        freeList_ = reinterpret_cast<void **>(ptr);
        OSAL_LOGD("Deallocated block at address: %p.", ptr);
    }

    void *reallocate(void *ptr, size_t newSize) override {
        if (newSize > blockSize_) {
            OSAL_LOGE("Reallocate failed: requested size %zu exceeds block size %zu.", newSize, blockSize_);
            return nullptr;
        }

        void *newPtr = allocate(newSize);
        if (newPtr && ptr) {
            std::memcpy(newPtr, ptr, blockSize_);  // 复制旧数据到新块
            deallocate(ptr);
        }

        return newPtr;
    }

    void *allocateAligned(size_t size, size_t alignment) override {
        if (alignment < sizeof(void *)) {
            alignment = sizeof(void *);
        }
        void *ptr = allocate(size + alignment);
        if (!ptr) {
            OSAL_LOGE("Aligned allocation failed: unable to allocate memory.");
            return nullptr;
        }

        uintptr_t alignedPtr = (reinterpret_cast<uintptr_t>(ptr) + alignment - 1) & ~(alignment - 1);
        OSAL_LOGD("Aligned allocation: original address: %p, aligned address: %p.", ptr,
                  reinterpret_cast<void *>(alignedPtr));
        return reinterpret_cast<void *>(alignedPtr);
    }

    [[nodiscard]] size_t getAllocatedSize() const override { return blockSize_; }

private:
    void *pool_;
    void **freeList_;
    size_t blockSize_;
    size_t blockCount_;
};

}  // namespace osal

#endif  // __OSAL_MEMORY_MANAGER_H__