#ifndef __GNSS_USART2_H
#define __GNSS_USART2_H

#include "stm32f10x.h"
#include "../../System/delay/delay.h"
#include <string.h>
#include <stdio.h>

#define USART_REC_LEN  		200  	//�����������ֽ��� 200
#define EN_USART1_RX 			1		  //ʹ�ܣ�1��/��ֹ��0������1����
	  	
extern char  USART_RX_BUF[USART_REC_LEN];   //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16   USART_RX_STA;         		      //����״̬���	



#define false 0
#define true 1

//�������鳤��
#define GPS_Buffer_Length 80
#define UTCTime_Length    11
#define latitude_Length   11
#define N_S_Length        2
#define longitude_Length  12
#define E_W_Length        2 


//����2-USART2
#define  GNSS_USART2x                   USART2
#define  GNSS_USART2_CLK                RCC_APB1Periph_USART2
#define  GNSS_USART2_APBxClkCmd         RCC_APB1PeriphClockCmd
#define  GNSS_USART2_BAUDRATE           9600

// USART GPIO ���ź궨��
#define  GNSS_USART2_GPIO_CLK           (RCC_APB2Periph_GPIOA)
#define  GNSS_USART2_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd
    
#define  GNSS_USART2_TX_GPIO_PORT       GPIOA   
#define  GNSS_USART2_TX_GPIO_PIN        GPIO_Pin_2
#define  GNSS_USART2_RX_GPIO_PORT       GPIOA
#define  GNSS_USART2_RX_GPIO_PIN        GPIO_Pin_3

#define  GNSS_USART2_IRQ                USART2_IRQn
#define  GNSS_USART2_IRQHandler         USART2_IRQHandler

#define  GNSS_ENABLE                    "$CCRMO,RMC,2,1*23\r\n"
#define  GNSS_DISABLE                   "$CCRMO,RMC,3,1*22\r\n"

typedef struct SaveData 
{
	char GPS_Buffer[GPS_Buffer_Length];
	char isGetData;		//�Ƿ��ȡ��GPS����
	char isParseData;	//�Ƿ�������
	char UTCTime[UTCTime_Length];		//UTCʱ��
	char latitude[latitude_Length];		//γ��
	char N_S[N_S_Length];		//N/S
	char longitude[longitude_Length];		//����
	char E_W[E_W_Length];		//E/W
	char isUsefull;		//��λ��Ϣ�Ƿ���Ч
} _SaveData;




void USART2_Config(void);
extern u16 point1;
extern _SaveData Save_Data;

void CLR_Buf(void);
u8 Hand(char *a);
void clrStruct(void);

void parseGpsBuffer(void);
_SaveData* getGpsBuffer(int n);

#endif
