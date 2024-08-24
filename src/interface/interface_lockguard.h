//
// Created by kamin.deng on 2024/8/23.
//
#ifndef ILOCKGUARD_H_
#define ILOCKGUARD_H_

namespace osal {

// RAII风格的锁管理类
class ILockGuard {
public:
    virtual ~ILockGuard() = default;

    virtual bool isLocked() = 0;
};
}  // namespace osal

#endif  // ILOCKGUARD_H_
