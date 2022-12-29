
#include  "main.h"

/********************************************************************
*
*	文件	：	TIMER.C

*	管脚	：	(NONE)

*	功能	：	此处应用两个定时器，TIM5和TIM3，
*	TIM5: 32bit定时器，MAX TIM CLK=84M，用来计脉宽，1us-300s

* TIM3: 16bit定时器，MAX TIM CLK=84M，用来计频率，最低1Hz，1s，最大30K，33.333ms
******************************************************************************/



sTimer2_t T2 = {0};

/********************************************************************
*	功能	：	中断函数，打点输出计数，在连续、按时长模式中需要TIM2参与，其中
******************************************************************************/
void TIM2_IRQHandler(void)
{
	uint8_t i = 0;
	
	if(TIM_GetITStatus(TIM2,TIM_IT_Update) != RESET)
	{
		Enable_Timer5(tim5_arr);						//在定时器2中触一次TIM5
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
*	功能	：	初始化，Tout = ((arr + 1)*(psc + 1))/Tclk,
				其中Tclk = 84MHz，Tout=(arr+1)(psc+1)/84 (us)
*	示例	：	Timer2_Init(719, 1);     
******************************************************************************/
void Timer2_Init(u32 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);  ///使能TIM6时钟
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM2_IRQn; //定时器3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x02; //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_TimeBaseInitStructure.TIM_Period = arr; 	//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);//初始化TIM3
	
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE); //允许定时器3更新中断
	//TIM_Cmd(TIM2,ENABLE); //使能定时器3
}








/*****************************************************************************
*	功能	：	TIM5中断函数，32bit定时器，控制脉宽输出
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
				SW_CV_OUTPUT = 0;   //关闭输出
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
						SW_CV_OUTPUT = 0;   //关闭输出
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
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA , ENABLE);//使能GPIOB时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);  ///使能TIM5时钟
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//上拉
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化
	GPIO_ResetBits(GPIOA,GPIO_Pin_1);	//设置为低电平
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM5_IRQn; //定时器5中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x02; //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_TimeBaseInitStructure.TIM_Period = arr; 	//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  	//定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseInitStructure);//初始化TIM5
	
	TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE); //允许定时器5更新中断
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
	
	
	//输出
	if(Wave_type == 0)	//单极性波形0，先正后零
	{
		Output_VorC(UserOperation.bVC, pPwmArrayParam[DO_TIM4]->Ampl, OUTPUT_ENABLE);
	}
	else if(Wave_type == 1)	//单极性波形1，先负后零
	{
		Output_VorC(UserOperation.bVC, (int64_t)(0-(int64_t)pPwmArrayParam[DO_TIM4]->Ampl), OUTPUT_ENABLE);
	}
	else if(Wave_type == 2)	//双极性波形0，先正后负后零
	{
		Output_VorC(UserOperation.bVC, pPwmArrayParam[DO_TIM4]->Ampl, OUTPUT_ENABLE);
	}
	else					//双极性波形1，先负后正后零
	{
		Output_VorC(UserOperation.bVC, (int64_t)(0-(int64_t)pPwmArrayParam[DO_TIM4]->Ampl), OUTPUT_ENABLE);
	}
	pTRIGGER_OUT = 1;
	TIM5_IRQ_Count=1;
	
}

void Disable_Timer5(void)
{
	TIM5->CR1 &= (uint16_t)~TIM_CR1_CEN;	//关闭定时器
	TIM5->CNT = 0;							//清零计数寄存器,确保下次计数是从0开始的
	
	
	if(UserOperation.fMode == UO_MODE_EXTBNC)
	{
		DOState.Status[DO_TIM4] = DOSTATE_STATUS_COMPLETE;
	}
}





