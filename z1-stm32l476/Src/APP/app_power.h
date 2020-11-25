#ifndef APP_POWER_H
#define APP_POWER_H
#include <stdint.h>
	
void app_power_open(void);

void app_power_close(void);

void PowerTimerCallback(void const * argument);
void power_timer_en(uint8_t en);
void led_timer_en(uint8_t en);

void rdTimerCallback(void const * argument);
#endif

