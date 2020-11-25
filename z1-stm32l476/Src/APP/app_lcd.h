#ifndef APP_LCD_H
#define APP_LCD_H



void app_lcd_first_page(void);
void app_lcd_main_page(void);
void app_lcd_sos_page(void);
void app_lcd_call_page(void);
void app_lcd_sn_page(void);
void app_lcd_success(void);
void app_lcd_error(void);
	
void app_lcd_mutex_enter(void);
void app_lcd_mutex_exit(void);
void app_lcd_get_sn(void);

#endif

