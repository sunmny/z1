#include "hw_gsensor.h" 
#include "hw_config.h"
#include "stm32l4xx_hal.h"
#include "hw_i2c.h"
#include "delay.h"

uint8_t running_falg = 0;
uint8_t running_count = 0;
uint8_t running_count_last = 0;


#define SENSOR_I2C                I2C_1
#define gsensor_delay_ms          100


void hw_gsensor_readdata(uint8_t addr,uint8_t *rec_data,uint16_t rec_len)
{
	
//	uint8_t send[2] = {0x00};
	
//	 send[0] = 0x40;
	// send[1] = data;
	
  hw_i2c_write_read(SENSOR_I2C, KXTJ2_1009_I2C_SLAVE_ADDR,&addr,1,rec_data,rec_len);

	
}

void hw_gsensor_writedata(uint8_t addr,uint8_t send_data)
{
	
	uint8_t send[2] = {0x00,0x00};
	
	 send[0] = addr;
	 send[1] = send_data;
	
	hw_i2c_write(SENSOR_I2C, KXTJ2_1009_I2C_SLAVE_ADDR, send, 2);
	
}


void hw_gsensor_checkid(void)
{

	uint8_t id=0x00;    
	int res = 0;
	
	
	hw_gsensor_readdata(KXTJ2_1009_REG_DEVID,&id,1);
	
   //HAL_GPIO_TogglePin(LED_KEY_R1_GPIO_Port, LED_KEY_R1_Pin);
	if(id>0)
	{
		/*
    HAL_GPIO_WritePin(LED_KEY_R2_GPIO_Port,LED_KEY_R2_Pin, GPIO_PIN_SET);
		delay_ms(100);
		HAL_GPIO_WritePin(LED_KEY_R2_GPIO_Port,LED_KEY_R2_Pin, GPIO_PIN_RESET);
		delay_ms(100);
		HAL_GPIO_WritePin(LED_KEY_R2_GPIO_Port,LED_KEY_R2_Pin, GPIO_PIN_SET);
		delay_ms(100);
		HAL_GPIO_WritePin(LED_KEY_R2_GPIO_Port,LED_KEY_R2_Pin, GPIO_PIN_RESET);
		*/
		
	}
   
}

void hw_gsensor_power(uint8_t state)
{
    
//#define SENSOR_VCC_EN_Pin GPIO_PIN_5
//#define SENSOR_VCC_EN_GPIO_Port GPIOC
	
	  HAL_GPIO_WritePin(SENSOR_VCC_EN_GPIO_Port,SENSOR_VCC_EN_Pin, (GPIO_PinState)state);
	/*
	  HAL_GPIO_WritePin(LED_KEY_R2_GPIO_Port,LED_KEY_R2_Pin, GPIO_PIN_SET);
		delay_ms(100);
		HAL_GPIO_WritePin(LED_KEY_R2_GPIO_Port,LED_KEY_R2_Pin, GPIO_PIN_RESET);
		delay_ms(100);
		HAL_GPIO_WritePin(LED_KEY_R2_GPIO_Port,LED_KEY_R2_Pin, GPIO_PIN_SET);
		delay_ms(100);
		HAL_GPIO_WritePin(LED_KEY_R2_GPIO_Port,LED_KEY_R2_Pin, GPIO_PIN_RESET);
	*/
	
}


/*

0x1b 0x00
0x1b 0x42
0x1d 0x06
0x1f 0x7f
0x29 0x05
0x6a 0x08
0x1e 0x30
0x1b 0xc2

*/
void hw_gsensor_open(void)
{
   hw_gsensor_power(GPIO_PIN_SET);
}

void hw_gsensor_close(void)
{
   hw_gsensor_power(GPIO_PIN_RESET);
}




void hw_gsensor_init(void)
{   
  uint8_t readvalue = 0;
  hw_gsensor_open();
	
	delay_ms(gsensor_delay_ms);
  hw_gsensor_checkid();
	
	/*
	hw_gsensor_writedata(0x1b,0x00);
	hw_gsensor_writedata(0x1b,0x60);
	hw_gsensor_writedata(0x1e,0x38);
	hw_gsensor_writedata(0x21,0x02);
	hw_gsensor_writedata(0x1b,0xe0);

	*/
	/*
	  0x1b 0x00
	  0x21 0x02
	  0x1d 0x06
	  0x17 0x3f
	  0x29 0x02
	  0x6a 0x02
		0x1e 0x30
		0x1b 0xc2
	
	
	*/
	delay_ms(gsensor_delay_ms);	
	// hw_gsensor_writedata(uint8_t addr,uint8_t send_data);
	// hw_gsensor_writedata(uint8_t addr,uint8_t send_data);
	hw_gsensor_writedata(0x1d,0x80);
		 delay_ms(gsensor_delay_ms);	
	// hw_gsensor_writedata(uint8_t addr,uint8_t send_data);
	// hw_gsensor_writedata(uint8_t addr,uint8_t send_data);
	hw_gsensor_writedata(0x1b,0x00);
	 delay_ms(gsensor_delay_ms);
	hw_gsensor_writedata(0x21,0x02);
	 delay_ms(gsensor_delay_ms);
	hw_gsensor_writedata(0x1d,0x06);
	 delay_ms(gsensor_delay_ms);
	hw_gsensor_writedata(0x17,0x3f);
	 delay_ms(gsensor_delay_ms);
	hw_gsensor_writedata(0x29,0x02);
	 delay_ms(gsensor_delay_ms);
	hw_gsensor_writedata(0x6a,0x02);
	 delay_ms(gsensor_delay_ms);
	hw_gsensor_writedata(0x1e,0x30);
	//delay_ms(5);
	//hw_gsensor_readdata(0x1e,&readvalue,1);
	//readvalue = 0;
	 delay_ms(gsensor_delay_ms);
	hw_gsensor_writedata(0x1b,0xc2);
	 delay_ms(gsensor_delay_ms);
//hw_gsensor_writedata(0x1A,0x00);
delay_ms(gsensor_delay_ms);
hw_gsensor_readdata(0x1a,&readvalue,1);//拉低中断
	
	/*
	 delay_ms(gsensor_delay_ms);	
	// hw_gsensor_writedata(uint8_t addr,uint8_t send_data);
	// hw_gsensor_writedata(uint8_t addr,uint8_t send_data);
	hw_gsensor_writedata(0x1b,0x00);
	 delay_ms(gsensor_delay_ms);
	hw_gsensor_writedata(0x1b,0x42);
	 delay_ms(gsensor_delay_ms);
	hw_gsensor_writedata(0x1d,0x06);
	 delay_ms(gsensor_delay_ms);
	hw_gsensor_writedata(0x1f,0x7f);
	 delay_ms(gsensor_delay_ms);
	hw_gsensor_writedata(0x29,0x05);
	 delay_ms(gsensor_delay_ms);
	hw_gsensor_writedata(0x6a,0x08);
	 delay_ms(gsensor_delay_ms);
	hw_gsensor_writedata(0x1e,0x38);
	//delay_ms(5);
	//hw_gsensor_readdata(0x1e,&readvalue,1);
	//readvalue = 0;
	 delay_ms(gsensor_delay_ms);
	hw_gsensor_writedata(0x1b,0xc2);
	 delay_ms(gsensor_delay_ms);
	hw_gsensor_writedata(0x1A,0x00);
*/
	
	
}

/*

  0x00  standby
  0x40
  0x42  standby 精度不同  带计步器
  0x60   standby 精度不同  数据更新唤醒
  0xc2  用以前的模式
  0xe0  正常模式

*/


void hw_gsensor_set_power_mode()
{
	
	/*
   uint8_t send[2]
	
	//
	
	KXTJ2_1009_REG_POWER_CTL
	KXTJ2_1009_MEASURE_MODE
	hw_gsensor_writedata(KXTJ2_1009_REG_POWER_CTL,uint8_t send_data)
*/
}



void hw_gsensor_irq(void)
{  
	
	uint8_t aa= 0;
  hw_gsensor_readdata(0x1a,&aa,1);//拉低中断
	
	//aa=0;
	/*
	hw_gsensor_writedata(0x1A,0x00);
	hw_gsensor_readdata(0x1a,&aa,1);
	aa=0;
	*/
	
	//比较相邻两次中断的时间，如果在5s内，则认为是运动，5s后没有中断产生，则认为停止运动
	if(((running_count-running_count_last)<=5)||((running_count_last-running_count)<=5))
	{
	   running_count_last = 0;
		 running_count = 0;
		 running_falg = 1;
	}
	
}

void hw_gsensor_running_detect(void)
{
				//运动
		running_count++;
		if(running_count>5)
		{
		  //静止
			running_falg = 0;
		}else if(running_count >100)
		{
		  running_count = 0;
		}
}


void hw_gsensor_suspend(void)
{
   hw_gsensor_writedata(0x1e,0x10);
}

void hw_gsensor_resume(void)
{
    hw_gsensor_writedata(0x1e,0x30);
}

