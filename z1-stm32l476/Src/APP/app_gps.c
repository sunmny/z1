#include "app_gps.h"
#include "hw_gps.h"
#include <string.h>
#include "app_meerwespe.h"
#include "hw_rtc.h"
#include "app_lora.h"
#include "app_lcd.h"

extern WORK_SEND_MODE  work_send_mode;



extern uint8_t send_gps_date[6];
extern uint8_t send_gps_time[6];
extern uint8_t send_gps_jingdu[12];
extern uint8_t send_gps_weidu[12];

extern uint8_t check_gps_time;


extern uint8_t first_flag;

uint8_t gps_data[1000] = {0x00};
//测试用
//定位成功
uint8_t test_dingwei_ok[1000]="GNGLL,,,,,092243.00,V$GNRMC,094834.00,A,3110.17720,N,12123.62252,E,4.220,317.43,010817,,,A*79$GNVTG,317.43,T,,M,4.220,N,7.815,K,A*2E$GNGGA,094834.00,3110.17720,N,12123.62252,E,1,04,4.36,60.1,M,9.7,M,,*4E$GNGSA,A,3,07,19,06,13,,,,,,,,,5.27,4.36,2.96*1BGNGSA,A,3,,,,,,,,,,,,,5.27,4.36,2.96*10$GPGSV,4,1,14,02,66,001,,05,57,288,,06,43,090,24,07,09,082,13*77$GNGLL,3110.17720,N,12123.62252,E,094834.00,A,A*77";
//定位不成功，但是有时间日期
uint8_t test_dingwei_no1[1000]="$GNGLL,,,,,092243.00,V,N*5A$GNRMC,092244.00,V,,,,,,,010817,,,N*65$GNVTG,,,,,,,,,N*2E$GNGGA,092244.00,,,,,0,00,99.99,,,,,,*71$GNGSA,A,1,,,,,,,,,,,,,99.99,99.99,99.99*2E$GNGSA,A,1,,,,,,,,,,,,,99.99,99.99,99.99*2E$GPGSV,1,1,03,07,,,30,17,,,35,19,,,37*72$GLGSV,1,1,01,81,,,41*68$GNGLL,,,,,092244.00,V,N*5D";
//定位不成功，但是有时间没日期
uint8_t test_dingwei_no2[1000]="$GNGLL,,,,,092108.97,V,N*58$GNTXT,01,01,01,More than 100 frame errors, UART RX was disabled*70$GNRMC,092110.00,V,,,,,,,,,,N*68$GNVTG,,,,,,,,,N*2E$GNGGA,092110.00,,,,,0,00,99.99,,,,,,*73$GNGSA,A,1,,,,,,,,,,,,,99.99,99.99,99.99*2E$GNGSA,A,1,,,,,,,,,,,,,99.99,99.99,99.99*2E$GPGSV,1,1,01,17,,,30*7D$GLGSV,1,1,01,,,,38*6F$GNGLL,,,,,092110.00,V,N*5F$GNRMC,092111.00,V,,,,,,,,,,N*69$GNVTG,,,,,,,,,N*2E";
//定位不成功，时间日期都没有
uint8_t test_dingwei_no3[1000]="$GNGSA,A,1,,,,,,,,,,,,,99.99,99.99,99.99*2E$GPGSV,1,1,00*79$GLGSV,1,1,00*65$GNGLL,,,,,,V,N*7A$GNRMC,,V,,,,,,,,,,N*4D$GNVTG,,,,,,,,,N*2E$GNGGA,,,,,,0,00,99.99,,,,,,*56$GNGSA,A,1,,,,,,,,,,,,,99.99,99.99,99.99*2E$GNGSA,A,1,,,,,,,,,,,,,99.99,99.99,99.99*2E$GPGSV,1,1,00*79$GLGSV,1,1,00*65";


unsigned char  gps_nub[4]={0x30,0x30,0x00,0x00};
uint8_t        gps_dingwei_state; 

//static uint8_t gnrmc[100]={0x00};
//static uint8_t gngga[100]={0x00};

 char GNGGA[6] = "$GNGGA";
 //char GNRMC[6] = "$GNRMC" ;
char GNRMC[1] = "$" ;
 char douhao = ',';



uint8_t dingwei_ok=0;


void app_gps_clean_data(void)
{
	
			app_lcd_mutex_enter();

		
 		gps_nub[0]=gps_nub[1]=0x30;
		gps_nub[2]='\0';
		
		app_lcd_mutex_exit();
    gps_dingwei_state  = 0; 
}


/*
//设置蜂鸣器响声次数
void APP_set_buzzer_nub(uint8 nub,uint32 ms)
{
	while(nub--)
		{  
  	    Buzzer_Set(1);
        vTaskDelay(ms);
			 //delay_ms(ms);
        Buzzer_Set(0);
			if(nub!=0)
        vTaskDelay(ms);
      // delay_ms(ms);			
    }  
}
*/

void app_gps_open_bd(void)
{
 
}


uint8_t app_gps_receive(void)
{
	uint16_t rec_len = 1000;
//	uint8_t kong = 0x00;
//	hw_gps_send(&kong,1);
//memcpy(gps_data,"0",rec_len);
	
return	hw_gps_receive(gps_data,&rec_len,1500);
	
}


//解析gnrmc 获取时间，日期
uint8_t app_gps_gnrmc_analysis(char *date,uint8_t dingwei_falg)
{
  char *p_date=NULL;//,*p_time=NULL;
	char get_date[6]={0x00};//ddjjyy
	char get_time[6]={0x00};//hhmmss
	p_date=strchr2(date,douhao,9);
	if(((*(p_date+1))!=douhao)&&((*(p_date+2))!=douhao))
	{
	  //已经获取了日期，获取时间
	  memcpy(get_date,p_date+1,6);
		memcpy(get_time,date+7,6);
		
		//设置系统rtc
		//while(hw_rtc_set_date_time((uint8_t *)get_time,(uint8_t *)get_date));
		hw_rtc_set_date_time((uint8_t *)get_time,(uint8_t *)get_date);
		///if(first_flag == 0)
		//{
		//	return 0;
		//first_flag = 1;
		//}
		//上传用
		if(dingwei_falg == 1)
		{
					 app_lora_mutex_enter();
              		  memcpy(send_gps_date,p_date+1,6);
	  	  memcpy(send_gps_time,date+8,6);

		app_lora_mutex_exit();

		}
		return 0;
	}else{
	return 1;
	}

}

//解析gnrmc 获取时间，日期
uint8_t app_gps_gngga_analysis(char *date)
{
  char *p_jingdu=NULL,*p_weidu=NULL,*p_dingwei=NULL,*p_nub = NULL;
//	char get_jingdu[10]={0x00};//ddjjyy
//	char get_weidu[10]={0x00};//hhmmss
//	char get_nub=0x00;//hhmmss
//	char get_haiba[3]={0x00};//hhmmss
	char dingwei_flag = 0;
	
	
	p_dingwei=strchr2(date,douhao,6);
	
	dingwei_flag = *(p_dingwei+1);
	
	if(dingwei_flag==0x30)
	{
	  return 1;//未定位
	}else
	{
 //已经定位
		p_nub=strchr2(date,douhao,7);
		app_lcd_mutex_enter();


    memcpy(gps_nub,p_nub+1,2);
		gps_nub[2]='\0';
		
		app_lcd_mutex_exit();
    gps_dingwei_state = 1; 
		
	  p_jingdu=strchr2(date,douhao,4);
		p_weidu=strchr2(date,douhao,2);
		
							 app_lora_mutex_enter();
		 memcpy(send_gps_jingdu,p_jingdu+1,11);
	   memcpy(send_gps_weidu,p_weidu+1,11);
		app_lora_mutex_exit();

		
		
		 //app_gps_gnrmc_analysis(gps_data,1);
		 return 0;
	}

}

uint8_t app_gps_data_analysis(void)
{
	
	uint8_t *p_gnrmc = NULL,*p_gngga = NULL;
	uint8_t ret =0;
	//gnrmc  用于设置rtc
	
	if(check_gps_time == 1)
	{
	check_gps_time = 0;
	//寻找时间
  p_gnrmc = (uint8_t *)my_strstr((char *)gps_data,"$GNRMC");
	//p_gnrmc = (uint8_t *)my_strstr((char *)test_dingwei_ok,GNRMC);
	//p_gnrmc=strchr2(gps_data,douhao,1);
 if(p_gnrmc != NULL)
 {
  //解gnrmc
	ret = app_gps_gnrmc_analysis((char *)p_gnrmc,1);

  //gngga
  //获取 定位状态,经纬度，海拔高度
	if(ret == 0)
	{
				if(first_flag == 0)
		{
			//first_flag = 1;
			//return 0;

		}
	}
}
 
}
		
	  p_gngga = (uint8_t *)my_strstr((char *)gps_data,"$GNGGA");
		
		if(p_gngga != NULL)
		{
		
	  ret = app_gps_gngga_analysis((char *)p_gngga);
		//if(ret == 0)
		//{
		 //发送  //定位成功
     //osMessagePut (WorkSendQueueHandle, lora_send, osWaitForever);
		 
		//}else{
		 //return 0;
		//}
		return ret;
	}
	return 1;

}


uint8_t app_gps_location(void)
{
//   if()
	uint8_t ret = 0;
  ret = app_gps_receive();//接收1000个字节
	//memcpy(gps_data,test_dingwei_ok,1000);
	//memcpy(gps_data,test_dingwei_no1,1000);
	//memcpy(gps_data,test_dingwei_no2,1000);
	//memcpy(gps_data,test_dingwei_no3,1000);
	
	if(ret == 0)
	{
	 //接收成功，开始解析
	 return app_gps_data_analysis();
		
	}else{
		
	 return 1;
		
	}		

}



