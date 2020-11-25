#ifndef APP_KEY_H
#define APP_KEY_H

#include <stdint.h>

void app_key_power(uint8_t state);
void app_key_next(uint8_t state);
void app_key_sos(uint8_t state);
void app_key_call(uint8_t state);

#endif

