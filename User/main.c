#include "../System/scheduler/scheduler.h"


int main(void)
{	
	NVIC_Configuration();
	delay_init();
	Adc_Init();
	
	taskScheduler_init();
	Lock_Cfg();
	Key_Cfg();
	
	USART_Config();
	USART2_Config();
	USART3_Config();
	
	TIM2_Int_Init();
	TIM3_Int_Init();
	TIM4_Int_Init();
	
	rx_queue_init();
	
	BC35_Init();
	
	printf("Welcome to use!\r\n");
	
	while (1){
		taskScheduler();
	}
}


