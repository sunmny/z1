#ifndef MY_COMMON_H
#define MY_COMMON_H

int toupper(int c);

// BCD?ASCII
int Asc2Bcd(unsigned char *input, unsigned int inputLen, unsigned char *output);
// ASCII?BCD
int Bcd2Asc(unsigned char *input, unsigned int inputLen, unsigned char *output);

#endif
