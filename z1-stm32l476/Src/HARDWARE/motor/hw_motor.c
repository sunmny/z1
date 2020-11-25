#include "hw_motor.h" 
#include "hw_config.h"
#include "stm32l4xx_hal.h"
#include "hw_pwm.h"
#include "delay.h"

//TIM_HandleTypeDef htim4;

//TIM_HandleTypeDef    TimHandle;
//TIM_HandleTypeDef htim4;

/* Timer Output Compare Configuration Structure declaration */
//TIM_OC_InitTypeDef sConfig;

/* Counter Prescaler value */
uint32_t uhPrescalerValue = 0;

#define  PERIOD_VALUE       (uint32_t)(666 - 1)  /* Period Value  */
#define  PULSE1_VALUE       (uint32_t)(PERIOD_VALUE/2)        /* Capture Compare 1 Value  */
#define  PULSE2_VALUE       (uint32_t)(PERIOD_VALUE*37.5/100) /* Capture Compare 2 Value  */
#define  PULSE3_VALUE       (uint32_t)(PERIOD_VALUE/4)        /* Capture Compare 3 Value  */
#define  PULSE4_VALUE       (uint32_t)(PERIOD_VALUE*12.5/100) /* Capture Compare 4 Value  */




void hw_motor_init(void)
{
	//MX_TIM4_Init();
	//HAL_TIM_PWM_Start(&TimHandle, TIM_CHANNEL_4);
	
	
}	



uint8_t hw_motor_set(uint8_t nub)
{
	uint8_t i = 0;
  for(i = 0;i<nub;i++)
	{
		//MX_TIME4_EN(1);
		//delay_us(1);
		//MX_TIME4_EN(0);
		//delay_ms(5);
	// osDelay(1000);
		//#define MOTOR_PWM_Pin GPIO_PIN_9
    //#define MOTOR_PWM_GPIO_Port GPIOB
		HAL_GPIO_WritePin(MOTOR_PWM_GPIO_Port, GPIO_PIN_9, GPIO_PIN_SET);	
		delay_ms(800);
		HAL_GPIO_WritePin(MOTOR_PWM_GPIO_Port, GPIO_PIN_9, GPIO_PIN_RESET);	
		delay_ms(50);
	}
	return 0;
}








