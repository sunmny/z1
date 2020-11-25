#ifndef HARDWARE_ADC_H
#define HARDWARE_ADC_H

#include <stdint.h>
#include "stm32l4xx_hal.h"


/*
	// Definition for ADCx clock resources 
#define ADCx                            ADC1
#define ADCx_CLK_ENABLE()               __HAL_RCC_ADC_CLK_ENABLE()
#define ADCx_CHANNEL_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOB_CLK_ENABLE()

#define ADCx_FORCE_RESET()              __HAL_RCC_ADC_FORCE_RESET()
#define ADCx_RELEASE_RESET()            __HAL_RCC_ADC_RELEASE_RESET()

// Definition for ADCx Channel Pin 
#define ADCx_CHANNEL_PIN                GPIO_PIN_0
#define ADCx_CHANNEL_GPIO_PORT          GPIOB

// Definition for ADCx's Channel 
#define ADCx_CHANNEL                    ADC_CHANNEL_15
#define SAMPLINGTIME                    ADC_SAMPLETIME_6CYCLES_5
*/



void hw_adc_init(void);

//uint16_t hw_get_adc(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
uint16_t hw_get_adc(void);
uint8_t hw_battery_get(void);

#endif
