/********************************************************************
*
*	�ļ�	��	Output.h

*	�ܽ�	��	
				

*	����	��	��ѹ/�����ն��������
*
******************************************************************************/
#ifndef __OUTPUT_H
#define __OUTPUT_H
#include "main.h"

#define OUTPUT_DISABLE	0
#define OUTPUT_ENABLE	1

void Output_VorC(uint8_t select_vc, int64_t ampl, uint8_t IsEnable);

//void EXIT2_Init(void);

//extern volatile unsigned char EXIT2_Egde_Count;

#if(TEST_OUTPUT)
	#define TEST_VOLT_L		0
	#define TEST_VOLT_H		1
	#define TEST_CURR		2
	void test_OUTPUT(uint8_t mode);
#endif

#endif

/**********************************************************************
* �ɹ�0.1%������в���
  ���赵λ	ѹ����1V����������
    1K		1.000mA		1000uA
	2K		0.500mA	 	 500uA
	3K		0.330mA	 	 330uA
    5K		0.200mA	 	 200uA
   10K		0.100mA	 	 100uA
   20K		0.050mA	  	  50uA
  100K		0.010mA	  	  10uA
  150K 		0.006mA   	   6uA

* ABOUT:NI USB-6255
		ADC resolution		16 bits
		Input range			��0.1V����0.2V����0.5V����1V����2V����5V����10V
		Input current during overvoltage condition		��20mA maximum/AI pin
		
* ABOUT:POWER��1/8W��
  ���赵λ��		�������������----��Ӧ��ѹ��
  100��			35.35mA				 3.5V
    1K 			11.18mA				11.1V
   10K			 3.53mA				35.3V
   
  ���Ե���ѡ�ͣ�
	��ѹL��		10K
	��ѹH��		10K + 100K
	���� ��		100��
*********************************************************************************/
