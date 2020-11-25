#include "hw_heartrate.h" 
#include "hw_heartrate_reg.h" 
#include "hw_config.h"
#include "stm32l4xx_hal.h"
#include "hw_i2c.h"
#include "delay.h"
#include "gui_message.h"
#include "app_lcd.h"


#include "hw_lora.h"

#include "hw_led.h"

/*
#define HEARTRATE_SENSOR_RESET_Pin GPIO_PIN_1
#define HEARTRATE_SENSOR_RESET_GPIO_Port GPIOB

#define HEARTRATE_SENSOR_INT_Pin GPIO_PIN_1
#define HEARTRATE_SENSOR_INT_GPIO_Port GPIOC
*/

uint8_t heartrate_state;

#define HEARTRATE_DELAY_MS  200

#define SENSOR_I2C                I2C_1



void hw_heartrate_readdata(uint8_t addr,uint8_t *rec_data,uint16_t rec_len)
{
  hw_i2c_write_read(SENSOR_I2C, HW_HEARTRATE_I2C_ADDR,&addr,1,rec_data,rec_len);
}

void hw_heartrate_writedata(uint8_t addr,uint8_t send_data)
{
	uint8_t send[2] = {0x00,0x00};
	
	 send[0] = addr;
	 send[1] = send_data;
	 hw_i2c_write(SENSOR_I2C, HW_HEARTRATE_I2C_ADDR, send, 2);
}


void hw_heartrate_checkid(void)
{

	uint8_t id=0x00;    
//	int res = 0;
	hw_heartrate_writedata(0x7f,0x00);
	
	hw_heartrate_readdata(0x00,&id,1);
	
   //HAL_GPIO_TogglePin(LED_KEY_R1_GPIO_Port, LED_KEY_R1_Pin);
	if(id>0)
	{
		
   // HAL_GPIO_WritePin(LED_KEY_R2_GPIO_Port,LED_KEY_R2_Pin, GPIO_PIN_SET);
		//delay_ms(100);
		//HAL_GPIO_WritePin(LED_KEY_R2_GPIO_Port,LED_KEY_R2_Pin, GPIO_PIN_RESET);
		//delay_ms(100);
		//HAL_GPIO_WritePin(LED_KEY_R2_GPIO_Port,LED_KEY_R2_Pin, GPIO_PIN_SET);
		//delay_ms(100);
		//HAL_GPIO_WritePin(LED_KEY_R2_GPIO_Port,LED_KEY_R2_Pin, GPIO_PIN_RESET);
		
	}
   
}

void hw_heartrate_power(GPIO_PinState state)
{
    
//#define SENSOR_VCC_EN_Pin GPIO_PIN_5
//#define SENSOR_VCC_EN_GPIO_Port GPIOC
	
	  HAL_GPIO_WritePin(SENSOR_VCC_EN_GPIO_Port,SENSOR_VCC_EN_Pin, state);
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

void hw_heartrate_reset(GPIO_PinState state)
{
    
//#define SENSOR_VCC_EN_Pin GPIO_PIN_5
//#define SENSOR_VCC_EN_GPIO_Port GPIOC
	
	  HAL_GPIO_WritePin(HEARTRATE_SENSOR_RESET_GPIO_Port,HEARTRATE_SENSOR_RESET_Pin, state);
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



void hw_heartrate_touch_mode_init(void)
{
  uint16_t i = 0;
/*
		for(i = 0; i < INIT_TOUCH_INT_ARRAY_SIZE;i++)
	{
	    hw_heartrate_writedata(  init_touch_register_INT_array[i][0],init_touch_register_INT_array[i][1]);
	  
  	}
	*/
	
			for(i = 0; i < INIT_TOUCH_REG_ARRAY_SIZE;i++)
	{
	    hw_heartrate_writedata(  init_touch_register_array[i][0],init_touch_register_array[i][1]);
	  
  	}

}

void hw_heartrate_test_init(void)
{
  uint16_t i = 0;
/*
		for(i = 0; i < INIT_TOUCH_INT_ARRAY_SIZE;i++)
	{
	    hw_heartrate_writedata(  init_touch_register_INT_array[i][0],init_touch_register_INT_array[i][1]);
	  
  	}
	*/
	
			for(i = 0; i < INIT_PPG_REG_ARRAY_SIZE;i++)
	  {
	    hw_heartrate_writedata(  init_ppg_register_array[i][0],init_ppg_register_array[i][1]);
	  
  	}

}

void hw_heartrate_open(void)
{
  hw_heartrate_power(GPIO_PIN_SET);
	hw_heartrate_reset(GPIO_PIN_SET);
}

void hw_heartrate_close(void)
{
  hw_heartrate_power(GPIO_PIN_RESET);
	hw_heartrate_reset(GPIO_PIN_RESET);
	delay_ms(200);
	hw_heartrate_power(GPIO_PIN_SET);
	hw_heartrate_reset(GPIO_PIN_SET);
	
}



void hw_heartrate_init(void)
{   
   hw_heartrate_open();
	
	
	
	delay_ms(HEARTRATE_DELAY_MS);
	
  //hw_heartrate_checkid();
	
	/*
1. Wake up Sensor (Read ID bank0 Addr0x00=0x02, Write 0x00 to Bank2 Addr0x70)
2. Run SW Reset (Write 0x00 to Bank0 Addr0xE1)
3. Load Touch setting for Touch mode. Refer to 11.1 and 11.2 appendix section.
4. Enable MCU interrupt.
5. Write 0x01 to Bank1 Addr0xD5 enable sensor.
6. Wait for touch interrupt.
7. When getting interrupt, execute (1.) action, then report touch status to MCU
	*/
	//1
	hw_heartrate_checkid();
	delay_ms(HEARTRATE_DELAY_MS);
	hw_heartrate_writedata(0x7f,0x02);
	delay_ms(HEARTRATE_DELAY_MS);
	hw_heartrate_writedata(0x70,0x00);
	delay_ms(HEARTRATE_DELAY_MS);
	
	//2
	hw_heartrate_writedata(0x7f,0x00);
	delay_ms(HEARTRATE_DELAY_MS);
	hw_heartrate_writedata(0xE1,0x00);
	delay_ms(HEARTRATE_DELAY_MS);
	
	//3
	hw_heartrate_touch_mode_init();
	delay_ms(HEARTRATE_DELAY_MS);
	
	//4
	
	
	//5
	hw_heartrate_writedata(0x7f,0x01);
	delay_ms(HEARTRATE_DELAY_MS);
	hw_heartrate_writedata(0xD5,0x01);
	delay_ms(HEARTRATE_DELAY_MS);
	
	//6
	
	//7
	
}


void hw_heartrate_set_power_mode(uint8_t en)
{
	
	/*
   uint8_t send[2]
	
	//
	
	KXTJ2_1009_REG_POWER_CTL
	KXTJ2_1009_MEASURE_MODE
	hw_gsensor_writedata(KXTJ2_1009_REG_POWER_CTL,uint8_t send_data)
*/
	 hw_heartrate_writedata(0x7f,0x02);
		
		hw_heartrate_writedata(0x1b,0x00);
	
	
}

void hw_heartrate_set_suspend(void)
{
	
  uint16_t i = 0;

	
			for(i = 0; i < SUSPEND_REG_ARRAY_SIZE;i++)
	  {
	    hw_heartrate_writedata(  suspend_register_array[i][0],suspend_register_array[i][1]);
	  
  	}
	
	
	
}




void hw_heartrate_irq(void)
{   

	//hw_heartrate_writedata(0x1A,0x00);
	
	//获取电平状态，高电平 佩戴  低电平 摘下
	// GPIO_PinState  state = GPIO_PIN_RESET;
	//uint16_t 
	
	//hw_led_key_chg_blue_set(1);
	//delay_ms(10);
	
	app_lcd_mutex_enter();
	heartrate_state = HAL_GPIO_ReadPin(HEARTRATE_SENSOR_INT_GPIO_Port, HEARTRATE_SENSOR_INT_Pin);
	app_lcd_mutex_exit();
/*
if(heartrate_state == 1)
{
  //touch
		hw_heartrate_checkid();
//	delay_ms(HEARTRATE_DELAY_MS);
	hw_heartrate_writedata(0x7f,0x02);
	//delay_ms(HEARTRATE_DELAY_MS);
	hw_heartrate_writedata(0x70,0x00);

}
 */

	
	//读取看是否有变动的心跳数据
	{
	  //0x64  0x5c
		//	hw_heartrate_writedata(0x7f,0x01);
	  //  delay_ms(2);
    //	hw_heartrate_writedata(0xD5,0x01);
	//delay_ms(HEARTRATE_DELAY_MS);
		/*
		{
			uint8_t tmp = 0;
		hw_heartrate_writedata(0x7f,0x00);
	  delay_ms(2); 
	  hw_heartrate_readdata(0x64,&tmp,1);	
			
			tmp;
			
			hw_heartrate_readdata(0x5c,&tmp,1);
			
			tmp;
			tmp;
			hw_heartrate_readdata(0x5c,&tmp,1);
			
		}
		*/
		/*
		{
			uint8_t tmp = 0, tmp1=0, tmp2 = 0; 
			hw_heartrate_writedata(0x7f,0x02);
	    delay_ms(2); 
	    hw_heartrate_readdata(0xA6,&tmp,1);	
			tmp;
			hw_heartrate_readdata(0xA7,&tmp1,1);	
				tmp1;
			hw_heartrate_readdata(0xA8,&tmp2,1);	
			
				tmp2;
			tmp2;
			
		}
		*/
/*
			uint8_t t = 0x01;
				hw_heartrate_writedata(0x7f,0x00);
			//	hw_heartrate_writedata(0x6b,0x01);
	  	hw_heartrate_writedata(0x6c,0x01);
		
		hw_heartrate_writedata(0x77,0x00);
		hw_heartrate_writedata(0x78,0x0c);
		hw_heartrate_writedata(0x7b,0x00);
		hw_heartrate_writedata(0x7c,0x0b);
		hw_heartrate_writedata(0x7d,0x00);
		hw_heartrate_writedata(0x7e,0x09);
		hw_heartrate_writedata(0x79,0x00);
		hw_heartrate_writedata(0x7a,0x08);
		hw_heartrate_writedata(0x6f,0x00);
		hw_heartrate_writedata(0x70,0x02);
		hw_heartrate_writedata(0x71,0x10);
		hw_heartrate_writedata(0x85,0x01);
		hw_heartrate_writedata(0xa7,0x01);
	*/
//hw_heartrate_test_init();	

hw_led_key_chg_blue_set(GPIO_PIN_RESET);
			/*
			
{0x77, 0x00},
{0x78, 0x0C},
			

{0x7B, 0x00},
{0x7C, 0x0B},

{0x7D, 0x00},
{0x7E, 0x09},

{0x79, 0x00},
{0x7A, 0x08},

{0x6F, 0x00},
{0x70, 0x02},

{0x71, 0x10},
{0x72, 0x00},

{0x85, 0x01},

{0xA7, 0x01},

//////////////////////
{0x6C, 0x00},

{0x0D, 0x78},
{0x0E, 0x00},

{0xBA, 0x7C},


*/

			
			
		
	}
	
	

  
	//gui_heartrate((uint8_t) state);
	//gui_gsensor((uint8_t) state);
	//gui_433((uint8_t) state);
/*  
	if(GPIO_PIN_RESET == state)
	{
	   gui_heartrate((uint8_t) state);
	}else{
	
	   gui_heartrate((uint8_t) state);
	}
	*/
}
	void hw_heartrate_test(void)
	{
		
			//uint8_t tmp = 0, tmp1=0, tmp2 = 0,tmp3 = 0,tmp4 = 0,fifo_size = 0;
		uint8_t fifo_size = 0;
		 uint16_t i=0; 
	   	uint8_t ss[5]="\r\n";
		
	  	uint8_t Touch_raw[600] = {0x00};
		
			hw_heartrate_writedata(0x7f,0x02);
	    delay_ms(2); 
	    hw_heartrate_readdata(0x91,&fifo_size,1);	
	  	delay_ms(2); 
		
		
		
			hw_heartrate_writedata(0x7f,0x03);
		
			hw_heartrate_readdata(0x00,Touch_raw,(fifo_size-1)*4);	
			
			
	  //	delay_ms(2); 
		/*
			//tmp1;
			hw_heartrate_readdata(0x02,&tmp2,1);	
	  	delay_ms(2); 

	  	hw_heartrate_readdata(0x03,&tmp3,1);	
	  	delay_ms(2); 
		
	  	hw_heartrate_readdata(0x04,&tmp4,1);	
			//tmp;
		
		 hw_heartrate_writedata(0x7f,0x02);
		
		hw_heartrate_writedata(0x75,0x01);
		hw_heartrate_writedata(0x75,0x00);
		
*/
//hw_lora_send(ss, 2);

	
	 //	if(fifo_size>1) 
	  { 
     //   i = (fifo_size-2)*4;
	//	tmp1 = (Touch_raw[i+3]<<24)+ (Touch_raw[i+2]<<16)+(Touch_raw[i+1]<<8)+ Touch_raw[i+0] ; 
			hw_lora_send(ss, 2);
			hw_lora_send(&fifo_size, 1);
			hw_lora_send(&Touch_raw[i+3], 1);
			hw_lora_send(&Touch_raw[i+2], 1);
			hw_lora_send(&Touch_raw[i+1], 1);
			hw_lora_send(&Touch_raw[i+0], 1);
      //  debug_printf("fifo_size: %d fifo_size_2: %d ", fifo_size,fifo_size_2); 
      //  debug_printf("Touch_raw: %6d  \n", tmp ); 
        
    } 
		
	////	hw_lora_send(ss, 2);
	//	hw_lora_send(&tmp, 1);
	//	hw_lora_send(&tmp1, 1);
	//	hw_lora_send(&tmp2, 1);
	//	hw_lora_send(&tmp3, 1);
	//	hw_lora_send(&tmp4, 1);
		
		/*
			hw_heartrate_writedata(0x7f,0x03);
	    delay_ms(2); 
	    hw_heartrate_readdata(0x00,&tmp,1);	
	  	delay_ms(2); 
					hw_heartrate_readdata(0x01,&tmp1,1);	
	  	delay_ms(2); 
			//tmp1;
			hw_heartrate_readdata(0x02,&tmp2,1);	
	  	delay_ms(2); 

	  	hw_heartrate_readdata(0x03,&tmp3,1);	
	  	delay_ms(2); 
		
	  	hw_heartrate_readdata(0x04,&tmp4,1);	
			//tmp;
		
		 hw_heartrate_writedata(0x7f,0x02);
		
		hw_heartrate_writedata(0x75,0x01);
		hw_heartrate_writedata(0x75,0x00);
		

		
		hw_lora_send(ss, 2);
		hw_lora_send(&tmp, 1);
		hw_lora_send(&tmp1, 1);
		hw_lora_send(&tmp2, 1);
		hw_lora_send(&tmp3, 1);
		hw_lora_send(&tmp4, 1);
*/			

		}
	

		