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
	status=Flash_If_Read(sn, NV_ID_DEFAULT_MSG_ADDR, NV_ID_DEFAULT_MSG_SIZE_BYTE);
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
	status=Flash_If_Read(userNvram, USER_NVRAM_START_ADDR, USER_NVRAM_SIZE);
	status=Flash_If_Init();
	if(status!=FLASH_OK)
		return status;
	status=Flash_If_Erase(NV_ID_DEFAULT_MSG_ADDR);
	if(status!=FLASH_OK)
	{
		Flash_If_DeInit();
		return status;
	}
	else
	{
		offset=NV_ID_DEFAULT_MSG_ADDR-USER_NVRAM_START_ADDR;
		memcpy(&userNvram[offset],sn,NV_ID_DEFAULT_MSG_SIZE_BYTE);
		status=Flash_If_Write(userNvram, USER_NVRAM_START_ADDR, USER_NVRAM_SIZE);
		Flash_If_DeInit();
		return status;
	}
}

flash_status_t get_nvram_airmod_flag(uint8_t *flag)
{
	flash_status_t status;
	status=Flash_If_Read(flag, NV_AIRMODE_FLAG_ADDR, NV_AIRMODE_FLAG_SIZE_BYTE);
	return status;
}

flash_status_t set_nvram_airmod_flag(uint8_t flag)
{
	flash_status_t status;
	uint8_t userNvram[USER_NVRAM_SIZE];
	status=Flash_If_Read(userNvram, USER_NVRAM_START_ADDR, USER_NVRAM_SIZE);
	status=Flash_If_Init();
	if(status!=FLASH_OK)
		return status;
	status=Flash_If_Erase(NV_AIRMODE_FLAG_ADDR);
	if(status!=FLASH_OK)
	{
		Flash_If_DeInit();
		return status;
	}
	else
	{
		userNvram[0]=flag;
		status=Flash_If_Write(userNvram, USER_NVRAM_START_ADDR, USER_NVRAM_SIZE);
		Flash_If_DeInit();
		return status;
	}

}



flash_status_t get_nvram_low_battery(uint8_t *battery)
{
	flash_status_t status;
	status=Flash_If_Read(battery, NV_LOW_BATTERY_ADDR, NV_LOW_BATTERY_SIZE_BYTE);
	return status;
}

flash_status_t set_nvram_low_battery(uint8_t battery)
{
	flash_status_t status;
	uint8_t userNvram[USER_NVRAM_SIZE];
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
		userNvram[0]=battery;
		status=Flash_If_Write(userNvram, USER_NVRAM_START_ADDR, USER_NVRAM_SIZE);
		Flash_If_DeInit();
		return status;
	}

}

flash_status_t get_nvram_report_dis(uint8_t *period)
{
	flash_status_t status;
	uint8_t buf;
	status=Flash_If_Read(&buf, NV_REPORT_PERIOD_ADDR, NV_REPORT_PERIOD_SIZE_BYTE);
	*period = buf;
	return status;
}

flash_status_t set_nvram_report_dis(uint8_t period)
{
	flash_status_t status;
	uint8_t userNvram[USER_NVRAM_SIZE];
	status=Flash_If_Read(userNvram, USER_NVRAM_START_ADDR, USER_NVRAM_SIZE);
	status=Flash_If_Init();
	if(status!=FLASH_OK)
		return status;
	status=Flash_If_Erase(NV_REPORT_PERIOD_ADDR);
	if(status!=FLASH_OK)
	{
		Flash_If_DeInit();
		return status;
	}
	else
	{
		userNvram[0]=period;
		status=Flash_If_Write(userNvram, USER_NVRAM_START_ADDR, USER_NVRAM_SIZE);
		Flash_If_DeInit();
		return status;
	}
}

flash_status_t get_nvram_center_address(uint8_t *address)
{
	flash_status_t status;
	status=Flash_If_Read(address, NV_CENTER_ADDRESS_ADDR, NV_CENTER_ADDRESS_SIZE_BYTE);
	return status;
}

flash_status_t set_nvram_center_address(uint8_t *address)
{
	flash_status_t status;
	uint8_t userNvram[USER_NVRAM_SIZE];
	uint32_t offset;
	status=Flash_If_Read(userNvram, USER_NVRAM_START_ADDR, USER_NVRAM_SIZE);
	status=Flash_If_Init();
	if(status!=FLASH_OK)
		return status;
	status=Flash_If_Erase(NV_CENTER_ADDRESS_ADDR);
	if(status!=FLASH_OK)
	{
		Flash_If_DeInit();
		return status;
	}
	else
	{

		offset=NV_CENTER_ADDRESS_ADDR-USER_NVRAM_START_ADDR;
		memcpy(&userNvram[offset],address,NV_CENTER_ADDRESS_SIZE_BYTE);
		
		status=Flash_If_Write(userNvram, USER_NVRAM_START_ADDR, USER_NVRAM_SIZE);
		Flash_If_DeInit();
		return status;
	}
}

flash_status_t get_nvram_rd_dest(uint32_t *dest)
{
	flash_status_t status;
	uint8_t buf[NV_RD_DEST_SIZE_BYTE]={0};
	status=Flash_If_Read(buf, NV_RD_DEST_ADDR, NV_RD_DEST_SIZE_BYTE);
	*dest=(uint32_t)buf[0]|((uint32_t)buf[1]<<8)|((uint32_t)buf[2]<<16)|((uint32_t)buf[3]<<24);
	return status;
}

flash_status_t get_nvram_lte_dest(uint8_t *dest)
{
	flash_status_t status;
	status=Flash_If_Read(dest, NV_LTE_DEST_ADDR, NV_LTE_DEST_SIZE_BYTE);
	return status;
}

flash_status_t get_nvram_sos_msg(uint8_t *dest)
{
	flash_status_t status;
	status=Flash_If_Read(dest, NV_SOS_DEFAULT_MSG_ADDR, NV_SOS_DEFAULT_MSG_SIZE_BYTE);
	return status;
}


flash_status_t set_nvram_sos_info(uint32_t rddest,uint8_t *ltedest,uint8_t *msg)
{
	flash_status_t status;
	uint8_t userNvram[USER_NVRAM_SIZE];
	uint32_t offset;
	status=Flash_If_Read(userNvram, USER_NVRAM_START_ADDR, USER_NVRAM_SIZE);
	status=Flash_If_Init();
	if(status!=FLASH_OK)
		return status;
	status=Flash_If_Erase(NV_RD_DEST_ADDR);
	if(status!=FLASH_OK)
	{
		Flash_If_DeInit();
		return status;
	}
	else
	{
		offset=NV_RD_DEST_ADDR-USER_NVRAM_START_ADDR;
		userNvram[offset]=(uint8_t)(rddest&0xff);
		userNvram[offset+1]=(uint8_t)((rddest>>8)&0xff);
		userNvram[offset+2]=(uint8_t)((rddest>>16)&0xff);
		userNvram[offset+3]=(uint8_t)((rddest>>24)&0xff);

		offset=NV_LTE_DEST_ADDR-USER_NVRAM_START_ADDR;
		memcpy(&userNvram[offset],ltedest,NV_LTE_DEST_SIZE_BYTE);

		offset=NV_SOS_DEFAULT_MSG_ADDR-USER_NVRAM_START_ADDR;
		memcpy(&userNvram[offset],msg,NV_SOS_DEFAULT_MSG_SIZE_BYTE);
		
		status=Flash_If_Write(userNvram, USER_NVRAM_START_ADDR, USER_NVRAM_SIZE);
		Flash_If_DeInit();
		return status;
	}
}

void local_nvram_init(void)
{
}

