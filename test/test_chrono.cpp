//
// Created by kamin.deng on 2024/8/23.
//
#include "osal_chrono.h"
#include "osal_system.h"
#include "test_framework.h"

using namespace osal;

TEST_CASE(TestOSALChronoNow) {
#if (TestOSALChronoNowEnabled)
    auto timePoint1 = OSALChrono::getInstance().now();
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
    auto duration = OSALChrono::getInstance().elapsed(timePoint1, timePoint2);
    OSAL_ASSERT_TRUE(duration >= 0.01f && duration < 0.02f);  // 允许一些误差
#endif
    return 0;  // 表示测试通过
}

TEST_CASE(TestOSALChronoToTimeT) {
#if (TestOSALChronoToTimeTEnabled)
    auto timePoint = OSALChrono::getInstance().now();
    std::time_t time = OSALChrono::getInstance().to_time_t(timePoint);
    OSAL_ASSERT_TRUE(time > 0);
#endif
    return 0;  // 表示测试通过
}

TEST_CASE(TestOSALChronoFromTimeT) {
#if (TestOSALChronoFromTimeTEnabled)
    std::time_t time = OSALChrono::getInstance().to_time_t(OSALChrono::getInstance().now());
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
