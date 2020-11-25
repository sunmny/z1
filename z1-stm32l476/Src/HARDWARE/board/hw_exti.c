#include "hw_exti.h" 
#include "hw_config.h"
#include "stm32l4xx_hal.h"
#include "hw_gsensor.h"
#include "hw_heartrate.h"
#include "hw_key.h"
#include "hw_led.h"
#include "delay.h"
#include "FreeRTOS.h"
#include "task.h"
#include "hw_charge.h"
#include "stm32l4xx_hal_tim.h"
#include "app_ble.h"
#include "app_aoa.h"
extern TIM_HandleTypeDef    TimHandle;
/*

KEY_CALL_Pin

KEY_NEXT_Pin

PRECHG_Pin

CHARGE_Pin

KEY_SOS_Pin

GSENSOR_INT_Pin

HEARTRATE_SENSOR_INT_Pin;

SYSTEM_POWERON_IND_Pin;

*/
	  GPIO_PinState state;
		

void Sos_IRQHandler_Config(void)
{
  GPIO_InitTypeDef   GPIO_InitStructure;

  /* Enable GPIOC clock */
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /* Configure PC.13 pin as input floating */
  GPIO_InitStructure.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStructure.Pull = GPIO_NOPULL;
  GPIO_InitStructure.Pin = GPIO_PIN_5;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* Enable and set EXTI line 13 Interrupt to the lowest priority */
  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
}
void charge1_IRQHandler_Config(void)
{
  GPIO_InitTypeDef   GPIO_InitStructure;

  /* Enable GPIOC clock */
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /* Configure PC.13 pin as input floating */
  GPIO_InitStructure.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStructure.Pull = GPIO_NOPULL;
  GPIO_InitStructure.Pin = GPIO_PIN_15;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* Enable and set EXTI line 13 Interrupt to the lowest priority */
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
}
void charge_IRQHandler_Config(void)
{
  GPIO_InitTypeDef   GPIO_InitStructure;

  /* Enable GPIOC clock */
  __HAL_RCC_GPIOG_CLK_ENABLE();

  /* Configure PC.13 pin as input floating */
  GPIO_InitStructure.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStructure.Pull = GPIO_NOPULL;
  GPIO_InitStructure.Pin = GPIO_PIN_9;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStructure);

  /* Enable and set EXTI line 13 Interrupt to the lowest priority */
  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
}
void M10_ID_IRQHandler_Config(void)
{
  GPIO_InitTypeDef   GPIO_InitStructure;

  /* Enable GPIOC clock */
  __HAL_RCC_GPIOG_CLK_ENABLE();

  /* Configure PC.13 pin as input floating */
  GPIO_InitStructure.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStructure.Pull = GPIO_NOPULL;
  GPIO_InitStructure.Pin = GPIO_PIN_12;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStructure);

  /* Enable and set EXTI line 13 Interrupt to the lowest priority */
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
}
void Key_Charger_IRQHandler_Config(void)
{
  GPIO_InitTypeDef   GPIO_InitStructure;

  /* Enable GPIOC clock */
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /* Configure PC.13 pin as input floating */
  GPIO_InitStructure.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStructure.Pull = GPIO_PULLUP;
  GPIO_InitStructure.Pin = GPIO_PIN_4;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);

  /* Enable and set EXTI line 13 Interrupt to the lowest priority */
  HAL_NVIC_SetPriority(EXTI4_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI4_IRQn);
}
void hw_exti_init(void)
{    	 	
	Sos_IRQHandler_Config();
	Key_Charger_IRQHandler_Config();
	M10_ID_IRQHandler_Config();
	charge_IRQHandler_Config();
	charge1_IRQHandler_Config();
}


/**
  * @brief EXTI line detection callbacks
  * @param GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */

uint8_t charger_status = 0xff;
extern void aoa_at_handle_bsossend(void);
extern void set_charge_light(void);
extern void set_charge1_light(void);
extern uint8_t ble_send_response(uint8_t *data, uint16_t len);
extern struct zdevice_setting zdev_set;
extern uint8_t report_charing_soc(void);
extern uint8_t zdev_charger_status;
extern uint8_t bd1_exist;

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	uint8_t ret;
	
	
  if (GPIO_Pin == SYSTEM_POWERON_IND_Pin)//power in
  {
		//	hw_key_power_in_irq();
	//	HAL_GPIO_WritePin(CHARGE_KEY_GPIO_Port, CHARGE_KEY_Pin, GPIO_PIN_RESET);
		//set_charge_light();
		//set_charge1_light();
		HAL_TIM_Base_Start_IT(&TimHandle);
	//	HAL_TIM_Base_Start_IT(&TimHandle1);
		ft_dev.pwkey_flags = 1;

  }
  else if(GPIO_Pin == BD1_DETECT_Pin)
  {
  /*todo, notify the app*/
  	
  	if(GPIO_PIN_RESET==HAL_GPIO_ReadPin(BD1_DETECT_GPIO_Port, BD1_DETECT_Pin)){
		bd1_exist=1;
		}
		else{
		
		bd1_exist=0;
	}
  }else if(GPIO_Pin == GPIO_PIN_4){
		
		ret =HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_4);
		//printf("sos ret = %d \r\n",ret );
		if(ret == 0){
		ft_dev.soskey_flags = 1;
		}else if(ret ==1){
		
		ft_dev.soskey_flags = 0;
		}
		//set_charge_key_high();
	 //aoa_at_handle_bsossend();
	
	}else if(GPIO_Pin == GPIO_PIN_5){
	printf("gpio_pin11111111111111111111111111111 = %x \r\n",GPIO_Pin);
	
		if(charger_status ==0xff){
		charger_status=1;
		//set_charge_key_high();
			zdev_charger_status =1;
		}else if(charger_status ==1){
			charger_status=0;
			//set_charge_key_low();
			zdev_charger_status =0;
		}else if(charger_status ==0){
				charger_status=1;
			zdev_charger_status =1;
				//set_charge_key_high();
		}
	}else if(GPIO_Pin == GPIO_PIN_15){
		
	// aoa_at_handle_bsossend();
		if(GPIO_PIN_RESET==HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_15)){
			 // ble_send_response("FTCHARGKEY2OUT##",16);
				zdev_set.bat1_chr_status = 0;
				//report_charing_soc();
			}
			else{ 
			//	ble_send_response("FTCHARGKEY2IN##",15);
				zdev_set.bat1_chr_status = 1;
				//report_charing_soc();
			}
	}else if(GPIO_Pin == GPIO_PIN_9){
	
			if(GPIO_PIN_RESET==HAL_GPIO_ReadPin(GPIOG, GPIO_PIN_9)){
				//	ble_send_response("FTCHARGKEY1OUT##",16);
			zdev_set.bat_chr_status = 0;
			//	report_charing_soc();
			}
			else{ 
				//	ble_send_response("FTCHARGKEY1IN##",15);
				zdev_set.bat_chr_status = 1;
			//	report_charing_soc();
			}
	}
}




