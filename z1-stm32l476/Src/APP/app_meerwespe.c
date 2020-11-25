#include "myconfig.h"
#include "gui_meerwespe.h"
#include "app_meerwespe.h"
#include "stm32l4xx_hal.h"
#include "cmsis_os.h"
#include "hw_meerwespe.h"
#include "hw_config.h"

#include "gui_message.h"
#include "hw_charge.h"
#include "hw_pwm.h"
#include "app_power.h"
#include "delay.h"
#include "app_lcd.h"
#include "task_lora.h"
#include "app_aoa.h"


static void delay_ms(uint32_t ms)
{
		int i;
		for(i = 0;i<ms;i++);

}
uint8_t bat_val1,bat_val2;
uint8_t bat_count;
uint8_t zdev_charger_status = 0xff;
extern uint8_t bat_zdev_soc ;
extern uint8_t hw_bd1_is_exist(void);
//extern uint8_t bd1_exist_status ;
//extern uint8_t bd1_exist_status_bak;
uint8_t  app_init(void)
{
	uint8_t i=0, flag =0;
	zdev_set.status.zrd_status = 0;
	zdev_set.status.zrn_status = 0;
	zdev_set.status.zrd_test_status = 0;
	zdev_set.status.zrn_test_status = 0;
	zdev_set.zbattery = 1;
	zdev_set.bdevtask2_num =0;
	zdev_set.bdevtask1_num =0;
	zdev_set.report_dis=5;
	zdev_set.report_bat_threshold =1;
	zdev_set.isMdev=0;
	zdev_set.report_mesage_id =2;
	
	zdev_charger_status = 0xff;
	
	memset(zdev_set.bbinddev.bdev_id,0x00,sizeof(zdev_set.bbinddev.bdev_id));
	//init hw
	hw_gpio_init();
  hw_i2c_init();
	hw_dma_init();
	hw_uart_init();
	hw_exti_init();
	delay_ms(10000);
		cw_init();
	delay_ms(10000000);
	app_power_open();
   //if(bat_val1 >= 0x2b ){
		//app_power_open();
		//}else if(bat_val1 <0x2b || bat_val1 == 0){
		//hw_buzzer_enable(0);
		//	app_power_close() ;
		//	return 1;
			
	//	}
	
//	bd1_exist_status = hw_bd1_is_exist();
	//bd1_exist_status_bak = hw_bd1_is_exist();
	#if 0
	while(1){
		
	   if(bat_val1 >= 0x2b){
			app_power_open();
			 break;
		 }else{
				zdev_read_battery_adc();
		 }
	
	}

#endif
		printf("bd2 init \r\n");
	  hw_bd2_init();
		printf("bd1 init \r\n");
    hw_bd1_init();
	//}
	//hw_lora_init();
		set_rn_blue_light(1);
		set_ble_red_light(1);
	
	return 0;

	
}


char *strchr2(char *s,char c,uint16_t count)
  {
		    uint16 i =0;
//     char*p=s;
    //  while(*s && *s!=c)
		while(*s)
		{
		//	if(*s!=c)
     //     s++;
     if(*s==c)
		        i++;
	      if(i==count)
         return s;
				s++;
			}
				return NULL; 
 }
//s2是否在s1  
 char strstr3(char *s1,char *s2)  
{  
    int n;  
    if (*s2)                      //??????  
    {  
        while(*s1)                 
        {  
            for (n=0;*(s1+n)==*(s2+n);n++)  
            {  
                if (!*(s2+n+1))            //???????????'\0'  
                {  
                    return 0;  
                }  
            }  
            s1++;  
        }  
        return NULL;  
    }  
    else  
    {  
        return 1;  
    }  
} 
 
 
//s2在s1中第一次出现的我位置  空格作为结束

char *strstr2(char *s1,char *s2)  
{  
    int n;  
    if (*s2)                      //??????  
    {  
        while(*s1)                 
        {  
            for (n=0;*(s1+n)==*(s2+n);n++)  
            {  
                if (!*(s2+n+1))            //???????????'\0'  
                {  
                    return (char*)s1;  
                }  
            }  
            s1++;  
        }  
        return NULL;  
    }  
    else  
    {  
        return (char*)s1;  
    }  
} 


void * memcpy2(char *strDest, const char *strSrc, int Count)
{
	    unsigned char *tempDest;
    unsigned char *tempSrc;
    if(strDest == NULL || strSrc == NULL || Count <= 0) return NULL;

    tempDest = (unsigned char *)tempDest;
    tempSrc = (unsigned char *)tempSrc;
    while(Count--)
    {
        *tempDest = *tempSrc;
        tempDest++;
        tempSrc++;
    }
    return strDest;
}







