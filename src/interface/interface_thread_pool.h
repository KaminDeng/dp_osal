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

#ifndef ITHREAD_POOL_H_
#define ITHREAD_POOL_H_

namespace osal {

class IThreadPool {
public:
    virtual ~IThreadPool() = default;

    // 启动线程池
    virtual void start(uint32_t numThreads, int priority = 0, int stack_size = 0) = 0;

    // 停止线程池
    virtual void stop() = 0;

    // 暂停线程池
    virtual int suspend() = 0;

    // 恢复线程池
    virtual int resume() = 0;

    // 检查线程池是否已经启动
    [[nodiscard]] virtual bool isStarted() const = 0;

    // 检查线程池是否已暂停
    [[nodiscard]] virtual bool isSuspended() const = 0;

    // 提交任务到线程池
    virtual void submit(std::function<void(void *)> taskFunction, void *taskArgument, int priority) = 0;

    // 设置线程优先级
    virtual void setPriority(int priority) = 0;

    // 获取线程优先级
    [[nodiscard]] virtual int getPriority() const = 0;

    // 获取当前任务队列的大小
    virtual size_t getTaskQueueSize() = 0;

    // 获取当前活跃线程数
    [[nodiscard]] virtual uint32_t getActiveThreadCount() const = 0;

    // 取消任务
    virtual bool cancelTask(std::function<void(void *)> &taskFunction) = 0;

    // 设置任务执行失败时的回调
    virtual void setTaskFailureCallback(std::function<void(void *)> callback) = 0;

    // 设置线程池的最大线程数
    virtual void setMaxThreads(uint32_t maxThreads) = 0;

    // 获取线程池的最大线程数
    [[nodiscard]] virtual uint32_t getMaxThreads() const = 0;

    // 设置线程池的最小线程数
    virtual void setMinThreads(uint32_t minThreads) = 0;

    // 获取线程池的最小线程数
    [[nodiscard]] virtual uint32_t getMinThreads() const = 0;
};
}  // namespace osal
#endif  // ITHREAD_POOL_H_
