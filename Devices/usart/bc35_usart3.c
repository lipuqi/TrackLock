#include "bc35_usart3.h"	

char RxCoun,RxBuf[100]; // ���ջ���
char RxCounter,RxBuffer[100];  // ������Ϣ����
SqQueue struc_queue; // ������ջ���

void Clear_RxBuf(void)//��ջ���
{
		u8 i;
		for(i=0;i<100;i++)
		RxBuf[i]=0;//����
		RxCoun=0;
	
}

void appendchar(void* source, void* des, int source_len, int des_len)
{
	char* pSource = (char*)source;
	char* pDes = (char*)des;
	while (source_len != 0)
	{
		*((pDes++) + des_len) = *pSource++;
		source_len--;
		if (source_len == 0) {
			*((pDes++) + des_len) = '\0';
		}
	}
}

static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* ����USARTΪ�ж�Դ */
  NVIC_InitStructure.NVIC_IRQChannel = BC35_USART3_IRQ;
  /* �������ȼ�*/
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  /* �����ȼ� */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
  /* ʹ���ж� */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  /* ��ʼ������NVIC */
  NVIC_Init(&NVIC_InitStructure);
}


void USART3_Config(void)
{
	
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	// �򿪴���GPIO��ʱ��
	BC35_USART3_GPIO_APBxClkCmd(BC35_USART3_GPIO_CLK, ENABLE);
	
	
	// ��USART Tx��GPIO����Ϊ���츴��ģʽ
	GPIO_InitStructure.GPIO_Pin = BC35_USART3_TX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(BC35_USART3_TX_GPIO_PORT, &GPIO_InitStructure);

  // ��USART Rx��GPIO����Ϊ��������ģʽ
	GPIO_InitStructure.GPIO_Pin = BC35_USART3_RX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(BC35_USART3_RX_GPIO_PORT, &GPIO_InitStructure);
	
	
	// �򿪴��������ʱ��
	BC35_USART3_APBxClkCmd(BC35_USART3_CLK, ENABLE);
	
	
	// ���ô��ڵĹ�������
	// ���ò�����
	USART_InitStructure.USART_BaudRate = BC35_USART3_BAUDRATE;
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
	USART_Init(BC35_USART3x, &USART_InitStructure);
	
	// �����ж����ȼ�����
	NVIC_Configuration();
	
	// ʹ�ܴ��ڽ����ж�
	USART_ITConfig(BC35_USART3x, USART_IT_RXNE, ENABLE);	
	USART_ITConfig(BC35_USART3x, USART_IT_IDLE, ENABLE);//���������ж�
	
	// ʹ�ܴ���
	USART_Cmd(BC35_USART3x, ENABLE);
	
}

void BC35_USART3_IRQHandler(void)                	//����3�жϷ������
{
    char Res;
    if(USART_GetITStatus(BC35_USART3x, USART_IT_RXNE) != RESET)  //�����жϣ�������չ������
    {
			Res=USART_ReceiveData(BC35_USART3x);//����ģ�������;
			RxBuf[RxCoun++] = Res;
			if(RxCoun > 99){
				RxCoun=0;
			}//���������趨
			USART_ClearFlag(BC35_USART3x,USART_IT_RXNE);
    } 
    if(USART_GetITStatus(BC35_USART3x, USART_IT_IDLE) != RESET)  //ģ�����
    {
      Res=USART_ReceiveData(BC35_USART3x);//����ģ�������;
			if(strstr((const char*)RxBuf,(const char*)"+NNMI")==NULL)
				{
					if(RxCoun + RxCounter > 99){
						RxCounter=0;
					}
					appendchar(RxBuf, RxBuffer, RxCoun, RxCounter);
					RxCounter += RxCoun;
					Clear_RxBuf();
				} else {
					EnQueue(&struc_queue, RxBuf);
					Clear_RxBuf();
				}
				USART_ClearFlag(BC35_USART3x,USART_IT_IDLE);
    } 
}

void Usart3_SendByte( USART_TypeDef * pUSARTx, uint8_t ch)
{
	/* ����һ���ֽ����ݵ�USART */
	USART_SendData(pUSARTx,ch);
		
	/* �ȴ��������ݼĴ���Ϊ�� */
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}


void Usart3_SendString(char *str)
{
	unsigned int k=0;
  do 
  {
      Usart3_SendByte( BC35_USART3x, *(str + k) );
      k++;
  } while(*(str + k)!='\0');
  /* �ȴ�������� */
  while(USART_GetFlagStatus(BC35_USART3x,USART_FLAG_TC)==RESET)
  {}
}
