#include "app_lora.h"
#include "app_meerwespe.h"
#include "hw_lora.h"
#include "cmsis_os.h"
#include "string.h"
#include "delay.h"

extern uint8_t gps_dingwei_state;
extern WORK_SEND_MODE  work_send_mode;
 uint8_t send_battery_level = 0;;

uint8_t send_gps_date[6]={0x00};
uint8_t send_gps_time[6]={0x00};
uint8_t send_gps_jingdu[12]={0x00};
uint8_t send_gps_weidu[12]={0x00};

//static uint8_t aaaa5[5] = {0x00,0x30,0x4e,0x0d,0x0a};
//uint8_t aaaa[17] = {0x00,0x1b,0x13,0x19,0x05,0xb9,0x46,0xb6,0x65,0xda,0x00,0x00,0x28,0xf0,0x4d,0x0d,0x0a};
//uint8_t aaaa2[17] = {0x00,0x1b,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x47,0x0d,0x0a};
//static uint8_t aaaa3[17] = {0x00,0x0b,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x47,0x0d,0x0a};
//static uint8_t lora_set_id[10]="AT+ID=40\r\n";
static uint8_t lora_get_id[8]="AT+ID?\r\n";
static uint8_t lora_open_net[10]="AT+NET=0\r\n";
static uint8_t lora_get_net[8]="AT+NS?\r\n";
static uint8_t lora_get_send_state[8]="AT+TS?\r\n";
static uint8_t lora_set_data_start[8]="AT+TX=0,";
//static uint8_t huanhang[2]="\r\n";
	
static uint8_t lora_atz[5] = "ATZ\r\n";
static uint8_t lora_enter_low_power[10] = "AT+ALP=1\r\n";
static uint8_t lora_exit_low_power[10] = "AT+ALP=0\r\n";

uint8_t test_20db[23]="AT+TTST=1D396260+0C+14+";
	
//static uint8_t ack_ok[6] = "\r\nOK\r\n";
//static uint8_t ack_error0[8] = "\r\nER00\r\n";
//static uint8_t ack_error1[8] = "\r\nER01\r\n";	
//static uint8_t ack_error2[8] = "\r\nER02\r\n";	
static uint8_t dian= '.';
	

	extern osMutexId loraMutexHandle;
	
	
	
	
extern GPS_MODE        gps_state; 
	
	static uint16_t send_flag = 0;
	

uint8_t lora_ack_ok[6]="\r\nOK\r\n";	
//--------------------------------------------------------------------------------------------------------------------------------
//uint8_t aa[30]={0x01,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x51,0x11,0x08,0x02,0x0a,0x1b,0x1e,0x02,0x00,0x01,0x0d,0x0a};
uint8_t lora_send_cmd[10]="AT+TX=0,\r\n";
//static uint8_t send[38]={0x00};
//static uint8_t ee[8]="AT+TX=0,";

static uint8_t send_data[50]={0x00};

void toInt(char *str,uint32_t *number)
{
    int i;
    *number = 0;
    for (i = 0; str[i] != 0; i++)
        *number = (int)(str[i] - 48) + *number * 10;
}


void app_lora_mutex_enter()
{
   osMutexWait( loraMutexHandle, osWaitForever );
}
	
void app_lora_mutex_exit()
{
   osMutexRelease( loraMutexHandle );
}


uint16_t  app_lora_get_senddata(uint8_t *senddata,uint8_t send_type)
{
//  	uint8_t i=0;
	uint8_t  send_len = 0;
	uint32_t jingdu =0, weidu =0;
	char *p_jingdu=NULL,*p_weidu=NULL,*p_nub = NULL;//*p_dingwei=NULL,
	
	
	//清空发送缓存区
	memcpy((char *)send_data,"0x00",50);
	memcpy((char *)send_data,lora_set_data_start,8);
	//memcpy((char *)send_data,test_20db,22);
	
	send_flag = 0;
	send_len = 8+2;
	//send_len = 22;
	
	//组包
	//定位成功与否
	if(gps_dingwei_state == 1)
	{
	   send_flag = send_flag | 0x03;//置位经纬度
		//
		 app_lora_mutex_enter();

	  p_jingdu=strchr2((char *)send_gps_jingdu,dian,1);
		p_weidu=strchr2((char *)send_gps_weidu,dian,1);
		app_lora_mutex_exit();
		 memcpy(p_jingdu,p_jingdu+1,5);
	   memcpy(p_weidu,p_weidu+1,5);
		*(p_jingdu+5) = 0x00;
		*(p_weidu+5) = 0x00;
		
		//转换
		app_lora_mutex_enter();
		toInt((char *)send_gps_jingdu,&jingdu);
		toInt((char *)send_gps_weidu,&weidu);
		app_lora_mutex_exit();
		send_data[send_len++] = (weidu >>24) & 0xff;
		send_data[send_len++] = (weidu >>16) & 0xff;
		send_data[send_len++] = (weidu >>8) & 0xff;
		send_data[send_len++] = (weidu >>0) & 0xff;
		
		send_data[send_len++] = (jingdu >>24) & 0xff;
		send_data[send_len++] = (jingdu >>16) & 0xff;
		send_data[send_len++] = (jingdu >>8) & 0xff;
		send_data[send_len++] = (jingdu >>0) & 0xff;
		
		//send_len = send_len+8;
	}
	
	//电池电量
	send_flag = send_flag | 0x10;
	send_data[send_len] = 	send_battery_level;
	//send_len = send_len+1;
	send_len++;
	//类型
	//if(work_send_mode == work_send_mode)
	if((send_type == sos_send)||(send_type == call_send))
	{
	  //
		send_flag = send_flag | 0x20;
	}else if(send_type == huxi_send)
	{
	  // send_flag = send_flag | 0x00;	  
	}
	
	send_data[8] = (send_flag >>8)&0xff;
	send_data[9] = (send_flag >>0)&0xff;
	
	send_data[send_len++] = '\r';
	send_data[send_len++] = '\n';
	
	
	return send_len;
	
}

uint8_t app_lora_rec_analysis(uint8_t *rec)
{
	
//	uint8_t  *p = NULL,*p1=NULL,*p2 = NULL;
//		uint32_t tt=0;
//	uint8_t ret = 0;

	if(my_strstr((char *)rec,"OK") != NULL)
	{
	   return OK;
	}
	else if(my_strstr((char *)rec,"ER01")!= NULL)
	{
	   return ER01;
	}
		else if(my_strstr((char *)rec,"ER02")!= NULL)
	{
	   return ER02;
	}
		else if(my_strstr((char *)rec,"ER03")!= NULL)
	{
	   return ER03;
	}
		else if(my_strstr((char *)rec,"ER04")!= NULL)
	{
	   return ER04;
	}
		else if(my_strstr((char *)rec,"ER05")!= NULL)
	{
	   return ER05;
	}
			else if(my_strstr((char *)rec,"ER06")!= NULL)
	{
	   return ER06;
	}
			else if(my_strstr((char *)rec,"ER06")!= NULL)
	{
	   return ER07;
	}
	else
	{
	
	  return 0xff;
	}
	

}




uint8_t app_lora_send(uint8_t send_type)
{
	  uint32_t i = 0,j=0;
   	uint8_t ret = 0;
  	uint8_t rec[10] = {0x00};
		uint8_t rec1[10] = {0x00};
		uint16_t rec_len = 0;
	  uint8_t send_data[100] = {0x00};
  	uint16_t send_len = 0;

/*		
//static uint8_t lora_open_net[10]="AT+NET=0\r\n";
//static uint8_t lora_get_net[8]="AT+NS?\r\n";
//static uint8_t lora_set_data_start[8]="AT+TX=0,"
		
		
//组包		
send_len =  app_lora_get_senddata(send_data,send_type);
	


		 	memcpy(rec,rec1,10);
		  hw_lora_send(lora_atz, 5);
		  osDelay(100);
		 if(hw_lora_receive(rec,&rec_len,2000) == 0)//接收成功
		 {
			     ret = app_lora_rec_analysis(rec);
			    if(ret != OK)
		        return ret;		
		 }else
		 {
		       return ERREC;
		 }

//唤醒		 
      osDelay(500);
			memcpy(rec,rec1,10);
		  hw_lora_send(lora_exit_low_power, 10);
		  osDelay(100);
		 if(hw_lora_receive(rec,&rec_len,2000) == 0)//接收成功
		 {
			     ret = app_lora_rec_analysis(rec);
			    if(ret != OK)
		        return ret;		
		 }else
		 {
		       return ERREC;
		 }

		 
 //打开网络 ,尝试3次
	i = 0;	 
	while(1)
	{
		  osDelay(500);
			memcpy(rec,rec1,10);
		  hw_lora_send(lora_open_net, 10);
		  osDelay(100);
		 if(hw_lora_receive(rec,&rec_len,2000) == 0)//接收成功
		 {
			     ret = app_lora_rec_analysis(rec);
			    	
		 }else
		 {
		       ret = ERREC;
		 }
		 
		 i++;
		 if(ret == OK)
		 {
		     break;   
		 }else
		 {
		    if(i>=3)
				return ret; 
		 }

	 }
		 

//查询网络状态，5次
	i = 0;	 
	while(1)
	{
		  osDelay(500);
			memcpy(rec,rec1,10);
		  hw_lora_send(lora_get_net, 8);
		  osDelay(100);
		 if(hw_lora_receive(rec,&rec_len,2000) == 0)//接收成功
		 {
			     ret = app_lora_rec_analysis(rec);
			    	
		 }else
		 {
		       ret = ERREC;
		 }
		 
		 i++;
		 if(ret == OK)
		 {
		     break;   
		 }else
		 {
			 osDelay(2000);
		    if(i>=3)
				return ret; 
		 }

	 }


//发送数据，
//查看报警状态，取最新	 
	i = 0;	 
	while(1)
	{
		  osDelay(500);
			memcpy(rec,rec1,10);
		  hw_lora_send(send_data, send_len);
		  osDelay(100);
		 if(hw_lora_receive(rec,&rec_len,2000) == 0)//接收成功
		 {
			     ret = app_lora_rec_analysis(rec);
			    	
		 }else
		 {
		       ret = ERREC;
		 }
		 
		 i++;
		 if(ret == OK)
		 {
		     break;   
		 }else
		 {
			 osDelay(2000);
		    if(i>=5)
				return ret; 
		 }

	 }	 

	 
	 
//查询发送成功与否，发送不成功，则缓存
	 	i = 0;	 
	while(1)
	{
		  osDelay(500);
			memcpy(rec,rec1,10);
		  hw_lora_send(lora_get_send_state, 8);
		  osDelay(100);
		 if(hw_lora_receive(rec,&rec_len,2000) == 0)//接收成功
		 {
			     ret = app_lora_rec_analysis(rec);
			    	
		 }else
		 {
		       ret = ERREC;
		 }
		 
		 i++;
		 if(ret == OK)
		 {
		     break;   
		 }else
		 {
			 osDelay(2000);
		    if(i>=3)
				{
					//失败，缓存数据
				  break; 
				}
		 }

	}
	 
		 
//设置睡眠	 
		  osDelay(500);
		 	memcpy(rec,rec1,10);
		  hw_lora_send(lora_enter_low_power, 10);
		  osDelay(100);
		 if(hw_lora_receive(rec,&rec_len,2000) == 0)//接收成功
		 {
			  //   ret = app_lora_rec_analysis(rec);
			  //  if(ret != OK)
		    //    return ret;		
		 }else
		 {
		       return ERREC;
		 }
		 
*/
    return ret;
}


uint8_t app_lora_get_sn(uint8_t *sn_raw,uint8_t *sn_len)
{

   	uint8_t ret = 0;
  	uint8_t rec[20] = {0x00};
		uint8_t rec1[20] = {0x00};
		uint16_t rec_len = 0;
//	  uint8_t send_data[30] = {0x00};
//  	uint16_t send_len = 0;
		uint8_t  *p = NULL,*p1=NULL;
	/*	
		 	memcpy(rec,rec1,20);
		  hw_lora_send(lora_atz, 5);
		  osDelay(100);
		 // delay_ms(200);
		 if(hw_lora_receive(rec,&rec_len,2000) == 0)//接收成功
		 {
			     ret = app_lora_rec_analysis(rec);
			    if(ret != OK)
		        return ret;		
		 }else
		 {
		       return ERREC;
		 }

//唤醒		 
      osDelay(500);
			memcpy(rec,rec1,20);
		  hw_lora_send(lora_exit_low_power, 10);
		  osDelay(100);
		 if(hw_lora_receive(rec,&rec_len,2000) == 0)//接收成功
		 {
			     ret = app_lora_rec_analysis(rec);
			    if(ret != OK)
		        return ret;		
		 }else
		 {
		       return ERREC;
		 }
		 
		 
		 
		 //获取id 
		  osDelay(500);
		 	memcpy(rec,rec1,20);
		  hw_lora_send(lora_get_id, 8);
		  osDelay(100);
		 if(hw_lora_receive(rec,&rec_len,2000) == 0)//接收成功
		 {
			     //ret = app_lora_rec_analysis(rec);
			 //解析id
			 //寻找两个\r\n直接数据

	       p = (unsigned char *)my_strstr((char *)rec,"\r\n");
			 
	      p1 = (unsigned char *)my_strstr((char *)p+2,"\r\n");	
			 
        if((p !=NULL)&&(p1 != NULL))
        {
					  *sn_len = p1-p-2;
            memcpy(sn_raw,p+2,p1-p-2);
					ret = OK;
         }	 
		 }else
		 {
		       return ERREC;
		 }


//设置睡眠	 
		  osDelay(500);
		 	memcpy(rec,rec1,10);
		  hw_lora_send(lora_enter_low_power, 10);
		  osDelay(100);
		 if(hw_lora_receive(rec,&rec_len,2000) == 0)//接收成功
		 {
			     ret = app_lora_rec_analysis(rec);
			    if(ret != OK)
		        return ret;		
		 }else
		 {
		       return ERREC;
		 }
		*/ 
		 return ret;

}
