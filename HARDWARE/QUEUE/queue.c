#include "main.h"

T_QUEUE at_send_Queue;
T_QUEUE at_rec_Queue;

//�ж�ѭ�������Ƿ�Ϊ��
int IsQueEmpty(PT_QUEUE ptQue)
{
   return ptQue->dwHead == ptQue->dwTail;
}

 //�ж�ѭ�������Ƿ�Ϊ��
int IsQueFull(PT_QUEUE ptQue)
{
  return (ptQue->dwTail + 1) % QUEUE_SIZE == ptQue->dwHead;
}

//��ȡѭ������Ԫ����Ŀ
int QueDataNum(PT_QUEUE ptQue)
{
  return (ptQue->dwTail - ptQue->dwHead + QUEUE_SIZE) % QUEUE_SIZE;
}

//��ȡѭ�����ж���λ��
int GetQueHead(PT_QUEUE ptQue)
{
  return ptQue->dwHead;
}
//��ȡѭ�����ж���Ԫ��
int GetQueHeadData(PT_QUEUE ptQue)
{
  return ptQue->aData[ptQue->dwHead];
}
//��ȡѭ�����ж�βλ��
int GetQueTail(PT_QUEUE ptQue)
{
  return ptQue->dwTail;
}

void InitQue(PT_QUEUE ptQue)
{
   memset(ptQue, 0, sizeof(*ptQue));
}

 //��ѭ�������в���Ԫ��
void EnterQue(PT_QUEUE ptQue, int dwElem)
{
   if(IsQueFull(ptQue))
   {
			return;
	 }
		ptQue->aData[ptQue->dwTail]= dwElem;
		ptQue->dwTail = (ptQue->dwTail + 1) % QUEUE_SIZE;
}

//��ѭ��������ȡ��Ԫ��
int LeaveQue(PT_QUEUE ptQue)
{
   int dwElem;
	if(IsQueEmpty(ptQue))
  {
//     printf("Queue %p is Empty!\n", ptQue);
     return -1;
  }
  dwElem = ptQue->aData[ptQue->dwHead];
  ptQue->dwHead = (ptQue->dwHead + 1) % QUEUE_SIZE;
  return dwElem;
}

//�Ӷ�������β������ʾ����Ԫ��ֵ
void DisplayQue(PT_QUEUE ptQue)
{
	int dwIdx;
	if(IsQueEmpty(ptQue))
  {
     return;
  }

  dwIdx= ptQue->dwHead;
  while((dwIdx % QUEUE_SIZE) != ptQue->dwTail);
}





