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
