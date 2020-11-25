#ifndef APP_BLE_H
#define APP_BLE_H

#include <stdint.h>
#include "stm32l4xx_hal.h"

#define BLE_UART_DATA_LOOP_SIZE 512
#define BLE_MAIL_BUF_LEN 256

extern uint8_t ble_uart_data_loop[BLE_UART_DATA_LOOP_SIZE+1];

typedef uint8_t (*bleathandle)(uint8_t*, uint16_t, uint8_t);

typedef struct {
	bleathandle handle;
    uint8_t *pData;
}ble_at_symbol;


typedef struct {
	uint16_t len;
	uint8_t data[BLE_MAIL_BUF_LEN];
}blemail;

typedef struct {
	uint16_t len;
	uint8_t data[BLE_MAIL_BUF_LEN];
}mytestmail;


struct bdevice_status{
		uint8_t rd_flags ;
		uint8_t rn_flags ;
		uint8_t sd_flags ;
		uint8_t gprs_flags;
		uint8_t pwkey_flags;
		uint8_t soskey_flags;
		uint8_t chargrkey_flags;
		uint8_t light_flags;
		uint8_t ps_flags;
		uint8_t hall_flags ;
		uint8_t lock_flags ;
		uint8_t moto_flags;
		uint8_t lcd_flags ;
		uint8_t lcdkey_flags;
		uint8_t writesn_flags ;
		uint8_t test_flags ;

	};

	
extern struct bdevice_status ft_dev;



extern void buzzer_timer_en(uint8_t en);
extern void BuzzerTimerCallback(void const * argument);

extern uint8_t ble_receive_and_handle(void);
extern void ble_test_send_data(void);
extern void ble_data_received(void);
extern uint8_t ble_mail_receive_and_handle(blemail *pmail);
#endif
