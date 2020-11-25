
/*
#include "includes.h"


void task_init(void * pvArg)
{
//	int i;
   	uint16 pow=0;
	uint8  kkkk =0;
    Event_Type evt;
//  evt.event = EVENT_REFRESH_LCD;
 // xQueueSend(g_queue_refresh, &evt, NULL);
	

   
   xQueueSend(g_queue_lcd, &evt, NULL);
	 xQueueSend(g_queue_gsm, &evt, NULL);
				 
taskENTER_CRITICAL();
   //初始化所有
	 APP_init();
	
   //显示信号强度、网络、电池信息
   APP_first_refresh(5);
 //  APP_refresh();
taskEXIT_CRITICAL();

	
	     
	
	#if USER_CHECK
	GUI_user_check("Baggio");
	#endif
				while(1)
				{
			#if USER_CHECK		
					
       //   GUI_userpwd_input();
					if(user_pwd_check_ok!=1)
				  	{
						APP_user_check();
						}
						else
						{
					//  vTaskResume(xHandlenfc);
						if(kkkk==0)
						{
							kkkk=1;
							//开启nfc主程序
							  evt.event = EVENT_NFC;
             xQueueSend(g_queue_nfc, &evt, NULL);
						}
					}
			#endif	

					
					 if(power_key()==1)
			{
			  pow++;
			}
			else
			{
			pow =0;
			}
						if(pow==400)
						{
							//显示关机画面

            GUI_goodbye();
								taskENTER_CRITICAL();
						APP_gsm_power_off();
						//	taskEXIT_CRITICAL();
						power_pwroff();
						}
	
		vTaskDelay(5);
				
				}


	*/
//}
/*
    while (1)
    {
  uint32 signal, network, batterycharge, batterylevel;

        evt.event = EVENT_REFRESH_LCD;
        xQueueSend(g_queue_refresh, &evt, NULL);
        vTaskDelay(500);
			if(power_key()==1)
			{
			  pow++;
			}
			if(pow>=5)
			{
				while( xQueueReceive(g_queue_lcd, &evt, portMAX_DELAY) != pdPASS);	
			  	
          lcd_showstring(0, 16, FONT16, "                ");				
				  lcd_showstring(0, 32, FONT16, "   关机中...    ");
				  lcd_showstring(0, 48, FONT16, "                ");
			//	lcd_showsignal(0, 0, 0);
           lcd_refresh();
				xQueueSend(g_queue_lcd, &evt, NULL);

     
				 power_gsm_pwroff();
        vTaskDelay(4000);
			//	delay_ms(5000);
				 power_gsm_pwron();
				vTaskDelay(1000);
	//			power_gsm_reset();
					pow =0;
			  power_pwroff();
			
			}
    }
	*/
//}

//		vTaskDelay(100);





