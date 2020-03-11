#include "gnss_usart2.h"	

u16 point1 = 0;

extern _SaveData Save_Data;
 	
char USART_RX_BUF[USART_REC_LEN];

void CLR_Buf(void)                           // 串口缓存清理
{
	memset(USART_RX_BUF, 0, USART_REC_LEN);      //清空
  point1 = 0;                    
}

void clrStruct()
{
	Save_Data.isGetData = false;
	Save_Data.isParseData = false;
	Save_Data.isUsefull = false;
	Save_Data.isNull = true;
	memset(Save_Data.GPS_Buffer, 0, GPS_Buffer_Length);      //清空
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
  
  
  /* 配置USART为中断源 */
  NVIC_InitStructure.NVIC_IRQChannel = GNSS_USART2_IRQ;
  /* 抢断优先级*/
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  /* 子优先级 */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  /* 使能中断 */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  /* 初始化配置NVIC */
  NVIC_Init(&NVIC_InitStructure);
}


void USART2_Config(void)
{
	
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	// 打开串口GPIO的时钟
	GNSS_USART2_GPIO_APBxClkCmd(GNSS_USART2_GPIO_CLK, ENABLE);
	
	
	// 将USART Tx的GPIO配置为推挽复用模式
	GPIO_InitStructure.GPIO_Pin = GNSS_USART2_TX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GNSS_USART2_TX_GPIO_PORT, &GPIO_InitStructure);

  // 将USART Rx的GPIO配置为浮空输入模式
	GPIO_InitStructure.GPIO_Pin = GNSS_USART2_RX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GNSS_USART2_RX_GPIO_PORT, &GPIO_InitStructure);
	
	
	// 打开串口外设的时钟
	GNSS_USART2_APBxClkCmd(GNSS_USART2_CLK, ENABLE);
	
	
	// 配置串口的工作参数
	// 配置波特率
	USART_InitStructure.USART_BaudRate = GNSS_USART2_BAUDRATE;
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
	USART_Init(GNSS_USART2x, &USART_InitStructure);
	
	// 串口中断优先级配置
	GNSS_Configuration();
	
}

/*****************  发送一个字节 **********************/
void Usart2_SendByte( USART_TypeDef * pUSARTx, uint8_t ch)
{
	/* 发送一个字节数据到USART */
	USART_SendData(pUSARTx,ch);
		
	/* 等待发送数据寄存器为空 */
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}

/*****************  发送字符串 **********************/
void Usart2_SendString(char *str)
{
	unsigned int k=0;
  do 
  {
      Usart2_SendByte( GNSS_USART2x, *(str + k) );
      k++;
  } while(*(str + k)!='\0');
  
  /* 等待发送完成 */
  while(USART_GetFlagStatus(GNSS_USART2x,USART_FLAG_TC)==RESET)
  {}
}

void GNSS_USART2_IRQHandler(void)                	//串口2中断服务程序
{
	u8 Res;
	
	if(USART_GetITStatus(GNSS_USART2x, USART_IT_RXNE) != RESET) 
	{
		Res =USART_ReceiveData(GNSS_USART2x);//(USART1->DR);	//读取接收到的数据
	if(Res == '$')
	{
		point1 = 0;	
	}
	  USART_RX_BUF[point1++] = Res;
	if(USART_RX_BUF[0] == '$' && USART_RX_BUF[4] == 'M' && USART_RX_BUF[5] == 'C')			//确定是否收到"GPRMC/GNRMC"这一帧数据
	{
		if(Res == '\n' && Save_Data.isNull)									   
		{
			Usart2_SendString(GNSS_DISABLE);
			memset(Save_Data.GPS_Buffer, 0, GPS_Buffer_Length);      //清空
			memcpy(Save_Data.GPS_Buffer, USART_RX_BUF, point1); 	//保存数据
			Save_Data.isGetData = true;
			point1 = 0;
			memset(USART_RX_BUF, 0, USART_REC_LEN);      //清空			
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
					errorLog(1);	//解析错误
			}
			else
			{
				subString++;
				if ((subStringNext = strstr(subString, ",")) != NULL)
				{
					char usefullBuffer[2]; 
					switch(i)
					{
						case 1:memcpy(Save_Data.UTCTime, subString, subStringNext - subString);break;	//获取UTC时间
						case 2:memcpy(usefullBuffer, subString, subStringNext - subString);break;	//获取UTC时间
						case 3:memcpy(Save_Data.latitude, subString, subStringNext - subString);break;	//获取纬度信息
						case 4:memcpy(Save_Data.N_S, subString, subStringNext - subString);break;	//获取N/S
						case 5:memcpy(Save_Data.longitude, subString, subStringNext - subString);break;	//获取经度信息
						case 6:memcpy(Save_Data.E_W, subString, subStringNext - subString);break;	//获取E/W

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
					errorLog(2);	//解析错误
				}
			}


		}
	}
}

void getGpsBuffer(int n)
{
	int index = n;
		// 使能串口接收中断
	USART_ITConfig(GNSS_USART2x, USART_IT_RXNE, ENABLE);	
	
	// 使能串口
	USART_Cmd(GNSS_USART2x, ENABLE);
	
	Usart2_SendString(GNSS_ENABLE);
	
	CLR_Buf();//清空缓存
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
	// 使能串口接收中断
	USART_ITConfig(GNSS_USART2x, USART_IT_RXNE, DISABLE);	
	CLR_Buf();//清空缓存
}



