#ifndef TASK_REFRESH_H
#define TASK_REFRESH_H
#include "stdint.h"
#include "cmsis_os.h"

void RefreshTask(void const * argument);
osStatus task_refresh_message_put(uint32_t info);

osEvent task_refresh_message_get(void);

#endif
