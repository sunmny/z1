#ifndef TASK_BLE_H
#define TASK_BLE_H

#include "stdint.h"
#include "cmsis_os.h"



void BleTask(void const * argument);
osStatus task_ble_message_put(uint32_t info);
osStatus task_ble_mail_put(uint8_t *data, uint16_t len);
#endif
