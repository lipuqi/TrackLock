#ifndef __BC35_H
#define __BC35_H	

#include "stm32f10x.h"
#include "../../System/delay/delay.h"
#include "../../System/IWDG/iwdg.h"
#include "../usart/bc35_usart3.h"
#include "../flash/flash_memory.h"

int BC35_Init(void);
void BC35_COAPdata(char *data);
void BC35_Restart(void);
void BC35_Shutdown(void);
int isCommunication(void);

#endif
