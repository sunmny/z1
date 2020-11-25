#ifndef _LCD_H_
#define _LCD_H_

#include "stdint.h"
//#include "stm32l4xx_hal_def.h"
/*
typedef signed char        int8;
typedef unsigned char      uint8;
typedef signed short       int16;
typedef unsigned short     uint16;
typedef signed long        int32;
typedef unsigned long      uint32;
typedef long long          int64;
typedef unsigned long long uint64;
typedef unsigned long      DWORD;
typedef unsigned char      BYTE;
typedef unsigned char      u8_sw;
typedef uint8              OutTime_u8;
typedef unsigned long   DWORD;
typedef unsigned short  WORD;
*/

typedef int8_t        int8;
typedef uint8_t      uint8;
typedef int16_t       int16;
typedef uint16_t     uint16;
typedef int32_t        int32;
typedef uint32_t      uint32;
typedef int64_t          int64;
typedef uint64_t         uint64;
//typedef uint32_t      DWORD;
typedef uint8_t      BYTE;
typedef uint8_t      u8_sw;
typedef uint8_t              OutTime_u8;
//typedef uint32_t   DWORD;
//typedef uint16_t  WORD2;

#define LCD_I2C                I2C_2
#define LCD_I2C_ADDRESS        0x78 //0x30F

//#include "type.h"

#define	FONT_CUSTOM     8   //6*8 自己画的
#define	FONT12          12
#define	FONT16          16
#define	FONT24          24
//#define	FONT32          32


#define	ICON_BATTERY        0
#define	ICON_GPS            1 //gps
#define	ICON_433            2 //lora
#define	ICON_433_LORA      3 //lora
#define ICON_GSENSOR        4 //动静检测
#define ICON_HEARTRATE      5 //心率
#define	ICON_ERWEIMA        6 //二维码

#define LCD_MAX_X       128
#define LCD_MAX_Y       32

/*
#define STATE_MAX 0xFF
#define STATE_MIN 0x00
#define STATE_55 0x55
#define STATE_AA 0xAA
#define START_PAGE 0xB0
#define PAGE_TOTAL 8
#define START_HIGH_BIT 0x10
#define START_LOW_BIT 0x00
#define FRAME_HIGH_ROW 0x01
#define FRAME_LOW_ROW 0x80

#define MIN 0
#define COLUMN_MAX 128
#define ROW_MAX 32
//
//
#define	Slave_Address 0x78		// 器件地址
#define	OP_Command 0x00
#define	OP_Data 0x40
//
//
#define Manual 1
#define Auto 0

*/

void hw_lcd_init(void);

//以下函数APP调用前必须取得互斥信号量g_queue_lcd，否则会产生冲突
void hw_lcd_power(uint8_t state);
void hw_lcd_displayon(void);
void hw_lcd_displayon2(void);
void hw_lcd_displayoff(void);
void hw_lcd_clear_all(void);
void hw_lcd_clear_all2(void);
void hw_lcd_clear(void);
void hw_lcd_refresh(void);
void hw_lcd_showstring(uint32 x,uint32 y,uint32 font,const uint8 *p);
void hw_lcd_show_battery(uint32 x,uint32 y, uint8 level);
void hw_lcd_show_433(uint32 x, uint32 y, uint8 level);
void hw_lcd_show_433_lora(uint32 x, uint32 y, uint8 level);
void hw_lcd_show_gps(uint32 x, uint32 y, uint8 level);
void hw_lcd_show_gsensor(uint32 x, uint32 y, uint8 level);
void hw_lcd_show_heartrate(uint32 x, uint32 y, uint8 level);
void hw_lcd_refresh(void);

void lcd_drawdot(uint32 x,uint32 y,uint32 t);
void lcd_drawdot2(uint32 x,uint32 y,uint32 t);
void hw_lcd_showstring2(uint32 x,uint32 y,uint32 font,const uint8 *p);
//void lcd_writecommand(unsigned char command);
//void lcd_writedata(unsigned char data);

//void show_test(void);

/*
void Clear_Screen();
void All_Screen();
void Row_Show135();
void Row_Show246();
void Row_Show147();
void Row_Show258();
void Row_Show369();
void Column_Show135();
void Column_Show246();
//void Column_Show147();
//void Column_Show258();
//void Column_Show369();
void Frame_Show();
void Chess_Board1();
void Chess_Board2();
void Column_Scan();
void Program_Edit();
void Photo_Show();
void Photo_Show1();
//void LED_VCC_Control(uchar Control_State);
//uchar Key_Scan();
//void Key_Processor(uchar key_v);

*/


#endif

