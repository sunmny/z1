#ifndef HARDWARE_GPS_H
#define HARDWARE_GPS_H

#include "stm32l4xx_hal.h"



void hw_gps_init(void);
void hw_gps_open(void);

void hw_gps_close(void);
uint8_t hw_gps_send(uint8_t *tx_buf, uint16_t tx_len);

uint8_t hw_gps_receive(uint8_t *rx_buf, uint16_t *rx_len,uint32_t timeout);


#endif


