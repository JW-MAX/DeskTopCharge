#ifndef __LED_H
#define __LED_H 

#include "pbdata.h"
 
 
#define LED_FAULT_A     PDout(2)     //A口故障LED控制信号
#define LED_PASS_A      PDout(3)     //A口通过测试LED控制信号
#define LED_FAULT_Q     PEout(0)     //Q口故障LED控制信号
#define LED_PASS_Q      PCout(8)     //Q口通过测试LED控制信号
#define LED_FAULT_C     PAout(8)     //C口故障LED控制信号
#define LED_PASS_C      PDout(4)     //C口通过测试LED控制信号
#define BEEP      			PDout(1)     //蜂鸣器

void GPIO_Configuration_Led(void);

#endif



