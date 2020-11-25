/**
---------------------------------------------------------------------
**
**  This device driver was created by 
**  8-Bit Single-Chip Microcontrollers
**
**  Copyright(C) visionox display Corporation 2002 - 2009
**  All rights reserved by Kunshan visionox display Corporation.
**
**  This program should be used on your own responsibility.
**
**  Filename :	Display_Mode.c
**  Abstract :	This file implements device driver for Display_Mode function.
**
**  Device :	STC89LE516RD
**
**  Compiler :	KEIL uVision2 
**
**  Module:  M01120
**
**  Dot Matrix: 96*32
**
**  Display Color: White
**
**  Driver IC: SH1106G
**
**  Edit : 
**
**  Creation date:	2012-08-16
---------------------------------------------------------------------
**/

//#include "IC.h"
//#include "IC_Init.h"
#include "Photo_Show.h"
//#include "Delay_Custom.h"
#include "hw_lcd.h"
//#include "intrins.h"

//
//

#define uchar unsigned char
#define uint unsigned int
	
uchar ROW[3][4]={
{0x49,0x92,0x24,0x49},
{0x92,0x24,0x49,0x92},
{0x24,0x49,0x92,0x24}
};

extern uchar ROW[3][4];

/******************************************************
//
//清屏显示模式
//
******************************************************/
//
//

void Clear_Screen()
{
   uchar page_number,column_number;
   for(page_number=MIN;page_number<PAGE_TOTAL/2;page_number++)
   {
     lcd_writecommand(START_PAGE+page_number);
     lcd_writecommand(START_HIGH_BIT);
     lcd_writecommand(START_LOW_BIT);
     for(column_number=MIN;column_number<COLUMN_MAX;column_number++)
     {
        lcd_writedata(STATE_MIN);
     }
   }
}

/******************************************************
//
//全屏显示模式
//
******************************************************/
//
//

void All_Screen()
{
   uchar page_number,column_number;
   for(page_number=MIN;page_number<PAGE_TOTAL/2;page_number++)
   {
     lcd_writecommand(START_PAGE+page_number);
     lcd_writecommand(START_HIGH_BIT);
     lcd_writecommand(START_LOW_BIT);
	 //Delay_s(50);
     for(column_number=MIN;column_number<COLUMN_MAX;column_number++)
     {	   
        lcd_writedata(STATE_MAX);
     }
   }
}

/******************************************************
//
//隔行135显示模式
//
******************************************************/
//
//

void Row_Show135()
{
   uchar page_number,column_number;
   for(page_number=MIN;page_number<PAGE_TOTAL/2;page_number++)
   {
     lcd_writecommand(START_PAGE+page_number);
     lcd_writecommand(START_HIGH_BIT);
     lcd_writecommand(START_LOW_BIT);

     for(column_number=MIN;column_number<COLUMN_MAX;column_number++)
     {
        lcd_writedata(STATE_55);
     }
   }
}

/******************************************************
//
//隔行246显示模式
//
******************************************************/
//
//

void Row_Show246()
{
   uchar page_number,column_number;
   for(page_number=MIN;page_number<PAGE_TOTAL/2;page_number++)
   {
     lcd_writecommand(START_PAGE+page_number);
     lcd_writecommand(START_HIGH_BIT);
     lcd_writecommand(START_LOW_BIT);
     for(column_number=MIN;column_number<COLUMN_MAX;column_number++)
     {
        lcd_writedata(STATE_AA);
     }
   }
}


/******************************************************
//
//隔列135显示模式
//
******************************************************/
//
//

void Column_Show135()
{
   uchar page_number,column_number;
   for(page_number=MIN;page_number<PAGE_TOTAL/2;page_number++)
   {
     lcd_writecommand(START_PAGE+page_number);
     lcd_writecommand(START_HIGH_BIT);
     lcd_writecommand(START_LOW_BIT);
     for(column_number=MIN;column_number<(COLUMN_MAX/2);column_number++)
     {
        lcd_writedata(STATE_MAX);
        lcd_writedata(STATE_MIN);
     }
   }
}

/******************************************************
//
//隔列246显示模式
//
******************************************************/
//
//

void Column_Show246()
{
   uchar page_number,column_number;
   for(page_number=MIN;page_number<PAGE_TOTAL/2;page_number++)
   {
     lcd_writecommand(START_PAGE+page_number);
     lcd_writecommand(START_HIGH_BIT);
     lcd_writecommand(START_LOW_BIT);
     for(column_number=MIN;column_number<(COLUMN_MAX/2);column_number++)
     {
        lcd_writedata(STATE_MIN);
        lcd_writedata(STATE_MAX);
     }
   }
}
 /******************************************************
//
//隔列147显示模式
//
******************************************************/
//
//

void Column_Show147()
{
   uchar page_number,column_number;
   for(page_number=MIN;page_number<PAGE_TOTAL/2;page_number++)
   {
     lcd_writecommand(START_PAGE+page_number);
     lcd_writecommand(START_HIGH_BIT);
     lcd_writecommand(START_LOW_BIT);
     for(column_number=MIN;column_number<42;column_number++)
     {
        lcd_writedata(STATE_MAX);
        lcd_writedata(STATE_MIN);
		lcd_writedata(STATE_MIN);
     }
	    lcd_writedata(STATE_MAX);
        lcd_writedata(STATE_MIN);
   }
}
/******************************************************
//
//隔列258显示模式
//
******************************************************/
//
//

void Column_Show258()
{
   uchar page_number,column_number;
   for(page_number=MIN;page_number<PAGE_TOTAL/2;page_number++)
   {
     lcd_writecommand(START_PAGE+page_number);
     lcd_writecommand(START_HIGH_BIT);
     lcd_writecommand(START_LOW_BIT);
     for(column_number=MIN;column_number<42;column_number++)
     {
	    lcd_writedata(STATE_MIN);
        lcd_writedata(STATE_MAX);
        lcd_writedata(STATE_MIN);
	
     }
	   lcd_writedata(STATE_MIN);
        lcd_writedata(STATE_MAX);
   }
}
/******************************************************
//
//隔列369显示模式
//
******************************************************/
//
//

void Column_Show369()
{
   uchar page_number,column_number;
   for(page_number=MIN;page_number<PAGE_TOTAL/2;page_number++)
   {
     lcd_writecommand(START_PAGE+page_number);
     lcd_writecommand(START_HIGH_BIT);
     lcd_writecommand(START_LOW_BIT);
     for(column_number=MIN;column_number<42;column_number++)
     {
	    lcd_writedata(STATE_MIN);
	    lcd_writedata(STATE_MIN);
        lcd_writedata(STATE_MAX);
       
	
     }
	 lcd_writedata(STATE_MIN);
	 lcd_writedata(STATE_MIN);
   }
}
/******************************************************
//
//边框显示模式128*36
//
******************************************************/
//
//

void Frame_Show()
{
   uchar page_number,column_number;
   lcd_writecommand(START_PAGE);
   lcd_writecommand(START_HIGH_BIT);
   lcd_writecommand(START_LOW_BIT);
   lcd_writedata(STATE_MAX);
   for(column_number=MIN;column_number<(COLUMN_MAX-2);column_number++)
   {
      lcd_writedata(FRAME_HIGH_ROW);
   }
   lcd_writedata(STATE_MAX);
   for(page_number=MIN+1;page_number<PAGE_TOTAL/2;page_number++)
   {
     lcd_writecommand(START_PAGE+page_number);
     lcd_writecommand(START_HIGH_BIT);
     lcd_writecommand(START_LOW_BIT);
     lcd_writedata(STATE_MAX);
     for(column_number=MIN;column_number<(COLUMN_MAX-2);column_number++)
     {
        lcd_writedata(STATE_MIN);
     }
     lcd_writedata(STATE_MAX);
   }
   lcd_writecommand(START_PAGE+3);
   lcd_writecommand(START_HIGH_BIT);
   lcd_writecommand(START_LOW_BIT);
   lcd_writedata(STATE_MAX);
   for(column_number=MIN;column_number<(COLUMN_MAX-2);column_number++)
   {
      lcd_writedata(FRAME_LOW_ROW);
   }
   lcd_writedata(STATE_MAX);

}

//
//

/******************************************************
//
//棋盘格1
//
******************************************************/
//
//

void Chess_Board1()
{
   uchar page_number,column_number_1,column_number_2,board;
   board=0xFF;
   for(page_number=MIN;page_number<PAGE_TOTAL/2;page_number++)
   {
	  lcd_writecommand(START_PAGE+page_number);
      lcd_writecommand(START_HIGH_BIT);
      lcd_writecommand(START_LOW_BIT);
	  for(column_number_1=MIN;column_number_1<COLUMN_MAX/8;column_number_1++)
	  {
		 for(column_number_2=MIN;column_number_2<COLUMN_MAX/16;column_number_2++)
	     {
	   	    lcd_writedata(board);
		 }
         board=~board;
	  }
	  board=~board;
   }
}	

/******************************************************
//
//棋盘格2
//
******************************************************/
//
//

void Chess_Board2()
{
    uchar page_number,column_number_1,column_number_2,board;
   board=0x00;
   for(page_number=MIN;page_number<PAGE_TOTAL/2;page_number++)
   {
	  lcd_writecommand(START_PAGE+page_number);
      lcd_writecommand(START_HIGH_BIT);
      lcd_writecommand(START_LOW_BIT);
	  for(column_number_1=MIN;column_number_1<COLUMN_MAX/8;column_number_1++)
	  {
		 for(column_number_2=MIN;column_number_2<COLUMN_MAX/16;column_number_2++)
	     {
	   	    lcd_writedata(board);
		 }
         board=~board;
	  }
	  board=~board;
   }
}

/******************************************************
//
//列扫描显示模式
//
******************************************************/
//
//

void Column_Scan()
{
   uchar page_number,column_number_1,column_number_2;
   for(column_number_1=MIN;column_number_1<COLUMN_MAX;column_number_1++)
   {
      for(page_number=MIN;page_number<PAGE_TOTAL/2;page_number++)
      {
         lcd_writecommand(START_PAGE+page_number);
         lcd_writecommand(START_HIGH_BIT);
         lcd_writecommand(START_LOW_BIT);
         for(column_number_2=MIN;column_number_2<COLUMN_MAX;column_number_2++)
         {
		    if(column_number_2==column_number_1)
		    lcd_writedata(STATE_MAX);
		    else lcd_writedata(STATE_MIN);
		 }
      } 
      delay_ms(60);
   }
}

//
//
/******************************************************
//
//程序版本信息
//
******************************************************/
//
//

void Program_Edit()
{
   uchar page_number,column_number;
   uint piexl=MIN;
   for(page_number=MIN;page_number<PAGE_TOTAL/2;page_number++)
   {
      lcd_writecommand(START_PAGE+page_number);
      lcd_writecommand(START_HIGH_BIT);
      lcd_writecommand(START_LOW_BIT);
      for(column_number=MIN;column_number<COLUMN_MAX;column_number++)
      {
				delay_ms(2);
		 lcd_writedata(tab_1[piexl]);
		 piexl++;
	  }
  }
} 
//
//
/******************************************************
//
//图片检测函数
//
******************************************************/
//
//

void Photo_Show()
{
   uchar page_number,column_number;
   uint piexl=MIN;
   for(page_number=MIN;page_number<PAGE_TOTAL/2;page_number++)
   {
      lcd_writecommand(START_PAGE+page_number);
      lcd_writecommand(START_HIGH_BIT);
      lcd_writecommand(START_LOW_BIT);
      for(column_number=MIN;column_number<COLUMN_MAX;column_number++)
      {
				delay_ms(2);
		 lcd_writedata(tab_2[piexl]);
		 piexl++;
	  }
  }
}

void Photo_Show1()
{
   uchar page_number,column_number;
   uint piexl=MIN;
   for(page_number=MIN;page_number<PAGE_TOTAL/2;page_number++)
   {
      lcd_writecommand(START_PAGE+page_number);
      lcd_writecommand(START_HIGH_BIT);
      lcd_writecommand(START_LOW_BIT);
      for(column_number=MIN;column_number<COLUMN_MAX;column_number++)
      {
				delay_ms(2);
		 lcd_writedata(tab_3[piexl]);
		 piexl++;
	   }
  }
}
//
//

/******************************************************
//
//LED指示灯 和 VCC_Change电压控制函数
//
******************************************************/
//
//
/*
void LED_VCC_Control(uchar Control_State)
{
  uchar value;
  value = Control_State;
  switch(value)
  {
     case 0: 
             {delay_ms(1);LED_Work=low;LED_Manual=high;LED_Auto=low;VCC_Change=low;} break;
     case 1: 
             {delay_ms(1);LED_Work=low;LED_Manual=low;LED_Auto=high;VCC_Change=low;} break;
	 default: ;
  }
}
*/
//
//

/******************************************************
//
//按键扫描函数
//
******************************************************/
//
//
/*
uchar Key_Scan()
{
  	uchar key_value;
	key_value = 0x00;
	key_value |= Key_Manual;
    key_value = _crol_(key_value,1);
	key_value |= Key_Auto;
	return key_value;		 
}
*/

/******************************************************
//
//按键处理函数
//*******手动按键检测
//*******自动按键检测
//
******************************************************/
//
//
/*

void Key_Processor(uchar key_proc)
{
   uchar i,n;
   i=163;
   if((key_proc & 0x01) == 0)                     //自动检测
   {
      LED_VCC_Control(Auto);
      while(i--)
	  {
	     All_Screen();
	     Delay_s(4);
	     Clear_Screen();
		 delay_ms(4);
	     Column_Scan();
	     Clear_Screen();
	     delay_ms(5);
	  }
      LED_Manual=high;LED_Auto=high;
      All_Screen();
   }
   else if((key_proc & 0x02) == 0)                           //手动检测
   {
      LED_VCC_Control(Manual);
      switch(n)
	  {
	     case 0:  
		         {Clear_Screen();delay_ms(10);Program_Edit();n++;} break;
		 case 1:
		         {Clear_Screen();delay_ms(10);All_Screen();n++;} break;
		 case 2:
		         {Clear_Screen();delay_ms(10);Clear_Screen();n++;} break;
		 case 3:
		         {Clear_Screen();delay_ms(10);Row_Show135();n++;} break;
         case 4:  
		         {Clear_Screen();delay_ms(10);Row_Show246();n++;} break;
		 case 5:
		         {Clear_Screen();delay_ms(10);Column_Show135();n++;} break;
		 case 6:
		         {Clear_Screen();delay_ms(10);Column_Show246();n++;} break;
		 case 7:
		         {Clear_Screen();delay_ms(10);Column_Show147();n++;} break;
		 case 8:  
		         {Clear_Screen();delay_ms(10);Column_Show258();n++;} break;
		 case 9:
		         {Clear_Screen();delay_ms(10);Column_Show369();n++;} break;
		 case 10:
		         {Clear_Screen();delay_ms(10);Frame_Show();n++;} break;
		 case 11:
		         {Clear_Screen();delay_ms(10);Chess_Board1();n++;} break;
		 case 12:  
		         {Clear_Screen();delay_ms(10);Chess_Board2();n++;} break;
		 case 13:
		         {Clear_Screen();delay_ms(10);Photo_Show();n = 0;} break;
		 
	 }
   }
}
	 */

void show_test(void)
{
  // Reset_IC();                    //复位IC
 //  Init_IC();                     //初始化IC
   All_Screen();                       //全屏显示效果
   while(1)
   {
     Program_Edit();  
		 delay_ms(1000);
		 
		 Photo_Show();
	   delay_ms(1000);
		  Photo_Show1();
	   delay_ms(1000);
   }
 }