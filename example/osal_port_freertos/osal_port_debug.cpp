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


