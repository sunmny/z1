#ifndef BD1_H
#define BD1_H

#include <stdint.h>
#include "stm32l4xx_hal.h"

#define BD1_TX_BUF_SIZE 256
#define BD1_RX_BUF_SIZE 512

typedef struct {
	uint32_t card_num;
	uint16_t card_freq;
} data_icxx;

typedef struct {
	uint8_t power_one;
	uint8_t power_two;
	uint8_t power_three;
	uint8_t power_four;
	uint8_t power_five;
	uint8_t power_six;
	//int card_status;
	uint32_t card_num;
} data_zjxx;


extern uint8_t bd1_tx_buf[BD1_TX_BUF_SIZE];
extern uint8_t bd1_rx_buf[BD1_RX_BUF_SIZE];

//#define BD1_MSG_SEND_DEBUG

//for debug
#ifdef BD1_MSG_SEND_DEBUG
extern uint16_t zjxx_count;
extern uint32_t bd1_receiver_num;
extern uint8_t bd1_msg_debug[5];//hello
extern uint16_t bd1_msg_debug_len;
#endif

extern uint8_t get_rd_rssi(void);
extern uint8_t rd_iccheck_icjx(void);
extern uint8_t rd_system_xtzj(void);
extern uint8_t rd_send_msg(uint32_t int_bdnum, uint8_t *data, uint16_t len);
extern void rd_receive_and_handle(void);
#endif
