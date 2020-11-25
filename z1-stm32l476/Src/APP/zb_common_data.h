#ifndef ZB_COMMON_DATA_H
#define ZB_COMMON_DATA_H

#include <stdint.h>
#include "stm32l4xx_hal.h"

typedef struct {
    const uint8_t*  p;
    const uint8_t*  end;
} ZbToken;

#define  MAX_ZB_TOKENS  24

typedef struct {
    uint16_t     count;
    ZbToken   tokens[ MAX_ZB_TOKENS ];
} ZbTokenizer;

extern double str2float( const char*  p, const char*  end );
extern void int2str(int n, char *str);
extern int str2int( const char*  p, const char*  end );

extern uint16_t zb_tokenizer_init( ZbTokenizer*  t, const uint8_t*  p, const uint8_t*  end );
extern ZbToken zb_tokenizer_get( ZbTokenizer*  t, int  index );
#endif
