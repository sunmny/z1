#include "hw_i2c.h" 
#include "hw_config.h"
#include "stm32l4xx_hal.h"
#include "app_aoa.h"
#define aw9523_addr    0x58

#define	READ_CW2015				0xc5
#define	WRITE_CW2015			0xc4

#define REG_VERSION             0x0
#define REG_VCELL               0x2
#define REG_SOC                 0x4
#define REG_RRT_ALERT           0x6
#define REG_CONFIG              0x8
#define REG_MODE                0xA
#define REG_BATINFO             0x10

#define MODE_SLEEP_MASK         (0x3<<6)
#define MODE_SLEEP              (0x3<<6)
#define MODE_NORMAL             (0x0<<6)
#define MODE_QUICK_START        (0x3<<4)
#define MODE_RESTART            (0xf<<0)

#define CONFIG_UPDATE_FLG       (0x1<<1)

#define ATHD                    (0x0<<3)        //ATHD = 0%

#define SIZE_BATINFO        64
static void delay_ms(uint32_t ms)
{	
		uint32_t i ,j;
		
		for(i =0;i<ms ;i++){
					for(j =0;j<1000;j++);
		
		}

}
#define BATTERY_UP_MAX_CHANGE   720             // the max time allow battery change quantity
#define BATTERY_DOWN_MIN_CHANGE 60             // the min time allow battery change quantity when run
#define BATTERY_DOWN_MIN_CHANGE_SLEEP 1800      // the min time allow battery change quantity when run 30min
//#define BAT_LOW_INTERRUPT    1
//#define CW2015_GET_RRT
uint8_t cw_read116(uint8_t point_reg,uint8_t *w_pdata,uint8_t len);
uint8_t cw_read(uint8_t point_reg,uint8_t *w_pdata);
uint8_t cw_read16(uint8_t point_reg,uint8_t *w_pdata,uint8_t len);
static unsigned char cw_bat_config_info[SIZE_BATINFO] = {
0x15  ,0x7e  ,0x63  ,0x5b  ,0x45  ,0x7d  ,0x64  ,
0x38  ,0x43  ,0x49  ,0x49  ,0x46  ,0x41  ,0x43  ,
0x39  ,0x29  ,0x1f  ,0x16  ,0x12  ,0x0E  ,0x16  ,
0x27  ,0x41  ,0x48  ,0x29  ,0x33  ,0x0C  ,0x29  ,
0x42  ,0x64  ,0x50  ,0x5a  ,0x68  ,0x6b  ,0x6D  ,
0x6d  ,0x3e  ,0x1a  ,0x8a  ,0x2c  ,0x09  ,0x37  ,
0x21  ,0x4b  ,0x7e  ,0x96  ,0xa2  ,0x3b  ,0x54  ,
0x83  ,0x95  ,0x97  ,0x80  ,0x83  ,0xa6  ,0xCB  ,
0x2F  ,0x00  ,0x64  ,0xA5  ,0xB5  ,0x0b  ,0xd8  ,
0x41
};

//****************************struct*********************************/
typedef struct tagSTRUCT_CW_BATTERY {
	unsigned char usb_online;
	unsigned int capacity;
	unsigned int voltage;
#ifdef CW2015_GET_RRT
	unsigned int time_to_empty;
#endif
	unsigned char alt;
}STRUCT_CW_BATTERY;
uint8_t bat_bd_soc = 0xff;

uint8_t bat_zdev_soc = 0xff;
void zdev_read_battery_soc(void)
{
	uint8_t val_1[2] ={0};
	uint8_t data;
	cw_read16(0x04,val_1,2);
	//cw_read(0x05,&vhl);
	
	bat_zdev_soc = (((val_1[0]*256)+val_1[1])*100)/(256*100);
	//printf(" data = %d val  = %x vhl  = %x \r\n",bat_zdev_soc,val_1[0],val_1[1]);


}


void zdev_read_battery1_soc(void)
{
	uint8_t val_2[2] ={0};
	
	cw_read116(0x04,val_2,2);
	//cw_read(0x05,&vhl);
	
	bat_bd_soc = (((val_2[0]*256)+val_2[1])*100)/(256*100);
	//printf("11 data = %d val  = %x vhl  = %x \r\n",bat_bd_soc ,val_2[0],val_2[1]);


}
extern uint8_t bat_val1,bat_val2;

void zdev_read_battery_adc(void)
{
	uint8_t val[2] ={0};
	
	cw_read16(0x02,val,2);
	//cw_read(0x05,&vhl);
	bat_val1 = val[0];
	bat_val2 = val[1];
	
	//*data = (((val[0]*256)+val[1])*100)/(256*100);
	//printf(" adc data val  = %x vhl  = %x \r\n",val[0],val[1]);

}
//0x2b -0x30
uint8_t set_val_to_report(void)
{
	
	if(zdev_set.bat_chr_status ==0){
	
	zdev_read_battery_adc();
		if(bat_val1 >0x33)
					zdev_set.zbat_soc =4;
		else if( bat_val1>0x31 && bat_val1 <=0x33)
					zdev_set.zbat_soc =3;
		else if(bat_val1 >= 0x2f && bat_val1 <= 0x31)
					zdev_set.zbat_soc =2;
		else if(bat_val1>=  0x2c && bat_val1 <0x2f)
					zdev_set.zbat_soc =1;
		else if(bat_val1 <0x2c){
					zdev_set.zbat_soc =0;
					app_power_open();
		}
		printf("zdev_set.zbat_soc = %x bat_val1 = %x\r\n",bat_zdev_soc,bat_val1);
	}else{
	
	
	zdev_read_battery_soc();
	if(bat_bd_soc!=0xff){
		if(bat_zdev_soc >=75 )
					zdev_set.zbat_soc =4;
		else if( (bat_zdev_soc>=50) && (bat_zdev_soc < 75))
					zdev_set.zbat_soc =3;
		else if((bat_zdev_soc>=25) && (bat_zdev_soc < 50) )
					zdev_set.zbat_soc =2;
		else if((bat_zdev_soc>=5) && (bat_zdev_soc < 25))
					zdev_set.zbat_soc =1;
		else if(bat_zdev_soc <= 5){
					zdev_set.zbat_soc =0;
						app_power_open();
		}
			return 0;
	}
	printf("bat_zdev_soc = %x \r\n",bat_zdev_soc);
}
			
}
uint8_t bat1_val2=0xff,bat1_val3;
void zdev_read_battery1_adc(void)
{
	uint8_t val[2] ={0},ret;
	
	ret = cw_read116(0x02,val,2);
	//cw_read(0x05,&vhl);
	if(ret ==0){
	 bat1_val2 = val[0];
	 bat1_val3 = val[1];
		zdev_set.zbattery = 1; /// beidou module is in
	}else{
		zdev_set.zbattery = 0;
	}
	//*data = (((val[0]*256)+val[1])*100)/(256*100);
	
	//printf(" adc data val1  = %x vhl1  = %x \r\n",bat1_val2,bat1_val3);

}
//0x2b -0x30

uint8_t set_val1_to_report(void)
{
	
	
	if(zdev_set.bat1_chr_status ==0){
		zdev_read_battery1_adc();
		if(bat1_val2 == 0xff){
			zdev_set.zbat_soc1 = 0;
		}else if(bat1_val2 >0x33&&bat1_val2!=0xff)
					zdev_set.zbat_soc1 = 4;
		else if( bat1_val2>0x30 && bat1_val2 <=0x33)
					zdev_set.zbat_soc1 = 3;
		else if(bat1_val2 > 0x2d && bat1_val2 <=0x30)
					zdev_set.zbat_soc1 = 2;
		else if(bat1_val2>  0x2c && bat1_val2 <=0x2d)
					zdev_set.zbat_soc1 = 1;
		else if(bat1_val2 <0x2c)
					zdev_set.zbat_soc1 = 0;
		
		printf("zdev_set.zbat_soc1 = %d bat1_val2 = %x\r\n",zdev_set.zbat_soc1,bat1_val2);
	
	}else{
		zdev_read_battery1_soc();
	if(bat_bd_soc!= 0xff){
	if(bat_bd_soc >=75 )
					zdev_set.zbat_soc1 =4;
		else if((bat_bd_soc>=50) && (bat_bd_soc < 75) )
					zdev_set.zbat_soc1 =3;
		else if((bat_bd_soc>=25) && (bat_bd_soc < 50))
					zdev_set.zbat_soc1 =2;
		else if((bat_bd_soc>=5) && (bat_bd_soc < 25))
					zdev_set.zbat_soc1 =1;
		else if(bat_bd_soc <= 5)
					zdev_set.zbat_soc1 =0;
		printf("zdev_set.zbat_soc1  = %d bat_bd_soc = %d\r\n",zdev_set.zbat_soc1,bat_bd_soc);
		return 0;
	}
}
		
}
uint8_t cw_write_9523(uint8_t point_reg,uint8_t w_pdata)
{
	uint8_t data[2] ;
	uint8_t ret ;
		data[0] = point_reg;
		data[1] =  w_pdata;
	
	ret = hw_i2c_write( 0x01, 0xb6, data,  2);
	if(ret)
			return 1;
		
		return 0;
	
}
uint8_t cw_write(uint8_t point_reg,uint8_t *w_pdata)
{
	uint8_t data[2] ;
	uint8_t ret ;
		data[0] = point_reg;
		data[1] =  *w_pdata;
	
	ret = hw_i2c_write( 0x01, 0xc4, data,  2);
	if(ret)
			return 1;
		
		return 0;
	
}


uint8_t cw_write1(uint8_t point_reg,uint8_t *w_pdata)
{
	uint8_t data[2] ;
	uint8_t ret ;
		data[0] = point_reg;
		data[1] =  *w_pdata;
	
	//ret = hw_i2c_write( 0x00, 0xc4, data,  2);
	//analog_i2c_write_data(uint8_t slave_addr,uint8_t *write_data, uint16_t start_addr, uint16_t data_length)
	analog_i2c_write_data(WRITE_CW2015,w_pdata,point_reg,1);
	ret =0;
	
	if(ret)
			return 1;
		
		return 0;
	
}
uint8_t cw_read1(uint8_t point_reg,uint8_t *w_pdata)
{

	uint8_t data ,ret;
	
		data = point_reg;

/*	
 ret = hw_i2c_write( 0x00, 0xc4, &data,  1);
	if(ret){
			return 1;
		}
 ret = hw_i2c_read(0x00,0xc5,w_pdata,1);
	if(ret){
			return 1;
	}
	*/
	//printf("w_pdata = %x \r\n",*w_pdata);
	ret = analog_i2c_read_data(READ_CW2015,w_pdata,point_reg,1);
	
	//ret =1;
	return ret;
}
uint8_t cw_read16(uint8_t point_reg,uint8_t *w_pdata,uint8_t len)
{

		uint8_t data ,ret;
	
		data = point_reg;

	
 ret = hw_i2c_write( 0x01, 0xc4, &data,  1);
	if(ret){
			return 1;
		}
 ret = hw_i2c_read(0x01,0xc5,w_pdata,len);
	if(ret){
			return 1;
	}
	//printf("w_pdata = %x \r\n",*w_pdata);
	return 0;
}

uint8_t cw_read116(uint8_t point_reg,uint8_t *w_pdata,uint8_t len)
{

		uint8_t ret;

	
 	ret = analog_i2c_read_data(READ_CW2015,w_pdata,point_reg,2);
	printf("w_pdata = %x \r\n",*w_pdata);

	
	if(ret){
			return 1;
		
	}
	return 0;
}
uint8_t cw_read(uint8_t point_reg,uint8_t *w_pdata)
{

	uint8_t data ,ret;
	
		data = point_reg;

	
 ret = hw_i2c_write( 0x01, 0xc4, &data,  1);
	if(ret){
			return 1;
		}
 ret = hw_i2c_read(0x01,0xc5,w_pdata,1);
	if(ret){
			return 1;
	}
	//printf("w_pdata = %x \r\n",*w_pdata);
	return 0;
}
uint8_t cw_read_9523(uint8_t point_reg,uint8_t *w_pdata)
{

	uint8_t data ,ret;
	
		data = point_reg;

	
 ret = hw_i2c_write( 0x01, 0xb6, &data,  1);
	if(ret){
			return 1;
		}
 ret = hw_i2c_read(0x01,0xb7,w_pdata,1);
	if(ret){
			return 1;
	}
	//printf("w_pdata = %x \r\n",*w_pdata);
	return 0;
}


uint8_t cw_write_2772(uint8_t point_reg,uint8_t w_pdata)
{
	uint8_t data[2] ;
	uint8_t ret ;
		data[0] = point_reg;
		data[1] =  w_pdata;
	
	ret = hw_i2c_write(0x01, 0x72, data,  2);
	if(ret)
			return 1;
		
		return 0;
	
}


uint8_t cw_read_2772(uint8_t point_reg,uint8_t *w_pdata)
{

	uint8_t data ,ret;
	
		data = point_reg;

	
 ret = hw_i2c_write( 0x01, 0x72, &data,  1);
	if(ret){
			return 1;
		}
 ret = hw_i2c_read(0x01,0x73,w_pdata,2);
	if(ret){
			return 1;
	}
	//printf("w_pdata = %x \r\n",*w_pdata);
	return 0;
}

uint8_t cw_update_config_info1(void)
{
	uint8_t ret = 0;
	unsigned char i;
	unsigned char reset_val;
	unsigned char reg_val;
	/* make sure no in sleep mode */
	ret = cw_read1(REG_MODE, &reg_val);
	if(ret)
	{
		return 1;
	}
	if((reg_val & MODE_SLEEP_MASK) == MODE_SLEEP)
	{
		return 2;
	}
	/* update new battery info */
	for(i = 0; i < SIZE_BATINFO; i++)
	{
		reg_val = cw_bat_config_info[i];
		ret = cw_write1(REG_BATINFO+i, &reg_val);
		if(ret)
		{
			return 1;
		}
	}
 
	return 0;
}
uint8_t cw_update_config_info(void)
{
	uint8_t ret = 0;
	unsigned char i;
	unsigned char reset_val;
	unsigned char reg_val;
	/* make sure no in sleep mode */
	ret = cw_read(REG_MODE, &reg_val);
	if(ret)
	{
		return 1;
	}
	if((reg_val & MODE_SLEEP_MASK) == MODE_SLEEP)
	{
		return 2;
	}
	/* update new battery info */
	for(i = 0; i < SIZE_BATINFO; i++)
	{
		reg_val = cw_bat_config_info[i];
		ret = cw_write(REG_BATINFO+i, &reg_val);
		if(ret)
		{
			return 1;
		}
	}

	/* readback & check */
	for(i = 0; i < SIZE_BATINFO; i++)
	{
		ret = cw_read(REG_BATINFO+i, &reg_val);
		if(ret)
		{
			return 1;
		}
		if(reg_val != cw_bat_config_info[i])
		{
			return 3;
		}
	}
	/* set cw2015/cw2013 to use new battery info */
	ret = cw_read(REG_CONFIG, &reg_val);
	if(ret)
	{
		return 1;
	}
	reg_val |= CONFIG_UPDATE_FLG;   /* set UPDATE_FLAG */
	reg_val &= 0x07;                /* clear ATHD */
	reg_val |= ATHD;                /* set ATHD */
	ret = cw_write(REG_CONFIG, &reg_val);
	if(ret)
	{
		return 1;
	}
	/* reset */
	reset_val = MODE_NORMAL;
	reg_val = MODE_RESTART;
	ret = cw_write(REG_MODE, &reg_val);
	if(ret)
	{
		return 1;
	}
  
	ret = cw_write(REG_MODE, &reset_val);
	if(ret)
	{
		return 1;
	}   
	return 0;
}
unsigned char cw_init(void)
{
	unsigned ret;
	unsigned char i;
	unsigned char reg_val = MODE_NORMAL;
	uint8_t bat;
	
//printf("init cw_init \r\n");
	
	zdev_read_battery_adc();
	/* wake up cw2015/13 from sleep mode */
	ret = cw_write(REG_MODE, &reg_val);
	
	if(ret != 0)
	{
		
		return 1;
	}

	/* check ATHD if not right */
	ret = cw_read(REG_CONFIG, &reg_val);
	printf(" init reg_val = %d ret = %d \r\n",reg_val,ret);
	if(ret)
	{
		return 1;
	}
	if((reg_val & 0xf8) != ATHD)
	{
		//"the new ATHD need set"
		reg_val &= 0x07;    /* clear ATHD */
		reg_val |= ATHD;    /* set ATHD */
		ret = cw_write(REG_CONFIG, &reg_val);
		if(ret)
		{
			return 1;
		}
	}
	
	/* check config_update_flag if not right */
	ret = cw_read(REG_CONFIG, &reg_val);
	if(ret)
	{
		return 1;
	}
	printf("init2 reg_val = %x \r\n",reg_val);
	
			ret = cw_read(REG_VCELL, &reg_val);
	printf("init3 val reg_val = %x \r\n",reg_val);
			bat =0x03;
	ret = cw_read(bat, &reg_val);
	printf("init4 val reg_val = %x \r\n",reg_val);
	if(!(reg_val & CONFIG_UPDATE_FLG))
	{
		//"update flag for new battery info need set"
		ret = cw_update_config_info();
		printf("CONFIG_UPDATE_FLG ret = %x  \r\n",ret);
		if(ret)
		{
			return ret;
		}
	}
	//else
	//{
		
		
		//for(i = 0; i < SIZE_BATINFO; i++)
	//	{ 
		//	ret = cw_read(REG_BATINFO +i, &reg_val);
		//	printf("reg_val = %x i = %d \r\n",reg_val,i);
		//	if(ret)
		//	{
		//		return 1;
		//	}
		//	if(cw_bat_config_info[i] != reg_val)
		//	{
			//	break;
		//	}
		//}
	//	printf("i = %x \r\n",i);
	//	if(i != SIZE_BATINFO)
	//	{
			//"update flag for new battery info need set"
		//	ret = cw_update_config_info();
		//	if(ret)
		//	{
		//		return ret;
		//	}
		//}
	//}
	
	ret = cw_read(REG_SOC, &reg_val);
	printf("check soc reg_val = %x \r\n",reg_val);
	/* check SOC if not eqaul 255 */
	//for (i = 0; i < 30; i++) {
		//ret = cw_read(REG_SOC, &reg_val);
		//printf("check soc reg_val = %x \r\n",reg_val);
	//	if (ret)
		//	return 1;
		//else if (reg_val <= 100) 
		//	break;		
   // }
	
   // if (i >=30){
      //  reg_val = MODE_SLEEP;
      //  ret = cw_write(REG_MODE, &reg_val);
        // "cw2015/cw2013 input unvalid power error_2\n";
       // return 4;
   // }
	zdev_read_battery_adc();

	}	

unsigned char cw_init1(void)
{
	unsigned ret;
	unsigned char i;
	unsigned char reg_val = MODE_NORMAL;
	uint8_t bat;
	
//printf("init cw_init1 \r\n");
	/* wake up cw2015/13 from sleep mode */
	ret = cw_write1(REG_MODE, &reg_val);
	


	/* check ATHD if not right */
	ret = cw_read1(REG_CONFIG, &reg_val);
//	printf("cw_read1 = %d \r\n",reg_val);
	if(ret)
	{
		return 1;
	}
	if((reg_val & 0xf8) != ATHD)
	{
		//"the new ATHD need set"
		reg_val &= 0x07;    /* clear ATHD */
		reg_val |= ATHD;    /* set ATHD */
		ret = cw_write1(REG_CONFIG, &reg_val);
		if(ret)
		{
			return 1;
		}
	}
	
	/* check config_update_flag if not right */
	ret = cw_read1(REG_CONFIG, &reg_val);
	if(ret)
	{
		return 1;
	}
//	printf("cw_read1 = %x \r\n",reg_val);
	
			ret = cw_read1(REG_VCELL, &reg_val);
//	printf(" cw_read1 = %x \r\n",reg_val);
			bat =0x03;
	ret = cw_read1(bat, &reg_val);
//	printf(" cw_read1 = %x \r\n",reg_val);
	if(!(reg_val & CONFIG_UPDATE_FLG))
	{
		//"update flag for new battery info need set"
		ret = cw_update_config_info1();
//		printf("cw_read1 CONFIG_UPDATE_FLG ret = %x  \r\n",ret);
		if(ret)
		{
			return ret;
		}
	}

	}		
void set_rd_red_light(uint8_t enable)
{
	uint8_t led_curr4 = 0x24;
	if(enable){
		cw_write_9523(led_curr4,0x30);
	}else {
		cw_write_9523(led_curr4,0x00);
	}
}
void set_rn_blue_light(uint8_t enable)
{
	uint8_t led_curr5 = 0x25;
	if(enable){
		cw_write_9523(led_curr5,0x30);
	}else {
		cw_write_9523(led_curr5,0x00);
	}
}

void set_ble_red_light(uint8_t enable)
{
	uint8_t led_curr6 = 0x26;
	if(enable){
		cw_write_9523(led_curr6,0x30);
	}else {
		cw_write_9523(led_curr6,0x00);
	}
}
void aw9523_init_led(void)
{
		uint8_t reg = 0x12,val = 0x00,i =0;
		uint8_t led_mode = 0x13;
		uint8_t led_mode1 = 0x12;
		uint8_t led_curr4 = 0x24;
		uint8_t led_curr5 = 0x25;
		uint8_t led_curr6 = 0x26;
		uint8_t led_mode2 = 0x11;
	// HAL_GPIO_WritePin(LED_9523_RESET_GPIO_Port, LED_9523_RESET_Pin, GPIO_PIN_SET);
	
			cw_read_9523(reg,&val);
//	printf("read reg12 = %x \r\n",val);
			
				delay_ms(100);
			cw_write_9523(led_mode2,0x13);

			delay_ms(100);
			//cw_write_9523(led_mode,0xcf);
		//	delay_ms(100);
			cw_write_9523(led_mode1,0xf8);
				delay_ms(100);
	
		cw_read_9523(led_mode1,&val);
//		printf("read reg123 = %x \r\n",val);
					#if 1

			
			set_rd_red_light(1);
		
		//	cw_write_9523(led_curr5,0x30);
			
		
			//cw_write_9523(led_curr6,0x30);
	
	
			
			#endif
		//	printf("9523 val = %x \r\n",val );
		//	printf("9523 val = %x \r\n",val );
		

}
void aw9523_set_light(uint8_t count,uint8_t val)
{
		
			cw_write_9523(count,val);
			//delay_ms(1000);
		
}


void tmd2772_init(void)
{
		uint8_t reg =0x80 ,val[2] ={0};
			
		cw_write_2772(reg,0x0f);
		delay_ms(100);
		reg =0x81;
		cw_write_2772(reg,0xee);
		delay_ms(100);
	
		reg =0x82;
		cw_write_2772(reg,0xff);
		delay_ms(100);
	
		reg =0x83;
		cw_write_2772(reg,0xfc);
		delay_ms(100);

			
}

void read_ps_data(uint8_t *val)
{
	uint8_t reg;
	
	
		reg = 0x98;
		cw_read_2772(reg,val);

		//printf("2772 val =  %x %x \r\n",val[0],val[1]);

}