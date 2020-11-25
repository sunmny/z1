#include "app_lcd.h"
#include <stdint.h>
#include <stdio.h>
#include "string.h"
#include "cmsis_os.h"
#include "task_refresh.h"
#include "gui_message.h"
#include "app_meerwespe.h"
#include "hw_adc.h"
#include "hw_rtc.h"
#include "hw_lcd.h"
#include "hw_led.h"
#include "app_lora.h"


static uint8_t sn_raw[20] = {0x00};
static uint8_t sn_len = 0;


extern uint8_t running_falg;
extern osMessageQId WorkQueueHandle;
extern osMutexId lcdMutexHandle;


extern osMutexId lcdMutexHandle;

//    osMutexWait( lcdMutexHandle, osWaitForever );
//    osDelay( 10 );
//    osMutexRelease( lcdMutexHandle );
		
//typedef struct
//{ 
extern uint8_t time_show[9]; 
extern uint8_t date_show[11]; 
extern uint8_t battery_level_show[4];
extern uint8_t gps_nub[4];
extern uint8_t gps_dingwei_state; 
extern uint8_t lora_state; 
extern uint8_t running_falg; 
extern uint8_t heartrate_state; 
extern uint8_t charge_state;


static uint8_t        show_time[9]; 
static uint8_t        show_date[11]; 
static uint8_t        show_level_battery[4]; 
static uint8_t        show_charge;
static uint8_t        show_level_gps_nub[4];
static uint8_t        show_level_gps; 
static uint8_t        show_level_433; 
static uint8_t        show_level_gsensor; 
static uint8_t        show_level_hearteate; 


//extern GPS_MODE gps_state;
//extern LORA_MODE lora_state;


void app_lcd_get_sn(void)
{
  uint8_t ret = 0;
	uint8_t sn_raw2[20] = {0x00};
	uint8_t sn_len2 = 0;
 ret =  app_lora_get_sn(sn_raw2,&sn_len2);
	if(ret != 0)
	{
	  memcpy(sn_raw,"123456",6);
		sn_len = 6;
	}else
	{
		sn_len = sn_len2;
	  memcpy(sn_raw,sn_raw2,sn_len);
	}

}




void app_lcd_mutex_enter()
{
   osMutexWait( lcdMutexHandle, osWaitForever );
}
	
void app_lcd_mutex_exit()
{
   osMutexRelease( lcdMutexHandle );
}

	
//}GuiMessage;	
//显示内容
void app_lcd_copy_message(void)
{
	//获取信号量
	
	 app_lcd_mutex_enter();
	 taskENTER_CRITICAL();
   memcpy(show_time,time_show,9);
	 memcpy(show_date,date_show,11);
	 memcpy(show_level_battery,battery_level_show,4); 
	 memcpy(show_level_gps_nub,gps_nub,4);
	 show_charge = charge_state;
   show_level_gps = gps_dingwei_state; 
   show_level_433 = lora_state; 
   show_level_gsensor = running_falg; 
   show_level_hearteate = heartrate_state; 
	 taskEXIT_CRITICAL();
	 app_lcd_mutex_exit();
}


void app_lcd_first_page(void)
{
		
//	char aa[10] = "0000000000";
	GuiMessage *gui_message;
	gui_message = pvPortMalloc(sizeof(GuiMessage));
	
	memcpy(gui_message->level_battery,"00\0\0",4);
	memcpy(gui_message->date,"2017/01/01\0",11);
	memcpy(gui_message->time,"00:00:00\0",9);
	memcpy(gui_message->level_gps_nub,"00\0",4);
		
	gui_message->level_433 = 0;	
	gui_main(gui_message);
		
	 //gui_gsensor(1);
		//delay_ms(1000);
	 //gui_call(NULL);
	//	delay_ms(1000);
   //gui_sos(NULL);
	//	delay_ms(1000);
	gui_refresh();
	
  work_mode = main_page;
}

void app_lcd_main_page(void)
{
     //获取各块相应值刷新
	   		if(work_mode == low_level)
		{
		 //
	    hw_lcd_displayon();

		}
			
	//	work_mode = main_page;
		hw_lcd_clear_all();
	//HAL_GPIO_TogglePin(LED_KEY_R1_GPIO_Port, LED_KEY_R1_Pin);
		hw_led_key_call_set(GPIO_PIN_RESET);
    hw_led_key_sos_set(GPIO_PIN_RESET);
	
	// work_mode = main_page;

   app_lcd_copy_message();
		
	 gui_gps(show_level_gps,show_level_gps_nub);

   gui_433(show_level_433);
		
   gui_heartrate(show_level_hearteate);
		
	 gui_battery(show_level_battery);

	 gui_gsensor(show_level_gsensor);
	
	
   gui_date(show_date);
   gui_time(show_time);
	 //  hw_rtc_time_show();
	
			
		gui_refresh();
}



	void app_lcd_sos_page(void)
{

	app_lcd_copy_message();
  hw_led_key_call_set(GPIO_PIN_RESET);
  hw_led_key_sos_set(GPIO_PIN_SET);
	//work_mode = sos_page;
		hw_lcd_clear_all();
	 gui_gps(show_level_gps,show_level_gps_nub);

   gui_433(show_level_433);
		
   gui_heartrate(show_level_hearteate);
		
	 gui_battery(show_level_battery);

	 gui_gsensor(show_level_gsensor);
  gui_sos();
	//gui_433(show_level_433);
	gui_refresh();
}	

	void app_lcd_call_page(void)
{

	app_lcd_copy_message();
  hw_led_key_sos_set(GPIO_PIN_RESET);
  hw_led_key_call_set(GPIO_PIN_SET);
hw_lcd_clear_all();
	//work_mode = call_page;
	gui_gps(show_level_gps,show_level_gps_nub);

   gui_433(show_level_433);
		
   gui_heartrate(show_level_hearteate);
		
	 gui_battery(show_level_battery);

	 gui_gsensor(show_level_gsensor);
	gui_call();
	//gui_433(1);
	gui_refresh();
}	

	void app_lcd_sn_page(void)
{
//	uint8_t sn[12] ="123456789123"; 
  //sn号
	//获取sn号以及长度，如果失败，则显示123456
	
	gui_sn(sn_raw,sn_len);
	gui_refresh();
	//二维码
}	

	void app_lcd_success(void)
{
	app_lcd_copy_message();
  hw_led_key_call_set(GPIO_PIN_RESET);
  hw_led_key_sos_set(GPIO_PIN_SET);
	//work_mode = sos_page;
		
	 gui_gps(show_level_gps,show_level_gps_nub);

   gui_433(show_level_433);
		
   gui_heartrate(show_level_hearteate);
		
	 gui_battery(show_level_battery);

	 gui_gsensor(show_level_gsensor);
  gui_send_success();
	//gui_433(show_level_433);
	gui_refresh();
}
	void app_lcd_error(void)
{
	app_lcd_copy_message();
  hw_led_key_call_set(GPIO_PIN_RESET);
  hw_led_key_sos_set(GPIO_PIN_SET);
	//work_mode = sos_page;
		
	 gui_gps(show_level_gps,show_level_gps_nub);

   gui_433(show_level_433);
		
   gui_heartrate(show_level_hearteate);
		
	 gui_battery(show_level_battery);

	 gui_gsensor(show_level_gsensor);
  gui_send_error();
	//gui_433(show_level_433);
	gui_refresh();
}
