#ifndef HARDWARE_RTC_H
#define HARDWARE_RTC_H

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



void hw_rtc_init(void);

//uint16_t hw_get_adc(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void hw_rtc_get_date_time(uint8_t *time,uint8_t *date);

void hw_rtc_date_time_detect(void);

uint8_t hw_rtc_set_date_time(uint8_t *time,uint8_t *date);

void hw_rtc_set_wakeup(uint16_t time);

#endif
