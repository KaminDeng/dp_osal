//
// Created by kamin.deng on 2024/8/23.
//

#include <atomic>

#include "osal_system.h"
#include "osal_timer.h"
#include "test_framework.h"

using namespace osal;

TEST_CASE(TestOSALTimerRepeat) {
#if (TestOSALTimerRepeatEnabled)
    osal::OSALTimer timer;
    std::atomic<int> count(0);

    timer.start(100, true, [&]() { count++; });
    OSAL_ASSERT_FALSE(count > 5);
    OSALSystem::getInstance().sleep_ms(700);
    OSAL_ASSERT_TRUE(count > 5);
    timer.stop();
#endif
    return 0;  // 表示测试通过
}

TEST_CASE(TestOSALTimerStart) {
#if (TestOSALTimerStartEnabled)
    osal::OSALTimer timer;
    std::atomic<bool> callbackExecuted(false);

    timer.start(100, false, [&]() { callbackExecuted = true; });

    OSAL_ASSERT_FALSE(callbackExecuted);
    OSALSystem::getInstance().sleep_ms(200);
    OSAL_ASSERT_TRUE(callbackExecuted);
    timer.stop();
#endif
    return 0;  // 表示测试通过
}

TEST_CASE(TestOSALTimerStop) {
#if (TestOSALTimerStopEnabled)
    osal::OSALTimer timer;
    std::atomic<bool> callbackExecuted(false);

    timer.start(100, false, [&]() { callbackExecuted = true; });

    OSALSystem::getInstance().sleep_ms(50);
    timer.stop();
    OSALSystem::getInstance().sleep_ms(100);
    OSAL_ASSERT_FALSE(callbackExecuted);
#endif
    return 0;  // 表示测试通过
}

TEST_CASE(TestOSALTimerIsRunning) {
#if (TestOSALTimerIsRunningEnabled)
    osal::OSALTimer timer;

    OSAL_ASSERT_FALSE(timer.isRunning());

    timer.start(100, false, []() {});
    OSAL_ASSERT_TRUE(timer.isRunning());

    timer.stop();
    OSAL_ASSERT_FALSE(timer.isRunning());
#endif
    return 0;  // 表示测试通过
}

TEST_CASE(TestOSALTimerGetRemainingTime) {
#if (TestOSALTimerGetRemainingTimeEnabled)
    osal::OSALTimer timer;

    timer.start(200, false, []() {});
    OSALSystem::getInstance().sleep_ms(100);
    uint32_t remainingTime = timer.getRemainingTime();
    OSAL_ASSERT_TRUE(remainingTime > 0 && remainingTime <= 100);

    timer.stop();
    OSAL_ASSERT_TRUE(timer.getRemainingTime() == 0);
#endif
    return 0;  // 表示测试通过
}

TEST_CASE(TestOSALTimerReset) {
#if (TestOSALTimerResetEnabled)
    osal::OSALTimer timer;
    std::atomic<bool> callbackExecuted(false);

    timer.start(200, false, [&]() { callbackExecuted = true; });

    OSALSystem::getInstance().sleep_ms(100);
    timer.reset();
    OSALSystem::getInstance().sleep_ms(150);
    OSAL_ASSERT_FALSE(callbackExecuted);

    OSALSystem::getInstance().sleep_ms(100);
    OSAL_ASSERT_TRUE(callbackExecuted);

    timer.stop();
#endif
    return 0;  // 表示测试通过
}