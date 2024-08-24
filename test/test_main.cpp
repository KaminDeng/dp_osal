//
// Created by kamin.deng on 2024/8/14.
//
#include "osal_debug.h"
#include "osal_system.h"
#include "osal_thread.h"
#include "test_framework.h"

using namespace osal;

[[noreturn]] void StartDefaultTask(void *argument) {
    //    setLogLevel(LOG_LEVEL_VERBOSE);
    runAllTests();
    for (;;) {
        OSAL_LOGI("hello world %d\n", *((int *)argument));
        OSALSystem::getInstance().sleep_ms(1000);
    }
}

extern "C" int osal_test_main(void) {
    static int arg = 12;
    OSAL_LOGI("System Type: %s\n", OSALSystem::getInstance().get_system_info());
    OSALThread test_thread("test_thread", StartDefaultTask, (void *)&arg);
    OSALSystem::getInstance().StartScheduler();
    return 0;
}