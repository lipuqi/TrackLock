#ifndef __UTILS_H
#define __UTILS_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
 #include "../ADC/adc.h"

#define NMEA_CONVSTR_BUF    (256)

#define PI                      3.1415926
#define EARTH_RADIUS            6378.137        //µØÇò½üËÆ°ë¾¶

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
int util_atoi(const char *str, int str_sz, int radix);
double util_atof(const char *str, int str_sz);
double get_distance(double lat1, double lng1, double lat2, double lng2);
double longitudeToOnenetFormat(char* lon_str);
double latitudeToOnenetFormat(char* lat_str);

#endif
