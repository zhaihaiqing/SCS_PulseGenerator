
#include "main.h"

#define STATUSBAR_CONFIG	(1 << 0)
#define STATUSBAR_RUN		(1 << 1)
#define STATUSBAR_COMPLETE	(1 << 2)
#define STATUSBAR_PAUSE		(1 << 3)
#define STATUSBAR_CONTINUE	(1 << 7)
static uint8_t  statusbar	= 0;																//程序运行进度

/********************************************************************
*	功能	：	运行状态处理函数
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

static uint32_t temp_runcnt = 0;																//运行时间暂存
static uint16_t Tbl_Temp_CCRx[8] = {0};
static uint16_t Tbl_Temp_CNT[8]  = {0};
/********************************************************************
*	功能	：	命令处理函数
******************************************************************************/
void Process_COMMAND_START(uint8_t chnl)
{
	uint8_t i = 0;
	
	if(chnl == DO_ALL)
	{
		for(i=0; i<8; i++)
		{
			if(DOState.Config & (1 << i))
			{
				DOState.Status[i] = DOSTATE_STATUS_RUNNING;
				*Tbl_TIM_ARR[i]   = pPwmArrayParam[i]->Pwm[0].PwmARR;
				*Tbl_TIM_PSC[i]   = pPwmArrayParam[i]->Pwm[0].PwmPSC;
				*Tbl_TIM_CCRx[i]  = pPwmArrayParam[i]->Pwm[0].PwmDuty;
				*Tbl_TIM_DIER[i] |= (1 << 0);
				*Tbl_TIM_CR1[i]  |= 0x01;
			}
			
			if(DOState.ZeroConfig & (1 << i))
			{
				DOState.Status[i] = DOSTATE_STATUS_RUNNING;
				TIM2->DIER   |= 1 << 0;
				TIM2->CR1    |= 0X01;
			}
		}
	}
	else if(chnl < DO_ALL)
	{
		if(DOState.Config & (1 << chnl))
		{
		     DOState.Status[chnl] = DOSTATE_STATUS_RUNNING;
			
			*Tbl_TIM_ARR[chnl]   = pPwmArrayParam[chnl]->Pwm[0].PwmARR;
			*Tbl_TIM_PSC[chnl]   = pPwmArrayParam[chnl]->Pwm[0].PwmPSC;
			*Tbl_TIM_CCRx[chnl]  = pPwmArrayParam[chnl]->Pwm[0].PwmDuty;
			*Tbl_TIM_DIER[chnl] |= (1 << 0);
			*Tbl_TIM_CR1[chnl]  |= 0x01;
		}
		
		if(DOState.ZeroConfig & (1 << chnl))
		{
			DOState.Status[chnl] = DOSTATE_STATUS_RUNNING;
			TIM2->DIER   |= 1 << 0;
			TIM2->CR1    |= 0X01;
		}
	}
	
	
	T6.RunCnt = 1;
	T2.OutputCnt = 0;
}

void Process_COMMAND_STOP(uint8_t chnl)
{
	uint8_t i = 0;
	
	if(chnl == DO_ALL)
	{
		for(i=0; i<8; i++)
		{
			DOState.Status[i] = DOSTATE_STATUS_COMPLETE;
			
			if(DOState.Config & (1 << i))
			{
				*Tbl_TIM_DIER[i] &= ~(1 << 0);
				*Tbl_TIM_CR1[i]  &= ~0x01;
				
				Tbl_Flag[i]       = 0;				
				*Tbl_TIM_CCRx[i]  = 0;
				
				if(*Tbl_TIM_ARR[i] - *Tbl_TIM_CNT[i] > 5)											//加快停止速度
				{
					*Tbl_TIM_CNT[i]  = *Tbl_TIM_ARR[i] - 2;
				}
				
				*Tbl_TIM_ARR[i]   = SYNC_ARR;														//此三条语句起主要作用，能够解决中途停止引起下一次不同步问题
				*Tbl_TIM_PSC[i]   = SYNC_PSC;
				*Tbl_TIM_CCRx[i]  = SYNC_CCR;
				
				*Tbl_TIM_DIER[i] |= (1 << 0);
				*Tbl_TIM_CR1[i]  |= 0x01;
			}
			else if(DOState.ZeroConfig & (1 << i))
			{
				TIM2->DIER   &= ~(1 << 0);
				TIM2->CR1    &= ~0X01;
				Output_VorC(UserOperation.bVC, 0, OUTPUT_DISABLE);									//[AATN]，若是后续扩展多通道，类似此处逻辑需注意调整
			}
		}
	}
	else if(chnl < DO_ALL)
	{
		DOState.Status[chnl] = DOSTATE_STATUS_COMPLETE;
		
		if(DOState.Config & (1 << chnl))
		{
			*Tbl_TIM_DIER[chnl]  &= ~(1 << 0);
			*Tbl_TIM_CR1[chnl]   &= ~0x01;
			
			Tbl_Flag[chnl]       = 0;			
			*Tbl_TIM_CCRx[chnl]  = 0;
			
			if(*Tbl_TIM_ARR[chnl] - *Tbl_TIM_CNT[chnl] > 5)											//加快停止速度
			{
				*Tbl_TIM_CNT[chnl]  = *Tbl_TIM_ARR[chnl] - 2;
			}
			
			*Tbl_TIM_ARR[chnl]   = SYNC_ARR;														//此三条语句起主要作用，能够解决中途停止引起下一次不同步问题
			*Tbl_TIM_PSC[chnl]   = SYNC_PSC;
			*Tbl_TIM_CCRx[chnl]  = SYNC_CCR;
			
			*Tbl_TIM_DIER[chnl] |= (1 << 0);
			*Tbl_TIM_CR1[chnl]  |= 0x01;
		}
		else if(DOState.ZeroConfig & (1 << chnl))
		{
			TIM2->DIER   &= ~(1 << 0);
			TIM2->CR1    &= ~0X01;
			Output_VorC(UserOperation.bVC, 0, OUTPUT_DISABLE);
		}
		 
	}
	
	SW_CV_OUTPUT = 0;   //关闭输出
	T6.RunCnt = 0;
	T2.OutputCnt = 0;
}

void Process_COMMAND_PAUSE(uint8_t chnl)
{
	uint8_t i = 0;
	
	if(chnl == DO_ALL)
	{
		for(i=0; i<8; i++)
		{
			if(DOState.Status[i] == DOSTATE_STATUS_RUNNING)
			{
				DOState.Status[i] = DOSTATE_STATUS_PAUSE;
				
				if(DOState.Config & (1 << i))
				{
					Tbl_Temp_CCRx[i]  = *Tbl_TIM_CCRx[i];
					*Tbl_TIM_CCRx[i]  = 0;
					Tbl_Temp_CNT[i]   = *Tbl_TIM_CNT[i];
					*Tbl_TIM_CNT[i]   = *Tbl_TIM_ARR[i];
				}
				else if(DOState.ZeroConfig & (1 << i))
				{
					TIM2->DIER   &= ~(1 << 0);
					TIM2->CR1    &= ~0X01;
					Output_VorC(UserOperation.bVC, 0, OUTPUT_DISABLE);
				}
			}
		}		
	}
	else if(chnl < DO_ALL)
	{
		if(DOState.Status[chnl] == DOSTATE_STATUS_RUNNING)
		{
			DOState.Status[chnl] = DOSTATE_STATUS_PAUSE;
			
			if(DOState.Config & (1 << chnl))
			{
				Tbl_Temp_CCRx[chnl]  = *Tbl_TIM_CCRx[chnl];
				*Tbl_TIM_CCRx[chnl]  = 0;
				Tbl_Temp_CNT[chnl]   = *Tbl_TIM_CNT[chnl];
				*Tbl_TIM_CNT[chnl]   = *Tbl_TIM_ARR[chnl];	
			}
			else if(DOState.ZeroConfig & (1 << chnl))
			{
				TIM2->DIER   &= ~(1 << 0);
				TIM2->CR1    &= ~0X01;
				Output_VorC(UserOperation.bVC, 0, OUTPUT_DISABLE);
			}
		}
		//log_info("SW_CV_OUTPUT = 0\r\n");
		//SW_CV_OUTPUT = 0;   //关闭输出
	}

	temp_runcnt = T6.RunCnt;
	T6.RunCnt   = 0;
}

void Process_COMMAND_CONTINUE(uint8_t chnl)
{
	uint8_t i = 0;
	
	if(chnl == DO_ALL)
	{
		for(i=0; i<8; i++)
		{
			if(DOState.Status[i] == DOSTATE_STATUS_PAUSE)
			{
				DOState.Status[i] = DOSTATE_STATUS_RUNNING;
				
				if(DOState.Config & (1 << i))
				{
					*Tbl_TIM_CCRx[i]  = Tbl_Temp_CCRx[i];
					*Tbl_TIM_CNT[i]   = Tbl_Temp_CNT[i];					
				}
				else if(DOState.ZeroConfig & (1 << i))
				{
					TIM2->DIER   |= 1 << 0;
					TIM2->CR1    |= 0X01;
				}
			}
		}
	}
	else if(chnl < DO_ALL)
	{
		if(DOState.Status[chnl] == DOSTATE_STATUS_PAUSE)
		{
			DOState.Status[chnl] = DOSTATE_STATUS_RUNNING;
			
			if(DOState.Config & (1 << chnl))
			{
				*Tbl_TIM_CCRx[chnl]  = Tbl_Temp_CCRx[chnl];
				*Tbl_TIM_CNT[chnl]   = Tbl_Temp_CNT[chnl];				
			}
			else if(DOState.ZeroConfig & (1 << chnl))
			{
				TIM2->DIER   |= 1 << 0;
				TIM2->CR1    |= 0X01;
			}			
		}
	}
	
//	log_info("SW_CV_OUTPUT = 1\r\n");
//	SW_CV_OUTPUT = 1;   //打开输出
//	Delay_ms(15);		//等待继电器稳定闭合
	
	T6.RunCnt   = temp_runcnt;
	temp_runcnt = 0;
}

/********************************************************************
*	功能	：	串口发送数据
******************************************************************************/
static void Protocol_Usart1_Tx(uint8_t command)
{
	uint8_t  i = 0, j = 0, k = 0, index = 0;
	uint16_t crcval = 0;
	
	U1.Ttbl[ADDR_OFFSET_HEAD]    = PROTOCOL_FIXED_HEAD;		
	U1.Ttbl[ADDR_OFFSET_VERSION] = PROTOCOL_FIXED_VERSION;										//①
	U1.Ttbl[ADDR_OFFSET_COMMAND] = command | 0x80;												//②
	U1.Ttbl[ADDR_OFFSET_RESERVE] = 0X00;
	
	index = ADDR_OFFSET_RESERVE;
	index++;
	
	if(command == COMMAND_QUERY)
	{
		for(i=0; i<8; i++)
		{
			if(DOState.Status[i] == DOSTATE_STATUS_RUNNING)										//运行
			{
				j |= 1 << i;
				StatusBar_Progress(STATUSBAR_RUN);
				break;
			}
			else if(DOState.Status[i] == DOSTATE_STATUS_COMPLETE)								//完成
			{
				k++;
				if(k == 8)
				{
					StatusBar_Progress(STATUSBAR_COMPLETE);
					T6.RunCnt = 0;
				}
			}
			else if(DOState.Status[i] == DOSTATE_STATUS_PAUSE)									//暂停
			{
				StatusBar_Progress(STATUSBAR_PAUSE);
				break;
			}
		}
		U1.Ttbl[index++] = (uint8_t)j;															//③
		
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
		
		U1.Ttbl[index++] = statusbar;															//运行进度
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
*	功能	：	处理串口接收数据
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
						rcvnum = i;																//跳入case 1
					}
					else
					{
						break;
					}
					
				case 1:
					
					datasize = U1.Rtbl[rcvnum+1]*256 + U1.Rtbl[rcvnum+2];
					if(U1.Rtbl[rcvnum+3+datasize] == PROTOCOL_FIXED_END)
					{
						rcvstep = 2;															//跳入case 2
					}
					else
					{
						rcvstep = 0;
						break;
					}
					
				case 2:
					
					rcvstep = 0;																//一组数据可能需识别多个命令
				
					if(Crc_16bit_Calculate((uint8_t *)(&U1.Rtbl[rcvnum+ADDR_OFFSET_COMMAND]), datasize-3) == U1.Rtbl[rcvnum+ADDR_OFFSET_COMMAND+datasize-3]*256 + U1.Rtbl[rcvnum+ADDR_OFFSET_COMMAND+datasize-2])
					{
						i = rcvnum + 3 + datasize;												//后续可能还有有效数据，不可直接跳出循环
						
						switch(U1.Rtbl[rcvnum+ADDR_OFFSET_COMMAND])
						{
							case COMMAND_START:
								
								Protocol_Usart1_Tx(COMMAND_START);
							
								Process_COMMAND_START(PulseConf.Channel);

								StatusBar_Progress(STATUSBAR_RUN);
							
								break;
							
							case COMMAND_PAUSE:
								
								if(U1.Rtbl[rcvnum+ADDR_OFFSET_CONTENT_START] == 0x00)
								{
									Process_COMMAND_PAUSE(PulseConf.Channel);
									
									StatusBar_Progress(STATUSBAR_PAUSE);
								}
								else if(U1.Rtbl[rcvnum+ADDR_OFFSET_CONTENT_START] == 0x01)
								{
									Process_COMMAND_CONTINUE(PulseConf.Channel);
									
									StatusBar_Progress(STATUSBAR_CONTINUE);
								}
								
								Protocol_Usart1_Tx(COMMAND_PAUSE);
								
								break;
							
							case COMMAND_STOP:
								
								Process_COMMAND_STOP(PulseConf.Channel);
							
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
								
								INTX_DISABLE();													//关闭中断，涉及PWM中断相关参数
								
								index = rcvnum + ADDR_OFFSET_CONTENT_START;
							
//								PulseConf.Channel = (ePulseDO_t)(U1.Rtbl[index] - 1);			//通道号，上位机【1，8】，位于单片机【0，7】协调，故而减一
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
									case PULSE_TYPE_ZERO:													//在100us定时器中断中执行此波形处理
										
										DOState.Config &= ~(1 << PulseConf.Channel);						//在ZeroConfig置标志位
										PulseConf.PulseParam.PulseZero.IndexNum[PulseConf.Channel] = BTOW(U1.Rtbl + index);	//待协议中添加打点数量字节
										index += 2;
										PulseConf.PulseParam.PulseZero.TblTime[PulseConf.Channel] = (uint32_t *)mymalloc(SRAMIN, sizeof(uint32_t) * PulseConf.PulseParam.PulseZero.IndexNum[PulseConf.Channel]);
										if(PulseConf.PulseParam.PulseZero.TblTime[PulseConf.Channel] == NULL)
										{											
											if(DOState.Status[PulseConf.Channel] == DOSTATE_STATUS_RUNNING)
											{
												Process_COMMAND_STOP(PulseConf.Channel);					//若正在运行时修改参数，则停止输出
											}
											
											break;
										}
										PulseConf.PulseParam.PulseZero.TblAmpl[PulseConf.Channel] = (uint32_t *)mymalloc(SRAMIN, sizeof(uint32_t) * PulseConf.PulseParam.PulseZero.IndexNum[PulseConf.Channel]);
										if(PulseConf.PulseParam.PulseZero.TblAmpl[PulseConf.Channel] == NULL)
										{										
											if(DOState.Status[PulseConf.Channel] == DOSTATE_STATUS_RUNNING)
											{
												Process_COMMAND_STOP(PulseConf.Channel);
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
											if(PulseConf.Channel == DO_TIM4 || (PulseConf.Channel != DO_TIM4 && rtn != RTN_ABNORMAL_AMPL))	//模拟通道 或 数字通道非幅值异常 时
											{
												DOState.Config &= ~(1 << PulseConf.Channel);	//用户设置参数为特殊值0或其它原因引起异常时，视为未配置处理
											
												if(DOState.Status[PulseConf.Channel] == DOSTATE_STATUS_RUNNING)
												{
													Process_COMMAND_STOP(PulseConf.Channel);	//若正在运行时修改参数，则停止输出
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
													Process_COMMAND_STOP(PulseConf.Channel);
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
													Process_COMMAND_STOP(PulseConf.Channel);
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
													Process_COMMAND_STOP(PulseConf.Channel);
												}
											}
										}
																				
										break;
										
									case PULSE_TYPE_FIVE:
										
										PulseConf.PulseParam.PulseFive.Freq					= BTOL(U1.Rtbl + index);
										index += 4;
									
										if(PulseConf.Channel == 0x07)							//最后一通道波形输出处理
										{
											PulseConf.PulseParam.PulseFive.Duty				= PULSE_DUTY_FULL / 2;		//非接收，约定值
										}
										else													//其它通道波形输出处理
										{
											PulseConf.PulseParam.PulseFive.Duty				= PULSE_DUTY_FULL;			//非接收，约定值
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
													Process_COMMAND_STOP(PulseConf.Channel);
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
																
								INTX_ENABLE();													//打开中断
								
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
