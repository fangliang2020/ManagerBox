#ifndef __QUEUE_H
#define __QUEUE_H

#define QUEUE_SIZE 20 //队列最大容纳8-1个元素
#define eEventType 20
#define eDevPort 20


typedef struct{	
   int aData[QUEUE_SIZE];  //队列元素
   int dwHead;  //指向队首元素
   int dwTail;  //指向队尾元素的下一个元素
}T_QUEUE, *PT_QUEUE;

extern T_QUEUE at_send_Queue;
extern T_QUEUE at_rec_Queue;

int IsQueFull(PT_QUEUE ptQue);
int QueDataNum(PT_QUEUE ptQue);
int GetQueHead(PT_QUEUE ptQue);
int GetQueHeadData(PT_QUEUE ptQue);
int GetQueTail(PT_QUEUE ptQue);

void InitQue(PT_QUEUE ptQue);
void EnterQue(PT_QUEUE ptQue, int dwElem);
int LeaveQue(PT_QUEUE ptQue);
void DisplayQue(PT_QUEUE ptQue);

#endif

