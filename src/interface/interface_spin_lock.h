//
// Created by kamin.deng on 2024/8/23.
//
#ifndef ISPIN_LOCK_H_
#define ISPIN_LOCK_H_

namespace osal {

class ISpinLock {
public:
    virtual ~ISpinLock() = default;

    // 获取锁
    virtual void lock() = 0;

    // 尝试获取锁
    virtual bool tryLock() = 0;

    // 带超时的锁
    virtual bool lockFor(uint32_t timeout) = 0;

    // 释放锁
    virtual void unlock() = 0;

    // 获取当前锁状态
    [[nodiscard]] virtual bool isLocked() const = 0;
};

}  // namespace osal
#endif  // ISPIN_LOCK_H_
