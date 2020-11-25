#include "hw_rtc.h"
#include "hw_config.h"
#include "stm32l4xx_hal.h"
#include "gui_message.h"
#include "app_power.h"
#include "stdint.h"
#include "app_lcd.h"
#include "task_lora.h"
#include "app_meerwespe.h"
#include "hw_led.h"
#include "delay.h"


/* Private variables ---------------------------------------------------------*/
RTC_HandleTypeDef hrtc;

#define RtcHandle  hrtc

uint8_t time_show[9]; 
uint8_t date_show[11];


#define RTC_ASYNCH_PREDIV  0x7F
#define RTC_SYNCH_PREDIV   0x00FF
/* RTC init function */
static void MX_RTC_Init(void)
{

  RTC_TimeTypeDef sTime;
  RTC_DateTypeDef sDate;
//  RTC_AlarmTypeDef sAlarm;

    /**Initialize RTC Only 
    */
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 127;
  hrtc.Init.SynchPrediv = 255;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutRemap = RTC_OUTPUT_REMAP_NONE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    //Error_Handler();
  }

    /**Initialize RTC and set the Time and Date 
    */
  if(HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR0) != 0x32F2){
  sTime.Hours = 0x0;
  sTime.Minutes = 0x0;
  sTime.Seconds = 0x0;
  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sTime.StoreOperation = RTC_STOREOPERATION_RESET;
  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK)
  {
    //Error_Handler();
  }

  sDate.WeekDay = RTC_WEEKDAY_MONDAY;
  sDate.Month = RTC_MONTH_JANUARY;
  sDate.Date = 0x1;
  sDate.Year = 0x17;

  if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD) != HAL_OK)
  {
   // Error_Handler();
  }

    HAL_RTCEx_BKUPWrite(&hrtc,RTC_BKP_DR0,0x32F2);
  }
    /**Enable the Alarm A 
    */
	/*
  sAlarm.AlarmTime.Hours = 0x0;
  sAlarm.AlarmTime.Minutes = 0x0;
  sAlarm.AlarmTime.Seconds = 0x0;
  sAlarm.AlarmTime.SubSeconds = 0x0;
  sAlarm.AlarmTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sAlarm.AlarmTime.StoreOperation = RTC_STOREOPERATION_RESET;
  sAlarm.AlarmMask = RTC_ALARMMASK_NONE;
  sAlarm.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_ALL;
  sAlarm.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE;
  sAlarm.AlarmDateWeekDay = 0x1;
  sAlarm.Alarm = RTC_ALARM_A;
  if (HAL_RTC_SetAlarm_IT(&hrtc, &sAlarm, RTC_FORMAT_BCD) != HAL_OK)
  {
   // Error_Handler();
  }
*/
    /**Enable the WakeUp 
    */
  if (HAL_RTCEx_SetWakeUpTimer_IT(&hrtc, 600, RTC_WAKEUPCLOCK_CK_SPRE_16BITS) != HAL_OK)
  {
   // Error_Handler();
  }






/*
  RTC_DateTypeDef sdatestructure;
  RTC_TimeTypeDef stimestructure;
	
	
	
	 //##-1- Configure the RTC peripheral #######################################
  // Configure RTC prescaler and RTC data registers 
  // RTC configured as follow:
  //    - Hour Format    = Format 12
  //    - Asynch Prediv  = Value according to source clock
  //    - Synch Prediv   = Value according to source clock
  //    - OutPut         = Output Disable
   //   - OutPutPolarity = High Polarity
  //    - OutPutType     = Open Drain 
  __HAL_RTC_RESET_HANDLE_STATE(&RtcHandle);
  RtcHandle.Instance = RTC;
  RtcHandle.Init.HourFormat     = RTC_HOURFORMAT_12;
  RtcHandle.Init.AsynchPrediv   = RTC_ASYNCH_PREDIV;
  RtcHandle.Init.SynchPrediv    = RTC_SYNCH_PREDIV;
  RtcHandle.Init.OutPut         = RTC_OUTPUT_DISABLE;
  RtcHandle.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  RtcHandle.Init.OutPutType     = RTC_OUTPUT_TYPE_OPENDRAIN;

  if(HAL_RTC_Init(&RtcHandle) != HAL_OK)
  {
    //Initialization Error //
    Error_Handler(); 
  }

  //##-1- Configure the Time Stamp peripheral ################################/
  //  RTC TimeStamp generation: TimeStamp Rising Edge on PC.13 Pin /
  //HAL_RTCEx_SetTimeStamp_IT(&RtcHandle, RTC_TIMESTAMPEDGE_RISING, RTC_TIMESTAMPPIN_DEFAULT);

  //##-2- Configure the Date #################################################/
  // Set Date: Monday April 14th 2014 /
  sdatestructure.Year    = 0x14;
  sdatestructure.Month   = RTC_MONTH_APRIL;
  sdatestructure.Date    = 0x14;
  sdatestructure.WeekDay = RTC_WEEKDAY_MONDAY;
  
  if(HAL_RTC_SetDate(&RtcHandle,&sdatestructure,RTC_FORMAT_BCD) != HAL_OK)
  {
    // Initialization Error /
    Error_Handler(); 
  } 
  
  //##-3- Configure the Time #################################################/
  // Set Time: 08:10:00 /
  stimestructure.Hours          = 0x08;
  stimestructure.Minutes        = 0x10;
  stimestructure.Seconds        = 0x00;
  stimestructure.SubSeconds     = 0x00;
  stimestructure.TimeFormat     = RTC_HOURFORMAT12_AM;
  stimestructure.DayLightSaving = RTC_DAYLIGHTSAVING_NONE ;
  stimestructure.StoreOperation = RTC_STOREOPERATION_RESET;
  
  if(HAL_RTC_SetTime(&RtcHandle,&stimestructure,RTC_FORMAT_BCD) != HAL_OK)
  {
    // Initialization Error/
    Error_Handler(); 
  }
	
	*/

}


void HAL_RTCEx_WakeUpTimerEventCallback(RTC_HandleTypeDef *hrtc)
{
	
	hw_led_key_chg_red_set(GPIO_PIN_SET);
	delay_ms(10);
	hw_led_key_chg_red_set(GPIO_PIN_RESET);
  
				if(work_mode == low_level)
				{
					  
					work_mode = lcd_off;
					app_power_stop2_exit();
				}

     //  app_power_stop2_exit();
	    task_lora_message_put(huxi_send);
	
	    
	   
	

}



void hw_rtc_init(void)
{ 
   MX_RTC_Init();
	
// HAL_RTC_SetAlarm(hrtc, RTC_AlarmTypeDef *sAlarm, uint32_t Format)
	
	//#define BATTERY_ADC_Pin GPIO_PIN_0
  //#define BATTERY_ADC_GPIO_Port GPIOB
	
	//hw_get_adc(BATTERY_ADC_GPIO_Port, BATTERY_ADC_Pin);
}

/*
void HAL_RTCEx_TimeStampEventCallback(RTC_HandleTypeDef *hrtc)
{
  RTC_DateTypeDef sTimeStampDateget;
  RTC_TimeTypeDef sTimeStampget;
  uint8_t aShowTimeStamp[50] = {0};
  uint8_t aShowDateStamp[50] = {0};

  HAL_RTCEx_GetTimeStamp(hrtc, &sTimeStampget, &sTimeStampDateget, RTC_FORMAT_BIN);

  // Display time Format : hh:mm:ss 
  sprintf((char*)aShowTimeStamp,"%.2d:%.2d:%.2d ", sTimeStampget.Hours, sTimeStampget.Minutes, sTimeStampget.Seconds);
  // Display date Format : mm-dd 
  sprintf((char*)aShowDateStamp,"%.2d-%.2d-%.2d ", sTimeStampDateget.Month, sTimeStampDateget.Date, sTimeStampDateget.Year);

 //刷新
	
 //日期
 gui_date(aShowDateStamp);

//时间
 gui_time(aShowTimeStamp);


	
	
}
*/

/**
  * @brief  Display the current time and date.
  * @param  showtime : pointer to buffer
  * @param  showdate : pointer to buffer
  * @retval None
  */
void hw_rtc_get_date_time(uint8_t *time,uint8_t *date)
{
  RTC_DateTypeDef sdatestructureget;
  RTC_TimeTypeDef stimestructureget;
	
	/* Buffers used for displaying Time and Date */
  //uint8_t aShowTime[50] = {0};
  //uint8_t aShowDate[50] = {0};
  
  /* Get the RTC current Time */
  HAL_RTC_GetTime(&hrtc, &stimestructureget, RTC_FORMAT_BIN);
  /* Get the RTC current Date */
  HAL_RTC_GetDate(&hrtc, &sdatestructureget, RTC_FORMAT_BIN);
  
  /* Display time Format : hh:mm:ss */
  //sprintf((char*)time,"%.2d:%.2d:%.2d", stimestructureget.Hours, stimestructureget.Minutes, stimestructureget.Seconds);
  /* Display date Format : mm-dd-yy */
  //sprintf((char*)date,"%.2d/%.2d/%.2d", 2000 + sdatestructureget.Year, sdatestructureget.Month, sdatestructureget.Date);
	
//用于发送
	 app_lcd_mutex_enter();
	
	*time = stimestructureget.Hours;
	*(time+1) = stimestructureget.Minutes;
	*(time+2) = stimestructureget.Seconds;
	
	*date = sdatestructureget.Year;
	*(date+1) = sdatestructureget.Month;
	*(date+2) = sdatestructureget.Date;
	
   app_lcd_mutex_exit();
	
} 


void hw_rtc_date_time_detect(void)
{
  RTC_DateTypeDef sdatestructureget;
  RTC_TimeTypeDef stimestructureget;
	
	/* Buffers used for displaying Time and Date */
  //uint8_t aShowTime[50] = {0};
  //uint8_t aShowDate[50] = {0};
  
  /* Get the RTC current Time */
  HAL_RTC_GetTime(&hrtc, &stimestructureget, RTC_FORMAT_BIN);
  /* Get the RTC current Date */
  HAL_RTC_GetDate(&hrtc, &sdatestructureget, RTC_FORMAT_BIN);
  
	 app_lcd_mutex_enter();
  /* Display time Format : hh:mm:ss */
  sprintf((char*)time_show,"%.2d:%.2d:%.2d", stimestructureget.Hours, stimestructureget.Minutes, stimestructureget.Seconds);
  /* Display date Format : mm-dd-yy */
  sprintf((char*)date_show,"%.2d/%.2d/%.2d", 2000 + sdatestructureget.Year, sdatestructureget.Month, sdatestructureget.Date);
	app_lcd_mutex_exit();
	 //刷新
	
     //日期
     //gui_date(aShowDate);

    //时间
    //gui_time(aShowTime);
} 

//从gps获取的时间，直接设置
uint8_t hw_rtc_set_date_time(uint8_t *time,uint8_t *date)
{
	RTC_TimeTypeDef sTime;
  RTC_DateTypeDef sDate;
	
	uint8_t beijing_hh = 0,beijing_mm =0,beijing_ss =0;
	uint8_t beijing_y = 0,beijing_m =0,beijing_d =0;
	
	//utc转北京
	beijing_hh = (time[0]-0x30)*16+(time[1]-0x30);
	beijing_mm = (time[2]-0x30)*16+(time[3]-0x30);
	beijing_ss = (time[4]-0x30)*16+(time[5]-0x30);
	
	beijing_d = (date[0]-0x30)*16+(date[1]-0x30);
	beijing_m = (date[2]-0x30)*16+(date[3]-0x30);
	beijing_y = (date[4]-0x30)*16+(date[5]-0x30);
	
	
//	sprintf((char *)&beijing_hh,"%x",beijing_hh);
//	sprintf((char *)&beijing_mm,"%x",beijing_mm);
//	sprintf((char *)&beijing_ss,"%x",beijing_ss);
//	sprintf((char *)&beijing_d,"%x",beijing_d);
//	sprintf((char *)&beijing_m,"%x",beijing_m);
//	sprintf((char *)&beijing_y,"%x",beijing_y);

	
	
//时区转换	
if((beijing_hh >= 0x16)&&(beijing_hh<=0x19))
{
  beijing_hh = beijing_hh+0x0e-0x24;
	beijing_d = beijing_d +1;
}
else if((beijing_hh>=0x20)&&(beijing_hh <= 0x23))
{
  beijing_hh = beijing_hh +0x08 -0x24;
	beijing_d = beijing_d +1;
}
else if((beijing_hh >= 0x00)&&(beijing_hh <=0x01))
{
  beijing_hh = beijing_hh + 0x08;
}	
else if((beijing_hh >= 0x02)&&(beijing_hh <=0x09))
{
  beijing_hh = beijing_hh +0x0e;
}
else if((beijing_hh >= 0x10)&&(beijing_hh <= 0x15))
{
beijing_hh = beijing_hh +0x08;
}
	

    /**Initialize RTC and set the Time and Date 
    */
 // if(HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR0) != 0x32F2){
  sTime.Hours = beijing_hh;//0x0;
  sTime.Minutes = beijing_mm;//0x0;
  sTime.Seconds = beijing_ss;//0x0;
  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sTime.StoreOperation = RTC_STOREOPERATION_RESET;
  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK)
  {
    //Error_Handler();
  }

  sDate.WeekDay = RTC_WEEKDAY_MONDAY;
  sDate.Month = beijing_m;//RTC_MONTH_JANUARY;
  sDate.Date = beijing_d;//0x1;
  sDate.Year = beijing_y;//0x17;

  if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD) != HAL_OK)
  {
    //Error_Handler();
  }

 //   HAL_RTCEx_BKUPWrite(&hrtc,RTC_BKP_DR0,0x32F2);
	return 0;
 // }else{
//	return 1;
//	}
}


void hw_rtc_set_wakeup(uint16_t time)
{
//	HAL_RTCEx_SetWakeUpTimer_IT(&hrtc, time, RTC_WAKEUPCLOCK_CK_SPRE_16BITS);
}