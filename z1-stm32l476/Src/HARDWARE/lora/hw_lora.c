#include "hw_lora.h" 
#include "hw_config.h"
#include "hw_uart.h"

//static uint8_t lora_state;

//指令集
//uint8_t at_id[]
/*
static const at_cmd_t at_short_cmds[] = {
    {"ATZ"       ,   ATZ_Handle       },
};

static const at_cmd_t at_que_cmds[] = {
    {"AT+ID?"    ,   AT_IdQHandle     },
#if APP_USE_AS_MODULE>0
    {"AT+NS?"    ,   AT_NsHandle      },
    {"AT+RX?"    ,   AT_RxHandle      },
    {"AT+TS?"    ,   AT_TsHandle      },
    {"AT+RS?"    ,   AT_RsHandle      },
#else
    {"AT+GPS?"   ,   AT_GpsHandle     },
#endif
    {"AT+BUG?"   ,   AT_BugHandle     }
};

static const at_cmd_t at_equ_cmds[] = {
    {"AT+ID="    ,   AT_IdSHandle     },
#if APP_USE_AS_MODULE>0
    {"AT+ALP="   ,   AT_AlpHandle     },
    {"AT+NET="   ,   AT_NetHandle     },
    {"AT+TX="    ,   AT_TxHandle      },
#endif
    {"AT+ELP="   ,   AT_ElpHandle     },
    {"AT+RTC="   ,   AT_RtcHandle     },
    {"AT+IO="    ,   AT_IOHandle      },
    {"AT+TTST="  ,   AT_TxTestHandle  },
    {"AT+RTST="  ,   AT_RxTestHandle  },
    {"AT+COMTST=",   AT_CommTestHandle}
};
*/


void hw_lora_power(GPIO_PinState state)
{	
  	//上电
	 HAL_GPIO_WritePin(LORA_POWER_ON_GPIO_Port, LORA_POWER_ON_Pin, state);	
}

void hw_lora_reset(GPIO_PinState state)
{
	//reset
   HAL_GPIO_WritePin(LORA_RESET_GPIO_Port, LORA_RESET_Pin, state);
}

void hw_lora_open(void)
{
   hw_lora_reset(GPIO_PIN_SET);	
   hw_lora_power(GPIO_PIN_SET);
	 //lora_state = 1;
}

void hw_lora_close(void)
{
   hw_lora_reset(GPIO_PIN_RESET);	
   hw_lora_power(GPIO_PIN_RESET);
	 //lora_state = 0;
}

//LED IO初始化
void hw_lora_init(void)
{    
   hw_lora_open();
}

uint8_t hw_lora_send(uint8_t *tx_buf, uint16_t tx_len)
{
   return hw_uart2_send(tx_buf,tx_len);
}


uint8_t hw_lora_receive(uint8_t *rx_buf, uint16_t *rx_len, uint32_t timeout)
{
   return hw_uart2_receive(rx_buf,rx_len,timeout);
}


