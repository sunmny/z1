#include "hw_adc.h"
#include "hw_config.h"
#include "stm32l4xx_hal.h"
#include "string.h"
#include "hw_led.h"
#include "cmsis_os.h"
#include "hw_charge.h"
#include "app_meerwespe.h"
#include "hw_lora.h"
#include "hw_gps.h"
#include "app_lcd.h"
#include "delay.h"
#include "app_power.h"



/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;
__IO uint16_t uhADCxConvertedValue = 0;

static uint16_t adc_value[10]={0x00};
//static uint8_t first_flag = 0;
//static uint16_t adc_vlue_last = 0;

uint8_t battery_level_show[4]; 

extern uint8_t charge_state;

static uint8_t first_flag2 = 0;

extern uint8_t send_battery_level;

extern  uint8_t charge_state ;

static uint16_t adc_level_last = 0;
static uint8_t  adc_level_0_count = 0 ;

/*
???? ????? 
4.031  100 
3.988  95 
3.947  90 
3.908  85 
3.873  80 
3.841  75 
3.811  70 
3.783  65 
3.759  60 
3.738  55 
3.719  50 
3.703  45 
3.691  40 
3.679  35 
3.668  30 
3.655  25 
3.635  20 
3.606  15 
3.556  10 
3.486  5 
3.411  0 


*/

uint16_t battery_level[11][2]={
{0,3411},
{10,3556},
{20,3635},
{30,3668},
{40,3691},
{50,3719},
{60,3759},
{70,3811},
{80,3873},
{90,3947},
{100,4031},
};


/* ADC1 init function */
static void MX_ADC1_Init(void)
{

  ADC_MultiModeTypeDef multimode;
  ADC_ChannelConfTypeDef sConfig;

    /**Common config 
    */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc1.Init.LowPowerAutoWait = DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.NbrOfDiscConversion = 1;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  hadc1.Init.OversamplingMode = DISABLE;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    //Error_Handler();
  }

    /**Configure the ADC multi-mode 
    */
  multimode.Mode = ADC_MODE_INDEPENDENT;
  if (HAL_ADCEx_MultiModeConfigChannel(&hadc1, &multimode) != HAL_OK)
  {
    //Error_Handler();
  }

    /**Configure Regular Channel 
    */
  sConfig.Channel = ADC_CHANNEL_15;
  sConfig.Rank = 1;
    sConfig.SamplingTime = ADC_SAMPLETIME_2CYCLES_5;
	//sConfig.SamplingTime = ADC_SAMPLETIME_24CYCLES_5;
  sConfig.SingleDiff = ADC_SINGLE_ENDED;
 // sConfig.OffsetNumber = ADC_OFFSET_NONE;
	sConfig.OffsetNumber =ADC_OFFSET_2;
  sConfig.Offset = 50;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    //Error_Handler();
  }

}



//uint16_t hw_get_adc(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
uint16_t hw_get_adc(void)
{
	
       /*##-3- Calibrate ADC then Start the conversion process ####################*/  
  if (HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED) !=  HAL_OK)
  {
    /* ADC Calibration Error */
    //Error_Handler();
  }
  
  if (HAL_ADC_Start(&hadc1) != HAL_OK)
  {
    /* Start Conversation Error */
    //Error_Handler();
  }

  /*##-4- Wait for the end of conversion #####################################*/
  /*  For simplicity reasons, this example is just waiting till the end of the
      conversion, but application may perform other tasks while conversion
      operation is ongoing. */
  if (HAL_ADC_PollForConversion(&hadc1, 10) != HAL_OK)
  {
    /* End Of Conversion flag not set on time */
    //Error_Handler();
  }

  /* Check if the continous conversion of regular channel is finished */
  if ((HAL_ADC_GetState(&hadc1) & HAL_ADC_STATE_REG_EOC) == HAL_ADC_STATE_REG_EOC)
  {
    /*##-5- Get the converted value of regular channel  ########################*/
    uhADCxConvertedValue = HAL_ADC_GetValue(&hadc1);
		
  }
  return  uhADCxConvertedValue;
}




void hw_adc_init(void)
{ 
   MX_ADC1_Init();
	
	//#define BATTERY_ADC_Pin GPIO_PIN_0
  //#define BATTERY_ADC_GPIO_Port GPIOB
	
	//hw_get_adc(BATTERY_ADC_GPIO_Port, BATTERY_ADC_Pin);
}

uint8_t get_battery_level(uint16_t adc)
{
	uint8_t level = 0;
	
	if(adc <= battery_level[0][1])
	{
	  level = 0;
	}else if(adc <= battery_level[1][1])
	{
	  level = (battery_level[1][1] - battery_level[0][1])/(adc- battery_level[0][1]); 
	}else if(adc <= battery_level[2][1])
	{
	  level = (battery_level[2][1] - battery_level[1][1])/(adc- battery_level[1][1]); 
		level = level + 10;
	}else if(adc <= battery_level[3][1])
	{
	  level = (battery_level[3][1] - battery_level[2][1])/(adc- battery_level[2][1]); 
		level = level + 20;
	}else if(adc <= battery_level[4][1])
	{
	  level = (battery_level[4][1] - battery_level[3][1])/(adc- battery_level[3][1]); 
		level = level + 30;
	}else if(adc <= battery_level[5][1])
	{
	  level = (battery_level[5][1] - battery_level[4][1])/(adc- battery_level[4][1]); 
		level = level + 40;
	}else if(adc <= battery_level[6][1])
	{
	  level = (battery_level[6][1] - battery_level[5][1])/(adc- battery_level[5][1]); 
		level = level + 50;
	}else if(adc <= battery_level[7][1])
	{
	  level = (battery_level[7][1] - battery_level[6][1])/(adc- battery_level[6][1]); 
		level = level + 60;
	}else if(adc <= battery_level[8][1])
	{
	  level = (battery_level[8][1] - battery_level[7][1])/(adc- battery_level[7][1]); 
		level = level + 70;
	}else if(adc <= battery_level[9][1])
	{
	  level = (battery_level[9][1] - battery_level[8][1])/(adc- battery_level[8][1]); 
		level = level + 80;
	}else if(adc <= battery_level[10][1])
	{
	  level = (battery_level[10][1] - battery_level[9][1])/(adc- battery_level[9][1]); 
		level = level + 90;
	}else 
	{
	  level = 100; 
	}
	if(level>100)
		level =100;
	return level;
	
	/*
   if(adc <= battery_level[5][1])//低半区  %50以下
	 {
		   if(adc <= battery_level[2][1])//%20
	     {
				 		   if(adc <= battery_level[1][1])//10%
	             {
				           				  if(adc <= battery_level[0][1])//0%
	                          {
				                       
	                         }else{//0%-10%
	                              level = ((battery_level[1][1] - battery_level[0][1])*10)/(adc- battery_level[0][1]); 
	                         }
	             }else{//%10-20%

	                         level = ((battery_level[2][1] - battery_level[1][1])*10)/(adc- battery_level[1][1]); 
	 
	             }
	     }else{//%20-%50
	             if(adc <= battery_level[3][1])//20%-30%
	             {
	                              level = ((battery_level[3][1] - battery_level[2][1])*10)/(adc- battery_level[2][1]); 
	                         
	             }else{//%30-50%
				           				  if(adc <= battery_level[4][1])//30%-40%
	                          {
				                       level = ((battery_level[4][1] - battery_level[3][1])*10)/(adc- battery_level[3][1]); 
	                         }else{//40%-50%
	                              level = ((battery_level[5][1] - battery_level[4][1])*10)/(adc- battery_level[4][1]); 
	                         }
	             }
	     }
	 
	 }else//高半区50%以上
	 {
	   		   if(adc <= battery_level[2][1])//%20
	     {
				 		   if(adc <= battery_level[1][1])//10%
	             {
				           				  if(adc <= battery_level[0][1])//0%
	                          {
				                       level = 0;
	                         }else{//0%-10%
	                              level = ((battery_level[1][1] - battery_level[0][1])*10)/(adc- battery_level[0][1]); 
	                         }
	             }else{//%10-20%

	                         level = ((battery_level[2][1] - battery_level[1][1])*10)/(adc- battery_level[1][1]); 
	 
	             }
	     }else{//%20-%50
	             if(adc <= battery_level[3][1])//20%-30%
	             {
	                              level = ((battery_level[3][1] - battery_level[2][1])*10)/(adc- battery_level[2][1]); 
	                         
	             }else{//%30-50%
				           				  if(adc <= battery_level[4][1])//30%-40%
	                          {
				                       level = ((battery_level[4][1] - battery_level[3][1])*10)/(adc- battery_level[3][1]); 
	                         }else{//40%-50%
	                              level = ((battery_level[5][1] - battery_level[4][1])*10)/(adc- battery_level[4][1]); 
	                         }
	             }
	 
	 }
	 */

}

uint8_t hw_battery_get(void)
{
  uint16_t adc = 0;
	uint16_t adc_level = 0;
	uint8_t i = 0;
	uint32_t tmp = 0;
	uint16_t tmp2[10]={0x00};
	
  adc = hw_get_adc();
	adc = adc *2;
	
		hw_charge_detect();
	/*
	{
	   // uint8_t huanhang[2]= "\r\n";
		uint8_t adc_print[6]= {0x00};
	  
		sprintf((char*)adc_print,"%d",adc);
		
		//hw_lora_send(adc_print,6);
		hw_gps_send(adc_print,6);
	
	}
	*/
	
	//第一次开机，读10次
	if(first_flag2 == 0)
	{
		//first_flag = 1;
	  for(i=0;i<10;i++)
		{
		  adc_value[i] = 2 * hw_get_adc();
					if(charge_state == 1)//充电
{
		adc_value[i] = adc_value[i]-30;				
}
			delay_ms(5);
		}
		tmp = 0;
		for(i=0;i<10;i++)
		{
		  tmp = tmp+adc_value[i];
		}
		adc = tmp/10;

//		adc_vlue_last = adc;
	 //first_flag2 = 1;
	}else{
	 //读取一次
		
	  for(i=0;i<10;i++)
		{
		  tmp2[i] = 2 * hw_get_adc();
			delay_ms(5);
		}
		tmp = 0;
		for(i=0;i<10;i++)
		{
		  tmp = tmp+tmp2[i];
		}
		adc = tmp/10;
		if(charge_state == 1)//充电
{
		adc = adc-30;				
}
		
		//memcpy(adc_value,adc_value+1,9);
		//memcpy(tmp2,adc_value+1,9);
		//memcpy(adc_value,tmp2,9);
				for(i=0;i<9;i++)
		{
		  adc_value[i] = adc_value[i+1];
		}
		adc_value[9] = adc;
	//	adc_value;
		
		tmp = 0;
		for(i=0;i<10;i++)
		{
		  tmp = tmp+adc_value[i];
		}
		adc = tmp/10;
	
	}
	/*
		{
	   // uint8_t huanhang[2]= "\r\n";
		uint8_t adc_print[6]= {0x00};
	  
		sprintf((char*)adc_print,"%d",adc);
		
		//hw_lora_send(adc_print,6);
	//	hw_gps_send("  ",2);
		hw_gps_send(adc_print,6);
	
	}
	*/


	
adc_level = get_battery_level(adc);
	
	
	
	if(first_flag2 == 0)
	{
	  adc_level_last = adc_level;
	}else{
					if(charge_state == 0)//未充电
		     	{
           	if(adc_level_last < adc_level)//只降不升
						{
						    adc_level = adc_level_last;
						}
							
					}else//充电中
					{
					  if(adc_level_last > adc_level)//只升
						{
						    adc_level = adc_level_last;
						}
					}
	 adc_level_last = adc_level;
	}
	
	//低电量闪灯
	//未充电
		if(HAL_GPIO_ReadPin(PRECHG_GPIO_Port, PRECHG_Pin) == GPIO_PIN_SET)
		{
		//if(HAL_GPIO_ReadPin(CHARGE_GPIO_Port, CHARGE_Pin) == GPIO_PIN_SET)//充满
		////{
			//HAL_GPIO_WritePin(LED_CHG_RED_GPIO_Port, LED_CHG_RED_Pin, state);	
			//hw_led_key_chg_red_set(GPIO_PIN_RESET);
			//hw_led_key_chg_green_set(GPIO_PIN_SET);
		//}
			if(charge_state == 0)
			{
				if(work_mode != low_level)
				{
	      if(adc_level<=15)
		     {
		         hw_led_key_chg_red_set(GPIO_PIN_SET);
			       //osDelay(5);
					   //HAL_Delay(1000);
					 delay_ms(40);
			       hw_led_key_chg_red_set(GPIO_PIN_RESET);
			       //osDelay(5);
			 
		     }
			 }
	    }
	 }else{
	 //充电中  检测充满
	 if(adc_level >=100)
	   {
	      //亮绿灯
			 hw_led_key_chg_green_set(GPIO_PIN_SET);
			 hw_led_key_chg_red_set(GPIO_PIN_RESET);
	   }
	 }
	send_battery_level = adc_level;
	 if(first_flag2 == 1)
	 {
  app_lcd_mutex_enter();
	sprintf((char*)battery_level_show,"%d",adc_level);
  app_lcd_mutex_exit();
	 }else{
	 sprintf((char*)battery_level_show,"%d",adc_level);
	 }
/*	 
	 	{
	    uint8_t huanhang[2]= "\r\n";
		uint8_t adc_print[6]= {0x00};
	  
		sprintf((char*)adc_print,"%d",adc_level);
		
		hw_gps_send(adc_print,6);
		hw_gps_send(huanhang,2);
	
	}
*/ 
	first_flag2 = 1;
	 
	 if(adc_level == 0)
	 {
	    adc_level_0_count++;
		 if(adc_level_0_count >= 5)
		 {
		    //关机
			    app_power_close();
		 }
	 }else
	 {
	   adc_level_0_count = 0;
	 }
	return adc_level;
	
}


