#include "TiMbase.h"

void NVIC2_config(void)
{ 
	 NVIC_InitTypeDef nvic;
	 nvic.NVIC_IRQChannel=TIM2_IRQn;
	 nvic.NVIC_IRQChannelCmd=ENABLE;
	 nvic.NVIC_IRQChannelPreemptionPriority=2;
	 nvic.NVIC_IRQChannelSubPriority=2;
	 NVIC_Init(&nvic);
}

void NVIC3_config(void)
{ 
	 NVIC_InitTypeDef nvic;
	 nvic.NVIC_IRQChannel=TIM3_IRQn;
	 nvic.NVIC_IRQChannelCmd=ENABLE;
	 nvic.NVIC_IRQChannelPreemptionPriority=2;
	 nvic.NVIC_IRQChannelSubPriority=3;
	 NVIC_Init(&nvic);
}

void NVIC4_config(void)
{ 
	 NVIC_InitTypeDef nvic;
	 nvic.NVIC_IRQChannel=TIM4_IRQn;
	 nvic.NVIC_IRQChannelCmd=ENABLE;
	 nvic.NVIC_IRQChannelPreemptionPriority=0;
	 nvic.NVIC_IRQChannelSubPriority=3;
	 NVIC_Init(&nvic);
}
 

void TIM2_Int_Init(void)
{
    TIM_TimeBaseInitTypeDef    TIM_TimeBaseStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    
    TIM_DeInit(TIM2);
    
    TIM_TimeBaseStructure.TIM_Period = 60000 - 1;
    TIM_TimeBaseStructure.TIM_Prescaler = 36000 - 1;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	
		NVIC2_config();//优先级函数调用
    
    TIM_ClearFlag(TIM2, TIM_FLAG_Update);
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
    
    TIM_Cmd(TIM2, ENABLE);
}

void TIM3_Int_Init(void)
{
    TIM_TimeBaseInitTypeDef    TIM_TimeBaseStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    
    TIM_DeInit(TIM3);
    
    TIM_TimeBaseStructure.TIM_Period = 60000 - 1;
    TIM_TimeBaseStructure.TIM_Prescaler = 36000 - 1;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	
		NVIC3_config();//优先级函数调用
    
    TIM_ClearFlag(TIM3, TIM_FLAG_Update);
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
    
    TIM_Cmd(TIM3, ENABLE);
}

void TIM4_Int_Init(void)
{
    TIM_TimeBaseInitTypeDef    TIM_TimeBaseStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
    
    TIM_DeInit(TIM4);
    
    TIM_TimeBaseStructure.TIM_Period = 3000 - 1;
    TIM_TimeBaseStructure.TIM_Prescaler = 36000 - 1;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	
		NVIC4_config();//优先级函数调用
    
    TIM_ClearFlag(TIM4, TIM_FLAG_Update);
    TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
    
}
