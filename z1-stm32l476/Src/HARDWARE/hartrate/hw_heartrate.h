
#ifndef PAH8002EP_H
#define PAH8002EP_H

#include <stdint.h>

#define HW_HEARTRATE_I2C_ADDR    0x2a  //0x2a //0x15  //I2C 7-bit ID
//#define I2C_ID_PAH8002  0x15  //I2C 7-bit ID




void hw_heartrate_init(void);


void hw_heartrate_irq(void);

	void hw_heartrate_test(void);
	void hw_heartrate_open(void);

	void hw_heartrate_close(void);
	void hw_heartrate_set_power_mode(uint8_t en);
	void hw_heartrate_set_suspend(void);
	 
#endif
