
#include "cmsis_os.h"
#include "app_ble.h"
#include <stdint.h>
#include "task_meerwespe.h"
#include "app_aoa.h"
#include "app_power.h"
#include "hw_config.h"
extern osMailQId    mytestQueueHandle;
extern  UART_HandleTypeDef huart1;
extern osStatus task_lte_message_put(uint32_t info);


//extern osTimerId LedTimerHandle;
osStatus task_test_message_put(uint32_t info)
{
  	osStatus  status = osOK;
	status = osMessagePut(mytestQueueHandle, info, osWaitForever);
	 
	return status;
}

osEvent task_test_message_get(void)
{
  return osMessageGet(mytestQueueHandle, osWaitForever);
}

osStatus task_test_mail_put(uint8_t *data, uint16_t len)
{
	mytestmail *pblemail;
	pblemail=osMailCAlloc(mytestQueueHandle, osWaitForever);
	if(len>BLE_MAIL_BUF_LEN)
		pblemail->len=BLE_MAIL_BUF_LEN;
	else
		pblemail->len=len;
	memcpy(pblemail->data,data,pblemail->len);
	return osMailPut(mytestQueueHandle, pblemail);
}

osEvent task_test_mail_get(void)
{
	return osMailGet(mytestQueueHandle,osWaitForever);//parameter to do, 20181025
}

/* SensorTask function */

uint8_t send_wl_temp[100][15];
uint8_t sendid_count;

uint8_t cp_taskid_temp(void)
{
	
		
		
}

static const uint8_t aoa_at_end_tok[4] = {0x23,0x23,0x0d,0x0a};//##\r\n
extern uint8_t receive_wl_temp[10][15];
uint8_t  send_to_ble_white_list(uint8_t lenth,uint8_t idx )
{
		
	
}

uint8_t wlist_send_count =0;

void send_wlist_to_ble(void)
{
			
			

}
void mytestTask(void const * argument)
{
	osEvent event;
	blemail *pblemail;
	uint8_t ret =0;
	
  /* USER CODE BEGIN GPSTask */
  /* Infinite loop */
  for(;;)
  {
			
		printf("my_test_task----2 \r\n");
		
			event = task_test_mail_get();
			pblemail=event.value.p;
	//	printf("event.value.p :%s \r\n",pblemail->data);
			if(memcmp(pblemail->data,"stoprdtimer",11) == 0){
					
			}else if(memcmp(pblemail->data,"nobd1",5) == 0){
//						 printf("bd1 card no in \r\n");
						set_rd_red_light(1);
						
			}if(memcmp(pblemail->data,"inbd1",5) == 0){
							set_rd_red_light(0);		
				     hw_bd1_close();

						
			}
				
			
			osMailFree(mytestQueueHandle, pblemail);
				osDelay(1000);
	

  }
  /* USER CODE END GPSTask */
}
extern osThreadId mytestHandle1;
uint8_t send_count_flag =0;
uint32_t send_count_flag1 =0;
uint32_t send_count_flag5 =0;
uint8_t send_count_flag2 =0;
uint8_t send_count_flag3 =0;
uint8_t send_count_flag6 =0;
uint8_t send_count_flag7 =0;
uint8_t send_count_flag8 =0;
uint8_t send_count_flag9 =0;
uint8_t send_count_flag10 =0;
extern uint8_t zdev_isbind ;
extern uint8_t open_airmode;
extern void BatTimerCallback(void);
uint32_t bd1_receiver_num;
uint8_t bd1_send_sos_num[2];
uint32_t bd1_receiver_num1=927630;
extern uint8_t rd_send_msg(uint32_t int_bdnum, uint8_t *data, uint16_t len);
extern uint8_t charger_status;
uint8_t charger_info_buf[20];
uint8_t charger_status_flag = 0;
extern uint8_t zdev_charger_status;
	
//len=strlen(gpslocation.longitude);
//	len=strlen(gpslocation.latitude);

uint8_t int_to_hex(uint32_t data,uint8_t *src)
{
		uint32_t temp,temp1,temp2;
			temp = data;
		uint8_t buf[20] = {0},buf1[20] = {0} ,i = 0 ,j =0,len = 0;
		
		if(data == 0){
			src[0] = 0x00;
				return 1;
		}
	
	while(temp != 0){ 
		temp1 = temp%16;
				
		buf[i] = temp1;
		printf("buf0:%x \r\n",buf[i]);
		i+=1;		 
		temp = temp/16;

	}
	printf("hex i = %d \r\n",i);
	//memset(buf1,0x00,20);
	if(i%2 ==1){
					j =0;
		while(i>0){						
					buf1[j] =  buf[i] <<4 | buf[i-1];					
					printf("buf[%d] =  %x \r\n",i,buf1[j]);
					j++;
					if(i>2)
							i-=2;
					else 
						break;				
		}
		len = j;
		memcpy(src,buf1,len);
	}else{
			j =0;
			i-=1;
		while(i>0){						
					buf1[j] =  buf[i] <<4 | buf[i-1];					
					printf("buf[%d] =  %x \r\n",i,buf1[j]);
					j++;
					if(i>2)
							i-=2;
					else 
						break;				
		}
		len = j;
		memcpy(src,buf1,len);	
	 }
		return len;
  //printf("hex temp %s \r\n",buf);
}

uint8_t  set_bd_car_num_hex(uint8_t *num,uint8_t *outbuf)
{
		uint32_t card_num;
		uint8_t buf[20],lenth;
	
		card_num = (num[0] - '0')*100000 + (num[1] - '0')*10000 + (num[2] -'0')*1000 +(num[3] -'0')*100+(num[4] -'0')*10 +(num[5] -'0');
		lenth = int_to_hex(card_num,buf);
		
		memcpy(outbuf,buf,lenth);
	
		return lenth;
	
}

extern uint8_t bd1_exist;
extern  uint8_t poweroff_flag ;
extern uint8_t charger_light;
extern uint8_t response_location[150];
extern uint16_t bd_send_resp_len;
extern uint8_t scan_begin_flag;
extern uint8_t bdinfo_count;
extern uint8_t  wlist_continue_send_flag;
uint8_t  wlist_continue_send_flag_count=0;
uint8_t  wlist_continue_send_flag_count1=0;
extern uint8_t zdev_bind_flag;
uint8_t save_flas =0;
uint8_t ubind_all_flag =0;
extern uint8_t save_nv_buf[70];
extern  uint8_t bbind_num;
//uint8_t temp_buf_11[38]=" bbind:132009220010,01,01,201025,ok##";
void mytestTask1(void const * argument)
{
		uint8_t i =0;

	while(1){
		//printf("mytestTask1---1\r\n");
		
		osDelay(500);
		if(save_flas){
			save_dev_addd_nv();
			save_nv_buf[2] = bbind_num;
			set_nvram_save_data(save_nv_buf);
			save_flas =0;
		}
		
		if(ubind_all_flag){
		  ubind_all_flag=0;
			clear_dev_nv();
			memset(save_nv_buf,0x00,70);
			set_nvram_save_data(save_nv_buf);
		}
	//ble_bind_back(temp_buf_11,38);
	#if 1
		if(charger_light==1){
		
			send_count_flag9 ++;
			
		}
	
		if(send_count_flag9 == 7)
		{
		HAL_GPIO_WritePin(CHARGE_KEY_GPIO_Port, CHARGE_KEY_Pin, GPIO_PIN_SET);
			//HAL_GPIO_WritePin(CHARGE1_KEY_GPIO_Port, CHARGE1_KEY_Pin, GPIO_PIN_SET);
					osDelay(100);
					HAL_GPIO_WritePin(CHARGE_KEY_GPIO_Port, CHARGE_KEY_Pin, GPIO_PIN_RESET);
		//	HAL_GPIO_WritePin(CHARGE1_KEY_GPIO_Port, CHARGE1_KEY_Pin, GPIO_PIN_RESET);
			osDelay(100);
			HAL_GPIO_WritePin(CHARGE_KEY_GPIO_Port, CHARGE_KEY_Pin, GPIO_PIN_SET);
			//HAL_GPIO_WritePin(CHARGE1_KEY_GPIO_Port, CHARGE1_KEY_Pin, GPIO_PIN_SET);
					osDelay(100);
					HAL_GPIO_WritePin(CHARGE_KEY_GPIO_Port, CHARGE_KEY_Pin, GPIO_PIN_RESET);
		//	HAL_GPIO_WritePin(CHARGE1_KEY_GPIO_Port, CHARGE1_KEY_Pin, GPIO_PIN_RESET);
		charger_light =0;
			send_count_flag9 =0;
		}
		
		if(poweroff_flag==1){
				HAL_GPIO_WritePin(CHARGE_KEY_GPIO_Port, CHARGE_KEY_Pin, GPIO_PIN_SET);
		//	HAL_GPIO_WritePin(CHARGE1_KEY_GPIO_Port, CHARGE1_KEY_Pin, GPIO_PIN_SET);
					osDelay(100);
					HAL_GPIO_WritePin(CHARGE_KEY_GPIO_Port, CHARGE_KEY_Pin, GPIO_PIN_RESET);
			//HAL_GPIO_WritePin(CHARGE1_KEY_GPIO_Port, CHARGE1_KEY_Pin, GPIO_PIN_RESET);
			osDelay(100);
			HAL_GPIO_WritePin(CHARGE_KEY_GPIO_Port, CHARGE_KEY_Pin, GPIO_PIN_SET);
		//	HAL_GPIO_WritePin(CHARGE1_KEY_GPIO_Port, CHARGE1_KEY_Pin, GPIO_PIN_SET);
					osDelay(100);
					HAL_GPIO_WritePin(CHARGE_KEY_GPIO_Port, CHARGE_KEY_Pin, GPIO_PIN_RESET);
			//HAL_GPIO_WritePin(CHARGE1_KEY_GPIO_Port, CHARGE1_KEY_Pin, GPIO_PIN_RESET);
			
			app_power_close();	
		}
		
		if(send_count_flag10 ==8){
					HAL_GPIO_WritePin(CHARGE_KEY_GPIO_Port, CHARGE_KEY_Pin, GPIO_PIN_SET);
			//HAL_GPIO_WritePin(CHARGE1_KEY_GPIO_Port, CHARGE1_KEY_Pin, GPIO_PIN_SET);
					osDelay(100);
					HAL_GPIO_WritePin(CHARGE_KEY_GPIO_Port, CHARGE_KEY_Pin, GPIO_PIN_RESET);
			//HAL_GPIO_WritePin(CHARGE1_KEY_GPIO_Port, CHARGE1_KEY_Pin, GPIO_PIN_RESET);
			osDelay(100);
			HAL_GPIO_WritePin(CHARGE_KEY_GPIO_Port, CHARGE_KEY_Pin, GPIO_PIN_SET);
			//HAL_GPIO_WritePin(CHARGE1_KEY_GPIO_Port, CHARGE1_KEY_Pin, GPIO_PIN_SET);
					osDelay(100);
					HAL_GPIO_WritePin(CHARGE_KEY_GPIO_Port, CHARGE_KEY_Pin, GPIO_PIN_RESET);
		//	HAL_GPIO_WritePin(CHARGE1_KEY_GPIO_Port, CHARGE1_KEY_Pin, GPIO_PIN_RESET);
		
		}
		
		if(zdev_set.zbattery ==0 || bd1_exist ==1){
				set_rd_red_light(1);
			bd1_exist = 0xff;
		}

		//if((zdev_set.bdevtask2_num == 0) && (zdev_set.bdevtask1_num == 0))
							//set_ble_red_light(1);
		
		if(zdev_charger_status == 1){
				if(charger_status_flag ==0){
				memcpy(charger_info_buf,"+ZCHARGE:ON",11);
				memcpy(&charger_info_buf[11],aoa_at_end_tok,4);
				aoa_send_response(charger_info_buf,15);
						HAL_GPIO_WritePin(CHARGE_KEY_GPIO_Port, CHARGE_KEY_Pin, GPIO_PIN_SET);
					osDelay(100);
					HAL_GPIO_WritePin(CHARGE_KEY_GPIO_Port, CHARGE_KEY_Pin, GPIO_PIN_RESET);
				//set_charge_light1();
					charger_status_flag ++;
					set_charge_key_high();
		
					printf("open charger 0\r\n");
				}else if(charger_status_flag ==1){
					
			//	set_charge_key_low42();
					charger_status_flag =0;
					zdev_charger_status = 0xff;
					printf("open charger 11111111111111111111111111111111111111111111\r\n");
				}
		}else if(zdev_charger_status == 0){
		if(charger_status_flag ==0){
				memcpy(charger_info_buf,"+ZCHARGE:OFF",12);
				memcpy(&charger_info_buf[12],aoa_at_end_tok,4);
				aoa_send_response(charger_info_buf,16);
			
			HAL_GPIO_WritePin(CHARGE_KEY_GPIO_Port, CHARGE_KEY_Pin, GPIO_PIN_SET);
					osDelay(100);
					HAL_GPIO_WritePin(CHARGE_KEY_GPIO_Port, CHARGE_KEY_Pin, GPIO_PIN_RESET);
			osDelay(100);
			HAL_GPIO_WritePin(CHARGE_KEY_GPIO_Port, CHARGE_KEY_Pin, GPIO_PIN_SET);
					osDelay(100);
					HAL_GPIO_WritePin(CHARGE_KEY_GPIO_Port, CHARGE_KEY_Pin, GPIO_PIN_RESET);
			
			charger_status_flag ++;
			
			printf("close charger 0\r\n");
		}else if(charger_status_flag ==1){
			set_charge_key_low();
			//set_charge_key_low42();
			osDelay(500);
			set_charge_key_high42();
					charger_status_flag =0;
			zdev_charger_status = 0xff;
			printf("00000000000000000000000000000000000close charger 11111111111111111\r\n");
				}
		
		}
		
//	printf("ft_dev.soskey_flags = %d \r\n",ft_dev.soskey_flags);
	if(open_airmode ==1){
	
		send_count_flag7++;
	}
		if(send_count_flag7 ==60){
		app_power_close();
		send_count_flag7 = 0;
		}
		if(ft_dev.soskey_flags ==1)
				send_count_flag6++;
		
		if(send_count_flag6 ==3){
				 hw_bd1_open();
		}
		if(send_count_flag6 ==5){
				ft_dev.soskey_flags = 0;
				
			bd1_receiver_num = (zdev_set.rn_num[0]-'0') *100000 + (zdev_set.rn_num[1]-'0')*10000 + (zdev_set.rn_num[2] - '0')*1000 + (zdev_set.rn_num[3]-'0')*100+ (zdev_set.rn_num[4] -'0')*10 + (zdev_set.rn_num[5] -'0');
			//aoa_at_handle_bsossend();
			printf("bd1_receiver_num = %d \r\n",bd1_receiver_num);
			if(bd1_receiver_num != 927237)
				bd1_receiver_num = 927237;
				send_count_flag6 = 0;
			 
			memcpy(bd1_send_sos_num,"00",2);
			bd1_send_sos_num[1] = zdev_set.report_mesage_id + '0';
			
			rd_send_msg(bd1_receiver_num,response_location,bd_send_resp_len);
			
			aoa_at_handle_bsossend();
			
			send_count_flag8 = 1;
			
		}
		if(send_count_flag8 >0)
				send_count_flag8++;
		
		if(send_count_flag8 ==10){
			hw_bd1_close();
			send_count_flag8 = 0;
		}
	// aoa_at_handle_bsossend();
		

		
		
		if((send_count_flag2 ==3)&&(bd1_exist ==0)){

				send_bd1_get_card_num();
				bd1_exist = 0xff;
			send_count_flag3 = 0;
			
			
		}else if((send_count_flag2 ==5)&&(send_count_flag3 ==0)){
					send_count_flag2 =0;
					send_count_flag3 = 1;
			printf("rd_receive \r\n");
			 rd_receive_and_handle();
		}

		
		if(zdev_isbind){
			if(send_count_flag1 ==300){
			if(!zdev_bind_flag){
				//set_ble_power_off();
				//osDelay(100);
				//set_ble_power();	
				//MX_USART4_UART_Init();
					send_count_flag1 = 0;
				//osDelay(500);
				 //cp_taskid_temp();
				// send_wlist_to_ble();
			}else{
				send_count_flag1 = 0;
			}
				//aoa_at_handle_taskbegin("taskbegin0",10,1);
			}

		if(send_count_flag == 60){
				aoa_at_handle_bbeatnow();
			//task_gps_message_put(0x02);
		}
		if(send_count_flag == 80){
				//aoa_at_handle_bpalert();
			
		}
		if(send_count_flag == 13){
			rn_receive_and_handle();
			}
		if(send_count_flag == 200){
			aoa_at_handle_zlocation();
			send_count_flag =0;
			BatTimerCallback();
		}else{
			send_count_flag ++;
		}
		send_count_flag1 ++;
		}else{
				if(send_count_flag == 100){
			send_count_flag =0;
			BatTimerCallback();
		}else{
				send_count_flag ++;
		}
		
		
		
		}
		
		if(send_count_flag5 ==20 ){
			//	BatTimerCallback();   //sunmny
				send_count_flag5 = 0;
			}
		
			send_count_flag5++;
	//if(send_count_flag5 ==3)
			
	
		if(send_count_flag3 ==0)
			      send_count_flag2 ++;
		
			send_count_flag10 ++;
	if(send_count_flag10 >100)
			send_count_flag10 = 10;
	#endif
	}
	


}