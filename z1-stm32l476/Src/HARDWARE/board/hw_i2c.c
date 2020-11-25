#include "hw_i2c.h" 
//#include "hw_config.h"
#include "stm32l4xx_hal.h"


extern void Error_Handler(void);

I2C_HandleTypeDef hi2c1;
I2C_HandleTypeDef hi2c2;



/* I2C1 init function */
//static void MX_I2C1_Init(void)
//{

//  hi2c1.Instance = I2C1;
//  hi2c1.Init.Timing =  0x0000030F;//0x00301347 ;//0x00300f38;//0x00301347;//0x00000004;//0x00000E14;
//  hi2c1.Init.OwnAddress1 = 0;
//  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
//  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
//  hi2c1.Init.OwnAddress2 = 0;
//  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
//  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
//  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
//  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
//  {
//   // Error_Handler();
//  }

//    /**Configure Analogue filter 
//    */
//  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
//  {
//  //  Error_Handler();
//  }

//    /**Configure Digital filter 
//    */
//  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
//  {
//  // Error_Handler();
//  }

//}

/* I2C2 init function */
static void MX_I2C2_Init(void)
{

  hi2c2.Instance = I2C2;
  hi2c2.Init.Timing = 0x0000030F;//0x00301347;// 0x00300f38;//0x00301347;// 0x00000004;//0x00000E14;
  hi2c2.Init.OwnAddress1 = 0;
  hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c2.Init.OwnAddress2 = 0;
  hi2c2.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c2) != HAL_OK)
  {
   // Error_Handler();
  }

    /**Configure Analogue filter 
    */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c2, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
   // Error_Handler();
  }

    /**Configure Digital filter 
    */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c2, 0) != HAL_OK)
  {
   // Error_Handler();
  }

}

/*
// I2C1 init function 
static void MX_I2C1_Init2(void)
{

  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x20000209;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
   // Error_Handler();
  }


  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
   // Error_Handler();
  }


  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
   // Error_Handler();
  }


  HAL_I2CEx_EnableFastModePlus(I2C_FASTMODEPLUS_I2C1);

}

static void MX_I2C2_Init2(void)
{

  hi2c2.Instance = I2C2;
  hi2c2.Init.Timing = 0x20000209;
  hi2c2.Init.OwnAddress1 = 0;
  hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c2.Init.OwnAddress2 = 0;
  hi2c2.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c2) != HAL_OK)
  {
   // Error_Handler();
  }


  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c2, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
   // Error_Handler();
  }


  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c2, 0) != HAL_OK)
  {
   // Error_Handler();
  }


  HAL_I2CEx_EnableFastModePlus(I2C_FASTMODEPLUS_I2C2);

}
*/

void hw_i2c_init(void)
{    	 
 //  MX_I2C1_Init();
	 MX_I2C2_Init();
}

void hw_i2c_deinit(void)
{    	 
  // HAL_I2C_DeInit(&hi2c1);
	 HAL_I2C_DeInit(&hi2c2);
}


uint8_t hw_i2c_write(uint8_t i2c_nub, uint16_t i2c_addr, uint8_t *tx_buf, uint16_t tx_len)
{
		I2C_HandleTypeDef I2cHandle;
	
	switch(i2c_nub)
	{
		case I2C_1:
			  I2cHandle = hi2c1;
		break;
	  case I2C_2:
			  I2cHandle = hi2c2;
		break;
		default:
			return 1;
	}
   /*##-2- Start the transmission process #####################################*/  
  /* While the I2C in reception process, user can transmit data through 
     "aTxBuffer" buffer */
  /* Timeout is set to 10S */
  //while(HAL_I2C_Master_Transmit(&I2cHandle, (uint16_t)i2c_addr, (uint8_t*)tx_buf, tx_len, 10000)!= HAL_OK)
		if(HAL_I2C_Master_Transmit(&I2cHandle, (uint16_t)i2c_addr, (uint8_t*)tx_buf, tx_len, 1000)!= HAL_OK)
		{
    /* Error_Handler() function is called when Timeout error occurs.
       When Acknowledge failure occurs (Slave don't acknowledge its address)
       Master restarts communication */
    if (HAL_I2C_GetError(&I2cHandle) != HAL_I2C_ERROR_AF)
    {
     // Error_Handler();
    }
			return 1;
	}
  

	return 0;
	
}


uint8_t hw_i2c_read(uint8_t i2c_nub, uint16_t i2c_addr, uint8_t *rx_buf,uint16_t rx_len)
{
	
	I2C_HandleTypeDef I2cHandle;
	
	switch(i2c_nub)
	{
		case I2C_1:
			  I2cHandle = hi2c1;
		break;
	  case I2C_2:
			  I2cHandle = hi2c2;
		break;
		default:
			return 1;
	}
	
	// while(HAL_I2C_Master_Receive(&I2cHandle, (uint16_t)i2c_addr, (uint8_t *)rx_buf, rx_len, 10000) != HAL_OK)
	if(HAL_I2C_Master_Receive(&I2cHandle, (uint16_t)i2c_addr, (uint8_t *)rx_buf, rx_len, 1000) != HAL_OK)
  {
    /* Error_Handler() function is called when Timeout error occurs.
       When Acknowledge failure occurs (Slave don't acknowledge it's address)
       Master restarts communication */
    if (HAL_I2C_GetError(&I2cHandle) != HAL_I2C_ERROR_AF)
    {
     // Error_Handler();
    }
			return 1;
  }

		return 0;

}

uint8_t hw_i2c_write_read(uint8_t i2c_nub, uint16_t i2c_addr, uint8_t *tx_buf, uint16_t tx_len,uint8_t *rx_buf, uint16_t rx_len)
{
	
	I2C_HandleTypeDef I2cHandle;
	
	switch(i2c_nub)
	{
		case I2C_1:
			  I2cHandle = hi2c1;
		break;
	  case I2C_2:
			  I2cHandle = hi2c2;
		break;
		default:
			return 1;
	}
	
	   /*##-2- Start the transmission process #####################################*/  
  /* While the I2C in reception process, user can transmit data through 
     "aTxBuffer" buffer */
  /* Timeout is set to 10S */
 // while(HAL_I2C_Master_Transmit(&I2cHandle, (uint16_t)i2c_addr, (uint8_t*)tx_buf, tx_len, 10000)!= HAL_OK)
	 if(HAL_I2C_Master_Transmit(&I2cHandle, (uint16_t)i2c_addr, (uint8_t*)tx_buf, tx_len, 1000)!= HAL_OK)
		{
    /* Error_Handler() function is called when Timeout error occurs.
       When Acknowledge failure occurs (Slave don't acknowledge its address)
       Master restarts communication */
			
			printf("i2c write error \r\n");
			return 1;
			
    if (HAL_I2C_GetError(&I2cHandle) != HAL_I2C_ERROR_AF)
    {
     // Error_Handler();
    }
	 }
	
	
	
	// while(HAL_I2C_Master_Receive(&I2cHandle, (uint16_t)i2c_addr, (uint8_t *)rx_buf, rx_len, 10000) != HAL_OK)
	if(HAL_I2C_Master_Receive(&I2cHandle, (uint16_t)i2c_addr, (uint8_t *)rx_buf, rx_len, 1000) != HAL_OK)
  {
    /* Error_Handler() function is called when Timeout error occurs.
       When Acknowledge failure occurs (Slave don't acknowledge it's address)
       Master restarts communication */
		printf("i2c read error \r\n");
			return 1;
    if (HAL_I2C_GetError(&I2cHandle) != HAL_I2C_ERROR_AF)
    {
     // Error_Handler();
    }
  }

		return 0;

}
