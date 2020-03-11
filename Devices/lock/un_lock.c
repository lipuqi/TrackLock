#include "un_lock.h"

char lockStatus = 0;

void Lock_Cfg(void){
	
	GPIO_InitTypeDef lock_gpio;
	
	RCC_APB2PeriphClockCmd( BELL_GPIO_CLK | LED_GPIO_CLK , ENABLE);
	
	lock_gpio.GPIO_Pin = BELL_GPIO_PIN;
	lock_gpio.GPIO_Mode = GPIO_Mode_Out_PP;
	lock_gpio.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(BELL_GPIO_PORT, &lock_gpio);
	
	lock_gpio.GPIO_Pin = LED_GPIO_PIN;
	GPIO_Init(BELL_GPIO_PORT, &lock_gpio);
	
	BELL_OFF;
	LED_OFF;
}

void toggleLockStatus(void)
{
	BELL_TOGGLE;
	if(lockStatus){
		lockStatus = 0;
	} else {
		lockStatus = 1;
	}
}

char getLockStatus(void)
{
	return lockStatus;
}
