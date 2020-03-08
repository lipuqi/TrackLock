#include "bc35_usart3.h"	

char RxCoun,RxBuf[100]; // 接收缓冲
char RxCounter,RxBuffer[100];  // 其他消息缓冲
SqQueue struc_queue; // 命令接收缓冲

void Clear_RxBuf(void)//清空缓存
{
		u8 i;
		for(i=0;i<100;i++)
		RxBuf[i]=0;//缓存
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
  
  /* 配置USART为中断源 */
  NVIC_InitStructure.NVIC_IRQChannel = BC35_USART3_IRQ;
  /* 抢断优先级*/
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  /* 子优先级 */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
  /* 使能中断 */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  /* 初始化配置NVIC */
  NVIC_Init(&NVIC_InitStructure);
}


void USART3_Config(void)
{
	
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	// 打开串口GPIO的时钟
	BC35_USART3_GPIO_APBxClkCmd(BC35_USART3_GPIO_CLK, ENABLE);
	
	
	// 将USART Tx的GPIO配置为推挽复用模式
	GPIO_InitStructure.GPIO_Pin = BC35_USART3_TX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(BC35_USART3_TX_GPIO_PORT, &GPIO_InitStructure);

  // 将USART Rx的GPIO配置为浮空输入模式
	GPIO_InitStructure.GPIO_Pin = BC35_USART3_RX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(BC35_USART3_RX_GPIO_PORT, &GPIO_InitStructure);
	
	
	// 打开串口外设的时钟
	BC35_USART3_APBxClkCmd(BC35_USART3_CLK, ENABLE);
	
	
	// 配置串口的工作参数
	// 配置波特率
	USART_InitStructure.USART_BaudRate = BC35_USART3_BAUDRATE;
	// 配置 针数据字长
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	// 配置停止位
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	// 配置校验位
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	// 配置硬件流控制
	USART_InitStructure.USART_HardwareFlowControl = 
	USART_HardwareFlowControl_None;
	// 配置工作模式，收发一起
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	// 完成串口的初始化配置
	USART_Init(BC35_USART3x, &USART_InitStructure);
	
	// 串口中断优先级配置
	NVIC_Configuration();
	
	// 使能串口接收中断
	USART_ITConfig(BC35_USART3x, USART_IT_RXNE, ENABLE);	
	USART_ITConfig(BC35_USART3x, USART_IT_IDLE, ENABLE);//开启空闲中断
	
	// 使能串口
	USART_Cmd(BC35_USART3x, ENABLE);
	
}

void BC35_USART3_IRQHandler(void)                	//串口3中断服务程序
{
    char Res;
    if(USART_GetITStatus(BC35_USART3x, USART_IT_RXNE) != RESET)  //接收中断，可以扩展来控制
    {
			Res=USART_ReceiveData(BC35_USART3x);//接收模块的数据;
			RxBuf[RxCoun++] = Res;
			if(RxCoun > 99){
				RxCoun=0;
			}//长度自行设定
			USART_ClearFlag(BC35_USART3x,USART_IT_RXNE);
    } 
    if(USART_GetITStatus(BC35_USART3x, USART_IT_IDLE) != RESET)  //模块空闲
    {
      Res=USART_ReceiveData(BC35_USART3x);//接收模块的数据;
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
	/* 发送一个字节数据到USART */
	USART_SendData(pUSARTx,ch);
		
	/* 等待发送数据寄存器为空 */
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
  /* 等待发送完成 */
  while(USART_GetFlagStatus(BC35_USART3x,USART_FLAG_TC)==RESET)
  {}
}
