#include "key1.h"

static void NVIC_Cfg(void){
	
	NVIC_InitTypeDef NVIC_InitTStruct;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_InitTStruct.NVIC_IRQChannel = KEY1_EXIT_IRQN;
	NVIC_InitTStruct.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitTStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitTStruct.NVIC_IRQChannelCmd = ENABLE;
	
	NVIC_Init(&NVIC_InitTStruct);
	
	NVIC_InitTStruct.NVIC_IRQChannel = KEY2_EXIT_IRQN;
	NVIC_InitTStruct.NVIC_IRQChannelSubPriority = 1;
	
	NVIC_Init(&NVIC_InitTStruct);

}


void Key_Cfg(void){
	
	GPIO_InitTypeDef key_gpio;
	EXTI_InitTypeDef key_exti;
	
	NVIC_Cfg();
	
	RCC_APB2PeriphClockCmd(KEY1_EXIT_GPIO_CLK | KEY2_EXIT_GPIO_CLK , ENABLE);
	
	key_gpio.GPIO_Pin = KEY1_EXIT_GPIO_PIN;
	key_gpio.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(KEY1_EXIT_GPIO_PORT, &key_gpio);
	
	key_gpio.GPIO_Pin = KEY2_EXIT_GPIO_PIN;
	GPIO_Init(KEY2_EXIT_GPIO_PORT, &key_gpio);
	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO , ENABLE);
	
	GPIO_EXTILineConfig(KEY1_EXIT_GPIO_PORTSOURCE, KEY1_EXIT_GPIO_PINSOURCE);
	key_exti.EXTI_Line = KEY1_EXIT_LINE;
	key_exti.EXTI_Mode = EXTI_Mode_Interrupt;
	key_exti.EXTI_Trigger = EXTI_Trigger_Rising;
	key_exti.EXTI_LineCmd = ENABLE;
	
	EXTI_Init(&key_exti);
	
	GPIO_EXTILineConfig(KEY2_EXIT_GPIO_PORTSOURCE, KEY2_EXIT_GPIO_PINSOURCE);
	key_exti.EXTI_Line = KEY2_EXIT_LINE;
	
	EXTI_Init(&key_exti);
	
}

