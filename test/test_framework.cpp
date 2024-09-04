//
// Created by kamin.deng on 2024/8/23.
//
#include "test_framework.h"

#include "osal_debug.h"
#include "test_condition_variable.cpp"
#include "test_lockguard.cpp"
#include "test_memory_manger.cpp"
#include "test_mutex.cpp"
#include "test_queue.cpp"
#include "test_rwlock.cpp"
#include "test_semaphore.cpp"
#include "test_spin_lock.cpp"
#include "test_thread.cpp"
#include "test_thread_pool.cpp"
#include "test_timer.cpp"
// 如果系统启动时时间为0, 可能会测试失败
#include "test_chrono.cpp"

using namespace osal;

void runAllTests() {
    const auto &tests = TestRegistry::instance().getTests();
    int passed = 0;
    int failed = 0;
    int not_executed = 0;
    int index = 1;

    // 计算最长的测试名称长度
    size_t maxNameLength = 0;
    for (const auto &test : tests) {
        if (test->name().length() > maxNameLength) {
            maxNameLength = test->name().length();
        }
    }

    for (const auto &test : tests) {
        // 构建格式化字符串
        char buffer[256];
        snprintf(buffer, sizeof(buffer), "TEST %-3d: %-*s -> ", index, static_cast<int>(maxNameLength + 2),
                 test->name().c_str());
        OSAL_LOGI("%s", buffer);

        if (test->run_flag) {
            int result = test->run();
            if (result == 0) {
                OSAL_LOG("PASSED\n");
                passed++;
            } else {
                OSAL_LOG("FAILED\n");
                failed++;
            }
        } else {
            OSAL_LOG("NOT EXECUTED\n");
            not_executed++;
        }

        index++;
    }

    OSAL_LOGI("Total tests: %d -> Passed: %d, Failed: %d, Not Executed: %d\n", static_cast<int>(tests.size()), passed, failed,
              not_executed);
}

// 用于封装返回值伟int的宏
void TestOSALPackFunc(std::function<int(void *)> taskFunction) {
    if(taskFunction)
        taskFunction(nullptr);
}