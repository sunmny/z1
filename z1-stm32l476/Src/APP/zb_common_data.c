#include "zb_common_data.h"
#include "string.h"
#include <stdint.h>

double str2float( const char*  p, const char*  end )
{
    int   result = 0;
    int   len    = end - p;
    char  temp[16];

    if (len >= (int)sizeof(temp))
        return 0.;

    memcpy( temp, p, len );
    temp[len] = 0;
    return strtod( temp, NULL );/*strtod maybe crash!!!*/
}

void int2str(int n, char *str)
{
    char buf[10] = "";
    int i = 0;
    int len = 0;
    int temp = n < 0 ? -n: n;

    if (str == NULL)
    {
        return;
    }

    while(temp)
    {
        buf[i++] = (temp % 10) + '0';
        temp = temp / 10;
    }

    len = n < 0 ? ++i: i;
    str[i] = 0;
    while(1)
    {
        i--;
        if (buf[len-i-1] ==0)
        {
            break;
        }
        str[i] = buf[len-i-1];
    }
    if (i == 0 )
    {
        str[i] = '-'; 
    }
}


int str2int( const char*  p, const char*  end )
{
    int   result = 0;
    int   len    = end - p;
    int   sign = 1;

    if (*p == '-')
    {
        sign = -1;
        p++;
        len = end - p;
    }

    for ( ; len > 0; len--, p++ )
    {
        int  c;

        if (p >= end)
            goto Fail;

        c = *p - '0';
        if ((unsigned)c >= 10)
            goto Fail;

        result = result*10 + c;
    }
    return  sign*result;

Fail:
    return -1;
}

uint16_t zb_tokenizer_init( ZbTokenizer*  t, const uint8_t*  p, const uint8_t*  end )
{
    uint16_t    count = 0;
    uint8_t*  q;

    // remove trailing newline
    if (end > p && (*(end-1) == '\n')) {
        end -= 1;
        if (end > p && (*(end-1) == '\r'))
            end -= 1;
	if (end > p && (*(end-1) == '#'))
            end -= 1;
	if (end > p && (*(end-1) == '#'))
            end -= 1;
    }

    while (p < end) {
        const uint8_t*  q = p;

        q = memchr(p, ',', end-p);
        if (q == NULL)
            q = end;

        if (q >= p) {
            if (count < MAX_ZB_TOKENS) {
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

ZbToken zb_tokenizer_get( ZbTokenizer*  t, int  index )
{
    ZbToken  tok;
    static const uint8_t*  dummy = "";

    if (index < 0 || index >= t->count) {
        tok.p = tok.end = dummy;
    } else
        tok = t->tokens[index];

    return tok;
}

