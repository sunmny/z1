
#include "task_bd1.h"
#include <stdint.h>
#include <stdio.h>
#include "cmsis_os.h"
#include "task_meerwespe.h"
#include "app_meerwespe.h"
#include "hw_bd1.h"
#include "app_power.h"

#include "hw_config.h"
#include "stm32l4xx_hal.h"
#include "delay.h"
#include "app_bd1.h"

 
/* Bd1 function */
void Bd1Task(void const * argument)
{

	osEvent event;
  /* USER CODE BEGIN 5 */
  /* Infinite loop */
	
	
  for(;;)
  {
		
//		printf("Bd1Task ---6\r\n");
  	  rd_receive_and_handle();

		osDelay(1000);
  }
  
 	  /* USER CODE END 5 */ 

}
