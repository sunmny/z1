#ifndef HARDWARE_CHARGE_H
#define HARDWARE_CHARGE_H
#include "stdint.h"

uint8_t hw_charge_detect(void);

void hw_charge_prechg(void);
void hw_charge_charge(void);

#endif
