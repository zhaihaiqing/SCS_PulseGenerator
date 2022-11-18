
/********************************************************************
*
*	�ļ�	��	DAC.h

*	�ܽ�	��	
				PA2 --AD5542_CS1
				PA3 --AD5542_CS2
				PA4 --AD5542_LDAC
				PA5 --AD5542_SCLK
				PA6 --AD5542_SDIN
				
				
				
				PA7 --��������ƣ��̵������ƣ�
				PB2 --ģʽ�л��������л���ѹģʽ������ģʽ
				



*	����	��	ģ���ź����
*	�����ϵ��	
				˫���Ե���Դ-----125�������������裬����-4V��+4V��Ӧ���-32mA��+32mA
				˫���Ե�ѹԴ-----25���Ŵ�����-4V��+4V��Ӧ���-100V��+100V
*
******************************************************************************/
#ifndef __DAC_H
#define __DAC_H
#include "main.h"

#define DA_OUTPUTRANGE_10000MV	10000		//10V
#define DA_OUTPUTRANGE_10800MV	10800		//10.8V
#define DA_OUTPUTRANGE_4096MV	4096		//4.96V
//#define DA_OUTPUTRANGE			DA_OUTPUTRANGE_10800MV
#define DA_OUTPUTRANGE			DA_OUTPUTRANGE_4096MV

//#define DA_CS1  	PAout(2)
//#define DA_CS2	PAout(3)
//#define DA_SDIN 	PAout(6)
//#define DA_SCLK	PAout(5)
//#define DA_LDAC 	PAout(4)


#define DA_CS1_H()	GPIOA->BSRRL = GPIO_Pin_2;
#define DA_CS1_L()	GPIOA->BSRRH = GPIO_Pin_2;

#define DA_CS2_H()	GPIOA->BSRRL = GPIO_Pin_3;
#define DA_CS2_L()	GPIOA->BSRRH = GPIO_Pin_3;

#define DA_LDAC_H()	GPIOA->BSRRL = GPIO_Pin_4;
#define DA_LDAC_L()	GPIOA->BSRRH = GPIO_Pin_4;

#define DA_SCLK_H()	GPIOA->BSRRL = GPIO_Pin_5;
#define DA_SCLK_L()	GPIOA->BSRRH = GPIO_Pin_5;

#define DA_SDIN_H()	GPIOA->BSRRL = GPIO_Pin_6;
#define DA_SDIN_L()	GPIOA->BSRRH = GPIO_Pin_6;


#define DA_CHNL_VOLT	1
#define DA_CHNL_CURR	2
#define DA_CHNL_ALL		3


void AD5542_Init(void);
void AD5542_Output(uint8_t ch, int32_t value);

#endif

/**********************************************************************
��ѹ����
			�����Χ-99.9V��+99.9V�����ɵ�
			���������50mA,��·�ɼ�⣨�޼�⣩
			
			����迹��С��1000ŷ
			����Ƶ��10Hz-50KHz�����ɵ�
��������
			�����Χ2uA-32mA�����ɵ�
			�������ѹ100V����·�ɼ�⣨�޼�⣩
			����迹��С��1000ŷ
			����Ƶ��10Hz-50KHz�����ɵ�
********************************************************************************/
