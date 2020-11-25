#include "app_power.h"
#include "hw_rtc.h"
#include "delay.h"
#include "hw_config.h"
#include "app_meerwespe.h"
#include "cmsis_os.h"
#include "hw_adc.h"
#include "hw_gps.h"
#include "hw_rtc.h"
#include "task_timer.h"
#include "stm32l4xx_hal_def.h"
#include "hw_uart.h"
#include "hw_i2c.h"
#include "hw_gpio.h"
#include "hw_dma.h"
#include "main.h"

//extern osTimerId PowerTimerHandle;
//extern osTimerId LedTimerHandle;


static uint8_t PowerTimerStatus=0;

static void delay_ms(uint32_t ms)
{
		int i;
		for(i = 0;i<ms;i++);

}

void app_power_open(void)
{
  HAL_GPIO_WritePin(SYSTEM_POWER_LOCK_GPIO_Port, SYSTEM_POWER_LOCK_Pin, GPIO_PIN_SET);
}

void app_power_close(void)
{
	
	
			//	HAL_GPIO_WritePin(CHARGE_KEY_GPIO_Port, CHARGE_KEY_Pin, GPIO_PIN_SET);
				//delay_ms(1000000);
			//HAL_GPIO_WritePin(CHARGE1_KEY_GPIO_Port, CHARGE1_KEY_Pin, GPIO_PIN_RESET);
			//	delay_ms(1000000);
			//	HAL_GPIO_WritePin(CHARGE_KEY_GPIO_Port, CHARGE_KEY_Pin, GPIO_PIN_SET); 
			//delay_ms(1000000);
		//	HAL_GPIO_WritePin(CHARGE1_KEY_GPIO_Port, CHARGE1_KEY_Pin, GPIO_PIN_RESET);
	// delay_ms(1000000);
set_charge_key_low();
			set_charge_key_low42();
  HAL_GPIO_WritePin(SYSTEM_POWER_LOCK_GPIO_Port, SYSTEM_POWER_LOCK_Pin, GPIO_PIN_RESET);
}

void power_timer_en(uint8_t en)
{
	if (PowerTimerStatus==1)
	{
		if(en == 1)
		{
			//osTimerStop (PowerTimerHandle);
		//	osTimerStart (PowerTimerHandle, 3000);
		}else{
			//osTimerStop (PowerTimerHandle);
		}
	}
	else if (PowerTimerStatus==0)
	{
		if(en == 1)
		{
//			osTimerStart (PowerTimerHandle, 3000);
		}else{
		}
	}
}


void PowerTimerCallback(void const * argument)
{
	GPIO_PinState status;
	status=HAL_GPIO_ReadPin(SYSTEM_POWER_LOCK_GPIO_Port, SYSTEM_POWER_LOCK_Pin);

	if(status==GPIO_PIN_RESET)
		app_power_open();
	else
		app_power_close();
}
extern uint16_t battery;
extern void aw9523_init(void);
static uint8_t led[3] = {0x24,0x25,0x26};
extern void aw9523_set_light(uint8_t count,uint8_t val);
uint8_t lte_connect_flag = 0;
extern void send_bd1_get_card_num(void);
