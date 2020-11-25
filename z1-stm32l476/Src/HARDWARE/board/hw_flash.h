#ifndef HARDWARE_FLASH_H
#define HARDWARE_FLASH_H

typedef enum
{
	FLASH_OK=0,
	FLASH_ERR,
	FLASH_CHECK_ERR,
}flash_status_t;

extern flash_status_t Flash_If_Init(void);
extern flash_status_t Flash_If_DeInit(void);
extern flash_status_t Flash_If_Erase(uint32_t Add);
extern flash_status_t Flash_If_Write(uint8_t *src, uint32_t dest_addr, uint32_t Len);
extern flash_status_t Flash_If_Read(uint8_t* buff, uint32_t dest_addr, uint32_t Len);

#endif
