#include "../Devices/common.h"


int main(void)
{	
	NVIC_Configuration();
	delay_init();
	Lock_Cfg();
	USART_Config();
	USART2_Config();
	USART3_Config();
	rx_queue_init();
	taskScheduler_init();
	TIM2_Int_Init();
	
	BC35_Init();
	printf("Welcome to use!\r\n");
	
	while (1){
		taskScheduler();
	}
}


