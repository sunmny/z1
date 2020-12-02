#include "hw_uart.h" 
#include "stm32l4xx_hal.h"
//#include "cmsis_os.h"
#include "string.h"
//#include "delay.h"
#include "app_aoa.h"
#include "hw_config.h"
#include "task_ble.h"
#include "zb_common_data.h"
#include "hw_bd1.h"
#include "app_bd1.h"
#include "hw_bd2.h"
#include "app_bd2.h"
#include "app_nvram.h"

#define SUPPORT_MAIL_NUM 1

#define BDEVICE_MAX_NUM 100
extern uint8_t save_nv_buf[70];


#define BDEVICE_GROUP_NUM   10

uint8_t DEVICE_ID_LEN = 12;

uint8_t bind_flag =0;
uint8_t aoa_uart_data_loop[AOA_UART_DATA_LOOP_SIZE+1];
extern osStatus task_aoauart_mail_put(uint8_t *data, uint16_t len);
uint16_t aoa_uart_data_loop_len;
//extern osTimerId ReportTimerHandle;
#define AT_END_TOK_LEN 4
static const uint8_t aoa_at_end_tok[4] = {0x23,0x23,0x0d,0x0a};//##\r\n
extern osTimerId LedTimerHandle;
uint8_t send_light_commnd = 0;
uint8_t timer_status =0;

uint8_t  get_timer_status(void)
{
	return timer_status ;
}
void set_timer_status(uint8_t status)
{
	timer_status = status;
	

}
typedef enum {
	TEST=0,
	ZSAVEB,
	TASKB,
	ZCALL,
	ZBEAT,
	ZBIND,
	ZUPDATE,
	RDSSPOWER,
	RNSSPOWER,
	AIRPLANEMODE,
	ZLOCATION,
	//BBROADCAST,
//	BCALL,
	BBIND,
	BUNBIND,
	BBEAT,
	BINFOWRITE,
	BBBEAT,
	BLIGHT,
	BINFOERASE,
	CENTERADDRESS,
	BSOSINFO,
	BSOS,
	//SELFCHECK,
	BDISTANCE,
	BLOWBATTERY,
	ZREPORTLOCTIME,
	BREPORTINFOTIME,
	//TASKOVER,
	BBOXUNLOCK,
	WARINGSTATUS,
	SUPPORT_AT_NUM
} aoa_at_request;
void set_command_type(uint8_t command);
uint8_t aoa_at_handle_zsaveb(uint8_t *data, uint16_t len, uint8_t idx);
uint8_t aoa_at_handle_zupdate(uint8_t *data, uint16_t len, uint8_t idx);
uint8_t aoa_at_handle_zbind(uint8_t *data, uint16_t len, uint8_t idx);
uint8_t aoa_at_handle_zcall(uint8_t *data, uint16_t len, uint8_t idx);
uint8_t aoa_at_handle_zbeat(uint8_t *data, uint16_t len, uint8_t idx);
uint8_t aoa_at_handle_rdsspower(uint8_t *data, uint16_t len, uint8_t idx);
uint8_t aoa_at_handle_rnsspower(uint8_t *data, uint16_t len, uint8_t idx);
uint8_t aoa_at_handle_airplanemode(uint8_t *data, uint16_t len, uint8_t idx);
uint8_t aoa_at_handle_zlocation(void);
uint8_t aoa_at_handle_bbroadcast(uint8_t *data, uint16_t len, uint8_t idx);
uint8_t aoa_at_handle_bcall(uint8_t *data, uint16_t len, uint8_t idx);
uint8_t aoa_at_handle_bbind(uint8_t *data, uint16_t len, uint8_t idx);
uint8_t aoa_at_handle_bunbind(uint8_t *data, uint16_t len, uint8_t idx);
uint8_t aoa_at_handle_bbeat(uint8_t *data, uint16_t len, uint8_t idx);
uint8_t aoa_at_handle_binfowrite(uint8_t *data, uint16_t len, uint8_t idx);
uint8_t aoa_at_handle_bbbeat(uint8_t *data, uint16_t len, uint8_t idx);
uint8_t aoa_at_handle_blight(uint8_t *data, uint16_t len, uint8_t idx);
uint8_t aoa_at_handle_binfoerase(uint8_t *data, uint16_t len, uint8_t idx);
uint8_t aoa_at_handle_centeraddress(uint8_t *data, uint16_t len, uint8_t idx);
uint8_t aoa_at_handle_bsosinfo(uint8_t *data, uint16_t len, uint8_t idx);
uint8_t aoa_at_handle_bsos(uint8_t *data, uint16_t len, uint8_t idx);
uint8_t aoa_at_handle_selfcheck(uint8_t *data, uint16_t len, uint8_t idx);
uint8_t aoa_at_handle_bdistance(uint8_t *data, uint16_t len, uint8_t idx);
uint8_t aoa_at_handle_blowbattery(uint8_t *data, uint16_t len, uint8_t idx);
uint8_t aoa_at_handle_zreportloctime(uint8_t *data, uint16_t len, uint8_t idx);
uint8_t aoa_at_handle_breportinfotime(uint8_t *data, uint16_t len, uint8_t idx);
uint8_t aoa_at_handle_taskover(uint8_t *data, uint16_t len, uint8_t idx);
uint8_t aoa_at_handle_bboxunlock(uint8_t *data, uint16_t len, uint8_t idx);
uint8_t aoa_at_handle_taskbegin(uint8_t *data, uint16_t len, uint8_t idx);
/***************at only for test start**********************/
uint8_t aoa_at_handle_rdmsg(uint8_t *data, uint16_t len, uint8_t idx);
/***************at only for test end**********************/
uint8_t aoa_at_handle_bbeatnow(void);
uint8_t aoa_send_response(uint8_t *data, uint16_t len);
uint8_t get_zdevice_battery(void);
uint8_t test_begin(uint8_t *data, uint16_t len, uint8_t idx);
uint8_t aoa_at_handle_close_bdev_report(uint8_t *data, uint16_t len, uint8_t idx);
void save_dev_addd_nv(void);


uint8_t task_flag_start =0;
struct zdevice_setting zdev_set;
uint8_t flag_airmod =0x00;
	uint8_t taskidnum; 
	uint8_t group_num_data =0;
uint8_t bbind_num =0;

uint8_t get_bdev_group_num(void)
{
	 
	group_num_data = (bbind_num /BDEVICE_GROUP_NUM) +1;
  
  return group_num_data;
}
	
static const aoa_at_handle_symbol aoa_at_table[SUPPORT_AT_NUM] = {
	{test_begin,(uint8_t *)"AT+START="},
	{aoa_at_handle_zsaveb, (uint8_t *)"AT+ZSAVEB="},
	{aoa_at_handle_taskbegin, (uint8_t *)"AT+TASK"},
	{aoa_at_handle_zcall, (uint8_t *)"AT+ZCALL?"},
	{aoa_at_handle_zbeat, (uint8_t *)"AT+ZBEAT?"},
	{aoa_at_handle_zbind, (uint8_t *)"AT+ZBIND="},	
	{aoa_at_handle_zupdate, (uint8_t *)"AT+ZUPDATE="},
	{aoa_at_handle_rdsspower, (uint8_t *)"AT+RDSSPOWER="},
	{aoa_at_handle_rnsspower, (uint8_t *)"AT+RNSSPOWER="},
	{aoa_at_handle_airplanemode, (uint8_t *)"AT+AIRPLANEMODE="},
	{aoa_at_handle_zlocation, (uint8_t *)"AT+ZLOCATION?"},
	//{aoa_at_handle_bbroadcast, (uint8_t *)"AT+BBROADCAST?"},
	//{aoa_at_handle_bcall, (uint8_t *)"AT+BCALL="},
	{aoa_at_handle_bbind, (uint8_t *)"AT+BBIND="},
	{aoa_at_handle_bunbind, (uint8_t *)"AT+BUNBIND="},
	{aoa_at_handle_bbeat, (uint8_t *)"AT+BBEAT="},
	{aoa_at_handle_binfowrite, (uint8_t *)"AT+BINFOWRITE="},
	{aoa_at_handle_bbbeat, (uint8_t *)"AT+BBBEAT="},
	{aoa_at_handle_blight, (uint8_t *)"AT+BLIGHT="},
	{aoa_at_handle_binfoerase, (uint8_t *)"AT+BINFOERASE="},
	{aoa_at_handle_centeraddress, (uint8_t *)"AT+CENTERADDRESS="},
	{aoa_at_handle_bsosinfo, (uint8_t *)"AT+BSOSINFO="},
	{aoa_at_handle_bsos, (uint8_t *)"AT+BSOS="},
//	{aoa_at_handle_selfcheck, (uint8_t *)"AT+SELFCHECK?"},
	{aoa_at_handle_bdistance, (uint8_t *)"AT+BDISTANCE="},
	{aoa_at_handle_blowbattery, (uint8_t *)"AT+BLOWBATTERY="},
	{aoa_at_handle_zreportloctime, (uint8_t *)"AT+ZREPORTLOCTIME="},
	{aoa_at_handle_breportinfotime, (uint8_t *)"AT+BREPORTINFOTIME="},
	//{aoa_at_handle_taskover, (uint8_t *)"AT+TASKOVER?"},
	{aoa_at_handle_bboxunlock, (uint8_t *)"AT+BBOXUNLOCK="},
	{aoa_at_handle_close_bdev_report,(uint8_t *)"AT+WARINGSTATUS="},
/***************at only for test start**********************/
	//{aoa_at_handle_rdmsg, (uint8_t *)"AT+RDMSG="},
/***************at only for test end**********************/

};
uint8_t save_data_type=0;

uint8_t  get_save_data_type(void)
{

	return save_data_type;
}

uint8_t set_save_data_type(uint8_t data_type)
{

	 save_data_type = data_type;
	
}
extern TaskHandle_t devsavetask;
void send_to_save(void)
{

	 xTaskNotifyGive( devsavetask );

}

void save_dev_addr(void)
{
		set_save_data_type(0xf0);
		send_to_save();
}

void save_dev_info(void)
{
		set_save_data_type(0xe0);
		send_to_save();
}

void save_dev_info_flash(void)
{

   set_nvram_save_data(save_nv_buf);
}
void devinfo_save_task(void const * argument)
{
	
	
	uint8_t cmd = 0xff;
	
		while(1){
			printf("save task info start  \r\n");
			osDelay(5000);
			 	save_dev_addd_nv();
			printf("save task info ok  \r\n");
			#if 0
				ulTaskNotifyTake( pdTRUE, portMAX_DELAY);
			
				cmd = get_save_data_type();
				printf("save task cmd %x \r\n",cmd);
			switch(cmd){
				case 0xf0:
					printf("save task addr start  \r\n");
				    save_dev_addd_nv();
							printf("save task addr ok  \r\n");
					break;
				case 0xe0:
					printf("save task info start  \r\n");
					  save_dev_info_flash();
						printf("save task info ok  \r\n");
					break;
				
				default:
					break;
				
				  }
		#endif
		}



}


void aoa_at_handle_bsossend(void)
{
		uint8_t report_stat[30];
		uint8_t len;
		
		len = strlen("+BSOSSEND:");
	
		memcpy(report_stat,"+BSOSSEND:", len);
		
		memcpy(&report_stat[len],zdev_set.mydev_id,12);
		len+=12;
		memcpy(&report_stat[len],",",1);
		len +=1;

		report_stat[len] = zdev_set.report_mesage_id + '0';
		len +=1;
		
		memcpy(&report_stat[len],aoa_at_end_tok,4);
		
		len+=4;
	
		aoa_send_response(report_stat,len);
		//task_aoauart_mail_put(report_stat,len);
}




uint8_t report_charing_buf[33];
uint8_t bat_temp_buf[21];
uint8_t first_bat_soc =0;

uint8_t report_charing_soc(void)
{
	memcpy(report_charing_buf,"oharging:",9);
	memset(bat_temp_buf,0x00,21);
	
	bat_temp_buf[0] =0x15;
	bat_temp_buf[1] = 0x08;
	bat_temp_buf[2] = 0xef;
	
	if(!first_bat_soc){
	    first_bat_soc = 1;
	}else{
			if(	zdev_set.zbat_soc_bak > zdev_set.zbat_soc){
					if((zdev_set.zbat_soc_bak - zdev_set.zbat_soc) >2)
							zdev_set.zbat_soc = zdev_set.zbat_soc_bak;
			}else{
					if((zdev_set.zbat_soc - zdev_set.zbat_soc_bak) >2)
						zdev_set.zbat_soc = zdev_set.zbat_soc_bak;;
			
			}
	
			if(	zdev_set.zbat_soc1_bak > zdev_set.zbat_soc1){
					if((zdev_set.zbat_soc1_bak - zdev_set.zbat_soc1) >2)
							zdev_set.zbat_soc1 = zdev_set.zbat_soc1_bak;
			}else{
					if((zdev_set.zbat_soc1 - zdev_set.zbat_soc1_bak) >2)
						zdev_set.zbat_soc1 = zdev_set.zbat_soc1_bak;;
			
			}
	}
	
	if(zdev_set.zbattery !=0){
	
			bat_temp_buf[5] = zdev_set.zbat_soc <<4 | zdev_set.zbat_soc1 ;
	}else{

		bat_temp_buf[5] = zdev_set.zbat_soc <<4 | 0x0f;
	}

	bat_temp_buf[20] = zdev_set.bat_chr_status << 4  | zdev_set.bat1_chr_status;
	memcpy(&report_charing_buf[9],bat_temp_buf,21);
	
		memcpy(&report_charing_buf[30],"##",2);
	
	zdev_set.zbat_soc_bak = zdev_set.zbat_soc;
	zdev_set.zbat_soc1_bak = zdev_set.zbat_soc1;
	
	if(0== task_flag_start)
			ble_send_response(report_charing_buf,32);
}
extern uint8_t version_info[10];
uint8_t aoa_at_handle_bbeatnow(void)
{
		uint8_t  report_stat[70];
	//	uint8_t report_location[80];
		uint8_t len,lenth;
		//get_zdevice_battery();
		len = strlen("+BBEATNOW:");
	  set_val_to_report();
	  set_val1_to_report();
		memcpy(report_stat,"+BBEATNOW:",len);
		memcpy(&report_stat[len],",",1);
		len+=1;
		//memcpy(zdev_set.mydev_id,"101122334455",12);  //sunmny for test 0604
		if(zdev_set.mydev_id[0] != '1'){
					return 1;
		}
		memcpy(&report_stat[len],zdev_set.mydev_id,12);
		len+=12;
		memcpy(&report_stat[len],",",1);
		len+=1;
		if(zdev_set.zbattery == 1){
			memcpy(&report_stat[len],"OK,",3);
			len+=3;
		}else {
			memcpy(&report_stat[len],"ERROR",5);
			len+=5;
			//memcpy(&report_stat[len],"OK,",3);
			//len+=3;
		}
		//if(zdev_set.status.zrd_test_status ==0){
			//	memcpy(&report_stat[len]," RD",3);
		//	len+=3;
	//	}
		//if(zdev_set.status.zrn_test_status ==0){
			//	memcpy(&report_stat[len]," RN",3);
			//len+=3;
		//}
		if(zdev_set.zbattery ==0){
			memcpy(&report_stat[len]," RD",3);
			len+=3;
				memcpy(&report_stat[len]," EF",3);
			len+=3;
			memcpy(&report_stat[len],",",1);
		  len+=1;
		}
		
		//memcpy(&report_stat[len],",",1);
		//len+=1;
		

		memcpy(&report_stat[len],",",1);
		len+=1;
		//memcpy(&report_stat[len],); zdev_set.zbat_soc +'0';
		//len+=1;
		if(zdev_set.zbat_soc == 0){
				memcpy(&report_stat[len],"0",1);
				len+=1;
		}else if(zdev_set.zbat_soc == 1){
				memcpy(&report_stat[len],"1",1);
				len+=1;
		}else if(zdev_set.zbat_soc == 2){
				memcpy(&report_stat[len],"2",1);
				len+=1;
		}else if(zdev_set.zbat_soc == 3){
			memcpy(&report_stat[len],"3",1);
				len+=1;
		
		}else if(zdev_set.zbat_soc == 4){
			memcpy(&report_stat[len],"4",1);
				len+=1;
		
		
		}else {
			memcpy(&report_stat[len],"F",1);
				len+=1;
		
		}
		memcpy(&report_stat[len],",",1);
		len+=1;
		if(zdev_set.zbattery ==0){
			memcpy(&report_stat[len],"F",1);
				len+=1;	
	}else{
			if(zdev_set.zbat_soc1 == 0){
				memcpy(&report_stat[len],"0",1);
				len+=1;
		}else if(zdev_set.zbat_soc1 == 1){
				memcpy(&report_stat[len],"1",1);
				len+=1;
		}else if(zdev_set.zbat_soc1 == 2){
				memcpy(&report_stat[len],"2",1);
				len+=1;
		}else if(zdev_set.zbat_soc1 == 3){
			memcpy(&report_stat[len],"3",1);
				len+=1;
		
		}else if(zdev_set.zbat_soc1 == 4){
			memcpy(&report_stat[len],"4",1);
				len+=1;
		
		
		}
	
		}
		memcpy(&report_stat[len],",",1);
		len+=1;
		
		memcpy(&report_stat[len],"ON,",3);
			len+=3;
		
		
	if(zdev_set.zbattery ==0){
		memcpy(&report_stat[len],"OFF,",4);
			len+=4;
	}else{
			memcpy(&report_stat[len],"ON,",3);
			len+=3;
		}
		
		memcpy(&report_stat[len],",,,",3);
		len+=3;
		
		
		memcpy(&report_stat[len],version_info,8);
		len = len+8;
		
		memcpy(&report_stat[len],aoa_at_end_tok,4);
			len+=AT_END_TOK_LEN;

	 aoa_send_response(report_stat,len);
//		printf("bbreport : %s \r\n",report_stat);
		
		//task_aoauart_mail_put(report_stat,len);
}

uint8_t  light_flag = 0;
uint8_t  report_dis_buf[12] = {0};
//uint8_t  report_lenth[2]="99";
void aoa_at_handle_bpalert(void);
extern uint8_t contiue_task;
 
uint8_t aoa_at_handle_bbeatnow_bdev(uint8_t *report_id,uint8_t *report_info)
{
		uint8_t  report_stat[70];
			
		uint8_t len,lenth;
		len = strlen("+BBEATNOW:");	
		memcpy(report_stat,"+BBEATNOW:",len);
	
		memcpy(&report_stat[len],"1",1);
		len+=1;
		memcpy(&report_stat[len],",",1);
		len+=1;
		//memcpy(zdev_set.mydev_id,"101122334455",12);  //sunmny for test 0604
		if(report_id[0] != '1')
		{
				return 1;
		}
			memcpy(&report_stat[len],report_id,12);
		  memcpy(report_dis_buf,report_id,12);
		
		len+=12;
	
		memcpy(&report_stat[len],",",1);
		len+=1;
		if(report_info[6] =='1'&&report_info[7]=='1'&&report_info[8]=='1'){
			memcpy(&report_stat[len],"OK,",3);
			len+=3;
		}else {
			memcpy(&report_stat[len],"OK,",3);
			len+=3;
		}
	//	if(zdev_set.status.zrd_test_status ==0){
			//	memcpy(&report_stat[len]," RD",3);
		//	len+=3;
	//	}
		//if(zdev_set.status.zrn_test_status ==0){
			//	memcpy(&report_stat[len]," RN",3);
			//len+=3;
		//}
		//memcpy(&report_stat[len],",",1);
		//len+=1;
			//if((zdev_set.isMdev ==1)){
				//	memcpy(&report_stat[len],"10",2);
					//memcpy(&report_stat[len],"02",2);
					//memcpy(report_lenth,report_info,2);
				//	len +=2;
			//}else{
				//	memcpy(&report_stat[len],"10",2);
				//memcpy(&report_stat[len],"02",2);
					//memcpy(report_lenth,report_info,2);
				//	len +=2;
			
		//	}
			
			
				if((zdev_set.isMdev ==1)){
					memcpy(&report_stat[len],&report_info[1],2);
					//memcpy(report_lenth,report_info,2);
					len +=2;
			}else{
					memcpy(&report_stat[len],&report_info[3],2);
				//	memcpy(report_lenth,report_info,2);
					len +=2;
			
			}
		
		
		memcpy(&report_stat[len],",",1);
		len+=1;
		//memcpy(&report_stat[len],); zdev_set.zbat_soc +'0';
		//len+=1;
		if(report_info[5] == '0'){
				memcpy(&report_stat[len],"0",1);
				len+=1;
		}else if(report_info[5] == '1'){
				memcpy(&report_stat[len],"1",1);
				len+=1;
		}else if(report_info[5] == '2'){
				memcpy(&report_stat[len],"2",1);
				len+=1;
		}else if(report_info[5] == '3'){
			memcpy(&report_stat[len],"3",1);
				len+=1;
		
		}else if(report_info[5] == '4'){
			memcpy(&report_stat[len],"4",1);
				len+=1;	
		}else {
			memcpy(&report_stat[len],"F",1);
				len+=1;	
		}
		memcpy(&report_stat[len],",",1);
		len+=1;
/*******	状态改成开和关******/
			if(report_info[6] =='1'){
			 memcpy(&report_stat[len],"ON,",3);
				len+=3;
			}else if(report_info[6] =='0'){
			 memcpy(&report_stat[len],"OFF,",4);
				len+=4;
			
			}
			//else if(report_info[6] =='2'){
				//memcpy(&report_stat[len],"OFF,",4);
				//len+=4;
			//}
		
		if(report_info[7] =='1'){
		
		memcpy(&report_stat[len],"ON,",3);
			len+=3;
		}else{
		memcpy(&report_stat[len],"OFF,",4);
			len+=4;
		}
		if(report_info[8] =='1'){
			memcpy(&report_stat[len],"1",1);
			}else {
			memcpy(&report_stat[len],"0",1);
			}
		len+=1;
		
		memcpy(&report_stat[len],",",1);
		len+=1;
		if(report_info[19] =='0 '){
		memcpy(&report_stat[len],"OFF,",4);
		len+=4;
		}else{
		memcpy(&report_stat[len],"ON,",3);
		len+=3;
		}
		
		if(light_flag == 0){
		memcpy(&report_stat[len],"OFF",3);
		len+=3;
		}else if(light_flag ==1){
		memcpy(&report_stat[len],"ON",2);
		len+=2;
		}
//BDINFO:112019060441,0000031111000015500000##	
		memcpy(&report_stat[len],",",1);
		len = len+1;
		
		memcpy(&report_stat[len],"20",2);
		len = len+2;
		
	
			memcpy(&report_stat[len],&report_info[10],6);
		
		len+=6;
		
		memcpy(&report_stat[len],aoa_at_end_tok,4);
			len+=AT_END_TOK_LEN;
		//printf("report_stat:%s  %d \r\n",report_stat,len);
	 aoa_send_response(report_stat,len);
	//	printf("aoa bdev send -------------------\r\n");
		
		//task_aoauart_mail_put(report_stat,len);
}

void aoa_at_handle_bpalert(void)
{
		uint8_t  report_stat[30];
		//uint8_t report_location[60];
		uint8_t len,lenth;
		//get_zdevice_battery();
		len = strlen("+BPALERT:");

		memcpy(report_stat,"+BPALERT:",len);

		if(zdev_set.mydev_id[0] != '1'){
				return ;
		}
		memcpy(&report_stat[len],zdev_set.mydev_id,12);  //sunmny for test 0604
		len+=12;
		memcpy(&report_stat[len],",",1);
		len+=1;
		
		
		memcpy(&report_stat[len],"2",1);
		len+=1;
		
		memcpy(&report_stat[len],",",1);
		len+=1;
		
		report_stat[len] = zdev_set.zbat_soc + '0';
			len = len+1;
		
		memcpy(&report_stat[len],",",1);
			len+=1;
		if(zdev_set.zbattery ==1){
				report_stat[len] = zdev_set.zbat_soc1 + '0';
			  len+=1;
		}else{
		
			report_stat[len] =  'F';
			len+=1;
			
		}
			
			memcpy(&report_stat[len],aoa_at_end_tok,4);
			len+=AT_END_TOK_LEN;
	if(zdev_set.zbattery ==1){
	//	printf("zdev_set.report_bat_threshold = %d  soc :%d  soc1:%d ",zdev_set.report_bat_threshold,zdev_set.zbat_soc,zdev_set.zbat_soc1);
			if(zdev_set.zbat_soc1<=zdev_set.report_bat_threshold || zdev_set.zbat_soc<=zdev_set.report_bat_threshold ){
			//	printf("IN zdev_set.report_bat_threshold = %d  soc :%d  soc1:%d ",zdev_set.report_bat_threshold,zdev_set.zbat_soc,zdev_set.zbat_soc1);
	   aoa_send_response(report_stat,len);
		} 
	}else{
	//printf("zdev_set.report_bat_threshold = %d  soc :%d  soc1:%d ",zdev_set.report_bat_threshold,zdev_set.zbat_soc,zdev_set.zbat_soc1);
			if( zdev_set.zbat_soc<=zdev_set.report_bat_threshold ){
		//	printf("IN zdev_set.report_bat_threshold = %d  soc :%d  soc1:%d ",zdev_set.report_bat_threshold,zdev_set.zbat_soc,zdev_set.zbat_soc1);
	   aoa_send_response(report_stat,len);
		} 
	
		}
	



	//	task_aoauart_mail_put(report_stat,len);
}

void BatTimerCallback(void)
{
			set_val1_to_report();
			set_val_to_report();
			report_charing_soc();
}
uint8_t timer_send_count= 0;

extern uint8_t airmod_flag ;






uint8_t aoa_at_handle_taskbegin(uint8_t *data, uint16_t len, uint8_t idx)
{
			
}



/*todo, 20181113*/
void get_zdevice_id(uint8_t *data)
{
	//uint8_t* p="123456789012";  //需要修改获取不到id 返回error 0418
	//memcpy(data,p,DEVICE_ID_LEN);
		memcpy(data,zdev_set.mydev_id,DEVICE_ID_LEN);
}

void add_separator_comma(uint8_t *data)
{
		*data=',';
}


/*todo, 20181113*/
uint8_t get_zdevice_battery(void)
{
	uint16_t bat_soc;
	
	//zdev_read_battery_soc(&bat_soc);
		zdev_set.zbat_soc =3;
	
		return 3;
	
	if(bat_soc >=80 && bat_soc <100){
		printf("retrun 4 \r\n");
		return 4;
	}
	else if(bat_soc >=60&&bat_soc <80 )
				return 3;
	else if(bat_soc >=40 && bat_soc <60)
				return 2;
	else if(bat_soc >=20 && bat_soc <40)
				return 1;
	else if(bat_soc >0 && bat_soc <20)
			 return 0;
	
}

/*todo, 20181113*/
uint8_t get_ble_rssi(void)
{
	return 0;
}


uint8_t aoa_send_response(uint8_t *data, uint16_t len)
{
	return hw_uart1_send(data,len);
}

uint8_t make_zdevice_okerror_response(uint8_t is_id_need, uint8_t is_errno_need, 
											uint8_t result, uint8_t at_index)
{
	uint16_t totallen=0,len=0;
	uint8_t   deviceid[DEVICE_ID_LEN];
	uint8_t response[64];

	//len=strlen(aoa_at_respone_table[at_index].pData);
	//memcpy(&response[totallen],aoa_at_respone_table[at_index].pData,len);
	//totallen+=len;

	if(is_id_need)
	{
		get_zdevice_id(deviceid);
		len=DEVICE_ID_LEN;
		memcpy(&response[totallen],deviceid,len);
		totallen+=len;
	
	len=1;
	add_separator_comma(&response[totallen]);
	totallen+=len;

	}


	if(result==0)
	{
		len=2;
		memcpy(&response[totallen],"OK",len);
		totallen+=len;
	}
	else/*to do, error number needed? 20181113*/
	{
		uint8_t buf[10];
		len=5;
		memcpy(&response[totallen],"ERROR",len);
		totallen+=len;

		if(is_errno_need)
		{
			len=1;
			add_separator_comma(&response[totallen]);
			totallen+=len;
			
			int2str(result, buf);
			len=strlen(buf);
			memcpy(&response[totallen],buf,len);
			totallen+=len;
		}
	}

	len=AT_END_TOK_LEN;
	memcpy(&response[totallen],aoa_at_end_tok,len);
	totallen+=AT_END_TOK_LEN;

	return aoa_send_response(response,totallen);
}


/*
AT+ZBEAT?
+ZBEAT:101234567890123,1,2,2
*/
uint16_t make_zbeat_response(uint8_t *response)
{
	uint16_t totallen=0,len=0;
	uint8_t data;
	
	len=strlen("+ZBEAT:");
	memcpy(response,"+ZBEAT:",len);
	totallen+=len;
	
	get_zdevice_id(&response[totallen]);
	totallen+=DEVICE_ID_LEN;
	
	add_separator_comma(&response[totallen]);
	totallen+=1;
	
	data=get_zdevice_battery();
	if(data<0 || data>4)
	{
	response[totallen]='F';
	}else{
	response[totallen]=data+0x30;
	}
	totallen+=1;
	
	add_separator_comma(&response[totallen]);
	totallen+=1;
	
	data=get_rd_rssi();
	if(data<0 || data>4){
	response[totallen]='F';
	}else{
	response[totallen]=data+0x30;
	}
	totallen+=1;
	
	//add_separator_comma(&response[totallen]);
//	totallen+=1;
	
	//data=get_ble_rssi();
	//response[totallen]=data+0x30;
	//totallen+=1;
	
	memcpy(&response[totallen],aoa_at_end_tok,AT_END_TOK_LEN);
	totallen+=AT_END_TOK_LEN;

	return totallen;
}

uint8_t add_ok_data(uint8_t *response)
{



}
uint16_t make_zcall_response(uint8_t *response)
{
	uint16_t totallen=0,len=0;
	uint8_t data;
	
	len=strlen("+ZCALL:");
	memcpy(response,"+ZCALL:",len);
	totallen+=len;
	
	#if 1
	get_zdevice_id(&response[totallen]);
	totallen+=DEVICE_ID_LEN;
	
	add_separator_comma(&response[totallen]);
	totallen+=1;
	
	
	memcpy(&response[totallen],"OK",2);
	totallen +=2;
	#endif  //需要修改返回值
	//memcpy(&response[totallen],"ERROR",5);
	//totallen +=5;
	
	memcpy(&response[totallen],aoa_at_end_tok,AT_END_TOK_LEN);
	totallen+=AT_END_TOK_LEN;

	return totallen;
}
//sunmny
uint16_t make_zsave_response(uint8_t *response)
{
	uint16_t totallen=0,len=0;
	uint8_t data;
	
	len=strlen("+ZSAVEB:");
	memcpy(response,"+ZSAVEB:",len);
	totallen+=len;
	
	//get_zdevice_id(&response[totallen]);
	//totallen+=DEVICE_ID_LEN;
	

	
	memcpy(&response[totallen],aoa_at_end_tok,AT_END_TOK_LEN);
	totallen+=AT_END_TOK_LEN;

	return totallen;
}
uint16_t make_zupdate_response(uint8_t *response)
{
	
	return 0;
}
uint16_t make_zbind_response(uint8_t *response)
{
	uint16_t totallen=0,len=0;
	uint8_t data;
	
	if(zdev_set.zdev_is_set){
	len=strlen("+ZBIND:OK");
	memcpy(response,"+ZBIND:OK",len);
	totallen+=len;
	}else if (zdev_set.zdev_is_set == 0){
	
	len=strlen("+ZBIND:ERROR");
	memcpy(response,"+ZBIND:ERROR",len);
	totallen+=len;
	}
	
	memcpy(&response[totallen],aoa_at_end_tok,AT_END_TOK_LEN);
	totallen+=AT_END_TOK_LEN;

	return totallen;
}
uint8_t get_zdevice_num(uint8_t *data,uint16_t len,uint8_t idx)
{
			uint8_t lenth = 0;
			uint8_t zd_num;
	
			lenth = strlen(aoa_at_table[idx].pData);
			if(data[lenth] == '2')
					return 2;
			else if(data[lenth] == '1')
					return 1;
			
}
//AT+ZBIND=2,234567890123,101234567890124
uint8_t test_buf[12];
uint8_t get_param_count(uint8_t *data,uint16_t len,uint8_t idx)
{
		uint8_t count ;
		uint8_t par_count =0;
	
			while(data[count]){
				if(data[count] == ',')
						par_count ++;
				
					count ++;
			}


}
extern uint8_t save_nv_buf[70];
uint8_t get_zdev_id(uint8_t *data , uint16_t len ,uint8_t idx)
{
		uint8_t head_len = 0,i=0,end_count = 0;
		uint8_t count = 0,par_count =0,temp_count[20];
		head_len = strlen("AT+ZBIND");
		count = head_len;
	
		while((data[count+1]!='#')&&(data[count+2]!='#')){
					if(data[count] == '=' || data[count] ==','){
							temp_count[par_count] = count;
							par_count ++;
					}
					count++;
		}
		end_count = count;
		
		printf("par_count = %d \r\n ",par_count);
		
		for(i =0;i<12;i++){
			//	printf("temp_count[%d] = %d \r\n ",i,temp_count[i]);
							switch(i){
								case 0:
										if(temp_count[1] -temp_count[0] ==1)
												//save_nv_buf[4] = data[temp_count[1]-1];
												zdev_set.zdev_num = 1;//data[temp_count[1]-1] - '0';
//												printf("zdev_set.zdev_num = %d \r\n",zdev_set.zdev_num);
									break;
								case 1:
											if(temp_count[2] -temp_count[1] >1){
												
													memcpy(zdev_set.other_id,&data[temp_count[1]+1],12);
													//memcpy(&save_nv_buf[5],zdev_set.other_id,12);
//												printf("zdev_set.other_id = %s\r\n",zdev_set.other_id);
											}
									break;
								case 2:
											//	memcpy(zdev_set.rn_num,&data[temp_count[i]+1],(temp_count[i+1] - temp_count[i] -1));
												//memcpy(&save_nv_buf[10],&data[temp_count[i]+1],6);
											//	printf("zdev_set.rn_num = %s\r\n",zdev_set.rn_num);
									break;
								case 3:
										//	memcpy(zdev_set.rd_num,&data[temp_count[i]+1],(temp_count[i+1] - temp_count[i] -1));
											// memcpy(&save_nv_buf[16],&data[temp_count[i]+1],6);
											//	printf("zdev_set.rd_num = %s\r\n",zdev_set.rd_num);
									break;
								case 4:
										//	memcpy(zdev_set.report_mesage_id,&data[temp_count[i]+1],(temp_count[i] - temp_count[i-1] -1));
											//zdev_set.report_mesage_id = data[temp_count[i]+1] - '0';
											//save_nv_buf[22]=zdev_set.report_mesage_id +'0';
//												printf("zdev_set.report_mesage_id = %d\r\n",zdev_set.report_mesage_id);
							//	printf("save_nv_buf %s\r\n",save_nv_buf);
									break;
								case 5:
									//	memcpy(zdev_set.zdev_report_time,&data[temp_count[i]+1],(temp_count[i+1] - temp_count[i] -1));
//									printf("(zdev_set.zdev_report_time = %s\r\n",zdev_set.zdev_report_time);	
									break;
								case 6:
//									printf("zdev_set.bdev_report_time = %d %d\r\n",temp_count[i+1], temp_count[i]);
								//	memcpy(zdev_set.bdev_report_time,&data[temp_count[i]+1],(temp_count[i+1] - temp_count[i] -1));
//												printf("zdev_set.bdev_report_time = %s\r\n",zdev_set.bdev_report_time);
									break;
								case 7:
									//memcpy(zdev_set.report_ip,&data[temp_count[i]+1],(temp_count[i+1] - temp_count[i] -1));
//												printf("zdev_set.report_ip = %s\r\n",zdev_set.report_ip);
									break;
								case 8:
									//memcpy(zdev_set.report_port,&data[temp_count[i]+1],(temp_count[i+1] - temp_count[i] -1));
//												printf("zdev_set.report_port = %s\r\n",zdev_set.report_port);
									break;
								
								case 9:
									//memcpy(zdev_set.report_apn,&data[temp_count[i]+1],(temp_count[i+1] - temp_count[i] -1));
//												printf("zdev_set.report_apn = %s\r\n",zdev_set.report_apn);
									break;
							case 10:
									//memcpy(zdev_set.user_name,&data[temp_count[i]+1],(temp_count[i+1] - temp_count[i] -1));
//												printf("zdev_set.user_name = %s\r\n",zdev_set.user_name);
									break;
							
							case 11:
									//memcpy(zdev_set.user_paswd,&data[temp_count[i]+1],end_count -temp_count[i] +1);
//												printf("zdev_set.user_paswd = %s\r\n",zdev_set.user_paswd);
									break;
						
								defualt:
									break;
							
							
							
							}
		
		}
	 
	zdev_set.zdev_is_set =1;
	
		#if 0
		while(data[count++]){
			if(data[count] == ','){
						par_count ++;
			
			
			if(par_count == 1){
					if(data[count -1] != '=' && data[count-1] != '1'){
						printf("count1 = %d\r\n",count);
						memcpy(zdev_set.other_id,&data[count+1],DEVICE_ID_LEN);
						
					}
			}else if(par_count == 2){
						printf("count2 = %d\r\n",count);
				
						temp_count = count;
			}
			else if(par_count == 3){
				printf("count3 = %d\r\n",count);
				memcpy(zdev_set.rn_num,&data[temp_count+1],6);
				temp_count = count;
				printf("rn_num %s \r\n",zdev_set.rn_num);		
			}
				else if(par_count ==4){
					printf("count4 = %d\r\n",count);
					memcpy(zdev_set.rd_num,&data[temp_count+1],6);
					temp_count = count;
					printf("rd_num %s \r\n",zdev_set.rd_num);	
						
				}else if(par_count == 5){
						zdev_set.report_mesage_id = data[temp_count+1] - '0';
						temp_count = count;
				}else if(par_count == 6){
						#if 0
						if((count-(temp_count+1)) == 1){
									zdev_set.zdev_report_time = data[temp_count+1] -'0';
						}else if((count -(temp_count +1)) == 2){
						
									zdev_set.zdev_report_time = (data[temp_count+1] -'0')*10 + (data[temp_count+2] -'0');
						
						}else if((count -(temp_count+1) ==3) ){
										zdev_set.zdev_report_time = (data[temp_count+1] -'0')*100 + ((data[temp_count+2] -'0')*10)\
								+(data[temp_count+3] -'0');
						}
						#endif
						memset(zdev_set.zdev_report_time,0x00,4);
						memcpy(zdev_set.zdev_report_time, &data[temp_count +1],count-(temp_count+1));
						temp_count = count;
				}else if(par_count == 7){
				#if 0
				if((count-(temp_count+1)) == 1){
									zdev_set.bdev_report_time = data[temp_count+1] -'0';
						}else if((count -(temp_count +1)) == 2){
						
									zdev_set.bdev_report_time = (data[temp_count+1] -'0')*10 + (data[temp_count+2] -'0');
						
						}else if((count -(temp_count+1) ==3) ){
										zdev_set.bdev_report_time = (data[temp_count+1] -'0')*100 + ((data[temp_count+2] -'0')*10)\
								+(data[temp_count+3] -'0');
						}
						#endif
						memset(zdev_set.bdev_report_time,0x00,4);
						memcpy(zdev_set.bdev_report_time, &data[temp_count +1],count-(temp_count+1));
						temp_count = count;
				}else if(par_count == 8){
				
//				memcpy(zdev_set.report_ip, &data[temp_count +1],count-(temp_count+1));
					temp_count = count ;
					
				}else if(par_count == 9){
					memset(zdev_set.report_port,0x00,8);
					printf("ssreprot = %c \r\n",data[temp_count +1]);
					printf("ssreprot = %c \r\n",data[temp_count +2]);
					printf("ssreprot = %c \r\n",data[temp_count +3]);
					printf("ssreprot = %c \r\n",data[temp_count +4]);
					memcpy(&zdev_set.report_port[0],&data[temp_count +1],4);
					printf("ssreprot = %s \r\n",zdev_set.report_port);
					temp_count = count ;
				
				}else if(par_count == 10){
				
//					memcpy(zdev_set.report_apn, &data[temp_count +1],count-(temp_count+1));
					temp_count = count ;
				
				
				}else if(par_count == 11){
				
					memcpy(zdev_set.user_name, &data[temp_count +1],count-(temp_count+1));
					temp_count = count ;
				}else if(par_count == 12){
				
				memcpy(zdev_set.user_paswd, &data[temp_count +1],count-(temp_count+1));
					temp_count = count ;
				
				}
			
				zdev_set.zdev_is_set =1;
		}else{
			
				if((data[count] == '#')&&(data[count+1] =='#')&&(data[count+2] ==0x0d)&&(data[count+3] == 0x0a)){
					memcpy(zdev_set.user_paswd, &data[temp_count +1],count-(temp_count+1));
					break;
				}else
						continue;
		}
			
		
	}
	#endif		
}
uint8_t aoa_at_handle_zupdate(uint8_t *data, uint16_t len, uint8_t idx) //需要修改返回值 
{
	uint16_t totallen;

	uint8_t zdev_id[12];
	
	uint8_t response[64];
	

	
	totallen=make_zupdate_response(response);
	return aoa_send_response(response,totallen);
}
uint8_t aoa_at_handle_zbind(uint8_t *data, uint16_t len, uint8_t idx)
{
	uint16_t totallen;
	uint8_t ret;
	uint8_t response[64];
	
	
		zdev_set.zdev_num = 1;//get_zdevice_num(data,len,idx);
	
	//get_zdev_id(data,len,idx);

		memcpy(response,"+ZBIND:OK",9);
		memcpy(&response[9],aoa_at_end_tok,4);
	//totallen=make_zbind_response(response);
	 aoa_send_response(response,13);
	return 0;
}

void send_getid_zdev(uint8_t *buf,uint8_t len)
{
	
		ble_send_response(buf,len);


}

extern uint8_t bleisok ;
extern uint8_t gsn_buf[12];
extern uint8_t zdev_isbind;

uint8_t zcall_lenth;
uint8_t aoa_at_handle_zcall(uint8_t *data, uint16_t len, uint8_t idx)
{
	uint16_t totallen;
	
	uint8_t buf[12];
		uint8_t zcall_response[64];
	memcpy(buf,"versionget##",12);
	
	ble_send_response(buf,12);
	
  printf("bleisok = %d \r\n",bleisok);
	if(bleisok == 0){
			memcpy(zcall_response,"+ZCALL:ERROR",12);
			memcpy(&zcall_response[12],"##",4);
		aoa_send_response(zcall_response, 16);
	}else{
			
	
			totallen = strlen("+ZCALL:");
		
			memcpy(zcall_response,"+ZCALL:",totallen);
			memcpy(&zcall_response[totallen],gsn_buf,12);			
			memcpy(zdev_set.mydev_id,gsn_buf,12);
			totallen +=12;
			memcpy(&zcall_response[totallen],",OK",3);
			totallen +=3;
		   
		 // zcall_lenth = totallen;
			//memcpy(&buf[lenth1],"20",2);
			//lenth1 +=2;
			//memcpy(&zcall_response[totallen],version_info,8);
			//totallen +=8;
			memcpy(&zcall_response[totallen],"##",2);
			totallen +=2;
	   	printf("zcall buf is %s \r\n",buf);
		   zdev_isbind = 1;
		aoa_send_response(zcall_response, totallen);
		
	}




	
	
}

uint8_t aoa_get_wlist_addr(uint8_t *data, uint16_t len, uint8_t idx)
{
			uint8_t head_len = 0,lenth;
			uint8_t count=0,count_bak =0;
			uint8_t back_buf[50];
			lenth=strlen("+ZSAVEB:");
			head_len = strlen("AT+ZSAVEB=");
			memcpy(back_buf,"+ZSAVEB:",lenth);
			
			while(data[count]){
						if(data[count] == ','){
								count_bak +=1;
						}
						if(count_bak == 2){
							if((data[count-1] -'0') == 1){
								memcpy(&zdev_set.bbinddev.bdev_id[0][zdev_set.bdevtask1_num][0],&data[count+1],12);
								
							  zdev_set.bbinddev.bdev_id[0][zdev_set.bdevtask1_num][12] = (1 + '0');
								
								printf("bdev_id %s \r\n",zdev_set.bbinddev.bdev_id[0][zdev_set.bdevtask1_num]);
								zdev_set.bdevtask1_num +=1;
							}else if((data[count-1] -'0') == 2){
								memcpy(&zdev_set.bbinddev.bdev_id[1][zdev_set.bdevtask2_num][0],&data[count+1],12);
								
								 zdev_set.bbinddev.bdev_id[1][zdev_set.bdevtask2_num][12] = (2 + '0');
								zdev_set.bdevtask2_num +=1;
							}
							
							break;
						}		
							count ++;
			}
		
			//printf();
			memcpy(&back_buf[lenth],&data[head_len],27);
			lenth +=27;
			
			memcpy(&back_buf[lenth],",",1);
			lenth +=1;
			memcpy(&back_buf[lenth],"OK",2);
			lenth +=2;
			
			memcpy(&back_buf[lenth],aoa_at_end_tok,AT_END_TOK_LEN);
			lenth +=AT_END_TOK_LEN;
			aoa_send_response(back_buf,lenth);
			return 0;
	

}

uint8_t aoa_at_handle_zsaveb(uint8_t *data, uint16_t len, uint8_t idx) //test 
{
	uint16_t totallen=0,lenth=0,ret =0;
	uint8_t response[64];
	zdev_set.isMdev =0;
	
	ret = aoa_get_wlist_addr( data,  len,  idx);
		
	
	
	
	
	
	
	#if 0
	if(ret == 0){
		if(data[10] == ','){
			
			memcpy(response,"+ZSAVEB:,",lenth+1);
			totallen+=lenth;
			totallen+=1;
			//memcpy(&response[totallen],&zdev_set.white_list[white_list_count-1][12],1);
			totallen+=1;
			memcpy(&response[totallen],",",1);
			totallen+=1;
			//memcpy(&response[totallen],&zdev_set.white_list[white_list_count-1][0],12);
			totallen+=12;
			memcpy(&response[totallen],",",1);
			totallen+=1;
			memcpy(&response[totallen],"OK",2);
			totallen+=2;
	}else if (data[10] == '1'){
	
			memcpy(response,"+ZSAVEB:",lenth);
			totallen+=lenth;
			memcpy(&response[totallen],zdev_set.other_id,12);
			totallen+=12;
			memcpy(&response[totallen],",",1);
			totallen+=1;
			memcpy(&response[totallen],&zdev_set.white_list[white_list_count-1][12],1);
			totallen+=1;
			memcpy(&response[totallen],",",1);
			totallen+=1;
			memcpy(&response[totallen],&zdev_set.white_list[white_list_count-1][0],12);
			totallen+=12;
			memcpy(&response[totallen],",",1);
			totallen+=1;
			memcpy(&response[totallen],"OK",2);
			totallen+=2;
	
	}
	
	}else if( ret ==1 ){
	
	if(data[10] == ','){
			
			memcpy(response,"+ZSAVEB:,",lenth+1);
			totallen+=lenth;
			totallen+=1;
			memcpy(&response[totallen],&zdev_set.white_list[white_list_count-1][12],1);
			totallen+=1;
			memcpy(&response[totallen],",",1);
			totallen+=1;
			memcpy(&response[totallen],&zdev_set.white_list[white_list_count-1][0],12);
			totallen+=12;
			memcpy(&response[totallen],",",1);
			totallen+=1;
			memcpy(&response[totallen],"ERROR,1",7);
			totallen+=7;
	}else if (data[10] == '1'){
	
			memcpy(response,"+ZSAVEB:",lenth);
			totallen+=lenth;
			memcpy(&response[totallen],zdev_set.other_id,12);
			totallen+=12;
			memcpy(&response[totallen],",",1);
			totallen+=1;
			memcpy(&response[totallen],&zdev_set.white_list[white_list_count-1][12],1);
			totallen+=1;
			memcpy(&response[totallen],",",1);
			totallen+=1;
			memcpy(&response[totallen],&zdev_set.white_list[white_list_count-1][0],12);
			totallen+=12;
			memcpy(&response[totallen],",",1);
			totallen+=1;
			memcpy(&response[totallen],"ERROR,1",7);
			totallen+=7;
	
	}
	
	
	}else if(ret == 2){
	
	if(data[10] == ','){
			
			memcpy(response,"+ZSAVEB:,",lenth+1);
			totallen+=lenth;
			totallen+=1;
			memcpy(&response[totallen],&zdev_set.white_list[white_list_count-1][12],1);
			totallen+=1;
			memcpy(&response[totallen],",",1);
			totallen+=1;
			memcpy(&response[totallen],&zdev_set.white_list[white_list_count-1][0],12);
			totallen+=12;
			memcpy(&response[totallen],",",1);
			totallen+=1;
			memcpy(&response[totallen],"ERROR,2",7);
			totallen+=7;
	}else if (data[10] == '1'){
	
			memcpy(response,"+ZSAVEB:",lenth);
			totallen+=lenth;
			memcpy(&response[totallen],zdev_set.other_id,12);
			totallen+=12;
			memcpy(&response[totallen],",",1);
			totallen+=1;
			memcpy(&response[totallen],&zdev_set.white_list[white_list_count-1][12],1);
			totallen+=1;
			memcpy(&response[totallen],",",1);
			totallen+=1;
			memcpy(&response[totallen],&zdev_set.white_list[white_list_count-1][0],12);
			totallen+=12;
			memcpy(&response[totallen],",",1);
			totallen+=1;
			memcpy(&response[totallen],"ERROR,2",7);
			totallen+=7;	
	}	
	
	}
		memcpy(&response[totallen],aoa_at_end_tok,AT_END_TOK_LEN);
	totallen+=AT_END_TOK_LEN;

		return aoa_send_response(response,totallen);
			#endif
}
uint8_t aoa_at_handle_zbeat(uint8_t *data, uint16_t len, uint8_t idx)  //需要获取实际电量，rd的值
{
	uint16_t totallen;
	uint8_t response[64];
	uint8_t buf[6];
	//memcpy(buf,"gzid##",6);
	//send_getid_zdev(buf,6);
	totallen=make_zbeat_response(response);
	return aoa_send_response(response,totallen);
}

/*todo, check param len, may split data into serveral packages, 20181113*/
uint8_t handle_bdevice_rdsspower(uint8_t *data, uint16_t len, uint8_t idx)
{
	osStatus ret;
	ret=task_ble_mail_put(data, len);
	return ((uint8_t)(ret));
}

uint8_t handle_zdevice_rdsspower(uint8_t onoff, uint8_t idx)
{
	


	uint8_t result=0,res=0;
	if(hw_bd1_is_exist()==1)
	{
		if(onoff==0)
			hw_bd1_close();
		else if(onoff==1)
			hw_bd1_open();
	}
	else
	{
		hw_bd1_close();
		result=1;
	}
	res=make_zdevice_okerror_response(1,1,result,idx);
	return res;

}

uint8_t aoa_at_handle_rdsspower(uint8_t *data, uint16_t len, uint8_t idx)
{
	uint16_t totallen=0,lenth=0,lenth1 = 0;
	uint8_t ret,id1,id2;
	uint8_t response[128];
	
	
	lenth = strlen("AT+RDSSPOWER=");
	
	
	lenth1=strlen("+RDSSPOWER:");
	memcpy(response,"+RDSSPOWER:",lenth1);
	totallen+=lenth1;
	
	memcpy(&response[totallen],&data[lenth],1);
	totallen +=1;
	
	add_separator_comma(&response[totallen]);
		totallen+=1;
	memcpy(&response[totallen],&data[lenth+2],12);
	totallen +=12;
	
add_separator_comma(&response[totallen]);
		totallen+=1;
	memcpy(&response[totallen],"OK",2);
	totallen +=2;
	
	memcpy(&response[totallen],aoa_at_end_tok,AT_END_TOK_LEN);
	totallen+=AT_END_TOK_LEN;
	
	ret = aoa_send_response(response,totallen);
	#if 0
	ZbTokenizer  tzer[1];
	ZbToken  tok_device_id;
	uint16_t headerlen=0;
	uint8_t   deviceid[DEVICE_ID_LEN],res;

	headerlen=strlen(aoa_at_table[idx].pData);
	zb_tokenizer_init(tzer, data+headerlen, data+len);
	get_zdevice_id(deviceid);

	if(tzer[0].count==2)//maybe zdevice
	{
		tok_device_id = zb_tokenizer_get(tzer,0);
		if(0==memcmp(deviceid,tok_device_id.p,DEVICE_ID_LEN))
		{
			ZbToken tok_power=zb_tokenizer_get(tzer,1);
			if(tok_power.end-tok_power.p==2)//on
				res=handle_zdevice_rdsspower(1,idx);
			else if(tok_power.end-tok_power.p==3)//off
				res=handle_zdevice_rdsspower(0,idx);
		}
		else
		{
			res=handle_bdevice_rdsspower(data, len, idx);
		}
	}
	else
	{
		res=handle_bdevice_rdsspower(data, len, idx);
	}
	return res;
	#endif
}

/*todo, check param len, may split data into serveral packages, 20181113*/
uint8_t handle_bdevice_rnsspower(uint8_t *data, uint16_t len, uint8_t idx)
{
	osStatus ret;
	ret=task_ble_mail_put(data, len);
	return ((uint8_t)(ret));
}

uint8_t handle_zdevice_rnsspower(uint8_t onoff, uint8_t idx)
{
	uint8_t result=0,res=0;
	if(onoff==0)
		hw_bd2_close();
	else if(onoff==1)
		hw_bd2_open();
	res=make_zdevice_okerror_response(1,1,result,idx);
	return res;
}


uint8_t aoa_at_handle_rnsspower(uint8_t *data, uint16_t len, uint8_t idx)
{
	uint16_t totallen=0,lenth=0,lenth1 = 0;
	uint8_t ret,id1,id2;
	uint8_t response[128];
	
	
	lenth = strlen("AT+RNSSPOWER=");
	
	
	lenth1=strlen("+RNSSPOWER:");
	memcpy(response,"+RNSSPOWER:",lenth1);
	totallen+=lenth1;
	
	memcpy(&response[totallen],&data[lenth],1);
	totallen +=1;
	
	add_separator_comma(&response[totallen]);
		totallen+=1;
	memcpy(&response[totallen],&data[lenth+2],12);
	totallen +=12;
	
add_separator_comma(&response[totallen]);
		totallen+=1;
	memcpy(&response[totallen],"OK",2);
	totallen +=2;
	
	memcpy(&response[totallen],aoa_at_end_tok,AT_END_TOK_LEN);
	totallen+=AT_END_TOK_LEN;
	
	ret = aoa_send_response(response,totallen);
	
}

uint8_t handle_zdevice_airplanemode(uint8_t idx)
{
	uint8_t result=0,res=0;
	hw_bd1_close();
	hw_bd2_close();
	res=make_zdevice_okerror_response(1,1,result, idx);
	return res;
}


/*todo, check param len, may split data into serveral packages, 20181113*/
uint8_t handle_bdevice_airplanemode(uint8_t *data, uint16_t len, uint8_t idx)
{
	osStatus ret;
	ret=task_ble_mail_put(data, len);
	return ((uint8_t)(ret));
}

uint8_t addr_nv_buf[1500];
extern uint8_t send_wl_temp[100][15];
void clear_dev_nv(void)
{
	memset(addr_nv_buf,0x00,1500);
	set_nvram_id_data(addr_nv_buf);
	
	//memset(addr_nv_buf,0x00,1500);
	//set_nvram_id_data(addr_nv_buf);
}
void save_dev_addd_nv(void)
{
			uint8_t i,j;
			uint8_t ret;
		memset(addr_nv_buf,0x00,1500);
	for(i = 0;i<100;i++)
	{
			memcpy(&addr_nv_buf[i*15],send_wl_temp[i],15);
	}
	
	taskENTER_CRITICAL();
	ret = set_nvram_id_data(addr_nv_buf);
	if(ret)
		 set_nvram_id_data(addr_nv_buf);
  taskEXIT_CRITICAL();
}
uint8_t open_airmode = 0;
uint8_t blebuf[50];
uint8_t group_location_count=0;
uint8_t air_count_bak =0;
uint8_t aoa_at_handle_airplanemode(uint8_t *data, uint16_t len, uint8_t idx)//需要修改
{
	uint8_t buf[30],flag =0;
	uint8_t lenth,lenth1 = 0;

	lenth1 = strlen("airplanemode:");
	lenth = strlen("AT+AIRPLANEMODE=");
	
	flag_airmod = 1;
	timer_send_count = 0;
	
	//osTimerStop(LedTimerHandle);
	
	
	 // save_nv_buf[2]=zdev_set.report_dis+'0';
	 // save_nv_buf[3]=zdev_set.report_bat_threshold+'0';
	
		if(zdev_set.isMdev ==1){
			save_nv_buf[1] = '1';
		}else{
			save_nv_buf[1] = '0';
		}
				save_nv_buf[0] = '1';
		//set_nvram_save_data(save_nv_buf);
		
	memcpy(buf,"+AIRPLANEMODE:ON,OK##\r\n",25);
	aoa_send_response(buf,25);
	//	set_nvram_save_data(save_nv_buf);
	if(data[lenth] =='O'&&data[lenth+1] =='N'){
	//	ble_send_response("airmod:on\r\n",13);
		memcpy(blebuf,"airplanemode:",lenth1);
		memcpy(&blebuf[lenth1],zdev_set.mydev_id,12);
		lenth1 +=12;
		blebuf[lenth1] = ',';
		lenth1 +=1;
		if(zdev_set.other_id[0] == '1' &&zdev_set.other_id[1] == '0'){
		memcpy(&blebuf[lenth1],zdev_set.other_id,12);
		}
		else
		{
		memcpy(&blebuf[lenth1],"000000000000",12);
		}
		lenth1 +=12;
		
		blebuf[lenth1] = ',';
		lenth1 +=1;
		
		blebuf[lenth1] = '0';
		lenth1 +=1;
		
		blebuf[lenth1] = '1';
		lenth1 +=1;
		
		blebuf[lenth1] = ',';
		lenth1 +=1;
		//group_location_count = lenth1;
		
	//	get_bdev_group_num();
	//	air_count_bak=lenth1;
		
	//	while(group_num_data){
		//lenth1 = air_count_bak;
		//blebuf[lenth1] = group_num_data /10 + '0';
		//lenth1 +=1;
		
		//blebuf[lenth1] = group_num_data %10 +'0';
		//lenth1 +=1;
		
		//memcpy(&blebuf[lenth1],"##",2);
	//	lenth1 +=2;
		//ble_send_response(blebuf,lenth1);
			//group_num_data--;
		//osDelay(300);
		//}
		hw_bd1_close();
		hw_bd2_close();
		open_airmode =1;
		set_command_type(0xf2);
	}else {
		hw_bd1_open();
		hw_bd2_open();
		set_command_type(0xff);
		//ble_send_response("airmod:off\r\n",14);
	}

}
extern uint8_t bd_send_resp_len;
uint8_t aoa_at_handle_zlocation(void)
{
	uint16_t resplen=0;
	uint8_t buf[80];/*to do, are 80 bytes enough?*/
	resplen=get_zdevice_location_response(buf);
	return aoa_send_response(response_location, bd_send_resp_len);
}



/*
AT+BBROADCAST?
+BBROADCAST:3,111234567890123,121234567890124,131234567890124
*/
uint16_t make_bbroadcast_response(uint8_t *response)
{
	
}
 //set bbro get how much bdev;
uint8_t aoa_at_handle_bbroadcast(uint8_t *data, uint16_t len, uint8_t idx)
{
	
}

uint8_t aoa_at_handle_bcall(uint8_t *data, uint16_t len, uint8_t idx)
{

	
}


/*
AT+BBIND=111234567890123,111234567890124
+BBIND:111234567890123,OK,111234567890124,ERROR
*/



uint8_t bdev_id_temp_buf[13];

uint8_t get_task_bdev_id(uint8_t *data , uint16_t len ,uint8_t idx)
{
		uint8_t head_len = 0,i=0,end_count = 0;
		uint8_t count = 0,par_count =0,temp_count[20];
		uint8_t j = 0;
		head_len = strlen("AT+BBIND=");
		count = head_len;
		
			
		if((data[head_len] == '1' ||data[head_len] == '2') && data[head_len+1] ==','){
					zdev_set.zdev_num = 1;
					zdev_set.bbinddev.bdev_taskid = data[head_len] - '0';
			if(zdev_set.bbinddev.bdev_taskid ==1){
						for(j =0;j<100;j++){
							printf("bdev id is come \r\n");
						if(0==(memcmp(&zdev_set.bbinddev.bdev_id[0][j][0], &data[head_len +2],12))){
						
						 printf("bdev id is in\r\n");
						break;
						}else{
						///	memset(&zdev_set.bbinddev.bdev_id[0][j][0],0x00,13);
							 printf("zdev_set.bbinddev.bdev_id[0][%d][0] = %x %x %x %x \r\n",j,zdev_set.bbinddev.bdev_id[0][j][0],zdev_set.bbinddev.bdev_id[0][j][1],zdev_set.bbinddev.bdev_id[0][j][2],zdev_set.bbinddev.bdev_id[0][j][3]);
							if(zdev_set.bbinddev.bdev_id[0][j][0] == 0 && zdev_set.bbinddev.bdev_id[0][j][1] == 0 && zdev_set.bbinddev.bdev_id[0][j][2] ==0 &&zdev_set.bbinddev.bdev_id[0][j][3] ==0){
						memcpy(&zdev_set.bbinddev.bdev_id[0][zdev_set.bdevtask1_num][0], &data[head_len +2],12);
						memcpy(&zdev_set.bbinddev.bdev_id[0][zdev_set.bdevtask1_num][12],"1",1);
					  memcpy(bdev_id_temp_buf,&data[head_len +2],12);
					  printf("bdev id is0 %s \r\n",zdev_set.bbinddev.bdev_id[0][zdev_set.bdevtask1_num]);
						zdev_set.bdevtask1_num++;
							break;
						}
						}
					}
				
					}else if(zdev_set.bbinddev.bdev_taskid ==2){
						
						for(j =0;j<100;j++){
						if(0==(memcmp(&zdev_set.bbinddev.bdev_id[1][j][0], &data[head_len +2],12))){
						
						 printf("bdev id is in\r\n");
						break;
						}else{
							
						if(zdev_set.bbinddev.bdev_id[1][j][0] == 0 && zdev_set.bbinddev.bdev_id[1][j][1] == 0 && zdev_set.bbinddev.bdev_id[1][j][2] ==0 &&zdev_set.bbinddev.bdev_id[1][j][3] ==0){ 
						memcpy(bdev_id_temp_buf,&data[head_len +2],12);
						memcpy(&zdev_set.bbinddev.bdev_id[1][zdev_set.bdevtask2_num][0], &data[head_len +2],12);
						zdev_set.bbinddev.bdev_id[1][zdev_set.bdevtask2_num][12] = '2' ;
						printf("bdev id is1 %s \r\n",zdev_set.bbinddev.bdev_id[1][zdev_set.bdevtask2_num]);
						zdev_set.bdevtask2_num++;
							break;
						}
						}
						
					
						
					}
		
		}
		}else{
				zdev_set.bbinddev.bdev_taskid = data[head_len +13] - '0';
				memcpy(zdev_set.other_id,&data[head_len],DEVICE_ID_LEN);
				printf("zdev_set.other_id is %s \r\n",zdev_set.other_id);
				zdev_set.zdev_num = 2;
				if(zdev_set.bbinddev.bdev_taskid ==1){
					
					for(j =0;j<100;j++){
						if(0==(memcmp(&zdev_set.bbinddev.bdev_id[0][j][0], &data[head_len +15],12))){
							printf("bdev id is in\r\n");
							break;
						}else{
								if(zdev_set.bbinddev.bdev_id[0][j][0] == 0 && zdev_set.bbinddev.bdev_id[0][j][1] == 0 && zdev_set.bbinddev.bdev_id[0][j][2] ==0 &&zdev_set.bbinddev.bdev_id[0][j][3] ==0){
						memcpy(&zdev_set.bbinddev.bdev_id[0][zdev_set.bdevtask1_num][0], &data[head_len +15],12);
						zdev_set.bbinddev.bdev_id[0][zdev_set.bdevtask1_num][12] = '1' ;
					memcpy(bdev_id_temp_buf,&data[head_len +15],12);
					printf("bdev id is4 %s \r\n",zdev_set.bbinddev.bdev_id[0][zdev_set.bdevtask1_num]);
						zdev_set.bdevtask1_num++;
						break;
								}
						}
					}
					}else if(zdev_set.bbinddev.bdev_taskid ==2){
						for(j =0;j<100;j++){
						if(0==(memcmp(&zdev_set.bbinddev.bdev_id[1][j][0], &data[head_len +15],12))){
							printf("bdev id is in\r\n");
							break;
						}else{
								if(zdev_set.bbinddev.bdev_id[1][j][0] == 0 && zdev_set.bbinddev.bdev_id[1][j][1] == 0 && zdev_set.bbinddev.bdev_id[1][j][2] ==0 &&zdev_set.bbinddev.bdev_id[1][j][2] ==0){ 
						memcpy(bdev_id_temp_buf,&data[head_len +15],12);
						memcpy(&zdev_set.bbinddev.bdev_id[1][zdev_set.bdevtask2_num][0], &data[head_len +15],12);
						zdev_set.bbinddev.bdev_id[1][zdev_set.bdevtask2_num][12] = '2' ;
						printf("bdev id is5 %s \r\n",zdev_set.bbinddev.bdev_id[1][zdev_set.bdevtask2_num]);
						zdev_set.bdevtask1_num++;
							break;
								}
						}
						}
					}
		
		}
	}	
	


extern void send_wlist_to_ble(void);
	
uint8_t command_type = 0xff;

void set_command_type(uint8_t command)
{
		command_type = command;
}
uint8_t get_command_type(void)
{
		return command_type ;
}
	

/*************group is 0-x***********/
	
extern osTimerId LedTimerHandle;
extern uint8_t devinfo_flag ;

uint8_t test_begin(uint8_t *data, uint16_t len, uint8_t idx)
{
	
	
	
	task_flag_start =1;
	contiue_task =1;
	
	save_nv_buf[0] = '1';
	if(zdev_set.isMdev ==1){
		save_nv_buf[1] = '1';
	}else if(zdev_set.isMdev ==0 ){
		save_nv_buf[1] = '0';
	}
	
	get_bdev_group_num();
	save_nv_buf[69] =  bbind_num;
	
	copy_addr_group();
	

  set_command_type(0xf1);
	if(!get_timer_status()){
				 osTimerStart(LedTimerHandle, 3500);
					set_timer_status(1);
	}
	save_dev_addr();
	
}
uint8_t task_send_count =1;

void send_bbind_command(void)
{
		uint8_t commnd_buf[50];
  uint8_t totallen1 =0;
		totallen1 = strlen("taskstart:");
	memcpy(commnd_buf,"taskstart:",totallen1);
	

	
	printf("group_num_data = %d \r\n",group_num_data);

	
		memcpy(&commnd_buf[totallen1],zdev_set.mydev_id,12);
		totallen1 +=12;
	commnd_buf[totallen1] = ',';
	totallen1 +=1;
	
	if(zdev_set.zdev_num ==2){
	
		memcpy(&commnd_buf[totallen1],zdev_set.other_id,12);
		totallen1 += 12;
	
	
	}else{
		
			memcpy(&commnd_buf[totallen1],"000000000000",12);
			totallen1 += 12;
	}
	
	commnd_buf[totallen1] = ',';
	totallen1 +=1;
	
		commnd_buf[totallen1] = '0';
	totallen1 +=1;
	
		
		commnd_buf[totallen1] = taskidnum + '0';
			
			totallen1 +=1;
	
		commnd_buf[totallen1] = ',';
	  totallen1 +=1;
	
	   commnd_buf[totallen1] = task_send_count/10 +'0';
			totallen1 += 1;
			commnd_buf[totallen1] = task_send_count%10 +'0';
			totallen1 += 1;
	   commnd_buf[totallen1] = ',';
	  totallen1 +=1;
	   memcpy(&commnd_buf[totallen1],"01",2);
			totallen1 += 2;
			 memcpy(&commnd_buf[totallen1],"##",2);
			totallen1 += 2;
			printf("task start commnd_buf %s \r\n",commnd_buf);
				get_bdev_group_num();
			if(task_send_count<group_num_data){
					task_send_count ++;
			  }else {
				task_send_count = 1;
				}
	
		   ble_send_response(commnd_buf, totallen1);	
}
uint8_t air_command_send_count =1;
extern uint8_t save_flas;
void send_airmode_command(void)
{
	uint8_t buf[30],flag =0;
	uint8_t lenth,lenth1 = 0;

	lenth1 = strlen("airplanemode:");
	get_bdev_group_num();

		memcpy(blebuf,"airplanemode:",lenth1);
		memcpy(&blebuf[lenth1],zdev_set.mydev_id,12);
		lenth1 +=12;
		blebuf[lenth1] = ',';
		lenth1 +=1;
		if(zdev_set.other_id[0] == '1' &&zdev_set.other_id[1] == '0'){
		memcpy(&blebuf[lenth1],zdev_set.other_id,12);
		}
		else
		{
		memcpy(&blebuf[lenth1],"000000000000",12);
		}
		lenth1 +=12;
		
		blebuf[lenth1] = ',';
		lenth1 +=1;
		
		blebuf[lenth1] = '0';
		lenth1 +=1;
		
		blebuf[lenth1] = '1';
		lenth1 +=1;
		
		blebuf[lenth1] = ',';
		lenth1 +=1;
		group_location_count = lenth1;
		
		get_bdev_group_num();
		
		
		
		
		blebuf[lenth1] = air_command_send_count /10 + '0';
		lenth1 +=1;
		
		blebuf[lenth1] = air_command_send_count %10 +'0';
		lenth1 +=1;
		
		memcpy(&blebuf[lenth1],"##",2);
		lenth1 +=2;
		ble_send_response(blebuf,lenth1);
		if(air_command_send_count <group_num_data){
			air_command_send_count++;
		}else{
			air_command_send_count =1;
			set_command_type(0xff);
			save_flas =1;
	}
			
	
		


}
uint8_t ubind_command_count =1;
extern uint8_t ubind_all_flag;
void send_ubind_command(void)
{
			uint8_t response[50];
			uint8_t totallen =0;
				get_bdev_group_num();
		    //if(ubind_command_count ==group_num_data){
		         //bbind_num =0;
					//	return ;
		     //}
		       memcpy(response,"ubind:",6);
					totallen +=6;
				
		      memcpy(&response[totallen],zdev_set.mydev_id,12);
					totallen +=12;
		       memcpy(&response[totallen],",",1);
					 totallen +=1;
				  if(zdev_set.other_id[0] == '1' && zdev_set.other_id[1] =='0'){
					memcpy(&response[totallen],zdev_set.other_id,12);
					}else{
					 memcpy(&response[totallen],"000000000000",12);
					
				  }
					totallen +=12;
		       memcpy(&response[totallen],",",1);
					 totallen +=1;
		      memcpy(&response[totallen],"00",2);
					totallen +=2;
					memcpy(&response[totallen],",",1);
					 totallen +=1;
				
					
					
					response[totallen] = ubind_command_count/10 + '0';
					totallen+=1;
					response[totallen] = ubind_command_count%10 + '0';
					
					totallen+=1;
					
		       memcpy(&response[totallen],"##",2);
					
					 totallen +=2;
		
				  ble_send_response(response, totallen);
					printf("ubind send ble :%s \r\n",response);
			     get_bdev_group_num();
					if(ubind_command_count <group_num_data){
						ubind_command_count++;
					}else{
							ubind_command_count =1;
						set_command_type(0xff);
						bbind_num= 0;
						if(get_timer_status()){
						osTimerStop(LedTimerHandle);
						set_timer_status(0);
						ubind_all_flag =1;
						}
					}
					
				
					

}


uint8_t call_flag=0;
//extern osTimerId ReportTimerHandle;
void LedTimerCallback(void const * argument)
{
uint8_t cmd;
	 
	if(call_flag ==1){
		if(task_flag_start ==1)
		   set_command_type(0xf1);	
	    send_light_commnd =0;
			call_flag =0;
	 }
	 if(send_light_commnd ==1){
				set_command_type(0xff);
				call_flag++;
	 }else if(send_light_commnd ==2){
					set_command_type(0xff);
					call_flag++;
	 }
	 cmd = get_command_type();
	printf("LedTimerCallback cmd = %x \r\n",cmd);
	switch(cmd){
		case 0xf1:
			send_bbind_command();
			break;
		case 0xf2:
			send_airmode_command();
			break;
		case 0xf3:
			//osTimerStop(ReportTimerHandle);
			send_ubind_command();
			break;
		
		default:
				break;
	 }

	


}


//+WARINGSTATUS=ON,OK##\r\n; 
uint8_t aoa_at_handle_close_bdev_report(uint8_t *data, uint16_t len, uint8_t idx)
{
			uint8_t lenth = 0,lenth1 = 0;
			lenth = strlen("AT+WARINGSTATUS=");
			uint8_t temp_buf[15],temp_buf2[25];
			memcpy(temp_buf,"bdclose:",8);
			lenth1 = 8;
			printf(" WARINGSTATUS data %s lenth %d\r\n ",data,lenth);
			if((data[lenth] == 'O')&&(data[lenth+1] == 'F')){
				memcpy(&temp_buf[lenth1],"off",3);
				lenth1 +=3;
				memcpy(temp_buf2,"+WARINGSTATUS:OFF,OK",20);
				memcpy(&temp_buf2[20],aoa_at_end_tok,AT_END_TOK_LEN);
				
				aoa_send_response(temp_buf2,24);
				printf(" WARINGSTATUS data %s \r\n ",temp_buf2);
			}else if((data[lenth] == 'O')&&(data[lenth+1] == 'N')){
			
				memcpy(&temp_buf[lenth1],"on",2);
				lenth1 +=2;
				memcpy(temp_buf2,"+WARINGSTATUS:ON,OK",19);
				
				memcpy(&temp_buf2[19],aoa_at_end_tok,AT_END_TOK_LEN);
				aoa_send_response(temp_buf2,23);
			
				printf(" WARINGSTATUS data %s \r\n ",temp_buf2);
			}
			
		ble_send_response(temp_buf, lenth1);
}

uint8_t backup_buf[50]; //= "AT+BBIND=1,132009220010,18001314967##";
uint8_t apn_buf[80];
uint8_t bind_command_buf_bak[100];
uint16_t bind_command_len_bak;
uint8_t wlist_continue_send_flag =0;
uint8_t zdev_bind_flag =0;

uint8_t new_dev = 0;

extern uint8_t send_wl_temp[100][15];
extern uint8_t devinfo_start_flag;
// bbind=1,2,2,223456,000000000000,109988998889##
uint8_t aoa_at_handle_bbind(uint8_t *data, uint16_t len, uint8_t idx)
{
//	osTimerStop(ReportTimerHandle);	
	uint16_t totallen=0,lenth = 0,lenth1;
	//uint8_t send_buf[100];
	uint8_t backbuf[40],bdadd_buf[80];
	uint8_t count =0,i =0;
	uint8_t count1 =0;
	uint8_t backlen  = 0;
  uint8_t group_num = 0;
	
	
	
	
	
	devinfo_start_flag =0;
	if(bbind_num >100)
		return 0;
	
		if(task_flag_start && get_timer_status()){
	    osTimerStop(LedTimerHandle);
			set_timer_status(0);
			set_command_type(0xff);
		}
		
		
		group_num = (bbind_num / BDEVICE_GROUP_NUM)+1;
	printf("group_num = %d \r\n",group_num );
		
		
	for(i = 0;i<100;i++){
	   if(send_wl_temp[i][0] == '1'){
			 if(memcmp(send_wl_temp[i],&data[11],12) ==0){
					new_dev =1;
					group_num = send_wl_temp[i][13];
				 break;
			 }
					
		 }else{
		      continue;
		 }
	
	}

	printf("new_dev = %d \r\n",new_dev );
	
	
	
	
	zdev_set.isMdev =1;
	zdev_bind_flag =1 ;
	
	
		save_nv_buf[1] = '1';
	
		printf("*********test6*******\r\n");
//		set_nvram_save_data(save_nv_buf);
	printf("*********test7******\r\n");
//	osTimerStop(ReportTimerHandle);
	//backlen = strlen("AT+BBIND=");
	memset(apn_buf,0x00,50);
	memcpy(backup_buf,data,len);
	hw_bd2_close();
	printf("*********test8*******\r\n");
	//	set_nvram_save_data(save_nv_buf);
		printf("*********test9*******\r\n");
	lenth = strlen("bbind:");
	lenth1 = lenth;
	//memcpy(apn_buf,"bbind=",lenth);
	totallen +=lenth;
//AT+BBIND=1,112019060053,19910232427##
	memcpy(bind_command_buf_bak,data,len);
	bind_command_len_bak = len;
	while(data[count]){
			if(data[count] == ','){
					count1++;
			}
			if(count1 ==1){
					if((data[count-2] == '=')&&(data[count-1] =='1' ||data[count-1]=='2'))
					{
						memcpy(bdadd_buf,"bbind:",lenth1);
					memcpy(&bdadd_buf[lenth1],&data[count+1],12);
						
					memcpy(send_wl_temp[bbind_num],&data[count+1],12);
					
						lenth1 +=12;
					//memcpy(&bdadd_buf[19],"##",2);
					taskidnum = data[count-1] - '0';
					send_wl_temp[bbind_num][12] = taskidnum; 
						printf("copy addr1 %s %d \r\n",send_wl_temp[bbind_num],send_wl_temp[bbind_num][12]);
						
					if(data[count+14] == '1')
						memcpy(&apn_buf[totallen],&data[count+14],11);
					else
						memcpy(&apn_buf[totallen],"00000000000",11);
					//ble_send_response(bdadd_buf, 21);
						break;
					}
			}
			if(count1 ==2){
					
					memcpy(bdadd_buf,"bbind:",lenth1);
					memcpy(&bdadd_buf[lenth1],&data[count+1],12);
					memcpy(send_wl_temp[bbind_num],&data[count+1],12);
				
					lenth1 +=12;
					taskidnum = data[count-1] - '0';
				  send_wl_temp[bbind_num][12] = taskidnum; 
				printf("copy addr2 %s %d \r\n",send_wl_temp[bbind_num],send_wl_temp[bbind_num][12]);
					//memcpy(&bdadd_buf[19],"##",2);
					if(data[count+14] == '1')
						memcpy(&apn_buf[totallen],&data[count+14],11);
					else
						memcpy(&apn_buf[totallen],"00000000000",11);
				 // ble_send_response(bdadd_buf, 21);
				break;
			}
	
		count++;
	}
	
  if(!new_dev)
		send_wl_temp[bbind_num][13] = group_num; 
	
	printf("mac group_nu %s %d \r\n",send_wl_temp[bbind_num],send_wl_temp[bbind_num][13]);
	
	if(new_dev){
			new_dev =0;
	}else{
		bbind_num ++;
	}	
	
bdadd_buf[lenth1] = ',';
lenth1+=1;
	
	
memcpy(&bdadd_buf[lenth1],zdev_set.mydev_id,12);
lenth1+=12;
	
bdadd_buf[lenth1] = ',';
lenth1+=1;
	
	
	if(zdev_set.zdev_num ==2){
	
		
		memcpy(&bdadd_buf[lenth1],zdev_set.other_id,12);
   lenth1+=12;
	
		
	memcpy(&apn_buf[totallen],zdev_set.other_id,12);
	totallen += 12;
	
	apn_buf[totallen] = ',';
	totallen +=1;
	}else{
		
			memcpy(&bdadd_buf[lenth1],"000000000000",12);
   lenth1+=12;
		
			memcpy(&apn_buf[totallen],"000000000000",12);
	totallen += 12;
	
	apn_buf[totallen] = ',';
	totallen +=1;
		
		
		
	}
			bdadd_buf[lenth1] = ',';
			lenth1+=1;
	
	bdadd_buf[lenth1] = '0';
			lenth1+=1;

	lenth = strlen(zdev_set.mydev_id);
	memcpy(&apn_buf[totallen],zdev_set.mydev_id,12); //调试注释掉 20190529
	//memcpy(&send_buf[totallen],"112233998877",12);
	totallen += 12;
	if((taskidnum ==1) || (taskidnum ==2)){
		
		bdadd_buf[lenth1] = taskidnum + '0';
			lenth1+=1;
			apn_buf[totallen] = taskidnum + '0';
	//send_buf[totallen] = ',';
	totallen +=1;
	}


bdadd_buf[lenth1] = ',';
			lenth1+=1;

if(group_num <10){
	 bdadd_buf[lenth1] = '0';
   lenth1+=1;
   bdadd_buf[lenth1] = group_num + '0';
   lenth1+=1;
}else{
  
   bdadd_buf[lenth1] = (group_num /10) +'0';
   lenth1+=1;
   bdadd_buf[lenth1] = (group_num%10) + '0';
   lenth1+=1;
}
bdadd_buf[lenth1] = ',';
			lenth1+=1;
	memcpy(&bdadd_buf[lenth1],"15",2);
   lenth1+=2;
bdadd_buf[lenth1] = ',';
			lenth1+=1;
	memcpy(&bdadd_buf[lenth1],"01",2);
   lenth1+=2;
	
	memcpy(&bdadd_buf[lenth1],"##",2);
   lenth1+=2;
	
	ble_send_response(bdadd_buf, lenth1);
	
	//ble_send_response(apn_buf, totallen);
		copy_addr_group();
if(task_flag_start &&(!get_timer_status())){
	 osTimerStart (LedTimerHandle, 3500);
	set_timer_status(1);
	
}
	if(task_flag_start){
		set_command_type(0xf1);
	}
	memset(backbuf,0x00,40);
	memcpy(backbuf,"+BBIND:",7);
	memcpy(&backbuf[7],&data[9],14);	
	memcpy(&backbuf[21],",OK",3);	
	memcpy(&backbuf[24],aoa_at_end_tok,AT_END_TOK_LEN);	
 
	if(task_flag_start)
			devinfo_start_flag =1;
	
	printf("bbind send_buf %s %d\r\n",bdadd_buf,lenth1);
	
}
 
uint8_t ubind_back[50];
uint8_t ubind_back_err[50];
uint8_t ubind_len =0;
uint8_t ubind_len_err =0;
extern uint8_t send_wl_temp[100][15];
uint8_t ubind_flag = 0;
extern uint8_t sendid_count;
//uint8_t response_ubind[32];
extern uint8_t bind_bak_buf[50];
extern uint8_t devinfo_start_flag ;
uint8_t aoa_at_handle_bunbind(uint8_t *data, uint16_t len, uint8_t idx)
{
	uint16_t totallen = 0,lenth,i,totallen_bak;
	uint8_t response[52];
	uint8_t unbind_id_buf[12];
	


	lenth=strlen("AT+BUNBIND=");
printf("aoa_at_handle_bunbind \r\n");
	#if 1
	switch(data[lenth]){
		case '0':
			
				task_flag_start=0;
		    devinfo_start_flag =0;
						//osTimerStop(LedTimerHandle);
				memset(bind_bak_buf,0x00,50);
					task_flag_start=0;
						save_nv_buf[0] = '0';
		      // memcpy(response,"ubind:",6);
		     if(!get_timer_status()){
			   osTimerStart(LedTimerHandle,3500);
		     set_timer_status(1);
		   }
				devinfo_flag =0;
					zdev_isbind=0;
					//memset(zdev_set.bbinddev.bdev_id,0x00,sizeof(zdev_set.bbinddev.bdev_id));
					//memset(&zdev_set.bbinddev.bdev_id[0][0][0],0x00,1300);
					//memset(&zdev_set.bbinddev.bdev_id[1][0][0],0x00,1300);
					//memset(save_nv_buf,0x00,70);
					//if(zdev_set.isMdev ==1)
				 
					//set_nvram_save_data(save_nv_buf);
					zdev_set.bdevtask1_num =0;
					zdev_set.bdevtask2_num = 0;
					set_ble_red_light(1);
 					save_nv_buf[0] = '0';
					//set_nvram_save_data(save_nv_buf);
					ubind_flag = 0;
					clear_dev_nv();
					memset(send_wl_temp,0x00,1500);
					set_command_type(0xf3);
					//bbind_num =0;
					printf("ubind0  %x  \r\n",command_type);
					//BatTimerCallback();
					//osTimerStop(LedTimerHandle);
		
			break;
		case '1':
			memcpy(response,"subind:",7);
					totallen +=7;
		  
						memset(bind_bak_buf,0x00,50);
							set_command_type(0xff);
		        send_light_commnd =1;
					if(data[lenth+1] != ','){

							//response[totallen] = '1';
							//totallen +=1;
							//memcpy(&response[totallen],aoa_at_end_tok,AT_END_TOK_LEN);
							//totallen+=AT_END_TOK_LEN;
							memset(zdev_set.bbinddev.bdev_id[0],0x00,1300);
					}else{
						
						
							//response[totallen] = '1';
							//totallen +=1;
							//response[totallen] = ',';
							//totallen +=1;
							memcpy(&response[totallen],&data[lenth+2],12);
							totallen +=12;
						response[totallen] = ',';
							totallen +=1;
						
					
							memcpy(unbind_id_buf,&data[lenth+2],12);
						
							
							memcpy(&response[totallen],"01",2);
							totallen+=2;
							response[totallen] = ',';
							totallen +=1;
						
						get_bdev_group_num();
					
					response[totallen] = group_num_data/10 + '0';
					totallen+=1;
					response[totallen] = group_num_data%10 + '0';
					
					totallen+=1;
					
		       memcpy(&response[totallen],"##",2);
					 totallen +=2;
						
						 
		
							for(i =0 ;i<100;i++){
								printf("send_wl_temp %s  \r\n",send_wl_temp[i]);
										if(0 == memcmp(unbind_id_buf,send_wl_temp[i],12)){
												printf("unbind_id_buf %s %s \r\n",unbind_id_buf,send_wl_temp[i]);
													memset(send_wl_temp[i],0x00,15);
											if(bbind_num >= 1)
												   bbind_num-=1;
										}
										
							
							}
							
		//					save_dev_addd_nv();
					}
						if((zdev_set.bdevtask2_num == 0) && (zdev_set.bdevtask1_num == 0))
							set_ble_red_light(1);
						
						ubind_flag = 1;
//						save_dev_addd_nv();
						
						copy_addr_group();
						
					//if(zdev_set.isMdev ==1)
					ble_send_response(response, totallen);
						printf("subind1  %s  \r\n",response);
		
			break;
		case '2':
			memcpy(response,"subind:",7);
					totallen +=7;
							memset(bind_bak_buf,0x00,50);
							send_light_commnd=1;
							set_command_type(0xff);
		        
					if(data[lenth+1] != ','){

							//response[totallen] = '2';
							//totallen +=1;
						//	memcpy(&response[totallen],aoa_at_end_tok,AT_END_TOK_LEN);
						//	totallen+=AT_END_TOK_LEN;
							memset(zdev_set.bbinddev.bdev_id[1],0x00,1300);
					}else{
							memcpy(&response[totallen],&data[lenth+2],12);
						  	totallen +=12;
							response[totallen] = ',';
							totallen +=1;
							
							memcpy(unbind_id_buf,&data[lenth+2],12);
						
							memcpy(&response[totallen],"02",2);
							totallen+=2;
						
						response[totallen] = ',';
							totallen +=1;
						
						get_bdev_group_num();
					
					response[totallen] = group_num_data/10 + '0';
					totallen+=1;
					response[totallen] = group_num_data%10 + '0';
					
					totallen+=1;
					
		       memcpy(&response[totallen],"##",2);
					 totallen +=2;
		
				// ble_send_response(response, totallen);
						
					
						
					
							for(i =0 ;i<100;i++){
										printf("send_wl_temp %s  \r\n",send_wl_temp[i]);
										if(0 == memcmp(unbind_id_buf,&send_wl_temp[i][0],12)){
											printf("unbind_id_buf %s %s \r\n",unbind_id_buf,send_wl_temp[i]);
													memset(send_wl_temp[i],0x00,15);
												if(bbind_num >= 1)
													   bbind_num-=1;
										}
							
							}
//							save_dev_addd_nv();
					}
					printf("response is %s \r\n",response);
					if((zdev_set.bdevtask2_num == 0) && (zdev_set.bdevtask1_num == 0))
							set_ble_red_light(1);
					
					ubind_flag = 2;
					//if(zdev_set.isMdev ==1)
					copy_addr_group();
					
					ble_send_response(response, totallen);
					printf("subind2  %s  \r\n",response);
			break;
				
			
			defualt:
					break;
	
	
	
	}
	#endif
	//+BUNBIND:
	if(data[lenth] =='0' &&data[lenth+1] =='#'){
		printf("AT+BUNBIND=0 \r\n");
		memset(ubind_back,0x00,50);
				memcpy(ubind_back,"+BUNBIND:",9);
		memcpy(&ubind_back[9],"0,OK",4);
	
		memcpy(&ubind_back[13],aoa_at_end_tok,AT_END_TOK_LEN);
		ubind_len =17;
	
		aoa_send_response(ubind_back,ubind_len);
			//memcpy(ubind_back_err,"+BUNBIND:",9);
	//	memcpy(&ubind_back_err[9],&data[lenth],len-15);
	//	memcpy(&ubind_back_err[len-6],",ERROR,1",8);
		//memcpy(&ubind_back_err[len+2],aoa_at_end_tok,AT_END_TOK_LEN);
		//ubind_len_err =len+6;
	
		memset(zdev_set.bbinddev.bdev_id ,0x00,sizeof(zdev_set.bbinddev.bdev_id));
		zdev_set.bdevtask1_num = 0;
		zdev_set.bdevtask2_num = 0;

	}else{
	memset(ubind_back,0x00,50);
		memcpy(ubind_back,"+BUNBIND:",9);
		memcpy(&ubind_back[9],&data[lenth],len-15);
		memcpy(&ubind_back[len-6],",OK",3);
		memcpy(&ubind_back[len-3],aoa_at_end_tok,AT_END_TOK_LEN);
		ubind_len =len+1;
	  aoa_send_response(ubind_back,ubind_len);
		printf("ubind_back  %s  \r\n",ubind_back);
	
		memcpy(ubind_back_err,"+BUNBIND:",9);
		memcpy(&ubind_back_err[9],&data[lenth],len-15);
		memcpy(&ubind_back_err[len-6],",ERROR,1",8);
		memcpy(&ubind_back_err[len+2],aoa_at_end_tok,AT_END_TOK_LEN);
		ubind_len_err =len+6;
	//aoa_send_response(ubind_back_err,ubind_len_err);
	
	}
		
}


/*
AT+BBEAT=111234567890123,121234567890124
+BBEAT:111234567890123,10,1,ON,ON,1,,121234567890124,20,4,OFF,OFF,0,OFF
+BBEAT:1,111234567890123,OK,10,1,ON,ON,1,,121234567890124,OK,20,4,OFF,OFF,0,OFF
*/
uint16_t make_bbeat_response(uint8_t *response)
{
	uint16_t totallen=0,len=0;
	uint8_t data;
	uint8_t str[10]={0};
	
	len=strlen("+BBEAT:");
	memcpy(response,"+BBEAT:",len);
	totallen+=len;
	
	memcpy(response,"1",1);
	totallen+=len;
	
add_separator_comma(&response[totallen]);
		totallen+=1;
	
	len=strlen("01234567898");
	memcpy(response,"01234567898",len);
	totallen+=len;
	
add_separator_comma(&response[totallen]);
		totallen+=1;
	
	len=strlen("OK");
	memcpy(response,"OK",len);
	totallen+=len;

	memcpy(&response[totallen],aoa_at_end_tok,AT_END_TOK_LEN);
	totallen+=AT_END_TOK_LEN;

	return totallen;
}


uint8_t aoa_at_handle_bbeat(uint8_t *data, uint16_t len, uint8_t idx)
{
	/*uint16_t totallen;
	uint8_t response[128];*/

	osStatus ret;
	ret=task_ble_mail_put(data, len);
	return ((uint8_t)(ret));
	
	/*totallen=make_bbeat_response(response);
	return aoa_send_response(response,totallen);*/
}

uint8_t aoa_at_handle_binfowrite(uint8_t *data, uint16_t len, uint8_t idx) //需要修改 需要去连接bbind
{
	osStatus ret;
	//ret=task_ble_mail_put(data, len);
	uint16_t totallen=0, lenth = 0,lenth1 = 0;
	uint8_t id1,id2;
	uint8_t response[128];
	
	lenth = strlen("AT+BINFOWRITE=");
	if(data[lenth] == '1'){
			id1 = 1;	
			memcpy(zdev_set.bbinddev.bdev_id[0][zdev_set.bdevtask1_num],&data[lenth+2],12);
			zdev_set.bbinddev.bdev_id[0][zdev_set.bdevtask1_num][12] ='1';
			zdev_set.bdevtask1_num +=1;
	}else if(data[lenth] =='2'){
			id2 =2;
		memcpy(zdev_set.bbinddev.bdev_id[1][zdev_set.bdevtask2_num],&data[lenth+2],12);
		zdev_set.bbinddev.bdev_id[1][zdev_set.bdevtask2_num][12] ='2';
		zdev_set.bdevtask2_num +=1;		
	}

	
	
	
	lenth1=strlen("+BINFOWRITE:");
	memcpy(response,"+BINFOWRITE:",lenth1);
	totallen+=lenth1;
	
	memcpy(&response[totallen],&data[lenth],1);
	totallen +=1;
	
	add_separator_comma(&response[totallen]);
		totallen+=1;
	memcpy(&response[totallen],&data[lenth+2],12);
	totallen +=12;
	
add_separator_comma(&response[totallen]);
		totallen+=1;
	
	memcpy(&response[totallen],"OK",2);
	totallen +=2;
	
//	add_separator_comma(&response[totallen]);
	//	totallen+=1;
	///	memcpy(&response[totallen],"1",1);
	//totallen +=1;
	memcpy(&response[totallen],aoa_at_end_tok,AT_END_TOK_LEN);
	totallen+=AT_END_TOK_LEN;
	aoa_send_response(response,totallen);
	return ((uint8_t)(ret));
}

uint8_t aoa_at_handle_bbbeat(uint8_t *data, uint16_t len, uint8_t idx)
{
	osStatus ret;
	ret=task_ble_mail_put(data, len);
	return ((uint8_t)(ret));
}

uint8_t aoa_at_handle_blight(uint8_t *data, uint16_t len, uint8_t idx) //需要改
{
	osStatus ret;
	//ret=task_ble_mail_put(data, len);
	
	
	uint16_t totallen=0,lenth=0,lenth1 = 0;
	uint8_t id1,id2;
	uint8_t response[128];
	uint8_t call_buf[25];
	
	
	lenth = strlen("AT+BLIGHT=");
	


	memcpy(call_buf,"call:",5);
	
	lenth1=strlen("+BLIGHT:");
	memcpy(response,"+BLIGHT:",lenth1);
	totallen+=lenth1;
	
	memcpy(&response[totallen],&data[lenth],15);
	totallen +=15;
	
	
	memcpy(&call_buf[5],&data[lenth+2],12);
	
	call_buf[17] = ',';
	call_buf[18] = '0';
	memcpy(&call_buf[19],&data[lenth],1);
	
		call_buf[20] = ',';
		
	
	//printf("light %c \r\n",data[lenth+15]);
	if(data[26] == 'N'){
		 memcpy(&call_buf[21],"01",2);	
		light_flag = 1;
			memcpy(&response[totallen],"ON",2);
	totallen +=2;
			send_light_commnd=1;
	}
	if(data[26] == 'F'){
			light_flag = 0;
			send_light_commnd=2;
		 memcpy(&call_buf[21],"00",2);	
			memcpy(&response[totallen],"OFF",3);
	totallen +=3;
	}
	
	  memcpy(&call_buf[23],"##",2);	
		ble_send_response(call_buf,24);
	
		memcpy(&response[totallen],",",1);
	totallen +=1;
	
	memcpy(&response[totallen],"OK",2);
	totallen +=2;
	
	memcpy(&response[totallen],aoa_at_end_tok,AT_END_TOK_LEN);
	totallen+=AT_END_TOK_LEN;
	
	ret = aoa_send_response(response,totallen);
	return ((uint8_t)(ret));
}

uint8_t aoa_at_handle_binfoerase(uint8_t *data, uint16_t len, uint8_t idx) //need to 2640
{
	osStatus ret;
	//ret=task_ble_mail_put(data, len);
	uint16_t totallen=0,lenth=0,lenth1 = 0;
	uint8_t id1,id2;
	uint8_t response[128];
	
	
	lenth = strlen("AT+BINFOERASE=");
	
	
	lenth1=strlen("+BINFOERASE:");
	memcpy(response,"+BINFOERASE:",lenth1);
	totallen+=lenth1;
	
	memcpy(&response[totallen],&data[lenth],1);
	totallen +=1;
	
	add_separator_comma(&response[totallen]);
		totallen+=1;
	memcpy(&response[totallen],&data[lenth+2],12);
	totallen +=12;
	
add_separator_comma(&response[totallen]);
		totallen+=1;
	memcpy(&response[totallen],"OK",2);
	totallen +=2;
	//add_separator_comma(&response[totallen]);
	//	totallen+=1;
		//memcpy(&response[totallen],"1",1);
	//	totallen+=1;
	memcpy(&response[totallen],aoa_at_end_tok,AT_END_TOK_LEN);
	totallen+=AT_END_TOK_LEN;
	
	memset(zdev_set.bbinddev.bdev_id,0x00,sizeof(zdev_set.bbinddev.bdev_id));
	zdev_set.bdevtask1_num = 0;
	zdev_set.bdevtask2_num = 0;
	
	ret = aoa_send_response(response,totallen);
	return ((uint8_t)(ret));
}

uint8_t aoa_at_handle_centeraddress(uint8_t *data, uint16_t len, uint8_t idx)
{
	osStatus ret;
	ret=task_ble_mail_put(data, len);
	return ((uint8_t)(ret));
}

uint8_t aoa_at_handle_bsosinfo(uint8_t *data, uint16_t len, uint8_t idx)
{
	//osStatus ret;
	//ret=task_ble_mail_put(data, len);
	///return ((uint8_t)(ret));
	uint8_t lenth,lenth1;
	uint8_t temp_buf[20];
	
		lenth = strlen("AT+BSOSINFO=");
		
	  
								memset(temp_buf,0x00,20);
								lenth1 = strlen("+BSOSINFO:OK");
								memcpy(temp_buf,"+BSOSINFO:OK",lenth1);
								memcpy(&temp_buf[lenth1],aoa_at_end_tok,4);
								zdev_set.report_mesage_id = data[lenth] -'0';
								aoa_send_response(temp_buf,lenth1+4);
		
	
			
	
	
}

uint8_t aoa_at_handle_bsos(uint8_t *data, uint16_t len, uint8_t idx)
{
	osStatus ret;
	ret=task_ble_mail_put(data, len);
	return ((uint8_t)(ret));
}

/*todo, 20181114*/
uint8_t handle_zdevice_selfcheck(uint8_t idx)
{
}

uint8_t handle_bdevice_selfcheck(uint8_t *data, uint16_t len, uint8_t idx)
{
	osStatus ret;
	ret=task_ble_mail_put(data, len);
	return ((uint8_t)(ret));
}


uint8_t aoa_at_handle_selfcheck(uint8_t *data, uint16_t len, uint8_t idx)
{
	ZbTokenizer  tzer[1];
	ZbToken  tok_device_id;
	uint16_t headerlen=0;
	uint8_t   deviceid[DEVICE_ID_LEN],res;

	headerlen=strlen(aoa_at_table[idx].pData);
	zb_tokenizer_init(tzer, data+headerlen, data+len);
	get_zdevice_id(deviceid);

	if(tzer[0].count==2)//maybe zdevice
	{
		tok_device_id = zb_tokenizer_get(tzer,0);
		if(0==memcmp(deviceid,tok_device_id.p,DEVICE_ID_LEN))
		{
			res=handle_zdevice_selfcheck(idx);
		}
		else
		{
			res=handle_bdevice_selfcheck(data, len, idx);
		}
	}
	else
	{
		res=handle_bdevice_selfcheck(data, len, idx);
	}
	return res;
}


uint8_t aoa_at_handle_bdistance(uint8_t *data, uint16_t len, uint8_t idx) //需要给2640
{
	 uint8_t lenth =0;
		uint8_t temp_buf[20];
		uint8_t ble_buf[11];
	
	lenth = strlen("AT+BDISTANCE=");

		if(data[lenth]!= '3' && data[lenth]!= '5' &&data[lenth]!= '1'){
		//	memcpy(temp);
				aoa_send_response("+BDISTANCE:ERROR##\r\n",20);
		}else{
					if(data[lenth]== '1'){
					memcpy(ble_buf,"bdrepd:",7);
					memcpy(&ble_buf[7],&data[lenth],2);
					memcpy(&ble_buf[9],"##",2);
					ble_send_response(ble_buf,11);
				zdev_set.report_dis = (data[lenth] -'0')*10 + (data[lenth+1] - '0');
				aoa_send_response("+BDISTANCE:OK##\r\n",17);
				}else if(data[lenth]== '3'){
				memcpy(ble_buf,"bdrepd:",7);
					memcpy(&ble_buf[7],&data[lenth],1);
					memcpy(&ble_buf[9],"##",2);
					ble_send_response(ble_buf,10);
				zdev_set.report_dis = (data[lenth] - '0');
				aoa_send_response("+BDISTANCE:OK##\r\n",17);
				
				}else if(data[lenth]== '5'){
				
				memcpy(ble_buf,"bdrepd:",7);
					memcpy(&ble_buf[7],&data[lenth],1);
					memcpy(&ble_buf[9],"##",2);
					ble_send_response(ble_buf,10);
				zdev_set.report_dis = (data[lenth] - '0');
				aoa_send_response("+BDISTANCE:OK##\r\n",17);
				}
			//set_nvram_report_dis(zdev_set.report_dis);
		}
	//ret=task_ble_mail_put(data, len);
	//return ((uint8_t)(ret));
}

/*todo, need nvram to save this value. 20181114*/
uint8_t handle_zdevice_blowbattery(uint8_t lowbat)
{
	//flash_status_t ret=0;
	//uint8_t ori_bat=0;

	//get_nvram_low_battery(&ori_bat);
	//if(ori_bat^lowbat)
	//{
	//	ret=set_nvram_low_battery(lowbat);
	//}
	//return (uint8_t)ret;
}

uint8_t handle_bdevice_blowbattery(uint8_t *data, uint16_t len, uint8_t idx)//需要给2640
{
	//osStatus ret;
	//ret=task_ble_mail_put(data, len);
	//return ((uint8_t)(ret));
	
	 uint8_t lenth =0, setbat=0xff;
		uint8_t temp_buf[30];
		uint8_t ble_buf[10];
	
	lenth = strlen("AT+BLOWBATTERY=");
			
		if(data[lenth]!= '0' && data[lenth]!= '2' &&data[lenth]!= '1'){ 
			memset(temp_buf,0x00,30);
			memcpy(temp_buf,"+BLOWBATTERY:ERROR",18);
			memcpy(&temp_buf[18],aoa_at_end_tok,4);
				aoa_send_response(temp_buf,22);
		}else{
				memcpy(ble_buf,"bdrepb:",7);
				zdev_set.report_bat_threshold = data[lenth] - '0';
				memcpy(&ble_buf[7],&data[lenth],1);
				memcpy(&ble_buf[8],"##",2);
				ble_send_response(ble_buf,10);
			
				memset(temp_buf,0x00,30);
				memcpy(temp_buf,"+BLOWBATTERY:OK",15);
				memcpy(&temp_buf[15],aoa_at_end_tok,4);
			printf("bat temp %s \r\n",temp_buf);
				aoa_send_response(temp_buf,19);
			//set_nvram_low_battery(zdev_set.report_bat_threshold);
		}
}

uint8_t aoa_at_handle_blowbattery(uint8_t *data, uint16_t len, uint8_t idx)
{
	uint8_t   res;
	#if 0
	ZbTokenizer  tzer[1];
	ZbToken battery;
	uint16_t headerlen=0;
	result,lowbat;

	headerlen=strlen(aoa_at_table[idx].pData);
	zb_tokenizer_init(tzer, data+headerlen, data+len);

	battery=zb_tokenizer_get(tzer, 0);
	lowbat=(uint8_t)str2int(battery.p, battery.end);

		
	if(lowbat>=0 && lowbat<=4)
	{
		if (0==handle_zdevice_blowbattery(lowbat))
			make_zdevice_okerror_response(0,0,0,idx);
		else
			make_zdevice_okerror_response(0,0,1,idx);
	}
	else
	{
		make_zdevice_okerror_response(0,0,1,idx);
	}
#endif
	handle_bdevice_blowbattery(data, len, idx);
	return res;
}

/*todo, need nvram to save this value. 20181114*/
uint8_t handle_zdevice_zreportloctime(uint32_t time_in_second)

{
	flash_status_t ret=0;
	uint32_t ori_data=0;

	//ret=get_nvram_report_period(&ori_data);
	//if(ori_data^time_in_second)
	//{
		//ret=set_nvram_report_period(time_in_second);
	//}
	//return (uint8_t)ret;
}

uint8_t aoa_at_handle_zreportloctime(uint8_t *data, uint16_t len, uint8_t idx)//需要改动
{
	
			uint8_t lenth,count=0,lenth1;
			uint8_t temp_buf[30];
	printf("aoa_at_handle_zreportloctime \r\n");
		lenth = strlen("AT+ZREPORTLOCTIME=");
	
	
		//	if(data[lenth] ==)
				if(data[lenth] != '2' &&data[lenth] != '5' && data[lenth] != '1' ){
				
								lenth1 = strlen("+ZREPORTLOCTIME:ERROR");
								memset(temp_buf,0x00,30);
								memcpy(temp_buf,"+ZREPORTLOCTIME:ERROR",lenth1);
								memcpy(&temp_buf[lenth1],aoa_at_end_tok,4);
								aoa_send_response(temp_buf,lenth1+4);
					}else{
								lenth1 = strlen("+ZREPORTLOCTIME:OK");
								memset(temp_buf,0x00,30);
								memcpy(temp_buf,"+ZREPORTLOCTIME:OK",lenth1);
								memcpy(&temp_buf[lenth1],aoa_at_end_tok,4);
								aoa_send_response(temp_buf,lenth1+4);
					
					
					}
		
#if 0	
	while(data[count]){
				printf("aoa_at_handle_zreportloctime count %d \r\n",count);
					if(data[count] == ','){
								if(data[count+1] != '2' &&data[count+1] != '5' && data[count+1] != '1' ){
								lenth = strlen("+ZREPORTLOCTIME:ERROR");
								memset(temp_buf,0x00,30);
								memcpy(temp_buf,"+ZREPORTLOCTIME:ERROR",lenth);
								memcpy(&temp_buf[lenth],aoa_at_end_tok,4);
								aoa_send_response(temp_buf,lenth+4);
								}else{
									lenth = strlen("AT+ZREPORTLOCTIME=");
									memcpy(zdev_set.zdev_report_time,&data[lenth+13],2);
								lenth = strlen("+ZREPORTLOCTIME:OK");
								
								memset(temp_buf,0x00,30);
								memcpy(temp_buf,"+ZREPORTLOCTIME:OK",lenth);
								memcpy(&temp_buf[lenth],aoa_at_end_tok,4);
								aoa_send_response(temp_buf,lenth+4);
									printf("zpepor temp %s \r\n",temp_buf);
								
								}
					}else if((data[count] == '#')&& (data[count+1] =='#')){
					
								break;
					}
					
				count ++;
					
			}
	
#endif
	
	#if 0
	ZbTokenizer  tzer[1];
	ZbToken  tok_device_id,tok_time;
	uint32_t time_in_second;
	uint16_t headerlen=0;
	uint8_t deviceid[DEVICE_ID_LEN],res;

	headerlen=strlen(aoa_at_table[idx].pData);
	zb_tokenizer_init(tzer, data+headerlen, data+len);
	get_zdevice_id(deviceid);

	tok_device_id=zb_tokenizer_get(tzer,0);
	tok_time=zb_tokenizer_get(tzer,1);

	if(0==memcmp(deviceid,tok_device_id.p,DEVICE_ID_LEN))
	{
		time_in_second=(uint32_t)str2int(tok_time.p, tok_time.end);
		res=handle_zdevice_zreportloctime(time_in_second);
		make_zdevice_okerror_response(0,0,0,idx);
	}
	else
	{
		res=make_zdevice_okerror_response(0,0,1,idx);
	}
	return res;
	#endif
	
}

uint8_t aoa_at_handle_breportinfotime(uint8_t *data, uint16_t len, uint8_t idx) //
{
	//osStatus ret;
	//ret=task_ble_mail_put(data, len);
//	return ((uint8_t)(ret));
	
		uint8_t lenth;
		uint8_t temp_buf[30];
		uint8_t ble_buf[11];
		lenth = strlen("AT+BREPORTINFOTIME=");
	
		if(data[lenth] != '2' &&data[lenth] != '5' && data[lenth] != '1' ){
								lenth = strlen("+BREPORTINFOTIME:ERROR");
								memset(temp_buf,0x00,30);
								memcpy(temp_buf,"+BREPORTINFOTIME:ERROR",lenth);
								memcpy(&temp_buf[lenth],aoa_at_end_tok,4);
								aoa_send_response(temp_buf,lenth+4);
								}else{
								memcpy(zdev_set.bdev_report_time,&data[lenth],2);
								memcpy(ble_buf,"bdrept:",7);
								memcpy(&ble_buf[7],&data[lenth],2);
								memcpy(&ble_buf[9],"##",2);
								ble_send_response(ble_buf,11);
								lenth = strlen("+BREPORTINFOTIME:OK");
								memset(temp_buf,0x00,30);
								memcpy(temp_buf,"+BREPORTINFOTIME:OK",lenth);
								memcpy(&temp_buf[lenth],aoa_at_end_tok,4);
								aoa_send_response(temp_buf,lenth+4);
									printf("zpepor temp %s \r\n",temp_buf);
								
								}
	
	
}

uint8_t aoa_at_handle_taskover(uint8_t *data, uint16_t len, uint8_t idx)
{
	osStatus ret;
	ret=task_ble_mail_put(data, len);
	return ((uint8_t)(ret));
}
uint8_t lock_back_buf[50];
uint8_t lock_back_buf_len;

uint8_t aoa_at_handle_bboxunlock(uint8_t *data, uint16_t len, uint8_t idx)
{
	uint8_t  ret;
	//ret=task_ble_mail_put(data, len);
	uint8_t totallen=0,lenth=0,lenth1 = 0;
	uint8_t temp_buf[25];
		
	
	lenth = strlen("AT+BBOXUNLOCK=");
	memcpy(temp_buf,"olock:",6);
	memcpy(&temp_buf[6],&data[lenth+2],16);
	memcpy(&temp_buf[22],"##",2);
	ble_send_response(temp_buf, 24);

	
	lenth1=strlen("+BBOXUNLOCK:");
	memcpy(lock_back_buf,"+BBOXUNLOCK:",lenth1);
	totallen+=lenth1;
	//AT+BBOXUNLOCK=1,122019090017,ON##
	if((data[lenth+16] == 'N') && (data[lenth+15] == 'O')){
		memcpy(&lock_back_buf[totallen],&data[lenth],17);
		totallen +=17;
	
	}else if(((data[lenth+16] == 'F') && (data[lenth+15] == 'O'))){
			memcpy(&lock_back_buf[totallen],&data[lenth],18);
		  totallen +=18;
	
	}
	memcpy(&lock_back_buf[totallen],",",1);
	totallen+=1;
	
	lock_back_buf_len = totallen;
	
	//memcpy(&lock_back_buf[totallen],aoa_at_end_tok,AT_END_TOK_LEN);
	//totallen+=AT_END_TOK_LEN;
	
	//ret = aoa_send_response(lock_back_buf,totallen);
	return ((uint8_t)(ret));
}

uint8_t aoa_at_handle_rdmsg(uint8_t *data, uint16_t len, uint8_t idx)
{
	uint32_t bd1_receiver_num=179773;//136166;
	uint8_t bd1_msg_debug[5]={0x68, 0x65, 0x6c, 0x6c, 0x6f};//hello
	rd_send_msg(bd1_receiver_num, bd1_msg_debug, 5);
}


/*
AT+BDISTANCE=10
+BDISTANCE: OK
*/
uint16_t make_bdistance_response(uint8_t *response)
{
	uint16_t totallen=0,len=0;
	uint8_t data;
	
	len=strlen("+ZBEAT:");
	memcpy(response,"+ZBEAT:",len);
	totallen+=len;
	
	get_zdevice_id(&response[totallen]);
	totallen+=DEVICE_ID_LEN;
	
	add_separator_comma(&response[totallen]);
	totallen+=1;
	
	data=get_zdevice_battery();
	response[totallen]=data+0x30;
	totallen+=1;
	
	add_separator_comma(&response[totallen]);
	totallen+=1;
	
	data=get_rd_rssi();
	response[totallen]=data+0x30;
	totallen+=1;
	
	add_separator_comma(&response[totallen]);
	totallen+=1;
	
	data=get_ble_rssi();
	response[totallen]=data+0x30;
	totallen+=1;
	
	memcpy(&response[totallen],aoa_at_end_tok,AT_END_TOK_LEN);
	totallen+=AT_END_TOK_LEN;

	return totallen;
}


uint8_t aoa_uart_data_handle(uint8_t *data, uint16_t len)
{
	uint8_t i=0,atlen=0;
	
	
	//printf("aoa_uart_data_handle \r\n");
	if(len<AT_END_TOK_LEN) return 1;
//	printf("len = %d data is %s  \r\n",len,data);
	
	for(i=0;i<SUPPORT_AT_NUM;i++)
	{

		atlen=strlen(aoa_at_table[i].pData);
	
		if((0==memcmp(data, aoa_at_table[i].pData, atlen))&&aoa_at_table[i].handle!=NULL){
			
			return aoa_at_table[i].handle(data, len,i);
		
		}
	}
	return 1;
}

uint8_t aoa_mail_data_handle(uint8_t *data, uint16_t len)
{
	return aoa_send_response(data,len);
}


uint8_t aoa_uart_receive_and_handle(void)
{
	
	
	uint8_t ret=0;
	memset(aoa_uart_data_loop,0x00,sizeof(aoa_uart_data_loop));
	hw_uart1_receive(aoa_uart_data_loop, &aoa_uart_data_loop_len, 10);
		
	printf("aoa:%s \r\n",aoa_uart_data_loop);
	
	
	if (aoa_uart_data_loop_len>0)
	{
		ret=aoa_uart_data_handle(aoa_uart_data_loop, aoa_uart_data_loop_len);
		aoa_uart_data_loop_len=0;
	}
	return ret;
}

uint8_t aoa_mail_receive_and_handle(aoamail *pmail)
{
	aoa_mail_data_handle(pmail->data, pmail->len);
	
	//hw_uart2_send(pmail->data, pmail->len);/*test code*/
}


/**************only for debug test***************/

void aoa_test_mail_fun(void)
{
//	task_ble_mail_put(aoa_at_table[0].pData,8);
}


void reset_bdev_begin(void)
{
	zdev_set.bdevtask1_num =0;
	zdev_set.bdevtask2_num =0;
}
/**************only for debug test***************/
