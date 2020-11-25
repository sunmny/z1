
/*
#include "gui.h"




void gui_signal(uint8 signal_level)
{
	Event_Type evt;
	
	while(xQueueReceive(g_queue_lcd, &evt, portMAX_DELAY) != pdPASS);
	taskENTER_CRITICAL();
	lcd_showsignal(0, 0, signal_level);
	lcd_refresh();
	taskEXIT_CRITICAL();
  xQueueSend(g_queue_lcd, &evt, NULL);
	
}



void gui_network(uint8 network_connection)
	
{
	Event_Type evt;	
	while(xQueueReceive(g_queue_lcd, &evt, portMAX_DELAY) != pdPASS);
	taskENTER_CRITICAL();
	lcd_shownetwork(38, 0, network_connection);
	lcd_refresh();
	taskEXIT_CRITICAL();
  xQueueSend(g_queue_lcd, &evt, NULL);
}

void gui_battery(uint8 battery_level)
{
	 Event_Type evt;
	
    while(xQueueReceive(g_queue_lcd, &evt, portMAX_DELAY) != pdPASS);
   	taskENTER_CRITICAL();
		lcd_showbattery(106, 0, battery_level);
	  lcd_refresh();
  	taskEXIT_CRITICAL();
    xQueueSend(g_queue_lcd, &evt, NULL);


}
*/