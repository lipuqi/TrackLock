#ifndef __UTILS_H
#define __UTILS_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define u32 unsigned long int
#define u16 unsigned int
#define u8 unsigned char


void clean_string(char* str);
u8 chrtodec(char chr);
int cal16match(u8 n);
int charhex_to_dec(char* chr);
void charToHex(char* src, char* res);
int getStrlen(char* str);
void appendchar(void* source, void* des, int source_len, int des_len);
void rand4UUID(char des[5]);

#endif
