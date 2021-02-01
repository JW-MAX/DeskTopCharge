#include "relay.h"



void GPIO_Configuration_Relay(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11  //PA_Control_DRV
															|GPIO_Pin_10 //PB_Control_DRV
															|GPIO_Pin_14 //PC_Control_DRV
															|GPIO_Pin_15;//PC_DRV_4-J31
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10  //PB_DRV_2-J26
	                            |GPIO_Pin_11;//PB_DRV_3-J28
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Init(GPIOD,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_5    //PA_DRV_1-J20
															|GPIO_Pin_3   //PA_DRV_2-J22
															|GPIO_Pin_8   //PA_DRV_3-J33
															|GPIO_Pin_10  //PA_DRV_4-J21
															|GPIO_Pin_11  //PA_DRV_5-J19
															|GPIO_Pin_12  //PB_DRV_1-J24
															|GPIO_Pin_6   //PB_DRV_4-J27
															|GPIO_Pin_4   //PB_DRV_5-J25
															|GPIO_Pin_2   //PB_DRV_6-J23
															|GPIO_Pin_7   //PB_DRV_7-J18
															|GPIO_Pin_9   //PB_DRV_8-J17
															|GPIO_Pin_13  //PC_DRV_1-J30
															|GPIO_Pin_14  //PC_DRV_2-J34
															|GPIO_Pin_15  //PC_DRV_3-J32
															|GPIO_Pin_1;  //PC_DRV_5-J29
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Init(GPIOE,&GPIO_InitStructure);
}

/*
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3;//PA_DRV_2-J22
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8;//PA_DRV_3-J33
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;//PA_DRV_4-J21
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11;//PA_DRV_5-J19
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_12;//PB_DRV_1-J24
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6;//PB_DRV_4-J27
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_4;//PB_DRV_5-J25
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2;//PB_DRV_6-J23
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_7;//PB_DRV_7-J18
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;//PB_DRV_8-J17
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_13;//PC_DRV_1-J30
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_15;//PC_DRV_2-J32
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_14;//PC_DRV_3-J34
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1;//PC_DRV_5-J29
	
	*/
