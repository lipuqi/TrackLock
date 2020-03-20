#ifndef __COMMON_H
#define __COMMON_H
#include "stm32f10x.h"
#include <stdio.h>

#include "../System/delay/delay.h"
#include "../System/sys/sys.h"
#include "../System/TIM/TiMbase.h"
#include "../System/ADC/adc.h"
#include "../System/IWDG/iwdg.h"
#include "../System/utils/ruleSelector.h"

#include "./lock/un_lock.h"
#include "./key/key1.h"
#include "./usart/bsp_usart.h"
#include "./usart/rx_data_queue.h"
#include "./usart/gnss_usart2.h"
#include "./usart/bc35_queue.h"
#include "./bc35g/BC35.h"
#include "./flash/flash_memory.h"

#endif
