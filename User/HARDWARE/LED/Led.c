
#include "main.h"


#if(TEST_LED)
/********************************************************************
*	����	��	���Ժ���
******************************************************************************/
	void test_LED(void)
	{
		pLEDBD = LED_DIRECTLY_ON;
		Delay_ms(500);
		pLEDBD = LED_DIRECTLY_OFF;
		Delay_ms(500);
	}
	
/********************************************************************
*	����	��	���Ժ���
*	˵��	��	��ʱ���е��ã�������ܵ����������Ͷ�ʱ����Э����ֹͣĳһ״̬
******************************************************************************/	
	void test_T_LED(void)
	{
		if(T6.LedCtrl == 0)
		{
			T6.LedCtrl = 1000;
		}
		
		if(T6.LedCtrl >= 500)
		{
			pLEDBD = LED_DIRECTLY_ON;
		}
		else
		{
			pLEDBD = LED_DIRECTLY_OFF;
		}
	}
#else
/********************************************************************
*	����	��	�����Ʒ�ʽ
******************************************************************************/
	void CtrlMode_Poll(void)
	{
		static uint8_t ctrlmode_pre = 0;
		if(pCTRLMODE != ctrlmode_pre)
		{
			switch(pCTRLMODE)
			{
				case CTRLMODE_MANNUAL:
					
					if(UserOperation.bVC == SELECT_VC_V)
					{
						UserOperation.fMode = UserOperation.fMode_VPre;
					}
					else if(UserOperation.bVC == SELECT_VC_C)
					{
						UserOperation.fMode = UserOperation.fMode_CPre;
					}
					
					//LZG_WFC,�л���ʾ����ȴ���
					
					break;
				
				case CTRLMODE_COMPUTER:
					
					UserOperation.fMode = UO_MODE_COMPUTER;
				
					//LZG_WFC,�л���ʾ����ȴ���
				
					break;
				
				default:
					break;
			}
			ctrlmode_pre = pCTRLMODE;
		}
	}
#endif

/********************************************************************
*	����	��	LED�˿ڳ�ʼ��
******************************************************************************/
void LED_Init(void)
{
	
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//ʹ��GPIOAʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//ʹ��GPIOBʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);//ʹ��GPIODʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);//ʹ��GPIOEʱ��

	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_8 | GPIO_Pin_11 | GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��
	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_9;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_6 | GPIO_Pin_12 | GPIO_Pin_13;
	GPIO_Init(GPIOD, &GPIO_InitStructure);//��ʼ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4;
	GPIO_Init(GPIOE, &GPIO_InitStructure);//��ʼ��
	

	
	GPIO_SetBits(GPIOA, GPIO_Pin_0 | GPIO_Pin_8 | GPIO_Pin_11 | GPIO_Pin_12);//���øߣ�����
	GPIO_SetBits(GPIOB, GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_9);//���øߣ�����
	GPIO_SetBits(GPIOD, GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_6 | GPIO_Pin_12 | GPIO_Pin_13);//���øߣ�����
	GPIO_SetBits(GPIOE, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4);//���øߣ�����
	

	pLEDPOWER = LED_DIRECTLY_ON;
}

/********************************************************************
*	����	��	ģʽ����LED�ر�
******************************************************************************/
void Led_ModePartOff(void)
{
	pLEDSINGLE	= LED_SN74HC240_OFF;
	pLEDFREERUN	= LED_SN74HC240_OFF;
	pLEDTRAIN	= LED_SN74HC240_OFF;
	pLEDEXTBNC	= LED_SN74HC240_OFF;
}

/********************************************************************
*	����	��	��������LED�ر�
******************************************************************************/
void Led_ParamPartOff(void)
{
	pLEDAMPL		= LED_SN74HC240_OFF;
	pLEDPULSE		= LED_SN74HC240_OFF;
	pLEDFREQUENCY	= LED_SN74HC240_OFF;
	pLEDDURATION	= LED_SN74HC240_OFF;
}

s_LedShortOn	LedShortOn = {0};
/********************************************************************
*	����	��	LED����������ѯ
******************************************************************************/
void Led_ShortOn_Poll(void)
{
	if(LedShortOn.fSinggleTrigger == LEDSHORTON_BEGIN)
	{
		pLEDSINGLETRIGGER                 = LED_SN74HC240_ON;
		LedShortOn.SingleTriggerCountDown = LEDSHORTONTIME;
		LedShortOn.fSinggleTrigger        = LEDSHORTON_ING;
	}
	else if(LedShortOn.fSinggleTrigger == LEDSHORTON_ING)
	{
		if(LedShortOn.SingleTriggerCountDown == 0)
		{
			pLEDSINGLETRIGGER          = LED_SN74HC240_OFF;
			LedShortOn.fSinggleTrigger = LEDSHORTON_END;
		}
	}
	
	if(LedShortOn.fVC == LEDSHORTON_BEGIN)
	{
		//pLEDVC                 = LED_SN74HC240_ON;
		LedShortOn.VCCountDown = LEDSHORTONTIME;
		LedShortOn.fVC         = LEDSHORTON_ING;
	}
	else if(LedShortOn.fVC == LEDSHORTON_ING)
	{
		if(LedShortOn.VCCountDown == 0)
		{
			//pLEDVC         = LED_SN74HC240_OFF;
			LedShortOn.fVC = LEDSHORTON_END;
		}
	}
	
	if(LedShortOn.fUnitSwitch == LEDSHORTON_BEGIN)
	{
		pLEDUNITSWITCH                 = LED_SN74HC240_ON;
		LedShortOn.UnitSwitchCountDown = LEDSHORTONTIME;
		LedShortOn.fUnitSwitch         = LEDSHORTON_ING;
	}
	else if(LedShortOn.fUnitSwitch == LEDSHORTON_ING)
	{
		if(LedShortOn.UnitSwitchCountDown == 0)
		{
			pLEDUNITSWITCH         = LED_SN74HC240_OFF;
			LedShortOn.fUnitSwitch = LEDSHORTON_END;
		}
	}
}
