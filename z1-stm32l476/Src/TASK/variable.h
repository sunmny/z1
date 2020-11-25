#ifndef _VARIABLE_H
#define _VARIABLE_H
#include "includes.h"

typedef struct
{
	unsigned char event;
	unsigned char code;
	unsigned int value;
	unsigned char client;
	void* pData;
}Event_Type;



#ifdef _VARIABLE_DEFINE_
#define CPU_EXTERN      
#else
#define CPU_EXTERN          extern
#endif


#define EVENT_REFRESH_LCD   	0x00
//刷新队列
CPU_EXTERN  xQueueHandle    g_queue_refresh;




CPU_EXTERN  xQueueHandle    g_queue_lcd;        //此变量在LCD中创建


CPU_EXTERN  unsigned char g_batteryvoltage;
CPU_EXTERN  unsigned char g_batterylevel;
CPU_EXTERN  unsigned char g_batterycharge;
CPU_EXTERN  unsigned char g_network;
CPU_EXTERN  unsigned char g_signal;

#endif
