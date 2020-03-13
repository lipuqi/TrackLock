/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"

/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}

char wakeLock;
void KEY1_EXIT_IRQHANDLER(void)
{
	 if(EXTI_GetITStatus(KEY1_EXIT_LINE) != RESET){
		 if(!wakeLock){
		 	reportHeartbeat();
			TIM_Cmd(TIM4, ENABLE); 
			wakeLock = 1;
		 }
	 }
	 EXTI_ClearITPendingBit(KEY1_EXIT_LINE);
}

char lockStatus;
char isUnLock;
void LOCK_EXIT_IRQHANDLER(void)
{
	 if(EXTI_GetITStatus(LOCK_EXIT_LINE) != RESET){
			
			uint8_t lock = LOCK_STA;
			if(lock != lockStatus){
				lockStatus = lock;
				isUnLock = 1;
			}
	 }
	 EXTI_ClearITPendingBit(LOCK_EXIT_LINE);
}

// �����жϷ�����
// �ѽ��յ�������д�뻺��������main��������ѯ�������������
void DEBUG_USART_IRQHandler(void)
{	
	uint8_t ucCh;
	QUEUE_DATA_TYPE *data_p; 
	
	if(USART_GetITStatus(DEBUG_USARTx,USART_IT_RXNE)!=RESET)
	{	
		ucCh  = USART_ReceiveData( DEBUG_USARTx );
		
						/*��ȡд������ָ�룬׼��д��������*/
		data_p = cbWrite(&rx_queue); 
		
		if (data_p != NULL)	//���������δ������ʼ����
		{		

			//��������д�����ݣ���ʹ�ô��ڽ��ա�dmaд��ȷ�ʽ
			*(data_p->head + data_p->len) = ucCh;
				
			if( ++data_p->len >= QUEUE_NODE_DATA_LEN)
			{
				cbWriteFinish(&rx_queue);
			}
		}else return;	
	}
	
	if ( USART_GetITStatus( DEBUG_USARTx, USART_IT_IDLE ) == SET )                                         //����֡�������
	{
			/*д�뻺�������*/
			cbWriteFinish(&rx_queue);
		ucCh = USART_ReceiveData( DEBUG_USARTx );                                                              //�������������жϱ�־λ(�ȶ�USART_SR��Ȼ���USART_DR)

	}
}

char isTim2;
uint32_t time2 = 0;
void TIM2_IRQHandler(void)
{
    
    if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    {   
				if(time2 > getPositionInterval()){
					isTim2 = 1;
					time2 = 0;
				} else {
					time2++;
				}
				
    }
    TIM_ClearITPendingBit(TIM2, TIM_FLAG_Update);
}

char isTim3;
uint32_t time3 = 0;
void TIM3_IRQHandler(void)
{
    
    if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
    {   
				if(time3 > getStateInterval()){
					isTim3 = 1;
					time3 = 0;
				} else {
					time3++;
				}
				
    }
    TIM_ClearITPendingBit(TIM3, TIM_FLAG_Update);
}

uint32_t time4 = 0;
void TIM4_IRQHandler(void)
{
    
    if(TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
    {   
			if(wakeLock){
				if(time4 > 20){
					wakeLock = 0;
				} else {
					time4++;
				}
				showBattery();	
			} else {
				time4 = 0;
				TIM_Cmd(TIM4, DISABLE); 
			}
    }
    TIM_ClearITPendingBit(TIM4, TIM_FLAG_Update);
}




/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
