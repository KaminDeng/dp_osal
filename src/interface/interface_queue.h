//
// Created by kamin.deng on 2024/8/23.
//
#ifndef IQUEUE_H_
#define IQUEUE_H_

namespace osal {

template <typename T>
class MessageQueue {
public:
    virtual ~MessageQueue() = default;

    virtual void send(const T &message) = 0;

    virtual T receive() = 0;

    virtual bool tryReceive(T &message) = 0;

    virtual bool receiveFor(T &message, uint32_t timeout) = 0;  // 带超时的接收
    [[nodiscard]] virtual size_t size() const = 0;              // 获取队列大小
    virtual void clear() = 0;                                   // 清空队列
};

}  // namespace osal
#endif  // IQUEUE_H_
