
/********************************************************************
*
*	�ļ�	��	TIMER.h

*	�ܽ�	��	(NONE)

*	����	��	�˴�Ӧ��������ʱ����TIM5��TIM9��
*	TIM5: 32bit��ʱ����MAX TIM CLK=84M������������1us-300s

* TIM3: 16bit��ʱ����MAX TIM CLK=84M��������Ƶ�ʣ����1Hz��1s�����30K��33.333ms
******************************************************************************/
#ifndef _TIMER_H
#define _TIMER_H
#include "main.h"



typedef struct
{
	uint32_t OutputCnt;
}sTimer2_t;
extern sTimer2_t T2;

/*	1minute = 60second = 60*1000millisecond	*/
#define T2_VALUE_MICROperCYCLE	(100)
#define T2_VALUE_MILLIS			(1000 / T2_VALUE_MICROperCYCLE)
#define T2_VALUE_SEC  			(T2_VALUE_MILLIS * 1000)
#define T2_VALUE_MIN  			(T2_VALUE_SEC * 60)

#define ARR_1S		7199
#define PSC_1S		9999
#define ARR_100MS	719
#define PSC_100MS	9999
#define ARR_10MS	719
#define PSC_10MS	999
#define ARR_1MS		719
#define PSC_1MS		99
#define ARR_100US	719
#define PSC_100US	9
#define ARR_50US	359
#define PSC_50US	9
#define ARR_10US	71
#define PSC_10US	9

void Timer2_Init(u32 arr,u16 psc);








#define TIM5_ENABLE()			TIM5->CR1 |= TIM_CR1_CEN
#define TIM5_DISABLE()		TIM5->CR1 &= (uint16_t)~TIM_CR1_CEN;

#define TIM3_ENABLE()			TIM3->CR1 |= TIM_CR1_CEN
#define TIM3_DISABLE()		TIM3->CR1 &= (uint16_t)~TIM_CR1_CEN;


extern volatile unsigned int TIM5_IRQ_Count;

void Timer5_Init(u32 arr, u16 psc);
void Timer3_Init(u16 arr, u16 psc);

void Enable_Timer5(u32 arr);
void Disable_Timer5(void);

#endif
