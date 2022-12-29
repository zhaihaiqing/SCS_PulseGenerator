
#include "main.h"

#define STATUSBAR_CONFIG	(1 << 0)
#define STATUSBAR_RUN		(1 << 1)
#define STATUSBAR_COMPLETE	(1 << 2)
#define STATUSBAR_PAUSE		(1 << 3)
#define STATUSBAR_CONTINUE	(1 << 7)
static uint8_t  statusbar	= 0;																//�������н���

/********************************************************************
*	����	��	����״̬������
******************************************************************************/
static void StatusBar_Progress(uint8_t status)
{
	if(status == STATUSBAR_CONFIG)
	{
		statusbar |= STATUSBAR_CONFIG;
	}
	else if(status == STATUSBAR_RUN)
	{
		statusbar &= ~(STATUSBAR_PAUSE | STATUSBAR_COMPLETE);
		statusbar |= STATUSBAR_RUN;
	}
	else if(status == STATUSBAR_PAUSE)
	{
		statusbar &= ~STATUSBAR_RUN;
		statusbar |= STATUSBAR_PAUSE;
	}
	else if(status == STATUSBAR_CONTINUE)
	{
		statusbar &= ~STATUSBAR_PAUSE;
		statusbar |= STATUSBAR_RUN;
	}
	else if(status == STATUSBAR_COMPLETE)
	{
		statusbar &= ~(STATUSBAR_RUN | STATUSBAR_PAUSE);
		statusbar |= STATUSBAR_COMPLETE;
	}
}

static uint32_t temp_runcnt = 0;																//����ʱ���ݴ�
static uint16_t Tbl_Temp_CCRx[8] = {0};
static uint16_t Tbl_Temp_CNT[8]  = {0};

__IO uint32_t tim5_arr=0;
__IO uint32_t train_count=0;	//TRAINģʽ��������
__IO uint32_t train_acount=0;	//TRAINģʽ��Ҫ���������������磺10S��Freq=10Hz��������=10*10=100����ע�⣬ĩβ����1����ȥ
/********************************************************************
*	����	��	�������
******************************************************************************/
void Process_COMMAND_START(void)
{
	uint8_t i = 0;
	
	if(UserOperation.fMode == UO_MODE_SINGLE)	//����ģʽ
	{
		
		uint64_t pulse=0;
		
		//����tim5_arrֵ
		if(UserOperation.bVC == SELECT_VC_V)
		{
			pulse = UserOperation.V_ModeSingle.Param[UO_PARAM_PULSE];	//��λΪns
		}
		else if(UserOperation.bVC == SELECT_VC_C)
		{
			pulse = UserOperation.C_ModeSingle.Param[UO_PARAM_PULSE];	//��λΪns
		}
		tim5_arr = pulse/500-1;				//����tim2��װ��ֵ��tim2��������,(TIM2��ƵֵΪ42��2Mʱ�ӣ�û��һ������ʱ��Ϊ0.5us����500ns)
		
		SW_CV_OUTPUT = 1;   //�����
		pLEDOUTPUT = LED_DIRECTLY_ON;
		Delay_ms(15);		//�ȴ��̵����պ�
		log_info("SINGLETRIGGER Mode,SW_CV_OUTPUT = 1,Wave_type=%d,tim5_arr=%d\r\n",Wave_type,tim5_arr);
		
		DOState.Status[DO_TIM4] = DOSTATE_STATUS_RUNNING;
		Enable_Timer5(tim5_arr);
	}
	
	else if(UserOperation.fMode == UO_MODE_FREERUN)				//����ģʽ�£�Ƶ�ʷ�Χ1-30KHz����Ӧ����Ϊ1000ms-33.333us��
	{
		uint64_t pulse=0,freq=0;
		
		if(UserOperation.bVC == SELECT_VC_V)
		{
			pulse = UserOperation.V_ModeFreeRun.Param[UO_PARAM_PULSE];	//��λΪns
			freq = UserOperation.V_ModeFreeRun.Param[UO_PARAM_FREQ];
		}
		else if(UserOperation.bVC == SELECT_VC_C)
		{
			pulse = UserOperation.C_ModeFreeRun.Param[UO_PARAM_PULSE];	//��λΪns
			freq = UserOperation.C_ModeFreeRun.Param[UO_PARAM_FREQ];
		}
		
		tim5_arr = pulse/500-1;
		
		log_info("pulse=%lldns,freq=%lldmhz,tim5_arr=%d\r\n",pulse,freq,tim5_arr);
		
		//���㶨ʱ��2������
		{
			uint32_t tim2_arr=0,tim2_psc=0;
			double T=0;
			tim2_psc=41;
			
			freq = freq/1000;
			
			T = 1000000000000.0/freq;	//ns����������
			tim2_arr = T/500-1;			//����tim2��װ��ֵ��tim2��������,(TIM2��ƵֵΪ42��2Mʱ�ӣ�û��һ������ʱ��Ϊ0.5us����500ns)
			log_info("T=%lf,tim2_arr=%d\r\n",T,tim2_arr);
			
			if(tim5_arr>tim2_arr)
			{
				tim5_arr = tim2_arr;
			}
			
			SW_CV_OUTPUT = 1;   //�����
			pLEDOUTPUT = LED_DIRECTLY_ON;
			Delay_ms(15);		//�ȴ��̵����պ�
			log_info("UO_MODE_FREERUN Mode,SW_CV_OUTPUT = 1,Wave_type=%d,tim5_arr=%d\r\n",Wave_type,tim5_arr);
			
			Timer2_Init(tim2_arr,tim2_psc);
			DOState.Status[DO_TIM4] = DOSTATE_STATUS_RUNNING;
			TIM_Cmd(TIM2,ENABLE);
		}		
	}
	else if(UserOperation.fMode == UO_MODE_TRAIN)
	{
		uint64_t pulse=0,freq=0,duration=0;
		
		if(UserOperation.bVC == SELECT_VC_V)
		{
			pulse 	 = UserOperation.V_ModeTrain.Param[UO_PARAM_PULSE];	//��λΪns
			freq 	 = UserOperation.V_ModeTrain.Param[UO_PARAM_FREQ];
			duration = UserOperation.V_ModeTrain.Param[UO_PARAM_DURATION];
		}
		else if(UserOperation.bVC == SELECT_VC_C)
		{
			pulse 	 = UserOperation.C_ModeTrain.Param[UO_PARAM_PULSE];	//��λΪns
			freq 	 = UserOperation.C_ModeTrain.Param[UO_PARAM_FREQ];
			duration = UserOperation.C_ModeTrain.Param[UO_PARAM_DURATION];//��λus
		}
		
		tim5_arr = pulse/500-1;
		
		log_info("pulse=%lldns,freq=%lldmhz,tim5_arr=%d,duration=%lld\r\n",pulse,freq,tim5_arr,duration);
		
		//���㶨ʱ��2������
		{
			uint32_t tim2_arr=0,tim2_psc=0;
			double T=0;
			tim2_psc=41;
			
			freq = freq/1000;
			
			T = 1000000000000.0/freq;	//ns
			tim2_arr = T/500-1;					//����tim2��װ��ֵ��tim2��������,(TIM2��ƵֵΪ42��2Mʱ�ӣ�û��һ������ʱ��Ϊ0.5us����500ns)
			
			if(tim5_arr>tim2_arr)
			{
				tim5_arr = tim2_arr;
			}
			
			train_acount= duration*1000 / T;
			
			log_info("T=%lf,tim2_arr=%d,train_acount=%d\r\n",T,tim2_arr,train_acount);
			
			SW_CV_OUTPUT = 1;   //�����
			pLEDOUTPUT = LED_DIRECTLY_ON;
			Delay_ms(15);		//�ȴ��̵����պ�
			log_info("UO_MODE_TRAIN Mode,SW_CV_OUTPUT = 1,Wave_type=%d,tim5_arr=%d\r\n",Wave_type,tim5_arr);
			
			Timer2_Init(tim2_arr,tim2_psc);
			DOState.Status[DO_TIM4] = DOSTATE_STATUS_RUNNING;
			TIM_Cmd(TIM2,ENABLE);
			
		}
	}
}

void Process_COMMAND_STOP(void)
{
	Disable_Timer5();
	TIM_Cmd(TIM2,DISABLE);
	TIM2->CNT = 0;
	
	DOState.Status[DO_TIM4] = DOSTATE_STATUS_COMPLETE;
	
	AD5542_Output(DA_CHNL_ALL, 0);
	
	SW_CV_OUTPUT = 0;//�ر����
	pLEDOUTPUT = LED_DIRECTLY_OFF;
	TIM5_IRQ_Count = 0;	//���������
}

void Process_COMMAND_PAUSE(void)
{
	//��ͣ��ʱ��2
	pLEDOUTPUT = LED_DIRECTLY_OFF;
	TIM_Cmd(TIM2,DISABLE);
	DOState.Status[DO_TIM4] = DOSTATE_STATUS_PAUSE;
}

void Process_COMMAND_CONTINUE(void)
{
	pLEDOUTPUT = LED_DIRECTLY_ON;
	TIM_Cmd(TIM2,ENABLE);
	DOState.Status[DO_TIM4] = DOSTATE_STATUS_RUNNING;

}

/********************************************************************
*	����	��	���ڷ�������
******************************************************************************/
static void Protocol_Usart1_Tx(uint8_t command)
{
	uint8_t  i = 0, j = 0, k = 0, index = 0;
	uint16_t crcval = 0;
	
	U1.Ttbl[ADDR_OFFSET_HEAD]    = PROTOCOL_FIXED_HEAD;		
	U1.Ttbl[ADDR_OFFSET_VERSION] = PROTOCOL_FIXED_VERSION;										//��
	U1.Ttbl[ADDR_OFFSET_COMMAND] = command | 0x80;												//��
	U1.Ttbl[ADDR_OFFSET_RESERVE] = 0X00;
	
	index = ADDR_OFFSET_RESERVE;
	index++;
	
	if(command == COMMAND_QUERY)
	{
		for(i=0; i<8; i++)
		{
			if(DOState.Status[i] == DOSTATE_STATUS_RUNNING)										//����
			{
				j |= 1 << i;
				StatusBar_Progress(STATUSBAR_RUN);
				break;
			}
			else if(DOState.Status[i] == DOSTATE_STATUS_COMPLETE)								//���
			{
				k++;
				if(k == 8)
				{
					StatusBar_Progress(STATUSBAR_COMPLETE);
					T6.RunCnt = 0;
				}
			}
			else if(DOState.Status[i] == DOSTATE_STATUS_PAUSE)									//��ͣ
			{
				StatusBar_Progress(STATUSBAR_PAUSE);
				break;
			}
		}
		U1.Ttbl[index++] = (uint8_t)j;															//��
		
		if(temp_runcnt != 0)
		{
			U1.Ttbl[index++] = temp_runcnt / 256 / 256 / 256;
			U1.Ttbl[index++] = temp_runcnt / 256 / 256 % 256;
			U1.Ttbl[index++] = temp_runcnt / 256 % 256;
			U1.Ttbl[index++] = temp_runcnt % 256;
		}
		else
		{
			U1.Ttbl[index++] = T6.RunCnt / 256 / 256 / 256;
			U1.Ttbl[index++] = T6.RunCnt / 256 / 256 % 256;
			U1.Ttbl[index++] = T6.RunCnt / 256 % 256;
			U1.Ttbl[index++] = T6.RunCnt % 256;
		}		
		
		U1.Ttbl[index++] = statusbar;															//���н���
	}
	
	crcval     			= Crc_16bit_Calculate((uint8_t *)(&U1.Rtbl[1]), index-1);
	U1.Ttbl[index++] 	= crcval / 256;
	U1.Ttbl[index++] 	= crcval % 256;	
	U1.Ttbl[index++] 	= PROTOCOL_FIXED_END;
	
	U1.Ttbl[ADDR_OFFSET_NUM_H]	= (index - 4) / 256;
	U1.Ttbl[ADDR_OFFSET_NUM_L]	= (index - 4) % 256;
	
	Uart1_Tx(U1.Ttbl, index);
}

sPulseConf_t PulseConf = {(ePulseDO_t)0, (ePulseType_t)0, {0}};
/********************************************************************
*	����	��	�����ڽ�������
******************************************************************************/
void Protocol_Usart1_Rx_Poll(void)
{
	uint8_t rcvstep = 0;
	uint16_t datasize = 0, rcvnum = 0, index = 0, i = 0, j = 0;
	eRtn_t rtn = RTN_NORMAL;
	
	if(U1.Rflag & 0x8000)
	{
		for(i = 0; i < (U1.Rflag&0X3FFF); i++)
		{
			rcvstep = 0;
			switch(rcvstep)
			{
				case 0:
					
					if(U1.Rtbl[i] == PROTOCOL_FIXED_HEAD)
					{
						rcvstep = 1;
						rcvnum = i;																//����case 1
					}
					else
					{
						break;
					}
					
				case 1:
					
					datasize = U1.Rtbl[rcvnum+1]*256 + U1.Rtbl[rcvnum+2];
					if(U1.Rtbl[rcvnum+3+datasize] == PROTOCOL_FIXED_END)
					{
						rcvstep = 2;															//����case 2
					}
					else
					{
						rcvstep = 0;
						break;
					}
					
				case 2:
					
					rcvstep = 0;																//һ�����ݿ�����ʶ��������
				
					if(Crc_16bit_Calculate((uint8_t *)(&U1.Rtbl[rcvnum+ADDR_OFFSET_COMMAND]), datasize-3) == U1.Rtbl[rcvnum+ADDR_OFFSET_COMMAND+datasize-3]*256 + U1.Rtbl[rcvnum+ADDR_OFFSET_COMMAND+datasize-2])
					{
						i = rcvnum + 3 + datasize;												//�������ܻ�����Ч���ݣ�����ֱ������ѭ��
						
						switch(U1.Rtbl[rcvnum+ADDR_OFFSET_COMMAND])
						{
							case COMMAND_START:
								
								Protocol_Usart1_Tx(COMMAND_START);
							
								Process_COMMAND_START();

								StatusBar_Progress(STATUSBAR_RUN);
							
								break;
							
							case COMMAND_PAUSE:
								
								if(U1.Rtbl[rcvnum+ADDR_OFFSET_CONTENT_START] == 0x00)
								{
									Process_COMMAND_PAUSE();
									
									StatusBar_Progress(STATUSBAR_PAUSE);
								}
								else if(U1.Rtbl[rcvnum+ADDR_OFFSET_CONTENT_START] == 0x01)
								{
									Process_COMMAND_CONTINUE();
									
									StatusBar_Progress(STATUSBAR_CONTINUE);
								}
								
								Protocol_Usart1_Tx(COMMAND_PAUSE);
								
								break;
							
							case COMMAND_STOP:
								
								Process_COMMAND_STOP();
							
								Protocol_Usart1_Tx(COMMAND_STOP);
							
								StatusBar_Progress(STATUSBAR_COMPLETE);
							
								break;
							
							case COMMAND_ALARM:
								
//								T6.AlarmValue = U1.Rtbl[rcvnum+ADDR_OFFSET_CONTENT_START] * 256 + U1.Rtbl[rcvnum+ADDR_OFFSET_CONTENT_START+1];
								Protocol_Usart1_Tx(COMMAND_ALARM);
							
								break;
							
							case COMMAND_QUERY:
								
								Protocol_Usart1_Tx(COMMAND_QUERY);
							
								break;
							
							case COMMAND_CONFIG:
								
								INTX_DISABLE();													//�ر��жϣ��漰PWM�ж���ز���
								
								index = rcvnum + ADDR_OFFSET_CONTENT_START;
							
//								PulseConf.Channel = (ePulseDO_t)(U1.Rtbl[index] - 1);			//ͨ���ţ���λ����1��8����λ�ڵ�Ƭ����0��7��Э�����ʶ���һ
								PulseConf.Channel = (ePulseDO_t)DO_TIM4;
								index++;
							
								UserOperation.bVC = U1.Rtbl[index++];
								if(UserOperation.bVC == SELECT_VC_V)
								{
									Switch.ModeCur &= ~(1 << MODE_BIT_VC);
									Switch.ModeCur |= SELECT_VC_V << MODE_BIT_VC;
								}
								else
								{
									Switch.ModeCur &= ~(1 << MODE_BIT_VC);
									Switch.ModeCur |= SELECT_VC_C << MODE_BIT_VC;
								}
								
								UserOperation.bPhase = (UO_PHASE_E)U1.Rtbl[index++];
							
								if(pPwmArrayParam[PulseConf.Channel]->Pwm != NULL)
								{
									myfree(SRAMIN, pPwmArrayParam[PulseConf.Channel]->Pwm);
									pPwmArrayParam[PulseConf.Channel]->Pwm = NULL;
									WDG_Feed();
								}
								DOState.Config |= 1 << PulseConf.Channel;
								DOState.ZeroConfig &= ~(1 << PulseConf.Channel);
								
								PulseConf.PulseType = (ePulseType_t)U1.Rtbl[index++];
								switch(PulseConf.PulseType)
								{
									case PULSE_TYPE_ZERO:													//��100us��ʱ���ж���ִ�д˲��δ���
										
										DOState.Config &= ~(1 << PulseConf.Channel);						//��ZeroConfig�ñ�־λ
										PulseConf.PulseParam.PulseZero.IndexNum[PulseConf.Channel] = BTOW(U1.Rtbl + index);	//��Э������Ӵ�������ֽ�
										index += 2;
										PulseConf.PulseParam.PulseZero.TblTime[PulseConf.Channel] = (uint32_t *)mymalloc(SRAMIN, sizeof(uint32_t) * PulseConf.PulseParam.PulseZero.IndexNum[PulseConf.Channel]);
										if(PulseConf.PulseParam.PulseZero.TblTime[PulseConf.Channel] == NULL)
										{											
											if(DOState.Status[PulseConf.Channel] == DOSTATE_STATUS_RUNNING)
											{
												Process_COMMAND_STOP();					//����������ʱ�޸Ĳ�������ֹͣ���
											}
											
											break;
										}
										PulseConf.PulseParam.PulseZero.TblAmpl[PulseConf.Channel] = (uint32_t *)mymalloc(SRAMIN, sizeof(uint32_t) * PulseConf.PulseParam.PulseZero.IndexNum[PulseConf.Channel]);
										if(PulseConf.PulseParam.PulseZero.TblAmpl[PulseConf.Channel] == NULL)
										{										
											if(DOState.Status[PulseConf.Channel] == DOSTATE_STATUS_RUNNING)
											{
												Process_COMMAND_STOP();
											}
											
											break;
										}
										
										for(j=0; j<PulseConf.PulseParam.PulseZero.IndexNum[PulseConf.Channel]; j++)
										{
											PulseConf.PulseParam.PulseZero.TblTime[PulseConf.Channel][j] = BTOL(U1.Rtbl + index);
											index += 4;
											PulseConf.PulseParam.PulseZero.TblAmpl[PulseConf.Channel][j] = BTOL(U1.Rtbl + index) * 1000;
											index += 4;
										}
										
										DOState.ZeroConfig |= 1 << PulseConf.Channel;
										
										break;
																			
									case PULSE_TYPE_ONE:
										
										PulseConf.PulseParam.PulseOne.IsLowFirst    = U1.Rtbl[index++];
										PulseConf.PulseParam.PulseOne.MsStageFirst	= BTOL(U1.Rtbl + index);
										index += 4;
										PulseConf.PulseParam.PulseOne.MsStageSecond	= BTOL(U1.Rtbl + index);
										index += 4;
										PulseConf.PulseParam.PulseOne.Ampl			= BTOL(U1.Rtbl + index);
										index += 4;
									
										rtn = PulseTran_One(&(PulseConf.PulseParam.PulseOne), pPwmArrayParam[PulseConf.Channel]);
										if(rtn != RTN_NORMAL)
										{
											if(PulseConf.Channel == DO_TIM4 || (PulseConf.Channel != DO_TIM4 && rtn != RTN_ABNORMAL_AMPL))	//ģ��ͨ�� �� ����ͨ���Ƿ�ֵ�쳣 ʱ
											{
												DOState.Config &= ~(1 << PulseConf.Channel);	//�û����ò���Ϊ����ֵ0������ԭ�������쳣ʱ����Ϊδ���ô���
											
												if(DOState.Status[PulseConf.Channel] == DOSTATE_STATUS_RUNNING)
												{
													Process_COMMAND_STOP();	//����������ʱ�޸Ĳ�������ֹͣ���
												}
											}
										}
										
										break;
									
									case PULSE_TYPE_TWO:
										
										PulseConf.PulseParam.PulseTwo.Freq			= BTOL(U1.Rtbl + index);
										index += 4;
										PulseConf.PulseParam.PulseTwo.Duty			= BTOW(U1.Rtbl + index);
										index += 2;
										PulseConf.PulseParam.PulseTwo.MsDuration	= BTOL(U1.Rtbl + index);
										index += 4;
										PulseConf.PulseParam.PulseTwo.Ampl			= BTOL(U1.Rtbl + index);
										index += 4;
								
										rtn = PulseTran_Two(&(PulseConf.PulseParam.PulseTwo), pPwmArrayParam[PulseConf.Channel]);
										if(rtn != RTN_NORMAL)
										{
											if(PulseConf.Channel == DO_TIM4 || (PulseConf.Channel != DO_TIM4 && rtn != RTN_ABNORMAL_AMPL))
											{
												DOState.Config &= ~(1 << PulseConf.Channel);
											
												if(DOState.Status[PulseConf.Channel] == DOSTATE_STATUS_RUNNING)
												{
													Process_COMMAND_STOP();
												}
											}
										}
										
										break;
									
									case PULSE_TYPE_THREE:
										
										PulseConf.PulseParam.PulseThree.Period		= BTOL(U1.Rtbl + index);
										index += 4;
										PulseConf.PulseParam.PulseThree.HighTime	= BTOW(U1.Rtbl + index);
										index += 2;
										PulseConf.PulseParam.PulseThree.PulseNum	= BTOL(U1.Rtbl + index);
										index += 4;
										PulseConf.PulseParam.PulseThree.Pause100ms	= BTOW(U1.Rtbl + index);
										index += 2;
										PulseConf.PulseParam.PulseThree.MsBegin		= BTOW(U1.Rtbl + index);
										index += 2;
										PulseConf.PulseParam.PulseThree.MsEnd		= BTOW(U1.Rtbl + index);
										index += 2;
										PulseConf.PulseParam.PulseThree.MsStep		= BTOW(U1.Rtbl + index);
										index += 2;
										PulseConf.PulseParam.PulseThree.CycleNum	= BTOL(U1.Rtbl + index);
										index += 4;
										PulseConf.PulseParam.PulseThree.Ampl		= BTOL(U1.Rtbl + index);
										index += 4;
																		
										rtn = PulseTran_Three(&(PulseConf.PulseParam.PulseThree), pPwmArrayParam[PulseConf.Channel]);
										if(rtn != RTN_NORMAL)
										{
											if(PulseConf.Channel == DO_TIM4 || (PulseConf.Channel != DO_TIM4 && rtn != RTN_ABNORMAL_AMPL))
											{
												DOState.Config &= ~(1 << PulseConf.Channel);
											
												if(DOState.Status[PulseConf.Channel] == DOSTATE_STATUS_RUNNING)
												{
													Process_COMMAND_STOP();
												}
											}
										}
										
										break;
										
									case PULSE_TYPE_FOUR:
										
										PulseConf.PulseParam.PulseFour.PeriodBegin	= BTOL(U1.Rtbl + index);
										index += 4;
										PulseConf.PulseParam.PulseFour.HighTime		= BTOW(U1.Rtbl + index);
										index += 2;
										PulseConf.PulseParam.PulseFour.PulseNum		= BTOL(U1.Rtbl + index);
										index += 4;
										PulseConf.PulseParam.PulseFour.Pause100ms	= BTOW(U1.Rtbl + index);
										index += 2;
										PulseConf.PulseParam.PulseFour.PeriodEnd	= BTOL(U1.Rtbl + index);
										index += 4;
										PulseConf.PulseParam.PulseFour.PeriodStep	= BTOL(U1.Rtbl + index);
										index += 4;
										PulseConf.PulseParam.PulseFour.CycleNum		= BTOL(U1.Rtbl + index);
										index += 4;
										PulseConf.PulseParam.PulseFour.Ampl			= BTOL(U1.Rtbl + index);
										index += 4;
									
										rtn = PulseTran_Four(&(PulseConf.PulseParam.PulseFour), pPwmArrayParam[PulseConf.Channel]);
										if(rtn != RTN_NORMAL)
										{
											if(PulseConf.Channel == DO_TIM4 || (PulseConf.Channel != DO_TIM4 && rtn != RTN_ABNORMAL_AMPL))
											{
												DOState.Config &= ~(1 << PulseConf.Channel);
											
												if(DOState.Status[PulseConf.Channel] == DOSTATE_STATUS_RUNNING)
												{
													Process_COMMAND_STOP();
												}
											}
										}
																				
										break;
										
									case PULSE_TYPE_FIVE:
										
										PulseConf.PulseParam.PulseFive.Freq					= BTOL(U1.Rtbl + index);
										index += 4;
									
										if(PulseConf.Channel == 0x07)							//���һͨ�������������
										{
											PulseConf.PulseParam.PulseFive.Duty				= PULSE_DUTY_FULL / 2;		//�ǽ��գ�Լ��ֵ
										}
										else													//����ͨ�������������
										{
											PulseConf.PulseParam.PulseFive.Duty				= PULSE_DUTY_FULL;			//�ǽ��գ�Լ��ֵ
										}
										
										PulseConf.PulseParam.PulseFive.ConfigNum			= BTOL(U1.Rtbl + index);
										index += 4;
										
										if(PulseConf.PulseParam.PulseFive.ConfigNum > PULSE_FIVE_TBL_MAX_NUM)
										{
											PulseConf.PulseParam.PulseFive.ConfigNum		= PULSE_FIVE_TBL_MAX_NUM;
										}
										for(j = 0; j < PulseConf.PulseParam.PulseFive.ConfigNum; j++)
										{
											PulseConf.PulseParam.PulseFive.MsPause[j]		= BTOW(U1.Rtbl + index);
											index += 2;
											PulseConf.PulseParam.PulseFive.MsDuration[j]	= BTOL(U1.Rtbl + index);
											index += 4;
										}
										
										rtn = PulseTran_Five(&(PulseConf.PulseParam.PulseFive), pPwmArrayParam[PulseConf.Channel]);
										if(rtn != RTN_NORMAL)
										{
											if(PulseConf.Channel == DO_TIM4 || (PulseConf.Channel != DO_TIM4 && rtn != RTN_ABNORMAL_AMPL))
											{
												DOState.Config &= ~(1 << PulseConf.Channel);
											
												if(DOState.Status[PulseConf.Channel] == DOSTATE_STATUS_RUNNING)
												{
													Process_COMMAND_STOP();
												}
											}
										}								
										
										break;
									
									default:
										
										break;
								}
								
								if(DOState.Config)
								{
									Protocol_Usart1_Tx(COMMAND_CONFIG);
									
									StatusBar_Progress(STATUSBAR_CONFIG);
								}
																
								INTX_ENABLE();													//���ж�
								
								break;
								
							default:
								
								break;
						}
					}
					
					break;
					
				default:
					break;
			}
		}
		U1.Rflag = 0;
	}
}
