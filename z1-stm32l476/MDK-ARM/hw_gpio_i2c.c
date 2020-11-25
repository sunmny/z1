/**
  ******************************************************************************
  * @file    Project/Drive/INC_I2C.c
  * @author  Inc
  * @version V0.0.1
  * @date    13-Sep-2015
  * @brief   The Module of I2C
  ******************************************************************************
  * @attention
  *
  *
  ******************************************************************************
  */

#include "stm32l4xx_hal.h"
//=============================
// I2C TIME CHART
//=============================
/**
* I2C TIME CHART
*
*
    -+      +----------+---------+...----+   ...-+     +-------+
SDA  |      |          |         |       |       |     |       |
     +------+----------+---------+...    +---...-+-----+       +--

SCL ---+     +----+      +----+      +----+          +------------+
       |     |    |      |    |      |    |          |         	  |
       +-----+    +------+    +-..---+    +--......--+            +--
     | |     |    |    | |           |  |          | | |
     |1|     |  3 |5   |6|           | 4|          |9|7|   8   |
     |                                  |              |
     |                                  |              |
     +START                              +RESTART      +STOP
   (4 1 5) (6  3   5)                              (9 7 8)
																	   stm32  Slave
																	   MIN		MAX
1: Hold Time START Condition											74ns	-
2: Clock Low Time(5+6)
3: Clock High Time								i2c_delay_clockhigh
4: Setup Time for a Repeated START Condition							370ns	-
5: Data Hold Time								i2c_delay_datahold		50ns	900ns
6: Data Setup Time								i2c_delay_datasetup		74ns	-
7: Set-up time for STOP condition										370ns	-
8: Bus Free Time between a STOP and a START Condition 					740ns	 -
9: prepare time for STOP condition
A: ack wait time

I2C SPEED
=========
	slow			depend on system.
	Standard		100kbps
	Fast mode		400kbps
	Fast mode plus	1Mbit
	High-Speed		3.4Mbit				HS-mode

	stm32 uses about 35kHz.

*/

//==============================================================================
//Include
//==============================================================================


//==============================================================================
//Define
//==============================================================================
//------------------------------------------------------------------------------\\


#define _OWN_I2C_SOFT
#define SDA_RCC_EN()	__HAL_RCC_GPIOB_CLK_ENABLE()              //SDA????
#define SCL_RCC_EN()    __HAL_RCC_GPIOB_CLK_ENABLE()              //SCL????

#define SDA_PORT        GPIOB
#define SCL_PORT        GPIOB

#define SDA_PIN         GPIO_PIN_9
#define SCL_PIN         GPIO_PIN_8

//-------------------------------------------------------------------------------
#define SDA_H           (SDA_PORT->BSRR |= SDA_PIN)
#define SCL_H           (SCL_PORT->BSRR |= SCL_PIN)

#define SDA_L           (SDA_PORT->BRR |= SDA_PIN)
#define SCL_L           (SCL_PORT->BRR |= SCL_PIN)

#define SDA_READ        (SDA_PORT->IDR & SDA_PIN)
#define SCL_READ        (SCL_PORT->IDR & SCL_PIN)

//-------------------------------------------------------------------------------
#define ACK		1
#define NACK	0
#define TRUE  0
#define FALSE 1
//-------------------------------------------------------------------------------
static uint8_t I2cDelayBase = 1;

//-----------------------
// I2C DELAY
// Note: It depends on CACHE, MCUSPI clock, & SPIOSD.
//-----------------------
static void dd(uint8_t delay)
{
	uint8_t i,j;
	j = I2cDelayBase;
	while(j--) {
		for(i=0; i < delay; i++);
	}
}

//I2cDelayBase = 1                   //32kHz
#define i2c_delay_1		dd(8)		 //1.8uS
#define i2c_delay_2		dd(13)		 //2.5uS
#define i2c_delay_3		dd(5)		 //1.5uS
#define i2c_delay_4		dd(37)		 //5.5uS
#define i2c_delay_5		dd(5)		 //1.5uS
#define i2c_delay_6		dd(8)		 //1.8uS
#define i2c_delay_7		dd(10)		 //5.5uS
#define i2c_delay_8		dd(74)		 //11uS
#define i2c_delay_9		dd(10)		 //2.2uS
#define i2c_delay_ACK	dd(8)		 //1.8uS


//==============================================================================
//Static Function
//==============================================================================
//------------------------------------------------------------------------------
//I2C_Soft Initial
//Param:  None
//Return: None
void i2c_soft_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure; //GPIO Init Type
    //RCC Enable
    SDA_RCC_EN();
    SCL_RCC_EN();

    //Set the GPIO config with OD mode, Speed :50Mhz

    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStructure.Pull = GPIO_NOPULL;
    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;

    GPIO_InitStructure.Pin = SDA_PIN;
	HAL_GPIO_Init(SDA_PORT,&GPIO_InitStructure);

    GPIO_InitStructure.Pin = SCL_PIN;
	HAL_GPIO_Init(SCL_PORT,&GPIO_InitStructure);


    //Set the I2C Bus in the free state
    SDA_H;
    SCL_H;
}


//------------------------------------------------------------------------------
//i2c_start
//Param:  None
//Return: bool FLASE : ??   TRUE : ??
/*
 *		----+
 *	SDA:	|_____
 *		------+
 *	SCL:      |___
*/
uint8_t  i2c_start(void)
{
    SDA_H;
    SCL_H;
    i2c_delay_7;
    if(!SDA_READ) {
        return FALSE; 
    }
    SDA_L;
    i2c_delay_7;
    if(SDA_READ) {
        return FALSE; 
    }
    SCL_L;
    i2c_delay_7;
    return TRUE;
}



//------------------------------------------------------------------------------
//i2c_stop
//Param:  None
//Return: None
/*
 *		     +-------
 *	SDA:_____|
 *		   +---------
 *	SCL:___|
*/

void i2c_stop(void)
{
    SCL_L;
    i2c_delay_7;
    SDA_L;
    i2c_delay_7;
    SCL_H;
    i2c_delay_7;
    SDA_H;
    i2c_delay_7;
}


//------------------------------------------------------------------------------
//i2c_ack
//Param:  None
//Return: None
/*
 *
 *	SDA:____________
 *		   +---+
 *	SCL:___|   |____
*/

void i2c_ack(void)
{
    SCL_L;
	i2c_delay_7;
	SDA_L;
	i2c_delay_7;
	SCL_H;
	i2c_delay_7;
	SCL_L;
	i2c_delay_7;
}



//------------------------------------------------------------------------------
//i2c_no_ack
//Param:  None
//Return: None
/*
 *		+--------------
 *	SDA:|
 *		   +---+
 *	SCL:___|   |_______
*/
void i2c_no_ack(void)
{
    SCL_L;
	i2c_delay_7;
	SDA_H;
	i2c_delay_7;
	SCL_H;
	i2c_delay_7;
	SCL_L;
	i2c_delay_7;
}



//------------------------------------------------------------------------------
//i2c_wait_ack
//Param:  None
//Return: ACK: TRUE;  NACK: FALSE
/*
 *		+-------- | +   +-----| +--------+ |
 *	SDA:          | |___|     |            |
 *		   +----- | ------+   | -------+   |
 *	SCL:___|      | ACK   |__ |  NACK  |__ |
*/

uint8_t i2c_wait_ack(void)
{
    SCL_L;
	i2c_delay_7;
	SDA_H;
	i2c_delay_7;
	SCL_H;
	i2c_delay_7;
	if(SDA_READ) {
        SCL_L;
        i2c_delay_7;
        return(NACK);
	}
	SCL_L;
	i2c_delay_7;
    return(ACK);
}


//------------------------------------------------------------------------------
//i2c_send_byte
//Param:  uint8_t chByte
//Return: None
/*
 *		  +-------+-------+-------+-------+-------+-------+-------+------+
 *	SDA:__|_______|_______|_______|_______|_______|_______|_______|______|
 *		    +---+   +---+   +---+   +---+   +---+   +---+   +---+   +---+
 *	SCL:____| 1 |___| 2 |___| 3 |___| 4 |___| 5 |___| 6 |___| 7 |___| 8 |________
 */

void i2c_send_byte(uint8_t chByte)
{
    uint8_t i = 0;
    for(i = 0; i< 8; i++) {
        SCL_L;
		i2c_delay_7;
		(chByte & 0x80) ? SDA_H : SDA_L;
		chByte<<=1;
		i2c_delay_7;
		SCL_H;
		i2c_delay_7;
    }
    SCL_L;
}


//------------------------------------------------------------------------------
//i2c_read_byte
//Param:  None
//Return: uint8_t
/*
 *		--+-------+-------+-------+-------+-------+-------+-------+------+
 *	SDA:  |_______|_______|_______|_______|_______|_______|_______|______|
 *		    +---+   +---+   +---+   +---+   +---+   +---+   +---+   +---+
 *	SCL:____| 1 |___| 2 |___| 3 |___| 4 |___| 5 |___| 6 |___| 7 |___| 8 |________
 */
uint8_t i2c_read_byte(void)
{
    uint8_t i=0;
    uint8_t ReceiveByte=0;

    SDA_H; // free SDA
    for(i = 0; i < 8; i++) {
		SCL_H;
		i2c_delay_7;
		ReceiveByte<<=1;
		if(SDA_READ) {
			ReceiveByte|=0x01;
		}
		SCL_L;
		i2c_delay_7;
    }
    SCL_L;
    return ReceiveByte;
}

//==============================================================================
//Globe Function
//==============================================================================
//------------------------------------------------------------------------------
//i2c_single_write
//Param:  uint8_t chSlaveAddr
//Param:  uint8_t chRegAddr
//Param:  uint8_t chByte
//Return: bool  FALSE : ??   TRUE : ??
uint8_t i2c_single_write( uint8_t chSlaveAddr,
                        uint8_t chRegAddr,
                        uint8_t chByte)
{
    if(!i2c_start()){
        return FALSE;
    }
    i2c_send_byte(chSlaveAddr);   //??????
    if(NACK == i2c_wait_ack()){
        i2c_stop(); //???
        return FALSE;
    }
    i2c_send_byte(chRegAddr );   //???????
    i2c_wait_ack();
    i2c_send_byte(chByte);
    i2c_wait_ack();
    i2c_stop();
    return TRUE;
}


//------------------------------------------------------------------------------
//i2c_single_write16
//Param:  uint8_t chSlaveAddr
//Param:  uint16_t hwRegAddr
//Param:  uint8_t chByte
//Return: bool  FALSE : ??   TRUE : ??
uint8_t i2c_single_write16( uint8_t chSlaveAddr,
                        uint16_t hwRegAddr,
                        uint8_t chByte)
{
    if(!i2c_start()){
        return FALSE;
    }
    i2c_send_byte(chSlaveAddr);   //??????
    if(NACK == i2c_wait_ack()){
        i2c_stop(); //???
        return FALSE;
    }
    i2c_send_byte(hwRegAddr>>8 );   //???????
    i2c_wait_ack();
	i2c_send_byte((uint8_t)hwRegAddr );   //???????
    i2c_wait_ack();
    i2c_send_byte(chByte);
    i2c_wait_ack();
    i2c_stop();
    return TRUE;
}


//------------------------------------------------------------------------------
//i2c_multi_write
//Param:  uint8_t chSlaveAddr
//Param:  uint8_t chRegAddr
//Param:  uint8_t *pBuf
//Param:  uint8_t chLen
//Return: bool  FALSE : ??   TRUE : ??
uint8_t i2c_multi_write( uint8_t chSlaveAddr,
						uint8_t chRegAddr,
						uint8_t *pBuf,
						uint16_t hwLen)
{
	uint16_t i;
	if(!i2c_start()){
        return FALSE;
    }
    i2c_send_byte(chSlaveAddr);   //??????
    if(NACK == i2c_wait_ack()){
        i2c_stop(); //???
        return FALSE;
    }
    i2c_send_byte(chRegAddr );   //???????
    i2c_wait_ack();
	for(i = 0; i< hwLen; i++) {
	    i2c_send_byte(pBuf[i]);
	    i2c_wait_ack();
	}
    i2c_stop();
    return TRUE;
}


//------------------------------------------------------------------------------
//i2c_multi_write16
//Param:  uint8_t chSlaveAddr
//Param:  uint16_t hwRegAddr
//Param:  uint8_t *pBuf
//Param:  uint16_t hwLen
//Return: bool  FALSE : ??   TRUE : ??
uint8_t i2c_multi_write16( uint8_t chSlaveAddr,
						uint16_t hwRegAddr,
						uint8_t *pBuf,
						uint16_t hwLen)
{
	uint16_t i;
	if(!i2c_start()){
        return FALSE;
    }
    i2c_send_byte(chSlaveAddr);   //??????
    if(NACK == i2c_wait_ack()){
        i2c_stop(); //???
        return FALSE;
    }
    i2c_send_byte(hwRegAddr>>8 );   //???????
    i2c_wait_ack();
	i2c_send_byte((uint8_t)hwRegAddr);   //???????
    i2c_wait_ack();
	for(i = 0; i< hwLen; i++) {
	    i2c_send_byte(pBuf[i]);
	    i2c_wait_ack();
	}
    i2c_stop();
    return TRUE;
}


//------------------------------------------------------------------------------
//i2c_single_read
//Param:  uint8_t chSlaveAddr
//Param:  uint8_t chRegAddr
//Return: uint8_t byte
uint8_t i2c_single_read(uint8_t chSlaveAddr,
                     uint8_t chRegAddr)
{
	uint8_t chRet;
    if(!i2c_start()){
        return 0;
    }
    i2c_send_byte(chSlaveAddr); //????ID
    if(!i2c_wait_ack()) {
		i2c_stop();
		return 0;
	}
    i2c_send_byte(chRegAddr);   //???????
    i2c_wait_ack();
    i2c_start();
    i2c_send_byte(chSlaveAddr|0x01);
    i2c_wait_ack();

	chRet = i2c_read_byte();
    i2c_no_ack();
    i2c_stop();
    return chRet;
}


//------------------------------------------------------------------------------
//i2c_single_read16
//Param:  uint8_t chSlaveAddr
//Param:  uint16_t hwRegAddr
//Return: uint8_t byte
uint8_t i2c_single_read16(uint8_t chSlaveAddr,
                     uint16_t hwRegAddr)
{
	uint8_t chRet;
    if(!i2c_start()){
        return 0;
    }
    i2c_send_byte(chSlaveAddr); //????ID
    if(!i2c_wait_ack()) {
		i2c_stop();
		return 0;
	}
    i2c_send_byte(hwRegAddr>>8);   //???????
    i2c_wait_ack();
    i2c_send_byte((uint8_t)hwRegAddr);   //???????
    i2c_wait_ack();
    i2c_start();
    i2c_send_byte(chSlaveAddr|0x01);
    i2c_wait_ack();

	chRet = i2c_read_byte();
    i2c_no_ack();
    i2c_stop();
    return chRet;
}



//------------------------------------------------------------------------------
//i2c_multi_read
//Param:  uint8_t chSlaveAddr
//Param:  uint8_t chRegAddr
//Param:  uint8_t *pBuf
//Param:  uint16_t hwLen
//Return: bool
void i2c_multi_read(uint8_t chSlaveAddr,
                    uint8_t chRegAddr,
                    uint8_t *pBuf,
                    uint16_t hwLen)
{
    uint16_t i;

    if(hwLen < 1)
			return;
    if(!i2c_start())
			return;
    i2c_send_byte(chSlaveAddr);
    if(!i2c_wait_ack()) {
		i2c_stop();
		return;
	}
    i2c_send_byte(chRegAddr);
    i2c_wait_ack();
	//---------
    i2c_start();
    i2c_send_byte(chSlaveAddr |0x01);
    i2c_wait_ack();

    for(i=1;i<hwLen; i++) {
        *pBuf++ = i2c_read_byte();
        i2c_ack();
    }
    *pBuf++ = i2c_read_byte();
    i2c_no_ack();
    i2c_stop();
}


//------------------------------------------------------------------------------
//i2c_multi_read16
//Param:  uint8_t chSlaveAddr
//Param:  uint16_t hwRegAddr
//Param:  uint8_t *pBuf
//Param:  uint16_t hwLen
//Return: bool
void i2c_multi_read16(uint8_t chSlaveAddr,
                    uint16_t hwRegAddr,
                    uint8_t *pBuf,
                    uint16_t hwLen)
{
    uint16_t i;

    if(hwLen < 1)
			return;
    if(!i2c_start())
			return;
    i2c_send_byte(chSlaveAddr);
    if(!i2c_wait_ack()) {
		i2c_stop();
		return;
	}
    i2c_send_byte(hwRegAddr>>8);
    i2c_wait_ack();
	 i2c_send_byte((uint8_t)hwRegAddr);
    i2c_wait_ack();
	//---------
    i2c_start();
    i2c_send_byte(chSlaveAddr |0x01);
    i2c_wait_ack();

    for(i=1;i<hwLen; i++) {
        *pBuf++ = i2c_read_byte();
        i2c_ack();
    }
    *pBuf++ = i2c_read_byte();
    i2c_no_ack();
    i2c_stop();
}



/*************************End Of File(Inc)*************************************/

