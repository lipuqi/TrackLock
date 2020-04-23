#include "../System/scheduler/scheduler.h"


int main(void)
{	
	NVIC_Configuration();
	delay_init();
	USART_Config();
	rx_queue_init();
	
	Lock_Cfg();
	initLed();
	Adc_Init();
	Flash_Init();
	
	taskScheduler_init();
	
	USART2_Config();
	USART3_Config();
	
	TIM2_Int_Init();
	TIM3_Int_Init();
	TIM4_Int_Init();
	
	Key_Cfg();
	
	IWDG_Init(4,3125); //5s

	while(!BC35_Init()){
		malfunctionLed();
		BC35_Restart();
	}

	printf("Welcome to use!\r\n");
	
	while (1){
		taskScheduler();
	}
}


