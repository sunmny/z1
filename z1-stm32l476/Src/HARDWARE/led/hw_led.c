#include "hw_led.h" 
#include "hw_config.h"




/*
#define LED_CHG_GREEN_Pin GPIO_PIN_3
#define LED_CHG_GREEN_GPIO_Port GPIOB

#define LED_KEY_R1_Pin GPIO_PIN_12
#define LED_KEY_R1_GPIO_Port GPIOA

#define LED_KEY_R2_Pin GPIO_PIN_9
#define LED_KEY_R2_GPIO_Port GPIOC

#define LED_CHG_RED_Pin GPIO_PIN_7
#define LED_CHG_RED_GPIO_Port GPIOC
*/

void hw_led_init(void)
{  
   // hw_led_key_chg_green_set(GPIO_PIN_RESET);
}


//±¨¾¯
GPIO_PinState hw_led_key_call_get(void)
{
  return  HAL_GPIO_ReadPin(LED_KEY_R2_GPIO_Port, LED_KEY_R2_Pin);
}


void hw_led_key_call_set(GPIO_PinState state)
{
    HAL_GPIO_WritePin(LED_KEY_R2_GPIO_Port, LED_KEY_R2_Pin, state);	
}


//Çó¾È
GPIO_PinState hw_led_key_sos_get(void)
{
  return  HAL_GPIO_ReadPin(LED_KEY_R1_GPIO_Port, LED_KEY_R1_Pin);
}


void hw_led_key_sos_set(GPIO_PinState state)
{
    HAL_GPIO_WritePin(LED_KEY_R1_GPIO_Port, LED_KEY_R1_Pin, state);	
}


//³äµç
GPIO_PinState hw_led_key_chg_red_get(void)
{
  return  HAL_GPIO_ReadPin(LED_CHG_RED_GPIO_Port, LED_CHG_RED_Pin);
}


void hw_led_key_chg_red_set(GPIO_PinState state)
{
    HAL_GPIO_WritePin(LED_CHG_RED_GPIO_Port, LED_CHG_RED_Pin, state);	
}


//³äÂú
GPIO_PinState hw_led_key_chg_green_get(void)
{
  return  HAL_GPIO_ReadPin(LED_CHG_GREEN_GPIO_Port, LED_CHG_GREEN_Pin);
}


void hw_led_key_chg_green_set(GPIO_PinState state)
{
    HAL_GPIO_WritePin(LED_CHG_GREEN_GPIO_Port, LED_CHG_GREEN_Pin, state);	
}

//lanse
GPIO_PinState hw_led_key_chg_blue_get(void)
{
  return  HAL_GPIO_ReadPin(LED_CHG_GREEN_GPIO_Port, LED_CHG_GREEN_Pin);
}


void hw_led_key_chg_blue_set(GPIO_PinState state)
{
    //HAL_GPIO_WritePin(LED_CHG_GREEN_GPIO_Port, LED_CHG_GREEN_Pin, state);	
	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, state);	
}




