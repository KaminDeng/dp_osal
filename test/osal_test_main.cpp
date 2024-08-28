//
// Created by kamin.deng on 2024/8/14.
//
#include "osal_debug.h"
#include "osal_system.h"
#include "osal_thread.h"
#include "test_framework.h"

using namespace osal;

void StartDefaultTask(void *argument) {
    (void)argument;
    // setLogLevel(LOG_LEVEL_VERBOSE);
    runAllTests();
}

OSALThread osal_test_thread;
extern "C" int osal_test_main(void) {
    static int arg = 12;
    OSAL_LOGI("System Type: %s\n", OSALSystem::getInstance().get_system_info());
    osal_test_thread.start("osal_test_thread", StartDefaultTask, (void *)&arg, 0, 2048);
    OSALSystem::getInstance().StartScheduler();
    return 0;
}