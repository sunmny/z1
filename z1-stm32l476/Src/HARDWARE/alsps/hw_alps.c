#include "hw_led.h" 
#include "hw_config.h"
//////////////////////////////////////////////////////////////////////////////////	 
//STM32F4工程模板-库函数版本
//淘宝店铺：http://mcudev.taobao.com							  
////////////////////////////////////////////////////////////////////////////////// 	 

//初始化PA6和PA7为输出口.并使能这两个口的时钟		    
//LED IO初始化
void hardware_led_init(void)
{    	 
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOC, ENABLE);//使能GPIOA时钟
	
  //GPIOF9,F10初始化设置
  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉

	GPIO_InitStructure.GPIO_Pin = LED_KEY_R1_PIN | LED_KEY_R2_PIN | LED_CHARGE_R_PIN;//LED0和LED1对应IO口
  GPIO_Init(LED_KEY_R1_PORT, &GPIO_InitStructure);//初始化GPIO
	
	GPIO_InitStructure.GPIO_Pin = LED_CHARGE_G_PIN;
	GPIO_Init(LED_CHARGE_G_PORT, &GPIO_InitStructure);//初始化GPIO
	
	GPIO_SetBits(LED_KEY_R1_PORT,LED_KEY_R1_PIN | LED_KEY_R2_PIN | LED_CHARGE_R_PIN);//设置高，灯灭
	GPIO_SetBits(LED_CHARGE_G_PORT,LED_CHARGE_G_PIN);//设置高，灯灭
	

}

uint8_t hardware_led_get(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
     return GPIO_ReadOutputDataBit(GPIOx, GPIO_Pin);
}


void hardware_led_set(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin,uint8_t level)
{
	if(level == ON)
		GPIO_SetBits(GPIOx,GPIO_Pin);
	else
		GPIO_ResetBits(GPIOx,GPIO_Pin);	
}







