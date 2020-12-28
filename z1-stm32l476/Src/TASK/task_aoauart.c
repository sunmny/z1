
#include "task_aoauart.h"
#include <stdint.h>
#include <stdio.h>
#include "cmsis_os.h"
#include "string.h"
#include "task_meerwespe.h"
#include "app_meerwespe.h"

#include "hw_config.h"
#include "stm32l4xx_hal.h"
#include "delay.h"
#include "app_aoa.h"



//extern WORK_SEND_MODE  work_send_mode;

//extern osMessageQId AoaUartQueueHandle;

//extern uint8_t wakeup_init_flag;

//extern  WORK_SEND_MODE work_send_mode_huxi ;

//extern osMailQId  AoaUartMailQueueHandle;

osStatus task_aoauart_message_put(uint32_t info)
{
  //	osStatus  status = osOK;
	//status = osMessagePut(AoaUartQueueHandle, info, 5000);
	 
	//return status;
}

osEvent task_aoauart_message_get(void)
{
 // return osMessageGet(AoaUartQueueHandle, 1/*osWaitForever*/);
}


osStatus task_aoauart_mail_put(uint8_t *data, uint16_t len)
{
	//aoamail *paoamail;
	//paoamail=osMailCAlloc(AoaUartMailQueueHandle, osWaitForever);
	//if(len>AOA_MAIL_BUF_LEN)
	//	paoamail->len=AOA_MAIL_BUF_LEN;
	//else
	//	paoamail->len=len;
//	memcpy(paoamail->data,data,paoamail->len);
//	return osMailPut(AoaUartMailQueueHandle, paoamail);
}

osEvent task_aoauart_mail_get(void)
{
//	return osMailGet(AoaUartMailQueueHandle, 1/*osWaitForever*/);//parameter to do 201814025
}


extern  WWDG_HandleTypeDef   WwdgHandle;

/* GpsTask function */
void AoaUartTask(void const * argument)
{

	

  for(;;)
  {
			//
	//	event = task_aoauart_mail_get();
	//	if (event.status == osEventMail)
		//{
		//	paoamail=event.value.p;
		//	aoa_mail_receive_and_handle(paoamail);
		//	osMailFree(AoaUartMailQueueHandle, paoamail);
			//HAL_GPIO_TogglePin(DEBUG_LED_1_GPIO_Port, DEBUG_LED_1_Pin);
	//	}

		ulTaskNotifyTake( pdTRUE, portMAX_DELAY);
		aoa_uart_receive_and_handle();
	//	printf("AoaUartTask ---3\r\n");

		// osDelay(400);
		//hw_uart1_send_dma("hello ",6);
	}
	  /* USER CODE END 5 */ 

}
