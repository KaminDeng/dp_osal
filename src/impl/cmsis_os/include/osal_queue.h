/*
 * Copyright (c) 2024 kamin.deng
 * Email: kamin.deng@gmail.com
 * Created on 2024/8/22.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef __OSAL_MESSAGE_QUEUE_H__
#define __OSAL_MESSAGE_QUEUE_H__

#include "osal.h"
#include "interface_queue.h"
#include "osal_debug.h"

namespace osal {

template <typename T>
class OSALMessageQueue : public MessageQueue<T> {
public:
    explicit OSALMessageQueue(uint32_t queue_size = 16) {
        osMessageQueueAttr_t queueAttr = {};
        queueAttr.name = "MessageQueue";
        queue_ = osMessageQueueNew(queue_size, sizeof(T), &queueAttr);
        if (queue_ == nullptr) {
            OSAL_LOGE("Failed to create message queue\n");
            // 处理消息队列创建失败的情况
        }
    }

    ~OSALMessageQueue() override {
        if (queue_ != nullptr) {
            osMessageQueueDelete(queue_);
        }
    }

    void send(const T &message) override {
        if (osMessageQueuePut(queue_, &message, 0, osWaitForever) != osOK) {
            OSAL_LOGE("Failed to send message\n");
        } else {
            OSAL_LOGD("Message sent\n");
        }
    }

    T receive() override {
        T message;
        if (osMessageQueueGet(queue_, &message, nullptr, osWaitForever) != osOK) {
            OSAL_LOGE("Failed to receive message\n");
            // 处理接收消息失败的情况
        } else {
            OSAL_LOGD("Message received\n");
        }
        return message;
    }

    bool tryReceive(T &message) override {
        if (osMessageQueueGet(queue_, &message, nullptr, 0) != osOK) {
            return false;
        }
        OSAL_LOGD("Message try-received\n");
        return true;
    }

    bool receiveFor(T &message, uint32_t timeout) override {
        if (osMessageQueueGet(queue_, &message, nullptr, timeout) != osOK) {
            return false;
        }
        OSAL_LOGD("Message received with timeout\n");
        return true;
    }

    [[nodiscard]] size_t size() const override { return osMessageQueueGetCount(queue_); }

    void clear() override {
        while (osMessageQueueGetCount(queue_) > 0) {
            T message;
            osMessageQueueGet(queue_, &message, nullptr, 0);
        }
        OSAL_LOGD("Message queue cleared\n");
    }

private:
    osMessageQueueId_t queue_;
};

}  // namespace osal

#endif  // __OSAL_MESSAGE_QUEUE_H__