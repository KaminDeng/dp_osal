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

#include "gtest/gtest.h"
#include "osal_chrono.h"
#include "osal_system.h"
#include "osal_test_framework_config.h"

using namespace osal;

TEST(OSALChronoTest, TestOSALChronoNow) {
#if (TestOSALChronoNowEnabled)
    auto timePoint1 = OSALChrono::getInstance().now();
    // 如果系统启动时时间为0, 可能会测试失败
    if (timePoint1 < 1000) {
        OSALSystem::getInstance().sleep_ms(1000);
        timePoint1 = OSALChrono::getInstance().now();
    }
    OSALSystem::getInstance().sleep_ms(15);
    auto timePoint2 = OSALChrono::getInstance().now();
    auto interval = timePoint2 - timePoint1;
    EXPECT_TRUE(interval >= 10 && interval < 20);
#else
    GTEST_SKIP();
#endif
}

TEST(OSALChronoTest, TestOSALChronoElapsed) {
#if (TestOSALChronoElapsedEnabled)
    auto timePoint1 = OSALChrono::getInstance().now();
    OSALSystem::getInstance().sleep_ms(15);
    auto timePoint2 = OSALChrono::getInstance().now();
    auto duration = static_cast<float>(OSALChrono::getInstance().elapsed(timePoint1, timePoint2));
    EXPECT_TRUE(duration >= 0.01f && duration < 0.02f);  // 允许一些误差
#else
    GTEST_SKIP();
#endif
}

TEST(OSALChronoTest, TestOSALChronoToTimeT) {
#if (TestOSALChronoToTimeTEnabled)
    auto timePoint = OSALChrono::getInstance().now();
    std::time_t time = OSALChrono::getInstance().to_time_t(timePoint);
    EXPECT_TRUE(time > 0);
#else
    GTEST_SKIP();
#endif
}

TEST(OSALChronoTest, TestOSALChronoFromTimeT) {
#if (TestOSALChronoFromTimeTEnabled)
    std::time_t time = OSALChrono::getInstance().to_time_t(OSALChrono::getInstance().now());
    auto timePoint = OSALChrono::getInstance().from_time_t(time);
    EXPECT_TRUE(timePoint > 0);
#else
    GTEST_SKIP();
#endif
}

TEST(OSALChronoTest, TestOSALChronoToString) {
#if (TestOSALChronoToStringEnabled)
    auto timePoint = OSALChrono::getInstance().now();
    std::string timeString = OSALChrono::getInstance().to_string(timePoint);
    EXPECT_TRUE(!timeString.empty());
#else
    GTEST_SKIP();
#endif
}