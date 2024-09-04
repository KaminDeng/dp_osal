//
// Created by kamin.deng on 2024/8/26.
//
#include <unistd.h>

#include "osal_port_config.h"

void osal_port_debug_write(char* buf, uint32_t len) {
    // Write the log message to file descriptor 1 (stdout)
    write(1, buf, len);
    // HAL_UART_Transmit(&huart6, (const uint8_t *)buf, len, HAL_MAX_DELAY);
}
