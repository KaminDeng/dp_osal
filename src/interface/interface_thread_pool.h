//
// Created by kamin.deng on 2024/8/23.
//
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
