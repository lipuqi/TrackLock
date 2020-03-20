#ifndef __BC35_QUEUE_H
#define __BC35_QUEUE_H

#include <string.h>

#define MAXSIZE 20

/*ѭ�����жԵ�˳��洢�ṹ*/
typedef struct
{
	char ais_data[MAXSIZE][100];  //�����еĶ�ά����Ԫ�� 
	int front;    //ͷָ��
	int rear;    //βָ��
}SqQueue;

int InitQueue(SqQueue* Q);
int ClearQueue(SqQueue* Q);
int QueueLength(const SqQueue* Q);
int EnQueue(SqQueue* Q, char* e);
int DeQueue(SqQueue* Q, char* e);

#endif
