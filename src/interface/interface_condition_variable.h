//
// Created by kamin.deng on 2024/8/23.
//
#ifndef ICONDITION_VARIABLE_H_
#define ICONDITION_VARIABLE_H_

#include "osal_mutex.h"

namespace osal {

class IConditionVariable {
public:
    virtual ~IConditionVariable() = default;

    virtual void wait(OSALMutex &mutex) = 0;

    virtual bool waitFor(OSALMutex &mutex, uint32_t timeout) = 0;  // 带超时的等待
    virtual void notifyOne() = 0;

    virtual void notifyAll() = 0;

    [[nodiscard]] virtual int getWaitCount() const = 0;  // 获取当前等待线程数
};

}  // namespace osal

#endif  // ICONDITION_VARIABLE_H_
