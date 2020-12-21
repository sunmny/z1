#include "hw_uart.h" 
#include "stm32l4xx_hal.h"
//#include "cmsis_os.h"
#include "string.h"
//#include "delay.h"
#include "app_ble.h"
#include "hw_config.h"
#include "task_ble.h"
#include "hw_bd1.h"
#include "hw_bd2.h"
#include "app_nvram.h"
#include "app_aoa.h"


struct bdinfo{			
				uint8_t	num_group;
				uint8_t dis_status;
				uint8_t bat_soc;
				uint8_t lost_flag;
				uint8_t version_bd[8];
				uint8_t addr[12];
				uint8_t bbind_sucess;
				
};

struct bdinfo bd_info[100];

static const uint8_t aoa_at_end_tok[4] = {0x23,0x23,0x0d,0x0a};
#define BLE_SUPPORT_UART_AT_NUM    15
extern osStatus task_test_mail_put(uint8_t *data, uint16_t len);
uint8_t rn_send_flag =0xff;
uint8_t rd_send_flag =0xff;
uint8_t testtimercount = 0;
uint8_t ble_uart_data_loop[BLE_UART_DATA_LOOP_SIZE+1];
//extern void zdev_read_battery_soc(uint16_t *data,uint8_t *val);
uint16_t ble_uart_data_loop_len;
#define  DEVICE_ID_LEN   12
#define  REPORT_TIMES	    2
//extern osTimerId testTimerHandle;
//extern osTimerId rdTimerHandle;
extern UART_HandleTypeDef huart2;
uint8_t ble_send_response(uint8_t *data, uint16_t len);
extern uint8_t rd_num[6];

extern void aw9523_set_light(uint8_t count,uint8_t val);
//extern osTimerId LedTimerHandle;
#define AT_END_TOK_LEN 4
static const uint8_t ble_at_end_tok[AT_END_TOK_LEN] = {0x23,0x23,0x0d,0x0a};//##\r\n
uint8_t ble_at_handle(uint8_t *data, uint16_t len, uint8_t at_index);

uint8_t rd_buf[16] = {0x24,0x43,0x43,0x49,0x43,0x41,0x2C,0x30,0x2C,0x30,0x30,0x2A,0x37,0x42,0x0D,0x0A};
//static uint8_t ptr[6]={0x23,0x23,0x31,0x32,0x0d,0x0a};//test code
//uint8_t mzdev_add[12];
struct bdevice_status ft_dev;
extern uint8_t gsn_buf[12];
uint8_t holdtask_flag =0;

/***********************test mode*************************/
uint8_t ble_uart_at_handle_rd(uint8_t *data, uint16_t len, uint8_t idx);
uint8_t ble_uart_at_handle_rn(uint8_t *data, uint16_t len, uint8_t idx);
uint8_t ble_uart_at_handle_sd(uint8_t *data, uint16_t len, uint8_t idx);
uint8_t ble_uart_at_handle_gprs(uint8_t *data, uint16_t len, uint8_t idx);
uint8_t ble_uart_at_handle_power_key(uint8_t *data, uint16_t len, uint8_t idx);
uint8_t ble_uart_at_handle_sos_key(uint8_t *data, uint16_t len, uint8_t idx);
uint8_t ble_uart_at_handle_charger_key(uint8_t *data, uint16_t len, uint8_t idx);
uint8_t ble_uart_at_handle_light(uint8_t *data, uint16_t len, uint8_t idx);
uint8_t ble_uart_at_handle_nfc(uint8_t *data, uint16_t len, uint8_t idx);
uint8_t ble_uart_at_handle_battery(uint8_t *data, uint16_t len, uint8_t idx);
uint8_t ble_uart_at_handle_sensor_ps(uint8_t *data, uint16_t len, uint8_t idx);
uint8_t ble_uart_at_handle_hall(uint8_t *data, uint16_t len, uint8_t at_index);
uint8_t ble_uart_at_handle_lock(uint8_t *data, uint16_t len, uint8_t at_index);
uint8_t ble_uart_at_handle_ble_connect(uint8_t *data, uint16_t len, uint8_t at_index);

uint8_t ble_uart_at_handle_nfc(uint8_t *data, uint16_t len, uint8_t idx);
uint8_t ble_uart_at_handle_moto(uint8_t *data, uint16_t len, uint8_t idx);
uint8_t ble_uart_at_handle_lcd(uint8_t *data, uint16_t len, uint8_t at_index);
uint8_t ble_uart_at_handle_lcd_key(uint8_t *data, uint16_t len, uint8_t at_index);
uint8_t ble_uart_at_handle_write_sn(uint8_t *data, uint16_t len, uint8_t at_index);
uint8_t ble_uart_at_handle_reset_back_data(uint8_t *data, uint16_t len, uint8_t at_index);
uint8_t aoa_at_handle_bdev_info(uint8_t *data, uint16_t len, uint8_t at_index);
uint8_t ble_uart_at_handle_battery1(uint8_t *data, uint16_t len, uint8_t idx);
uint8_t ble_uart_at_handle_ble_getmid(uint8_t *data, uint16_t len, uint8_t at_index);
uint8_t ble_at_handle1(uint8_t *data, uint16_t len, uint8_t at_index);
uint8_t ble_open_lock_back(uint8_t *data, uint16_t len, uint8_t at_index);
uint8_t ble_airplanmode_handle(uint8_t *data, uint16_t len, uint8_t at_index);
uint8_t ble_versionget_handle(uint8_t *data, uint16_t len, uint8_t at_index);
uint8_t ble_set_bleaddr(uint8_t *data, uint16_t len, uint8_t at_index);
uint8_t ble_at_handle_call(uint8_t *data, uint16_t len, uint8_t at_index);
uint8_t ble_at_hold_task(uint8_t *data, uint16_t len, uint8_t at_index);
uint8_t ble_at_start_task(uint8_t *data, uint16_t len, uint8_t at_index);
typedef enum{
	BDINFO =0,
	VERSION,
	BDISTANCE,
	writedev,
	bbind,
	UBIND,
	airplanmode,
	call,
	hold,
	subind,
	FTCN,
	GMID,
	FTWRSN,
	FTXX,
	//TX_BBOXLOCKED,//unsolicited
	SUPPORT_AT_NUM
}ble_at_request;

static const ble_at_symbol ble_uart_at_table[BLE_SUPPORT_UART_AT_NUM] = {
	{aoa_at_handle_bdev_info, (uint8_t *)"devinfo:"},
	{ble_versionget_handle,(uint8_t *)"versionget:"},
	{ble_at_handle,(uint8_t *)"+BDISTANCE:"},
	{ble_set_bleaddr,(uint8_t *)"writedev:"},
	{ble_at_handle,(uint8_t *)"bbind:"},	
	{ble_at_handle,(uint8_t *)"ubind:"},
  {ble_airplanmode_handle,(uint8_t *)"airplanemode:"},
	{ble_at_handle_call,(uint8_t *)"call:"},
	{ble_at_hold_task,(uint8_t *)"HoldTask"},
	{ble_open_lock_back,(uint8_t *)"subind:"},
	{ble_at_start_task, (uint8_t *)"hello"},
	{ble_uart_at_handle_ble_getmid, (uint8_t *)"GMID##"},
	{ble_uart_at_handle_write_sn, (uint8_t *)"FTSN"},
	{ble_uart_at_handle_reset_back_data, (uint8_t *)"FTXX##"},
};

uint8_t ble_set_bleaddr(uint8_t *data, uint16_t len, uint8_t at_index);
extern uint8_t aoa_at_handle_bbeatnow_bdev(uint8_t *report_id,uint8_t *report_info);
extern uint8_t wlist_send_count;
extern uint8_t bind_flag;



void clear_bd_info(void)
{
	 memset(bd_info,0x00,sizeof(bd_info));
}



extern uint8_t sendid_count;

uint8_t bleisok =0;

uint8_t ble_set_bleaddr(uint8_t *data, uint16_t len, uint8_t at_index)
{
     uint8_t lenth = 0;
	  lenth = strlen("writedev:");
	
	  if(data[lenth] == 'o' && data[lenth+1] == 'k')
			 bleisok =1;
		else
			bleisok =0;
}
extern uint8_t light_response[60];
extern uint8_t light_back_len;
uint8_t light_back_buf_bak[50];
extern uint8_t send_light_commnd;
uint8_t ble_at_handle_call(uint8_t *data, uint16_t len, uint8_t at_index)
{
//call:132011260038,01,01,LED on ok##            2640回复的开启LED完整数据格式
//call:132011260038,01,01,LED off ok##           2640回复的关闭LED完整数据格式
	uint8_t call_dev_buf[12];
	uint8_t i,lenth=0;
	lenth = strlen("call:");
	
   if(0 == memcmp(light_back_buf_bak,data,len))
			return 0 ;
	memcpy(call_dev_buf,&data[lenth],12);
	 for(i =0;i<len;i++){
			if(data[i] =='#'&&data[i+1] == '#'){
				if(data[i-2] == 'o' && data[i-1] == 'k'){
				if(0 == memcmp(call_dev_buf,&light_response[10],12)){
							send_light_commnd =0;
					     aoa_send_response(light_response, light_back_len);
					break;
				}
				}
			
			}
	 
	 
	 }
	 
	 
	 
	 
	  memcpy(light_back_buf_bak,data,len);
		//aoa_send_response(data, len);
	return 0;
}

uint8_t ble_at_hold_task(uint8_t *data, uint16_t len, uint8_t at_index)
{

		holdtask_flag = 1;
	  set_command_type(0xff);



}
uint8_t ble_at_start_task(uint8_t *data, uint16_t len, uint8_t at_index)
{
		if(holdtask_flag ==1){		
		   set_command_type(0xf1);
			holdtask_flag =0;
		}




}
uint8_t version_info[10]="20";

uint8_t ble_versionget_handle(uint8_t *data, uint16_t len, uint8_t at_index)
{
		uint8_t lenth =0,totallen ;
		uint8_t zcall_response[70];
	  lenth = strlen("versionget:");
	 	 memcpy(&version_info[2],&data[lenth],6);
	
		
			totallen = strlen("+ZCALL:");
		
			memcpy(zcall_response,"+ZCALL:",totallen);
		memcpy(&zcall_response[totallen],gsn_buf,12);			
			memcpy(zdev_set.mydev_id,gsn_buf,12);
			totallen +=12;
			memcpy(&zcall_response[totallen],",OK",3);
			totallen +=3;
		   

			//memcpy(&zcall_response[totallen],version_info,8);
		//	totallen +=8;
			memcpy(&zcall_response[totallen],"##",2);
			totallen +=2;
	   	printf("zcall buf is %s \r\n",zcall_response);
		   //zdev_isbind = 1;
		aoa_send_response(zcall_response, totallen);
	  
		
}
uint8_t bdinfo_count =0;
extern uint8_t send_wl_temp[100][15];
extern uint8_t send_wl_temp1[100][15];
uint8_t bind_dev_version[6]="201126";

uint8_t  copy_addr_group(void)
{
   uint8_t i =0,count=0;
		memset(bd_info,0x00,sizeof(bd_info));
	for(i = 0;i<100;i++){
		 if(send_wl_temp1[i][0] !='1' ||send_wl_temp1[i][14]==0){
					continue;
		 }else{
				memcpy(bd_info[count].addr,send_wl_temp1[i],12);
				memcpy(bd_info[count].version_bd,bind_dev_version,6);
				bd_info[count].num_group = send_wl_temp1[i][13];
			  printf("bd_info[%d].addr %s \r\n",count,bd_info[count].addr);
				count ++;
		 }	
		
			
		 
	}
		return count;

}
uint8_t devinfo_flag =0;
uint8_t devinfo_start_flag =0;
extern uint8_t task_flag_start;
//extern osTimerId ReportTimerHandle;
uint8_t aoa_at_handle_bdev_info(uint8_t *data, uint16_t len, uint8_t at_index)
{
			uint8_t lenth =0,group_num = 0xff;
	    uint16_t i =0,j=0;
			lenth = strlen("devinfo:");
	
		//printf("sunmny data %d %d %s \r\n",len, (len%31),data );
	     if(!task_flag_start){
								memset(bd_info,0x00,sizeof(bd_info));
							   return 0;
			 }
	  		if(!devinfo_flag){
//				osTimerStart(ReportTimerHandle, 200);
				devinfo_flag =1;
				devinfo_start_flag =1;
			}

			
			if(len%31 !=0){
				return 0;
			
			}else{
			   while(i <len){
					if(data[i] == 'd'){
						if(memcmp(&data[i],"devinfo:",8) ==0){
								group_num = (data[i+lenth+13] - '0')*10 + (data[i+lenth+14] - '0');
							  printf("devinfo groupnum %d  %d \r\n",i,group_num);
								for(j =0;j<100;j++){
									 printf("devinfo dev gnu %d  \r\n",i,bd_info[j].num_group);
									if(bd_info[j].num_group == group_num){
										
										if(memcmp(bd_info[j].addr,&data[i+lenth],12) == 0){
								      
											bd_info[j].dis_status = (data[i+lenth+16] - '0' )*10 + (data[i+lenth+17] - '0' );
							
											bd_info[j].bat_soc = (data[i+lenth+19] - '0' )*10 + (data[i+lenth+20] - '0' );
											bd_info[j].lost_flag = 1;
											printf("devinfo  bd_info[j].dis_status %d  %d  %d\r\n",bd_info[j].dis_status,bd_info[j].bat_soc,bd_info[j].lost_flag);
						}				
						
				}else{
					 continue;	
				
				}
			}		
						}				
					}
			i++;					
				 }		
			}
		

}

uint8_t report_count =0;
uint8_t report_once =0;
void ReportTimerCallback(void)
{
  uint8_t lost_buf[35];
	uint8_t report_battery[30];
	uint8_t report_buf[22] = "5100041111201126000000";
//	printf("report bd info\r\n");
	if(report_count >99){
		  report_once++;
			report_count =0;
	}else{
		printf("sunmny  bd_info.addr:%s bd_info[%d].dis_status %d  %d %d %d \r\n",bd_info[report_count].addr, report_count,bd_info[report_count].dis_status,bd_info[report_count].bat_soc,bd_info[report_count].lost_flag,report_once);
			if((bd_info[report_count].addr[0] == '1')&&(bd_info[report_count].lost_flag ==1)&&(bd_info[report_count].dis_status ==0)){
			
					if(bd_info[report_count].bat_soc ==0)
						report_buf[5] = '0';
					else
						report_buf[5] = '4';
				   memcpy(&report_buf[10],bd_info[report_count].version_bd,6);
					aoa_at_handle_bbeatnow_bdev(bd_info[report_count].addr,report_buf);
				  bd_info[report_count].lost_flag=0;
				
			}else if((bd_info[report_count].addr[0] == '1')&&(bd_info[report_count].lost_flag ==0)&&(REPORT_TIMES ==report_once)){
										memcpy(lost_buf,"+BPALERT:",9);
										memcpy(&lost_buf[9],bd_info[report_count].addr,12);
										memcpy(&lost_buf[21],",",1);
										memcpy(&lost_buf[22],"3",1);
										memcpy(&lost_buf[23],",",1);
										memcpy(&lost_buf[24],"1",1);
										memcpy(&lost_buf[25],aoa_at_end_tok,4);
										report_once=0;
										aoa_send_response(lost_buf,29);
			
			}else if((bd_info[report_count].addr[0] == '1')&&(bd_info[report_count].dis_status ==1)){
			
			
					          memcpy(lost_buf,"+BPALERT:",9);
										memcpy(&lost_buf[9],bd_info[report_count].addr,12);
										memcpy(&lost_buf[21],",",1);
										memcpy(&lost_buf[22],"1",1);
										memcpy(&lost_buf[23],",",1);
										memcpy(&lost_buf[24],"20",2);
										memcpy(&lost_buf[26],aoa_at_end_tok,4);
									 aoa_send_response(lost_buf,30);
										bd_info[report_count].dis_status =0;
			
			}else if((bd_info[report_count].addr[0] == '1')&&(bd_info[report_count].bat_soc ==0)){
			
			
										memcpy(report_battery,"+BPALERT:",9);
										memcpy(&report_battery[9],bd_info[report_count].addr,12);
										memcpy(&report_battery[21],",",1);
										memcpy(&report_battery[22],"1",1);
										memcpy(&report_battery[23],",",1);
								
										memcpy(&report_battery[24],"1",1);
										
									

											memcpy(&report_battery[25],aoa_at_end_tok,4);
			
						        aoa_send_response(report_battery,29);
			}
	
			if(REPORT_TIMES ==report_once)
				    report_once =0;
			report_count++;
	}
			
}
uint8_t uart_2640_flag =0;
uint16_t uart_2640_flag_count =0;
void devinfo_report_task(void const * argument)
{

		while(1){
				osDelay(100);
				printf("start_flag %d %d  \r\n",devinfo_start_flag,task_flag_start);
			if(devinfo_start_flag && task_flag_start)
				     ReportTimerCallback();
			if(uart_2640_flag ==0){
				uart_2640_flag_count ++;
				if((uart_2640_flag_count>30)){
					printf("not receive from 2640 \r\n");
				
				set_ble_power_off();
				osDelay(100);
				set_ble_power();
							
				uart_2640_flag_count = 0;
				}
			}else{
				uart_2640_flag = 0;
				uart_2640_flag_count =0;
			}
		}


}



//extern osTimerId ReportTimerHandle;
extern uint8_t contiue_task;
uint8_t zdev_isbind = 0;

void ble_zid_data(uint8_t *data,uint8_t len)
{
	uint8_t lenth ,lenth1;
	uint8_t buf[40];
	lenth =	strlen(data);
	if(lenth <6){
			memcpy(buf,"+ZCALL:ERROR",12);
			memcpy(&buf[12],aoa_at_end_tok,4);
		aoa_send_response(buf, 16);
	}else{
			
		lenth = strlen("+BDADDR:");
			lenth1 = strlen("+ZCALL:");
		
			memcpy(buf,"+ZCALL:",lenth1);
			memcpy(&buf[lenth1],gsn_buf,DEVICE_ID_LEN);			
		//	memcpy(zdev_set.mydev_id,gsn_buf,DEVICE_ID_LEN);
			lenth1 +=12;
			memcpy(&buf[lenth1],",OK",3);
			lenth1 +=3;
			//memcpy(&buf[lenth1],"20",2);
			//lenth1 +=2;
		//	memcpy(&version_info[2],&data[lenth+13],6);
			//lenth1 +=6;
			memcpy(&buf[lenth1],aoa_at_end_tok,4);
			lenth1 +=4;
		printf("zcall buf is %s \r\n",buf);
		zdev_isbind = 1;
		//aoa_send_response(buf, lenth1);
		
	}

		
}
extern uint8_t backup_buf[50];
extern uint8_t get_task_bdev_id(uint8_t *data , uint16_t len ,uint8_t idx);
extern uint8_t bind_command_buf_bak[100];
extern uint16_t bind_command_len_bak;
extern uint8_t scan_begin_flag;

//ble bbind:132009220010,01,01,201025,ok## 
uint8_t bind_bak_buf[50];
extern uint8_t bbind_num;
extern uint8_t save_nv_buf[70];
extern uint8_t bbind_back_dev_buf[12];
void ble_bind_back(uint8_t *data,uint8_t len)
{
	uint8_t lenth ,lenth1,lenth2,count=0,count1 =0;
	uint8_t buf[50];
  uint8_t addr_buf[12];
	uint8_t group_num_back[2];
	uint8_t tasknum[2];
	uint8_t version_bd[6];
	uint8_t i;
	
	lenth1 = strlen("+BBIND:");
	lenth = lenth1;
	lenth2 = strlen("AT+BBIND=");
	memcpy(buf,"+BBIND:",7);
	
	//hw_bd2_open();
		printf("bind num = %d bind backup_buf is %s \r\n",bbind_num,buf);
	if(0 == memcmp(bind_bak_buf,data,len)){
				return ;
	}
	while(backup_buf[count]){
				if(backup_buf[count] =='#' && backup_buf[count+1] == '#'){
						memcpy(&buf[lenth1],&backup_buf[lenth2],(count-lenth2));
						lenth1 = lenth1+(count-lenth2);
						break;
				}else{
				count++;
				}
		}
	  count =0;
		//printf("sunmny  is %s \r\n",buf);
		while(data[count] != '#' ){
			if(data[count] == ','){
				count1 ++;
							
			printf("count1 is %d \r\n",count1 );
			 if(count1 ==1){
				 printf("count is %d \r\n",count );
			   memcpy(addr_buf,&data[count-12],12);
			   printf("addr_buf %s \r\n",addr_buf);
			 }else if(count1 ==2){
			   memcpy(group_num_back,&data[count -2],2);
			  printf("group_num %s \r\n",group_num_back);
			 }else if(count1 ==3){
			 
			   memcpy(tasknum,&data[count -2],2);
				  printf("tasknum %s \r\n",tasknum);
			 }else if(count1 ==4){
			 
			  memcpy(bind_dev_version,&data[count -6],6);
				  printf("version_bd %s \r\n",version_bd);
			 }
			 	}
				count ++;
		}
	
		 printf("data %c %c \r\n",data[count-2],data[count-1]);
		if(data[count-2] == 'o' && data[count -1] == 'k'){
					
					memcpy(&buf[lenth1],",",1);
					lenth1+=1;
					memcpy(&buf[lenth1],group_num_back,2);
					lenth1+=2;
					memcpy(&buf[lenth1],",",1);
					lenth1+=1;
					memcpy(&buf[lenth1],"OK",2);
					lenth1+=2;
					set_ble_red_light(0);
		
						memcpy(&buf[lenth1],aoa_at_end_tok,4);
						lenth1 +=4;
					printf("bind buf back %s \r\n",buf);
					memcpy(bind_bak_buf,data,len);
				if(0 == memcmp(bbind_back_dev_buf,addr_buf,12))
					aoa_send_response(buf, lenth1);
					
//						set_nvram_save_data(save_nv_buf);
			   // get_task_bdev_id(bind_command_buf_bak,bind_command_len_bak,10);
			
			
		}else if(data[count-1] == 'd' && data[count-2] == 'e'&& data[count-3] == 'l'){
		
		
		}

		
					
		//	memset(backup_buf,0x00,50);
}
extern uint8_t ubind_back[50];
extern uint8_t ubind_back_err[50];
extern uint8_t ubind_len;
extern uint8_t ubind_len_err;
extern uint8_t zdev_bind_flag;
void ble_ubind_back(uint8_t *data,uint8_t len)
{
      uint8_t lenth = 0;
			uint8_t aoa_buf[30];
			lenth = strlen("ubind:");
	
		//devinfo_flag=0;
		
		//if(data[len-4] == 'o'&&data[lenth-3] =='k'){
		
			//aoa_send_response(ubind_back,ubind_len);
				//  aoa_send_response(ubind_back,17);
			//aoa_send_response(ubind_back,ubind_len);
		//}else{
		//aoa_send_response(ubind_back_err,ubind_len_err);
			//aoa_send_response(ubind_back_err,ubind_len_err);
		//}
		

//			osTimerStart(ReportTimerHandle,5000);	
		//	memcpy(aoa_buf,"+BUNBIND:",9);
		//	memcpy(&aoa_buf[9],&data[lenth],(len -7));
			//memcpy(&aoa_buf[len],aoa_at_end_tok,4);

		//task_aoauart_mail_put(aoa_buf, len+4);
}
extern uint8_t group_num_data;
uint8_t send_ble_count =0;
extern uint8_t blebuf[50]; 
extern uint8_t group_location_count;

uint8_t ble_airplanmode_handle(uint8_t *data, uint16_t len, uint8_t at_index)
{
		send_ble_count++;
	
	  if(send_ble_count <= group_num_data ){
				 
		//blebuf[group_location_count] = '0' + (send_ble_count/10);
		//blebuf[group_location_count +1] = '1' + (send_ble_count%10);
		//blebuf[group_location_count +2] = '#';
		//blebuf[group_location_count +3] = '#';
		//ble_send_response(blebuf,group_location_count+3);
		}
}
uint8_t ble_at_handle(uint8_t *data, uint16_t len, uint8_t at_index)
{
	osStatus ret;
//printf("ble at handle \r\n");
	switch (at_index)
	{
		case 2:
				ble_zid_data(data,len);
					
			break;
		case 4:
				ble_bind_back(data,len);
					zdev_bind_flag =0;
			break;
		case 5:
				ble_ubind_back(data,len);
				break;
				
		default:
			//ret=task_aoauart_mail_put(data, len);
			break;
	}
	return (uint8_t)ret;
}

uint8_t ble_at_handle1(uint8_t *data, uint16_t len, uint8_t at_index)
{


					

}
uint8_t ble_send_response(uint8_t *data, uint16_t len);
uint8_t ble_uart_data_handle(uint8_t *data, uint16_t len)
{
	uint8_t i=0,atlen=0;
//	printf("uart ble receive %s \r\n",data);
	
	//ble_send_response("helloworld",10);
	if(len<AT_END_TOK_LEN) return 1;

	//if(memcmp(&data[len-AT_END_TOK_LEN],ble_at_end_tok,AT_END_TOK_LEN)!=0) return 1;

	for(i=0;i<SUPPORT_AT_NUM;i++)
	{
		atlen=strlen(ble_uart_at_table[i].pData);
		if(0==memcmp(data, ble_uart_at_table[i].pData, atlen))
			return ble_uart_at_table[i].handle(data, len, i);
	}
	return 1;
}

uint8_t ble_send_response(uint8_t *data, uint16_t len)
{
	//printf("uart 4 send len  %d \r\n",len);
	return hw_uart4_send_no_dma(data,len);
}



uint8_t ble_receive_and_handle(void)
{
	uint8_t ret=0;
	memset(ble_uart_data_loop,0x00,sizeof(ble_uart_data_loop));
	hw_uart4_receive(ble_uart_data_loop, &ble_uart_data_loop_len, 0xff);
	printf("ble %s \r\n",ble_uart_data_loop);
	
	//hw_uart1_send(ble_uart_data_loop, ble_uart_data_loop_len);
	if (ble_uart_data_loop_len>0)
	{
		//hw_uart2_send(ble_uart_data_loop, ble_uart_data_loop_len);
		ret=ble_uart_data_handle(ble_uart_data_loop,ble_uart_data_loop_len);
		uart_2640_flag = 1;
		ble_uart_data_loop_len=0;
	}
	return ret;
}

extern uint8_t lock_back_buf[50];
extern uint8_t lock_back_buf_len;
uint8_t ble_open_lock_back(uint8_t *data, uint16_t len, uint8_t at_index)
{
	uint8_t i;
			while(data[i]){
	
						if(data[i] == '#' && data[i+1] =='#'){
							
									break;
			}else {
							i++;
						}
					if(i>200){
							break;
					}
			
			}
			if(data[i-1] == 'k' && data[i-2] == 'o'){
				//if(bbind_num >= 1)
										//bbind_num -=1;
				
				//save_nv_buf[69] = bbind_num;
						//set_nvram_save_data(save_nv_buf);
				printf("subind bbind_num = %d \r\n",bbind_num);
			}
			
}


void send_bd1_get_card_num(void)
{
		//hw_uart2_send(rd_buf,16);
	HAL_UART_Transmit(&huart2,rd_buf,16, 0xFFFF);
}

	





uint8_t ble_uart_at_handle_ble_getmid(uint8_t *data, uint16_t len, uint8_t at_index)
{
		//uint8_t gsn_buf[12] ={0};
		uint8_t send_buf[20];
			memcpy(send_buf,"GMID",4);
		if(gsn_buf[0] !='1' &&gsn_buf[1] !='0'){
			get_nvram_sn(gsn_buf);
				memcpy(&send_buf[4],gsn_buf,12);
		}else{
				memcpy(&send_buf[4],gsn_buf,12);
		}
	//	osDelay(2000);
	
		//memcpy(&send_buf[4],gsn_buf,12);
		memcpy(&send_buf[16],"##",2);
		
//		printf("sn send_buf = %s \r\n",send_buf);
	//	osDelay(500);
		ble_send_response(send_buf,18);
		printf("sn send_buf = %s \r\n",send_buf);
		
	
}
uint8_t ble_uart_at_handle_write_sn(uint8_t *data, uint16_t len, uint8_t at_index)
{
		uint8_t sn_buf[13] = "112233445566",buf[10];
		uint8_t i;
		uint8_t send_buf[20];
		printf("l476writesn\r\n",15);
		memcpy(sn_buf,&data[4],12);
	//	osDelay(2000);
		ble_send_response(sn_buf,12);
		set_nvram_sn_data(sn_buf);
		memset(sn_buf,0x00,13);
	//	osDelay(2000);
		//hw_uart1_send_dma(sn_buf,12);
		get_nvram_sn(sn_buf);
	printf("sn_buf = %s \r\n",sn_buf);
		//hw_uart1_send_dma(sn_buf,12);
	  for(i =0;i<3;i++){
		if(memcmp(sn_buf,&data[4],12)!= 0){
		
			memcpy(sn_buf,&data[4],12);
			set_nvram_sn_data(sn_buf);
		//	osDelay(2000);
			memset(sn_buf,0x00,13);
		//	osDelay(1000);
			get_nvram_sn(sn_buf);
		//	osDelay(500);
		}else{
			memcpy(send_buf,"GMID",4);
			memcpy(&send_buf[4],sn_buf,12);
			memcpy(&send_buf[16],"##",2);
			memcpy(buf,"FTSNOK",6);
			memcpy(&buf[6],ble_at_end_tok,4);			
			ble_send_response(buf,10);
			osDelay(1000);
			ble_send_response(send_buf,18);
			break;
		}
//		printf("sn_buf = %s \r\n",sn_buf);
	}
//		printf("sn_buf = %s \r\n",sn_buf);
			if((i ==2)&&(memcmp(sn_buf,&data[4],12)!= 0)){
				memcpy(buf,"FTSNERR",7);
			memcpy(&buf[7],ble_at_end_tok,4);
				osDelay(500);
			ble_send_response(buf,11);
			}
		
}
uint8_t ble_uart_at_handle_ble_connect(uint8_t *data, uint16_t len, uint8_t at_index)
{
//		printf("ble_conect \r\n");
		//osTimerStart (LedTimerHandle, 1000);

}
uint8_t ble_uart_at_handle_reset_back_data(uint8_t *data, uint16_t len, uint8_t at_index)
{
			uint8_t buf[10];
			//uint8_t led[6] = {0x29,0x2d,0x2c,0x2b,0x26,0x2a};

			 memcpy(buf,"FTXX",4);
			 memcpy(&buf[4],ble_at_end_tok,4);
			 ble_send_response(buf,8);


}
