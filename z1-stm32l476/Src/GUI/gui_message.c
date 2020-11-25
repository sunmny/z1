

#include "gui_message.h"
#include "hw_lcd.h"
#include "app_qrcode.h"
#include "app_meerwespe.h"
#include "string.h"


extern GPS_MODE gps_state;
extern LORA_MODE lora_state;

uint8_t jishu[10]={0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39};
uint8_t ji = 0;

void gui_main(GuiMessage *gui_message)
{
	
hw_lcd_clear_all();
	
//gps
		if(gps_state == gps_close)
	{
hw_lcd_show_gps(3, 0, gui_message->level_gps);
hw_lcd_showstring(3+9,3,8,gui_message->level_gps_nub);//定位的星
	}
//433
hw_lcd_show_433_lora(128-18-13-4-19-1,0,0);
hw_lcd_show_433(128-18-13-4, 0, gui_message->level_433);	
//电池
hw_lcd_show_battery(128-18-1,0, 0);
//电池

//hw_lcd_show_battery(128-18,0, 0);
	
  if(*(gui_message->level_battery + 1) == '\0')
	{
	  hw_lcd_showstring(128-18+6+2-1,1,8, gui_message->level_battery);//电池电量百分比  10以下
	}
	else if(*(gui_message->level_battery + 2 )== '\0')
	{
		hw_lcd_showstring(128-18+6-1,1,8, gui_message->level_battery);//电池电量百分比  100
	}else if(*(gui_message->level_battery + 3 )== '\0')
	{	
    hw_lcd_showstring(128-18+6-3-1,1,8, gui_message->level_battery);//电池电量百分比
	}
//心率
hw_lcd_show_heartrate(3, 32-7, gui_message->level_hearteate);
//时间 ，日期
hw_lcd_showstring(32,8,16,gui_message->time);
hw_lcd_showstring(39,32-8,8,gui_message->date);
//动静	
hw_lcd_show_gsensor(128-10-1, 32-15, gui_message->level_gsensor);
	

//刷屏
//hw_lcd_refresh();
	
	
//hw_lcd_refresh();

}

void gui_refresh(void)
{
  //刷屏
   hw_lcd_refresh();
}

void test_refresh(void)
{
	/*
		uint8_t count[2] = {0x00,0x00};
  	//计数
	count[0] = jishu[ji];
	count[1] = '\0';
	ji++;
	if(ji >=9)
		ji = 0;
	
	//hw_lcd_showstring(30,0,8, count);//
*/	

//刷屏
//hw_lcd_refresh();

}


//gps
void gui_gps(uint8_t level_gps,unsigned char *level_gps_nub )
{
	if(gps_state == gps_close)
	{
  hw_lcd_show_gps(3, 0, level_gps);
  hw_lcd_showstring(3+9,3,8,level_gps_nub);//定位的星
  //hw_lcd_refresh();
	}
}

//433
void gui_433(uint8_t level_433)
{
  hw_lcd_show_433_lora(128-18-13-4-19-3,0,0);
  hw_lcd_show_433(128-18-13-4, 0,level_433);	
  //hw_lcd_refresh();
}

//battery
void gui_battery(unsigned char *level_battery)
{
	/*
  //电池
  hw_lcd_show_battery(128-18-1,0, 0);
	
  if(*(level_battery + 1) == '\0')
	{
	  hw_lcd_showstring(128-18+6+2-1,1,8, level_battery);//电池电量百分比 10以下
	}
	else if(*(level_battery + 2 )== '\0')
	{
		hw_lcd_showstring(128-18+6-1,1,8,level_battery);//电池电量百分比  10-99
	}else if(*(level_battery + 3 )== '\0')
	{	
    hw_lcd_showstring(128-18+6-3-1,1,8,level_battery);//电池电量百分比 100
	}
	*/
	//电池
hw_lcd_show_battery(128-18-1,0, 0);
//电池

//hw_lcd_show_battery(128-18,0, 0);
  if(*(level_battery + 1) == '\0')
	{
	  hw_lcd_showstring(128-18+6+2-1,1,8, level_battery);//电池电量百分比  10以下
	}
	else if(*(level_battery + 2 )== '\0')
	{
		hw_lcd_showstring(128-18+6-1,1,8, level_battery);//电池电量百分比  100
	}else if(*(level_battery + 3 )== '\0')
	{	
    hw_lcd_showstring(128-18+6-3-1,1,8, level_battery);//电池电量百分比
	}
 // hw_lcd_refresh();
}

//hearteate
void gui_heartrate(uint8_t level_hearteate)
{
  hw_lcd_show_heartrate(3, 32-7, level_hearteate);
	//hw_lcd_refresh();
}

//日期
void gui_date(unsigned char  *date)
{
  hw_lcd_showstring(39,32-8,8,date);
	//hw_lcd_refresh();
}

//时间
void gui_time(unsigned char  *time)
{
  hw_lcd_showstring(32,8,16,time);
	//hw_lcd_refresh();
}


//动静
void gui_gsensor(uint8_t level_gsensor)
{

  hw_lcd_show_gsensor(128-10-1, 32-15, level_gsensor);

	//hw_lcd_refresh();
}


//SN
void gui_sn(unsigned char *sn,uint8_t len )
{
	unsigned char name[16]="SN:\0";
	unsigned char sn2[14]={0x00};
	
	memcpy(name+3,sn,len);
	name[3+len] = '\0';

  hw_lcd_clear_all2();
	if(len>6)
	{
	hw_lcd_showstring2(3+6*3,17,12,name+9);//
	name[9] = '\0';
  hw_lcd_showstring2(3,5,12,name);//
	
	}else{
	hw_lcd_showstring2(3,8,16,name);
	}
	
	memcpy(sn2,sn,len);
	sn2[len] = '\r';
	sn2[len+1] = '\n';
	
	app_qrcode(sn2);

	//hw_lcd_refresh();
}

void gui_call(void)
{
	
	unsigned char name[6]="报 警\0";
	unsigned char time[9]="        \0";
	unsigned char date[11]="          \0";
	unsigned char sn[12]="           \0";
	//清除时间和日期区域

	hw_lcd_showstring(32,8,16,time);
  hw_lcd_showstring(39,32-8,8,date);
	hw_lcd_showstring(10,12,12,sn);
  //hw_lcd_clear_all();
  hw_lcd_showstring(37,8,24,name);//

	//hw_lcd_refresh();
}

//求救
void gui_sos(void)
{
	unsigned char name[6]="求 救\0";
	unsigned char time[9]="        \0";
	unsigned char date[11]="          \0";
	unsigned char sn[12]="           \0";
	//清除时间和日期区域
	hw_lcd_showstring(32,8,16,time);
  hw_lcd_showstring(39,32-8,8,date);
	hw_lcd_showstring(10,12,12,sn);
 // hw_lcd_clear_all();
  hw_lcd_showstring(37,8,24,name);//
	//hw_lcd_refresh();
}

//成功
void gui_send_success(void)
{
	unsigned char name[6]="成 功\0";
	//unsigned char time[9]="        \0";
	//unsigned char date[11]="          \0";
	//unsigned char sn[12]="           \0";
	//清除时间和日期区域
	//hw_lcd_showstring(32,8,16,time);
  //hw_lcd_showstring(39,32-8,8,date);
	//hw_lcd_showstring(10,12,12,sn);
 // hw_lcd_clear_all();
  hw_lcd_showstring(37,8,24,name);//
	//hw_lcd_refresh();
}

//失败
void gui_send_error(void)
{
	unsigned char name[6]="失 败\0";
	//unsigned char time[9]="        \0";
	//unsigned char date[11]="          \0";
	//unsigned char sn[12]="           \0";
	//清除时间和日期区域
	//hw_lcd_showstring(32,8,16,time);
  //hw_lcd_showstring(39,32-8,8,date);
	//hw_lcd_showstring(10,12,12,sn);
 // hw_lcd_clear_all();
  hw_lcd_showstring(37,8,24,name);//
	//hw_lcd_refresh();
}
