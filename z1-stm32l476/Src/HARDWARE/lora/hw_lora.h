#ifndef HARDWARE_LORA_H
#define HARDWARE_LORA_H
#include <stdint.h>
#include "stm32l4xx_hal.h"



void hw_lora_power(GPIO_PinState state);

void hw_lora_reset(GPIO_PinState state);

//LED IO≥ı ºªØ
void hw_lora_init(void);

uint8_t hw_lora_send(uint8_t *tx_buf, uint16_t tx_len);

uint8_t hw_lora_receive(uint8_t *rx_buf, uint16_t *rx_len,uint32_t timeout);


void hw_lora_open(void);
void hw_lora_close(void);


#endif

