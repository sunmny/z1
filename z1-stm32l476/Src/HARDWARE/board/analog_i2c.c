
/**

  * @file  iic_dup.c

  * @brief IIC上层程序

  * @par   date        version    author    remarks

  *        2016-03-21  v1.0       zbt       初次创建

  *

  */

 

/** 头文件包含区 ------------------------------------------------ */


#include "analog_i2c.h"


#include "stm32L4xx_hal.h"
#define SDA_RCC_EN()	  __HAL_RCC_GPIOB_CLK_ENABLE()              //SDA时钟使能
#define SCL_RCC_EN()    __HAL_RCC_GPIOB_CLK_ENABLE()              //SCL时钟使能

#define SDA_PORT        GPIOB
#define SCL_PORT        GPIOB

#define SDA_PIN         GPIO_PIN_6
#define SCL_PIN         GPIO_PIN_7




 

/** 私有宏(类型定义) -------------------------------------------- */ 

#define IIC1_SCL(pin_status)        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, pin_status);

#define IIC1_SDA(pin_status)        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, pin_status);

#define IIC1_SCL_IS_HIGH()          (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7) != GPIO_PIN_RESET)

#define IIC1_SDA_IS_HIGH()          (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_6) != GPIO_PIN_RESET)

 #define IIC_WRITE 0xfe
 #define IIC_READ  0xff
 
 #define AT24C02_ADDR_BYTE           1

/** 私有变量 --------------------------------------------------- */

 

 

/** 外部变量 --------------------------------------------------- */

 

/** 私有函数原型 ----------------------------------------------- */

static void iic_delay(void);

 

/** 公有函数 --------------------------------------------------- */

/**

  * @brief  IIC启动

  * @param  None

  * @retval None

  * @note   当SCL处于高电平状态时，SDA出现一个下降沿

            即产生IIC启动信号

  */
	void i2c_soft_init(void)
{
/*
    GPIO_InitTypeDef GPIO_InitStructure; //GPIO Init Type
    //RCC Enable
    SDA_RCC_EN();
    SCL_RCC_EN();

    //Set the GPIO config with OD mode, Speed :50Mhz

    GPIO_InitStructure.Mode =  GPIO_MODE_OUTPUT_OD;
	

    GPIO_InitStructure.Pull = GPIO_NOPULL;
    GPIO_InitStructure.Speed = GPIO_SPEED_FAST;

    GPIO_InitStructure.Pin = SDA_PIN;
	  HAL_GPIO_Init(SDA_PORT,&GPIO_InitStructure);

    GPIO_InitStructure.Pin = SCL_PIN; 
	  HAL_GPIO_Init(SCL_PORT,&GPIO_InitStructure);


    //Set the I2C Bus in the free state
//    SDA_H;
//    SCL_H;
//    	  delay_ms(100);
*/
}

void iic_start(void)

{
	
//    i2c_soft_init();
    IIC1_SCL(GPIO_PIN_SET);

    //SDA产生一个下降沿 

    IIC1_SDA(GPIO_PIN_SET);

    iic_delay(); 

    

    IIC1_SDA(GPIO_PIN_RESET);

    iic_delay(); 

    IIC1_SCL(GPIO_PIN_RESET);   //< 拉低准备发送数据 

    iic_delay();   
	
	/*
//    i2c_soft_init();
    IIC1_SCL(GPIO_PIN_SET);

    //SDA产生一个下降沿 

    IIC1_SDA(GPIO_PIN_SET);

    iic_delay(); 

    

    IIC1_SDA(GPIO_PIN_RESET);

    iic_delay(); 

  //  IIC1_SCL(GPIO_PIN_RESET);   //< 拉低准备发送数据 

  //  iic_delay();  
	*/
}

 

/**

  * @brief  IIC停止

  * @param  None

  * @retval None

  * @note   当SCL处于高电平状态时，SDA出现一个上升沿

            即产生IIC停止信号

  */

void iic_stop(void)

{

    IIC1_SCL(GPIO_PIN_RESET);

    iic_delay();

    /** SDA产生一个上升沿 */

    IIC1_SDA(GPIO_PIN_RESET);

    iic_delay();

    

    IIC1_SCL(GPIO_PIN_SET);

    iic_delay();

    IIC1_SDA(GPIO_PIN_SET);

    iic_delay();

}

 

/**

  * @brief  IIC发送1byte数据

  * @param  None

  * @retval None

  * @note   

  */

void iic_sendbyte(uint8_t byte)

{

    uint8_t i;

    

    // 发送一个字节的高7位 

    for (i = 0; i < 8; i++)

    {

        if (byte & 0x80)

        {

            IIC1_SDA(GPIO_PIN_SET);

        }

        else

        {

            IIC1_SDA(GPIO_PIN_RESET);

        }

        

        iic_delay();

        IIC1_SCL(GPIO_PIN_SET);

        iic_delay();

        IIC1_SCL(GPIO_PIN_RESET);

        

        if (i == 7)

        {

            IIC1_SDA(GPIO_PIN_SET);

        }

        byte <<= 1;

        iic_delay();

    }     


/*
    // 发送一个字节的高7位 

    for (i = 0; i < 8; i++)

    {
			
IIC1_SCL(GPIO_PIN_RESET);
			
			 iic_delay();
			
        if (byte & 0x80)

        {

            IIC1_SDA(GPIO_PIN_SET);

        }

        else

        {

            IIC1_SDA(GPIO_PIN_RESET);

        }

        

        iic_delay();

        IIC1_SCL(GPIO_PIN_SET);

        iic_delay();

        IIC1_SCL(GPIO_PIN_RESET);

        

        if (i == 7)

        {

            IIC1_SDA(GPIO_PIN_SET);

        }

        byte <<= 1;

      //  iic_delay();

    } 
		IIC1_SCL(GPIO_PIN_RESET);
			
			 iic_delay();
			 */

}

 

/**

  * @brief  IIC读取1byte数据

  * @param  None

  * @retval None

  * @note             

  */

uint8_t iic_readbyte(void)

{

    uint8_t i;

    uint8_t recv_value = 0;

    

    IIC1_SDA(GPIO_PIN_SET);

    iic_delay();

    

    for (i = 0; i < 8; i++)

    {

        IIC1_SCL(GPIO_PIN_SET);

        iic_delay();

        recv_value <<= 1;

        if (IIC1_SDA_IS_HIGH())

        {

            recv_value |= 0x01;

        }

        else

        {

            recv_value &= ~0x01;

        }

        

        iic_delay();

        IIC1_SCL(GPIO_PIN_RESET);

    }

    

    return recv_value;

}

 

/**

  * @brief  IIC等待应答信号

  * @param  None

  * @retval ack_status： 应答状态，0表示应答，1表示设备无响应

  */

uint8_t iic_wait_ack(void)

{

    uint8_t ack_status = 0;

    

    /** 在等待应答信号之前，要释放总线，即将SDA置位 */

  //  IIC1_SDA(GPIO_PIN_SET);
	//

   // iic_delay();

    IIC1_SCL(GPIO_PIN_SET);

    iic_delay();

    

    if (IIC1_SDA_IS_HIGH())

    {    

        ack_status = 1;

        iic_stop();

    }

    else

    {

        ack_status = 0;

    }

    

    IIC1_SCL(GPIO_PIN_RESET);

    iic_delay();

   // ack_status = 0;

    return ack_status;

}

 

/**

  * @brief  主机（主控制器）产生应答信号

  * @param  None

  * @retval None

  */

void iic_ack(void)

{

    IIC1_SDA(GPIO_PIN_RESET);

    iic_delay();

    

    IIC1_SCL(GPIO_PIN_SET);

    iic_delay();

    IIC1_SCL(GPIO_PIN_RESET);

    iic_delay();

    

    IIC1_SDA(GPIO_PIN_SET);

}

 

/**

  * @brief  主机（主控制器）产生不应答信号

  * @param  None

  * @retval None

  */

void iic_nack(void)

{

    IIC1_SDA(GPIO_PIN_SET);

    iic_delay();

    

    IIC1_SCL(GPIO_PIN_SET);

    iic_delay();

    IIC1_SCL(GPIO_PIN_RESET);

    iic_delay();

}

 

/**

  * @brief  检测IIC总线上的设备状态

  * @param  device_addr： 从机设备地址 

  * @retval ack_status： 0 （正常）or 1（异常）

  * @note   主机发送设备地址等待从机应答，若有从机正确的应答信号

            则表明IIC总线上挂接了设备，否则表示IIC总线上未检测到

            设备

  */

uint8_t iic_check_device_status(uint8_t device_addr)

{

    uint8_t ack_status;

    

    if (IIC1_SCL_IS_HIGH() && IIC1_SDA_IS_HIGH())

    {

        iic_start();

        

        iic_sendbyte(device_addr | IIC_WRITE);

        ack_status = iic_wait_ack();

 

        iic_stop();

        

        return ack_status;

    }

    

    return 1;

}

 

/** 私有函数 --------------------------------------------------- */

/**

  * @brief  用于模拟IIC时的简单延时

  * @param  None

  * @retval None

  */

static void iic_delay(void)

{

    uint8_t i = 0;

   // uint8_t delay = 1;

    for(i= 0;i<200;i++);
    // HAL_Delay(1U);
	/*
    while (delay--)

    {

        //i = 4;

        //while (i--);

    }
	*/

}





/**

  * @file  at24c02.c

  * @brief at24c02驱动程序

  * @par   date        version    author    remarks

  *        2016-03-21  v1.0       zbt       初次创建

  *

  */


 

/** 私有宏(类型定义) -------------------------------------------- */ 

//#define AT24C02_DEVICE_ADDR         0xA0

//#define AT24C02_PAGE_SIZE           8

//#define AT24C02_MEM_SIZE            256

//#define AT24C02_ADDR_BYTE           1

 

/** 私有变量 --------------------------------------------------- */

//uint8_t test_buffer[AT24C02_MEM_SIZE];

 

/** 外部变量 --------------------------------------------------- */

 

/** 私有函数原型 ----------------------------------------------- */

//static void AT24C02_ack(void);

//static void AT24C02_error_handle(void);

//static void AT24C02_read_test(void);

//static void AT24C02_write_test(void);

//static void AT24C02_erase_test(void);

 static void analog_i2c_error_handle(void);

/** 公有函数 --------------------------------------------------- */

/**

  * @brief  AT24C02与主控制器的IIC通讯测试代码

  * @param  None

  * @retval None

  */
	/*

void AT24C02_iic_test(void)

{

    iic_stop();     //< 必须先复位IIC总线上的设备到待机模式 

    HAL_Delay(10);

    

    // 检测总线上是否挂接了IIC设备（此处为AT24C02） 

    if (iic_check_device_status(AT24C02_DEVICE_ADDR) == 0)

    {

//        printf("iic device exists\n");

    }

    else

    {

        printf("no iic device exists\n");

    }

    

    AT24C02_write_test();

    HAL_Delay(5);

    AT24C02_read_test();

    HAL_Delay(5);

    AT24C02_erase_test();

}

*/
 

/**

  * @brief  从AT24C02中读取数据

  * @param  read_data： 读取到的数据

  * @param  start_addr： 读取数据的起始地址

  * @param  data_length： 数据的长度

  * @retval None

  */

uint8_t analog_i2c_read_data(uint8_t slave_addr,uint8_t *read_data, uint16_t start_addr, uint16_t data_length)

{

    uint16_t i;

    

    iic_start();

    iic_sendbyte(slave_addr & IIC_WRITE);
	
	//printf("write addr=0%x\r\n",slave_addr& IIC_WRITE);
	//iic_delay();

//    AT24C02_ack();

    if (iic_wait_ack() != 0)

    {

        analog_i2c_error_handle();

      //  printf("first read error\r\n");
			return 1;

    }

    

    if (AT24C02_ADDR_BYTE == 1)

    {

        iic_sendbyte((uint8_t)start_addr & 0xff);

//        AT24C02_ack();

        if (iic_wait_ack() != 0)

        {

            analog_i2c_error_handle();

            printf("addr byte error\r\n");
					return 1;

        }

    }

    

    iic_start();

    //iic_sendbyte(slave_addr | IIC_READ);
		 iic_sendbyte(slave_addr);
		//printf("read addr=0%x\r\n",slave_addr);
//iic_delay();

//    AT24C02_ack();

    if (iic_wait_ack() != 0)

    {

        analog_i2c_error_handle();

        printf("read data error\r\n");
			return 1;

    }

    

    for (i = 0; i < data_length; i++)

    {

        read_data[i] = iic_readbyte();

        

        if (i != data_length - 1)

        {

            iic_ack();      /**< 读完非最后一个字节后， 产生应答信号 */

        }

        else

        {

            iic_nack();     /**< 最后一个字节读完后 产生非应答信号 */

        }

//        printf("read data is %d\n", read_data[i]);  /**< 调试代码 */

    }

    

    iic_stop();
		
		return 0;

}

 

/**

  * @brief  通过IIC向AT24C02写数据

  * @param  write_data：  要写入AT24C02的数据指针

  * @param  start_addr：  要写入AT24C02的起始地址

  * @param  data_length： 要写入AT24C02的数据长度

  * @retval None

  */

uint8_t analog_i2c_write_data(uint8_t slave_addr,uint8_t *write_data, uint16_t start_addr, uint16_t data_length)

{

    uint16_t i, j;

    uint16_t start_addr_dup;

    

    start_addr_dup = start_addr;

    for (i = 0; i < data_length; i++)
    {
				if(i == 0)
        {
         for (j = 0; j < 20; j++)
         {
                iic_start();
                iic_sendbyte(slave_addr & IIC_WRITE);
                if (iic_wait_ack() == 0)   
                {

                    break;

                }

            }

            if (j >= 20)

            {

                printf("j = 1000\r\n");

                analog_i2c_error_handle();
							return 1;

            }

            if (AT24C02_ADDR_BYTE == 1)

            {

                iic_sendbyte((uint8_t)start_addr_dup & 0xff);

//                AT24C02_ack();

                if (iic_wait_ack() != 0)

                {

                    analog_i2c_error_handle();

                    printf("addr_byte wrong\r\n");
									return 1;

                }

            }
						

        }

        

        iic_sendbyte(write_data[i]);

//        AT24C02_ack();

        if (iic_wait_ack() != 0)

        {

            analog_i2c_error_handle();

            printf("write failed\r\n");
					return 1;

        }

        

//        printf("write_data is %d \n", write_data[i]);    /**< 调试代码 */ 

        start_addr_dup++;

    }

    

    iic_stop();
		return 0;

}

 

/** 私有函数 --------------------------------------------------- */

/** 以下为测试用程序 ------------------------------------------- */

/**

  * @brief  AT24C02读取数据测试

  * @param  None

  * @retval None

  */

/*
static void AT24C02_read_test(void)

{

    uint16_t i;

    

    AT24C02_read_data(test_buffer, 0, (AT24C02_MEM_SIZE % 10));

    

    printf("read data is:\n");

    for (i = 0; i < (AT24C02_MEM_SIZE % 10); i++)

    {

        printf("%d ", test_buffer[i]);

    }

    

    printf("\r\nread test ok\r\n");

}
*/
 

/**

  * @brief  AT24C02写数据测试

  * @param  None

  * @retval None

  */
/*
static void AT24C02_write_test(void)

{

    uint16_t i;

    

    for (i = 0; i < (AT24C02_MEM_SIZE % 10); i++)

    {

        test_buffer[i] = i;

    }

    

    AT24C02_write_data(test_buffer, 0, (AT24C02_MEM_SIZE % 10));

    

    printf("write data is:\n");

    for (i = 0; i < (AT24C02_MEM_SIZE % 10); i++)

    {

        printf("%d ", test_buffer[i]);

    }

    

    printf("\r\nwrite test ok\r\n");

}
*/
 

/**

  * @brief  AT24C02擦除数据测试

  * @param  None

  * @retval None

  */
/*
static void AT24C02_erase_test(void)

{

    uint16_t i;

    

    for (i = 0; i < (AT24C02_MEM_SIZE % 10); i++)

    {

        test_buffer[i] = 0xff;

    }

    

    AT24C02_write_data(test_buffer, 0, (AT24C02_MEM_SIZE % 10));

    

    printf("erase value is: \n");

    for (i = 0; i < (AT24C02_MEM_SIZE % 10); i++)

    {

        printf("%d ", test_buffer[i]);

    }

    

    printf("\r\nerase test ok\r\n");

}
*/

/** 以上为测试用程序 ------------------------------------------- */

 

/**

  * @brief  AT24C02应答错误处理程序

  * @param  None

  * @retval None

  */

static void analog_i2c_error_handle(void)

{

    iic_stop();

//    printf("At24C02 read failed\r\n");   /**< 调试用 */

}

 

//static void AT24C02_ack(void)

//{

//    if (iic_wait_ack() != 0)

//    {

//        AT24C02_error_handle();

//    }

//}

 

 

