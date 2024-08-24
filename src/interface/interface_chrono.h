//
// Created by kamin.deng on 2024/8/23.
//
#ifndef ICHRONO_H_
#define ICHRONO_H_

#include <string>

namespace osal {

class IChrono {
public:
    // 定义具体的类型
    using TimePoint = uint32_t;  // 使用内核计时器的滴答计数作为时间点
    using Duration = double;     // 使用秒作为时间间隔单位

    // 获取当前时间点
    [[nodiscard]] virtual TimePoint now() const = 0;

    // 计算两个时间点之间的时间间隔
    [[nodiscard]] virtual Duration elapsed(const TimePoint &start, const TimePoint &end) const = 0;

    // 将时间点转换为time_t类型
    [[nodiscard]] virtual std::time_t to_time_t(const TimePoint &timePoint) const = 0;

    // 将time_t类型转换为时间点
    [[nodiscard]] virtual TimePoint from_time_t(std::time_t time) const = 0;

    // 将时间点转换为字符串
    [[nodiscard]] virtual std::string to_string(const TimePoint &timePoint) const = 0;
};

}  // namespace osal
#endif  // ICHRONO_H_
