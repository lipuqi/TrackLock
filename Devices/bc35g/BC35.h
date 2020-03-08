#ifndef __BC35_H
#define __BC35_H	

#include "stm32f10x.h"
#include <string.h>
#include "../../System/delay/delay.h"
#include "../usart/bc35_usart3.h"


void BC35_Init(void);
void BC35_COAPdata(char *data);

#endif
