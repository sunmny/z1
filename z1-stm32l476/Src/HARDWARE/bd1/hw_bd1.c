#include "hw_bd1.h" 
#include "hw_config.h"
#include "stm32l4xx_hal.h"
#include "delay.h"
#include "cmsis_os.h"
#include "app_aoa.h"

static uint8_t bd1_power_status=0;

extern struct zdevice_setting zdev_set;
void hw_bd1_power(GPIO_PinState state)
{	
	if(state == GPIO_PIN_SET)
	{
		HAL_GPIO_WritePin(BD1_POWER_GPIO_Port, BD1_POWER_Pin, state);
		HAL_GPIO_WritePin(BD1_POWER_1_GPIO_Port, BD1_POWER_1_Pin, state);
		delay_ms(5);
	 	HAL_GPIO_WritePin(BD1_ENABLE_GPIO_Port, BD1_ENABLE_Pin, state);
		bd1_power_status=1;
	}
	else if (state == GPIO_PIN_RESET)
	{
	 	HAL_GPIO_WritePin(BD1_ENABLE_GPIO_Port, BD1_ENABLE_Pin, state);	
		HAL_GPIO_WritePin(BD1_POWER_GPIO_Port, BD1_POWER_Pin, state);
		HAL_GPIO_WritePin(BD1_POWER_1_GPIO_Port, BD1_POWER_1_Pin, state);
		bd1_power_status=0;
	}
}


void hw_bd1_open(void)
{
		
   	hw_bd1_power(GPIO_PIN_SET);
		zdev_set.status.zrd_status = 1;
	
}

void hw_bd1_close(void)
{
	//hw_bd1_power(GPIO_PIN_RESET);
	zdev_set.status.zrd_status = 0;
}

void hw_bd1_init(void)
{    	 	
    hw_bd1_open();
}

uint8_t hw_bd1_get_power_status(void)
{
	return bd1_power_status;
}

/*todo, 20181113*/
uint8_t hw_bd1_is_exist(void)
{
	__IO GPIO_PinState status;
	status=HAL_GPIO_ReadPin(BD1_DETECT_GPIO_Port, BD1_DETECT_Pin);
	if(status==GPIO_PIN_RESET)
		return 1;
	else
		return 0;
}