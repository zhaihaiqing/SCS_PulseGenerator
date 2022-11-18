
/********************************************************************
*
*	�ļ�	��	ADC.h

*	�ܽ�	��	
				PA0 --ADC_HV
				PA1 --ADC_LV

*	����	��	��ѹ�ɼ�
*
******************************************************************************/
#ifndef __ADC_H
#define __ADC_H
#include "HeaderFiles.h"

#define ADC_INTERVAL	(5 * T_VALUE_MILLISEC)

#define ADC_CHNL_VH		0
#define ADC_CHNL_VL		1

extern float Volt_High, Volt_Low;

void ADC_Init(void);
void ADC_Poll(void);

#endif
