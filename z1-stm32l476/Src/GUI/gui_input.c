
/*
#include "gui.h"
#include "includes.h"

 
uint8 user_pwd_nb;
uint32 user_pwd_refresh_nb;
uint8 user_pwd_input_enter;
uint8 tmp[2];

uint8 money_nb;
uint32 money_refresh_nb;
//uint8 money_input[MONEY_LEN]={0};

Event_Type evt;

 

//输入操作员密码
void GUI_userpwd_input(void)
{
  
	  uint8 key=0;
	  tmp[0]=0;
		tmp[1]=0;
    		 
//	while(1)
//	{
//	tmp[1]=0;
		key=key_getkey();
		tmp[0]=key;
		if(user_pwd_nb<USER_PASSWORD_LEN)
		{
	//	if(user_pwd_refresh_nb==0 || user_pwd_refresh_nb==1)
			if(user_pwd_refresh_nb==100)
		{
    while(xQueueReceive(g_queue_lcd, &evt, portMAX_DELAY) != pdPASS);	  
			taskENTER_CRITICAL();
  	lcd_showstring(MESSAGE_FONT_SIZE*4+user_pwd_nb*MESSAGE_FONT_SIZE/2, 32, MESSAGE_FONT_SIZE, "_");
    lcd_refresh();
			taskEXIT_CRITICAL();
    xQueueSend(g_queue_lcd, &evt, NULL);
	  }
	  // vTaskDelay(5);
		if(user_pwd_refresh_nb==200)
		{
			user_pwd_refresh_nb=0;
  	 while(xQueueReceive(g_queue_lcd, &evt, portMAX_DELAY) != pdPASS);	
				taskENTER_CRITICAL();
	    lcd_showstring(MESSAGE_FONT_SIZE*4+user_pwd_nb*MESSAGE_FONT_SIZE/2, 32, MESSAGE_FONT_SIZE, " ");
	    lcd_refresh();
			taskEXIT_CRITICAL();
      xQueueSend(g_queue_lcd, &evt, NULL);
			
	  }
	    user_pwd_refresh_nb++;
		  	
		if((tmp[0]>=KEY_VALUE_0)&&(tmp[0]<=KEY_VALUE_9))
		{
			 pwd_input[user_pwd_nb]=key;
		   pwd_input[user_pwd_nb+1]=0;
			APP_set_buzzer_nub(1,50);
	    while(xQueueReceive(g_queue_lcd, &evt, portMAX_DELAY) != pdPASS);	
				taskENTER_CRITICAL();
	    lcd_showstring(MESSAGE_FONT_SIZE*4+user_pwd_nb*MESSAGE_FONT_SIZE/2, 32, MESSAGE_FONT_SIZE, tmp);

			lcd_refresh();
			taskEXIT_CRITICAL();
			delay_ms(5);
				taskENTER_CRITICAL();
			lcd_showstring(MESSAGE_FONT_SIZE*4+user_pwd_nb*MESSAGE_FONT_SIZE/2, 32, MESSAGE_FONT_SIZE, "*");
	    lcd_refresh();
				taskEXIT_CRITICAL();
      xQueueSend(g_queue_lcd, &evt, NULL);
	     user_pwd_nb++;
		 }
		
		}
	
		if(tmp[0]==KEY_VALUE_CANCEL)
		{
		  	APP_set_buzzer_nub(1,100);
			  if(user_pwd_nb!=0)
				{
					user_pwd_nb--;
				}
				 pwd_input[user_pwd_nb]=0;
		     pwd_input[user_pwd_nb+1]=0;
				while(xQueueReceive(g_queue_lcd, &evt, portMAX_DELAY) != pdPASS);	
					taskENTER_CRITICAL();
		    lcd_showstring(MESSAGE_FONT_SIZE*4+user_pwd_nb*MESSAGE_FONT_SIZE/2, 32, MESSAGE_FONT_SIZE, "  ");
	      lcd_refresh();
					taskEXIT_CRITICAL();
				xQueueSend(g_queue_lcd, &evt, NULL);
		}
		
		if(tmp[0]==KEY_VALUE_ENTER)
		{
		//   pw=pwd;
			APP_set_buzzer_nub(1,100);
			user_pwd_input_enter = 1;
			//strcpy(pw,pwd);
		//	 break;

		}

//	}
	
}
//输入金额
//return 0 enter  1 cancel
uint8 GUI_money_input(uint8* money)
{
   // Event_Type evt;
	  uint8 key=0;
	
	  uint8 point_nb=0;//点次数
	 uint8 point_location=0;//点在数组中位置
 		tmp[0]=0;
		tmp[1]=0;
	    money_nb=0;
      money_refresh_nb=0;
	while(1)
	{
//	tmp[1]=0;
		key=key_getkey();
		tmp[0]=key;
		if(money_nb<MONEY_LEN)
		{
			if(((money_nb-point_location)<=2)||point_nb==0)
			{
	//	if(money_refresh_nb==0 || money_refresh_nb==1)
			if(money_refresh_nb==100)
		{
    while(xQueueReceive(g_queue_lcd, &evt, portMAX_DELAY) != pdPASS);	  
			taskENTER_CRITICAL();
  	lcd_showstring(INPUT_FONT_SIZE*3+money_nb*INPUT_FONT_SIZE/2, 39, INPUT_FONT_SIZE, "_");
    lcd_refresh();
					taskEXIT_CRITICAL();
    xQueueSend(g_queue_lcd, &evt, NULL);
	  }
	   vTaskDelay(5);
		if(money_refresh_nb==200)
		{
			money_refresh_nb=0;
  	 while(xQueueReceive(g_queue_lcd, &evt, portMAX_DELAY) != pdPASS);	
			taskENTER_CRITICAL();
	    lcd_showstring(INPUT_FONT_SIZE*3+money_nb*INPUT_FONT_SIZE/2, 39, INPUT_FONT_SIZE, " ");
	    lcd_refresh();
				taskEXIT_CRITICAL();
      xQueueSend(g_queue_lcd, &evt, NULL);
			vTaskDelay(10);
	  }
	    money_refresh_nb++;
	   
		if(((tmp[0]>=KEY_VALUE_0)&&(tmp[0]<=KEY_VALUE_9)) || tmp[0]==KEY_VALUE_DOT)
		{
			if(tmp[0]==KEY_VALUE_DOT)
			{
			
			   point_nb++;
				
			}
			if(point_nb<=1)
			{
					if(tmp[0]==KEY_VALUE_DOT)
			    {
			
			   point_location =money_nb;
				
		     	}
				
			 money[money_nb]=key;
		   money[money_nb+1]=0;
			APP_set_buzzer_nub(1,50);
	    while(xQueueReceive(g_queue_lcd, &evt, portMAX_DELAY) != pdPASS);	
					taskENTER_CRITICAL();
	    lcd_showstring(INPUT_FONT_SIZE*3+money_nb*INPUT_FONT_SIZE/2, 39, INPUT_FONT_SIZE, tmp);
		//	lcd_refresh();
		//	delay_ms(5);
		//	lcd_showstring(INPUT_FONT_SIZE*3+money_nb*INPUT_FONT_SIZE/2, 39, INPUT_FONT_SIZE, "*");
			lcd_showstring(0, 52, FONT12, "退格");
	    lcd_refresh();
				taskEXIT_CRITICAL();
      xQueueSend(g_queue_lcd, &evt, NULL);
			tmp[0]=0;
	     money_nb++;
			}
			else
			{
			  point_nb=2;
			}
	 }
		}
		}
	
		if(money_nb==0)
		{
		   if(tmp[0]==KEY_VALUE_CANCEL)
		   {
				 return 1;
	    	}
		}
		
		if(tmp[0]==KEY_VALUE_CANCEL)
		{
		  	APP_set_buzzer_nub(1,100);
			  if(money_nb!=0)
				{
					money_nb--;
				}
				if(point_location==money_nb)
				{
				 point_nb=0;
				}
				 money[money_nb]=0;
		     money[money_nb+1]=0;
				while(xQueueReceive(g_queue_lcd, &evt, portMAX_DELAY) != pdPASS);	
				taskENTER_CRITICAL();
		    lcd_showstring(INPUT_FONT_SIZE*3+money_nb*INPUT_FONT_SIZE/2, 39, INPUT_FONT_SIZE, "  ");
				if(money_nb==0)
					lcd_showstring(0, 52, FONT12, "返回");
	      lcd_refresh();
				taskEXIT_CRITICAL();
				xQueueSend(g_queue_lcd, &evt, NULL);
				tmp[0]=0;
		}
		
		if(tmp[0]==KEY_VALUE_ENTER)
		{
		//   pw=pwd;
			APP_set_buzzer_nub(1,100);
			
			return 0;

			
		}

	}
	
}

*/