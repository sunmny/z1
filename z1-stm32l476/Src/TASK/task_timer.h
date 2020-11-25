#ifndef TASK_TIMER_H
#define TASK_TIMER_H
#include <stdint.h>

void task_timer_en(uint8_t en);
void RefreshTimerCallback(void const * argument);

#endif
