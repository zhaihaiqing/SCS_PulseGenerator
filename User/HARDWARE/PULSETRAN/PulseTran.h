
/********************************************************************
*
*	�ļ�	��	PulseTran.h

*	�ܽ�	��	(NONE)

*	����	��	�Խ��յ������ݽ��н���������
				������������λ������
*
******************************************************************************/
#ifndef _PULSETRAN_H
#define _PULSETRAN_H
#include "main.h"

#if(PCB_VERSION >= PCB_V15)

#define PULSE_FREQ_MAGNIFY 		  100.0f		// Ƶ�ʻ�����С�������λ��Ч����	freq = freq / 100.0		100 = 1hz
//#define PULSE_UNIT_DUTY   	 1000.0f		// ռ�ձȻ���						duty = duty / 1000.0	500 = 50%��old version
#define PULSE_DUTY_FULL			50000.0f		// �°�ռ�ձ�Э��					duty = duty / 50000  	50000 = 100%
#define PULSE_FREQ_LOWERLIMIT	    2.0f		// Ƶ����Ч��Χ��0.02Hz�� 30KHz������PULSE_FREQ_MAGNIFY�ʴ˴�Ϊ2

//#define PULSE_FREQ_5K	  		 500000
//#define PULSE_FREQ_10K  		1000000 		// 10khz  10 * 1000 * 100������Ҫֻ����͵�ƽ��ߵ�ƽʱ�õ�Ƶ�ʻ�׼
//#define PULSE_FREQ_20K  		2000000
//#define PULSE_FREQ_30K		3000000
#define PULSE_FREQ_50K			5000000
#define PULSE_FREQ_AUXILIARY	PULSE_FREQ_50K	//����Ƶ�ʣ������û�����Ƶ�ʣ����Ǹ����û����ò���������������Ƶ��

__packed typedef struct
{
	uint32_t PwmNum;							//ͬһռ�ձȲ�����������
	uint16_t PwmARR;							//��ǰ������������
	uint16_t PwmPSC;							//��Ƶ
	uint16_t PwmDuty;							//ռ�ձ�
}sPwmParam_t;

__packed typedef struct
{
	uint32_t	Ampl;							//�����ֵ
	
	uint32_t	CycleNum;						//ѭ������
	uint32_t	VarietyNum;						//PWM������
	sPwmParam_t	*Pwm;							//����PWM����
}sPwmArrayParam_t;
extern sPwmArrayParam_t *pPwmArrayParam[8];

typedef enum
{
	RTN_NORMAL			= 0,					//��������
	RTN_ABNORMAL_AMPL	= 1,					//��ֵ��Ϊ0���쳣
	RTN_ABNORMAL_FREQ	= 2,					//Ƶ�ʣ�Ϊ0���쳣
	RTN_ABNORMAL_DUTY	= 3,					//ռ�ȣ�Ϊ0���쳣
	RTN_ABNORMAL_NUM	= 4,					//������Ϊ0���쳣
	RTN_ABNORMAL_SRAM	= 5,					//�洢��Ϊ0���쳣
	RTN_ABNORMAL_PARAM	= 6,					//������Ϊ0���쳣
	RTN_ABNORMAL_OTHER	= 7,					//�����쳣
}eRtn_t;

eRtn_t PulseTran_Zero(sPulseZero_t *zero, sPwmArrayParam_t *param);
eRtn_t PulseTran_One(sPulseOne_t *one, sPwmArrayParam_t *param);
eRtn_t PulseTran_Two(sPulseTwo_t *two, sPwmArrayParam_t *param);
eRtn_t PulseTran_Three(sPulseThree_t *three, sPwmArrayParam_t *param);
eRtn_t PulseTran_Four(sPulseFour_t *four, sPwmArrayParam_t *param);
eRtn_t PulseTran_Five(sPulseFive_t *five, sPwmArrayParam_t *param);

eRtn_t PulseTran_Single(sPwmArrayParam_t *param);
eRtn_t PulseTran_FreeRun(sPwmArrayParam_t *param);
eRtn_t PulseTran_Train(sPwmArrayParam_t *param);

#endif

#endif
