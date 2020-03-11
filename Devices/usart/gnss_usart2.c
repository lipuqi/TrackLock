#include "gnss_usart2.h"	

u16 point1 = 0;

extern _SaveData Save_Data;
 	
char USART_RX_BUF[USART_REC_LEN];

void CLR_Buf(void)                           // ���ڻ�������
{
	memset(USART_RX_BUF, 0, USART_REC_LEN);      //���
  point1 = 0;                    
}

void clrStruct()
{
	Save_Data.isGetData = false;
	Save_Data.isParseData = false;
	Save_Data.isUsefull = false;
	Save_Data.isNull = true;
	memset(Save_Data.GPS_Buffer, 0, GPS_Buffer_Length);      //���
	memset(Save_Data.UTCTime, 0, UTCTime_Length);
	memset(Save_Data.latitude, 0, latitude_Length);
	memset(Save_Data.N_S, 0, N_S_Length);
	memset(Save_Data.longitude, 0, longitude_Length);
	memset(Save_Data.E_W, 0, E_W_Length);
	
}

void errorLog(int num)
{
	
}

static void GNSS_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  
  /* ����USARTΪ�ж�Դ */
  NVIC_InitStructure.NVIC_IRQChannel = GNSS_USART2_IRQ;
  /* �������ȼ�*/
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  /* �����ȼ� */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  /* ʹ���ж� */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  /* ��ʼ������NVIC */
  NVIC_Init(&NVIC_InitStructure);
}


void USART2_Config(void)
{
	
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	// �򿪴���GPIO��ʱ��
	GNSS_USART2_GPIO_APBxClkCmd(GNSS_USART2_GPIO_CLK, ENABLE);
	
	
	// ��USART Tx��GPIO����Ϊ���츴��ģʽ
	GPIO_InitStructure.GPIO_Pin = GNSS_USART2_TX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GNSS_USART2_TX_GPIO_PORT, &GPIO_InitStructure);

  // ��USART Rx��GPIO����Ϊ��������ģʽ
	GPIO_InitStructure.GPIO_Pin = GNSS_USART2_RX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GNSS_USART2_RX_GPIO_PORT, &GPIO_InitStructure);
	
	
	// �򿪴��������ʱ��
	GNSS_USART2_APBxClkCmd(GNSS_USART2_CLK, ENABLE);
	
	
	// ���ô��ڵĹ�������
	// ���ò�����
	USART_InitStructure.USART_BaudRate = GNSS_USART2_BAUDRATE;
	// ���� �������ֳ�
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	// ����ֹͣλ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	// ����У��λ
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	// ����Ӳ��������
	USART_InitStructure.USART_HardwareFlowControl = 
	USART_HardwareFlowControl_None;
	// ���ù���ģʽ���շ�һ��
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	// ��ɴ��ڵĳ�ʼ������
	USART_Init(GNSS_USART2x, &USART_InitStructure);
	
	// �����ж����ȼ�����
	GNSS_Configuration();
	
}

/*****************  ����һ���ֽ� **********************/
void Usart2_SendByte( USART_TypeDef * pUSARTx, uint8_t ch)
{
	/* ����һ���ֽ����ݵ�USART */
	USART_SendData(pUSARTx,ch);
		
	/* �ȴ��������ݼĴ���Ϊ�� */
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}

/*****************  �����ַ��� **********************/
void Usart2_SendString(char *str)
{
	unsigned int k=0;
  do 
  {
      Usart2_SendByte( GNSS_USART2x, *(str + k) );
      k++;
  } while(*(str + k)!='\0');
  
  /* �ȴ�������� */
  while(USART_GetFlagStatus(GNSS_USART2x,USART_FLAG_TC)==RESET)
  {}
}

void GNSS_USART2_IRQHandler(void)                	//����2�жϷ������
{
	u8 Res;
	
	if(USART_GetITStatus(GNSS_USART2x, USART_IT_RXNE) != RESET) 
	{
		Res =USART_ReceiveData(GNSS_USART2x);//(USART1->DR);	//��ȡ���յ�������
	if(Res == '$')
	{
		point1 = 0;	
	}
	  USART_RX_BUF[point1++] = Res;
	if(USART_RX_BUF[0] == '$' && USART_RX_BUF[4] == 'M' && USART_RX_BUF[5] == 'C')			//ȷ���Ƿ��յ�"GPRMC/GNRMC"��һ֡����
	{
		if(Res == '\n' && Save_Data.isNull)									   
		{
			Usart2_SendString(GNSS_DISABLE);
			memset(Save_Data.GPS_Buffer, 0, GPS_Buffer_Length);      //���
			memcpy(Save_Data.GPS_Buffer, USART_RX_BUF, point1); 	//��������
			Save_Data.isGetData = true;
			point1 = 0;
			memset(USART_RX_BUF, 0, USART_REC_LEN);      //���			
		}	
				
	}
	
	if(point1 >= USART_REC_LEN)
	{
		point1 = USART_REC_LEN;
	}	
		 		 
   } 
}


void parseGpsBuffer()
{
	char *subString;
	char *subStringNext;
	char i = 0;
	if (Save_Data.isGetData)
	{
		Save_Data.isGetData = false;

		for (i = 0 ; i <= 6 ; i++)
		{
			if (i == 0)
			{
				if ((subString = strstr(Save_Data.GPS_Buffer, ",")) == NULL)
					errorLog(1);	//��������
			}
			else
			{
				subString++;
				if ((subStringNext = strstr(subString, ",")) != NULL)
				{
					char usefullBuffer[2]; 
					switch(i)
					{
						case 1:memcpy(Save_Data.UTCTime, subString, subStringNext - subString);break;	//��ȡUTCʱ��
						case 2:memcpy(usefullBuffer, subString, subStringNext - subString);break;	//��ȡUTCʱ��
						case 3:memcpy(Save_Data.latitude, subString, subStringNext - subString);break;	//��ȡγ����Ϣ
						case 4:memcpy(Save_Data.N_S, subString, subStringNext - subString);break;	//��ȡN/S
						case 5:memcpy(Save_Data.longitude, subString, subStringNext - subString);break;	//��ȡ������Ϣ
						case 6:memcpy(Save_Data.E_W, subString, subStringNext - subString);break;	//��ȡE/W

						default:break;
					}

					subString = subStringNext;
					Save_Data.isParseData = true;
					if(usefullBuffer[0] == 'A')
						Save_Data.isUsefull = true;
					else if(usefullBuffer[0] == 'V')
						Save_Data.isUsefull = false;

				}
				else
				{
					errorLog(2);	//��������
				}
			}


		}
	}
}

void getGpsBuffer(int n)
{
	int index = n;
		// ʹ�ܴ��ڽ����ж�
	USART_ITConfig(GNSS_USART2x, USART_IT_RXNE, ENABLE);	
	
	// ʹ�ܴ���
	USART_Cmd(GNSS_USART2x, ENABLE);
	
	Usart2_SendString(GNSS_ENABLE);
	
	CLR_Buf();//��ջ���
	clrStruct();
		
	while (index > 0)
	{
		parseGpsBuffer();
		if(Save_Data.isParseData)
		{
			Save_Data.isParseData = false;
				if(Save_Data.isUsefull)
				{
					break;
				} else {
					Usart2_SendString(GNSS_ENABLE);
				}	
		}	
		delay_ms(1000);
		index--;	
	}	
	
	Usart2_SendString(GNSS_DISABLE);			
	// ʹ�ܴ��ڽ����ж�
	USART_ITConfig(GNSS_USART2x, USART_IT_RXNE, DISABLE);	
	CLR_Buf();//��ջ���
}



