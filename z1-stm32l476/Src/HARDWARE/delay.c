#include "delay.h"
#include "stm32l4xx_hal.h"

void delay_us(uint32_t us)
{
	while(us--);
}

void delay_ms(uint32_t ms)
{
	uint32_t i=0;
	for(i=0;i<ms*1000;i++)
    delay_us(1);
}
	
				 
void delay_ms2(uint32_t ms)
{
   HAL_Delay(ms);
}

