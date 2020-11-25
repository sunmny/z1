#ifndef APP_AOA_H
#define APP_AOA_H

#include <stdint.h>
#include "stm32l4xx_hal.h"

#define AOA_UART_DATA_LOOP_SIZE 512
#define AOA_MAIL_BUF_LEN 20



#if 0
typedef struct {	
		uint8_t dev_num;
		uint8_t bdev_id[2][100][12]; /*两个任务id 所对应的id*/
}zdevice_save_wlist;

typedef struct {	
		uint8_t zdev_num;
		uint8_t zdev_id[12];
		uint8_t mydev_id[12];
}zdevice_id;







typedef struct {	
		uint8_t task_flag;
		zdevice_save_wlist zdev_wlist;
	  zdevice_id  zdev_id;
		zdevice_status zdev_stat;
		zdevice_setting zdev_set;
		zdevice_location zdev_locat;
		uint8_t task_num ;
		uint8_t distance;
		zdevsave_dis_addr bdev_dis[100];
}zdevice;

typedef struct {	
				uint8_t brn_number[6];
				uint8_t brn_report_mes[50];
				uint8_t brn_report_cyc;
}bdevice_rn_setting;

typedef struct {
				uint8_t brd_stats;
}bdevice_rd_setting;

typedef struct {	
				uint8_t bip[15];
				uint8_t busername[50];
				uint8_t bpasswd[20];
}bdevice_lte_setting;

typedef struct {	
			uint8_t bat;
			uint8_t brn_status;
			uint8_t brd_status;
			uint8_t blte_status;
			uint8_t block_status;
	
}bdevice_status;

typedef struct {	
			 uint8_t breport_info[50];				
}bdevice_location;

typedef struct {
		bdevice_rn_setting brn;
		bdevice_rd_setting brd;
		bdevice_lte_setting blte;	
		
}bdevice_setting;

typedef struct {
		uint8_t btaskid;
		uint8_t bdev_mac[12];
		uint8_t bdis_report;
		bdevice_status bdevstats;
		bdevice_setting bdev_set;
		bdevice_location bdev_locat;
	
}bdevice;



typedef struct {
		uint8_t saddr[100];
		uint8_t raddr[100];
		uint8_t light_onoff;
	
}mdevice_setting;

typedef struct {
		uint8_t mtaskid;
		uint8_t mdev_mac[12];
		uint8_t mdis_report;
		mdevice_setting mdev_set;
	
}mdevice;

#endif



extern uint8_t aoa_uart_data_loop[AOA_UART_DATA_LOOP_SIZE+1];

typedef uint8_t (*aoarxhandle)(uint8_t*, uint16_t, uint8_t);


typedef struct {
	aoarxhandle handle;
	uint8_t *pData;
}aoa_at_handle_symbol;


typedef struct {
	uint16_t len;
	uint8_t data[AOA_MAIL_BUF_LEN];
}aoamail;
#define taskid      2
#define max_dev     100

struct zdevice_bbind_bdev{
				uint8_t bdev_taskid;
				uint8_t bdev_id[taskid][max_dev][16];
};
typedef struct {	
			uint8_t zrn_status;
			uint8_t zrd_status;	
			uint8_t zrn_test_status;
			uint8_t zrd_test_status;
}zdevice_status;
struct zdevice_setting{
					uint8_t isMdev;
					uint8_t bbat_report;
					uint8_t zbattery;
					uint8_t bat_chr_status;
					uint8_t bat1_chr_status;
					uint8_t zbat_soc;
					uint8_t zbat_soc_bak;
					uint8_t zbat_soc1_bak;
					uint8_t zbat_soc1;
					uint8_t bdevtask1_num;
					uint8_t bdevtask2_num;
					uint8_t zdev_is_set;
					uint8_t zdev_num;
					uint8_t report_mesage_id ;
					uint8_t lowbat[4];
				  uint8_t report_dis;
					uint8_t report_bat_threshold;
					//uint8_t zdev_report_app_time[4];
					uint8_t report_port[8];
					uint8_t zdev_report_time[4] ;
					uint8_t bdev_report_time[4];					
					uint8_t rn_num[8];
					uint8_t rd_num[8];
					uint8_t other_id[12];
					uint8_t mydev_id[12];
					//uint8_t report_ip[36];			
					//uint8_t report_apn[36];
					//uint8_t user_name[36];
					//uint8_t user_paswd[36];
				  zdevice_status status;
				//	uint8_t  white_list[100][16];
					struct zdevice_bbind_bdev bbinddev;
					
};



extern uint8_t bdev_id_temp_buf[13];
//extern void zdev_read_battery_soc(uint16_t *data,uint8_t *val);
extern struct zdevice_setting zdev_set;

extern void aoa_test_mail_fun(void);

extern uint8_t aoa_uart_receive_and_handle(void);
extern uint8_t aoa_mail_receive_and_handle(aoamail *pmail);

#endif
