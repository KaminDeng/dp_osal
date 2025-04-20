# DP_OSAL (Operating System Abstraction Layer)

## 概述
提供统一的系统抽象接口，目的是统一不同操作系统平台的接口，统一编程行为提高代码的可移植性
* posix接口
* 单片机平台cmsis-os接口(可用于支持cmsis-os的RTOS，比如FreeRTOS, Zephyr等)。

## 功能特性
- 线程管理（创建/销毁/优先级控制）
- 同步原语（互斥锁、信号量、条件变量、读写锁、自旋锁）
- 消息队列（支持阻塞/超时/优先级消息）
- 内存管理（动态内存分配监控）
- 定时器（单次/周期模式，剩余时间查询）
- 调试支持（日志分级、资源统计、单元测试框架）
- 跨平台适配（FreeRTOS/POSIX 统一接口）
- 线程安全容器（锁守卫、线程池管理）
- 系统抽象（时钟管理）

## 目录结构
- `src/`: 核心实现代码
  - `debug/`: debug接口实现
  - `impl/`: 不同的系统平台接口实现
  - `interface/`: 系统抽象接口
- `example/`: 移植接口示例

## 快速入门
使用CMake构建系统：
```bash
cd example/getting_started
mkdir build && cd build
cmake ..
make

# hello_world demo
./hello_world
# unit test
./osal_test
```

## 测试框架
- 支持gtest和原生测试框架（`test/gtest/`和`test/test_framework/`）
- 测试用例覆盖率（基于`osal_test_framework_config.h`）：

## 使用示例
- 参考`example/`目录中的移植示例
- 参考RoboMaster-A开发板CMake工程：[dp_stm32f427_dev_cmake](https://github.com/KaminDeng/dp_stm32f427_dev_cmake)

## 贡献指南
欢迎提交Pull Request，请确保代码风格一致并通过测试。

## License
详见项目根目录 [LICENSE](LICENSE) 文件