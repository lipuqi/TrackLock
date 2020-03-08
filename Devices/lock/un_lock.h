#ifndef __UN_LOCK_H
#define __UN_LOCK_H

#include "stm32f10x.h"

#define LED_GPIO_PORT    GPIOB		              /* GPIO端口 */
#define LED_GPIO_CLK 	  RCC_APB2Periph_GPIOB		/* GPIO端口时钟 */
#define LED_GPIO_PIN			GPIO_Pin_8	
#define LED_ON      			GPIO_SetBits(LED_GPIO_PORT, LED_GPIO_PIN)
#define LED_OFF     			GPIO_ResetBits(LED_GPIO_PORT, LED_GPIO_PIN)
#define LED_TOGGLE  			LED_GPIO_PORT -> ODR ^= LED_GPIO_PIN

#define BELL_GPIO_PORT    	GPIOB		              /* GPIO端口 */
#define BELL_GPIO_CLK 	    RCC_APB2Periph_GPIOB		/* GPIO端口时钟 */
#define BELL_GPIO_PIN			GPIO_Pin_9	
#define BELL_ON      			GPIO_SetBits(BELL_GPIO_PORT, BELL_GPIO_PIN)
#define BELL_OFF     			GPIO_ResetBits(BELL_GPIO_PORT, BELL_GPIO_PIN)
#define BELL_TOGGLE  			BELL_GPIO_PORT -> ODR ^= BELL_GPIO_PIN

void Lock_Cfg(void);

#endif
