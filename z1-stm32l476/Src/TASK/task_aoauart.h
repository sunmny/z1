#ifndef TASK_AOAUART_H
#define TASK_AOAUART_H

#include "stdint.h"
#include "cmsis_os.h"


void AoaUartTask(void const * argument);

osStatus task_aoauart_message_put(uint32_t info);

#endif
