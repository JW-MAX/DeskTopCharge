#ifndef __SWITCH_H
#define __SWITCH_H 

#include"pbdata.h"


#define IO_INPUT0 GPIOC
#define IO_INPUT1 GPIOC
#define IO_INPUT2 GPIOC
#define IO_INPUT3 GPIOC

#define IO_INPUT0_Pin GPIO_Pin_6
#define IO_INPUT1_Pin GPIO_Pin_7
#define IO_INPUT2_Pin GPIO_Pin_9
#define IO_INPUT3_Pin GPIO_Pin_12



void GPIO_Configuration_switch(void); 


#endif
