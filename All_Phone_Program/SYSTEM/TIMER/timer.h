#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"


extern u16 TimeMS;
extern u16 Timer10ms,Timer40ms;

extern int DelayCondition;//��ʱ״̬
extern int DelayClear;//��ʱ��־���
extern u16 BeepCnt;    //���������������ʱ��
extern u16 VltControlCnt; //��ѹ�����źŷ��ʹ���������
extern u16 VltControlTimeCnt;//��ѹ�����е�ѹ�ȼ������ʱ������
extern u16 Temp,TempPer10ms,TempPer10ms_1,TempPer10ms_2,TempPer10ms_3;


void TIM2_Int_Init(void);
void TIM3_Int_Init(u16 arr,u16 psc);
void Drv_SysTimer(void);
//void TIM4_Configuration(void);

#endif
