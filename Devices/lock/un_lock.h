#ifndef __UN_LOCK_H
#define __UN_LOCK_H

#include "stm32f10x.h"
#include "../../System/delay/delay.h"

#define LEDRGB_GPIO_PORT    	GPIOB		
#define LEDRGB_GPIO_CLK 	  	RCC_APB2Periph_GPIOB	
#define LEDR_GPIO_PIN			    GPIO_Pin_12	
#define LEDG_GPIO_PIN			    GPIO_Pin_13	
#define LEDB_GPIO_PIN			    GPIO_Pin_14	

#define RED     1
#define GREEN   2
#define BLUE    3
#define YELLOW  4
#define PURPLE  5
#define NAVY    6
#define WHITE   7

#define BELL_GPIO_PORT    	GPIOB		              /* GPIO�˿� */
#define BELL_GPIO_CLK 	    RCC_APB2Periph_GPIOB		/* GPIO�˿�ʱ�� */
#define BELL_GPIO_PIN			  GPIO_Pin_9	
#define BELL_ON      			  GPIO_SetBits(BELL_GPIO_PORT, BELL_GPIO_PIN)
#define BELL_OFF     			  GPIO_ResetBits(BELL_GPIO_PORT, BELL_GPIO_PIN)
#define BELL_TOGGLE  			  BELL_GPIO_PORT -> ODR ^= BELL_GPIO_PIN

#define LOCK_EXIT_GPIO_CLK			 		RCC_APB2Periph_GPIOB
#define LOCK_EXIT_GPIO_PORT			 		GPIOB
#define LOCK_EXIT_GPIO_PIN			 		GPIO_Pin_3
#define LOCK_EXIT_GPIO_PORTSOURCE	  GPIO_PortSourceGPIOB
#define LOCK_EXIT_GPIO_PINSOURCE	  GPIO_PinSource3
#define LOCK_EXIT_LINE						  EXTI_Line3
#define LOCK_EXIT_IRQN							EXTI3_IRQn
#define LOCK_EXIT_IRQHANDLER				EXTI3_IRQHandler

#define LOCK_STA GPIO_ReadInputDataBit(LOCK_EXIT_GPIO_PORT, LOCK_EXIT_GPIO_PIN)

#define LOCK_OUT_GPIO_CLK			 		RCC_APB2Periph_GPIOB
#define LOCK_OUT_GPIO_PORT			 	GPIOB
#define LOCK_OUT_GPIO_PIN			 		GPIO_Pin_4

void Lock_Cfg(void);
void unLockControl(void);
void initLed(void);
void malfunctionLed(void);
void LED_ctrl(u8 colour, char state);
void enableToRising(void);
void enableToFalling(void);

#endif
