//
// Created by kamin.deng on 2024/8/23.
//
#ifndef IREAD_WRITE_LOCK_H_
#define IREAD_WRITE_LOCK_H_

namespace osal {

class IRWLock {
public:
    virtual ~IRWLock() = default;

    // 获取读锁
    virtual void readLock() = 0;

    // 尝试获取读锁
    virtual bool tryReadLock() = 0;

    // 带超时的读锁
    virtual bool readLockFor(uint32_t timeout) = 0;

    // 释放读锁
    virtual void readUnlock() = 0;

    // 获取写锁
    virtual void writeLock() = 0;

    // 尝试获取写锁
    virtual bool tryWriteLock() = 0;

    // 带超时的写锁
    virtual bool writeLockFor(uint32_t timeout) = 0;

    // 释放写锁
    virtual void writeUnlock() = 0;

    // 获取当前读锁数量
    [[nodiscard]] virtual size_t getReadLockCount() const = 0;

    // 获取当前写锁状态
    [[nodiscard]] virtual bool isWriteLocked() const = 0;
};

}  // namespace osal
#endif  // IREAD_WRITE_LOCK_H_
