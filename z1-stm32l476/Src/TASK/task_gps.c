
#include "task_gps.h"
#include <stdint.h>
#include <stdio.h>
#include "cmsis_os.h"
#include "task_meerwespe.h"
#include "app_meerwespe.h"
#include "app_gps.h"
#include "hw_bd2.h"
#include "app_power.h"

#include "hw_config.h"
#include "stm32l4xx_hal.h"
#include "delay.h"
#include "app_bd2.h"



extern WORK_SEND_MODE  work_send_mode;
extern uint8_t first_flag;

extern osMessageQId GpsQueueHandle;
extern osMessageQId LoRaQueueHandle;

extern uint8_t wakeup_init_flag;

GPS_MODE gps_state = gps_close;
extern  WORK_SEND_MODE work_send_mode_huxi ;
extern  UART_HandleTypeDef huart1;
extern GpsLocation gpslocation;

osStatus task_gps_message_put(uint32_t info)
{
  	osStatus  status = osOK;
	 if(gps_state == gps_close)
	 {
		 //hw_gps_open();
		 	  gps_state = gps_open;
    status = osMessagePut(GpsQueueHandle, info, 5000);

	 }else
	 {
	   //查看当前发送状态
	   status = osMessagePut(GpsQueueHandle, info, 5000);
	 
	 }
	  return status;
}

osEvent task_gps_message_get(void)
{
  return osMessageGet(GpsQueueHandle,osWaitForever);
}






/* GpsTask function */
 uint8_t report_data[256]={0};
extern uint8_t dis_data[10];
extern osTimerId LteTimerHandle;
static uint8_t len ;
static uint8_t pack_head_andid[30] = "+BLOCATION:123456789123,";


void set_local_data(void)
{
	

}
void GpsTask(void const * argument)
{

	osEvent event;
  /* USER CODE BEGIN 5 */
  /* Infinite loop */

	uint8_t len;
	
	
  for(;;)
  {

//	printf("gps task \r\n");
			//获取队列
	 event = task_gps_message_get();
	if(event.status == osEventMessage)
	{
		
		switch(event.value.v)
		{
				case gps_open://首页
					hw_bd2_open();
					//HAL_GPIO_WritePin(DEBUG_LED_1_GPIO_Port, DEBUG_LED_1_Pin, GPIO_PIN_SET);
				break;
				case 2:
						 rn_receive_and_handle();
//						printf("event.value.v = %x \r\n",event.value.v);
					break;
				
			default://发回队列
					//	set_local_data();
			  break;
			
		}
	  }
	//while(0)
		
	{
	 
	  //HAL_GPIO_WritePin(DEBUG_LED_1_GPIO_Port, DEBUG_LED_1_Pin, GPIO_PIN_RESET);
	  //HAL_GPIO_TogglePin(DEBUG_LED_1_GPIO_Port, DEBUG_LED_1_Pin);
	  //osDelay(200);
		}
	}

	  /* USER CODE END 5 */ 

}
