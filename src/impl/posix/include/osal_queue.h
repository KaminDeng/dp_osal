/*
 * Copyright (c) 2024 kamin.deng
 * Email: kamin.deng@gmail.com
 * Created on 2024/8/23.
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

#include <chrono>
#include <condition_variable>
#include <mutex>
#include <queue>

#include "interface_queue.h"
#include "osal_debug.h"

namespace osal {

template <typename T>
class OSALMessageQueue : public MessageQueue<T> {
public:
    OSALMessageQueue() = default;

    ~OSALMessageQueue() = default;

    void send(const T &message) override {
        std::lock_guard<std::mutex> lock(mutex_);
        queue_.push(message);
        OSAL_LOGD("Message sent\n");
        condVar_.notify_one();
    }

    T receive() override {
        std::unique_lock<std::mutex> lock(mutex_);
        condVar_.wait(lock, [this] { return !queue_.empty(); });
        T message = queue_.front();
        queue_.pop();
        OSAL_LOGD("Message received\n");
        return message;
    }

    bool tryReceive(T &message) override {
        std::lock_guard<std::mutex> lock(mutex_);
        if (queue_.empty()) {
            return false;
        }
        message = queue_.front();
        queue_.pop();
        OSAL_LOGD("Message try-received\n");
        return true;
    }

    bool receiveFor(T &message, uint32_t timeout) override {
        std::unique_lock<std::mutex> lock(mutex_);
        if (!condVar_.wait_for(lock, std::chrono::milliseconds(timeout), [this] { return !queue_.empty(); })) {
            return false;
        }
        message = queue_.front();
        queue_.pop();
        OSAL_LOGD("Message received with timeout\n");
        return true;
    }

    size_t size() const override {
        std::lock_guard<std::mutex> lock(mutex_);
        return queue_.size();
    }

    void clear() override {
        std::lock_guard<std::mutex> lock(mutex_);
        std::queue<T> empty;
        std::swap(queue_, empty);
        OSAL_LOGD("Message queue cleared\n");
    }

private:
    mutable std::mutex mutex_;
    std::queue<T> queue_;
    std::condition_variable condVar_;
};

}  // namespace osal

#endif  // __OSAL_MESSAGE_QUEUE_H__