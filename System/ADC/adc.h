#ifndef __ADC_H
#define __ADC_H	
#include "../sys/sys.h"
#include "../delay/delay.h"


void Adc_Init(void);
u16  Get_Adc(void); 
u16 Get_Adc_Average(u8 times); 
char Get_battery(void);

#endif 
