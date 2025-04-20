/*
 * Copyright (c) 2024 kamin.deng
 * Email: kamin.deng@gmail.com
 * Created on 2024/8/22.
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

#ifndef OSAL_SYSTEM_H_
#define OSAL_SYSTEM_H_

#include "osal.h"
#include "interface_system.h"

namespace osal {

class OSALSystem : public ISystem {
public:
    static OSALSystem &getInstance() {
        static OSALSystem instance;
        return instance;
    }

    void StartScheduler() override {
        osKernelInitialize();
        osKernelStart();
    }

    void sleep_ms(const uint32_t milliseconds) const override { osDelay(milliseconds); }

    void sleep(const uint32_t seconds) const override { osDelay(seconds * 1000); }

    [[nodiscard]] const char *get_system_info() const override {
        // Return some basic system information
        return "CMSIS-RTOS2 System";
    }

private:
    OSALSystem(){};

    ~OSALSystem(){};
};

}  // namespace osal
#endif  // OSAL_SYSTEM_H_
