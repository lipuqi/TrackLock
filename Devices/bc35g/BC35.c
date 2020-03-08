#include "BC35.h"

char *strx;
extern char RxCounter,RxBuffer[100];

int getStrlen(char* str)
{
	int count = 0;
	while (*str++)count++;

	return count;
}

void Clear_Buffer(void)//��ջ���
{
		u8 i;
		for(i=0;i<100;i++)
		RxBuffer[i]=0;//����
		RxCounter=0;
	
}

void BC35_Init(void)
{
    Usart3_SendString("AT\r\n"); 
    delay_ms(300);
    strx=strstr((const char*)RxBuffer,(const char*)"OK");//����OK
    Clear_Buffer();	
    while(strx==NULL)
    {
        Clear_Buffer();	
        Usart3_SendString("AT\r\n"); 
        delay_ms(300);
        strx=strstr((const char*)RxBuffer,(const char*)"OK");//����OK
    }
		
    Usart3_SendString("AT+CSQ\r\n");//���Բ�ѯ��ǰ���ź�ֵ
    delay_ms(300);
    strx=strstr((const char*)RxBuffer,(const char*)"OK");//��OK
    Clear_Buffer();	
		
		Usart3_SendString("AT+CFUN=1\r\n"); 
    delay_ms(300);
    strx=strstr((const char*)RxBuffer,(const char*)"OK");//����OK
    Clear_Buffer();		
		
    Usart3_SendString("AT+CGATT=1\r\n");//
    delay_ms(300);    
    Usart3_SendString("AT+CGATT?\r\n");//��ѯע��״̬
    delay_ms(300);
    strx=strstr((const char*)RxBuffer,(const char*)"+CGATT:1");//��+CGATT:1
    Clear_Buffer();	
    while(strx==NULL)
    {
        Clear_Buffer();	
        Usart3_SendString("AT+CGATT?\r\n");//��ѯע��״̬
        delay_ms(300);
        strx=strstr((const char*)RxBuffer,(const char*)"+CGATT:1");//��+CGATT:1
    } 
		
    Usart3_SendString("AT+CGPADDR\r\n");//��ȡIPֵ
    delay_ms(300);
    strx=strstr((const char*)RxBuffer,(const char*)"+CGPADDR:0");//��+CGCONTRDP:
    Clear_Buffer();	
    while(strx==NULL)
    {
        Clear_Buffer();	
        Usart3_SendString("AT+CGPADDR\r\n");//��ȡIPֵ
        delay_ms(300);
        strx=strstr((const char*)RxBuffer,(const char*)"+CGPADDR:0");//��+CGCONTRDP:������IP������ֻ����OK
    }  
		
		Usart3_SendString("AT+CMEE=1\r\n"); 
    delay_ms(300);
    strx=strstr((const char*)RxBuffer,(const char*)"OK");//����OK
    Clear_Buffer();	
    while(strx==NULL)
    {
        Clear_Buffer();	
        Usart3_SendString("AT+CMEE=1\r\n"); 
        delay_ms(300);
        strx=strstr((const char*)RxBuffer,(const char*)"OK");//����OK
    }
		
		Usart3_SendString("AT+NNMI=1\r\n"); 
    delay_ms(300);
    strx=strstr((const char*)RxBuffer,(const char*)"OK");//����OK
    Clear_Buffer();	
    while(strx==NULL)
    {
        Clear_Buffer();	
        Usart3_SendString("AT+NNMI=1\r\n"); 
        delay_ms(300);
        strx=strstr((const char*)RxBuffer,(const char*)"OK");//����OK
    }
}


//COAP��������
void BC35_COAPdata(char *data)
{
		char res[200] = { 0 };

		sprintf(res, "%s%d%s%s%s", "AT+NMGS=",getStrlen(data) / 2, ",", data, "\r\n");
		
    Usart3_SendString(res);//�ύ���ݵ�ƽ̨��
    delay_ms(300);  
    while(strx==NULL)
    {
        strx=strstr((const char*)RxBuffer,(const char*)"OK");//����OK 
    }
    Clear_Buffer(); 
}
