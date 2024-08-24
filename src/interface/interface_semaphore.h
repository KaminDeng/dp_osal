//
// Created by kamin.deng on 2024/8/23.
//
#ifndef ISEMAPHORE_H_
#define ISEMAPHORE_H_

#include <stdint.h>

namespace osal {

class ISemaphore {
public:
    virtual ~ISemaphore() = default;

    virtual void wait() = 0;

    virtual void signal() = 0;

    virtual bool tryWait() = 0;

    virtual bool tryWaitFor(uint32_t timestamp) = 0;

    [[nodiscard]] virtual int getValue() const = 0;  // 获取当前信号量的值
    virtual void init(int initialValue) = 0;         // 初始化信号量的值
};

}  // namespace osal
#endif  // ISEMAPHORE_H_
