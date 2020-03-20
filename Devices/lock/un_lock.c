#include "un_lock.h"

static void NVIC_Cfg(void){
	
	NVIC_InitTypeDef NVIC_InitTStruct;
	
	NVIC_InitTStruct.NVIC_IRQChannel = LOCK_EXIT_IRQN;
	NVIC_InitTStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitTStruct.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitTStruct.NVIC_IRQChannelCmd = ENABLE;
	
	NVIC_Init(&NVIC_InitTStruct);
}

void Lock_Cfg(void){
	
	GPIO_InitTypeDef lock_gpio;
	EXTI_InitTypeDef lock_exti;
	
	NVIC_Cfg();
	
	RCC_APB2PeriphClockCmd( BELL_GPIO_CLK | LEDRGB_GPIO_CLK | LOCK_EXIT_GPIO_CLK | LOCK_OUT_GPIO_CLK, ENABLE);
	
	lock_gpio.GPIO_Pin = BELL_GPIO_PIN;
	lock_gpio.GPIO_Mode = GPIO_Mode_Out_PP;
	lock_gpio.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(BELL_GPIO_PORT, &lock_gpio);
	
	lock_gpio.GPIO_Pin = LEDR_GPIO_PIN | LEDG_GPIO_PIN | LEDB_GPIO_PIN;    
	lock_gpio.GPIO_Mode = GPIO_Mode_Out_PP;       
	lock_gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(LEDRGB_GPIO_PORT, &lock_gpio);
	
	lock_gpio.GPIO_Pin = LOCK_EXIT_GPIO_PIN;
	lock_gpio.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_Init(LOCK_EXIT_GPIO_PORT, &lock_gpio);
	
	lock_gpio.GPIO_Pin = LOCK_OUT_GPIO_PIN;    
	lock_gpio.GPIO_Mode = GPIO_Mode_Out_PP;       
	lock_gpio.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(LOCK_OUT_GPIO_PORT, &lock_gpio);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO , ENABLE);
	
	GPIO_EXTILineConfig(LOCK_EXIT_GPIO_PORTSOURCE, LOCK_EXIT_GPIO_PINSOURCE);
	lock_exti.EXTI_Line = LOCK_EXIT_LINE;
	lock_exti.EXTI_Mode = EXTI_Mode_Interrupt;
	lock_exti.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	lock_exti.EXTI_LineCmd = ENABLE;
	
	EXTI_Init(&lock_exti);
	
	GPIO_SetBits(LOCK_OUT_GPIO_PORT, LOCK_OUT_GPIO_PIN);
	
	BELL_OFF;
	GPIO_SetBits(LEDRGB_GPIO_PORT, LEDR_GPIO_PIN);
	GPIO_SetBits(LEDRGB_GPIO_PORT, LEDG_GPIO_PIN);
	GPIO_SetBits(LEDRGB_GPIO_PORT, LEDB_GPIO_PIN);
}

void enableToRising(void)
{
    EXTI->RTSR &= ~LOCK_EXIT_LINE;
    EXTI->FTSR &= ~LOCK_EXIT_LINE;
	
	  EXTI->RTSR |= LOCK_EXIT_LINE;
}

void enableToFalling(void)
{
    EXTI->RTSR &= ~LOCK_EXIT_LINE;
    EXTI->FTSR &= ~LOCK_EXIT_LINE;
	
	  EXTI->FTSR |= LOCK_EXIT_LINE;
}

void unLockControl(void)
{
	BELL_ON;
	LED_ctrl(BLUE,1);
	delay_ms(1000);
	BELL_OFF;
	LED_ctrl(BLUE,0);
}

void LED_ctrl(u8 colour, char state)
{
    switch(colour){
        case RED:
            if(state){
                GPIO_ResetBits(LEDRGB_GPIO_PORT, LEDR_GPIO_PIN);
            }
            else {
                GPIO_SetBits(LEDRGB_GPIO_PORT, LEDR_GPIO_PIN);
            }
            break;
        case GREEN:
            if(state){
                GPIO_ResetBits(LEDRGB_GPIO_PORT, LEDG_GPIO_PIN);
            }
            else{
		        GPIO_SetBits(LEDRGB_GPIO_PORT, LEDG_GPIO_PIN);
            }
            break;
        case BLUE:
            if(state){
                GPIO_ResetBits(LEDRGB_GPIO_PORT, LEDB_GPIO_PIN);
            }
            else {
                GPIO_SetBits(LEDRGB_GPIO_PORT, LEDB_GPIO_PIN);
            }
            break;
        case YELLOW:
            if(state){
                GPIO_ResetBits(LEDRGB_GPIO_PORT, LEDR_GPIO_PIN);
                GPIO_ResetBits(LEDRGB_GPIO_PORT, LEDG_GPIO_PIN);
            }
            else{
                GPIO_SetBits(LEDRGB_GPIO_PORT, LEDR_GPIO_PIN);
                GPIO_SetBits(LEDRGB_GPIO_PORT, LEDG_GPIO_PIN);
            }
            break;
        case PURPLE:
            if(state){
                GPIO_ResetBits(LEDRGB_GPIO_PORT, LEDR_GPIO_PIN);
                GPIO_ResetBits(LEDRGB_GPIO_PORT, LEDB_GPIO_PIN);
                }
            else{
                GPIO_SetBits(LEDRGB_GPIO_PORT, LEDR_GPIO_PIN);
                GPIO_SetBits(LEDRGB_GPIO_PORT, LEDB_GPIO_PIN);
            }
            break;
        case NAVY:
            if(state){
                GPIO_ResetBits(LEDRGB_GPIO_PORT, LEDG_GPIO_PIN);
                GPIO_ResetBits(LEDRGB_GPIO_PORT, LEDB_GPIO_PIN);
            }
            else{
                GPIO_SetBits(LEDRGB_GPIO_PORT, LEDG_GPIO_PIN);
                GPIO_SetBits(LEDRGB_GPIO_PORT, LEDB_GPIO_PIN);
            }
            break;
        case WHITE:
            if(state){
                GPIO_ResetBits(LEDRGB_GPIO_PORT, LEDR_GPIO_PIN);
                GPIO_ResetBits(LEDRGB_GPIO_PORT, LEDG_GPIO_PIN);
                GPIO_ResetBits(LEDRGB_GPIO_PORT, LEDB_GPIO_PIN);
            }
            else{
                GPIO_SetBits(LEDRGB_GPIO_PORT, LEDR_GPIO_PIN);
                GPIO_SetBits(LEDRGB_GPIO_PORT, LEDG_GPIO_PIN);
                GPIO_SetBits(LEDRGB_GPIO_PORT, LEDB_GPIO_PIN);
            }
            break;
        default:
            break;
    }
}
