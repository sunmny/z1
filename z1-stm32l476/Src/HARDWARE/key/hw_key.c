#include "hw_key.h" 
//#include "hw_config.h"
#include "hw_config.h"

#include "delay.h"

#include "app_key.h"
#include "app_power.h"

#define KEY_DELAY_MS 60


void hw_key_init(void)
{    	 

}

void hw_key_power_in_irq(void)
{
	power_timer_en(1);
}


