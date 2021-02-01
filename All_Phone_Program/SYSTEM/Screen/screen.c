#include"screen.h"

u8 SendData[50]={0};
u8 u8_senddataCnt; 
 
u16 tamp,checksum=0;
u16 u16_RxCount;
int *i16_pRxBuff;
int i16_RxBuff[50];
u16 u16_StartMeasureConfCnt;

u16 i=0;
int ProductModel_Address;
int VoltageCondition;

struct structMeasureVltVaule m_st_MeasureVltVaule;
struct structMeasureVltFault m_st_MeasureVltFault;



/******************************************************
��������void MeasureVltInit(void)
��������ѹֵ������ʼ��
��������
*******************************************************/
void MeasureVltInit(void)
{
	u16 u16_Temp = 0;
	
	SendData[0]=0x03;//��ʼ֡
	SendData[1]=(42&0x7F);//Э������
	SendData[41] = 0x0d;//֡����
	
	m_st_MeasureVltVaule.bCOpMeasureStart =1;
	m_st_MeasureVltVaule.bMeasureFinish = 1;
	
	u16_StartMeasureConfCnt = 0;
	
	i16_pRxBuff = i16_RxBuff;
	u16_RxCount = 0;
	
	for(u16_Temp = 0;u16_Temp < (sizeof(i16_RxBuff)/sizeof(i16_RxBuff[0]));u16_Temp ++)
	{
		i16_RxBuff[u16_Temp] = 0;
	}
}


void UART4_IRQHandler(void)
{ 
	 u8 Res;
	 u16 u16_Temp = 0;
   if(USART_GetITStatus(UART4,USART_IT_RXNE)!=RESET)
   {
		 USART_ClearITPendingBit(UART4,USART_IT_RXNE);
		 	
		 Res =USART_ReceiveData(UART4);	//��ȡ���յ�������
		 if(i16_pRxBuff == i16_RxBuff)
		 {
			 if(Res == 0x03)
			 {
				 *i16_pRxBuff ++ = Res;
				 u16_RxCount = 1;
			 }
		 }
		 else
		 {
			 if((i16_pRxBuff - i16_RxBuff) > sizeof(i16_RxBuff)) //���������
			 {
				 i16_pRxBuff = i16_RxBuff;
				 u16_RxCount = 0;
				 return;
			 }
			 
			 *i16_pRxBuff ++ = Res;
			 u16_RxCount++;
			 
			 if((Res == 0x0d) && (u16_RxCount >= 21)) //�������
			 {				 
				 i16_pRxBuff = i16_RxBuff; //Buff ��λ
				 m_st_MeasureVltVaule.bRxDataFinish = 1;
			 }
		 
		 }
		 
		if(m_st_MeasureVltVaule.bRxDataFinish == 1)
	{
		
		DataLoad();
		
		for(u16_Temp=0;u16_Temp<42;u16_Temp++)  //��������
		{
			USART_SendData(UART4,SendData[u16_Temp]);
			//SEGGER_RTT_printf(0,"k = %d;  SendData[k] = 0x%x\n",u16_Temp,SendData[u16_Temp]);
		 
			while(!(UART4->SR & USART_FLAG_TXE));//�ȴ��������
		}
		
		m_st_MeasureVltVaule.bRxDataFinish = 0;
	} 
		 
   }
}

/****************************************************************************
��������void  SendDataProcess(void)
�������������ݴ���
��������
����ֵ����
****************************************************************************/
void  SendDataProcess(void)
{
	u16 u16_Temp = 0;
	
	if(m_st_MeasureVltVaule.bRxDataFinish == 1)
	{
		
		DataLoad();
		
		for(u16_Temp=0;u16_Temp<42;u16_Temp++)  //��������
		{
			USART_SendData(UART4,SendData[u16_Temp]);
			//SEGGER_RTT_printf(0,"u16_Temp = %d\n",SendData[u16_Temp]);
		 
			while(!(UART4->SR & USART_FLAG_TXE));//�ȴ��������
		}
		
		m_st_MeasureVltVaule.bRxDataFinish = 0;
	}
}
/*****************************
��������void  DataLoad(void)
���������ݸ���
��������
*****************************/

void  DataLoad(void)
{
	static u16 u16_Temp2 = 0;
	u16 tamp,checksum=0;
	
	switch(u16_Temp2)
	{
		
		case 0://����ֵ�ϴ�
			tamp=ProductModel_Address;
			SendData[2]=(tamp>>8)&0xff;//��Ʒ�ͺš���ַ0
			SendData[3]=tamp&0xff;
		
		  tamp=((u32)m_st_MeasureVltVaule.u16_AVlt_5_N *cof)>>12;
			SendData[4]=(tamp>>8)&0xff;  //A��5V����ַ1
			SendData[5]=tamp&0xff;

		  tamp=((u32)m_st_MeasureVltVaule.u16_CVlt_5_N*cof)>>12;
			SendData[6]=(tamp>>8)&0xff;//C��5V����ַ2
			SendData[7]=tamp&0xff;
		
		  tamp=((u32)m_st_MeasureVltVaule.u16_CVlt_9_N*cof)>>12;
			SendData[8]=(tamp>>8)&0xff;//C��9V����ַ3
			SendData[9]=tamp&0xff;
		
		  tamp=((u32)m_st_MeasureVltVaule.u16_CVlt_12_N*cof)>>12;
			SendData[10]=(tamp>>8)&0xff;//C��12V����ַ4
			SendData[11]=tamp&0xff;
		
			tamp=((u32)m_st_MeasureVltVaule.u16_CVlt_15_N*cof)>>12;
			SendData[12]=(tamp>>8)&0xff;//C��15V����ַ5
			SendData[13]=tamp&0xff;
			
			tamp=((u32)m_st_MeasureVltVaule.u16_CVlt_20_N*cof)>>12;
			SendData[14]=(tamp>>8)&0xff;//C��20V����ַ6
			SendData[15]=tamp&0xff;
			
			tamp=((u32)m_st_MeasureVltVaule.u16_QVlt_5_N*cof)>>12;;
			SendData[16]=(tamp>>8)&0xff;;//	Q��5V����ַ7
			SendData[17]=tamp&0xff;
			
			tamp=((u32)m_st_MeasureVltVaule.u16_QVlt_9_N*cof)>>12;;
			SendData[18]=(tamp>>8)&0xff;;//Q��9V����ַ8
			SendData[19]=tamp&0xff;
			
			tamp=((u32)m_st_MeasureVltVaule.u16_QVlt_12_N*cof)>>12;;
			SendData[20]=(tamp>>8)&0xff;;//Q��12V����ַ9
			SendData[21]=tamp&0xff;
			
			tamp=0;
			SendData[22]=(tamp>>8)&0xff;//���ء���ַ10
			SendData[23]=tamp&0xff;
			
      tamp=m_st_MeasureVltFault.un16_Alarm_1.All;
			SendData[24]=(tamp>>8)&0xff;//����A��5V��C��5V��9V��12����ַ11
			SendData[25]=tamp&0xff;
			
			tamp=m_st_MeasureVltFault.un16_Alarm_2.All;
			SendData[26]=(tamp>>8)&0xff;//	����C��15V��20V������Q��5V��9V����ַ12
			SendData[27]=tamp&0xff;
			
			tamp=m_st_MeasureVltFault.un16_Alarm_3.All;
			SendData[28]=(tamp>>8)&0xff;//����Q��12V����A��Q��C������AC���ʷ��䡢AQC���ʷ��䣬��ַ13
			SendData[29]=tamp&0xff;
			
			tamp=m_st_MeasureVltFault.un16_Alarm_4.All;
			SendData[30]=(tamp>>8)&0xff;//����C�ڻָ���C�ڷ��ߣ���ַ14
			SendData[31]=tamp&0xff;
			
			tamp=((u32)m_st_MeasureVltVaule.u16_CVltDisA_20_F*cof)>>12;;
			SendData[32]=(tamp>>8)&0xff;;//A��C��ͬʱ����,C��20Vʵ�ʵ�ѹ����ַ15
			SendData[33]=tamp&0xff;
			
			tamp=((u32)m_st_MeasureVltVaule.u16_CVltDisAQ_20_F*cof)>>12;;
			SendData[34]=(tamp>>8)&0xff;;//A��Q��C��ͬʱ����,C��20Vʵ�ʵ�ѹ����ַ16
			SendData[35]=tamp&0xff;
			
			tamp=((u32)m_st_MeasureVltVaule.u16_CVltDisQ_20_F*cof)>>12;;
			SendData[36]=(tamp>>8)&0xff;;//Q��C��ͬʱ����,C��20Vʵ�ʵ�ѹ����ַ17
			SendData[37]=tamp&0xff;
			
			tamp=((u32)m_st_MeasureVltVaule.u16_CVltCover_20_F*cof)>>12;;
			SendData[38]=(tamp>>8)&0xff;;//Q��C��ͬʱ����,C��20Vʵ�ʵ�ѹ����ַ18
			SendData[39]=tamp&0xff;
			
			for(i = 0;i<40;i++)
			{
				checksum +=SendData[i];
			}
			//SEGGER_RTT_printf(0,"null_checksum = 0x%x\n",checksum);
			checksum &= 0xff;
			SendData[40] = checksum;//У���
	
			u16_Temp2 = 1;
			break;
			
		case 1://����ֵ�ϴ�
			tamp=ProductModel_Address;
			SendData[2]=(tamp>>8)&0xff;//��Ʒ�ͺš���ַ0
			SendData[3]=tamp&0xff;

		  tamp=((u32)m_st_MeasureVltVaule.u16_AVlt_5_F *cof)>>12;
			SendData[4]=(tamp>>8)&0xff;  //A��5V����ַ1
			SendData[5]=tamp&0xff;

		  tamp=((u32)m_st_MeasureVltVaule.u16_CVlt_5_F*cof)>>12;
			SendData[6]=(tamp>>8)&0xff;//C��5V����ַ2
			SendData[7]=tamp&0xff;
		
		  tamp=((u32)m_st_MeasureVltVaule.u16_CVlt_9_F*cof)>>12;
			SendData[8]=(tamp>>8)&0xff;//C��9V����ַ3
			SendData[9]=tamp&0xff;
		
		  tamp=((u32)m_st_MeasureVltVaule.u16_CVlt_12_F*cof)>>12;
			SendData[10]=(tamp>>8)&0xff;//C��12V����ַ4
			SendData[11]=tamp&0xff;
		
			tamp=((u32)m_st_MeasureVltVaule.u16_CVlt_15_F*cof)>>12;
			SendData[12]=(tamp>>8)&0xff;//C��15V����ַ5
			SendData[13]=tamp&0xff;
			
			tamp=((u32)m_st_MeasureVltVaule.u16_CVlt_20_F*cof)>>12;
			SendData[14]=(tamp>>8)&0xff;//C��20V����ַ6
			SendData[15]=tamp&0xff;
			
			tamp=((u32)m_st_MeasureVltVaule.u16_QVlt_5_F*cof)>>12;;
			SendData[16]=(tamp>>8)&0xff;;//	Q��5V����ַ7
			SendData[17]=tamp&0xff;
			
			tamp=((u32)m_st_MeasureVltVaule.u16_QVlt_9_F*cof)>>12;;
			SendData[18]=(tamp>>8)&0xff;;//Q��9V����ַ8
			SendData[19]=tamp&0xff;
			
			tamp=((u32)m_st_MeasureVltVaule.u16_QVlt_12_F*cof)>>12;;
			SendData[20]=(tamp>>8)&0xff;;//Q��12V����ַ9
			SendData[21]=tamp&0xff;
			
			tamp=1;
			SendData[22]=(tamp>>8)&0xff;//���ء���ַ10
			SendData[23]=tamp&0xff;
			
      tamp=m_st_MeasureVltFault.un16_Alarm_1.All;
			SendData[24]=(tamp>>8)&0xff;//����A��5V��C��5V��9V��12����ַ11
			SendData[25]=tamp&0xff;
			
			tamp=m_st_MeasureVltFault.un16_Alarm_2.All;
			SendData[26]=(tamp>>8)&0xff;//	����C��15V��20V������Q��5V��9V����ַ12
			SendData[27]=tamp&0xff;
			//SEGGER_RTT_printf(0,"m_st_MeasureVltFault.un16_Alarm_2.All = %d\n",m_st_MeasureVltFault.un16_Alarm_2.All);

			
			tamp=m_st_MeasureVltFault.un16_Alarm_3.All;
			SendData[28]=(tamp>>8)&0xff;//����Q��12V����A��Q��C������AC���ʷ��䡢AQC���ʷ��䣬��ַ13
			SendData[29]=tamp&0xff;
			
			tamp=m_st_MeasureVltFault.un16_Alarm_4.All;
			SendData[30]=(tamp>>8)&0xff;//����C�ڻָ���C�ڷ��ߡ���ַ14
			SendData[31]=tamp&0xff;
			
			tamp=((u32)m_st_MeasureVltVaule.u16_CVltDisA_20_F*cof)>>12;;
			SendData[32]=(tamp>>8)&0xff;;//A��C��ͬʱ����,C��20Vʵ�ʵ�ѹ����ַ15
			SendData[33]=tamp&0xff;
			
			tamp=((u32)m_st_MeasureVltVaule.u16_CVltDisAQ_20_F*cof)>>12;;
			SendData[34]=(tamp>>8)&0xff;;//A��Q��C��ͬʱ����,C��20Vʵ�ʵ�ѹ����ַ16
			SendData[35]=tamp&0xff;
			
			tamp=((u32)m_st_MeasureVltVaule.u16_CVltDisQ_20_F*cof)>>12;;
			SendData[36]=(tamp>>8)&0xff;;//Q��C��ͬʱ����,C��20Vʵ�ʵ�ѹ����ַ17
			SendData[37]=tamp&0xff;
			
			tamp=((u32)m_st_MeasureVltVaule.u16_CVltCover_20_F*cof)>>12;;
			SendData[38]=(tamp>>8)&0xff;;//Q��C��ͬʱ����,C��20Vʵ�ʵ�ѹ����ַ18
			SendData[39]=tamp&0xff;
			
			for(i = 0;i<40;i++)
			{
				checksum +=SendData[i];
			}
			//SEGGER_RTT_printf(0,"null_checksum = 0x%x\n",checksum);
			checksum &= 0xff;
			SendData[40] = checksum;//У���
	
			u16_Temp2 = 0;
			break;
			
		default:
			break;
	}			
}






