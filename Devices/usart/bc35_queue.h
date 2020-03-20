#ifndef __BC35_QUEUE_H
#define __BC35_QUEUE_H

#include <string.h>

#define MAXSIZE 20

/*循环队列对的顺序存储结构*/
typedef struct
{
	char ais_data[MAXSIZE][100];  //队列中的二维数组元素 
	int front;    //头指针
	int rear;    //尾指针
}SqQueue;

int InitQueue(SqQueue* Q);
int ClearQueue(SqQueue* Q);
int QueueLength(const SqQueue* Q);
int EnQueue(SqQueue* Q, char* e);
int DeQueue(SqQueue* Q, char* e);

#endif
