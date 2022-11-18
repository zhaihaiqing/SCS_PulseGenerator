
#include  "main.h"

sPwmArrayParam_t PwmArrayParam[8]   = {0};															//�粻���壬ÿ�ζ������漰����̬����ʱ������������pPwmArrayParam����Ұָ�����
sPwmArrayParam_t *pPwmArrayParam[8] = {&PwmArrayParam[0], &PwmArrayParam[1], &PwmArrayParam[2], &PwmArrayParam[3], &PwmArrayParam[4], &PwmArrayParam[5], &PwmArrayParam[6], &PwmArrayParam[7]};
	
__align(8) static float_t dTempVar = 0;																//��ʱ������������ʱ�洢����������ֹ������������������ɺ��Ӧ��ֵ
/********************************************************************
*	����	��	��������Ƶ�ʡ�ռ�ձȼ���PWMƥ��ARR/PSC/DUTY
*	����	��	
				freq--Ƶ�ʣ�Ϊʵ������ֵ��100�����Լ���С�������λ��Ч����,[0.02Hz, 30KHz] * PULSE_FREQ_MAGNIFY
				duty--ռ�ձȣ�����50000������ռ�ձ�ʱΪ50000
				pwm---������νṹ��ָ��
*	��ֵ	��	
				psc--[15:0]				arr--[15:0]
*	��ע	��	
				��������72MHz
				�����������Ƶ��20KHz��������������� T = 1 / 20KHz = 50us
				����psc = 1��Ƶ��ÿ1us����Ϊ	72M / 1M = 72����ÿ���������ڼ���	50 * (72 / 1) = 3600
				  ��psc = 2��Ƶ��ÿ1us����Ϊ	72M / 2 / 1M = 36,ÿ���������ڼ���	50 * (72 / 2) = 1800
				�ڸ���Tout = ((arr+1)*(psc+1))/Tclk
				��ʱ��������	arr * psc = 72M / 20K = 3600,��	arr = 72M / 20K / psc
				�����	(1M / 20KHz) * (72 / psc) > 72M / 20KHz / psc�������������ȫ�ߵ�ƽ
				  ��Ч	(1 / freq) * (72 / psc) > 72M / freq / psc�������������ȫ�ߵ�ƽ
******************************************************************************/
static uint8_t PulseTran_Sub_CalcPwmPara(float_t freq, float_t duty, sPwmParam_t *pwm)
{
	uint16_t psc1  = 0, psc2 = 0;
	float_t  temp = 0;
	
#ifdef NEED_FLOAT_CALC
	float_t  arr1 = 0, arr2 = 0;		`																//WFC�����޸�Ϊ������������ռ��ʱ������������ֲ��������ᴥ�����Ź�
#else
	uint32_t arr1 = 0, arr2 = 0;
#endif
	
	/*	temp--[2.400, 3.600.000.000]	Tout = ((arr+1)*(psc+1))/Tclk	(arr+1)*(psc+1) = Tclk/freq		*/
	temp = 84000000  / ((float_t)freq / 100);															//�������ڣ���λ����Ƶ��72000000 / ����Ƶ�� = �������������������γ�10ΪPULSE_FREQ_MAGNIFY����˴������ֱ������������ƫ���

	/*	
		�Ա�С�㷨����ƫ������Ƶ�ʽ�С���ò������޷�ʹ�õ��Զ˿��õ���ٷ�������ƫ��
		˼·����Сʱֱ������pscΪ1�������ϴ�ʱ���жϿ����;����n��10����λ�����ý��бȽϣ�ƫ��С����Ϊ����ֵ
	*/
	if(temp <= 65535)
	{
		arr1 = temp;
		psc1 = 1;
	}
	else
	{
		arr1 = sqrt(temp);
		psc1 = temp / arr1;
		
		if(temp > 65535 * 10000)
		{
			arr2 = temp / 60000;
			psc2 = 60000;
		}
		else if(temp > 65535 * 1000)
		{
			arr2 = temp / 10000;																		//����1000�ǿ϶������ʵģ�ֻ�ܳ���10000����λ���������0�� 999֮��
			psc2 = 10000;
		}
		else if(temp > 65535 * 100)
		{
			arr2 = temp / 1000;
			psc2 = 1000;
		}
		else if(temp > 65535 * 10)
		{
			arr2 = temp / 100;
			psc2 = 100;
		}
		else
		{
			arr2 = temp / 10;
			psc2 = 10;
		}
		
		if(temp - arr1 * psc1 > temp - arr2 * psc2)
		{
			arr1 = arr2;
			psc1 = psc2;
		}
	}

	pwm->PwmARR = arr1 - 1;																			//arr����������������
	pwm->PwmPSC = psc1 - 1;																			//psc����λ����Ƶ��72000000 / psc / arr��psc�������Ƶ�ʷ�Ƶ��ͨ���������ڽ��м���	
	
	if(duty >= PULSE_DUTY_FULL)
	{
		pwm->PwmDuty = pwm->PwmARR + 1;																//100%ռ�ձ�ֱ������ΪARR����Ƶʱ�ܲ�׽����ƽ��ת
	}
	else if(duty == 0)
	{
		pwm->PwmDuty = 0;
	}
	else
	{
		if(pwm->PwmARR + 1 < PULSE_DUTY_FULL / duty)												//��Ƶֵ���󣬵��¼���ֵ�޷�����ռ�ձȴ���1�������������2018-1-19��
		{
			temp = pwm->PwmARR;
			pwm->PwmARR = PULSE_DUTY_FULL / duty - 1;
			temp = pwm->PwmPSC * temp / pwm->PwmARR;
			if(temp < 1)																			//��֤��ʹ���������쳣��psc��Ϊ�쳣ֵ����ʱ������������û�������ƫ����������ڱ�����С�������н��Ʋ������
			{
				temp = 1;
			}
			pwm->PwmPSC = temp - 1;
		}
		
		temp = (pwm->PwmARR + 1) * duty / PULSE_DUTY_FULL;
		pwm->PwmDuty = (uint16_t)(temp + 0.5);
	}
	return 0;
}

/***************************************************************************************************
*	����	��	��һ�ֲ��δ���
*	����	��	
				�����������
*	����	��	
				����Ƶ�ʹ������������ģ��������׶ε�PWM���Σ��ֱ�Ϊ0ռ�ձȺ�100ռ�ձȵĽ׶�
				ÿ���׶εĳ��ȳ���ʱ���ۺϳ�PWM�������̶�Ƶ��10kHz,����100us
*	����	��	
				sPulseXXX_t     --���ڽ��յ�����������
				sPwmArrayParam_t--����Ϊ���õĲ�������
*******************************************************************************************************************/
eRtn_t PulseTran_One(sPulseOne_t *one, sPwmArrayParam_t *param)
{
	uint8_t  index = 0;
	uint32_t duty  = 0;
	float_t  freq  = 0;
	
	if((one == NULL) || (param == NULL))
	{
		return RTN_ABNORMAL_PARAM;
	}
	
	if((one->MsStageFirst == 0) && (one->MsStageSecond == 0))
	{
		return RTN_ABNORMAL_OTHER;
	}
	
	if(one->Ampl == 0)
	{
		return RTN_ABNORMAL_AMPL;
	}
	else
	{
		param->Ampl = one->Ampl * 1000;																//ͳһ������ʹ�õ�λ��mV/uA->uV/nA
	}
	
	if(param->Pwm != NULL)
	{
		myfree(SRAMIN, param->Pwm);
	}
	
	param->Pwm = (sPwmParam_t *)mymalloc(SRAMIN, sizeof(sPwmParam_t) * 2);
	if(param->Pwm == NULL)
	{
		return RTN_ABNORMAL_SRAM;
	}
	
	freq = PULSE_FREQ_AUXILIARY;
	/*	��һ��	*/
	if(one->MsStageFirst)																			//�û���������0ֵ����
	{		
		duty = (one->IsLowFirst)? 0:PULSE_DUTY_FULL;												//�͵�ƽ��0%ռ�ձȣ�����100%
		PulseTran_Sub_CalcPwmPara(freq, duty, &(param->Pwm[index]));	
		dTempVar = one->MsStageFirst * ((float_t)freq / 1000 / PULSE_FREQ_MAGNIFY);
		param->Pwm[index].PwmNum = (uint32_t)dTempVar;												//��ʱ�ϳ�ʱ����freq����ֻ�����uint32_t����������ms�ȳ��ᵼ�»�Ϊ0���
		index++;
		#if 0
			printf("ARR10 = %u\r\n",  	 param->Pwm[0].PwmARR);
			printf("Psc10 = %u\r\n",  	 param->Pwm[0].PwmPSC);
			printf("Duty10 = %u\r\n", 	 param->Pwm[0].PwmDuty);
			printf("PwmNum10 = %u\r\n",  param->Pwm[0].PwmNum);
		#endif
	}
	
	/*	�ڶ���	*/
	if(one->MsStageSecond)
	{
		duty = (one->IsLowFirst)? PULSE_DUTY_FULL:0;
		PulseTran_Sub_CalcPwmPara(freq, duty, &(param->Pwm[index]));
		dTempVar = one->MsStageSecond * ((float_t)freq / 1000 / PULSE_FREQ_MAGNIFY);
		param->Pwm[index].PwmNum = (uint32_t)dTempVar;
		index++;
		#if 0
			printf("ARR11 = %u\r\n",     param->Pwm[1].PwmARR);
			printf("Psc11 = %u\r\n",     param->Pwm[1].PwmPSC);
			printf("Duty11 = %u\r\n",    param->Pwm[1].PwmDuty);
			printf("PwmNum11 = %u\r\n",  param->Pwm[1].PwmNum);
		#endif
	}
	
	param->VarietyNum = index;
	param->CycleNum   = 1;
	
	return RTN_NORMAL;
}

/***************************************************************************************************
*	����	��	�ڶ��ֲ��δ���
*	����	��	
				�̶��������
*	����	��	
				����ģ���һ���׶ε�PWM���Σ�Ƶ�ʺ�ռ�ձ�����ͨ�������ó�������ʱ��ת��ΪPWM����
				���θ���
					Ϊ����ʱ����������
					���򣬸�����������
						�����ڸߵ�ƽʱ�������θ�����1�󣬰����������δ���
						��С�ڸߵ�ƽʱ�������������������㲨��ռ�ձ�
*******************************************************************************************************************/
eRtn_t PulseTran_Two(sPulseTwo_t *two, sPwmArrayParam_t *param)
{
	uint32_t duty = 0;
	float_t  freq = 0;
	
	if((two == NULL) || (param == NULL))
	{
		return RTN_ABNORMAL_PARAM;
	}
	
	if(two->Duty == 0 || two->MsDuration == 0)
	{
		return RTN_ABNORMAL_DUTY;
	}
	
	if(two->Ampl == 0)
	{
		return RTN_ABNORMAL_AMPL;
	}
	else
	{
		param->Ampl = two->Ampl * 1000;
	}

	if(param->Pwm != NULL)
	{
		myfree(SRAMIN, param->Pwm);
	}
	
	param->Pwm = (sPwmParam_t *)mymalloc(SRAMIN, sizeof(sPwmParam_t) * 2);
	if(param->Pwm == NULL)
	{
		#if 0
			printf("PULSETRAN_TWO_p0\r\n");
		#endif
		return RTN_ABNORMAL_SRAM;
	}
	#if 0	
		printf("PULSETRAN_TWO_p1\r\n");
	#endif	
	
	if(two->Freq < PULSE_FREQ_LOWERLIMIT)
	{
		two->Freq = PULSE_FREQ_LOWERLIMIT;
	}
	freq = two->Freq;
	duty = two->Duty;
	PulseTran_Sub_CalcPwmPara(freq, duty, &(param->Pwm[0]));
	dTempVar = two->MsDuration * ((float_t)freq / 1000 / PULSE_FREQ_MAGNIFY);
	param->Pwm[0].PwmNum = (uint32_t)dTempVar;														//MsDuration >= 1000/ (freq / PULSE_FREQ_MAGNIFY)
	#if 0
		printf("Freq2 = %u\r\n",   freq);
		printf("duty2 = %u\r\n",   duty);
		printf("PwmNum2 = %u\r\n", param->Pwm[0].PwmNum);
	#endif	

	if(dTempVar - param->Pwm[0].PwmNum == 0)														//����������
	{		
		param->VarietyNum = 1;
	}
	else
	{
		if((dTempVar - param->Pwm[0].PwmNum) * param->Pwm[0].PwmARR >= param->Pwm[0].PwmDuty)		//�������ڸߵ�ƽʱ��
		{
			param->Pwm[0].PwmNum += 1;
			
			param->VarietyNum     = 1;
		}
		else if(param->Pwm[0].PwmNum == 0)															//û���������֣��������㲨��ռ�ձ�
		{
			param->Pwm[0].PwmDuty = (dTempVar - param->Pwm[0].PwmNum) * param->Pwm[0].PwmARR;
			param->Pwm[0].PwmNum  = 1;
			
			param->VarietyNum     = 1;
		}
		else if(param->Pwm[0].PwmNum > 0)															//���������֣���������ռ�ձ�
		{
			param->Pwm[1].PwmARR  = param->Pwm[0].PwmARR;
			param->Pwm[1].PwmPSC  = param->Pwm[0].PwmPSC;
			param->Pwm[1].PwmDuty = (dTempVar - param->Pwm[0].PwmNum) * param->Pwm[0].PwmARR;
			param->Pwm[1].PwmNum  = 1;
			
			param->VarietyNum     = 2;
		}
	}
	param->CycleNum   = 1;
		
	return RTN_NORMAL;
}

/***************************************************************************************************
*	����	��	�����ֲ��δ���
*	����	��	
				����׶Σ�ÿ���׶���n���������һ����ǰ����ʱ��ɣ�����ʱ��׶����Ա仯���׶μ�̶���ʱ
*	����	��	
				ÿ��ѭ���׶ο���ģ�������PWM���Σ�Ƶ����ʱ�任�㣬ռ�ձ�������ʱ��͹̶��ߵ�ƽʱ������

				��һ��--�׶����ӳ�ʱ��ǰ��PWM����-1�����θ���������ÿ���׶����һ���ߵ�ƽ��
				�ڶ���--����ʼ/��ֹ�ࣩʱ�������һ�����θߵ�ƽ���� ������ ʱ�䲻һ�����������еĵ͵�ƽʱ�䣬���ݣ������
				������--����ʼ/��ֹ�ࣩʱ���ȥ���θߵ�ƽʱ��
				���Ĳ�--����ʼ/��ֹ�ࣩʱ������һ������
				���岽--�׶μ���ͣʱ��

				�׶����ɳ�ʼ�ӳ�ʱ�䡢��ֹ�ӳ�ʱ��Ͳ����ӳ�ʱ�����
*******************************************************************************************************************/
eRtn_t PulseTran_Three(sPulseThree_t *three, sPwmArrayParam_t *param)
{
	uint8_t  KindNum = 0;
	uint32_t SectionNum = 0, msDealy = 0;
	uint32_t i = 0, index = 0;
	float_t  freq = 0, duty = 0, three_freq = 0;
	
	if(three == NULL || param == NULL)
	{
		return RTN_ABNORMAL_PARAM;
	}
	if(three->Ampl == 0)
	{
		return RTN_ABNORMAL_AMPL;
	}
	else
	{
		param->Ampl = three->Ampl * 1000;
	}
	
	if(three->MsBegin == three->MsEnd || three->MsStep == 0)									//�û���������ֵ����
	{
		SectionNum = 1;
	}
	else if(three->MsEnd > three->MsBegin)
	{
		SectionNum = 1 + (three->MsEnd - three->MsBegin) / three->MsStep;						//n*�������е�Ԫ
	}
	else
	{
		SectionNum = 1 + (three->MsBegin - three->MsEnd) / three->MsStep;
	}
	KindNum = (three->Pause100ms!=0)? 5:4;														//�������е�Ԫ�в���������,bug�޸���2018-2-2��
	#if 0
		printf("SectionNum3 = %u\r\n", SectionNum);
		printf("TABLEINDEX3 = %u\r\n", sizeof(sPwmParam_t) * KindNum * SectionNum);	
	#endif	

	if(param->Pwm != NULL)
	{
		myfree(SRAMIN, param->Pwm);
	}
	
	param->Pwm = (sPwmParam_t *)mymalloc(SRAMIN, sizeof(sPwmParam_t) * KindNum * SectionNum);
	if(param->Pwm == NULL)
	{
		return RTN_ABNORMAL_SRAM;
	}
	
	three_freq = PULSE_FREQ_MAGNIFY * 1000 / three->Period;										//	(1000 / three->WaveLenght) * PULSE_UNIT_FREQ��������˳�򣬱�������ϴ����
	for(i=0; i<SectionNum; i++)
	{
		WDG_Feed();																				//��ʱ�ϳ������ӿ��Ź�����������
		/*	��һ��	*/
		freq = three_freq;
		duty = three->HighTime * PULSE_DUTY_FULL / three->Period;
		PulseTran_Sub_CalcPwmPara(freq, duty, &(param->Pwm[index]));
		param->Pwm[index].PwmNum = three->PulseNum - 1;
		index++;
		
		/*	�ڶ���	*/
		freq = three_freq * PULSE_DUTY_FULL / duty;												//�ѵ����ߵ�ƽʱ����ԭ�����λ�����Ƶ������Ϊһ�����ߵ�ƽ���Ѹߵ�ƽ����һ���������������ӦƵ�ʴ���
		duty = PULSE_DUTY_FULL;
		PulseTran_Sub_CalcPwmPara(freq, duty, &(param->Pwm[index]));
		param->Pwm[index].PwmNum = 1;
		index++;
		
		/*	������	*/
		if(three->MsEnd > three->MsBegin)
		{
			msDealy = three->MsBegin + i * three->MsStep;
		}
		else
		{
			if(three->MsBegin > i*three->MsStep)
			{
				msDealy = three->MsBegin - i * three->MsStep;
			}
			else
			{
				msDealy = 0;
			}
		}

		if(msDealy - three->HighTime > 0)														//���ʱ���ȥ��һ�����ߵ�ƽʱ��
		{
			if(msDealy - three->HighTime < 50 * 1000)
			{
				freq = 1000 * PULSE_FREQ_MAGNIFY / (msDealy - three->HighTime);
				duty = 0;
				PulseTran_Sub_CalcPwmPara(freq, duty, &(param->Pwm[index]));
				param->Pwm[index].PwmNum = 1;													//ms�ڲ��θ���
				index++;
			}
			else																				//����ʱ��ϳ��������
			{
				freq = PULSE_FREQ_AUXILIARY;
				duty = 0;
				PulseTran_Sub_CalcPwmPara(freq, duty, &(param->Pwm[index]));
				
				dTempVar = (msDealy - three->HighTime) * ((float_t)freq / 1000 / PULSE_FREQ_MAGNIFY);	
				param->Pwm[index].PwmNum = (uint32_t)dTempVar;									//ms�ڲ��θ���
				index++;
			}
		}
		
		/*	���Ĳ�	*/
		freq = three_freq;
		duty = three->HighTime * PULSE_DUTY_FULL / three->Period;
		PulseTran_Sub_CalcPwmPara(freq, duty, &(param->Pwm[index]));
		param->Pwm[index].PwmNum = 1;
		index++;
		
		/*	���岽	*/
		msDealy = three->Pause100ms * 100;
		if(msDealy != 0)
		{
			freq = PULSE_FREQ_AUXILIARY;
			duty = 0;
			PulseTran_Sub_CalcPwmPara(freq, duty, &(param->Pwm[index]));
			dTempVar = msDealy * ((float_t)freq / 1000 / PULSE_FREQ_MAGNIFY);
			param->Pwm[index].PwmNum = (uint32_t)dTempVar;			
			index++;
		}
		#if 0
			printf("PULSETRAN_THREE_p4\r\n");
		#endif
	}
	#if 0	
		printf("PwmNum30 = %u\r\n", param->Pwm[0].PwmNum);
		printf("PwmNum31 = %u\r\n", param->Pwm[1].PwmNum);
		printf("PwmNum32 = %u\r\n", param->Pwm[2].PwmNum);
		printf("PwmNum33 = %u\r\n", param->Pwm[3].PwmNum);
		printf("PULSETRAN_THREE_p3\r\n");
	#endif
	
	if(three->CycleNum == 0)
	{
		return RTN_ABNORMAL_NUM;
	}
	
	param->VarietyNum = index;
	param->CycleNum   = three->CycleNum;
	
	return RTN_NORMAL;
}

/********************************************************************
*	����	��	����ռ�ձ�
*	����	��	���ڸı䣬�ߵ�ƽʱ�䲻��
******************************************************************************/
static uint16_t PulseTran_Four_Sub_CalcDuty(uint16_t hightime, uint32_t curPeriod)
{
    double dutyCur;

	dutyCur = hightime * PULSE_DUTY_FULL / curPeriod;
    return (uint16_t)(dutyCur + 0.5);
} 
  
/***************************************************************************************************
*	����	��	�����ֲ��δ���
*	����	��	
				����׶Σ�ÿ���׶���n���ߵ�ƽʱ���̶����������Ա仯�Ĳ�����ɣ��׶μ�̶���ʱ
*	����	��	
				ÿ��ѭ���׶ο���ģ��ɶ���PWM���Σ�Ƶ�������ڻ��㣬ռ�ձ������ں͹̶��ߵ�ƽʱ������
				��һ��--�׶��ڲ�������
				�ڶ���--�׶μ���ʱʱ��
				�׶����ɳ�ʼ���ڡ���ֹ���ںͲ������ھ���
*******************************************************************************************************************/
eRtn_t PulseTran_Four(sPulseFour_t *four, sPwmArrayParam_t *param)
{
	uint8_t  KindNum = 0;
	uint32_t i = 0, index = 0;
	uint32_t SectionNum = 0, msDelay = 0;
	float_t  four_period = 0, freq = 0, duty = 0;
	
	#if 0
		printf("PULSETRAN_FOUR_p0\r\n");
	#endif
	if(param == NULL || four == NULL)
	{
		return RTN_ABNORMAL_PARAM;
	}
	#if 0
		printf("PULSETRAN_FOUR_p1\r\n");
	#endif
	if(four->Ampl == 0)
	{
		return RTN_ABNORMAL_AMPL;
	}
	else
	{
		param->Ampl = four->Ampl * 1000;
	}
	
	if((four->PeriodBegin == 0) && (four->PeriodStep == 0))										//�û���������ֵ����
	{
		return RTN_ABNORMAL_PARAM;
	}
	if(four->PeriodBegin == 0)
	{
		four->PeriodBegin += four->PeriodStep;
	}
	if(four->PeriodEnd == 0)
	{
		four->PeriodEnd += four->PeriodStep;
	}
	
	if(four->PeriodStep == 0)
	{
		SectionNum = 1;																			//����Ϊ0�����һ���׶�
	}
	else if(four->PeriodEnd > four->PeriodBegin)
	{
		SectionNum = (four->PeriodEnd - four->PeriodBegin) / four->PeriodStep;
	}
	else
	{
		SectionNum = (four->PeriodBegin - four->PeriodEnd) / four->PeriodStep;
	}
		
	if(four->PeriodEnd != 0 && four->PeriodBegin != 0)
	{
		SectionNum += 1;
	}
	if(SectionNum == 0)																			//[V134]��������ú�������������ֹ���ڲ�ֻ����һ��ʱ���ֳ����趨���ڷ�Χ������һ�����bug
	{
		SectionNum = 1;
	}
	if(four->PulseNum == 0)
	{
		return RTN_ABNORMAL_NUM;
	}
	else
	{
		KindNum = 2;
	}
	#if 0
		printf("SectionNum4 = %u\r\n", SectionNum);
		printf("TABLEINDEX4 = %u\r\n", sizeof(sPwmParam_t) * KindNum * SectionNum);
	#endif
	
	if(param->Pwm != NULL)
	{
		myfree(SRAMIN, param->Pwm);
	}
	
	param->Pwm = (sPwmParam_t *)mymalloc(SRAMIN, sizeof(sPwmParam_t) * KindNum * SectionNum);
	if(param->Pwm == NULL)
	{
		return RTN_ABNORMAL_SRAM;
	}
	#if 0
		printf("PULSETRAN_FOUR_p2\r\n");
	#endif
	
	four_period = four->PeriodBegin;															//����freq��Ϊ��ʱΪ������������д���
	for(i=0; i<SectionNum; i++)
	{
		WDG_Feed();																				//��ʱ�ϳ������ӿ��Ź�����������
		/*	��һ��	*/
		if(four->PeriodEnd > four->PeriodBegin)
		{
			four_period = four->PeriodBegin + four->PeriodStep * i;
		}
		else
		{
			if(four->PeriodBegin > four->PeriodStep * i)
			{
				four_period = four->PeriodBegin - four->PeriodStep * i;
			}
			else
			{
				four_period = four_period;														//����һfreqֵһ�£���ֹ��������
			}
		}
		freq = 1000 / four_period * PULSE_FREQ_MAGNIFY;
		duty = PulseTran_Four_Sub_CalcDuty(four->HighTime, four_period);
		PulseTran_Sub_CalcPwmPara(freq, duty, &(param->Pwm[index]));
		param->Pwm[index].PwmNum = four->PulseNum;
		index++;
		
		/*	�ڶ���	*/
		msDelay = four->Pause100ms * 100;
		if(msDelay != 0)
		{
			if(msDelay > 50 * 1000)
			{
				freq = PULSE_FREQ_AUXILIARY;
				duty = 0;
				PulseTran_Sub_CalcPwmPara(freq, duty, &(param->Pwm[index]));
				dTempVar = msDelay * ((float_t)freq / 1000 / PULSE_FREQ_MAGNIFY);
				param->Pwm[index].PwmNum = (uint32_t)dTempVar;
				index++;
			}
			else
			{
				freq = 1000 * PULSE_FREQ_MAGNIFY / msDelay;										//ԭ˳��1000 / msDelay * PULSE_FREQ_MAGNIFY�������������������쵼����������쳣
				duty = 0;
				PulseTran_Sub_CalcPwmPara(freq, duty, &(param->Pwm[index]));
				param->Pwm[index].PwmNum = 1;													//ms�ڲ��θ���
				index++;
			}
		}
		#if 0	
			printf("PULSETRAN_FOUR_p4\r\n");
			printf("index = %u\r\n", index);
		#endif
	}	
	#if 0	
		printf("PwmNum40 = %u\r\n", param->Pwm[0].PwmNum);
		printf("PwmNum41 = %u\r\n", param->Pwm[1].PwmNum);
		printf("PULSETRAN_FOUR_p3\r\n");
	#endif	
	
	if(four->CycleNum == 0)
	{
		return RTN_ABNORMAL_NUM;
	}
	
	param->VarietyNum = index;
	param->CycleNum   = four->CycleNum;
	
	return RTN_NORMAL;
}

/***************************************************************************************************
*	����	��	�����ֲ��δ���
*	����	��	
				����׶Σ������׶������õļ��ʱ�������ʱ����ɣ�����ʱ��
				�ٵڰ�ͨ��������n��Ƶ�ʹ̶���ռ�ձ�50%�Ĳ������
				������ͨ��������n��Ƶ�ʹ̶���ռ�ձ�100%�Ĳ������
*	����	��	
				ÿ���׶β��޹̶���ϵ��������׶δ��������ÿ���׶η�����
				��һ��--���ʱ�䣬����10K���θ���
				�ڶ���--����ʱ�䣬��������������θ���
*******************************************************************************************************************/
eRtn_t PulseTran_Five(sPulseFive_t *five, sPwmArrayParam_t *param)
{
	uint16_t index = 0, i = 0;
	uint32_t duty = 0;
	uint32_t freq = 0;
	
	if((five == NULL) || (param == NULL))
	{
		return RTN_ABNORMAL_PARAM;
	}
	
	if(param->Pwm != NULL)
	{
		myfree(SRAMIN, param->Pwm);
	}
	
	param->Pwm = (sPwmParam_t *)mymalloc(SRAMIN, sizeof(sPwmParam_t) * 2 * five->ConfigNum);
	if(param->Pwm == NULL)
	{
		return RTN_ABNORMAL_SRAM;
	}
	
	if(five->Ampl == 0)
	{
		return RTN_ABNORMAL_AMPL;
	}
	else
	{
		param->Ampl = five->Ampl * 1000;
	}	
	
	for(i=0; i<five->ConfigNum; i++)
	{
		/*	��һ��	*/
		if(five->MsPause[i])
		{
			freq = PULSE_FREQ_AUXILIARY;
			duty = 0;
			PulseTran_Sub_CalcPwmPara(freq, duty, &(param->Pwm[index]));
			dTempVar = five->MsPause[i] * ((float_t)freq / 1000 / PULSE_FREQ_MAGNIFY);
			param->Pwm[index].PwmNum = (uint32_t)dTempVar;
			index++;
		}
		
		/*	�ڶ���	*/
		if(five->MsDuration[i])
		{
			freq = five->Freq;
			duty = five->Duty;
			PulseTran_Sub_CalcPwmPara(freq, duty, &(param->Pwm[index]));
			dTempVar = five->MsDuration[i] * ((float_t)freq / 1000 / PULSE_FREQ_MAGNIFY);
			param->Pwm[index].PwmNum = (uint32_t)dTempVar;
			index++;
		}
		else
		{
			freq = five->Freq;
			duty = 0;																			//ʱ��Ϊ0ʱʹ��ռ�ձ�Ϊ0�������[V138]
			PulseTran_Sub_CalcPwmPara(freq, duty, &(param->Pwm[index]));
			dTempVar = five->MsDuration[i] * ((float_t)freq / 1000 / PULSE_FREQ_MAGNIFY);
			param->Pwm[index].PwmNum = (uint32_t)dTempVar;
			index++;
		}
	}
	
	if(index == 0)
	{
		return RTN_ABNORMAL_NUM;
	}
	
	param->VarietyNum = index;
	param->CycleNum   = 1;
	
	return RTN_NORMAL;
}

/***************************************************************************************************
*	����	��	Single���δ���
*	����	��	
				�����ߵ�ƽ����
*	����	��	
				����Ƶ�ʹ������������ģ���100ռ�ձȵĲ���
				ÿ���׶εĳ��ȳ���ʱ���ۺϳ�PWM�������̶�Ƶ��10kHz,����100us
*	����	��					
				sPwmArrayParam_t--����Ϊ���õĲ�������
*******************************************************************************************************************/
eRtn_t PulseTran_Single(sPwmArrayParam_t *param)
{
	uint32_t duty  = 0;
	float_t  freq  = 0;
	
	if(param->Pwm != NULL)																//����Ѿ������ڴ棬�����ͷ����������룬��ֹ�ڴ�й¶�����
	{
		myfree(SRAMIN, param->Pwm);
	}
	
	param->Pwm = (sPwmParam_t *)mymalloc(SRAMIN, sizeof(sPwmParam_t) * UserOperation.bPhase);
	if(param->Pwm == NULL)
	{
		return RTN_ABNORMAL_SRAM;
	}	
	
	if (UserOperation.bVC == SELECT_VC_V)
	{
		param->Ampl = UserOperation.V_ModeSingle.Param[UO_PARAM_AMPL];
	}
	else
	{
		param->Ampl = UserOperation.C_ModeSingle.Param[UO_PARAM_AMPL];
	}
	
	if(param->Ampl == 0)
	{
		return RTN_ABNORMAL_AMPL;
	}
	
	freq = PULSE_FREQ_AUXILIARY;
	duty = PULSE_DUTY_FULL;
	PulseTran_Sub_CalcPwmPara(freq, duty, &(param->Pwm[0]));
	
	if (UserOperation.bVC == SELECT_VC_V)
	{
		dTempVar = UserOperation.V_ModeSingle.Param[UO_PARAM_PULSE] / 1000 * ((float_t)freq / 1000 / PULSE_FREQ_MAGNIFY) / 1000;
	}
	else
	{
		dTempVar = UserOperation.C_ModeSingle.Param[UO_PARAM_PULSE] / 1000 * ((float_t)freq / 1000 / PULSE_FREQ_MAGNIFY) / 1000;
	}
	param->Pwm[0].PwmNum = (uint32_t)dTempVar;
	if(UserOperation.bPhase == UO_PHASE_BIPHASE)
	{
		PulseTran_Sub_CalcPwmPara(freq, duty, &(param->Pwm[1]));
		param->Pwm[1].PwmNum = (uint32_t)dTempVar;					//�ݰ�˫��ȿ�򻯴���
	}
	
	if(param->Pwm[0].PwmNum == 0)
	{
		return RTN_ABNORMAL_NUM;
	}

	param->VarietyNum = UserOperation.bPhase;
	param->CycleNum = 1;
	
	return RTN_NORMAL;
}

/***************************************************************************************************
*	����	��	FreeRun���δ���
*	����	��	
				���ֲ�������������
*	����	��	
				
*	����	��					
				sPwmArrayParam_t--����Ϊ���õĲ�������
*******************************************************************************************************************/
eRtn_t PulseTran_FreeRun(sPwmArrayParam_t *param)
{
	uint32_t duty  = 0;																						//[V176]ԭ������uint16_t��Χ̫С�����������������쳣
	float_t  freq  = 0;
	
	if(param->Pwm != NULL)
	{
		myfree(SRAMIN, param->Pwm);
	}
	
	param->Pwm = (sPwmParam_t *)mymalloc(SRAMIN, sizeof(sPwmParam_t) * (UserOperation.bPhase + 1));
	if(param->Pwm == NULL)
	{
		return RTN_ABNORMAL_SRAM;
	}
	

	if (UserOperation.bVC == SELECT_VC_V)
	{
		param->Ampl = UserOperation.V_ModeFreeRun.Param[UO_PARAM_AMPL];
		
		freq = UserOperation.V_ModeFreeRun.Param[UO_PARAM_FREQ] / 1000;
		duty = UserOperation.V_ModeFreeRun.Param[UO_PARAM_PULSE] * freq * PULSE_DUTY_FULL / 1000000000;		//UserOperation.V_ModeFreeRun.Param[UO_PARAM_PULSE] / 1000000000 * freq * PULSE_DUTY_FULL;		
	}
	else
	{
		param->Ampl = UserOperation.C_ModeFreeRun.Param[UO_PARAM_AMPL];
		
		freq = UserOperation.C_ModeFreeRun.Param[UO_PARAM_FREQ] / 1000;
		duty = UserOperation.C_ModeFreeRun.Param[UO_PARAM_PULSE] * freq * PULSE_DUTY_FULL / 1000000000;
	}
	
	if(param->Ampl == 0)
	{
		return RTN_ABNORMAL_AMPL;
	}
	if(freq == 0)
	{
		return RTN_ABNORMAL_FREQ;
	}
	if(duty == 0)
	{
		return RTN_ABNORMAL_DUTY;
	}
	
	PulseTran_Sub_CalcPwmPara(freq * PULSE_FREQ_MAGNIFY, duty, &(param->Pwm[0]));			//[V182]�޸�bug��Ϊ������ԭЭ����ݱ�����PULSE_FREQ_MAGNIFY��������ʱ��©
	
	param->Pwm[0].PwmNum = 65535;															//ͨ������ж��жԱȽ�ֵ���������ֵ��֤���������
	
	param->VarietyNum = 1;
	param->CycleNum = 1;
	
	
	return RTN_NORMAL;
}

/***************************************************************************************************
*	����	��	Train���δ���
*	����	��	
				���ֲ�������������ָ��ʱ���
*	����	��	
				
*	����	��					
				sPwmArrayParam_t--����Ϊ���õĲ�������
*******************************************************************************************************************/
eRtn_t PulseTran_Train(sPwmArrayParam_t *param)
{
	uint32_t duty  = 0;
	float_t  freq  = 0;
	
	if(param->Pwm != NULL)
	{
		myfree(SRAMIN, param->Pwm);
	}
	
	param->Pwm = (sPwmParam_t *)mymalloc(SRAMIN, sizeof(sPwmParam_t) * (UserOperation.bPhase + 1));
	if(param->Pwm == NULL)
	{
		return RTN_ABNORMAL_SRAM;
	}
	
	if (UserOperation.bVC == SELECT_VC_V)
	{
		param->Ampl = UserOperation.V_ModeTrain.Param[UO_PARAM_AMPL];
	}
	else
	{
		param->Ampl = UserOperation.C_ModeTrain.Param[UO_PARAM_AMPL];
	}
	
	if(param->Ampl == 0)
	{
		return RTN_ABNORMAL_AMPL;
	}

    if (UserOperation.bVC == SELECT_VC_V)
	{
		param->Ampl = UserOperation.V_ModeTrain.Param[UO_PARAM_AMPL];
		
		freq = UserOperation.V_ModeTrain.Param[UO_PARAM_FREQ] / 1000;						//���ֳ�bug������Ը�PULSE_FREQ_MAGNIFY���޸Ĺ����д�����ʣ�û�м�ʱ�����������̣�û��ʵ����ϸ����
		duty = UserOperation.V_ModeTrain.Param[UO_PARAM_PULSE] * freq * PULSE_DUTY_FULL / 1000000000;
	}
	else
	{
		param->Ampl = UserOperation.C_ModeTrain.Param[UO_PARAM_AMPL];
		
		freq = UserOperation.C_ModeTrain.Param[UO_PARAM_FREQ] / 1000;
		duty = UserOperation.C_ModeTrain.Param[UO_PARAM_PULSE] * freq * PULSE_DUTY_FULL / 1000000000;
	}
	
	if(param->Ampl == 0)
	{
		return RTN_ABNORMAL_AMPL;
	}
	if(freq == 0)
	{
		return RTN_ABNORMAL_FREQ;
	}
	if(duty == 0)
	{
		return RTN_ABNORMAL_DUTY;
	}

	PulseTran_Sub_CalcPwmPara(freq * PULSE_FREQ_MAGNIFY, duty, &(param->Pwm[0]));
	
	param->Pwm[0].PwmNum = UserOperation.V_ModeTrain.Param[UO_PARAM_DURATION];
	
	if (UserOperation.bVC == SELECT_VC_V)
	{
		param->Pwm[0].PwmNum = UserOperation.V_ModeTrain.Param[UO_PARAM_DURATION] / 1000 * freq / 1000;		//[V190],freq��λΪHz��ֱ��ʹ�ü���
	}
	else
	{
		param->Pwm[0].PwmNum = UserOperation.C_ModeTrain.Param[UO_PARAM_DURATION] / 1000 * freq / 1000;
	}
	if(param->Pwm[0].PwmNum == 0)
	{
		return RTN_ABNORMAL_NUM;
	}
	
	param->VarietyNum = 1;
	param->CycleNum = 1;
	
	return RTN_NORMAL;
}
