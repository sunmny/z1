#include "hw_uart.h" 
#include "stm32l4xx_hal.h"
//#include "cmsis_os.h"
//#include "string.h"
//#include "delay.h"
#include "app_bd1.h"
#include "string.h"
#include "hw_config.h"

uint8_t rd_num[6] = {0};

static uint8_t out_iccheck_icjx[12] = { 0x24, 0x49, 0x43, 0x4A, 0x43, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x2B };
static uint8_t out_system_xtzj[13] = { 0x24, 0x58, 0x54, 0x5A, 0x4A, 0x00, 0x0D, 0x00, 0x00, 0x00, 0x00, 0x03, 0x36 };

uint8_t bd1_tx_buf[BD1_TX_BUF_SIZE];
uint8_t bd1_rx_buf[BD1_RX_BUF_SIZE];
uint8_t byte_TXSQ[BD1_TX_BUF_SIZE];
uint16_t bd1_rx_len;

static data_icxx icxx;
static data_zjxx zjxx;

//for debug
#ifdef BD1_MSG_SEND_DEBUG
uint16_t zjxx_count =1;

uint8_t bd1_msg_debug[5]={0x68, 0x65, 0x6c, 0x6c, 0x6f};//hello
uint16_t bd1_msg_debug_len = 5;
#endif

uint8_t onCheckByte(uint8_t *bytein, uint16_t len) 
{
		uint8_t byteout = 0;
		uint16_t i;
		for (i = 0; i < len; i++) {
			byteout = (uint8_t) (byteout ^ bytein[i]);
		}
		return byteout;
}

uint8_t rd_iccheck_icjx(void)
{
	uint8_t ret=0;
	memset(bd1_tx_buf, 0x00, sizeof(bd1_tx_buf));
	memcpy(bd1_tx_buf, out_iccheck_icjx, sizeof(out_iccheck_icjx));
	ret=hw_uart2_send(bd1_tx_buf, sizeof(out_iccheck_icjx));
	return ret;
}

uint8_t rd_system_xtzj(void)
{
	uint8_t ret=0;
	memset(bd1_tx_buf, 0x00, sizeof(bd1_tx_buf));
	memcpy(bd1_tx_buf, out_system_xtzj, sizeof(out_system_xtzj));
	ret=hw_uart2_send(bd1_tx_buf, sizeof(out_system_xtzj));
	return ret;
}

static uint16_t txsq_msg_package(uint32_t int_bdnum, uint8_t *data, uint16_t len)
{
	uint16_t total_len=16+len+1+1; //header len 17 bytes, hunfa flag(A4) len 1 byte, checksum 1 byte
	uint16_t data_len_bit=(len+1)*8;//len:data length, 1:hunfa flag (A4)
	uint16_t i=0;
	byte_TXSQ[0] =  0x24;
	byte_TXSQ[1] =  0x54;
	byte_TXSQ[2] =  0x58;
	byte_TXSQ[3] =  0x53;
	byte_TXSQ[4] =  0x51;
	byte_TXSQ[5] =  (uint8_t)((total_len>>8) & 0xff);
	byte_TXSQ[6] =  (uint8_t)(total_len & 0xff);
	byte_TXSQ[7] =  0x00;
	byte_TXSQ[8] =  0x00;
	byte_TXSQ[9] =  0x00;
	byte_TXSQ[10] =  0x46;// 不同1： 通信类别0100 0110 代码
	byte_TXSQ[11] = (uint8_t) ((int_bdnum >> 16) & 0xff);// 11 12 13用户地址
	byte_TXSQ[12] = (uint8_t) ((int_bdnum >> 8) & 0xff);
	byte_TXSQ[13] = (uint8_t) (int_bdnum&0xff);

	byte_TXSQ[14] = (uint8_t) ((data_len_bit >> 8) & 0xff);// 电文长度
	byte_TXSQ[15] = (uint8_t) (data_len_bit&0xff);
	byte_TXSQ[16] = 0x00;// 是否应答

	//byte_TXSQ[17] = 0xA4;

	for (i = 0; i < (len+1); i++)
	{
		byte_TXSQ[17+i] = data[i];
	}
	byte_TXSQ[total_len-1] = (uint8_t) onCheckByte(byte_TXSQ, total_len-1);
	return total_len;
}

uint8_t rd_send_msg(uint32_t int_bdnum, uint8_t *data, uint16_t len)
{
	uint8_t ret=0;
	uint16_t total_len=txsq_msg_package(int_bdnum,data,len);
	ret=hw_uart2_send(byte_TXSQ, total_len);
	return ret;
}

void handle_rd_rx_icxx(uint8_t * data, uint16_t len)
{
	if(data[7]==0x1f && data[8]==0xff && data[9]==0xff)
	{
		icxx.card_num=0;
		icxx.card_freq=0;
	}
	else
	{
		icxx.card_num= ((uint32_t) ((data[7] & 0xff) << 16)
			+ ((uint32_t) (data[8] & 0xff) << 8) 
			+ (uint32_t) (data[9] & 0xff));
		icxx.card_freq= (((uint16_t) (data[15] & 0xff) << 8) + (uint16_t) (data[16] & 0xff));// 频度
	}
}

void handle_rd_rx_zjxx(uint8_t * data, uint16_t len)
{
	if(data[7]==0x1f && data[8]==0xff && data[9]==0xff)
	{
		zjxx.card_num=0;
		zjxx.power_one = 0;
		zjxx.power_two=  0;
		zjxx.power_three=  0;
		zjxx.power_four=  0;
		zjxx.power_five=  0;
		zjxx.power_six=  0;
#ifdef BD1_MSG_SEND_DEBUG
		zjxx_count=0;
#endif		
	}
	else
	{
		zjxx.card_num= ((uint32_t) ((data[7] & 0xff) << 16)
			+ ((uint32_t) (data[8] & 0xff) << 8) 
			+ (uint32_t) (data[9] & 0xff));
		zjxx.power_one =  data[14];
		zjxx.power_two=  data[15];
		zjxx.power_three=  data[16];
		zjxx.power_four=  data[17];
		zjxx.power_five=  data[18];
		zjxx.power_six=  data[19];
	}
}

void handle_rd_rx_fkxx(uint8_t * data, uint16_t len)
{
	
}

void handle_rd_rx_txxx(uint8_t * data, uint16_t len)
{
	hw_uart1_send_dma(data, len);
}

extern uint8_t send_count_flag3;
void rd_rx_data_handle(uint8_t * data, uint16_t len)
{
	uint8_t checksum=0;
	if (len < 5) return;
	checksum = onCheckByte(data, len-1);
	

	if(memcmp(data,"$BDICI",6) ==0){
		
			memset(rd_num ,0x00,6);
			memcpy(rd_num,&data[8],6);
		if(memcmp("097151",rd_num,6) == 0){
				task_test_mail_put("nobd1",5);
			set_rd_red_light(1);
			}else{
			task_test_mail_put("inbd1",5);
					send_count_flag3 = 1;
					set_rd_red_light(0);
			}
			//printf("rd_num %s \r\n",rd_num);
	}
	if (checksum!=data[len-1]) return;
	if (data[0]==(uint8_t) 0x24 
		&& data[1] == (uint8_t) 0x5A 
		&& data[2] == (uint8_t) 0x4A 
		&& data[3] == (uint8_t) 0x58 
		&& data[4] == (uint8_t) 0x58)
		handle_rd_rx_zjxx(data,len);
	else if (data[0] == (uint8_t) 0x24 
		&& data[1] == (uint8_t) 0x49
		&& data[2] == (uint8_t) 0x43 
		&& data[3] == (uint8_t) 0x58 
		&& data[4] == (uint8_t) 0x58)
		handle_rd_rx_icxx(data,len);
	else if (data[0] == (uint8_t) 0x24 
		&& data[1] == (uint8_t) 0x46 
		&& data[2] == (uint8_t) 0x4B 
		&& data[3] == (uint8_t) 0x58 
		&& data[4] == (uint8_t) 0x58) 
		handle_rd_rx_fkxx(data,len);
	else if (data[0] == (uint8_t) 0x24 
		&& data[1] == (uint8_t) 0x54 
		&& data[2] == (uint8_t) 0x58 
		&& data[3] == (uint8_t) 0x58 
		&& data[4] == (uint8_t) 0x58) 
		handle_rd_rx_txxx(data,len);
}

uint8_t get_rd_rssi(void)
{
	if(hw_bd1_get_power_status()==0)
		return 0;
	else
	{
		if(zjxx.power_one==4||zjxx.power_two==4||zjxx.power_three==4
			||zjxx.power_four==4||zjxx.power_five==4||zjxx.power_six==4)
			return 4;
		else if(zjxx.power_one==3||zjxx.power_two==3||zjxx.power_three==3
			||zjxx.power_four==3||zjxx.power_five==3||zjxx.power_six==3)
			return 3;
		else if(zjxx.power_one==2||zjxx.power_two==2||zjxx.power_three==2
			||zjxx.power_four==2||zjxx.power_five==2||zjxx.power_six==2)
			return 2;
		else if(zjxx.power_one==1||zjxx.power_two==1||zjxx.power_three==1
			||zjxx.power_four==1||zjxx.power_five==1||zjxx.power_six==1)
			return 1;
		else
			return 0;
	}
}


void rd_receive_and_handle()
{
	memset(bd1_rx_buf,0x00,BD1_RX_BUF_SIZE);
	hw_uart2_receive(bd1_rx_buf, &bd1_rx_len, 0xff);
	//hw_uart2_send("$B",2);
	printf("bd1_rx_buf %s \r\n",bd1_rx_buf);
	if(bd1_rx_len>0)
	{
		//HAL_GPIO_TogglePin(DEBUG_LED_1_GPIO_Port, DEBUG_LED_1_Pin);
		rd_rx_data_handle(bd1_rx_buf,bd1_rx_len);
		bd1_rx_len=0;
	}
}


