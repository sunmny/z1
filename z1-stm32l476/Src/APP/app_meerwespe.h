#ifndef APP_MEERWESPE_H
#define APP_MEERWESPE_H

#include <stdint.h>


typedef enum WORK_SEND
{
	  no_send = 0,
	  call_send,
	  sos_send,
	  huxi_send,
	  
}WORK_SEND_MODE;

typedef enum GPS
{
		gps_close = 0,
	  gps_open,
}GPS_MODE;

typedef enum LORA
{ 
		lora_close = 0,
	  lora_open,
	
}LORA_MODE;


char strstr3(char *s1,char *s2);
uint8_t app_init(void);
char *strchr2(char *s,char c,uint16_t count);
//s2在s1中第一次出现的我位置

char *strstr2(char *s1,char *s2) ;

void * memcpy2(char *strDest, const char *strSrc, int Count);



#endif
