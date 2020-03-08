#include "bc35_queue.h"	

/*��ʼ��һ���ն���*/
int InitQueue(SqQueue* Q)
{
	Q->front = 0;        //�ṹ��ָ�� 
	Q->rear = 0;
	return 1;
}

/* ��������� */
int ClearQueue(SqQueue* Q)
{
	Q->front = 0;
	Q->rear = 0;
	return 1;
}

/*���ض��е�ǰ����*/
int QueueLength(const SqQueue* Q)
{
	return ((Q->rear - Q->front + MAXSIZE) % MAXSIZE);
	/* ���ض�����Ԫ�صĸ��� */
}

/*ѭ��������Ӳ���  eΪ�����Ԫ��*/
int EnQueue(SqQueue* Q, char* e)
{
	if ((Q->rear + 1) % MAXSIZE == Q->front)  /*��������*/
	{
		return 0;
	}
	strcpy(Q->ais_data[Q->rear], e);    //��Ԫ��e��ֵ����β 
	Q->rear = (Q->rear + 1) % MAXSIZE;       //���ָ�����һλ��ת�ص�����ͷ 
	return 1;
}

/*�����в�Ϊ��,��ɾ��Q�ж�ͷԪ��,��eֵ����*/
int DeQueue(SqQueue* Q, char* e)
{
	if (Q->front == Q->rear)    /*����Ϊ���ж�*/
	{
		return 0;
	}
	strcpy(e, Q->ais_data[Q->front]);
	Q->front = (Q->front + 1) % MAXSIZE;
	return 1;

}
