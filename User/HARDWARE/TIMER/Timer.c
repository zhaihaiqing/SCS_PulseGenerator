
#include  "main.h"

/********************************************************************
*
*	�ļ�	��	TIMER.C

*	�ܽ�	��	(NONE)

*	����	��	�˴�Ӧ��������ʱ����TIM5��TIM3��
*	TIM5: 32bit��ʱ����MAX TIM CLK=84M������������1us-300s

* TIM3: 16bit��ʱ����MAX TIM CLK=84M��������Ƶ�ʣ����1Hz��1s�����30K��33.333ms
******************************************************************************/



sTimer2_t T2 = {0};

/********************************************************************
*	����	��	�жϺ���������������������������ʱ��ģʽ����ҪTIM2���룬����
******************************************************************************/
void TIM2_IRQHandler(void)
{
	uint8_t i = 0;
	
	if(TIM_GetITStatus(TIM2,TIM_IT_Update) != RESET)
	{
		Enable_Timer5(tim5_arr);						//�ڶ�ʱ��2�д�һ��TIM5
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
		
		if(UserOperation.fMode == UO_MODE_TRAIN)
		{
			train_count++;
			if(train_count>=train_acount)
			{
				train_count=0;
				Process_COMMAND_STOP();
			}
		}
	}	
}

/********************************************************************
*	����	��	��ʼ����Tout = ((arr + 1)*(psc + 1))/Tclk,
				����Tclk = 84MHz��Tout=(arr+1)(psc+1)/84 (us)
*	ʾ��	��	Timer2_Init(719, 1);     
******************************************************************************/
void Timer2_Init(u32 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);  ///ʹ��TIM6ʱ��
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM2_IRQn; //��ʱ��3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x02; //�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_TimeBaseInitStructure.TIM_Period = arr; 	//�Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);//��ʼ��TIM3
	
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE); //����ʱ��3�����ж�
	//TIM_Cmd(TIM2,ENABLE); //ʹ�ܶ�ʱ��3
}








/*****************************************************************************
*	����	��	TIM5�жϺ�����32bit��ʱ���������������
******************************************************************************/
volatile unsigned int TIM5_IRQ_Count=0;

void TIM5_IRQHandler(void)
{	
	if(TIM_GetITStatus(TIM5,TIM_IT_Update) != RESET)
	{	
		
		if((Wave_type == 0) || (Wave_type == 1))
		{
			Output_VorC(UserOperation.bVC, 0, OUTPUT_ENABLE);
			pTRIGGER_OUT = 0;
			Disable_Timer5();
			if(UserOperation.fMode == UO_MODE_SINGLE)
			{
				SW_CV_OUTPUT = 0;   //�ر����
				pLEDOUTPUT = LED_DIRECTLY_OFF;
				DOState.Status[DO_TIM4] = DOSTATE_STATUS_COMPLETE;
			}
		}
		else
		{
			if(UserOperation.fMode == UO_MODE_EXTBNC)
			{
				TIM5_IRQ_Count++;
				if(TIM5_IRQ_Count == 2)
				{
					if(Wave_type == 2)
					{
						Output_VorC(UserOperation.bVC, (int64_t)(0-(int64_t)pPwmArrayParam[DO_TIM4]->Ampl), OUTPUT_ENABLE);
					}
					else
					{
						Output_VorC(UserOperation.bVC, pPwmArrayParam[DO_TIM4]->Ampl, OUTPUT_ENABLE);
					}
				}
				else if(TIM5_IRQ_Count >= 3)
				{
					Output_VorC(UserOperation.bVC, 0, OUTPUT_ENABLE);
					pTRIGGER_OUT = 0;
					Disable_Timer5();
					TIM5_IRQ_Count=0;
				}
			}
			else
			{
				TIM5_IRQ_Count++;
				if(TIM5_IRQ_Count == 2)
				{
					if(Wave_type == 2)
					{
						Output_VorC(UserOperation.bVC, (int64_t)(0-(int64_t)pPwmArrayParam[DO_TIM4]->Ampl), OUTPUT_ENABLE);
					}
					else
					{
						Output_VorC(UserOperation.bVC, pPwmArrayParam[DO_TIM4]->Ampl, OUTPUT_ENABLE);
					}	
					
				}
				else if(TIM5_IRQ_Count >= 3)
				{
					Output_VorC(UserOperation.bVC, 0, OUTPUT_ENABLE);
					pTRIGGER_OUT = 0;
					Disable_Timer5();
					TIM5_IRQ_Count=0;
					if(UserOperation.fMode == UO_MODE_SINGLE)
					{
						SW_CV_OUTPUT = 0;   //�ر����
						pLEDOUTPUT = LED_DIRECTLY_OFF;
						DOState.Status[DO_TIM4] = DOSTATE_STATUS_COMPLETE;
					}
				}
			}
		}
		
			
		
		
		TIM_ClearITPendingBit(TIM5,TIM_IT_Update);
	}	
}




//TIM5_CH2
void Timer5_Init(u32 arr,u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA , ENABLE);//ʹ��GPIOBʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);  ///ʹ��TIM5ʱ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//����
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��
	GPIO_ResetBits(GPIOA,GPIO_Pin_1);	//����Ϊ�͵�ƽ
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM5_IRQn; //��ʱ��5�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x02; //�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_TimeBaseInitStructure.TIM_Period = arr; 	//�Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  	//��ʱ����Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseInitStructure);//��ʼ��TIM5
	
	TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE); //����ʱ��5�����ж�
}


void Enable_Timer5(u32 arr)
{
	
	
	if(Wave_type <= 1)
	{
		TIM5->ARR = arr+1;
	}
	else
	{
		TIM5->ARR = (arr+1)/2;
	}
	
	TIM5->PSC = 41;
	TIM5->CNT	=	0;
	TIM5->CR1 |= TIM_CR1_CEN;
	
	
	//���
	if(Wave_type == 0)	//�����Բ���0����������
	{
		Output_VorC(UserOperation.bVC, pPwmArrayParam[DO_TIM4]->Ampl, OUTPUT_ENABLE);
	}
	else if(Wave_type == 1)	//�����Բ���1���ȸ�����
	{
		Output_VorC(UserOperation.bVC, (int64_t)(0-(int64_t)pPwmArrayParam[DO_TIM4]->Ampl), OUTPUT_ENABLE);
	}
	else if(Wave_type == 2)	//˫���Բ���0�������󸺺���
	{
		Output_VorC(UserOperation.bVC, pPwmArrayParam[DO_TIM4]->Ampl, OUTPUT_ENABLE);
	}
	else					//˫���Բ���1���ȸ���������
	{
		Output_VorC(UserOperation.bVC, (int64_t)(0-(int64_t)pPwmArrayParam[DO_TIM4]->Ampl), OUTPUT_ENABLE);
	}
	pTRIGGER_OUT = 1;
	TIM5_IRQ_Count=1;
	
}

void Disable_Timer5(void)
{
	TIM5->CR1 &= (uint16_t)~TIM_CR1_CEN;	//�رն�ʱ��
	TIM5->CNT = 0;							//��������Ĵ���,ȷ���´μ����Ǵ�0��ʼ��
	
	
	if(UserOperation.fMode == UO_MODE_EXTBNC)
	{
		DOState.Status[DO_TIM4] = DOSTATE_STATUS_COMPLETE;
	}
}





