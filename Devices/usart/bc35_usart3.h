#ifndef __BC35_USART3_H
#define __BC35_USART3_H

#include "stm32f10x.h"
#include "bc35_queue.h"
#include "../../System/utils/utils.h"
	  	

// 串口3-USART3
#define  BC35_USART3x                   USART3
#define  BC35_USART3_CLK                RCC_APB1Periph_USART3
#define  BC35_USART3_APBxClkCmd         RCC_APB1PeriphClockCmd
#define  BC35_USART3_BAUDRATE           9600

// USART GPIO 引脚宏定义
#define  BC35_USART3_GPIO_CLK           (RCC_APB2Periph_GPIOB)
#define  BC35_USART3_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd
    
#define  BC35_USART3_TX_GPIO_PORT       GPIOB   
#define  BC35_USART3_TX_GPIO_PIN        GPIO_Pin_10
#define  BC35_USART3_RX_GPIO_PORT       GPIOB
#define  BC35_USART3_RX_GPIO_PIN        GPIO_Pin_11

#define  BC35_USART3_IRQ                USART3_IRQn
#define  BC35_USART3_IRQHandler         USART3_IRQHandler


void USART3_Config(void);
void Usart3_SendString(char *str);

#endif
