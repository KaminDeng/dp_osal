//
// Created by kamin.deng on 2024/8/26.
//

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
