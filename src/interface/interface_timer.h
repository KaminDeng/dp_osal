//
// Created by kamin.deng on 2024/8/23.
//
#ifndef ITIMER_H_
#define ITIMER_H_

#include <functional>

namespace osal {

class ITimer {
public:
    virtual ~ITimer() {}

    // 启动定时器
    virtual void start(uint32_t interval, bool periodic, std::function<void()> callback) = 0;

    // 停止定时器
    virtual void stop() = 0;

    // 检查定时器是否正在运行
    virtual bool isRunning() const = 0;

    // 获取剩余时间
    virtual uint32_t getRemainingTime() const = 0;

    // 重置定时器
    virtual void reset() = 0;
};

}  // namespace osal
#endif  // ITIMER_H_
