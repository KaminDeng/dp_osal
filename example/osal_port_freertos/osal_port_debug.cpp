//
// Created by kamin.deng on 2024/8/26.
//
#include <unistd.h>

#include "usart.h"
#include "osal_port_config.h"

void osal_port_debug_write(char* buf, uint32_t len) {
    HAL_UART_Transmit(&huart6, (const uint8_t *)buf, len, HAL_MAX_DELAY);
//    write(1, buf, len);
}

// 重定向标准输出/错误
extern "C" int _write(int fd, const void* buf, size_t count) {
    if (fd == STDOUT_FILENO || fd == STDERR_FILENO) {
        // 注意：需要强制转换const修饰符
        osal_port_debug_write((char*)buf, count);
        return count;
    }
    return -1; // 其他文件描述符不处理
}