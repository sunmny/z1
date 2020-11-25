#ifndef APP_LORA_H
#define APP_LORA_H
#include <stdint.h>
	
	typedef enum error
{ 
		OK = 0,
	  ER01,
	ER02,
	ER03,
	ER04,
	ER05,
	ER06,
	ER07,
	ER08,
	ER09,
	
	
	ERREC =0xff,

	
	
}LORA_ERROR;
	
uint8_t app_lora_send(uint8_t send_type);

uint8_t app_lora_get_sn(uint8_t *sn_raw,uint8_t *sn_len);

void app_lora_mutex_enter();
void app_lora_mutex_exit();

#endif


