#include <stdint.h>
#include <stdio.h>
#include "cmsis_os.h"
#include "task_refresh.h"
#include "gui_message.h"
#include "app_meerwespe.h"
#include "hw_adc.h"
#include "hw_rtc.h"
#include "app_lcd.h"
#include "hw_lcd.h"
#include "delay.h"
#include "task_timer.h"
#include "app_gps.h"
#include "app_power.h"


extern uint32_t low_level_timeout;

extern osMessageQId WorkQueueHandle;
extern osMutexId lcdMutexHandle;

uint8_t success_error_page_flag = 0;

uint8_t wakeup_init_flag = 1;

//static uint8_t lcd_flag = 0;

//static uint8_t lcd_in =0;

extern GPS_MODE gps_state;
extern LORA_MODE lora_state;


static uint8_t sn_first = 0;

extern uint8_t lcd_power_on_flag;



osStatus task_refresh_message_put(uint32_t info)
{
  	osStatus  status;
    status = osMessagePut(WorkQueueHandle, info, osWaitForever);
	  return status;
}

osEvent task_refresh_message_get(void)
{
  return osMessageGet(WorkQueueHandle, osWaitForever);
}



/* RefreshTask function */
void RefreshTask(void const * argument)
{
  /* USER CODE BEGIN RefreshTask */
  /* Infinite loop */
//	uint8_t aShowDate[10]={0x00};
	
	osEvent event;
	
  for(;;)
  {
	
	//获取队列
 event = task_refresh_message_get();
	//osStatus osMessagePut (osMessageQId queue_id, uint32_t info, uint32_t millisec);
if(event.status == osEventMessage)
{
 //
	switch(event.value.v)
	{
		case main_page://首页
			//lcd_flag = 1;
			app_gps_clean_data();
		  if(lcd_power_on_flag == 0)//已经下电
			{
			 hw_lcd_init();
				lcd_power_on_flag = 1;
			}
		
		  success_error_page_flag = 0;
		 	if((gps_state != gps_close)||(lora_state != lora_close))
			{
         hw_lcd_displayon2();
		  }

		  app_lcd_main_page();
			if(sn_first == 0)
			{
			   app_lcd_get_sn();
				sn_first = 1;
			}

			break;
		case sos_page://sos
					  if(lcd_power_on_flag == 0)//已经下电
			{
			 hw_lcd_init();
				lcd_power_on_flag = 1;
			}
		   if((gps_state != gps_close)||(lora_state != lora_close))
			{
         hw_lcd_displayon2();
		  }

      app_lcd_sos_page();

		  //osMessagePut(WorkSendQueueHandle, gps_send, osWaitForever);
			break;
		case call_page://报警
      if(lcd_power_on_flag == 0)//已经下电
			{
			 hw_lcd_init();
				lcd_power_on_flag = 1;
			}
					
		   if((gps_state != gps_close)||(lora_state != lora_close))
			{
         hw_lcd_displayon2();
		  }
      app_lcd_call_page();
			
			//osMessagePut(WorkSendQueueHandle, gps_send, osWaitForever);
			break;
		case sn_page://sn

      app_lcd_sn_page();
			
			break;
	//	case low_level://灭屏低功耗		
      //灭屏			
	//		break;
		
		case success://成功

      app_lcd_success();
			success_error_page_flag = success;

			break;
				
		case error://失败

      app_lcd_error();
			success_error_page_flag = error;

		
			break;
		
		case lcd_off://灭		
		 
		if(work_mode != lcd_off)
		{
      hw_lcd_displayoff();
			work_mode = lcd_off;
		}
      low_level_timeout=0;
		 
		  //task_timer_en(0);
		 	//
   		if((gps_state == gps_close)&&(lora_state == lora_close))
			{
				
		      work_mode = low_level;
//				wakeup_init_flag = 0;
	        app_power_stop2_enter();
				
				//taskENTER_CRITICAL();
		   //   app_power_stop2_exit();
				
//				   work_mode = main_page;
//	      	hw_lcd_displayon2();
//				  app_lcd_main_page();
				//  taskEXIT_CRITICAL();
				//	wakeup_init_flag = 1;
				
		 }
		
		 // HAL_PWREx_EnterSTOP2Mode(PWR_STOPENTRY_WFI);
		
			break;
		
		case 0x30://灭		
			//lcd_flag = 0;
      hw_lcd_displayoff();
      low_level_timeout=0;
		  work_mode = low_level;
		  //task_timer_en(0);
		 	//	if((gps_state == gps_close)&&(lora_state == lora_close))
					 {
	    app_power_stop2_enter();
		  app_power_stop2_exit();
	  	hw_lcd_displayon2();
		  //lcd_flag = 1;
					 }
		
		 // HAL_PWREx_EnterSTOP2Mode(PWR_STOPENTRY_WFI);
		
			break;
				
		case 0x31://亮		
			//lcd_flag = 1;
		  delay_ms(800);
      hw_lcd_displayon2();
		  low_level_timeout=0;
		  //work_mode = main_page;
      //task_timer_en(1);		
			break;
	
		default:break;
	}

}
/*	
	if((work_mode == main_page)||(work_mode == sos_page)||(work_mode==call_page))
	{
	  sprintf((char*)aShowDate,"%d",hw_battery_get());
		gui_battery(aShowDate);

		//刷运动状态
		//sprintf((char*)aShowDate,"%d",hw_battery_get());
	  gui_gsensor(running_falg);
	}
	
	if(work_mode == main_page)
	{
	  	  hw_rtc_time_show();
	}
		
		gui_refresh();
		
		osDelay(900);
	 */	
     osDelay(10);
   }
	
  /* USER CODE END RefreshTask */
}





