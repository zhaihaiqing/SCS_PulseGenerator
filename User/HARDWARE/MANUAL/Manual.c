
#include  "main.h"

sUserOperation_t UserOperation = {
									UO_PHASE_UNIPHASE, 
									0, 
									UO_MODE_NONE, 
									UO_PARAM_NONE, 
									UO_PARAM_NONE, 
									0, 
									UO_MODE_NONE, 
									{0}, 
									{0}, 
									{0}, 
									{0}, 
									UO_MODE_NONE, 
									0
								};

uint64_t Ceiling[4] = {CEILING_V_AMPLITUDE, CEILING_PULSE, CEILING_FREQUENCY, CEILING_DURATION};
/********************************************************************
*	����	��	��������õĲ��������Ϣ��ȡ
*	����	��	�����ѹ/����--ģʽ--����--����ֵ����λ��
******************************************************************************/
static void SUB_SettingInfo_Get(uint8_t IsParamTypeChanged, uint8_t IsUnitSwitched, uint8_t unitbit)
{
	if(IsParamTypeChanged)
	{
		UserOperation.GetSetupData.pParamNAD = &(UserOperation.GetSetupData.pModeParam->ParamNAD[UserOperation.fParamType]);
		UserOperation.GetSetupData.ParamValue = UserOperation.GetSetupData.pModeParam->Param[UserOperation.fParamType];
		UserOperation.GetSetupData.Ceiling = Ceiling[UserOperation.fParamType];
	}
	if(IsUnitSwitched)
	{
		if((UserOperation.fUnitCur & (1 << unitbit)) == 0)
		{
			UserOperation.fUnitCur |= (1 << unitbit);
		}
		else
		{
			UserOperation.fUnitCur &= ~(1 << unitbit);
		}
		UserOperation.GetSetupData.pModeParam->fUnit = UserOperation.fUnitCur;											//�����л����ʶ�����Ա������л�ģʽ/�����ص�
		
		UserOperation.Modify.bUnitSwitched = !UserOperation.Modify.bUnitSwitched;
	}
	UserOperation.GetSetupData.bUnit = ((UserOperation.fUnitCur & (1 << unitbit)) > 0)? 1:0;							//[V88]���ָ������ò����ĵ�λ����ʶ��Ϣ���޸�ֻ�л�����ʱ��λ��ƥ��bug
}
								
#define PARAMTYPECHANGE_N	0
#define PARAMTYPECHANGE_Y	1
#define UNITSWITCH_N		0
#define UNITSWITCH_Y		1
/********************************************************************
*	����	��	�������õĲ��������Ϣ��ȡ
*	����	��	�����ѹ/����--ģʽ--����--����ֵ����λ��
******************************************************************************/
static void SettingInfo_Get(uint8_t IsParamTypeChanged, uint8_t IsUnitSwitched)
{
	uint8_t unitbit = 0;
	
	if(IsParamTypeChanged == 0 && IsUnitSwitched == 0)
	{
		return;
	}
	
	unitbit = UserOperation.fParamType;
	if(UserOperation.bVC == SELECT_VC_V)
	{
		Ceiling[UO_PARAM_AMPL] = CEILING_V_AMPLITUDE;
		
		switch(UserOperation.fMode)
		{
			case UO_MODE_SINGLE:
				UserOperation.GetSetupData.pModeParam = &UserOperation.V_ModeSingle;
				SUB_SettingInfo_Get(IsParamTypeChanged, IsUnitSwitched, unitbit);
				break;
			case UO_MODE_FREERUN:
				UserOperation.GetSetupData.pModeParam = &UserOperation.V_ModeFreeRun;
				SUB_SettingInfo_Get(IsParamTypeChanged, IsUnitSwitched, unitbit);
				break;
			case UO_MODE_TRAIN:
				UserOperation.GetSetupData.pModeParam = &UserOperation.V_ModeTrain;
				SUB_SettingInfo_Get(IsParamTypeChanged, IsUnitSwitched, unitbit);
				break;
			case UO_MODE_EXTBNC:
				UserOperation.GetSetupData.pModeParam = &UserOperation.V_ModeExtBnc;
				SUB_SettingInfo_Get(IsParamTypeChanged, IsUnitSwitched, unitbit);
				break;
			default:
				break;
		}
	}
	else
	{
		Ceiling[UO_PARAM_AMPL] = CEILING_C_AMPLITUDE;
		
		if(unitbit == UO_UNIT_BIT_AMPL_V)
		{
			unitbit = UO_UNIT_BIT_AMPL_C;
		}
		
		switch(UserOperation.fMode)
		{
			case UO_MODE_SINGLE:
				UserOperation.GetSetupData.pModeParam = &UserOperation.C_ModeSingle;
				SUB_SettingInfo_Get(IsParamTypeChanged, IsUnitSwitched, unitbit);
				break;
			case UO_MODE_FREERUN:
				UserOperation.GetSetupData.pModeParam = &UserOperation.C_ModeFreeRun;
				SUB_SettingInfo_Get(IsParamTypeChanged, IsUnitSwitched, unitbit);
				break;
			case UO_MODE_TRAIN:
				UserOperation.GetSetupData.pModeParam = &UserOperation.C_ModeTrain;
				SUB_SettingInfo_Get(IsParamTypeChanged, IsUnitSwitched, unitbit);
				break;
			case UO_MODE_EXTBNC:
				UserOperation.GetSetupData.pModeParam = &UserOperation.C_ModeExtBnc;
				SUB_SettingInfo_Get(IsParamTypeChanged, IsUnitSwitched, unitbit);
				break;
			default:
				break;
		}
	}
}

/********************************************************************
*	����	��	�޸Ĳ���ֵ
*	����	��	����0-9���ֻ��/����ͷ�趨����
******************************************************************************/
void SettingInfo_Modify(uint8_t btn_value)
{
	uint32_t value_expand = VALUE_EXPAND_102;
	
	if(UserOperation.fMode == UO_MODE_NONE || UserOperation.fParamType == UO_PARAM_NONE)
	{
		return;
	}	
	
	if(UserOperation.GetSetupData.bUnit == 1)
	{
		value_expand = VALUE_EXPAND_105;
	}
	
	switch(btn_value)
	{
		case KBD_INPUT_0:
		case KBD_INPUT_1:
		case KBD_INPUT_2:
		case KBD_INPUT_3:
		case KBD_INPUT_4:
		case KBD_INPUT_5:
		case KBD_INPUT_6:
		case KBD_INPUT_7:
		case KBD_INPUT_8:
		case KBD_INPUT_9:
			
			if((UserOperation.Modify.fStart == 1) || (UserOperation.Modify.bUnitSwitched == 1))				//�״�������ֵ���л�һ�ε�λ������ֱֵ�Ӹ���ԭֵ
			{
				UserOperation.Modify.fStart = 2;
				UserOperation.Modify.NumAfterDot = 0;
				
				UserOperation.Modify.bUnitSwitched = 0;
				
				*UserOperation.GetSetupData.pParamNAD = UserOperation.Modify.NumAfterDot;
				UserOperation.GetSetupData.ParamValue = btn_value * value_expand;
			}
			else																							//��������У�λ�������ӣ�С�����̶�6λ��Ч����
			{
				if(UserOperation.GetSetupData.bUnit == 1)
				{
					if(UserOperation.Modify.NumAfterDot == 4)
					{
						*UserOperation.GetSetupData.pParamNAD = 3;
					}
					else
					{
						*UserOperation.GetSetupData.pParamNAD = UserOperation.Modify.NumAfterDot;
					}					
					switch(UserOperation.Modify.NumAfterDot)
					{
						case 0:												//YYYX.YYYYYY
							UserOperation.GetSetupData.ParamValue = UserOperation.GetSetupData.ParamValue * 10 + btn_value * value_expand;
							break;
						
						case 1:												//YYYY.XYYYYY							
							UserOperation.Modify.NumAfterDot++;						
							UserOperation.GetSetupData.ParamValue = UserOperation.GetSetupData.ParamValue + btn_value * value_expand / 10;
							break;
						
						case 2:												//YYYY.YXYYYY
							UserOperation.Modify.NumAfterDot++;
							UserOperation.GetSetupData.ParamValue = UserOperation.GetSetupData.ParamValue + btn_value * value_expand / 100;
							break;
						
						case 3:						
							UserOperation.Modify.NumAfterDot++;
							UserOperation.GetSetupData.ParamValue = UserOperation.GetSetupData.ParamValue + btn_value * value_expand / 1000;
							break;
						
						default:
							break;
					}					
				}
				else
				{					
					UserOperation.Modify.NumAfterDot = 4;
					
					*UserOperation.GetSetupData.pParamNAD = 3;
					UserOperation.GetSetupData.ParamValue = UserOperation.GetSetupData.ParamValue * 10 + btn_value * value_expand;
				}
			}
			
			break;
			
		case KBD_INPUT_BACKSPACE:
			
			if(UserOperation.Modify.fStart == 1)
			{
				UserOperation.Modify.fStart = 2;
				if(UserOperation.GetSetupData.bUnit == 1)
				{
					UserOperation.Modify.NumAfterDot = 4;
					
					*UserOperation.GetSetupData.pParamNAD = 3;
				}
			}
			
			if(UserOperation.GetSetupData.bUnit == 1)
			{
				if(UserOperation.Modify.NumAfterDot)
				{				
					UserOperation.Modify.NumAfterDot--;
				}
				switch(UserOperation.Modify.NumAfterDot)
				{
					case 0:
						UserOperation.GetSetupData.ParamValue = (UserOperation.GetSetupData.ParamValue - UserOperation.GetSetupData.ParamValue % (value_expand * 10)) / 10;
						break;
					
					case 1:			//�迼���û�ɾ��ʱ�ĵ�Ŵ���					
						UserOperation.Modify.NumAfterDot--;														//ɾ�����
						UserOperation.GetSetupData.ParamValue = UserOperation.GetSetupData.ParamValue - UserOperation.GetSetupData.ParamValue % (value_expand  / 10 * 10);
						break;
					
					case 2:
						UserOperation.GetSetupData.ParamValue = UserOperation.GetSetupData.ParamValue - UserOperation.GetSetupData.ParamValue % (value_expand  / 100 * 10);
						break;
					
					case 3:
						UserOperation.GetSetupData.ParamValue = UserOperation.GetSetupData.ParamValue - UserOperation.GetSetupData.ParamValue % (value_expand  / 1000 * 10);
						break;
					
					default:
						break;
				}
				
				*UserOperation.GetSetupData.pParamNAD = UserOperation.Modify.NumAfterDot;
				if(*UserOperation.GetSetupData.pParamNAD)
				{
					*UserOperation.GetSetupData.pParamNAD = *UserOperation.GetSetupData.pParamNAD - 1;
				}
			}
			else
			{				
				UserOperation.Modify.NumAfterDot = 4;
				
				*UserOperation.GetSetupData.pParamNAD = 3;
				
				UserOperation.GetSetupData.ParamValue = (UserOperation.GetSetupData.ParamValue - UserOperation.GetSetupData.ParamValue % (value_expand * 10)) / 10;
			}
			
			break;
			
		case KBD_INPUT_CLEAR:
			
			UserOperation.Modify.fStart = 1;
			UserOperation.Modify.NumAfterDot = 0;
		
			*UserOperation.GetSetupData.pParamNAD = UserOperation.Modify.NumAfterDot;
			UserOperation.GetSetupData.ParamValue = 0;
		
			break;
		
		case KBD_INPUT_CODEC_INC_SLOW:
			if(UserOperation.Modify.fStart == 1)				//�״�������ֵ���л�һ�ε�λ������ֱֵ�Ӹ���ԭֵ
			{
				UserOperation.Modify.fStart = 2;
				if(UserOperation.GetSetupData.bUnit == 1)
				{
					if(*UserOperation.GetSetupData.pParamNAD)	//[V145]���޸�ֱ���ж�ָ�뵼�¸�ֵ�쳣bug
					{
						UserOperation.Modify.NumAfterDot = *UserOperation.GetSetupData.pParamNAD + 1;
					}
					else
					{
						UserOperation.Modify.NumAfterDot = 0;
					}
				}
			}
			if(UserOperation.GetSetupData.bUnit == 1)
			{
				switch(*UserOperation.GetSetupData.pParamNAD)
				{
					case 0:
						UserOperation.GetSetupData.ParamValue += ROTATE_STEP_SLOW * value_expand;
						break;
					
					case 1:
						UserOperation.GetSetupData.ParamValue += ROTATE_STEP_SLOW * value_expand / 10;
						break;
					
					case 2:
						UserOperation.GetSetupData.ParamValue += ROTATE_STEP_SLOW * value_expand / 100;
						break;
					
					case 3:
						UserOperation.GetSetupData.ParamValue += ROTATE_STEP_SLOW * value_expand / 1000;
						break;
					
					default:
						break;
				}				
			}
			else
			{				
				UserOperation.Modify.NumAfterDot = 4;
				
				*UserOperation.GetSetupData.pParamNAD = 3;
				
				UserOperation.GetSetupData.ParamValue += ROTATE_STEP_SLOW * value_expand;
			}
			
			break;
			
		case KBD_INPUT_CODEC_INC_QUICK:
			
			if(UserOperation.Modify.fStart == 1)
			{
				UserOperation.Modify.fStart = 2;
				if(UserOperation.GetSetupData.bUnit == 1)
				{
					if(*UserOperation.GetSetupData.pParamNAD)
					{
						UserOperation.Modify.NumAfterDot = *UserOperation.GetSetupData.pParamNAD + 1;
					}
					else
					{
						UserOperation.Modify.NumAfterDot = 0;
					}
				}
			}
			
			if(UserOperation.GetSetupData.bUnit == 1)
			{
				switch(*UserOperation.GetSetupData.pParamNAD)
				{
					case 0:
						UserOperation.GetSetupData.ParamValue += ROTATE_STEP_QUICK * value_expand;
						break;
					
					case 1:
						UserOperation.GetSetupData.ParamValue += ROTATE_STEP_QUICK * value_expand / 10;
						break;
					
					case 2:
						UserOperation.GetSetupData.ParamValue += ROTATE_STEP_QUICK * value_expand / 100;
						break;
					
					case 3:
						UserOperation.GetSetupData.ParamValue += ROTATE_STEP_QUICK * value_expand / 1000;
						break;
					
					default:
						break;
				}
			}
			else
			{				
				UserOperation.Modify.NumAfterDot = 4;
				
				*UserOperation.GetSetupData.pParamNAD = 3;
				
				UserOperation.GetSetupData.ParamValue += ROTATE_STEP_QUICK * value_expand;
			}
			
			break;
			
		case KBD_INPUT_CODEC_DEC_SLOW:
			
			if(UserOperation.Modify.fStart == 1)
			{
				UserOperation.Modify.fStart = 2;
				if(UserOperation.GetSetupData.bUnit == 1)
				{
					if(*UserOperation.GetSetupData.pParamNAD)
					{
						UserOperation.Modify.NumAfterDot = *UserOperation.GetSetupData.pParamNAD + 1;
					}
					else
					{
						UserOperation.Modify.NumAfterDot = 0;
					}
				}
			}
			
			if(UserOperation.GetSetupData.bUnit == 1)
			{
				switch(*UserOperation.GetSetupData.pParamNAD)
				{
					case 0:
						if(UserOperation.GetSetupData.ParamValue >= ROTATE_STEP_SLOW * value_expand)
						{
							UserOperation.GetSetupData.ParamValue -= ROTATE_STEP_SLOW * value_expand;
						}
						else
						{
							UserOperation.GetSetupData.ParamValue = 0;
						}
						break;
					
					case 1:
						if(UserOperation.GetSetupData.ParamValue >= ROTATE_STEP_SLOW * value_expand / 10)
						{
							UserOperation.GetSetupData.ParamValue -= ROTATE_STEP_SLOW * value_expand / 10;
						}
						else
						{
							UserOperation.GetSetupData.ParamValue = 0;
						}
						break;
					
					case 2:
						if(UserOperation.GetSetupData.ParamValue >= ROTATE_STEP_SLOW * value_expand / 100)
						{
							UserOperation.GetSetupData.ParamValue -= ROTATE_STEP_SLOW * value_expand / 100;
						}
						else
						{
							UserOperation.GetSetupData.ParamValue = 0;
						}
						break;
					
					case 3:
						if(UserOperation.GetSetupData.ParamValue >= ROTATE_STEP_SLOW * value_expand / 1000)
						{
							UserOperation.GetSetupData.ParamValue -= ROTATE_STEP_SLOW * value_expand / 1000;
						}
						else
						{
							UserOperation.GetSetupData.ParamValue = 0;
						}
						break;
					
					default:
						break;
				}				
			}
			else
			{
				if(UserOperation.GetSetupData.ParamValue >= ROTATE_STEP_SLOW * value_expand)
				{
					UserOperation.GetSetupData.ParamValue -= ROTATE_STEP_SLOW * value_expand;
				}
				else
				{
					UserOperation.GetSetupData.ParamValue = 0;
				}
				UserOperation.Modify.NumAfterDot = 4;
				*UserOperation.GetSetupData.pParamNAD = 3;
			}
			
			break;
			
		case KBD_INPUT_CODEC_DEC_QUICK:
			
			if(UserOperation.Modify.fStart == 1)
			{
				UserOperation.Modify.fStart = 2;
				if(UserOperation.GetSetupData.bUnit == 1)
				{
					if(*UserOperation.GetSetupData.pParamNAD)
					{
						UserOperation.Modify.NumAfterDot = *UserOperation.GetSetupData.pParamNAD + 1;
					}
					else
					{
						UserOperation.Modify.NumAfterDot = 0;
					}
				}
			}
			if(UserOperation.GetSetupData.bUnit == 1)
			{
				switch(*UserOperation.GetSetupData.pParamNAD)
				{
					case 0:
						if(UserOperation.GetSetupData.ParamValue >= ROTATE_STEP_QUICK * value_expand)
						{
							UserOperation.GetSetupData.ParamValue -= ROTATE_STEP_QUICK * value_expand;
						}
						else
						{
							UserOperation.GetSetupData.ParamValue = 0;
						}
						break;
					
					case 1:
						if(UserOperation.GetSetupData.ParamValue >= ROTATE_STEP_QUICK * value_expand / 10)
						{
							UserOperation.GetSetupData.ParamValue -= ROTATE_STEP_QUICK * value_expand / 10;
						}
						else
						{
							UserOperation.GetSetupData.ParamValue = 0;
						}
						break;
					
					case 2:
						if(UserOperation.GetSetupData.ParamValue >= ROTATE_STEP_QUICK * value_expand / 100)
						{
							UserOperation.GetSetupData.ParamValue -= ROTATE_STEP_QUICK * value_expand / 100;
						}
						else
						{
							UserOperation.GetSetupData.ParamValue = 0;
						}
						break;
					
					case 3:
						if(UserOperation.GetSetupData.ParamValue >= ROTATE_STEP_QUICK * value_expand / 1000)
						{
							UserOperation.GetSetupData.ParamValue -= ROTATE_STEP_QUICK * value_expand / 1000;
						}
						else
						{
							UserOperation.GetSetupData.ParamValue = 0;
						}
						break;
					
					default:
						break;
				}				
			}
			else
			{
				if(UserOperation.GetSetupData.ParamValue >= ROTATE_STEP_QUICK * value_expand)
				{
					UserOperation.GetSetupData.ParamValue -= ROTATE_STEP_QUICK * value_expand;
				}
				else
				{
					UserOperation.GetSetupData.ParamValue = 0;
				}
				UserOperation.Modify.NumAfterDot = 4;
				*UserOperation.GetSetupData.pParamNAD = 3;
			}
			
			break;
			
		default:
			break;
	}
	
	if(UserOperation.GetSetupData.ParamValue > UserOperation.GetSetupData.Ceiling)
	{
		UserOperation.GetSetupData.ParamValue = UserOperation.GetSetupData.Ceiling;
	}
	/*	UserOperation.SettingValue������ɺ󷵻������õĲ���	*/
	UserOperation.GetSetupData.pModeParam->Param[UserOperation.fParamType] = UserOperation.GetSetupData.ParamValue;
	
	T6.MemoryUpdateCnt = MEMORYUPDATE_UPCNT_START;
}

/********************************************************************
*	����	��	������ѯ
******************************************************************************/
void Manual_Poll(void)
{
	uint8_t keycode = 0;
	uint8_t i = 0, j = 0;
	
	keycode = bsp_GetKey();
	if(keycode != KEY_NONE)
	{
		i = (keycode - 1) / 3;																					//��ֵ
		j = (keycode - 1) % 3;																					//��״̬��0--���� 1--���� 2--����
		if(j == 0 && i < KEY_NUMBER)
		{
			/*	1.������Ʋ���	*/
			if(i == BTN_RUN)														//��ʼ����
			{		
		
				if(UserOperation.fMode == UO_MODE_SINGLE)														//[V111]���޸�����߼���SINGLEģʽ�£�RUN������Ч
				{
					if(DOState.Config & (1 << DO_TIM4))															//[V189],�޸�����߼���SINGLEģʽ�£�RUN����ֹͣ�����
					{
						DOState.Config &= ~(1 << DO_TIM4);
					}
					if(DOState.Status[DO_TIM4] != DOSTATE_STATUS_COMPLETE)
					{
						Process_COMMAND_STOP(DO_TIM4);
						//SW_CV_OUTPUT = 0;   //�ر����
				    }
					
					return;
				}
				
				if(UserOperation.fMode == UO_MODE_EXTBNC)														//[V181]���޸�����߼����޸�EXTBNC��ʼ���к�һֱ����RUN״̬���޷��л�ģʽbug
				{
					if(DOState.Status[DO_TIM4] != DOSTATE_STATUS_COMPLETE)
					{
						Output_VorC(UserOperation.bVC, 0, OUTPUT_DISABLE);
					}
					#if(PCB_VERSION == PCB_V14)
						TIM2->CR1    |= 0X01;
						T2.OutputCnt = 0;
					#endif
						
					pLEDPAUSE = LED_SN74HC240_OFF;
					DOState.Status[DO_TIM4] = DOSTATE_STATUS_COMPLETE;
					
					return;
				}
				
				UserOperation.fParamType = UO_PARAM_NONE;
				ParamEdit_RefreshPre();
				
				Led_ParamPartOff();
				
				if(DOState.Status[DO_TIM4] == DOSTATE_STATUS_COMPLETE)
				{
					if(T6.MemoryUpdateCnt)
					{
						T6.MemoryUpdateCnt = T6_VALUE_MINUTE - T6_VALUE_SECOND * 20;							//�����б䶯�Ҵ������������´洢,20s�����϶ʱ���������ȴ���RUN����
					}
					
					UO_Update(UPDATE_ALL);
					UserOperation.Update = UO_UPDATE_VALID;
					
					log_info("SW_CV_OUTPUT = 1\r\n");
					SW_CV_OUTPUT = 1;   //�����
					Delay_ms(15);		//�ȴ��̵����ȶ��պ�
				
					
					Process_COMMAND_START(DO_TIM4);
					//SW_CV_OUTPUT = 1;   //�����
					
				}
				else if(DOState.Status[DO_TIM4] != DOSTATE_STATUS_COMPLETE)
				{
					Output_VorC(UserOperation.bVC, 0, OUTPUT_DISABLE);
										
				#if(PCB_VERSION == PCB_V14)
					TIM2->CR1    |= 0X01;
					T2.OutputCnt = 0;
				#endif
					
					pLEDPAUSE = LED_SN74HC240_OFF;

					Process_COMMAND_STOP(DO_TIM4);
					//SW_CV_OUTPUT = 0;   //�ر����
				}
			}
			else if(i == BTN_PAUSE)															//��ͣ�̼�
			{
				UserOperation.fParamType = UO_PARAM_NONE;
				ParamEdit_RefreshPre();
				
				Led_ParamPartOff();			
				
				if(DOState.Status[DO_TIM4] == DOSTATE_STATUS_RUNNING)
				{
				#if(PCB_VERSION == PCB_V14)
					TIM2->CR1    &= ~0X01;
				#endif
					EXTI->IMR    &= ~(1<<0);
					
					Output_VorC(UserOperation.bVC, 0, OUTPUT_DISABLE);
					//SW_CV_OUTPUT = 0;   //�ر����		
					Process_COMMAND_PAUSE(DO_TIM4);					
					pLEDPAUSE = LED_SN74HC240_ON;
				}
				else if(DOState.Status[DO_TIM4] == DOSTATE_STATUS_PAUSE)
				{					
				#if(PCB_VERSION == PCB_V14)
					TIM2->CR1    |= 0X01;
				#endif
					EXTI->IMR    |= 1<<0;
					
					
					//Delay_ms(15);		//�ȴ��̵����պ�
					//SW_CV_OUTPUT = 1;   //�����
					Process_COMMAND_CONTINUE(DO_TIM4);					
					pLEDPAUSE = LED_SN74HC240_OFF;
				}
				else if(DOState.Status[DO_TIM4] == DOSTATE_STATUS_COMPLETE)
				{
					;
				}
			}
			else if(i == BTN_PHASE)																				//��ת�������
			{
				if(UserOperation.bPhase == UO_PHASE_UNIPHASE)
				{
					UserOperation.bPhase = UO_PHASE_BIPHASE;			//�л�Ϊ������
					Voltage_Ploar_Flag = 1;
					Current_Ploar_Flag = 1;
				}
				else
				{
					UserOperation.bPhase = UO_PHASE_UNIPHASE;			//�л�Ϊ������
					
					Voltage_Ploar_Flag = 0;
					Current_Ploar_Flag = 0;
					
				}
				
				UserOperation.fParamType = UO_PARAM_NONE;
				ParamEdit_RefreshPre();
				
				Led_ParamPartOff();

				T6.MemoryUpdateCnt = MEMORYUPDATE_UPCNT_START;
			}
			else if(i == BTN_SINGLETRIGGER)													//�������
			{
				UserOperation.fParamType = UO_PARAM_NONE;
				ParamEdit_RefreshPre();
				
				Led_ParamPartOff();
				
				if(UserOperation.fMode == UO_MODE_SINGLE)														//�ݶ���RUN�������ƴ���ʽ������������������޶�����
				{
					UO_Update(UPDATE_ALL);
					UserOperation.Update = UO_UPDATE_VALID;
					
					log_info("SW_CV_OUTPUT = 1\r\n");
					SW_CV_OUTPUT = 1;   //�����
					Delay_ms(15);	//�ȴ��̵����պ�
					//1:����������ö�ʱ����2����ʱ�������趨ʱ����ر����
					Process_COMMAND_START(DO_TIM4);
					LedShortOn.fSinggleTrigger = LEDSHORTON_BEGIN;
				}
			}
			
			/*	2.������Բ���	*/
			else if(i == BTN_VC)																				//��ѹ/�����л�����
			{
				if(DOState.Config & (1 << DO_TIM4))																//[V189],�޸��л�ģʽVCʱ��������RUN״̬�������Զ�ֹͣ��bug
				{
					DOState.Config &= ~(1 << DO_TIM4);
				}
				if(DOState.Status[DO_TIM4] != DOSTATE_STATUS_COMPLETE)
				{
					Process_COMMAND_STOP(DO_TIM4);
				}
				
				UserOperation.bVC = !UserOperation.bVC;
				
				LedShortOn.fVC = LEDSHORTON_BEGIN;
				
				UserOperation.fParamType = UO_PARAM_NONE;
				Led_ModePartOff();
				
				Led_ParamPartOff();
				
				UserOperation.Modify.fStart = 0;
				UserOperation.Modify.NumAfterDot = 0;
				
				if(UserOperation.bVC == SELECT_VC_V)
				{
					Switch.ModeCur &= ~(1 << MODE_BIT_VC);    //MODE_BIT_VC		0
					Switch.ModeCur |= SELECT_VC_V << MODE_BIT_VC;  //SELECT_VC_C		0
					
					UserOperation.fMode_VPre = UserOperation.fMode_CPre;										//[V111],�����߼�����Ϊ�л����ʱģʽ����
					UserOperation.fMode = UserOperation.fMode_VPre;
					
					switch(UserOperation.fMode)
					{
						case UO_MODE_SINGLE:
							UserOperation.fUnitCur	= UserOperation.V_ModeSingle.fUnit;
							pLEDSINGLE				= LED_SN74HC240_ON;
							break;
						
						case UO_MODE_FREERUN:
							UserOperation.fUnitCur	= UserOperation.V_ModeFreeRun.fUnit;
							pLEDFREERUN				= LED_SN74HC240_ON;
							break;
						
						case UO_MODE_TRAIN:
							UserOperation.fUnitCur	= UserOperation.V_ModeTrain.fUnit;
							pLEDTRAIN				= LED_SN74HC240_ON;
							break;
						
						case UO_MODE_EXTBNC:
							UserOperation.fUnitCur	= UserOperation.V_ModeExtBnc.fUnit;
							pLEDEXTBNC				= LED_SN74HC240_ON;
							break;
						
						default:
							break;
					}
				}
				else
				{
					Switch.ModeCur &= ~(1 << MODE_BIT_VC);
					Switch.ModeCur &= SELECT_VC_C << MODE_BIT_VC;
					
					UserOperation.fMode_CPre = UserOperation.fMode_VPre;										//[V111],�����߼�����Ϊ�л����ʱģʽ����
					UserOperation.fMode = UserOperation.fMode_CPre;
					
					switch(UserOperation.fMode)
					{
						case UO_MODE_SINGLE:
							UserOperation.fUnitCur	= UserOperation.C_ModeSingle.fUnit;
							pLEDSINGLE				= LED_SN74HC240_ON;
							break;
						
						case UO_MODE_FREERUN:
							UserOperation.fUnitCur	= UserOperation.C_ModeFreeRun.fUnit;
							pLEDFREERUN				= LED_SN74HC240_ON;
							break;
						
						case UO_MODE_TRAIN:
							UserOperation.fUnitCur	= UserOperation.C_ModeTrain.fUnit;
							pLEDTRAIN				= LED_SN74HC240_ON;
							break;
						
						case UO_MODE_EXTBNC:
							UserOperation.fUnitCur	= UserOperation.C_ModeExtBnc.fUnit;
							pLEDEXTBNC				= LED_SN74HC240_ON;
							break;
						
						default:
							break;
					}
				}
				
			  if((Switch.ModeCur & (1 << MODE_BIT_VC)) != (Switch.ModePre & (1 << MODE_BIT_VC)))   //MODE_BIT_VC  0
               {
				Switch_Ctrl(UO_PHASE_UNIPHASE);	//�л�����
			   }

				UI.fFlush = FLUSH_START;
				
				T6.MemoryUpdateCnt = MEMORYUPDATE_UPCNT_START;
			}
			else if(i == BTN_UNITSWITCH && UserOperation.fParamType != UO_PARAM_NONE)							//��λ�л�����
			{
				LedShortOn.fUnitSwitch = LEDSHORTON_BEGIN;
				
				SettingInfo_Get(PARAMTYPECHANGE_N, UNITSWITCH_Y);
				
				T6.MemoryUpdateCnt = MEMORYUPDATE_UPCNT_START;
			}
			
			/*	3.ģʽ���Ʋ���	*/
			else if(i == BTN_SINGLE && DOState.Status[DO_TIM4] == DOSTATE_STATUS_COMPLETE)						//������̲������޸����ģʽ
			{				
				Led_ParamPartOff();
				
				if(UserOperation.fMode != UO_MODE_SINGLE)
				{
					Led_ModePartOff();
					
					pLEDSINGLE			= LED_SN74HC240_ON;
					
					UserOperation.fMode = UO_MODE_SINGLE;
					
					if(UserOperation.bVC == SELECT_VC_V)
					{
						UserOperation.fMode_VPre = UserOperation.fMode;
						UserOperation.fUnitCur = UserOperation.V_ModeSingle.fUnit;
					}
					else
					{
						UserOperation.fMode_CPre = UserOperation.fMode;
						UserOperation.fUnitCur = UserOperation.C_ModeSingle.fUnit;
					}

					UI.fFlush = FLUSH_START;
				}
				
				UserOperation.fParamType = UO_PARAM_NONE;
				ParamEdit_RefreshPre();
				
				T6.MemoryUpdateCnt = MEMORYUPDATE_UPCNT_START;
			}
			else if(i == BTN_TRAIN && DOState.Status[DO_TIM4] == DOSTATE_STATUS_COMPLETE)
			{				
				Led_ParamPartOff();
				
				if(UserOperation.fMode != UO_MODE_TRAIN)
				{
					Led_ModePartOff();
					
					pLEDTRAIN			= LED_SN74HC240_ON;
					
					UserOperation.fMode = UO_MODE_TRAIN;
										
					if(UserOperation.bVC == SELECT_VC_V)
					{
						UserOperation.fMode_VPre = UserOperation.fMode;
						UserOperation.fUnitCur = UserOperation.V_ModeTrain.fUnit;
					}
					else
					{
						UserOperation.fMode_CPre = UserOperation.fMode;
						UserOperation.fUnitCur = UserOperation.C_ModeTrain.fUnit;
					}

					UI.fFlush = FLUSH_START;
				}
				
				UserOperation.fParamType = UO_PARAM_NONE;
				ParamEdit_RefreshPre();
				
				T6.MemoryUpdateCnt = MEMORYUPDATE_UPCNT_START;
			}
			else if(i == BTN_FREERUN && DOState.Status[DO_TIM4] == DOSTATE_STATUS_COMPLETE)
			{				
				Led_ParamPartOff();
				
				if(UserOperation.fMode != UO_MODE_FREERUN)
				{
					Led_ModePartOff();
					
					pLEDFREERUN			= LED_SN74HC240_ON;
					
					UserOperation.fMode = UO_MODE_FREERUN;
					
					if(UserOperation.bVC == SELECT_VC_V)
					{
						UserOperation.fMode_VPre = UserOperation.fMode;
						UserOperation.fUnitCur = UserOperation.V_ModeFreeRun.fUnit;
					}
					else
					{
						UserOperation.fMode_CPre = UserOperation.fMode;
						UserOperation.fUnitCur = UserOperation.C_ModeFreeRun.fUnit;
					}

					UI.fFlush = FLUSH_START;
				}
				
				UserOperation.fParamType = UO_PARAM_NONE;
				ParamEdit_RefreshPre();
				
				T6.MemoryUpdateCnt = MEMORYUPDATE_UPCNT_START;
			}
			else if(i == BTN_EXTBNC && DOState.Status[DO_TIM4] == DOSTATE_STATUS_COMPLETE)
			{				
				Led_ParamPartOff();
				
				if(UserOperation.fMode != UO_MODE_EXTBNC)
				{
					Led_ModePartOff();
					
					pLEDEXTBNC = LED_SN74HC240_ON;
					
					UserOperation.fMode = UO_MODE_EXTBNC;
										
					if(UserOperation.bVC == SELECT_VC_V)
					{
						UserOperation.fMode_VPre = UserOperation.fMode;
						UserOperation.fUnitCur = UserOperation.V_ModeExtBnc.fUnit;
					}
					else
					{
						UserOperation.fMode_CPre = UserOperation.fMode;
						UserOperation.fUnitCur = UserOperation.C_ModeExtBnc.fUnit;
					}

					UI.fFlush = FLUSH_START;
				}
				
				UserOperation.fParamType = UO_PARAM_NONE;
				ParamEdit_RefreshPre();
				
				T6.MemoryUpdateCnt = MEMORYUPDATE_UPCNT_START;
			}
			
			/*	4.�������Ͳ���	*/
			else if(i == BTN_PULSE)
			{								
				if(UserOperation.fMode == UO_MODE_SINGLE || UserOperation.fMode == UO_MODE_FREERUN || UserOperation.fMode == UO_MODE_TRAIN)
				{
					Led_ParamPartOff();
					
					if(UserOperation.fParamType != UO_PARAM_PULSE)
					{
						pLEDPULSE = LED_SN74HC240_ON;
						
						if(UserOperation.Update == UO_UPDATE_VALID)
						{							
							UserOperation.Update = UO_UPDATE_SETTING;
						}

						UserOperation.fParamType = UO_PARAM_PULSE;
						
						SettingInfo_Get(PARAMTYPECHANGE_Y, UNITSWITCH_N);
						
						UserOperation.Modify.fStart = 1;
						UserOperation.Modify.bUnitSwitched = 0;
						UserOperation.Modify.NumAfterDot = 0;
					}
					else
					{
						if(UserOperation.Update == UO_UPDATE_SETTING)
						{
							UserOperation.Update = UO_UPDATE_INVALID;
					
							UO_Update(UPDATE_ALL);

							UserOperation.Update = UO_UPDATE_VALID;
						}
						
						UserOperation.fParamType = UO_PARAM_NONE;
						
						UserOperation.Modify.fStart = 0;
						UserOperation.Modify.NumAfterDot = 0;
					}
					
					ParamEdit_RefreshPre();
				}
			}
			else if(i == BTN_AMPL)
			{
				Led_ParamPartOff();
				
				if(UserOperation.fParamType != UO_PARAM_AMPL)
				{
					pLEDAMPL = LED_SN74HC240_ON;
					
					if(UserOperation.Update == UO_UPDATE_VALID)
					{							
					   UserOperation.Update = UO_UPDATE_SETTING;
					}
					
					UserOperation.fParamType = UO_PARAM_AMPL;
					
					SettingInfo_Get(PARAMTYPECHANGE_Y, UNITSWITCH_N);
					
					UserOperation.Modify.fStart = 1;
					UserOperation.Modify.bUnitSwitched = 0;
					UserOperation.Modify.NumAfterDot = 0;
				}
				else
				{					
					if(UserOperation.Update == UO_UPDATE_SETTING)
					{
						UserOperation.Update = UO_UPDATE_INVALID;
					
						UO_Update(UPDATE_ALL);

						UserOperation.Update = UO_UPDATE_VALID;
					}
						
					UserOperation.fParamType = UO_PARAM_NONE;
					
					UserOperation.Modify.fStart = 0;
					UserOperation.Modify.NumAfterDot = 0;
				}
				
				ParamEdit_RefreshPre();
			}
			else if(i == BTN_FREQ)
			{				
				if(UserOperation.fMode == UO_MODE_FREERUN || UserOperation.fMode == UO_MODE_TRAIN)
				{
					Led_ParamPartOff();
					
					if(UserOperation.fParamType != UO_PARAM_FREQ)
					{
						pLEDFREQUENCY = LED_SN74HC240_ON;
						
						if(UserOperation.Update == UO_UPDATE_VALID)
						{							
							UserOperation.Update = UO_UPDATE_SETTING;
						}
						
						UserOperation.fParamType = UO_PARAM_FREQ;
						
						SettingInfo_Get(PARAMTYPECHANGE_Y, UNITSWITCH_N);
						
						UserOperation.Modify.fStart = 1;
						UserOperation.Modify.bUnitSwitched = 0;
						UserOperation.Modify.NumAfterDot = 0;						
					}
					else
					{
						if(UserOperation.Update == UO_UPDATE_SETTING)
						{
							UserOperation.Update = UO_UPDATE_INVALID;
					
							UO_Update(UPDATE_ALL);

							UserOperation.Update = UO_UPDATE_VALID;
						}
					
						UserOperation.fParamType = UO_PARAM_NONE;
						
						UserOperation.Modify.fStart = 0;
						UserOperation.Modify.NumAfterDot = 0;
					}
					
					ParamEdit_RefreshPre();
				}
			}
			else if(i == BTN_DURATION)
			{				
				if(UserOperation.fMode == UO_MODE_TRAIN)
				{
					Led_ParamPartOff();
					
					if(UserOperation.fParamType != UO_PARAM_DURATION)
					{
						pLEDDURATION = LED_SN74HC240_ON;
						
						if(UserOperation.Update == UO_UPDATE_VALID)
						{							
							UserOperation.Update = UO_UPDATE_SETTING;
						}
						
						UserOperation.fParamType = UO_PARAM_DURATION;
						
						SettingInfo_Get(PARAMTYPECHANGE_Y, UNITSWITCH_N);
						
						UserOperation.Modify.fStart = 1;
						UserOperation.Modify.bUnitSwitched = 0;
						UserOperation.Modify.NumAfterDot = 0;
					}
					else
					{
						if(UserOperation.Update == UO_UPDATE_SETTING)
						{
							UserOperation.Update = UO_UPDATE_INVALID;
							
							UO_Update(UPDATE_ALL);

							UserOperation.Update = UO_UPDATE_VALID;
						}
						
						UserOperation.fParamType = UO_PARAM_NONE;
						
						UserOperation.Modify.fStart = 0;
						UserOperation.Modify.NumAfterDot = 0;
					}
					
					ParamEdit_RefreshPre();
				}
			}
			
			/*	5.��ֵ���벿��	*/
			else if(i == BTN_0)
			{
				SettingInfo_Modify(KBD_INPUT_0);
			}
			else if(i == BTN_1)
			{
				SettingInfo_Modify(KBD_INPUT_1);
			}
			else if(i == BTN_2)
			{
				SettingInfo_Modify(KBD_INPUT_2);
			}
			else if(i == BTN_3)
			{
				SettingInfo_Modify(KBD_INPUT_3);
			}
			else if(i == BTN_4)
			{
				SettingInfo_Modify(KBD_INPUT_4);
			}
			else if(i == BTN_5)
			{
				SettingInfo_Modify(KBD_INPUT_5);
			}
			else if(i == BTN_6)
			{
				SettingInfo_Modify(KBD_INPUT_6);
			}
			else if(i == BTN_7)
			{
				SettingInfo_Modify(KBD_INPUT_7);
			}
			else if(i == BTN_8)
			{
				SettingInfo_Modify(KBD_INPUT_8);
			}
			else if(i == BTN_9)
			{
				SettingInfo_Modify(KBD_INPUT_9);
			}
			else if(i == BTN_BACKSPACE)
			{
				SettingInfo_Modify(KBD_INPUT_BACKSPACE);
			}
			else if(i == BTN_CLEAR)
			{
				SettingInfo_Modify(KBD_INPUT_CLEAR);
			}
			else if(i == BTN_DOT)
			{
				if(UserOperation.Modify.NumAfterDot == 0)
				{
					UserOperation.Modify.NumAfterDot = 1;
				}
			}
			else if(i == BTN_ENTER)
			{
				Led_ParamPartOff();
				
				UserOperation.fParamType = UO_PARAM_NONE;
				ParamEdit_RefreshPre();
				
				UserOperation.Modify.fStart = 0;
				UserOperation.Modify.NumAfterDot = 0;
				
				if(UserOperation.Update == UO_UPDATE_SETTING)
				{
					UserOperation.Update = UO_UPDATE_INVALID;
					
					UO_Update(UPDATE_ALL);

					UserOperation.Update = UO_UPDATE_VALID;
				}
			}
		}
	}
}

