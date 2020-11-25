#include "app_qrcode.h"
#include <stdint.h>
#include "QR_Encode.h"
#include "string.h"
#include "hw_lcd.h"  

#define QRCODE_Y 	38		//TFT二维码显示坐标y


void app_qrcode(uint8_t *qrcode_data)
{
	uint8_t i,j,k,m;
	uint16_t x,y,p,xlen;
	uint8_t qrencode_buff[12];			//存放LCD ID字符串
	EncodeData((char *)qrcode_data);
	//m_nSymbleSize = 10;
	//LCD_Fill(10,110,15,115,BLACK);
//	LCD_Fill(0,40,240,320,WHITE);//清屏
	//sprintf((char*)qrencode_buff,"size:%d",m_nSymbleSize);//将LCD ID打印到lcd_id数组。
	//LCD_ShowString(10,40,200,16,16,qrencode_buff);		//显示LCD ID	 
	if(m_nSymbleSize*2>240)	
	{
	//	LCD_ShowString(10,60,200,16,16,(u8 *)"The QR Code is too large!");//太大显示不下
		return;
	}
	for(i=0;i<10;i++)
	{
		if((m_nSymbleSize*i*2)>44)	break;
	}
	p=(i-1)*1;//点大小
	x=(220-m_nSymbleSize*p)/2;
	y=QRCODE_Y;
	//sprintf((char*)qrencode_buff,"piont:%d",p);//将LCD ID打印到lcd_id数组。
//	LCD_ShowString(10,60,200,16,16,qrencode_buff);
	for(i=0;i<m_nSymbleSize;i++)
	{
		for(j=0;j<m_nSymbleSize;j++)
		{
			//USART1_SendData(m_byModuleData[j][i]);
			if(m_byModuleData[i][j]==1)
			{
			//	LCD_Fill(x+p*i,y+p*j,x+p*(i+1)-1,y+p*(j+1)-1,BLACK);//填充黑色点
				
		//LCD_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 color)		
		xlen=x+p*(i+1)-1-x+p*i+1;	 
		for(k=y+p*j;k<=y+p*(j+1)-1;k++)
		{
		// 	LCD_SetCursor(sx,i);      				//ÉèÖÃ¹â±êÎ»ÖÃ 
		//	LCD_WriteRAM_Prepare();     			//¿ªÊ¼Ð´ÈëGRAM	  
		//	for(j=0;j<xlen;j++)LCD_WR_DATA(color);	//ÉèÖÃ¹â±êÎ»ÖÃ 	  
         lcd_drawdot(x+p*i,k,0);			
		}
 
				
				
				
			}
		}
			
	}

}






void app_qrcode2(uint8_t *qrcode_data)
{
	uint8_t i,j,k,m;
	uint16_t x,y,p,xlen;
	uint8_t qrencode_buff[12];			//存放LCD ID字符串
	EncodeData((char *)qrcode_data);
	//m_nSymbleSize = 10;
	//LCD_Fill(10,110,15,115,BLACK);
//	LCD_Fill(0,40,240,320,WHITE);//清屏
	//sprintf((char*)qrencode_buff,"size:%d",m_nSymbleSize);//将LCD ID打印到lcd_id数组。
	//LCD_ShowString(10,40,200,16,16,qrencode_buff);		//显示LCD ID	 
	if(m_nSymbleSize*2>240)	
	{
	//	LCD_ShowString(10,60,200,16,16,(u8 *)"The QR Code is too large!");//太大显示不下
		return;
	}
	for(i=0;i<10;i++)
	{
		if((m_nSymbleSize*i*2)>44)	break;
	}
	p=(i-1)*1;//点大小
	x=(240-m_nSymbleSize*p)/2;
	y=QRCODE_Y;
	//sprintf((char*)qrencode_buff,"piont:%d",p);//将LCD ID打印到lcd_id数组。
//	LCD_ShowString(10,60,200,16,16,qrencode_buff);
	for(i=0;i<m_nSymbleSize;i++)
	{
		for(j=0;j<m_nSymbleSize;j++)
		{
			//USART1_SendData(m_byModuleData[j][i]);
			if(m_byModuleData[i][j]==1)
			{
			//	LCD_Fill(x+p*i,y+p*j,x+p*(i+1)-1,y+p*(j+1)-1,BLACK);//填充黑色点
				
		//LCD_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 color)		
		xlen=x+p*(i+1)-1-x+p*i+1;	 
		for(k=y+p*j;k<=y+p*(j+1)-1;k++)
		{
		// 	LCD_SetCursor(sx,i);      				//ÉèÖÃ¹â±êÎ»ÖÃ 
		//	LCD_WriteRAM_Prepare();     			//¿ªÊ¼Ð´ÈëGRAM	  
		//	for(j=0;j<xlen;j++)LCD_WR_DATA(color);	//ÉèÖÃ¹â±êÎ»ÖÃ 	  
         lcd_drawdot(x+p*i,k,1);			
		}
 
				
				
				
			}
		}
			
	}

}