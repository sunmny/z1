#include "my_common.h"
#include <stdint.h>


int toupper(int c)
{
  if((c>='a')&&(c<='z'))
	{
	  return c+('A'-'a');
	}
   return c;
}


// BCDתASCII
int Asc2Bcd(unsigned char *input, unsigned int inputLen, unsigned char *output)
{
    unsigned char temp;
    unsigned int i;
    //if (input == null || output == NULL) {
    //   return -1;
    //}
   for (i = 0; i < inputLen; i += 2) {
        temp = input[i];
        if (temp > '9') {
			temp = (unsigned char)toupper((unsigned char)temp) - 'A' + 0x0A;
		} else {
			temp &= 0x0F;
		}
		output[i / 2] = temp << 4;
		temp = input[i+1];
		if (temp > '9') {
			temp = (unsigned char)toupper((unsigned char)temp) - 'A' + 0x0A;
		} else {
			temp &= 0x0F;
		}
		output[i/2] |= temp;
	}
	return 0;
}
// ASCIIתBCD
int Bcd2Asc(unsigned char *input, unsigned int inputLen, unsigned char *output)
{
	const unsigned char ascii[] = "0123456789ABCDEF";
	unsigned int i;
	//if (input == NULL || output == NULL) {
	//	return 1;
	//}
	for (i = 0; i < inputLen; i++)
	{
		output[2 * i] = ascii[(input[i] >> 4)];
		output[2 * i + 1] = ascii[(input[i] & 0x0F)];
	}
	return 0;
}
