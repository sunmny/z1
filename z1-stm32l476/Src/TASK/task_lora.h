#ifndef TASK_LORA_H
#define TASK_LORA_H

#include "stdint.h"
#include "cmsis_os.h"

void LoraTask(void const * argument);

void lora_send2(void);

	void lora_send3(void);
	
	osStatus task_lora_message_put(uint32_t info);

#endif
