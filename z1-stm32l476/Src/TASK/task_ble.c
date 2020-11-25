
#include "task_ble.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "cmsis_os.h"
#include "task_meerwespe.h"
#include "app_meerwespe.h"

#include "hw_config.h"
#include "stm32l4xx_hal.h"
#include "delay.h"
#include "app_ble.h"

#include "hw_uart.h" //for test





extern osMessageQId BleQueueHandle;

extern osMailQId BleMailQueueHandle;


osStatus task_ble_message_put(uint32_t info)
{
  //	osStatus  status = osOK;
	//status = osMessagePut(BleQueueHandle, info, osWaitForever);
	 
	//return status;
}

osEvent task_ble_message_get(void)
{
  //return osMessageGet(BleQueueHandle, osWaitForever);
}

osStatus task_ble_mail_put(uint8_t *data, uint16_t len)
{
//blemail *pblemail;
//	pblemail=osMailCAlloc(BleMailQueueHandle, osWaitForever);
//	if(len>BLE_MAIL_BUF_LEN)
	//	pblemail->len=BLE_MAIL_BUF_LEN;
//	else
	//	pblemail->len=len;
	//memcpy(pblemail->data,data,pblemail->len);
	//return osMailPut(BleMailQueueHandle, pblemail);
}

osEvent task_ble_mail_get(void)
{
	//return osMailGet(BleMailQueueHandle, 1/*osWaitForever*/);//parameter to do, 20181025
}



/* BleTask function */
void BleTask(void const * argument)
{

	//osEvent event;
	//blemail *pblemail;
  /* USER CODE BEGIN 5 */
  /* Infinite loop */
	

	
  for(;;)
  {
		
		//}
//printf("ble task\r\n");
		//event = task_ble_mail_get();
		
	//	task_test_mail_put("hello",5);
		//if (event.status == osEventMail)
	//	{
		//	pblemail=event.value.p;
		//	ble_mail_receive_and_handle(pblemail);
			
			//hw_uart4_send(pblemail->data, pblemail->len);
			//osMailFree(BleMailQueueHandle, pblemail);
			//HAL_GPIO_TogglePin(DEBUG_LED_1_GPIO_Port, DEBUG_LED_1_Pin);
		//}

	//osDelay(500);
		
			ulTaskNotifyTake( pdTRUE, portMAX_DELAY);
		printf("BleTask ---2\r\n");
		//osDelay(1000);
		ble_receive_and_handle();
		}

	  /* USER CODE END 5 */ 
}
