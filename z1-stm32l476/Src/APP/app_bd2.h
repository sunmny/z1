#ifndef BD2_H
#define BD2_H

#include <stdint.h>
#include "stm32l4xx_hal.h"

#define BD2_UART_DATA_LOOP_SIZE 1536
#define NMEA_DATA_MAX_SIZE 512//127
#define LATITUDE_SIZE 11
#define LONGITUDE_SIZE 12
#define ALTITUDE_SIZE 12
#define LONG_LAT_SIZE 1
#define DATA_SIZE 8
#define TIME_SIZE 6
#define SPEED_SIZE 8

extern uint8_t one_complete_nmea_sentence[NMEA_DATA_MAX_SIZE+1];
extern uint8_t bd2_uart_data_loop[BD2_UART_DATA_LOOP_SIZE+1];
extern uint8_t response_location[256];

typedef struct {
	uint8_t latitude[LATITUDE_SIZE+1];
	uint8_t lat_hemi[LONG_LAT_SIZE+1];
	uint8_t longitude[LONGITUDE_SIZE+1];
	uint8_t long_hemi[LONG_LAT_SIZE+1];
	uint8_t altitude[ALTITUDE_SIZE+1];
	uint16_t	flags;
	uint8_t date[DATA_SIZE+1];
	uint8_t time[TIME_SIZE+1];
	uint8_t   speed[SPEED_SIZE+1];
} GpsLocation;

typedef struct {
    const uint8_t*  p;
    const uint8_t*  end;
} Token;

#define  MAX_NMEA_TOKENS  24

typedef struct {
    uint16_t     count;
    Token   tokens[ MAX_NMEA_TOKENS ];
} NmeaTokenizer;

/** Flags to indicate which values are valid in a GpsLocation. */
typedef uint16_t GpsLocationFlags;
/* IMPORTANT: Note that the following values must match
 * constants in GpsLocationProvider.java. */
/** GpsLocation has valid latitude and longitude. */
#define GPS_LOCATION_HAS_LAT_LONG   0x0001
/** GpsLocation has valid altitude. */
#define GPS_LOCATION_HAS_ALTITUDE   0x0002
/** GpsLocation has valid speed. */
#define GPS_LOCATION_HAS_SPEED      0x0004
/** GpsLocation has valid bearing. */
#define GPS_LOCATION_HAS_BEARING    0x0008
/** GpsLocation has valid accuracy. */
#define GPS_LOCATION_HAS_ACCURACY   0x0010

#define GPS_LOCATION_HAS_DATATIME   0x0020

extern GpsLocation gpslocation;

extern uint16_t get_zdevice_location_response(uint8_t *buf);
extern uint8_t bd2_uart_loop_data_handle(uint8_t *data, uint16_t len);
extern void rn_receive_and_handle();

#endif
