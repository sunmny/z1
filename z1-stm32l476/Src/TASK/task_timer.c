#include <stdint.h>
#include <stdio.h>
#include "cmsis_os.h"
#include "task_timer.h"
#include "hw_gsensor.h"
#include "app_meerwespe.h"
#include "hw_adc.h"
#include "hw_rtc.h"
#include "hw_lcd.h"
#include "hw_gps.h"
#include "hw_lora.h"
#include "hw_config.h"
#include "hw_heartrate.h"
#include "task_refresh.h"

//static uint32_t send_10_timeout = 0;

extern osMessageQId WorkQueueHandle;
extern osTimerId RefreshTimerHandle;

extern uint8_t success_error_page_flag ;

static uint8_t delete_adc = 0;

uint8_t first_flag =0;
extern uint8_t shutdown_flag ;

static uint8_t heartrate_count = 0;

//static  uint8_t kk = 0;
//static uint8_t  aa = 0;
/*
osStatus osTimerStart (osTimerId timer_id, uint32_t millisec);//开始定时
osStatus osTimerStop (osTimerId timer_id);//停止定时
osStatus osTimerDelete (osTimerId timer_id);//删除定时器
*/

extern uint8_t running_falg;
extern uint8_t running_count ;


uint32_t low_level_timeout = 0;
//static uint8_t state = 0;

extern uint8_t lcd_in;

static uint8_t success_error_page_count = 0;


void task_timer_en(uint8_t en)
{
  if(en == 1)
	{
	osTimerStart (RefreshTimerHandle, 1000);//开始定时
	}else{
	osTimerStop (RefreshTimerHandle);//停止定时	
	}

}
void RefreshTimerCallback2(void const * argument)
{
	
}


/* RefreshTimerCallback function */
void RefreshTimerCallback(void const * argument)
{
  /* USER CODE BEGIN RefreshTimerCallback */
  
  /* USER CODE END RefreshTimerCallback */
	
	//发送刷屏事件
//	if(state == 1)
	//if(lcd_in ==0)
	if(success_error_page_flag == 0)
	{
		success_error_page_count = 0;
	if((work_mode == main_page)||(work_mode == sos_page)||(work_mode == call_page))
	{
	  //osMessagePut (WorkQueueHandle, work_mode, osWaitForever);
		task_refresh_message_put(work_mode);
	}
}
	else{
	if((work_mode == call_page)||(work_mode == sos_page)){
	     low_level_timeout = 0;
		success_error_page_count ++;
		//osMessagePut (WorkQueueHandle, success_error_page_flag, osWaitForever);
	  task_refresh_message_put(success_error_page_flag);
		
		if(success_error_page_count>=5)
		{
		   success_error_page_count = 0;
			success_error_page_flag = 0;
			work_mode = main_page;
			//osMessagePut (WorkQueueHandle, work_mode, osWaitForever);
			task_refresh_message_put(work_mode);
		}
	
	}
}
	
	//gsensor
   hw_gsensor_running_detect();

 	hw_battery_get();
/*
kk++;

if(kk>=5)
{
	if(aa == 0)
	{
		aa = 1;
   hw_gps_open();
	}else
	{
   aa = 0;
   hw_gps_close();
	}
   kk = 0;
}
*/

//delete_adc++;
	//时间日期
	hw_rtc_date_time_detect();

	//hw_battery_get();


if(delete_adc>=2)
{
	
	//探测电量
	if(work_mode != low_level)
	{
	hw_battery_get();
	}
	delete_adc = 0;
}
	

	
	/*
  send_10_timeout++;
	if(send_10_timeout>=600)
	{
    hw_gps_open();
		if(send_10_timeout>=602)
	  {
		send_10_timeout = 0;
		osMessagePut (WorkQueueHandle, gps_send, osWaitForever);
	  }
	}
		*/
	
	
//	if(low_level_timeout == 0)
//	state = 1;
//	if(work_mode == )
//	low_level_timeout++;
//if(state == 1)
//{


if(first_flag !=0)
{
	if((work_mode == main_page)||(work_mode == sn_page))
	{
		low_level_timeout++;
	  if(low_level_timeout>=11)
	  {
			
//			state = 0;
			
	    
			//hw_lcd_displayoff();
			
			low_level_timeout=0;
			if((work_mode == main_page)||(work_mode == sn_page))
	{
			//osMessagePut (WorkQueueHandle, 0x30, osWaitForever);
			task_refresh_message_put(lcd_off);
	}
    //	work_mode = low_level;
		//  hw_gps_close();
		  //hw_lora_close();
      //task_timer_en(0);
			
	  }
  }
}else{

	if((work_mode == main_page)||(work_mode == sn_page))
	{
  low_level_timeout++;
		  if(low_level_timeout>=60)
	  {
			
//			state = 0;
			
	   
			//hw_lcd_displayoff();
			
			low_level_timeout=0;
			
	if((work_mode == main_page)||(work_mode == sn_page))
	{
			//osMessagePut (WorkQueueHandle, 0x30, osWaitForever);
			task_refresh_message_put(lcd_off);
	}
	     //work_mode = low_level;
		  //hw_gps_close();
		  //hw_lora_close();
      //task_timer_en(0);
			
	  }
  }
}


if(shutdown_flag >=1)
{
  //检测next pin状态
//	#define KEY_NEXT_Pin GPIO_PIN_14
//#define KEY_NEXT_GPIO_Port GPIOG
	if(HAL_GPIO_ReadPin(KEY_NEXT_GPIO_Port, KEY_NEXT_Pin)==0)//长按
	{
	  shutdown_flag++;
	
	}else
	{
	  shutdown_flag = 0;
	
	}

}
	
//}
heartrate_count++;
if(heartrate_count>=1)
{
	//hw_heartrate_test();
	heartrate_count = 0;
}
}
