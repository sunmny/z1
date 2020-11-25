#ifndef GUI_MESSAGE_H
#define GUI_MESSAGE_H
#include <stdint.h>
	
typedef struct
{ 
uint8_t        time[9]; 
uint8_t        date[11]; 
uint8_t        level_battery[4]; 
uint8_t        level_gps_nub[4];
uint8_t        level_gps; 
uint8_t        level_433; 
uint8_t        level_gsensor; 
uint8_t        level_hearteate; 
	
}GuiMessage;


void gui_main(GuiMessage *gui_message);
void gui_gps(uint8_t level_gps,unsigned char *level_gps_nub );
void gui_433(uint8_t level_433);
void gui_battery(unsigned char *level_battery);
void gui_heartrate(uint8_t level_hearteate);
void gui_date(unsigned char  *date);
void gui_time(unsigned char  *time);
void gui_gsensor(uint8_t level);
//报警
void gui_call(void);
//求救
void gui_sos(void);

//void gui_sn(unsigned char *sn );
void gui_sn(unsigned char *sn,uint8_t len );

void test_refresh(void);
void gui_refresh(void);


//成功
void gui_send_success(void);

//失败
void gui_send_error(void);

#endif

