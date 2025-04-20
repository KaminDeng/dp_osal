/*
 * Copyright (c) 2024 kamin.deng
 * Email: kamin.deng@gmail.com
 * Created on 2024/8/23.
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

#include "osal_chrono.h"
#include "osal_system.h"
#include "test_framework.h"

using namespace osal;

TEST_CASE(TestOSALChronoNow) {
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
    OSAL_ASSERT_TRUE(interval >= 10 && interval < 20);
#endif
    return 0;  // 表示测试通过
}

TEST_CASE(TestOSALChronoElapsed) {
#if (TestOSALChronoElapsedEnabled)
    auto timePoint1 = OSALChrono::getInstance().now();
    OSALSystem::getInstance().sleep_ms(15);
    auto timePoint2 = OSALChrono::getInstance().now();
    auto duration = (float)OSALChrono::getInstance().elapsed(timePoint1, timePoint2);
    OSAL_ASSERT_TRUE(duration >= 0.01f && duration < 0.02f);  // 允许一些误差
#endif
    return 0;  // 表示测试通过
}

TEST_CASE(TestOSALChronoToTimeT) {
#if (TestOSALChronoToTimeTEnabled)
    auto timePoint = OSALChrono::getInstance().now();
    // 如果系统启动时时间为0, 可能会测试失败
    std::time_t time = OSALChrono::getInstance().to_time_t(timePoint);
    OSAL_ASSERT_TRUE(time > 0);
#endif
    return 0;  // 表示测试通过
}

TEST_CASE(TestOSALChronoFromTimeT) {
#if (TestOSALChronoFromTimeTEnabled)
    std::time_t time = OSALChrono::getInstance().to_time_t(OSALChrono::getInstance().now());
    // 如果系统启动时时间为0, 可能会测试失败
    auto timePoint = OSALChrono::getInstance().from_time_t(time);
    OSAL_ASSERT_TRUE(timePoint > 0);
#endif
    return 0;  // 表示测试通过
}

TEST_CASE(TestOSALChronoToString) {
#if (TestOSALChronoToStringEnabled)
    auto timePoint = OSALChrono::getInstance().now();
    std::string timeString = OSALChrono::getInstance().to_string(timePoint);
    OSAL_ASSERT_TRUE(!timeString.empty());
#endif
    return 0;  // 表示测试通过
}
