#include "app_meerwespe.h"
#include "task_meerwespe.h"
#include "stm32l4xx_hal.h"
#include "cmsis_os.h"

#include "task_refresh.h"
#include "task_sensor.h"
#include "task_gps.h"
#include "task_lora.h"
#include "task_timer.h"
#include "task_aoauart.h"
#include "task_ble.h"
#include "task_bd1.h"

#include "app_power.h"
#include "app_ble.h"
#include "app_aoa.h"

//#define WorkQueueLen 16
#define GpsQueueLen  10

//#define AoaUartQueueLen 10
//#define BleQueueLen 10

//#define AoaUartMailQueueLen 2
//#define BleMailQueueLen 2
#define mytestQueueLen 8

osThreadId GpsHandle;
osThreadId Bd1Handle;

osThreadId AoaUartHandle;
osThreadId BleHandle;
osThreadId mytestHandle;
osThreadId mytestHandle1;
osMessageQId GpsQueueHandle;

//osMessageQId AoaUartQueueHandle;
//osMessageQId BleQueueHandle;

//osMailQId    AoaUartMailQueueHandle;
//osMailQId    BleMailQueueHandle;
osMailQId    mytestQueueHandle;

//osTimerId PowerTimerHandle;
osTimerId LedTimerHandle;
//osTimerId BatTimerHandle;
//osTimerId testTimerHandle;
//osTimerId rdTimerHandle;
//osTimerId ReportTimerHandle;


extern void mytestTask(void const * argument);
extern void mytestTask1(void const * argument);

extern void devinfo_report_task(void const * argument);
extern void devinfo_save_task(void const * argument);

uint8_t save_nv_buf[70]; /* 10 begin rd num rd num2****/
extern uint8_t addr_nv_buf[1500];
uint8_t airmod_flag =0;
extern uint8_t send_wl_temp[100][15];
uint8_t contiue_task =0;
uint8_t gsn_buf[12] ={0};

 TaskHandle_t bletask = NULL;
 TaskHandle_t aoatask = NULL;
 TaskHandle_t devreporttask = NULL;
 TaskHandle_t devsavetask = NULL;
uint8_t send_mac_buf[20];
extern void LedTimerCallback(void const * argument);
//extern void ReportTimerCallback(void const * argument);
extern uint8_t group_num_data;
void task_init(void)
{

  /* USER CODE BEGIN 2 */
	uint8_t flag =0,i = 0,j=0;


 // osTimerDef(PowerTimer, PowerTimerCallback);
 // PowerTimerHandle = osTimerCreate(osTimer(PowerTimer), osTimerOnce, NULL);
    osTimerDef(LedTimer, LedTimerCallback);
    LedTimerHandle = osTimerCreate(osTimer(LedTimer), osTimerPeriodic, NULL);
	
	 //   osTimerDef(BatTimer, BatTimerCallback);
  //BatTimerHandle = osTimerCreate(osTimer(BatTimer), osTimerPeriodic, NULL);
	
	//osTimerDef(ReportTimer, ReportTimerCallback);
  //ReportTimerHandle = osTimerCreate(osTimer(ReportTimer), osTimerPeriodic, NULL);
	
	// osTimerDef(rdTimer, rdTimerCallback);
  //rdTimerHandle = osTimerCreate(osTimer(rdTimer), osTimerPeriodic, NULL);
	
	  // osTimerDef(testTimer, testTimerCallback);
   // testTimerHandle = osTimerCreate(osTimer(testTimer), osTimerPeriodic, NULL);
  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the thread(s) */
  /* definition and creation of Gps */
	osThreadDef(my_test, mytestTask, osPriorityNormal, 0, 128);
   mytestHandle = osThreadCreate(osThread(my_test), NULL);
	 
	osThreadDef(my_test1, mytestTask1, osPriorityNormal, 0, 512);
   mytestHandle1 = osThreadCreate(osThread(my_test1), NULL);
	//osThreadDef(thread_sd, SdTask, osPriorityNormal, 0, 128);
  //SdHandle = osThreadCreate(osThread(thread_sd), NULL);
	
	
  osThreadDef(Gps, GpsTask, osPriorityNormal, 0, 128);
  GpsHandle = osThreadCreate(osThread(Gps), NULL);

  /* definition and creation of Bd1 */
 // osThreadDef(Bd1, Bd1Task, osPriorityNormal, 0, 128);
  //Bd1Handle = osThreadCreate(osThread(Bd1), NULL);

  /* definition and creation of AoaUart */
  //osThreadDef(AoaUart, AoaUartTask, osPriorityNormal, 0, 512);
 // AoaUartHandle = osThreadCreate(osThread(AoaUart), NULL);

  /* definition and creation of Ble */
  //osThreadDef(Ble, BleTask, osPriorityNormal, 0, 512);
  //BleHandle = osThreadCreate(osThread(Ble), NULL);
	xTaskCreate( AoaUartTask, "Task2",1024, NULL, tskIDLE_PRIORITY, &aoatask );

	xTaskCreate( BleTask, "Task3",512, NULL, tskIDLE_PRIORITY, &bletask );

	xTaskCreate( devinfo_report_task, "Task5",512, NULL, tskIDLE_PRIORITY, &devreporttask );
	
	xTaskCreate( devinfo_save_task, "Task6",256, NULL, tskIDLE_PRIORITY, &devsavetask );
  /* definition and creation of GPSQueue */
  osMessageQDef(GpsQueue, GpsQueueLen, uint16_t);
  GpsQueueHandle = osMessageCreate(osMessageQ(GpsQueue), NULL);

  /* definition and creation of AoaUartQueue */
 // osMessageQDef(AoaUartQueue, AoaUartQueueLen, uint16_t);
 // AoaUartQueueHandle = osMessageCreate(osMessageQ(AoaUartQueue), NULL);

    /* definition and creation of BleUartQueue */
 // osMessageQDef(BleQueue, BleQueueLen, uint16_t);
 // BleQueueHandle = osMessageCreate(osMessageQ(BleQueue), NULL);


 // osMailQDef(AoaUartMailQueue, AoaUartMailQueueLen, aoamail);
  //AoaUartMailQueueHandle = osMailCreate(osMailQ(AoaUartMailQueue), NULL);


//  osMailQDef(BleMailQueue, BleMailQueueLen, blemail);
 // BleMailQueueHandle = osMailCreate(osMailQ(BleMailQueue), NULL);
	
	
	  osMailQDef(mytestQueue, mytestQueueLen, mytestmail);
  mytestQueueHandle = osMailCreate(osMailQ(mytestQueue), NULL);



	//	osTimerStart(BatTimerHandle,50000);
	
		
		
		get_nvram_sn(gsn_buf);
		memcpy(zdev_set.mydev_id,gsn_buf,12);
		printf("gsn_buf = %s \r\n",gsn_buf);
		
		memset(&zdev_set.bbinddev.bdev_id[0][0][0],0x00,1300);
		memset(&zdev_set.bbinddev.bdev_id[1][0][0],0x00,1300);
		
		memcpy(send_mac_buf,"writedev:",9);
		memcpy(&send_mac_buf[9],gsn_buf,12);
		memcpy(&send_mac_buf[21],"##",2);
		ble_send_response(send_mac_buf,23);
		
	#if 0
		memset(save_nv_buf,0x00,70);
				
		get_nvram_save_data(save_nv_buf);
			printf("save_nv_buf %s\r\n",save_nv_buf);
		flag = save_nv_buf[0] -'0';
	printf("get nv flag = %d \r\n",flag);
		if(flag ==1){
			
			zdev_set.isMdev = save_nv_buf[1] -'0';
			
			memcpy(zdev_set.rn_num,&save_nv_buf[10],6);
			memcpy(zdev_set.rd_num,&save_nv_buf[16],6);
			zdev_set.report_mesage_id = save_nv_buf[22] - '0'; 
			set_ble_red_light(0);
			printf("zdev_set.isMdev = %d \r\n",zdev_set.isMdev);
			contiue_task = 1;
			get_nvram_id(addr_nv_buf);
			for(i = 0;i<100;i++)
			{
				memcpy(&send_wl_temp[i][0],&addr_nv_buf[i*15],15);
				if(addr_nv_buf[(i*12)+12] == '1'){
					memcpy(&zdev_set.bbinddev.bdev_id[0][i][0],&addr_nv_buf[i*15],13);
					printf("nv id = %s \r\n",zdev_set.bbinddev.bdev_id[0][i]);
					zdev_set.bdevtask1_num =zdev_set.bdevtask1_num+1;
//					printf("zdev_set.bdevtask1_num = %d \r\n",zdev_set.bdevtask1_num );
				}else if(addr_nv_buf[(i*12)+12] == '2'){
					memcpy(&zdev_set.bbinddev.bdev_id[1][i][0],&addr_nv_buf[i*15],13);
					printf("nv id = %s \r\n",zdev_set.bbinddev.bdev_id[0][i]);
					zdev_set.bdevtask2_num =zdev_set.bdevtask2_num+1;
				}
			}
			copy_addr_group();
			group_num_data = save_nv_buf[69];
			if(!get_timer_status()){
			   osTimerStart(LedTimerHandle,5000);
			   set_timer_status(1);
			}
			//	aoa_at_handle_taskbegin("taskbegin0",10,1);
		}	else{
		memset(save_nv_buf,0x00,70);
		}
		printf("get nv flag ok\r\n");
		#endif
}

void send_notify_ble_task(void)
{
	 BaseType_t xHigherPriorityTaskWoken = pdFALSE;
		vTaskNotifyGiveFromISR(bletask,&xHigherPriorityTaskWoken );  
 
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken );  
}
void send_notify_aoa_task(void)
{
	 BaseType_t xHigherPriorityTaskWoken = pdFALSE;
		vTaskNotifyGiveFromISR(aoatask,&xHigherPriorityTaskWoken );  
 
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken );  
}
