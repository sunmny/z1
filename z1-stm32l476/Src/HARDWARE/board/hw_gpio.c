#include "hw_config.h"
#include "hw_gpio.h"
#include "stm32l4xx_hal.h"



/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
*/

static void delay_ms(uint32_t ms)
{	
		uint32_t i ,j;
		
		for(i =0;i<ms ;i++){
					for(j =0;j<1000;j++);
		
		}

}

void set_ble_power(void)
{
	HAL_GPIO_WritePin(BLE_POWER_GPIO_Port, BLE_POWER_Pin, GPIO_PIN_SET);

}
void set_ble_power_off(void)
{
	HAL_GPIO_WritePin(BLE_POWER_GPIO_Port, BLE_POWER_Pin, GPIO_PIN_RESET);

}
void reset_uart_to_usb(void)
{
	 HAL_GPIO_WritePin(CH9343_POWER_GPIO_Port, CH9343_POWER_Pin, GPIO_PIN_RESET);
 // HAL_GPIO_WritePin(VCHG_OUT_CH9343_GPIO_Port, VCHG_OUT_CH9343_Pin, GPIO_PIN_RESET);
	delay_ms(1000);
	 HAL_GPIO_WritePin(CH9343_POWER_GPIO_Port, CH9343_POWER_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(VCHG_OUT_CH9343_GPIO_Port, VCHG_OUT_CH9343_Pin, GPIO_PIN_SET);

}


uint8_t bd1_exist=0;


static void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOG_CLK_ENABLE();
  HAL_PWREx_EnableVddIO2();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, BD1_POWER_Pin |BD2_POWER_Pin|VCHG_OUT_CH9343_Pin|VCHG_POWER_Pin|SYSTEM_POWER_LOCK_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOG, BD1_ENABLE_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, DEBUG_LED_1_Pin|DEBUG_LED_2_Pin|/*BD2_RESET_Pin|*/CH9343_POWER_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, BLE_POWER_Pin|BLE_RESET_Pin|BD1_POWER_1_Pin, GPIO_PIN_RESET);


  HAL_GPIO_WritePin(GPIOA,CHARGE_KEY_Pin,GPIO_PIN_SET);

  GPIO_InitStruct.Pin = DEBUG_LED_1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(DEBUG_LED_1_GPIO_Port, &GPIO_InitStruct);
  HAL_GPIO_WritePin(DEBUG_LED_1_GPIO_Port, DEBUG_LED_1_Pin, GPIO_PIN_SET);
	
	
		  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, SCL_Pin|SDA_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : PBPin PBPin */
  GPIO_InitStruct.Pin = SCL_Pin|SDA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	
	

  GPIO_InitStruct.Pin = DEBUG_LED_2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(DEBUG_LED_2_GPIO_Port, &GPIO_InitStruct);
  HAL_GPIO_WritePin(DEBUG_LED_2_GPIO_Port, DEBUG_LED_2_Pin, GPIO_PIN_SET);

  GPIO_InitStruct.Pin = BD1_POWER_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(BD1_POWER_GPIO_Port, &GPIO_InitStruct);
  HAL_GPIO_WritePin(BD1_POWER_GPIO_Port, BD1_POWER_Pin, GPIO_PIN_RESET);

  GPIO_InitStruct.Pin = BD1_POWER_1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(BD1_POWER_1_GPIO_Port, &GPIO_InitStruct);
  HAL_GPIO_WritePin(BD1_POWER_1_GPIO_Port, BD1_POWER_1_Pin, GPIO_PIN_RESET);


  GPIO_InitStruct.Pin = BD1_ENABLE_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(BD1_ENABLE_GPIO_Port, &GPIO_InitStruct);
  HAL_GPIO_WritePin(BD1_ENABLE_GPIO_Port, BD1_ENABLE_Pin, GPIO_PIN_RESET);

  GPIO_InitStruct.Pin = BD2_POWER_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(BD2_POWER_GPIO_Port, &GPIO_InitStruct);
  HAL_GPIO_WritePin(BD2_POWER_GPIO_Port, BD2_POWER_Pin, GPIO_PIN_RESET);

  GPIO_InitStruct.Pin = BD2_RESET_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(BD2_RESET_GPIO_Port, &GPIO_InitStruct);
  HAL_GPIO_WritePin(BD2_RESET_GPIO_Port, BD2_RESET_Pin, GPIO_PIN_RESET);

  GPIO_InitStruct.Pin = CHARGE_KEY_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
 HAL_GPIO_Init(CHARGE_KEY_GPIO_Port, &GPIO_InitStruct);
  HAL_GPIO_WritePin(CHARGE_KEY_GPIO_Port, CHARGE_KEY_Pin, GPIO_PIN_RESET);

  delay_ms(500);

 HAL_GPIO_WritePin(CHARGE_KEY_GPIO_Port, CHARGE_KEY_Pin, GPIO_PIN_SET);
   delay_ms(500);
HAL_GPIO_WritePin(CHARGE_KEY_GPIO_Port, CHARGE_KEY_Pin, GPIO_PIN_RESET);



  GPIO_InitStruct.Pin = CHARGE1_KEY_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
 HAL_GPIO_Init(CHARGE1_KEY_GPIO_Port, &GPIO_InitStruct);
  HAL_GPIO_WritePin(CHARGE1_KEY_GPIO_Port, CHARGE1_KEY_Pin, GPIO_PIN_SET);

  delay_ms(500);

 HAL_GPIO_WritePin(CHARGE1_KEY_GPIO_Port, CHARGE1_KEY_Pin, GPIO_PIN_RESET);
   delay_ms(500);
HAL_GPIO_WritePin(CHARGE1_KEY_GPIO_Port, CHARGE1_KEY_Pin, GPIO_PIN_SET);



  GPIO_InitStruct.Pin = VCHG_OUT_CH9343_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(VCHG_OUT_CH9343_GPIO_Port, &GPIO_InitStruct);
  HAL_GPIO_WritePin(VCHG_OUT_CH9343_GPIO_Port, VCHG_OUT_CH9343_Pin, GPIO_PIN_SET);

  GPIO_InitStruct.Pin = CH9343_POWER_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(CH9343_POWER_GPIO_Port, &GPIO_InitStruct);
  HAL_GPIO_WritePin(CH9343_POWER_GPIO_Port, CH9343_POWER_Pin, GPIO_PIN_SET);

  GPIO_InitStruct.Pin = VCHG_POWER_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(VCHG_POWER_GPIO_Port, &GPIO_InitStruct);
  HAL_GPIO_WritePin(VCHG_POWER_GPIO_Port, VCHG_POWER_Pin, GPIO_PIN_RESET);

  GPIO_InitStruct.Pin = SYSTEM_POWER_LOCK_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(SYSTEM_POWER_LOCK_GPIO_Port, &GPIO_InitStruct);
  HAL_GPIO_WritePin(SYSTEM_POWER_LOCK_GPIO_Port, SYSTEM_POWER_LOCK_Pin, GPIO_PIN_RESET);

  GPIO_InitStruct.Pin = SYSTEM_POWERON_IND_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(SYSTEM_POWERON_IND_GPIO_Port, &GPIO_InitStruct);


  GPIO_InitStruct.Pin = RD_POWERLIGHT_SET_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(RD_POWERLIGHT_SET_GPIO_Port, &GPIO_InitStruct);
	 HAL_GPIO_WritePin(RD_POWERLIGHT_SET_GPIO_Port, RD_POWERLIGHT_SET_Pin, GPIO_PIN_SET);
	
	
	GPIO_InitStruct.Pin = LED_9523_RESET_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED_9523_RESET_GPIO_Port, &GPIO_InitStruct);
  HAL_GPIO_WritePin(LED_9523_RESET_GPIO_Port, LED_9523_RESET_Pin, GPIO_PIN_SET);
	
	
  GPIO_InitStruct.Pin = BLE_POWER_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(BLE_POWER_GPIO_Port, &GPIO_InitStruct);
  HAL_GPIO_WritePin(BLE_POWER_GPIO_Port, BLE_POWER_Pin, GPIO_PIN_SET);
	
	
	
  GPIO_InitStruct.Pin = BD1_DETECT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(BD1_DETECT_GPIO_Port, &GPIO_InitStruct);
	GPIO_InitStruct.Pin = SD_POWER_IND_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(SD_POWER_IND_GPIO_Port, &GPIO_InitStruct);
  HAL_GPIO_WritePin(SD_POWER_IND_GPIO_Port, SD_POWER_IND_Pin, GPIO_PIN_SET);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
  HAL_NVIC_SetPriority(EXTI3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI3_IRQn);

#if 0
  HAL_NVIC_SetPriority(EXTI0_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);

  HAL_NVIC_SetPriority(EXTI1_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(EXTI1_IRQn);

  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
#endif
}


void hw_gpio_init(void)
{
   MX_GPIO_Init();
	
	bd1_exist = HAL_GPIO_ReadPin(BD1_DETECT_GPIO_Port, BD1_DETECT_Pin);
}

void hw_gpio_deinit_irq(void)
{

//GPIO_InitTypeDef GPIO_InitStruct;
	
	
	/*
  GPIO_InitStruct.Pin = KEY_NEXT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(KEY_NEXT_GPIO_Port, &GPIO_InitStruct);

	
		GPIO_InitStruct.Pin = HEARTRATE_SENSOR_INT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);


  GPIO_InitStruct.Pin = GSENSOR_INT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	
			GPIO_InitStruct.Pin = PRECHG_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	 GPIO_InitStruct.Pin = CHARGE_Pin;
   GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
   GPIO_InitStruct.Pull = GPIO_NOPULL;
   HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	*/
	
	 
/*HAL_GPIO_DeInit(KEY_NEXT_GPIO_Port,KEY_NEXT_Pin);
HAL_GPIO_DeInit(GPIOC,HEARTRATE_SENSOR_INT_Pin);
HAL_GPIO_DeInit(GPIOC,GSENSOR_INT_Pin);*/
//HAL_GPIO_DeInit(GPIOB,PRECHG_Pin);
//HAL_GPIO_DeInit(GPIOB,CHARGE_Pin);


}

void set_charge_key_high42(void)
{
		HAL_GPIO_WritePin(VCHG_OUT_CH9343_GPIO_Port, VCHG_OUT_CH9343_Pin, GPIO_PIN_SET);

}

void set_charge_key_low42(void)
{
	HAL_GPIO_WritePin(VCHG_OUT_CH9343_GPIO_Port, VCHG_OUT_CH9343_Pin, GPIO_PIN_RESET);
}
void set_charge_key_high(void)
{
// HAL_GPIO_WritePin(CHARGE_KEY_GPIO_Port, CHARGE_KEY_Pin, GPIO_PIN_RESET);

 // delay_ms(500);

// HAL_GPIO_WritePin(CHARGE_KEY_GPIO_Port, CHARGE_KEY_Pin, GPIO_PIN_SET);
//	delay_ms(500);
 //HAL_GPIO_WritePin(CHARGE_KEY_GPIO_Port, CHARGE_KEY_Pin, GPIO_PIN_RESET);
	
	HAL_GPIO_WritePin(VCHG_POWER_GPIO_Port, VCHG_POWER_Pin, GPIO_PIN_SET);
	printf("pb12 set hight\r\n");
}
void set_charge_light(void)
{


	//HAL_GPIO_WritePin(CHARGE_KEY_GPIO_Port, CHARGE_KEY_Pin, GPIO_PIN_RESET);
		//delay_ms(500);
 HAL_GPIO_WritePin(CHARGE_KEY_GPIO_Port, CHARGE_KEY_Pin, GPIO_PIN_SET);
	delay_ms(700);
	HAL_GPIO_WritePin(CHARGE_KEY_GPIO_Port, CHARGE_KEY_Pin, GPIO_PIN_RESET);

}


void set_charge_light1(void)
{


	//HAL_GPIO_WritePin(CHARGE_KEY_GPIO_Port, CHARGE_KEY_Pin, GPIO_PIN_RESET);
		//delay_ms(500);
 HAL_GPIO_WritePin(CHARGE_KEY_GPIO_Port, CHARGE_KEY_Pin, GPIO_PIN_SET);
	delay_ms(700);
	HAL_GPIO_WritePin(CHARGE_KEY_GPIO_Port, CHARGE_KEY_Pin, GPIO_PIN_RESET);
	//delay_ms(700);
	//HAL_GPIO_WritePin(CHARGE_KEY_GPIO_Port, CHARGE_KEY_Pin, GPIO_PIN_SET);
}

void set_charge1_light(void)
{

	
	HAL_GPIO_WritePin(CHARGE1_KEY_GPIO_Port, CHARGE1_KEY_Pin, GPIO_PIN_SET);
	delay_ms(500);
 HAL_GPIO_WritePin(CHARGE1_KEY_GPIO_Port, CHARGE1_KEY_Pin, GPIO_PIN_RESET);
	delay_ms(500);
	HAL_GPIO_WritePin(CHARGE1_KEY_GPIO_Port, CHARGE1_KEY_Pin, GPIO_PIN_SET);

}
void set_charge_key_low(void)
{
// HAL_GPIO_WritePin(CHARGE_KEY_GPIO_Port, CHARGE_KEY_Pin, GPIO_PIN_RESET);

 // delay_ms(500);

// HAL_GPIO_WritePin(CHARGE_KEY_GPIO_Port, CHARGE_KEY_Pin, GPIO_PIN_SET);
//	delay_ms(500);
 //HAL_GPIO_WritePin(CHARGE_KEY_GPIO_Port, CHARGE_KEY_Pin, GPIO_PIN_RESET);
	
	HAL_GPIO_WritePin(VCHG_POWER_GPIO_Port, VCHG_POWER_Pin, GPIO_PIN_RESET);
printf("pb12 set low\r\n");
}
void hw_gpio_init_irq(void)
{
/*
GPIO_InitTypeDef GPIO_InitStruct;
	
	
	
  GPIO_InitStruct.Pin = KEY_NEXT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(KEY_NEXT_GPIO_Port, &GPIO_InitStruct);

	
		GPIO_InitStruct.Pin = HEARTRATE_SENSOR_INT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);


  GPIO_InitStruct.Pin = GSENSOR_INT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	*/
	/*
			GPIO_InitStruct.Pin = PRECHG_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	 GPIO_InitStruct.Pin = CHARGE_Pin;
   GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
   GPIO_InitStruct.Pull = GPIO_NOPULL;
   HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	*/
	
	/* 
HAL_GPIO_DeInit(KEY_NEXT_GPIO_Port,KEY_NEXT_Pin);
HAL_GPIO_DeInit(GPIOC,HEARTRATE_SENSOR_INT_Pin);
HAL_GPIO_DeInit(GPIOC,GSENSOR_INT_Pin);
HAL_GPIO_DeInit(GPIOB,PRECHG_Pin);
HAL_GPIO_DeInit(GPIOB,CHARGE_Pin);
*/

}

