#ifndef __LED_H
#define __LED_H 

#include "pbdata.h"
 
 
#define LED_FAULT_A     PDout(2)     //A�ڹ���LED�����ź�
#define LED_PASS_A      PDout(3)     //A��ͨ������LED�����ź�
#define LED_FAULT_Q     PEout(0)     //Q�ڹ���LED�����ź�
#define LED_PASS_Q      PCout(8)     //Q��ͨ������LED�����ź�
#define LED_FAULT_C     PAout(8)     //C�ڹ���LED�����ź�
#define LED_PASS_C      PDout(4)     //C��ͨ������LED�����ź�
#define BEEP      			PDout(1)     //������

void GPIO_Configuration_Led(void);

#endif



