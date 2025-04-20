/*
 * Copyright (c) 2024 kamin.deng
 * Email: kamin.deng@gmail.com
 * Created on 2024/8/26.
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

#ifndef OSAL_PORT_CONFIG_H
#define OSAL_PORT_CONFIG_H

#include <cmsis_os2.h>
#include <stdint.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#define OSAL_CONFIG_THREAD_MINIMAL_STACK_SIZE 512
#define OSAL_CONFIG_THREAD_DEFAULT_PRIORITY osPriorityNormal

// 如果没有实现CMSIS-RTOS v2，则将这些函数定义为空
// zephyr CMSIS-RTOS v2
// 实现相关信息：https://docs.zephyrproject.org/latest/services/portability/cmsis_rtos_v2.html
#define osKernelInitialize() ((void)0)
#define osKernelStart() ((void)0)
//#define osKernelInitialize() { printk("osKernelInitialize is not supported\n"); while(1); }
//#define osKernelStart() { printk("osKernelStart is not supported\n"); while(1); }
#define osKernelGetState() { printk("osKernelGetState is not supported\n"); while(1); }
#define osKernelSuspend() { printk("osKernelSuspend is not supported\n"); while(1); }
#define osKernelResume() { printk("osKernelResume is not supported\n"); while(1); }

void osal_port_debug_write(char *buf, uint32_t len);

#endif // OSAL_PORT_CONFIG_H
