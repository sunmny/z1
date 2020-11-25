/*
#include "includes.h"

void task_nfc(void * pvArg)
{
		 Event_Type evt;
//vTaskSuspend(xHandlenfc);

	while(1)
	{
     if( xQueueReceive(g_queue_nfc, &evt, portMAX_DELAY) != pdPASS)
            continue;

        switch (evt.event)
        {
        case EVENT_NFC:
					  APP_nfc();
				default:
            break;
        }
				 vTaskDelay(10);
        taskYIELD();
		   
	}
}

*/