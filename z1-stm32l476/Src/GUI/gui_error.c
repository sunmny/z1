//#include "includes.h"
//#include "gui.h"

/*
void gui_err(uint8 err)
{
	 Event_Type evt;
	
   switch(err)
	 {
		 case ERR_USER_CHECK:
          while(xQueueReceive(g_queue_lcd, &evt, portMAX_DELAY) != pdPASS);	 
	     	 taskENTER_CRITICAL();
  	     lcd_clean_nostateinfo();
	    	 lcd_showstring(64-MESSAGE_FONT_SIZE*4, 25, MESSAGE_FONT_SIZE, "密码错误,请重新输入");
		     lcd_showstring(104, 52, FONT12, "确认");
         lcd_refresh();
	    	 taskEXIT_CRITICAL();
         xQueueSend(g_queue_lcd, &evt, NULL);
	    	 break;
		 case ERR_CARD_PWD:
         while(xQueueReceive(g_queue_lcd, &evt, portMAX_DELAY) != pdPASS);	  
	    	 taskENTER_CRITICAL();
  	     lcd_clean_nostateinfo();
	       lcd_showstring(64-MESSAGE_FONT_SIZE*4, 25, MESSAGE_FONT_SIZE, "卡密码错误,请重新输入");
		     lcd_showstring(104, 52, FONT12, "确认");
         lcd_refresh();
	    	 taskEXIT_CRITICAL();
         xQueueSend(g_queue_lcd, &evt, NULL);
		     break;
		 case ERR_GSM_CONNECT:
			    while(xQueueReceive(g_queue_lcd, &evt, portMAX_DELAY) != pdPASS);	  
	    	 taskENTER_CRITICAL();
  	     lcd_clean_nostateinfo();
	       lcd_showstring(64-MESSAGE_FONT_SIZE*3, 25, MESSAGE_FONT_SIZE, "网络连接错误");
         lcd_showstring(104, 52, FONT12, "确认");
		     lcd_refresh();
	    	 taskEXIT_CRITICAL();
         xQueueSend(g_queue_lcd, &evt, NULL);
			   break;
		  case ERR_GSM_SENDDATA:
			    while(xQueueReceive(g_queue_lcd, &evt, portMAX_DELAY) != pdPASS);	  
	    	 taskENTER_CRITICAL();
  	     lcd_clean_nostateinfo();
	       lcd_showstring(64-MESSAGE_FONT_SIZE*4, 25, MESSAGE_FONT_SIZE, "请求错误");
         lcd_showstring(104, 52, FONT12, "确认");
			   lcd_refresh();
	    	 taskEXIT_CRITICAL();
         xQueueSend(g_queue_lcd, &evt, NULL);
			   break;
			 case ERR_GSM_CARDNO:
			    while(xQueueReceive(g_queue_lcd, &evt, portMAX_DELAY) != pdPASS);	  
	    	 taskENTER_CRITICAL();
  	     lcd_clean_nostateinfo();
	       lcd_showstring(64-MESSAGE_FONT_SIZE*3, 25, MESSAGE_FONT_SIZE, "卡号未注册");
         lcd_showstring(104, 52, FONT12, "确认");
			   lcd_refresh();
	    	 taskEXIT_CRITICAL();
         xQueueSend(g_queue_lcd, &evt, NULL);
			   break;
			  case ERR_GSM_SQL:
			    while(xQueueReceive(g_queue_lcd, &evt, portMAX_DELAY) != pdPASS);	  
	    	 taskENTER_CRITICAL();
  	     lcd_clean_nostateinfo();
	       lcd_showstring(64-MESSAGE_FONT_SIZE*4, 25, MESSAGE_FONT_SIZE, "服务器数据库错误");
         lcd_showstring(104, 52, FONT12, "确认");
				 lcd_refresh();
	    	 taskEXIT_CRITICAL();
         xQueueSend(g_queue_lcd, &evt, NULL);
			   break;
				case ERR_GSM_MONEYLOW:
			    while(xQueueReceive(g_queue_lcd, &evt, portMAX_DELAY) != pdPASS);	  
	    	 taskENTER_CRITICAL();
  	     lcd_clean_nostateinfo();
	       lcd_showstring(64-MESSAGE_FONT_SIZE*2, 25, MESSAGE_FONT_SIZE, "余额不足");
         lcd_showstring(104, 52, FONT12, "确认");
				 lcd_refresh();
	    	 taskEXIT_CRITICAL();
         xQueueSend(g_queue_lcd, &evt, NULL);
			   break;
		case ERR_GSM_TIMEOUT:
			    while(xQueueReceive(g_queue_lcd, &evt, portMAX_DELAY) != pdPASS);	  
	    	 taskENTER_CRITICAL();
  	     lcd_clean_nostateinfo();
	       lcd_showstring(64-MESSAGE_FONT_SIZE*2, 25, MESSAGE_FONT_SIZE, "连接超时");
         lcd_showstring(104, 52, FONT12, "确认");
		     lcd_refresh();
	    	 taskEXIT_CRITICAL();
         xQueueSend(g_queue_lcd, &evt, NULL);
			   break;
		case ERR_GSM_RECEIVE:
			    while(xQueueReceive(g_queue_lcd, &evt, portMAX_DELAY) != pdPASS);	  
	    	 taskENTER_CRITICAL();
  	     lcd_clean_nostateinfo();
	       lcd_showstring(64-MESSAGE_FONT_SIZE*3, 25, MESSAGE_FONT_SIZE, "应答数据错误");
         lcd_showstring(104, 52, FONT12, "确认");
		     lcd_refresh();
	    	 taskEXIT_CRITICAL();
         xQueueSend(g_queue_lcd, &evt, NULL);
			   break;
		
		
		
		 default:
	     break;
	}
}
*/
