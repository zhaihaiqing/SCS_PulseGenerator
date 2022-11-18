
/********************************************************************
*
*	�ļ�	��	Codec.h

*	�ܽ�	��	
				PB12--BTN
				PB13--PULSE-A
				PB14--PULSE-B
				PB15--RSV

*	����	��	������/��ť
*
******************************************************************************/
#ifndef __CODEC_H
#define __CODEC_H
#include "main.h"

#define ROTATE_SPEED_BOUNDARY_MS	250	//���١����ٷֽ�ֵ[ms]
#define ROTATE_BOUNDARY_TIMES		4	//�����������������һ����Ч��ֵ��Ҫ��⵽�Ĵ���

#define ROTATE_STEP_SLOW			1
#define ROTATE_STEP_QUICK			5

#define PULSE_A	PBin(13)
#define PULSE_B	PBin(14)

extern uint8_t Rotate_Increase_Cnt, Rotate_Decrease_Cnt;

void Codec_Init(void);

#endif
