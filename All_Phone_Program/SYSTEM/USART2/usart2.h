#ifndef __USART2_H
#define __USART2_H	 
#include "sys.h"
#include "delay.h"
#include "stdarg.h"	 	 
#include "stdio.h"	 	 
#include "string.h"
#include"pbdata.h"
//////////////////////////////////////////////////////////////////////////////////	 
//ALIENTEK STM32������
//����2��������	   
//����ԭ��@ALIENTEK
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	   

#define USART2_MAX_RECV_LEN		200					//�����ջ����ֽ���
#define USART2_MAX_SEND_LEN		200					//����ͻ����ֽ���
#define USART2_RX_EN 			1					//0,������;1,����.

extern u8  USART2_RX_BUF[USART2_MAX_RECV_LEN]; 		//���ջ���,���USART2_MAX_RECV_LEN�ֽ�
extern u8  USART2_TX_BUF[USART2_MAX_SEND_LEN]; 		//���ͻ���,���USART2_MAX_SEND_LEN�ֽ�
extern u16 USART2_RX_STA;   						//��������״̬

void USART2_Init(u32 bound);				//����2��ʼ�� 
//void USART2_IRQHandler(void);
//void GPIO_Configuration(void);
//void NVIC_Configuration(void);
//void USART_Configuration(void);
//void TIM5_Set(u8 sta);
//void TIM5_Init(u16 arr,u16 psc);
//void UART_DMA_Config(DMA_Channel_TypeDef*DMA_CHx,u32 cpar,u32 cmar);
//void UART_DMA_Enable(DMA_Channel_TypeDef*DMA_CHx,u8 len);
//void u2_printf(char* fmt, ...);
#endif







