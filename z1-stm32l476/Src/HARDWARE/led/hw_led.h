#ifndef HARDWARE_LED_H
#define HARDWARE_LED_H

#include "stm32l4xx_hal.h"


void hw_led_init(void);

//±¨¾¯
GPIO_PinState hw_led_key_call_get(void);

void hw_led_key_call_set(GPIO_PinState state);

//Çó¾È
GPIO_PinState hw_led_key_sos_get(void);

void hw_led_key_sos_set(GPIO_PinState state);

//³äµç
GPIO_PinState hw_led_key_chg_red_get(void);

void hw_led_key_chg_red_set(GPIO_PinState state);

//³äÂú
GPIO_PinState hw_led_key_chg_green_get(void);
void hw_led_key_chg_green_set(GPIO_PinState state);

//lanse
GPIO_PinState hw_led_key_chg_blue_get(void);

void hw_led_key_chg_blue_set(GPIO_PinState state);
#endif
