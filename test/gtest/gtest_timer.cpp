/*
 * Copyright (c) 2024 kamin.deng
 * Email: kamin.deng@gmail.com
 * Created on 2025/1/3.
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