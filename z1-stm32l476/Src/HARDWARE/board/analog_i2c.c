
/**

  * @file  iic_dup.c

  * @brief IIC�ϲ����

  * @par   date        version    author    remarks

  *        2016-03-21  v1.0       zbt       ���δ���

  *

  */

 

/** ͷ�ļ������� ------------------------------------------------ */


#include "analog_i2c.h"


#include "stm32L4xx_hal.h"
#define SDA_RCC_EN()	  __HAL_RCC_GPIOB_CLK_ENABLE()              //SDAʱ��ʹ��
#define SCL_RCC_EN()    __HAL_RCC_GPIOB_CLK_ENABLE()              //SCLʱ��ʹ��

#define SDA_PORT        GPIOB
#define SCL_PORT        GPIOB

#define SDA_PIN         GPIO_PIN_6
#define SCL_PIN         GPIO_PIN_7




 

/** ˽�к�(���Ͷ���) -------------------------------------------- */ 

#define IIC1_SCL(pin_status)        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, pin_status);

#define IIC1_SDA(pin_status)        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, pin_status);

#define IIC1_SCL_IS_HIGH()          (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7) != GPIO_PIN_RESET)

#define IIC1_SDA_IS_HIGH()          (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_6) != GPIO_PIN_RESET)

 #define IIC_WRITE 0xfe
 #define IIC_READ  0xff
 
 #define AT24C02_ADDR_BYTE           1

/** ˽�б��� --------------------------------------------------- */

 

 

/** �ⲿ���� --------------------------------------------------- */

 

/** ˽�к���ԭ�� ----------------------------------------------- */

static void iic_delay(void);

 

/** ���к��� --------------------------------------------------- */

/**

  * @brief  IIC����

  * @param  None

  * @retval None

  * @note   ��SCL���ڸߵ�ƽ״̬ʱ��SDA����һ���½���

            ������IIC�����ź�

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

    //SDA����һ���½��� 

    IIC1_SDA(GPIO_PIN_SET);

    iic_delay(); 

    

    IIC1_SDA(GPIO_PIN_RESET);

    iic_delay(); 

    IIC1_SCL(GPIO_PIN_RESET);   //< ����׼���������� 

    iic_delay();   
	
	/*
//    i2c_soft_init();
    IIC1_SCL(GPIO_PIN_SET);

    //SDA����һ���½��� 

    IIC1_SDA(GPIO_PIN_SET);

    iic_delay(); 

    

    IIC1_SDA(GPIO_PIN_RESET);

    iic_delay(); 

  //  IIC1_SCL(GPIO_PIN_RESET);   //< ����׼���������� 

  //  iic_delay();  
	*/
}

 

/**

  * @brief  IICֹͣ

  * @param  None

  * @retval None

  * @note   ��SCL���ڸߵ�ƽ״̬ʱ��SDA����һ��������

            ������IICֹͣ�ź�

  */

void iic_stop(void)

{

    IIC1_SCL(GPIO_PIN_RESET);

    iic_delay();

    /** SDA����һ�������� */

    IIC1_SDA(GPIO_PIN_RESET);

    iic_delay();

    

    IIC1_SCL(GPIO_PIN_SET);

    iic_delay();

    IIC1_SDA(GPIO_PIN_SET);

    iic_delay();

}

 

/**

  * @brief  IIC����1byte����

  * @param  None

  * @retval None

  * @note   

  */

void iic_sendbyte(uint8_t byte)

{

    uint8_t i;

    

    // ����һ���ֽڵĸ�7λ 

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
    // ����һ���ֽڵĸ�7λ 

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

  * @brief  IIC��ȡ1byte����

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

  * @brief  IIC�ȴ�Ӧ���ź�

  * @param  None

  * @retval ack_status�� Ӧ��״̬��0��ʾӦ��1��ʾ�豸����Ӧ

  */

uint8_t iic_wait_ack(void)

{

    uint8_t ack_status = 0;

    

    /** �ڵȴ�Ӧ���ź�֮ǰ��Ҫ�ͷ����ߣ�����SDA��λ */

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

  * @brief  ��������������������Ӧ���ź�

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

  * @brief  ����������������������Ӧ���ź�

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

  * @brief  ���IIC�����ϵ��豸״̬

  * @param  device_addr�� �ӻ��豸��ַ 

  * @retval ack_status�� 0 ��������or 1���쳣��

  * @note   ���������豸��ַ�ȴ��ӻ�Ӧ�����дӻ���ȷ��Ӧ���ź�

            �����IIC�����Ϲҽ����豸�������ʾIIC������δ��⵽

            �豸

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

 

/** ˽�к��� --------------------------------------------------- */

/**

  * @brief  ����ģ��IICʱ�ļ���ʱ

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

  * @brief at24c02��������

  * @par   date        version    author    remarks

  *        2016-03-21  v1.0       zbt       ���δ���

  *

  */


 

/** ˽�к�(���Ͷ���) -------------------------------------------- */ 

//#define AT24C02_DEVICE_ADDR         0xA0

//#define AT24C02_PAGE_SIZE           8

//#define AT24C02_MEM_SIZE            256

//#define AT24C02_ADDR_BYTE           1

 

/** ˽�б��� --------------------------------------------------- */

//uint8_t test_buffer[AT24C02_MEM_SIZE];

 

/** �ⲿ���� --------------------------------------------------- */

 

/** ˽�к���ԭ�� ----------------------------------------------- */

//static void AT24C02_ack(void);

//static void AT24C02_error_handle(void);

//static void AT24C02_read_test(void);

//static void AT24C02_write_test(void);

//static void AT24C02_erase_test(void);

 static void analog_i2c_error_handle(void);

/** ���к��� --------------------------------------------------- */

/**

  * @brief  AT24C02������������IICͨѶ���Դ���

  * @param  None

  * @retval None

  */
	/*

void AT24C02_iic_test(void)

{

    iic_stop();     //< �����ȸ�λIIC�����ϵ��豸������ģʽ 

    HAL_Delay(10);

    

    // ����������Ƿ�ҽ���IIC�豸���˴�ΪAT24C02�� 

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

  * @brief  ��AT24C02�ж�ȡ����

  * @param  read_data�� ��ȡ��������

  * @param  start_addr�� ��ȡ���ݵ���ʼ��ַ

  * @param  data_length�� ���ݵĳ���

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

            iic_ack();      /**< ��������һ���ֽں� ����Ӧ���ź� */

        }

        else

        {

            iic_nack();     /**< ���һ���ֽڶ���� ������Ӧ���ź� */

        }

//        printf("read data is %d\n", read_data[i]);  /**< ���Դ��� */

    }

    

    iic_stop();
		
		return 0;

}

 

/**

  * @brief  ͨ��IIC��AT24C02д����

  * @param  write_data��  Ҫд��AT24C02������ָ��

  * @param  start_addr��  Ҫд��AT24C02����ʼ��ַ

  * @param  data_length�� Ҫд��AT24C02�����ݳ���

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

        

//        printf("write_data is %d \n", write_data[i]);    /**< ���Դ��� */ 

        start_addr_dup++;

    }

    

    iic_stop();
		return 0;

}

 

/** ˽�к��� --------------------------------------------------- */

/** ����Ϊ�����ó��� ------------------------------------------- */

/**

  * @brief  AT24C02��ȡ���ݲ���

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

  * @brief  AT24C02д���ݲ���

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

  * @brief  AT24C02�������ݲ���

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

/** ����Ϊ�����ó��� ------------------------------------------- */

 

/**

  * @brief  AT24C02Ӧ����������

  * @param  None

  * @retval None

  */

static void analog_i2c_error_handle(void)

{

    iic_stop();

//    printf("At24C02 read failed\r\n");   /**< ������ */

}

 

//static void AT24C02_ack(void)

//{

//    if (iic_wait_ack() != 0)

//    {

//        AT24C02_error_handle();

//    }

//}

 

 

