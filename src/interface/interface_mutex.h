//
// Created by kamin.deng on 2024/8/23.
//
#ifndef IMUTEX_H_
#define IMUTEX_H_

namespace osal {
class IMutex {
public:
    virtual ~IMutex() = default;

    // 锁定互斥锁
    virtual bool lock() = 0;

    // 解锁互斥锁
    virtual bool unlock() = 0;

    // 尝试锁定互斥锁
    virtual bool tryLock() = 0;

    // 带超时的锁定互斥锁
    virtual bool tryLockFor(uint32_t timeout) = 0;
};
}  // namespace osal

#endif  // IMUTEX_H_
