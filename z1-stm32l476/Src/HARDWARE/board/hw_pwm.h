
#ifndef HARDWARE_PWM_H
#define HARDWARE_PWM_H

#include <stdint.h>
#include "stm32l4xx_hal.h"

void hw_pwm_init(void);
void MX_TIME4_EN(uint8_t en);

#endif
