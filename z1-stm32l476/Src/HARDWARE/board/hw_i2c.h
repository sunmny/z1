#ifndef HARDWARE_I2C_H
#define HARDWARE_I2C_H

#include <stdint.h>

#define  I2C_1   0
#define  I2C_2   1

void hw_i2c_init(void);
void hw_i2c_deinit(void);
uint8_t hw_i2c_read(uint8_t i2c_nub, uint16_t i2c_addr, uint8_t *rx_buf,uint16_t rx_len);
uint8_t hw_i2c_write(uint8_t i2c_nub, uint16_t i2c_addr, uint8_t *tx_buf, uint16_t tx_len);
uint8_t hw_i2c_write_read(uint8_t i2c_nub, uint16_t i2c_addr, uint8_t *tx_buf, uint16_t tx_len,uint8_t *rx_buf, uint16_t rx_len);
	
#endif
