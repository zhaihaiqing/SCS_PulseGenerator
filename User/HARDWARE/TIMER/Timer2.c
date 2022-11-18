
#include  "main.h"

sTimer2_t T2 = {0};

/********************************************************************
*	����	��	�жϺ���������������
******************************************************************************/
void TIM2_IRQHandler(void)
{
	uint8_t i = 0;
	
	if(TIM2->SR & 0X0001)
	{
		//GPIOG->ODR ^= GPIO_Pin_10;
		
		if(T2.OutputCnt == 0)
		{
			for(i = 0; i < 8; i++)
			{
				DOState.ZeroIndexCnt[i] = 0;
			}
		}
		T2.OutputCnt++;
		
		for(i = 0; i < 8; i++)
		{
			if(DOState.ZeroConfig & (1 << i) && (DOState.Status[i] == DOSTATE_STATUS_RUNNING))
			{
				if(T2.OutputCnt < PulseConf.PulseParam.PulseZero.TblTime[i][0] * 100 / T2_VALUE_MICROperCYCLE)
				{
					DOState.ZeroIndexCnt[i] = 0;
					DOState.Status[i] = DOSTATE_STATUS_RUNNING;
				}
				else if(T2.OutputCnt > PulseConf.PulseParam.PulseZero.TblTime[i][PulseConf.PulseParam.PulseZero.IndexNum[i] - 1] * 100 / T2_VALUE_MICROperCYCLE)		//�������
				{					
					Output_VorC(UserOperation.bVC, 0, OUTPUT_DISABLE);
					DOState.Status[i] = DOSTATE_STATUS_COMPLETE;
					break;
				}
				else
				{
					while(DOState.ZeroIndexCnt[i] > 0 && T2.OutputCnt <= PulseConf.PulseParam.PulseZero.TblTime[i][DOState.ZeroIndexCnt[i] - 1] * 100 / T2_VALUE_MICROperCYCLE)
					{
						DOState.ZeroIndexCnt[i]--;
					}
					while(T2.OutputCnt > PulseConf.PulseParam.PulseZero.TblTime[i][DOState.ZeroIndexCnt[i] + 1] * 100 / T2_VALUE_MICROperCYCLE)
					{
						DOState.ZeroIndexCnt[i]++;
					}
					DOState.Status[i] = DOSTATE_STATUS_RUNNING;
				}
				Output_VorC(UserOperation.bVC, PulseConf.PulseParam.PulseZero.TblAmpl[i][DOState.ZeroIndexCnt[i]], OUTPUT_ENABLE);
			}
		}

		
		TIM2->SR &= ~(1 << 0);
	}	
}

/********************************************************************
*	����	��	��ʼ����Tout = ((arr + 1)*(psc + 1))/Tclk,
				����Tclk = 72MHz��Tout=(arr+1)(psc+1)/72 (us)
*	ʾ��	��	Timer2_Init(719, 1);     //72MHZ,50KHZ���Ƶ��,20us
******************************************************************************/
//void Timer2_Init(u16 arr, u16 psc)
//{
//	RCC->APB1ENR |= 1 << 0;
//	TIM2->ARR 	  = arr;
//	TIM2->PSC     = psc;
//	TIM2->DIER   &= ~(1 << 0);
//	TIM2->CR1    &= ~0X01;
//	MY_NVIC_Init(0, 2, TIM2_IRQn, 2);
//}

void Timer2_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);  ///ʹ��TIM6ʱ��
	
	TIM_TimeBaseInitStructure.TIM_Period = arr; 	//�Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);//��ʼ��TIM3
	
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE); //����ʱ��3�����ж�
	TIM_Cmd(TIM2,ENABLE); //ʹ�ܶ�ʱ��3
	
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM2_IRQn; //��ʱ��3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x02; //�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}


