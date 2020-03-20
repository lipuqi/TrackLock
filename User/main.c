#include "../System/scheduler/scheduler.h"


int main(void)
{	
	NVIC_Configuration();
	delay_init();
	USART_Config();
	rx_queue_init();
	
	printf("main -> start");
	
	Adc_Init();
	
	taskScheduler_init();
	
	USART2_Config();
	USART3_Config();
	
	TIM2_Int_Init();
	TIM3_Int_Init();
	TIM4_Int_Init();
	
	BC35_Init();
	
	Lock_Cfg();
	Key_Cfg();
	
	IWDG_Init(4,3125); //5s
	printf("Welcome to use!\r\n");
	
	while (1){
		taskScheduler();
	}
}


