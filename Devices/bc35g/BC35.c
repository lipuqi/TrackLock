#include "BC35.h"

char *strx;
extern char RxCounter,RxBuffer[100];

void Clear_Buffer(void)//��ջ���
{
		u8 i;
		for(i=0;i<100;i++)
		RxBuffer[i]=0;//����
		RxCounter=0;
	
}

int BC35_Init(void)
{
		int index = 10; //������ֵ
    Usart3_SendString("AT\r\n"); 
    delay_ms(300);
    strx=strstr((const char*)RxBuffer,(const char*)"OK");//����OK
    Clear_Buffer();	
    while(strx==NULL && index > 0)
    {
        Clear_Buffer();	
        Usart3_SendString("AT\r\n"); 
        delay_ms(300);
        strx=strstr((const char*)RxBuffer,(const char*)"OK");//����OK
				index--;
    }	
		if(index < 0 ){
			return 0;
		} else {
			index = 10;
			IWDG_Feed();
		}
		Usart3_SendString("AT+CFUN=1\r\n"); 
    delay_ms(300);
    strx=strstr((const char*)RxBuffer,(const char*)"OK");//����OK
    Clear_Buffer();		
		while(strx==NULL && index > 0)
    {
				delay_ms(300);
				Usart3_SendString("AT+CFUN=1\r\n"); 
				delay_ms(300);
				strx=strstr((const char*)RxBuffer,(const char*)"OK");//����OK
				Clear_Buffer();	
				index--;
    }
		if(index < 0 ){
			return 0;
		} else {
			index = 10;
			IWDG_Feed();
		}
    Usart3_SendString("AT+CGATT=1\r\n");//
    delay_ms(300);    
    Usart3_SendString("AT+CGATT?\r\n");//��ѯע��״̬
    delay_ms(300);
    strx=strstr((const char*)RxBuffer,(const char*)"+CGATT:1");//��+CGATT:1
    Clear_Buffer();	
    while(strx==NULL && index > 0)
    {
        Clear_Buffer();	
        Usart3_SendString("AT+CGATT?\r\n");//��ѯע��״̬
        delay_ms(300);
        strx=strstr((const char*)RxBuffer,(const char*)"+CGATT:1");//��+CGATT:1
				index--;
    } 
		if(index < 0 ){
			return 0;
		} else {
			index = 10;
			IWDG_Feed();
		}
    Usart3_SendString("AT+CGPADDR\r\n");//��ȡIPֵ
    delay_ms(300);
    strx=strstr((const char*)RxBuffer,(const char*)"+CGPADDR:0");//��+CGCONTRDP:
    Clear_Buffer();	
    while(strx==NULL && index > 0)
    {
        Clear_Buffer();	
        Usart3_SendString("AT+CGPADDR\r\n");//��ȡIPֵ
        delay_ms(300);
        strx=strstr((const char*)RxBuffer,(const char*)"+CGPADDR:0");//��+CGCONTRDP:������IP������ֻ����OK
				index--;
    }  
		if(index < 0 ){
			return 0;
		} else {
			index = 10;
			IWDG_Feed();
		}
		Usart3_SendString("AT+CMEE=1\r\n"); 
    delay_ms(300);
    strx=strstr((const char*)RxBuffer,(const char*)"OK");//����OK
    Clear_Buffer();	
    while(strx==NULL && index > 0)
    {
        Clear_Buffer();	
        Usart3_SendString("AT+CMEE=1\r\n"); 
        delay_ms(300);
        strx=strstr((const char*)RxBuffer,(const char*)"OK");//����OK
				index--;
    }
		if(index < 0 ){
			return 0;
		} else {
			index = 10;
			IWDG_Feed();
		}
		Usart3_SendString("AT+NNMI=1\r\n"); 
    delay_ms(300);
    strx=strstr((const char*)RxBuffer,(const char*)"OK");//����OK
    Clear_Buffer();	
    while(strx==NULL && index > 0)
    {
        Clear_Buffer();	
        Usart3_SendString("AT+NNMI=1\r\n"); 
        delay_ms(300);
        strx=strstr((const char*)RxBuffer,(const char*)"OK");//����OK
				index--;
    }
		if(index < 0 ){
			return 0;
		} else {
			index = 10;
			IWDG_Feed();
		}
		return isCommunication();
}


//COAP��������
void BC35_COAPdata(char *data)
{
		char res[200] = { 0 };

		sprintf(res, "%s%d%s%s%s", "AT+NMGS=",getStrlen(data) / 2, ",", data, "\r\n");
		printf(">BC35_COAPdata -> res:%s\r\n", res);
    Usart3_SendString(res);//�ύ���ݵ�ƽ̨��
		IWDG_Feed();
		delay_ms(300);
}

void BC35_Shutdown(void)
{
	  Usart3_SendString("AT+QLWULDATAEX=3,AA34BB,0x0001\r\n"); 
    delay_ms(300);
    strx=strstr((const char*)RxBuffer,(const char*)"OK");//����OK
    Clear_Buffer();	
    while(strx==NULL)
    {
        Clear_Buffer();	
        Usart3_SendString("AT+QLWULDATAEX=3,AA34BB,0x0001\r\n"); 
        delay_ms(300);
        strx=strstr((const char*)RxBuffer,(const char*)"OK");//����OK
    }
		IWDG_Feed();
		Usart3_SendString("AT+CFUN=0\r\n"); 
    delay_ms(300);
    strx=strstr((const char*)RxBuffer,(const char*)"OK");//����OK
    Clear_Buffer();	
    while(strx==NULL)
    {
        Clear_Buffer();	
        Usart3_SendString("AT+CFUN=0\r\n"); 
        delay_ms(300);
        strx=strstr((const char*)RxBuffer,(const char*)"OK");//����OK
    }
}

void BC35_Restart(void)
{
	Usart3_SendString("AT+QLWULDATAEX=3,AA34BB,0x0001\r\n"); 
	delay_ms(300);
	Clear_Buffer();
	Usart3_SendString("AT+CFUN=0\r\n"); 
	delay_ms(300);
	Clear_Buffer();
	delay_ms(1000);
	Usart3_SendString("AT+NRB\r\n"); 
	delay_ms(300);
	IWDG_Feed();
	while((strx=strstr((const char*)RxBuffer,(const char*)"OK"))!=NULL);
	Clear_Buffer();
}

int isCommunication(void)
{
	int index = 5;
	while(index > 0){
		Usart3_SendString("AT+CSQ\r\n");//���Բ�ѯ��ǰ���ź�ֵ
		delay_ms(300);
		printf("isCommunication ->%s\r\n", strx);
		strx=strstr((const char*)RxBuffer,(const char*)"+CSQ:99,99");
		Clear_Buffer();
		while(strx==NULL)
		{
			return 1;
		}
		index--;
		delay_ms(300);
		IWDG_Feed();
	}
	return 0;
}


