/*
 * Copyright (c) 2024 kamin.deng
 * Email: kamin.deng@gmail.com
 * Created on 2024/8/14.
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

#include "osal_debug.h"
#include "osal_system.h"
#include "osal_thread.h"

using namespace osal;

#ifdef OSAL_CONFIG_SELFTEST_ENABLE
#include "test_chrono.cpp"  // 如果系统启动时时间为0, 可能会测试失败
#include "test_condition_variable.cpp"
#include "test_framework.h"
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
#endif

#ifdef OSAL_CONFIG_GOOGLETEST_ENABLE
#include <gtest/gtest.h>

#include "gtest_chrono.cpp"  // 如果系统启动时时间为0, 可能会测试失败
#include "gtest_condition_variable.cpp"
#include "gtest_lockguard.cpp"
#include "gtest_memory_manger.cpp"
#include "gtest_mutex.cpp"
#include "gtest_queue.cpp"
#include "gtest_rwlock.cpp"
#include "gtest_semaphore.cpp"
#include "gtest_spin_lock.cpp"
#include "gtest_thread.cpp"
#include "gtest_thread_pool.cpp"
#include "gtest_timer.cpp"
#endif

void StartDefaultTask(void *argument) {
    (void)argument;
    // setLogLevel(LOG_LEVEL_VERBOSE);

#ifdef OSAL_CONFIG_SELFTEST_ENABLE
    runAllTests();
#endif

#ifdef OSAL_CONFIG_GOOGLETEST_ENABLE
    int argc = 1;
    char *argv[1] = {const_cast<char *>("osal_gtest")};
    ::testing::InitGoogleTest(&argc, argv);
    RUN_ALL_TESTS();
#endif
}

OSALThread osal_test_thread;
extern "C" int osal_test_main(void) {
    static int arg = 12;
    OSAL_LOGI("System Type: %s\n", OSALSystem::getInstance().get_system_info());
    osal_test_thread.start("osal_test_thread", StartDefaultTask, (void *)&arg, 0, 2048);
    OSALSystem::getInstance().StartScheduler();
    return 0;
}