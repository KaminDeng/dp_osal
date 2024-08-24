//
// Created by kamin.deng on 2024/8/23.
//
#ifndef ITHREAD_H_
#define ITHREAD_H_

namespace osal {

class IThread {
public:
    virtual ~IThread() = default;

    // 启动线程
    virtual int start(const char *name, std::function<void(void *)> taskFunction, void *taskArgument, int priority = 0,
                      int stack_size = 0, void *pstack = nullptr) = 0;

    // 停止线程
    virtual void stop() = 0;

    // 加入线程（等待线程完成）
    virtual void join() = 0;

    // 分离线程（允许线程独立执行）
    virtual void detach() = 0;

    // 暂停线程（使线程暂停执行）
    virtual int suspend() = 0;

    // 恢复线程（使线程恢复执行）
    virtual int resume() = 0;

    // 获取线程是否正在运行的状态
    [[nodiscard]] virtual bool isRunning() const = 0;

    // 设置线程优先级
    virtual void setPriority(int priority) = 0;

    // 获取线程优先级
    [[nodiscard]] virtual int getPriority() const = 0;
};

}  // namespace osal
#endif  // ITHREAD_H_
