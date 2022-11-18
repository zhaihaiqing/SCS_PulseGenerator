
#include  "HeaderFiles.h"

#define PWM_ARR		10000
#define PWM_PSC		720
/********************************************************************
*	����	��	��ʼ��PWM
*	����	��	Ƶ�� = 72*1000000/((arr+1)*(psc+1))
*				psc--Ԥ��Ƶֵ��������������
				arr--�Զ���װ��ֵ��һ���������μ�������ֵ
******************************************************************************/
void Pwm_Init(void)
{	
	RCC->APB2ENR |= 1 << 3;
	RCC->APB1ENR |= 1 << 2;
	GPIOB->CRH   &= 0XFFFFFFF0;
	GPIOB->CRH   |= 0X0000000B;
	TIM4->ARR     = PWM_ARR-1;		//�Զ���װ��ֵ,ARR
	TIM4->PSC     = PWM_PSC-1;		//Ԥ��Ƶֵ��PSC
	
	TIM4->CCMR2  |= 6 << 4;			//TIM3 CH3,���ϼ���ʱ��TIMx_CNT<TIMx_CNT_CCRxͨ���ǻ��
	TIM4->CCMR2  |= 1 << 3;
	TIM4->CCER   |= 1 << 8;			//OC3 ENABLE
	TIM4->CR1    |= 0X0080;
	TIM4->CR1    |= 0X01;
	
	TIM4->CCR3    = PWM_ARR;				//��ʼһ��ֵ
}

/********************************************************************
*	����	��	����ߵ�ƽ��ռǧ�ֱ�
******************************************************************************/
void Pwm_WidthSet(uint16_t permillage)
{
	TIM4->CCR3 = permillage * (PWM_ARR / 1000);
}

