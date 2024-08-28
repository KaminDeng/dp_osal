//
// Created by kamin.deng on 2024/8/26.
//

#ifndef OSAL_PORT_CONFIG_H
#define OSAL_PORT_CONFIG_H

#include <stdint.h>

#include "cmsis_os.h"
#define OSAL_CONFIG_THREAD_MINIMAL_STACK_SIZE configMINIMAL_STACK_SIZE

void osal_port_debug_write(char* buf, uint32_t len);

#endif  // OSAL_PORT_CONFIG_H
