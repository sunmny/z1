#ifndef APP_NVRAM_H
#define APP_NVRAM_H

#include <stdint.h>
#include "stm32l4xx_hal.h"
#include "hw_flash.h"

#define USER_NVRAM_START_ADDR 0x807F000
#define USER_NVRAM_SIZE 0x800  /*2K, one page*/

#define NV_LOW_BATTERY_ADDR 0x807F000
#define NV_LOW_BATTERY_SIZE_BYTE  70

#define NV_REPORT_PERIOD_ADDR 0x807F001
#define NV_REPORT_PERIOD_SIZE_BYTE 1

#define NV_AIRMODE_FLAG_ADDR 0x807F004
#define NV_AIRMODE_FLAG_SIZE_BYTE 1

#define NV_CENTER_ADDRESS_ADDR 0x807F005
#define NV_CENTER_ADDRESS_SIZE_BYTE 20


#define NV_RD_DEST_ADDR 0x807F025/*bd1 card number*/
#define NV_RD_DEST_SIZE_BYTE 4

#define NV_LTE_DEST_ADDR 0x807F029/*ip or www? need to check the length*/
#define NV_LTE_DEST_SIZE_BYTE 40

#define NV_SOS_DEFAULT_MSG_ADDR 0x807F069 /*need to check the length*/
#define NV_SOS_DEFAULT_MSG_SIZE_BYTE  40
#define NV_SN_DEFAULT_MSG_ADDR 0x807F109 /*need to check the length*/
#define NV_SN_DEFAULT_MSG_SIZE_BYTE 13

#define NV_ID_DEFAULT_MSG_ADDR 0x807F130 /*need to check the length*/
#define NV_ID_DEFAULT_MSG_SIZE_BYTE 1300
typedef struct{
	uint8_t low_battery;
	uint32_t report_period;
}user_nvram_t;
extern flash_status_t get_nvram_sn(uint8_t *sn);
extern flash_status_t set_nvram_sn_data(uint8_t *sn);
extern flash_status_t get_nvram_low_battery(uint8_t *battery);
extern flash_status_t set_nvram_low_battery(uint8_t battery);
extern flash_status_t get_nvram_report_period(uint32_t *period);
extern flash_status_t set_nvram_report_period(uint32_t period);
extern flash_status_t get_nvram_center_address(uint8_t *address);
extern flash_status_t set_nvram_center_address(uint8_t *address);
extern flash_status_t get_nvram_rd_dest(uint32_t *dest);
extern flash_status_t get_nvram_lte_dest(uint8_t *dest);
extern flash_status_t get_nvram_sos_msg(uint8_t *dest);
extern flash_status_t set_nvram_sos_info(uint32_t rddest,uint8_t *ltedest,uint8_t *msg);
#endif
