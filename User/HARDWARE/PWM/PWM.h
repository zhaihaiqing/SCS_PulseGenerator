
/********************************************************************
*
*	�ļ�	��	PWM.h

*	�ܽ�	��	
				PB8[TIM4 CH3]

*	����	��	�������
*
******************************************************************************/
#ifndef __PWM_H
#define __PWM_H
#include "HeaderFiles.h"

void Pwm_Init(void);
void Pwm_WidthSet(uint16_t permillage);

#endif
