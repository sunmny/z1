#include "hw_uart.h" 
#include "stm32l4xx_hal.h"
//#include "cmsis_os.h"
#include "string.h"
//#include "delay.h"
#include "app_bd2.h"
#include "hw_config.h"
#include "zb_common_data.h"
#include "app_ble.h"
#include "app_aoa.h"

uint8_t one_complete_nmea_sentence[NMEA_DATA_MAX_SIZE+1];
uint8_t bd2_uart_data_loop[BD2_UART_DATA_LOOP_SIZE+1];
uint8_t response_location[256];
uint8_t response_location_header[]="+ZLOCINFO:";

uint8_t have_response_location = 0;

GpsLocation gpslocation;


uint16_t bd2_uart_data_loop_len;


static uint16_t
nmea_tokenizer_init( NmeaTokenizer*  t, const uint8_t*  p, const uint8_t*  end )
{
    uint16_t    count = 0;
    uint8_t*  q;

    // the initial '$' is optional
    if (p < end && p[0] == '$')
        p += 1;

    // remove trailing newline
    if (end > p && (*(end-1) == '\n')) {
        end -= 1;
        if (end > p && (*(end-1) == '\r'))
            end -= 1;
    }

    // get rid of checksum at the end of the sentecne
    if (end >= p+3 && (*(end-3) == '*')) {
        end -= 3;
    }

    while (p < end) {
        const uint8_t*  q = p;

        q = memchr(p, ',', end-p);
        if (q == NULL)
            q = end;

        if (q >= p) {
            if (count < MAX_NMEA_TOKENS) {
                t->tokens[count].p   = p;
                t->tokens[count].end = q;
                count += 1;
            }
        }
        if (q < end)
            q += 1;

        p = q;
    }

    t->count = count;
    return count;
}

static Token
nmea_tokenizer_get( NmeaTokenizer*  t, uint16_t  index )
{
    Token  tok;
    static const uint8_t*  dummy = "";

    if (index < 0 || index >= t->count) {
        tok.p = tok.end = dummy;
    } else
        tok = t->tokens[index];

    return tok;
}

static double
convert_from_hhmm( Token  tok )
{
    double  val     = str2float(tok.p, tok.end);
    int     degrees = (int)(floor(val) / 100);
    double  minutes = val - degrees*100.;
    double  dcoord  = degrees + minutes / 60.0;
    return dcoord;
}

static uint16_t
nmea_reader_update_latlong(Token        latitude,
                            uint8_t         latitudeHemi,
                            Token        longitude,
                            uint8_t         longitudeHemi )
{
#if 0
    double   lat, lon;
    Token    tok;

    tok = latitude;
    if (tok.p + 6 > tok.end) {
        return 1;
    }
    lat = convert_from_hhmm(tok);
    if (latitudeHemi == 'S')
        lat = -lat;

    tok = longitude;
    if (tok.p + 6 > tok.end) {
        return 1;
    }
    lon = convert_from_hhmm(tok);
    if (longitudeHemi == 'W')
        lon = -lon;
#else //need change to dd.mmm, upload nmea origin data now, need discuss with customer
	Token    tok;

	tok = latitude;
	memset(gpslocation.latitude,0x00,sizeof(gpslocation.latitude));
	memcpy(gpslocation.latitude, tok.p, tok.end-tok.p);
	gpslocation.lat_hemi[0]=latitudeHemi;

	tok=longitude;
	memset(gpslocation.longitude,0x00,sizeof(gpslocation.longitude));
	memcpy(gpslocation.longitude, tok.p, tok.end-tok.p);
	gpslocation.long_hemi[0]=longitudeHemi;

	gpslocation.flags |= GPS_LOCATION_HAS_LAT_LONG;
#endif
    return 0;
}

static uint16_t
nmea_reader_update_altitude(Token        altitude,
                             Token        units )
{
	double  alt;
	Token   tok = altitude;

	if (tok.p >= tok.end)
	    return -1;

	gpslocation.flags |= GPS_LOCATION_HAS_ALTITUDE;
	memset(gpslocation.altitude,0x00,sizeof(gpslocation.altitude));
	memcpy(gpslocation.altitude, tok.p, tok.end-tok.p);
    return 0;
}


void nmea_data_gga_parse(NmeaTokenizer* tzer)
{
	Token  tok_latitude      = nmea_tokenizer_get(tzer,2);
	Token  tok_latitudeHemi  = nmea_tokenizer_get(tzer,3);
	Token  tok_longitude     = nmea_tokenizer_get(tzer,4);
	Token  tok_longitudeHemi = nmea_tokenizer_get(tzer,5);
	Token  tok_fixStatus 	 = nmea_tokenizer_get(tzer,6);
	Token  tok_altitude      = nmea_tokenizer_get(tzer,9);
	Token  tok_altitudeUnits = nmea_tokenizer_get(tzer,10);

	if(tok_fixStatus.p[0] != '0')
	{
		nmea_reader_update_latlong(tok_latitude,
	                                      tok_latitudeHemi.p[0],
	                                      tok_longitude,
	                                      tok_longitudeHemi.p[0]);
		nmea_reader_update_altitude(tok_altitude, tok_altitudeUnits);
	}
	else			
	{
		gpslocation.flags &= 0;//to do...
		memset(gpslocation.latitude,0x00,sizeof(gpslocation.latitude));
		memset(gpslocation.lat_hemi,0x00,sizeof(gpslocation.lat_hemi));
		memset(gpslocation.longitude,0x00,sizeof(gpslocation.longitude));
		memset(gpslocation.long_hemi,0x00,sizeof(gpslocation.long_hemi));
		memset(gpslocation.altitude,0x00,sizeof(gpslocation.altitude));
	}
	
}

static uint16_t
nmea_reader_update_date(Token  date, Token  time )
{
	Token  tok = date;
	uint16_t    year;

	if (tok.p + 6 != tok.end) {
	    return 1;
	}
	year = (uint16_t)str2int(tok.p+4, tok.p+6) + 2000;
	int2str(year,gpslocation.date);
	memcpy(&gpslocation.date[4],tok.p+2,2);
	memcpy(&gpslocation.date[6],tok.p,2);

	tok = time;
	memcpy(gpslocation.time,tok.p,6);//only hhmmss
	gpslocation.flags |=GPS_LOCATION_HAS_DATATIME;
	return 0;
}

static uint16_t
nmea_reader_update_speed(Token speed )
{
    double  alt;
    Token   tok = speed;

    if (tok.p >= tok.end)
        return 1;

    gpslocation.flags   |= GPS_LOCATION_HAS_SPEED;

    //Modify by ZQH to convert the speed unit from knot to m/s
    //r->fix.speed    = str2float(tok.p, tok.end);
    //gpslocation.speed = str2float(tok.p, tok.end) / 1.942795467;

    if(tok.end - tok.p > SPEED_SIZE)
    {
    	memcpy(gpslocation.speed,tok.p,SPEED_SIZE);
    }
	else
	{
		memcpy(gpslocation.speed,tok.p,tok.end-tok.p);
	}
    return 0;
}


void nmea_data_rmc_parse(NmeaTokenizer* tzer)
{
	Token  tok_time          = nmea_tokenizer_get(tzer,1);
	Token  tok_fixStatus     = nmea_tokenizer_get(tzer,2);
	Token  tok_latitude      = nmea_tokenizer_get(tzer,3);
	Token  tok_latitudeHemi  = nmea_tokenizer_get(tzer,4);
	Token  tok_longitude     = nmea_tokenizer_get(tzer,5);
	Token  tok_longitudeHemi = nmea_tokenizer_get(tzer,6);
	Token  tok_speed         = nmea_tokenizer_get(tzer,7);
	Token  tok_bearing       = nmea_tokenizer_get(tzer,8);
	Token  tok_date          = nmea_tokenizer_get(tzer,9);

	// VER("in RMC, fixStatus=%c", tok_fixStatus.p[0]);
	if (tok_fixStatus.p[0] == 'A')
	{
	    nmea_reader_update_date(tok_date, tok_time );

	    nmea_reader_update_latlong(tok_latitude,
	                                   tok_latitudeHemi.p[0],
	                                   tok_longitude,
	                                   tok_longitudeHemi.p[0] );

	    nmea_reader_update_speed(tok_speed);
	}
	else
	{
		gpslocation.flags &= 0;//to do...
		memset(gpslocation.latitude,0x00,sizeof(gpslocation.latitude));
		memset(gpslocation.lat_hemi,0x00,sizeof(gpslocation.lat_hemi));
		memset(gpslocation.longitude,0x00,sizeof(gpslocation.longitude));
		memset(gpslocation.long_hemi,0x00,sizeof(gpslocation.long_hemi));
		memset(gpslocation.date,0x00,sizeof(gpslocation.date));
		memset(gpslocation.time,0x00,sizeof(gpslocation.time));
		memset(gpslocation.speed,0x00,sizeof(gpslocation.speed));
	}
}

static void add_separator_comma(uint8_t *data)
{
		*data=',';
}

void set_to_hex(uint8_t *data)
{
		uint8_t temp,temp1;
		temp = (*data)/16;
		if(temp<1){
			temp1 = *data%16;
			if(temp1<0x0a){
				*data = temp1;
			}else if(temp1 == 0x0a){
					*data = 0x0a;
			}else if(temp1 ==0x0b){
					*data = 0x0b;
			}else if(temp1 ==0x0c){
					*data = 0x0c;
			}else if(temp1 ==0x0d){
					*data = 0x0d;
			}else if(temp1 ==0x0e){
					*data = 0x0e;
			}else if(temp1 ==0x0f){
					*data = 0x0f;
			}
		}else if(temp >= 1){
					
				temp1 = *data%16;
			
				if(temp1<0x0a){
				*data = temp<<4 | temp1;
			}else if(temp1 == 0x0a){
					*data = temp<<4 |0x0a;
			}else if(temp1 ==0x0b){
					*data = temp<<4 |0x0b;
			}else if(temp1 ==0x0c){
					*data = temp<<4 |0x0c;
			}else if(temp1 ==0x0d){
					*data = temp<<4 |0x0d;
			}else if(temp1 ==0x0e){
					*data = temp<<4 |0x0e;
			}else if(temp1 ==0x0f){
					*data = temp<<4 |0x0f;
			}
		}


}
uint8_t set_latitude_to_int(uint8_t *data,uint8_t *out)
{
		uint8_t i =0,j=0;
		uint8_t buf[12];
	
	
			while(data[i]){
					if((data[i]>= '0')&& (data[i] <= '9')){
									buf[j] = data[i];
									j++;
					}
					i++;
					if(i > strlen(data))
							break;
			}
			for(i =0;i<(j/2);i++){
				out[i] = (buf[i*2] -'0')*10 + buf[i*2+1] - '0';
			}
		return i;
}

uint8_t set_longitude_to_int(uint8_t *data,uint8_t *out)
{
		uint8_t i =0,j=0;
		uint8_t buf[12];
	
	
			while(data[i]){
					if((data[i]>= '0')&& (data[i] <= '9')){
									buf[j] = data[i];
									j++;
					}
					i++;
					if(i > strlen(data))
							break;
			}
			out[0] = (buf[0] -'0')*100 + (buf[1] - '0')*10 + (buf[2]-'0');
			
			for(i =1;i<(j/2);i++){
				out[i] = (buf[i*2+1] -'0')*10 + buf[i*2] - '0';
			}
		return i;
}



uint16_t make_location_response(void)
{
	uint16_t totlelen=0;
	uint16_t len=0;

	len=strlen(response_location_header);
	memcpy(response_location,response_location_header,len);
	totlelen+=len;
	
	memcpy(&response_location[totlelen],zdev_set.mydev_id,12);
	totlelen+=12;
	memcpy(&response_location[totlelen],",",1);
	totlelen+=1;
	len=strlen(gpslocation.latitude);
	if(len<1){
		memcpy(&response_location[totlelen],"F",1);
	totlelen+=1;
	}else{
	memcpy(&response_location[totlelen],gpslocation.latitude,len);
	totlelen+=len;
	}
	add_separator_comma(&response_location[totlelen]);
	totlelen+=1;

	len=strlen(gpslocation.lat_hemi);
	if(len<1){
		memcpy(&response_location[totlelen],"F",1);
		totlelen+=1;
	}else{
	memcpy(&response_location[totlelen],gpslocation.lat_hemi,len);
	totlelen+=len;
	}
	add_separator_comma(&response_location[totlelen]);
	totlelen+=1;

	len=strlen(gpslocation.longitude);
	if(len <1){
		memcpy(&response_location[totlelen],"F",1);
		totlelen+=1;
	}else{
	memcpy(&response_location[totlelen],gpslocation.longitude,len);
	totlelen+=len;
	}
	add_separator_comma(&response_location[totlelen]);
	totlelen+=1;

	
	len=strlen(gpslocation.long_hemi);
	if(len<1){
		memcpy(&response_location[totlelen],"F",1);
		totlelen+=1;
	}else{
	memcpy(&response_location[totlelen],gpslocation.long_hemi,len);
	totlelen+=len;
	}

	add_separator_comma(&response_location[totlelen]);
	totlelen+=1;


	len=strlen(gpslocation.speed);
	if(len <1){
			memcpy(&response_location[totlelen],"F",1);
		totlelen+=1;
	}else {
	memcpy(&response_location[totlelen],gpslocation.speed,len);
	totlelen+=len;
	}
	memcpy(&response_location[totlelen],"##\r\n",4);
	totlelen+=4;
	
	return totlelen;
}


void nmea_data_parse(uint8_t *data, uint16_t len)
{
	uint16_t resp_len;
	if((0==memcmp(data, "$GNGGA", 6)) || (0==memcmp(data, "$GPGGA", 6)))
	{
		NmeaTokenizer  tzer[1];
		Token          tok;
		
		nmea_tokenizer_init(tzer, data, data+len);
		nmea_data_gga_parse(tzer);
		//hw_uart2_send(data, len);
	}
	else if((0==memcmp(data, "$GNRMC", 6))||(0==memcmp(data, "$GPRMC", 6)))
	{
		NmeaTokenizer  tzer[1];
		Token          tok;
		
		nmea_tokenizer_init(tzer, data, data+len);
		nmea_data_rmc_parse(tzer);
		//hw_uart2_send(data, len);
	}
	printf("gps gpslocation.flags = %x \r\n",gpslocation.flags);
	if((gpslocation.flags & GPS_LOCATION_HAS_LAT_LONG) && (gpslocation.flags &GPS_LOCATION_HAS_SPEED) && (gpslocation.flags &GPS_LOCATION_HAS_DATATIME))
	{
		resp_len=make_location_response();
		ft_dev.rn_flags = 1;
		printf("ft_dev.rn_flags = %d \r\n",ft_dev.rn_flags);
		
	}
	
}

uint16_t bd_send_resp_len;
uint16_t get_zdevice_location_response(uint8_t *buf)
{
		uint8_t resp_len ;
	
	bd_send_resp_len=make_location_response();/*todo, this sentence needed? maybe deleted 20181113*/
	memcpy(buf,response_location,bd_send_resp_len);
	resp_len = bd_send_resp_len;
	return resp_len;
}

uint8_t bd2_uart_loop_data_handle(uint8_t *data, uint16_t len)
{
	uint16_t index = 0;
	uint16_t parse_len=0;
	//while(bd2_uart_data_loop[index++]!=0x00)
	for(index=0;index<len;index++)
	{
		one_complete_nmea_sentence[parse_len] = bd2_uart_data_loop[index];
		parse_len++;

		if(one_complete_nmea_sentence[parse_len-1]=='\n')
		{
			nmea_data_parse(one_complete_nmea_sentence, parse_len);
			memset(one_complete_nmea_sentence,0x00,sizeof(one_complete_nmea_sentence));
			parse_len=0;
		}
	}
	memset(bd2_uart_data_loop,0x00,sizeof(bd2_uart_data_loop));	
}
extern USART_RECEIVETYPE UsartType3;
void rn_receive_and_handle(void)
{
	memset(bd2_uart_data_loop,0x00,sizeof(bd2_uart_data_loop));
	hw_uart3_receive(bd2_uart_data_loop, &bd2_uart_data_loop_len, 0xff);
	printf(" rn bd2_uart_data_loop= %s \r\n",bd2_uart_data_loop);

	if (bd2_uart_data_loop_len>0)
	{
		bd2_uart_loop_data_handle(bd2_uart_data_loop, bd2_uart_data_loop_len);
		//hw_uart2_send(bd2_uart_data_loop, bd2_uart_data_loop_len);
		bd2_uart_data_loop_len=0;
	}
}

