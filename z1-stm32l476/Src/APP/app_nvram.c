#include "stm32l4xx_hal.h"
#include "app_nvram.h"
#include "string.h"
#include "hw_flash.h"

//static uint8_t userNvram[USER_NVRAM_SIZE];

flash_status_t get_nvram_sn(uint8_t *sn)
{
	flash_status_t status;
	status=Flash_If_Read(sn, NV_SN_DEFAULT_MSG_ADDR, NV_SN_DEFAULT_MSG_SIZE_BYTE);
	return status;
}
flash_status_t get_nvram_id(uint8_t *sn)
{
	flash_status_t status;
	status=Flash_If_Read(sn, USER_NVRAM_DEVID_START_ADDR, NV_DEVID_DEFAULT_MSG_SIZE_BYTE);
	return status;
}
flash_status_t get_nvram_save_data(uint8_t *sn)
{
	flash_status_t status;
	status=Flash_If_Read(sn, NV_LOW_BATTERY_ADDR, NV_LOW_BATTERY_SIZE_BYTE);
	return status;
}
flash_status_t set_nvram_save_data(uint8_t *sn)
{
	
	
	flash_status_t status;
	uint8_t userNvram[USER_NVRAM_SIZE];
	uint32_t offset;
	status=Flash_If_Read(userNvram, USER_NVRAM_START_ADDR, USER_NVRAM_SIZE);
	status=Flash_If_Init();
	if(status!=FLASH_OK)
		return status;
	status=Flash_If_Erase(NV_LOW_BATTERY_ADDR);
	if(status!=FLASH_OK)
	{
		Flash_If_DeInit();
		return status;
	}
	else
	{
		offset=0;//NV_SN_DEFAULT_MSG_ADDR-USER_NVRAM_START_ADDR;
		memcpy(&userNvram[offset],sn,NV_LOW_BATTERY_SIZE_BYTE);
		status=Flash_If_Write(userNvram, NV_LOW_BATTERY_ADDR, USER_NVRAM_SIZE);
		Flash_If_DeInit();
		return status;
	}
}

flash_status_t set_nvram_sn_data(uint8_t *sn)
{
	flash_status_t status;
	uint8_t userNvram[USER_NVRAM_SIZE];
	uint32_t offset;
	status=Flash_If_Read(userNvram, USER_NVRAM_START_ADDR, USER_NVRAM_SIZE);
	status=Flash_If_Init();
	if(status!=FLASH_OK)
		return status;
	status=Flash_If_Erase(NV_SN_DEFAULT_MSG_ADDR);
	if(status!=FLASH_OK)
	{
		Flash_If_DeInit();
		return status;
	}
	else
	{
		offset=NV_SN_DEFAULT_MSG_ADDR-USER_NVRAM_START_ADDR;
		memcpy(&userNvram[offset],sn,NV_SN_DEFAULT_MSG_SIZE_BYTE);
		status=Flash_If_Write(userNvram, USER_NVRAM_START_ADDR, USER_NVRAM_SIZE);
		Flash_If_DeInit();
		return status;
	}
}


flash_status_t set_nvram_id_data(uint8_t *sn)
{
	flash_status_t status;
	uint8_t userNvram[USER_NVRAM_SIZE];
	uint32_t offset;
	status=Flash_If_Read(userNvram, USER_NVRAM_DEVID_START_ADDR, USER_NVRAM_DEVID_SIZE);
	status=Flash_If_Init();
	if(status!=FLASH_OK)
		return status;
	status=Flash_If_Erase(NV_DEVID_DEFAULT_MSG_ADDR);
	if(status!=FLASH_OK)
	{
		Flash_If_DeInit();
		return status;
	}
	else
	{
		offset=NV_DEVID_DEFAULT_MSG_ADDR-USER_NVRAM_DEVID_START_ADDR;
		memcpy(&userNvram[offset],sn,NV_DEVID_DEFAULT_MSG_SIZE_BYTE);
		status=Flash_If_Write(userNvram, USER_NVRAM_DEVID_START_ADDR, USER_NVRAM_DEVID_SIZE);
		Flash_If_DeInit();
		return status;
	}
}















void local_nvram_init(void)
{
}

