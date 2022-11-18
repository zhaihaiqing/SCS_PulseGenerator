
#include  "main.h"

sTimer6_t T6 = {0}; 

/********************************************************************
*	����	��	����������
******************************************************************************/
static void T_Codec_Process(void)
{
	if(T6.CodecRotateCnt > 0 && T6.CodecRotateCnt < T6_VALUE_SECOND)
	{
		T6.CodecRotateCnt++;
		if(T6.CodecRotateCnt > ROTATE_SPEED_BOUNDARY_MS * T6_VALUE_MILLISEC)
		{
			if(Rotate_Increase_Cnt)
			{
				if(Rotate_Increase_Cnt >= ROTATE_BOUNDARY_TIMES)
				{
					SettingInfo_Modify(KBD_INPUT_CODEC_INC_QUICK);
				}
				else
				{
					SettingInfo_Modify(KBD_INPUT_CODEC_INC_SLOW);
				}
				Rotate_Increase_Cnt = 0;
				T6.CodecRotateCnt   = 0;
			}
			if(Rotate_Decrease_Cnt)
			{
				if(Rotate_Decrease_Cnt >= ROTATE_BOUNDARY_TIMES)
				{
					SettingInfo_Modify(KBD_INPUT_CODEC_DEC_QUICK);
				}
				else
				{
					SettingInfo_Modify(KBD_INPUT_CODEC_DEC_SLOW);
				}
				Rotate_Decrease_Cnt = 0;
				T6.CodecRotateCnt   = 0;
			}
		}
	}
}

/********************************************************************
*	����	��	�Լ�����
******************************************************************************/
static void T_SelfSubtraction(uint16_t *param)
{
	if(*param > 0)
	{
		(*param)--;
	}
}

/********************************************************************
*	����	��	�ԼӺ���
******************************************************************************/
static void T_SelfAddition(uint16_t *param, uint16_t upperlimit)
{
	if(*param > 0 && *param < upperlimit)
	{
		(*param)++;
	}
}

/********************************************************************
*  ����   ��ÿ1ms���ִ��һ��
******************************************************************************/
static void Timer_Execute_Per1ms(void)
{
	T_Codec_Process();

	#if(TEST_LED)
		T_SelfSubtraction(&T6.LedCtrl);
		//test_T_LED();
	#endif

	T_SelfSubtraction(&T6.BeepDecCnt);
	T_SelfSubtraction(&LedShortOn.SingleTriggerCountDown);
	T_SelfSubtraction(&LedShortOn.VCCountDown);
	T_SelfSubtraction(&LedShortOn.UnitSwitchCountDown);
	T_SelfAddition(&T6.MemoryUpdateCnt, T6_VALUE_MINUTE+10);

	T6.RunCnt = (T6.RunCnt < T6_VALUE_MINUTE * 60 * 24) ? (T6.RunCnt + 1) : 0;
}

/********************************************************************
*  ����   ��ÿ10ms���ִ��һ��
******************************************************************************/
static void Timer_Execute_Per10ms(void)
{
	bsp_KeyScan();													//������ѯ
}

/********************************************************************
*	����	����ʱ���жϷ������2ms����һ���ж�
******************************************************************************/
void TIM6_DAC_IRQHandler(void)
{
	if(TIM6->SR&0X0001)												//TIM6����ж�
	{
		
		//GPIOG->ODR ^= GPIO_Pin_10;
		
		Timer_Execute_Per1ms();									//
		
		if(++T6.Scan10msCnt >= 10 * T6_VALUE_MILLISEC)
		{
			Timer_Execute_Per10ms();								//������ѯ����
			T6.Scan10msCnt = 0;
		}
		
		TIM6->SR&=~(1<<0);											//���TIM6�жϱ�־λ
	}
}

/********************************************************************
*	����	��	��ʼ��
*	����	��	Tout = ((arr + 1)*(psc + 1))/Tclk,
				����Tclk = 72MHz��Tout=(arr+1)(psc+1)/72 (us)
*	����	��	TimerX_Init(719, 1);     //72MHZ,500KHZ���Ƶ��,20us
******************************************************************************/
void Timer6_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6,ENABLE);  ///ʹ��TIM6ʱ��
	
	TIM_TimeBaseInitStructure.TIM_Period = arr; 	//�Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM6,&TIM_TimeBaseInitStructure);//��ʼ��TIM3
	
	TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE); //����ʱ��3�����ж�
	TIM_Cmd(TIM6,ENABLE); //ʹ�ܶ�ʱ��3
	
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM6_DAC_IRQn; //��ʱ��3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x02; //��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; //�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}



