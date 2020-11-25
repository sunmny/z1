#ifndef HARDWARE_BD1_H
#define HARDWARE_BD1_H

#include "stm32l4xx_hal.h"



void hw_bd1_init(void);
void hw_bd1_open(void);

void hw_bd1_close(void);
uint8_t hw_bd1_get_power_status(void);
uint8_t hw_bd1_is_exist(void);

#endif
