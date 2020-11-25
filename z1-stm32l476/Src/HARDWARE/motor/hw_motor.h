#ifndef HARDWARE_MOTOR_H
#define HARDWARE_MOTOR_H
#include <stdint.h>
#include "stm32l4xx_hal.h"

void hw_motor_init(void);
uint8_t hw_motor_set(uint8_t nub);

#endif
