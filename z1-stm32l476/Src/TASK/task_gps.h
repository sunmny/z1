#ifndef TASK_GPS_H
#define TASK_GPS_H

#include "stdint.h"
#include "cmsis_os.h"

void GpsTask(void const * argument);

osStatus task_gps_message_put(uint32_t info);

#endif
