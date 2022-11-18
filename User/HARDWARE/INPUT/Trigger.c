
#include  "main.h"

#define TRIGGER_EXTBNC_ACTIVE		0			//�����أ��ߵ�ƽ�������
#define TRIGGER_EXTBNE_INACTIVE		1			//�½��أ��͵�ƽֹͣ������������򣬹��½���ʱ���ŵ�ƽΪ��

#define TRIGGER_PWM_ACTIVE			1
#define TRIGGER_PWM_INACTIVE		0
/********************************************************************
*  ����   ���жϴ���
			�����BNC�ⲿ�жϿ�������EXTBNCģʽ�������������Ļ��ֵ������
******************************************************************************/
void EXTI0_IRQHandler(void)
{
	if(UserOperation.fMode == UO_MODE_EXTBNC)
	{
		if(pTRIGGER_IN == TRIGGER_EXTBNE_INACTIVE)
		{
			Output_VorC(UserOperation.bVC, 0, OUTPUT_DISABLE);
			DOState.Status[DO_TIM4] = DOSTATE_STATUS_COMPLETE;														//[V194]
		}
		else if(pTRIGGER_IN == TRIGGER_EXTBNC_ACTIVE)
		{
			if(UserOperation.bVC == SELECT_VC_V)
			{
				pPwmArrayParam[DO_TIM4]->Ampl = UserOperation.V_ModeExtBnc.Param[UO_PARAM_AMPL];
			}
			else if(UserOperation.bVC == SELECT_VC_C)
			{
				pPwmArrayParam[DO_TIM4]->Ampl = UserOperation.C_ModeExtBnc.Param[UO_PARAM_AMPL];
			}
			
			if(pPwmArrayParam[DO_TIM4]->Ampl != 0 && DOState.Status[DO_TIM4] == DOSTATE_STATUS_COMPLETE)			//[V194]�߼����Ͻ������ڽ�������ݴ̼������������running/stop�ַ�
			{
				DOState.Status[DO_TIM4] = DOSTATE_STATUS_RUNNING;
			}
			else if(pPwmArrayParam[DO_TIM4]->Ampl == 0 && DOState.Status[DO_TIM4] == DOSTATE_STATUS_RUNNING)		//[V194]
			{
				DOState.Status[DO_TIM4] = DOSTATE_STATUS_COMPLETE;
			}

			Output_VorC(UserOperation.bVC, pPwmArrayParam[DO_TIM4]->Ampl, OUTPUT_ENABLE);
		}
	}
	
	EXTI->PR |= 1 << 0;
}

#if(PCB_VERSION >= PCB_V15)
void UO_Update(uint8_t updatecmd)
{
	INTX_DISABLE();								//�ر��жϣ��漰PWM�ж���ز���
										
	switch(UserOperation.fMode)
	{
		case UO_MODE_SINGLE:
			
			if(updatecmd == UPDATE_ALL)
			{				
				if(PulseTran_Single(pPwmArrayParam[DO_TIM4]) == RTN_NORMAL)
				{
					DOState.Config |= 1 << DO_TIM4;
				}
				else
				{
					DOState.Config &= ~(1 << DO_TIM4);
					
					if(DOState.Status[DO_TIM4] != DOSTATE_STATUS_COMPLETE)
					{
						Process_COMMAND_STOP(DO_TIM4);
					}
				}
			}
			else if(updatecmd == UPDATE_AMPL)
			{
				if (UserOperation.bVC == SELECT_VC_V)
				{
					pPwmArrayParam[DO_TIM4]->Ampl = UserOperation.V_ModeSingle.Param[UO_PARAM_AMPL];
				}
				else
				{
					pPwmArrayParam[DO_TIM4]->Ampl = UserOperation.C_ModeSingle.Param[UO_PARAM_AMPL];
				}
				
				if(pPwmArrayParam[DO_TIM4]->Ampl == 0)									//[V188],�޸�bug������ӦΪ��ֵΪ0ʱ����ֹͣ�������
				{
					DOState.Config &= ~(1 << DO_TIM4);									//[V189],�޸�bug����ֵΪ0ʱ���������ñ�־λ
					
					if(DOState.Status[DO_TIM4] != DOSTATE_STATUS_COMPLETE)
					{
						Process_COMMAND_STOP(DO_TIM4);
					}
				}
			}
			
			if(pTRIGGER_PWM == TRIGGER_PWM_ACTIVE)										//[V191],�޸���ֵ�����TRIGGER���µ��µ��ӳ�bug
			{
				Output_VorC(UserOperation.bVC, pPwmArrayParam[DO_TIM4]->Ampl, OUTPUT_ENABLE);
			}
		
			break;
		
		case UO_MODE_FREERUN:
			
			if(updatecmd == UPDATE_ALL)
			{
				if(PulseTran_FreeRun(pPwmArrayParam[DO_TIM4]) == RTN_NORMAL)
				{
                    DOState.Config |= 1 << DO_TIM4;
				}
				else
				{
					DOState.Config &= ~(1 << DO_TIM4);
					
					if(DOState.Status[DO_TIM4] != DOSTATE_STATUS_COMPLETE)
					{
						Process_COMMAND_STOP(DO_TIM4);
					}	
				}				
			}
			else if(updatecmd == UPDATE_AMPL)
			{
				if (UserOperation.bVC == SELECT_VC_V)
				{
					pPwmArrayParam[DO_TIM4]->Ampl = UserOperation.V_ModeFreeRun.Param[UO_PARAM_AMPL];
				}
				else
				{
					pPwmArrayParam[DO_TIM4]->Ampl = UserOperation.C_ModeFreeRun.Param[UO_PARAM_AMPL];
				}
				
				if(pPwmArrayParam[DO_TIM4]->Ampl == 0)
				{
					DOState.Config &= ~(1 << DO_TIM4);
					
					if(DOState.Status[DO_TIM4] != DOSTATE_STATUS_COMPLETE)
					{
						Process_COMMAND_STOP(DO_TIM4);
					}					
				}
			}
			
			if(pTRIGGER_PWM == TRIGGER_PWM_ACTIVE)
			{
				Output_VorC(UserOperation.bVC, pPwmArrayParam[DO_TIM4]->Ampl, OUTPUT_ENABLE);
			}
		
			break;
		
		case UO_MODE_TRAIN:
			
			if(updatecmd == UPDATE_ALL)
			{
				if(PulseTran_Train(pPwmArrayParam[DO_TIM4]) == RTN_NORMAL)
				{
					DOState.Config |= 1 << DO_TIM4;
				}
				else
				{
					DOState.Config &= ~(1 << DO_TIM4);
					
					if(DOState.Status[DO_TIM4] != DOSTATE_STATUS_COMPLETE)
					{
						Process_COMMAND_STOP(DO_TIM4);						
					}	
				}				
			}
			else if(updatecmd == UPDATE_AMPL)
			{
				if (UserOperation.bVC == SELECT_VC_V)
				{
					pPwmArrayParam[DO_TIM4]->Ampl = UserOperation.V_ModeTrain.Param[UO_PARAM_AMPL];
				}
				else
				{
					pPwmArrayParam[DO_TIM4]->Ampl = UserOperation.C_ModeTrain.Param[UO_PARAM_AMPL];
				}
				
				if(pPwmArrayParam[DO_TIM4]->Ampl == 0)
				{
					DOState.Config &= ~(1 << DO_TIM4);
					
					if(DOState.Status[DO_TIM4] != DOSTATE_STATUS_COMPLETE)
					{
						Process_COMMAND_STOP(DO_TIM4);
					}				
				}
			}
			
			if(pTRIGGER_PWM == TRIGGER_PWM_ACTIVE)
			{
				Output_VorC(UserOperation.bVC, pPwmArrayParam[DO_TIM4]->Ampl, OUTPUT_ENABLE);
			}
		
			break;
		
		case UO_MODE_COMPUTER:					//ֱ����Protocol�н����˸��µȽ������˴����账��
			
			switch(PulseConf.PulseType)
			{
				case PULSE_TYPE_ZERO:
					
					break;
				
				case PULSE_TYPE_ONE:								
					
					break;
				
				case PULSE_TYPE_TWO:
					
					break;
				
				case PULSE_TYPE_THREE:
					
					break;
				
				case PULSE_TYPE_FOUR:
					
					break;
				
				case PULSE_TYPE_FIVE:
					
					break;
				
				default:
					
					break;
			}
			
			break;
			
		default:
			
			break;
	}
	
	if(updatecmd == UPDATE_ALL)																				//[V188],�޸����´����߼�©������ʼ��������ȫ���bug
	{
		*Tbl_TIM_ARR[DO_TIM4]		= pPwmArrayParam[DO_TIM4]->Pwm[TimerRun[DO_TIM4].Index].PwmARR;
		*Tbl_TIM_PSC[DO_TIM4]		= pPwmArrayParam[DO_TIM4]->Pwm[TimerRun[DO_TIM4].Index].PwmPSC;
		*Tbl_TIM_CCRx[DO_TIM4]		= pPwmArrayParam[DO_TIM4]->Pwm[TimerRun[DO_TIM4].Index].PwmDuty;		
		TimerRun[DO_TIM4].Index		= 0;
		TimerRun[DO_TIM4].PulseNum	= 0;
		TimerRun[DO_TIM4].CycleCnt	= 0;
	}
	
	INTX_ENABLE();								//���ж�
}


#endif

/********************************************************************
*  ����   ����ʼ��
******************************************************************************/
void TriggerEx_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//ʹ��GPIOBʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//ʹ��SYSCFGʱ��
	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��ͨ���ģʽ
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��
	
	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource0);//PE2 ���ӵ��ж���2
	
	
	/* ����EXTI_Line0 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line0;//LINE0
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//�ж��¼�
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling; //�������½��ؾ�������������������Σ��½������0 
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;//ʹ��LINE0
	EXTI_Init(&EXTI_InitStructure);//����
	
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;//�ⲿ�ж�0
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;//��ռ���ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//�����ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure);//����
	
	
	
	
		
////	/* ����EXTI_Line0 */
//	EXTI_InitStructure.EXTI_Line = EXTI_Line15;//LINE0
//	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//�ж��¼�
//	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //�����ش��� 
//	EXTI_InitStructure.EXTI_LineCmd = ENABLE;//ʹ��LINE0
//	EXTI_Init(&EXTI_InitStructure);//����
//	
//	
//	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;//�ⲿ�ж�0
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;//��ռ���ȼ�2
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//�����ȼ�2
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//ʹ���ⲿ�ж�ͨ��
//	NVIC_Init(&NVIC_InitStructure);//����
	
	
//	RCC->APB2ENR |= 1 << 3;
//	GPIOB->CRL &= 0XFFFFFF00;
//	GPIOB->CRL |= 0X00000038;
//	GPIOB->ODR |= (0 << 0) | (1 << 1);
//	
//	Ex_NVIC_Config(GPIO_B, 0, FTIR|RTIR);
//	MY_NVIC_Init(2, 2, EXTI0_IRQn, 2);
	
//	#if(PCB_VERSION >= PCB_V15)
//		GPIOB->CRH &= 0X0FFFFFFF;
//		GPIOB->CRH |= 0X80000000;
//		GPIOB->ODR |= 0 << 15;
		
		//Ex_NVIC_Config(GPIO_B, 15, FTIR|RTIR);
//	#endif
}
