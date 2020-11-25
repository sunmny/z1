#include "hw_rtc.h"
#include "hw_config.h"
#include "stm32l4xx_hal.h"
#include "hw_pwm.h"
//#include "gui_message.h"

static uint8_t htim4_flag = 0;

TIM_HandleTypeDef htim4;

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* TIM4 init function */
static void MX_TIM4_Init(uint32_t pulse)
{
/*
  TIM_MasterConfigTypeDef sMasterConfig;
  TIM_OC_InitTypeDef sConfigOC;

  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 400-1;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 5500-1;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  if (HAL_TIM_PWM_Init(&htim4) != HAL_OK)
  {
   // Error_Handler();
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
  {
    //Error_Handler();
  }

  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = pulse;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
  {
   // Error_Handler();
  }

  HAL_TIM_MspPostInit(&htim4);
*/
}

void MX_TIME4_EN(uint8_t en)
{
	if(en == 1)
	{
		if(htim4_flag == 0)
		{
          /* Start channel 1 */ 
        while(HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_4) != HAL_OK)
			
         {
           /* PWM Generation Error */
           // Error_Handler();
         }
	  	 htim4_flag = 1;
	  }
}else{

	      if(htim4_flag == 1)
		    {
	         /* Start channel 1 */
          while (HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_4) != HAL_OK)
					
         {
           /* PWM Generation Error */
          // Error_Handler();
         }
				htim4_flag = 0; 
			 }
		 
  }
}

void hw_pwm_init(void)
{
  MX_TIM4_Init((2500-1)/2);
  //MX_TIME4_EN(1);
}

