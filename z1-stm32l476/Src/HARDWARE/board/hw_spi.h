#ifndef HARDWARE_LED_H
#define HARDWARE_LED_H
#include "sys.h"
#include <stdint.h>

#define LED_KEY_R1_PORT								GPIOC
#define LED_KEY_R1_PIN								GPIO_Pin_8

#define LED_KEY_R2_PORT							  GPIOC
#define LED_KEY_R2_PIN								GPIO_Pin_9

#define LED_CHARGE_R_PORT							GPIOC
#define LED_CHARGE_R_PIN							GPIO_Pin_7

#define LED_CHARGE_G_PORT							GPIOB
#define LED_CHARGE_G_PIN							GPIO_Pin_3

#define ON     1
#define OFF    0


//LED端口定义
#define LED_KEY_R1 PCout(8)	// D2
#define LED_KEY_R2 PCout(9)	// D3	
#define LED_CHARGE_R PCout(7)	// D2
#define LED_CHARGE_G PBout(3)	// D3	 

void    hardware_led_init(void);//初始化		
uint8_t hardware_led_get(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void    hardware_led_set(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin,uint8_t level);

#endif
