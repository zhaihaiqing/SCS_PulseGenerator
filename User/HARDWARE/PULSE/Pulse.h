
/********************************************************************
*
*	�ļ�	��	Pulse.h

*	�ܽ�	��	
				PB8		--TIM4 CH3

*	����	��	�����������
*
******************************************************************************/
#ifndef __PULSE_H
#define __PULSE_H
#include "main.h"

/*	ԭSYNC_ARR=65455	SYNC_PSC=219������ȷʵΪ����0.199s����ԭ��	*/
#define SYNC_ARR 39
#define SYNC_PSC 41
#define SYNC_CCR 0

extern uint8_t Tbl_Flag[8];

typedef enum
{
	DOSTATE_STATUS_RUNNING = 0,
	DOSTATE_STATUS_COMPLETE,
	DOSTATE_STATUS_PAUSE,
}DO_STATUS_E;

__packed typedef struct
{
	uint8_t 	Config;				//��ͨһ���岨�����ñ�־��bit7~bit0--8·����Ƿ�����
	
	DO_STATUS_E Status[8];			//���б�־��0--���У�1--��ɣ�2--��ͣ
	
	uint8_t		ZeroConfig;			//��������ֲ������ñ�־������config����һ�£�����ʵ�ַ�ʽ��ͬ������ͳһ��һ��
	uint16_t    ZeroIndexCnt[8];	//������������	
}sDOState_t;						//DATA OUTPUT--PWM
extern sDOState_t DOState;

__packed typedef struct
{
	uint16_t Index;					//���β��������±�
	uint32_t PulseNum;				//����ռ�ձȲ��θ���
	uint32_t CycleCnt;				//ѭ������
}sTimerRun_t;

/*	��ʱ��ͨ���Ŷ�Ӧ����	*/
#define DO_TIM11	0
#define DO_TIM4		1
#define DO_TIM3		2
#define DO_TIM9		3
#define DO_TIM2		4
#define DO_TIM8		5
#define DO_TIM1		6
#define DO_TIM5		7
#define DO_ALL		8
#define DO_NO		0XFF

extern volatile uint32_t *Tbl_TIM_CNT[8];
extern volatile uint32_t *Tbl_TIM_ARR[8];
extern volatile uint16_t *Tbl_TIM_PSC[8];
extern volatile uint32_t *Tbl_TIM_CCRx[8];
extern volatile uint16_t *Tbl_TIM_DIER[8];
extern volatile uint16_t *Tbl_TIM_CR1[8];

extern sTimerRun_t TimerRun[8];

extern void (*Tbl_DOX_TIM_Init[8])(uint16_t arr, uint16_t psc);
extern void DO0_PB9_TIM11_CH1(uint16_t arr, uint16_t psc);		//��ʱδʹ��ͨ�����ɹ���չ
extern void DO1_PB8_TIM4_CH3(uint16_t arr, uint16_t psc);
extern void DO2_PA6_TIM3_CH1(uint16_t arr, uint16_t psc);		//��ʱδʹ��ͨ�����ɹ���չ
extern void DO3_PA2_TIM9_CH1(uint16_t arr, uint16_t psc);		//��ʱδʹ��ͨ�����ɹ���չ
extern void DO4_PB10_TIM2_CH3_F(uint16_t arr, uint16_t psc);	//��ʱδʹ��ͨ�����ɹ���չ
extern void DO5_PC6_TIM8_CH1(uint16_t arr, uint16_t psc);		//��ʱδʹ��ͨ�����ɹ���չ
extern void DO6_PA11_TIM1_CH4(uint16_t arr, uint16_t psc);		//��ʱδʹ��ͨ�����ɹ���չ
extern void DO7_PA3_TIM5_CH4(uint16_t arr, uint16_t psc);		//��ʱδʹ��ͨ�����ɹ���չ
void DO_ALL_SYNC_Init(uint8_t do_timx);

#endif
