#include "myconfig.h"
#include "gui.h"
#include "app_key.h"
#include "app_meerwespe.h"
#include "gui_message.h"
#include "hw_lcd.h"

#include "hw_rtc.h"
#include "hw_adc.h"
#include "hw_heartrate.h"
#include "app_lcd.h"
#include "cmsis_os.h"
#include "hw_gps.h"
#include "hw_lora.h"
#include "task_timer.h"
#include "hw_motor.h"
#include "app_power.h"
#include "app_gps.h"
#include "stm32l4xx_hal.h"
#include "my_common.h"
#include "hw_lora.h"

#include "task_refresh.h"
#include "task_gps.h"
#include "task_lora.h"
#include "delay.h"


extern GPS_MODE gps_state;
extern LORA_MODE lora_state;

extern uint32_t low_level_timeout;

extern uint8_t running_falg ;
extern uint8_t running_count;
extern uint8_t first_flag;
extern WORK_SEND_MODE work_send_mode;


uint8_t shutdown_flag = 0;

extern uint8_t success_error_page_flag ;









void app_key_power(uint8_t state)
{
  		low_level_timeout =0;
		
		if(work_mode == main_page)
		{
			//??
			//work_mode = low_level;
			
			//osMutexWait(lcdMutexHandle,osWaitForever);
			//	hw_lcd_displayoff();
			//osMutexRelease(lcdMutexHandle);
			//osMessagePut(WorkQueueHandle, 0x30, osWaitForever);
		  //task_timer_en(0);
		 // hw_gps_close();
     //hw_lora_close();
		//low_level_timeout = 0;
			task_refresh_message_put(lcd_off);
			
		}else if(work_mode == low_level)
		{
			
			app_power_stop2_exit();
		 //
				work_mode = main_page;
	    //hw_lcd_displayon();
			//osMutexWait(lcdMutexHandle,osWaitForever);
			//hw_lcd_displayon2();
			//osMutexRelease(lcdMutexHandle);
			//low_level_timeout = 0;
 			//osMessagePut(WorkQueueHandle, 0x31, osWaitForever);
			 //task_timer_en(1);
			task_refresh_message_put(work_mode);
			
		}
			
		else if(work_mode == lcd_off)
		{
		 //
				work_mode = main_page;
	    //hw_lcd_displayon();
			//osMutexWait(lcdMutexHandle,osWaitForever);
			//hw_lcd_displayon2();
			//osMutexRelease(lcdMutexHandle);
			//low_level_timeout = 0;
 			//osMessagePut(WorkQueueHandle, 0x31, osWaitForever);
			 //task_timer_en(1);
			task_refresh_message_put(work_mode);
			
		}
		 else if((work_mode == sos_page)||(work_mode == call_page))
		{
		 //?lora ?gps
	   // hw_lcd_displayon();
     //hw_gps_close();
//	  hw_lora_close();
			success_error_page_flag = 0;
		//	work_send_mode = no_send;
			
			//发送模式
		
			work_send_mode = no_send;
			work_mode = main_page;
		 
      //app_lcd_main_page();
     // osMessagePut(WorkQueueHandle, work_mode, osWaitForever);
		task_refresh_message_put(work_mode);

	}
		 
		
		
		
	 

}


void app_key_sos(uint8_t state)
{
		hw_motor_set(1);
		low_level_timeout =0;
		
		if(shutdown_flag >=6)//??
		{
		    app_power_close();
		}
		
		
		if(work_mode != sos_page)
		{
			//如果是待机，则先唤醒初始化
			if(work_mode == low_level)
		{
			app_power_stop2_exit();
			delay_ms(50);
		}

			 work_mode = sos_page;
			 task_refresh_message_put(work_mode);
			
		delay_ms(500);

			//work_send_mode = sos_send;
      task_lora_message_put(sos_send);
			
	 }
 

  /*
				if(work_mode == low_level)
				{
					  
					work_mode = lcd_off;
					app_power_stop2_exit();
				}

     //  app_power_stop2_exit();
	    task_lora_message_put(huxi_send);
*/
}


void app_key_call(uint8_t state)
{
		hw_motor_set(1);
		low_level_timeout =0;
	
	
		if(work_mode != call_page)
		{
			
		if(work_mode == low_level)
		{
			app_power_stop2_exit();
			delay_ms(50);
		}

	  work_mode = call_page;
		task_refresh_message_put(work_mode);
		delay_ms(500);

		  //???lora
			//work_send_mode = call_send;
		  //osMessagePut(WorkSendQueueHandle, lora_send, osWaitForever);
      task_lora_message_put(call_send);


  	}
		

  /*
				if(work_mode == low_level)
				{
					  
					work_mode = lcd_off;
					app_power_stop2_exit();
				}

     //  app_power_stop2_exit();
	    task_lora_message_put(huxi_send);
*/
}

void app_key_next(uint8_t state)
{
	//HAL_GPIO_TogglePin(LED_KEY_R1_GPIO_Port, LED_KEY_R1_Pin);
	//HAL_GPIO_WritePin(LED_KEY_R1_GPIO_Port,LED_KEY_R1_Pin,GPIO_PIN_SET);
		
		low_level_timeout =0;
	if(work_mode == main_page)
	{
	 work_mode = sn_page;

   //app_lcd_sn_page();
		//osMessagePut(WorkQueueHandle, work_mode, osWaitForever);
		task_refresh_message_put(work_mode);
	}else if(work_mode == sn_page)
	{
		shutdown_flag = 1;
	  work_mode = main_page;
    //app_lcd_main_page();
	//	osMessagePut(WorkQueueHandle, work_mode, osWaitForever);
    task_refresh_message_put(work_mode);
	}

}
