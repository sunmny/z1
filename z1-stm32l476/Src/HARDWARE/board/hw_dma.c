#include "hw_dma.h" 
#include "stm32l4xx_hal.h"

/*uart3 -->bd2, uart2-->bd1, uart1 -->AOA(Z1) or LTE(B1), uart4 -->BLE2640*/
DMA_HandleTypeDef hdma_usart3_rx;
DMA_HandleTypeDef hdma_usart3_tx;
DMA_HandleTypeDef hdma_usart2_rx;
DMA_HandleTypeDef hdma_usart2_tx;
DMA_HandleTypeDef hdma_usart1_rx;
DMA_HandleTypeDef hdma_usart1_tx;
DMA_HandleTypeDef hdma_usart4_rx;
DMA_HandleTypeDef hdma_usart4_tx;


static void MX_DMA_Init(void);

/** 
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void) 
{
  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();
  __HAL_RCC_DMA2_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel4_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel4_IRQn, 0, 1);//uart1 tx
  HAL_NVIC_EnableIRQ(DMA1_Channel4_IRQn);
  
  /* DMA1_Channel5_IRQn interrupt configuration *///uart1 rx
  HAL_NVIC_SetPriority(DMA1_Channel5_IRQn, 0, 1);
  HAL_NVIC_EnableIRQ(DMA1_Channel5_IRQn);
  
  /* DMA1_Channel6_IRQn interrupt configuration *///uart2 tx
  HAL_NVIC_SetPriority(DMA1_Channel6_IRQn, 0, 1);
  HAL_NVIC_EnableIRQ(DMA1_Channel6_IRQn);
  
  /* DMA1_Channel7_IRQn interrupt configuration */ //uart2 rx
  HAL_NVIC_SetPriority(DMA1_Channel7_IRQn, 0, 1);
  HAL_NVIC_EnableIRQ(DMA1_Channel7_IRQn);

    /* DMA1_Channel2_IRQn interrupt configuration *///uart3 tx
  HAL_NVIC_SetPriority(DMA1_Channel2_IRQn, 0, 1);
  HAL_NVIC_EnableIRQ(DMA1_Channel2_IRQn);
  
  /* DMA1_Channel3_IRQn interrupt configuration */ //uart3 rx
  HAL_NVIC_SetPriority(DMA1_Channel3_IRQn, 0, 1);
  HAL_NVIC_EnableIRQ(DMA1_Channel3_IRQn);

      /* DMA2_Channe3_IRQn interrupt configuration *///uart4 tx
  HAL_NVIC_SetPriority(DMA2_Channel3_IRQn, 0, 1);
  HAL_NVIC_EnableIRQ(DMA2_Channel3_IRQn);
  
  /* DMA2_Channel4_IRQn interrupt configuration */ //uart4 rx
  HAL_NVIC_SetPriority(DMA2_Channel5_IRQn, 0, 1);
  HAL_NVIC_EnableIRQ(DMA2_Channel5_IRQn);


}




void hw_dma_init(void)
{ 	
   MX_DMA_Init();
}

void hw_dma_deinit(void)
{ 	



  /* DMA interrupt init */
  /* DMA1_Channel4_IRQn interrupt configuration *///uart1 tx
  HAL_NVIC_SetPriority(DMA1_Channel4_IRQn, 0, 1);
  HAL_NVIC_DisableIRQ(DMA1_Channel4_IRQn);
  
  /* DMA1_Channel5_IRQn interrupt configuration *///uart1 rx
  HAL_NVIC_SetPriority(DMA1_Channel5_IRQn, 0, 1);
  HAL_NVIC_DisableIRQ(DMA1_Channel5_IRQn);
  
  /* DMA1_Channel6_IRQn interrupt configuration *///uart2 tx
  HAL_NVIC_SetPriority(DMA1_Channel6_IRQn, 0, 1);
  HAL_NVIC_EnableIRQ(DMA1_Channel6_IRQn);
  
  /* DMA1_Channel7_IRQn interrupt configuration *///uart2 rx
  HAL_NVIC_SetPriority(DMA1_Channel7_IRQn, 0, 1); 
  HAL_NVIC_DisableIRQ(DMA1_Channel7_IRQn);

    /* DMA1_Channel2_IRQn interrupt configuration *///uart3 tx
  HAL_NVIC_SetPriority(DMA1_Channel2_IRQn, 0, 1);
  HAL_NVIC_EnableIRQ(DMA1_Channel2_IRQn);
  
  /* DMA1_Channel3_IRQn interrupt configuration *///uart3 rx
  HAL_NVIC_SetPriority(DMA1_Channel3_IRQn, 0, 1); 
  HAL_NVIC_DisableIRQ(DMA1_Channel3_IRQn);

        /* DMA2_Channe3_IRQn interrupt configuration *///uart4 tx
  HAL_NVIC_SetPriority(DMA2_Channel3_IRQn, 0, 1);
  HAL_NVIC_DisableIRQ(DMA2_Channel3_IRQn);
  
  /* DMA2_Channel4_IRQn interrupt configuration */ //uart4 rx
  HAL_NVIC_SetPriority(DMA2_Channel5_IRQn, 0, 1);
  HAL_NVIC_DisableIRQ(DMA2_Channel5_IRQn);


	
		  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_DISABLE();
  __HAL_RCC_DMA2_CLK_DISABLE();

}

