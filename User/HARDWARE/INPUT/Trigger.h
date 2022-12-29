
/********************************************************************
*
*	�ļ�	��	Trigger.h

*	�ܽ�	��	
				PB0 --TRIGGER_IN
				PB1 --TRIGGER_OUT
				PB15--TRIGGER_PWM

*	����	��	�ⲿ�����źţ�TTL��ʽ���жϴ���
				��������źţ�TTL��ʽ��ͬ������ߵ�ƽ����
				��Ƭ��PWM�����жϣ�TTL��ʽ
*
******************************************************************************/
#ifndef __TRIGGER_H
#define __TRIGGER_H
#include "main.h"

#define pTRIGGER_IN		PBin(0)
#define pTRIGGER_OUT	PBout(1)

#if(PCB_VERSION >= PCB_15)
	//#define pTRIGGER_PWM	PBin(15)
	#define pTRIGGER_PWM	PBin(2)
	
	#define UPDATE_NO	0	//������
	#define UPDATE_ALL	1	//����ȫ�����������������Ƿ�������״̬��PSC ARR CCRX AMPL�Ȳ���
	#define UPDATE_AMPL	2	//ֻ����AMPL����
	void UO_Update(uint8_t updatecmd);
	
	void PWM_EXTI15_IRQHandler(void);
#endif

extern __IO unsigned int ex_bnc_flag;



void TriggerExIN01_Init(void);
void TriggerExIN23_Init(void);
void TriggerOUT_Init(void);

void TriggerEx_Init(void);



#endif
