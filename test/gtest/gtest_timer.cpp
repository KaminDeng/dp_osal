#include <atomic>

#include "gtest/gtest.h"
#include "osal_system.h"
#include "osal_test_framework_config.h"
#include "osal_timer.h"

using namespace osal;

TEST(OSALTimerTest, TestOSALTimerRepeat) {
#if (TestOSALTimerRepeatEnabled)
    osal::OSALTimer timer;
    std::atomic<int> count(0);

    timer.start(100, true, [&]() { count++; });
    EXPECT_FALSE(count > 5);
    OSALSystem::getInstance().sleep_ms(700);
    EXPECT_TRUE(count > 5);
    timer.stop();
#else
    GTEST_SKIP();
#endif
}

TEST(OSALTimerTest, TestOSALTimerStart) {
#if (TestOSALTimerStartEnabled)
    osal::OSALTimer timer;
    std::atomic<bool> callbackExecuted(false);

    timer.start(100, false, [&]() { callbackExecuted = true; });

    EXPECT_FALSE(callbackExecuted);
    OSALSystem::getInstance().sleep_ms(200);
    EXPECT_TRUE(callbackExecuted);
    timer.stop();
#else
    GTEST_SKIP();
#endif
}

TEST(OSALTimerTest, TestOSALTimerStop) {
#if (TestOSALTimerStopEnabled)
    osal::OSALTimer timer;
    std::atomic<bool> callbackExecuted(false);

    timer.start(100, false, [&]() { callbackExecuted = true; });

    OSALSystem::getInstance().sleep_ms(50);
    timer.stop();
    OSALSystem::getInstance().sleep_ms(100);
    EXPECT_FALSE(callbackExecuted);
#else
    GTEST_SKIP();
#endif
}

TEST(OSALTimerTest, TestOSALTimerIsRunning) {
#if (TestOSALTimerIsRunningEnabled)
    osal::OSALTimer timer;

    EXPECT_FALSE(timer.isRunning());

    timer.start(100, false, []() {});
    EXPECT_TRUE(timer.isRunning());

    timer.stop();
    EXPECT_FALSE(timer.isRunning());
#else
    GTEST_SKIP();
#endif
}

TEST(OSALTimerTest, TestOSALTimerGetRemainingTime) {
#if (TestOSALTimerGetRemainingTimeEnabled)
    osal::OSALTimer timer;

    timer.start(200, false, []() {});
    OSALSystem::getInstance().sleep_ms(100);
    uint32_t remainingTime = timer.getRemainingTime();
    EXPECT_TRUE(remainingTime > 0 && remainingTime <= 100);

    timer.stop();
    EXPECT_EQ(timer.getRemainingTime(), 0);
#else
    GTEST_SKIP();
#endif
}

TEST(OSALTimerTest, TestOSALTimerReset) {
#if (TestOSALTimerResetEnabled)
    osal::OSALTimer timer;
    std::atomic<bool> callbackExecuted(false);

    timer.start(200, false, [&]() { callbackExecuted = true; });

    OSALSystem::getInstance().sleep_ms(100);
    timer.reset();
    OSALSystem::getInstance().sleep_ms(150);
    EXPECT_FALSE(callbackExecuted);

    OSALSystem::getInstance().sleep_ms(100);
    EXPECT_TRUE(callbackExecuted);

    timer.stop();
#else
    GTEST_SKIP();
#endif
}