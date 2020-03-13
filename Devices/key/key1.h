#ifndef __KEY1_H
#define __KEY1_H

#include "stm32f10x.h"

#define KEY1_EXIT_GPIO_CLK			 		RCC_APB2Periph_GPIOA
#define KEY1_EXIT_GPIO_PORT			 		GPIOA
#define KEY1_EXIT_GPIO_PIN			 		GPIO_Pin_0
#define KEY1_EXIT_GPIO_PORTSOURCE	  GPIO_PortSourceGPIOA
#define KEY1_EXIT_GPIO_PINSOURCE	  GPIO_PinSource0
#define KEY1_EXIT_LINE						  EXTI_Line0
#define KEY1_EXIT_IRQN							EXTI0_IRQn
#define KEY1_EXIT_IRQHANDLER				EXTI0_IRQHandler

#define KEY1_STA GPIO_ReadInputDataBit(KEY1_EXIT_GPIO_PORT, KEY1_EXIT_GPIO_PIN)

#define KEY_DN 1

void Key_Cfg(void);

#endif
