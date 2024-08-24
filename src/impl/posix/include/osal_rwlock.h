//
// Created by kamin.deng on 2024/8/23.
//
#ifndef __OSAL_RWLOCK_H__
#define __OSAL_RWLOCK_H__

#include <chrono>
#include <shared_mutex>

#include "interface_rwlock.h"
#include "osal_debug.h"

namespace osal {

class OSALRWLock : public IRWLock {
public:
    OSALRWLock() = default;

    ~OSALRWLock() = default;

    void readLock() override {
        sharedMutex_.lock_shared();
        OSAL_LOGD("Read lock acquired\n");
    }

    bool tryReadLock() override {
        bool result = sharedMutex_.try_lock_shared();
        OSAL_LOGD("Try read lock %s\n", result ? "succeeded" : "failed");
        return result;
    }

    bool readLockFor(uint32_t timeout) override {
        bool result = sharedMutex_.try_lock_shared_for(std::chrono::milliseconds(timeout));
        OSAL_LOGD("Read lock with timeout %s\n", result ? "succeeded" : "failed");
        return result;
    }

    void readUnlock() override {
        sharedMutex_.unlock_shared();
        OSAL_LOGD("Read lock released\n");
    }

    void writeLock() override {
        sharedMutex_.lock();
        OSAL_LOGD("Write lock acquired\n");
    }

    bool tryWriteLock() override {
        bool result = sharedMutex_.try_lock();
        OSAL_LOGD("Try write lock %s\n", result ? "succeeded" : "failed");
        return result;
    }

    bool writeLockFor(uint32_t timeout) override {
        bool result = sharedMutex_.try_lock_for(std::chrono::milliseconds(timeout));
        OSAL_LOGD("Write lock with timeout %s\n", result ? "succeeded" : "failed");
        return result;
    }

    void writeUnlock() override {
        sharedMutex_.unlock();
        OSAL_LOGD("Write lock released\n");
    }

    size_t getReadLockCount() const override {
        // 这个功能在标准库中没有直接支持，通常需要自定义实现。
        // 这里仅作为示例，返回0。
        OSAL_LOGD("Requested read lock count\n");
        return 0;
    }

    bool isWriteLocked() const override {
        // 这个功能在标准库中没有直接支持，通常需要自定义实现。
        // 这里仅作为示例，返回false。
        OSAL_LOGD("Requested write lock status\n");
        return false;
    }

private:
    mutable std::shared_timed_mutex sharedMutex_;
};

}  // namespace osal

#endif  // __OSAL_RWLOCK_H__