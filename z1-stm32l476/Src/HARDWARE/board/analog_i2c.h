#ifndef HW_I2C_H
#define HW_I2C_H

#include <stdint.h>


uint8_t analog_i2c_read_data(uint8_t slave_addr,uint8_t *read_data, uint16_t start_addr, uint16_t data_length);
uint8_t analog_i2c_write_data(uint8_t slave_addr,uint8_t *write_data, uint16_t start_addr, uint16_t data_length);
	
#endif


