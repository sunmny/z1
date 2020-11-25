#include "hw_font.h"
#include "hw_i2c.h"
#include "hw_lcd.h"
#include "hw_config.h"
#include "stm32l4xx_hal.h"
#include "delay.h"


//#include "Display_Mode.h"


volatile unsigned char lcd_gram[128][4];

void i2c_config(void)
{
	/*
    I2C_InitTypeDef I2C_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;


    RCC_AHB1PeriphClockCmd(LCD_SCL_GPIO_CLK, ENABLE);
    RCC_APB1PeriphClockCmd(LCD_I2C_CLK, ENABLE);
    GPIO_PinAFConfig(LCD_SCL_GPIO_PORT, LCD_SCL_GPIO_SOURCE, LCD_I2C_AF);
    GPIO_InitStructure.GPIO_Pin =  LCD_SCL_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(LCD_SCL_GPIO_PORT, &GPIO_InitStructure);

    RCC_AHB1PeriphClockCmd(LCD_SDA_GPIO_CLK, ENABLE);
    GPIO_PinAFConfig(LCD_SDA_GPIO_PORT, LCD_SDA_GPIO_SOURCE, LCD_I2C_AF);
    GPIO_InitStructure.GPIO_Pin =  LCD_SDA_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(LCD_SDA_GPIO_PORT, &GPIO_InitStructure);

    I2C_DeInit(LCD_I2C);
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitStructure.I2C_OwnAddress1 = 0x78;  //BMP085地址
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;  //ack enable
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_InitStructure.I2C_ClockSpeed = 100000;  //100k

    
    I2C_Init(LCD_I2C, &I2C_InitStructure);
		I2C_Cmd(LCD_I2C, ENABLE);
		*/
}

//------------------------------------------------------------
void lcd_writedata(unsigned char data)
{
	
	uint8_t send[2] = {0x00};
	
	 send[0] = 0x40;
	 send[1] = data;
	
	hw_i2c_write(LCD_I2C, 0x78, send, 2);
	
	
	/*
    I2C_GenerateSTART(LCD_I2C,ENABLE);
    //产生起始条件
    while(!I2C_CheckEvent(LCD_I2C, I2C_EVENT_MASTER_MODE_SELECT));
    //等待ACK
    I2C_Send7bitAddress(LCD_I2C,0x78,I2C_Direction_Transmitter);

    //向设备发送设备地址
    while(!I2C_CheckEvent(LCD_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
    //等待ACK

    I2C_SendData(LCD_I2C, 0x40);

    //发送写入的寄存器地址
    while(!I2C_CheckEvent(LCD_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    //等待ACK
    I2C_SendData(LCD_I2C, data);

    //发送数据
    while(!I2C_CheckEvent(LCD_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    //发送完成
    I2C_GenerateSTOP(LCD_I2C, ENABLE);
    //产生结束信号
*/		
		
	/*	
		IIC_Start();  
//	if(EE_TYPE>AT24C16)
//	{
		IIC_Send_Byte(0X78);	    //发送写命令
		IIC_Wait_Ack();
		IIC_Send_Byte(0x40);//发送高地址	  
//	}else IIC_Send_Byte(0XA0+((WriteAddr/256)<<1));   //发送器件地址0XA0,写数据 	 
	IIC_Wait_Ack();	   
    IIC_Send_Byte(data);   //发送低地址
	IIC_Wait_Ack(); 	 										  		   
	//IIC_Send_Byte(DataToWrite);     //发送字节							   
	//IIC_Wait_Ack();  		    	   
    IIC_Stop();//产生一个停止条件 
	delay_ms(10);
*/		
		
}
//------------------------------------------------------------
void lcd_writecommand(unsigned char command)
{
	uint8_t send[2] = {0x00};
	
	 send[0] = 0x00;
	 send[1] = command;
	
	hw_i2c_write(LCD_I2C, 0x78, send, 2);
	
	/*
    I2C_GenerateSTART(LCD_I2C,ENABLE);
    //产生起始条件
    while(!I2C_CheckEvent(LCD_I2C, I2C_EVENT_MASTER_MODE_SELECT));
    //等待ACK
    I2C_Send7bitAddress(LCD_I2C,0x78,I2C_Direction_Transmitter);
    //向设备发送设备地址
    while(!I2C_CheckEvent(LCD_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
    //等待ACK

    I2C_SendData(LCD_I2C, 0x00);
    //发送写入的寄存器地址
    while(!I2C_CheckEvent(LCD_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    //等待ACK

    I2C_SendData(LCD_I2C, command);
    //发送数据
    while(!I2C_CheckEvent(LCD_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    //发送完成

    I2C_GenerateSTOP(LCD_I2C, ENABLE);
    //产生结束信号
		*/
		
		/*
		IIC_Start();  
//	if(EE_TYPE>AT24C16)
//	{
		IIC_Send_Byte(0X78);	    //发送写命令
		IIC_Wait_Ack();
		IIC_Send_Byte(0x00);//发送高地址	  
//	}else IIC_Send_Byte(0XA0+((WriteAddr/256)<<1));   //发送器件地址0XA0,写数据 	 
	IIC_Wait_Ack();	   
    IIC_Send_Byte(command);   //发送低地址
	IIC_Wait_Ack(); 	 										  		   
	//IIC_Send_Byte(DataToWrite);     //发送字节							   
	//IIC_Wait_Ack();  		    	   
    IIC_Stop();//产生一个停止条件 
//	delay_ms(10);
*/
		
}

 // GPIO_PIN_RESET = 0,
//  GPIO_PIN_SET

void hw_lcd_power(uint8_t state)
{	
  	//上电
	 HAL_GPIO_WritePin(LCD_VCC_EN_GPIO_Port, LCD_VCC_EN_Pin,(GPIO_PinState) state);	
	 HAL_GPIO_WritePin(LCD_RESET_GPIO_Port, LCD_RESET_Pin, (GPIO_PinState)state);
}

void hw_lcd_power2(GPIO_PinState state)
{	
  	//上电
	 HAL_GPIO_WritePin(LCD_VCC_EN_GPIO_Port, LCD_VCC_EN_Pin, state);	
	 HAL_GPIO_WritePin(LCD_RESET_GPIO_Port, LCD_RESET_Pin, state);
}

void hw_lcd_reset(GPIO_PinState state)
{
   HAL_GPIO_WritePin(LCD_RESET_GPIO_Port, LCD_RESET_Pin, state);
}



void hw_lcd_init(void)
{
	/*
    GPIO_InitTypeDef  GPIO_InitStructure;

 
	
    RCC_AHB1PeriphClockCmd(LCD_PWRON_GPIO_CLK, ENABLE);
    GPIO_InitStructure.GPIO_Pin = LCD_PWRON_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(LCD_PWRON_GPIO_PORT, &GPIO_InitStructure);
    GPIO_SetBits(LCD_PWRON_GPIO_PORT, LCD_PWRON_GPIO_PIN);

    RCC_AHB1PeriphClockCmd(LCD_RST_GPIO_CLK, ENABLE);
    GPIO_InitStructure.GPIO_Pin = LCD_RST_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(LCD_RST_GPIO_PORT, &GPIO_InitStructure);
    GPIO_ResetBits(LCD_RST_GPIO_PORT, LCD_RST_GPIO_PIN);
    delay_ms(20);
    GPIO_SetBits(LCD_RST_GPIO_PORT, LCD_RST_GPIO_PIN);
    delay_ms(20);

    //初始化IIC总线
  //  i2c_config();
		*/
	//	IIC_Init();
  //  delay_ms(20);
		hw_lcd_power(GPIO_PIN_SET);
		
		delay_ms(20);
		
		//hw_lcd_reset(GPIO_PIN_SET);
		
		//delay_ms(20);
		
	  //hw_i2c_init();
		
		//delay_ms(20);

/*
//S1
    lcd_writecommand(0xAE); //Set Display Off
    lcd_writecommand(0xd5); //display divide ratio/osc. freq. mode
    lcd_writecommand(0x80); //
    lcd_writecommand(0xA8); //multiplex ration mode:63
    lcd_writecommand(0x3F);
    lcd_writecommand(0xD3); //Set Display Offset
    lcd_writecommand(0x00);
    lcd_writecommand(0x40); //Set Display Start Line
    lcd_writecommand(0xAD); //DC-DC Control Mode Set
    lcd_writecommand(0x8B); //8A:External 8b:Built-in DC-DC is used
    lcd_writecommand(0x32); //Set Pump voltage value
    lcd_writecommand(0xA1); //Segment Remap
    lcd_writecommand(0xff); //Sst COM Output Scan Direction  //C8
    lcd_writecommand(0xDA); //common pads hardware: alternative
    lcd_writecommand(0x12);
    lcd_writecommand(0x81);  //contrast control
    lcd_writecommand(0x40);
    lcd_writecommand(0xD9); //set pre-charge period
    lcd_writecommand(0x1f);
    lcd_writecommand(0xDB); //VCOM deselect level mode
    lcd_writecommand(0x40);
    lcd_writecommand(0xA4); //Set Entire Display On/Off
    lcd_writecommand(0xA6); //Set Normal Display

    lcd_clr();

    lcd_writecommand(0xAF); //Set Display On
		
		
		
	//S5			
   lcd_writecommand(0xAE);     //Set Display Off 

   lcd_writecommand(0xd5);     //display divide ratio/osc. freq. mode	
   lcd_writecommand(0xc1);     // 115HZ
 
   lcd_writecommand(0xA8);     //multiplex ration mode: 
   lcd_writecommand(0x1F);

   lcd_writecommand(0xAD);    //External or Internal VCOMH Selection	/External or internal IREF Selection
   lcd_writecommand(0x00);		// Internal VCOMH/ External	IREF

	 lcd_writecommand(0x40);	//Set Memory Addressing Mode
	 lcd_writecommand(0x02);	//Page Addressing Mode

   lcd_writecommand(0xD3);     //Set Display Offset   
   lcd_writecommand(0x00);
   
   lcd_writecommand(0x40);     //Set Display Start Line 
   
   lcd_writecommand(0x8D);     //DC-DC Control Mode Set 
   lcd_writecommand(0x14);     //DC-DC ON/OFF Mode Set 

   lcd_writecommand(0xA0);     //Segment Remap	 

   lcd_writecommand(0xC8);     //Sst COM Output Scan Direction	

   lcd_writecommand(0xDA);     //seg pads hardware: alternative	
   lcd_writecommand(0x12);

   lcd_writecommand(0x81);     //contrast control 
   lcd_writecommand(0x53);		

   lcd_writecommand(0xD9);	    //set pre-charge period	  
   lcd_writecommand(0x22);

   lcd_writecommand(0xDB);     //VCOM deselect level mode 
   lcd_writecommand(0x00);	    

   lcd_writecommand(0xA4);     //Set Entire Display On/Off	

   lcd_writecommand(0xA6);     //Set Normal Display 

   lcd_writecommand(0xAF);     //Set Display On 
		*/

		
   lcd_writecommand(0xAE);     //Set Display Off 

   lcd_writecommand(0xd5);     //display divide ratio/osc. freq. mode	
   lcd_writecommand(0xc1);     // 115HZ
 
   lcd_writecommand(0xA8);     //multiplex ration mode: 
   lcd_writecommand(0x1F);

   lcd_writecommand(0xAD);    //External or Internal VCOMH Selection	/External or internal IREF Selection
   lcd_writecommand(0x00);		// Internal VCOMH/ External	IREF

	 lcd_writecommand(0x40);	//Set Memory Addressing Mode
	 lcd_writecommand(0x02);	//Page Addressing Mode

   lcd_writecommand(0xD3);     //Set Display Offset   
   lcd_writecommand(0x00);
   
   lcd_writecommand(0x40);     //Set Display Start Line 
   
   lcd_writecommand(0x8D);     //DC-DC Control Mode Set 
   lcd_writecommand(0x14);     //DC-DC ON/OFF Mode Set 

   lcd_writecommand(0xA0);     //Segment Remap	 

   lcd_writecommand(0xC8);     //Sst COM Output Scan Direction	

   lcd_writecommand(0xDA);     //seg pads hardware: alternative	
   lcd_writecommand(0x12);

   lcd_writecommand(0x81);     //contrast control 
   lcd_writecommand(0x53);		

   lcd_writecommand(0xD9);	    //set pre-charge period	  
   lcd_writecommand(0x22);

   lcd_writecommand(0xDB);     //VCOM deselect level mode 
   lcd_writecommand(0x00);	    

   lcd_writecommand(0xA4);     //Set Entire Display On/Off	

   lcd_writecommand(0xA6);     //Set Normal Display 

   lcd_writecommand(0xAF);     //Set Display On 
	 
	 
	/* 
while(1)
{
//	lcd_clr();
	
  lcd_showstring(0,0,12,"123456789");
  lcd_refresh();
	
	delay_ms(500);
	

}	
*/

	//show_test();
/*	
	//显示
while(1)
{
	hw_lcd_clr();
hw_lcd_show_battery(128-19,0, 0);
hw_lcd_show_433(128-19-13-4, 0, 0);
	hw_lcd_show_433_lora(128-19-13-4-19-1,0,0);
hw_lcd_show_gps(3, 0, 0);
hw_lcd_show_gsensor(128-11, 32-15, 0);
hw_lcd_show_heartrate(3, 32-7, 0);
hw_lcd_showstring(32,8,16,"09:09:57");
	hw_lcd_showstring(39,32-8,8,"2017/07/20");
hw_lcd_refresh();
delay_ms(1000);
	
	
hw_lcd_clr();
hw_lcd_show_battery(128-19,0, 0);
hw_lcd_showstring(128-19+6,1,8, "96");
//hw_lcd_showstring(70,5,12,"48");
hw_lcd_show_433(128-19-13-4, 0, 1);
//	hw_lcd_showstring(60,0,12,"lora");
hw_lcd_show_433_lora(128-19-13-4-19-1,0,0);
hw_lcd_show_gps(3, 0, 1);
hw_lcd_showstring(3+9,3,8,"59");
hw_lcd_show_gsensor(128-11, 32-15,0);
hw_lcd_show_heartrate(3, 32-7, 1);
hw_lcd_showstring(32,8,16,"09:09:57");
//hw_lcd_showstring(70,5,12,"48");

hw_lcd_showstring(39,32-8,8,"2017/07/20");
hw_lcd_refresh();
delay_ms(1000);
}
*/

}


//更新显存到LCD
void hw_lcd_refresh(void)
{
    unsigned int i, j;

    for(i=0; i<4; i++)
    {
        //lcd_writecommand (0xb7-i);    //设置页地址（0~7）
        //lcd_writecommand (0x00);      //设置显示位置—列低地址
        //lcd_writecommand (0x10);      //设置显示位置—列高地址
       // lcd_writedata(0);
       // lcd_writedata(0);
			
			  lcd_writecommand (0xb3-i);    //设置页地址（0~7）
        lcd_writecommand (0x10);      //设置显示位置—列低地址
        lcd_writecommand (0x00);      //设置显示位置—列高地址
			

        for(j=0; j<128; j++)
        {
					 // delay_ms(5);
            lcd_writedata(lcd_gram[127-j][i]);
        }
    }
}

//开启OLED显示
void hw_lcd_displayon(void)
{
   // lcd_writecommand(0X8D);  //SET DCDC命令
   // lcd_writecommand(0X14);  //DCDC ON
   // lcd_writecommand(0XAF);  //DISPLAY ON
}

void hw_lcd_displayon2(void)
{
    lcd_writecommand(0X8D);  //SET DCDC命令
    lcd_writecommand(0X14);  //DCDC ON
    lcd_writecommand(0XAF);  //DISPLAY ON
}
//关闭OLED显示
void hw_lcd_displayoff(void)
{
    lcd_writecommand(0X8D);  //SET DCDC命令
    lcd_writecommand(0X10);  //DCDC OFF
    lcd_writecommand(0XAE);  //DISPLAY OFF
}


//清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!
void hw_lcd_clear_all(void)
{
    unsigned int i, j;

    for(i=0; i<LCD_MAX_Y/8; i++)
    {
        for(j=0; j<LCD_MAX_X; j++)
            lcd_gram[j][i] = 0;
    }
    //hw_lcd_refresh();
}
void hw_lcd_clear_all2(void)
{
    unsigned int i, j;

    for(i=0; i<LCD_MAX_Y/8; i++)
    {
        for(j=0; j<LCD_MAX_X; j++)
            lcd_gram[j][i] = 0xFF;
    }
    //hw_lcd_refresh();
}
/*
void hw_lcd_clear_all(void)
{
    unsigned int i, j;

    for(i=0; i<LCD_MAX_Y/8; i++)
    {
        for(j=0; j<LCD_MAX_X; j++)
            lcd_gram[j][i] = 0;
    }
    //hw_lcd_refresh();
}
*/

//画点
//x:0~127
//y:0~63
//t:1 填充 0,清空
void lcd_drawdot(uint32 x,uint32 y,uint32 t)
{
    uint32 data;

    data = 1 << (7 - (y - ((y >> 3) << 3)));
    if (t)
        lcd_gram[x][y>>3] |= data;
    else
        lcd_gram[x][y>>3] &= ~data;
}

void lcd_drawdot2(uint32 x,uint32 y,uint32 t)
{
    uint32 data;

    data = 1 << (7 - (y - ((y >> 3) << 3)));
    if (t)
        lcd_gram[x][y>>3] |= ~data;
    else
        lcd_gram[x][y>>3] &= data;
}


int lcd_getGBKoffset(const uint8 *p)
{
    int i;
    uint32 offset;

    offset = (((*p)<<8)|(*(p+1)));

    for( i=0; i<=69; i++)
    {
        if(offset == Oled_GBK_HZ[i])
            return i;
    }
    return 0;
}

void lcd_showchar(uint32 x,uint32 y,const uint8 *p,uint32 font,uint32 mode)
{
    uint32 i, j;
    uint32 temp, offset, count;
    uint32 xsize,ysize;

    if((*p) >= 0x80)
        offset = lcd_getGBKoffset(p);
    else
        offset = (*p)-' ';

    switch(font)
    {
			 case FONT_CUSTOM :
        if((*p) >= 0x80)
        {
            xsize = 0;
            p = Oled_Ascii_0608[offset];
        }
        else
        {
            xsize = 5;
            p = Oled_Ascii_0608[offset];
        }
        ysize = 8;
        break;
			
    case FONT12 :
        if((*p) >= 0x80)
        {
            xsize = 24;
            p = Oled_1212_HZ[offset];
        }
        else
        {
            xsize = 12;
            p = Oled_Ascii_0612[offset];
        }
        ysize = 12;
        break;
    case FONT16 :
        if((*p) >= 0x80)
        {
            xsize = 32;
            p = Oled_1616_HZ[offset];
        }
        else
        {
            xsize = 16;
            p = Oled_Ascii_0816[offset];
        }
        ysize = 16;
        break;
    case FONT24 :
        if((*p) >= 0x80)
        {
            xsize = 72;
            p = Oled_2424_HZ[offset];
        }
        else
        {
            xsize = 36;
            p = Oled_Ascii_1224[offset];
        }
        ysize = 24;
        break;
    /*
    case FONT32 :
    if((*p) >= 0x80)
    {
    xsize = 128;
    p = Oled_3232_HZ[offset];
    }
    else
    {
    xsize = 64;
    p = Oled_Ascii_1632[offset];
    }
    ysize = 32;
    break;
    */
    default :
        return;
    }

    count = 0;
    for (i=0; i<xsize; i++)
    {
        temp = *p++;
        for (j=0; j<8; j++)
        {
            if (temp & 0x80)
                lcd_drawdot(x, y+count, mode);
            else
                lcd_drawdot(x, y+count, !mode);
            temp <<= 1;
            count++;
            if(count == ysize)
            {
                count = 0;
                x++;
                break;
            }
        }
    }
}

//显示字符串
//x,y:起点坐标
//*p:字符串起始地址
//用16字体
void hw_lcd_showstring(uint32 x,uint32 y,uint32 font,const uint8 *p)
{
    uint8 xsize,ysize;

    switch(font)
    {
		case FONT_CUSTOM:
        xsize = 5;
        ysize = 8;
        break;
    case FONT12 :
        xsize = FONT12/2;
        ysize = FONT12;
        break;
    case FONT16 :
        xsize = FONT16/2;
        ysize = FONT16;
        break;
    case FONT24 :
        xsize = FONT24/2;
        ysize = FONT24;
        break;
    /*
    case FONT32 :
    xsize = 16;
    ysize = 32;
    break;
    */
    default :
        return;
    }
    while(*p!='\0')
    {
        if(x >= LCD_MAX_X)
        {
            x = 0;
            y += ysize;
        }
        if(y > LCD_MAX_Y)
        {
            x = 0;
            y = 0;
        }
        lcd_showchar(x, y, p, font, 1);

        if((*p) >= 0x80)
        {
            x += xsize*2;
            p += 2;
        }
        else
        {
            x += xsize;
            p++;
        }
    }
}


void hw_lcd_showstring2(uint32 x,uint32 y,uint32 font,const uint8 *p)
{
    uint8 xsize,ysize;

    switch(font)
    {
		case FONT_CUSTOM:
        xsize = 5;
        ysize = 8;
        break;
    case FONT12 :
        xsize = FONT12/2;
        ysize = FONT12;
        break;
    case FONT16 :
        xsize = FONT16/2;
        ysize = FONT16;
        break;
    case FONT24 :
        xsize = FONT24/2;
        ysize = FONT24;
        break;
    /*
    case FONT32 :
    xsize = 16;
    ysize = 32;
    break;
    */
    default :
        return;
    }
    while(*p!='\0')
    {
        if(x >= LCD_MAX_X)
        {
            x = 0;
            y += ysize;
        }
        if(y > LCD_MAX_Y)
        {
            x = 0;
            y = 0;
        }
        lcd_showchar(x, y, p, font, 0);

        if((*p) >= 0x80)
        {
            x += xsize*2;
            p += 2;
        }
        else
        {
            x += xsize;
            p++;
        }
    }
}


void lcd_showicon(uint32 x, uint32 y, uint32 level, uint8 icon, uint8 mode)
{
    uint32 i, j, count, len, temp,hight;
    const uint8 *p;
/*
	#define	ICON_BATTERY        0
#define	ICON_GPS            1 //gps
#define	ICON_433            2 //lora
#define ICON_GSENSOR        3 //动静检测
#define ICON_HEARTRATE      4 //心率
#define	ICON_ERWEIMA        5 //二维码
	*/
    switch(icon)
    {
		case ICON_BATTERY :
        p = Font_Battery[level];
        len =38;
		    hight = 11;
        break;
    case ICON_GPS :
        p = Font_GPS[level];
        len = 14;
		    hight = 11;
        break;
    case ICON_433 :
        p = Font_433[level];
        len = 26;
		    hight = 9;
        break;
		case ICON_433_LORA :
        p = Font_433_Lora[level];
        len = 20;
		    hight = 7;
        break;
	  case ICON_GSENSOR :
        p = Font_Gsensor[level];
        len = 22;
		    hight = 15;
        break;

		case ICON_HEARTRATE:       //心率:
			  p = Font_Heartrate[level];
        len =7;
		    hight = 7;
        break;
		case ICON_ERWEIMA:
			//	p = Battery_Font[0];
       // len =128;
        break;
    default:
        return;
    }

    count = 0;
    for(i=0; i<len; i++)
    {
        temp = *p++;

        for(j=0; j<8; j++)
        {
            if(temp & 0x80)
                lcd_drawdot(x, y+count, mode);
            else
                lcd_drawdot(x, y+count, !mode);
            temp<<=1;
            count++;
            if(count == hight)
            {
                count = 0;
                x++;
                break;
            }
        }
    }
}



/*

#define	ICON_BATTERY        0
#define	ICON_GPS            1 //gps
#define	ICON_433            2 //lora
#define ICON_GSENSOR        3 //动静检测
#define ICON_HEARTRATE      4 //心率
#define	ICON_ERWEIMA        5 //二维码

*/

void hw_lcd_show_battery(uint32 x,uint32 y, uint8 level)
{
    if(level <= 5)
        lcd_showicon(x, y, level, ICON_BATTERY, 1);
}

void hw_lcd_show_433(uint32 x, uint32 y, uint8 level)
{
    if(level <= 3)
        lcd_showicon(x, y, level, ICON_433, 1);
}

void hw_lcd_show_433_lora(uint32 x, uint32 y, uint8 level)
{
    if(level <= 3)
        lcd_showicon(x, y, level, ICON_433_LORA, 1);
}

void hw_lcd_show_gps(uint32 x, uint32 y, uint8 level)
{
    if(level <= 1)
        lcd_showicon(x, y, level, ICON_GPS, 1);
}

void hw_lcd_show_gsensor(uint32 x, uint32 y, uint8 level)
{
    if(level <= 1)
        lcd_showicon(x, y, level, ICON_GSENSOR, 1);
}

void hw_lcd_show_heartrate(uint32 x, uint32 y, uint8 level)
{
    if(level <= 1)
        lcd_showicon(x, y, level, ICON_HEARTRATE, 1);
}

////////////////////////////////////////////////////
/*
void show_test(void)
{
  // Reset_IC();                    //复位IC
 //  Init_IC();                     //初始化IC
   All_Screen();                       //全屏显示效果
   while(1)
   {
     Program_Edit();  
		 delay_ms(2000);
		 
		 Photo_Show();
	   delay_ms(5000);
		  Photo_Show1();
	   delay_ms(5000);
   }
 }
*/

