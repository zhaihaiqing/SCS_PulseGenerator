
/********************************************************************
*
*	�ļ�	��	Led.h

*	�ܽ�	��	
//				PE6		--LEDBD/CTRLMODE
//				
//				PA8		--LED1			--SINGLE
//				PA11	--LED3			--TRAIN
//				PA12	--LED2			--FREERUN
//				PB6		--LED_POWER		--LED POWER
//				PB7		--LED_OVERLOAD	--LED OVERLOAD
//				PB8		--LED_OUTPUT	--LED OUTPUT
//				PB9		--LED4			--EXTTRI
//				PC14	--LED5			--V/C
//				PD2		--LED6			--AMPLE
//				PD3		--LED7			--PULSE
//				PD6		--LED8			--FREQ
//				PD12	--LED9			--DURATION
//				PD13	--LED10			--UNIT SWITCH
//				PE0		--LED11			--RUN
//				PE1		--LED12			--PAUSE
//				PE2		--LED13			--FLIP
//				PE3		--LED14			--SINGGLE TRIGGER
//				PE4		--NC
//				
				
				
				
				PE6		--LEDBD/CTRLMODE
				
				PA0		--LED_OUTPUT	--LED OUTPUT 
				
				PA8		--LED1			--SINGLE
				PA11	--LED3			--TRAIN
				PA12	--LED2			--FREERUN
				
				PB6		--LED_POWER		--LED POWER
				PB7		--LED_OVERLOAD	--LED OVERLOAD
				PB9		--LED4			--EXTTRI
				
				
				
				PD2		--LED6			--AMPLE
				PD3		--LED7			--PULSE
				PD6		--LED8			--FREQ
				PD12	--LED9			--DURATION
				PD13	--LED10			--UNIT SWITCH
				
				PE0		--LED11			--RUN
				PE1		--LED12			--PAUSE
				PE2		--LED13			--FLIP
				PE3		--LED14			--SINGGLE TRIGGER
				PE4		--NC
				
				

*	����	��	ָʾ��
*
******************************************************************************/
#ifndef __LED_H
#define __LED_H
#include "main.h"

#define LED_DIRECTLY_ON		0
#define LED_DIRECTLY_OFF	1
#define LED_SN74HC240_ON	1
#define LED_SN74HC240_OFF	0

#define pLEDPOWER			PBout(6)		//ǰ���ָʾ�ƣ��û�ʹ��
#if(PCB_VERSION == PCB_V14)
	#define pLEDOUTPUT		PBout(8)
#elif(PCB_VERSION >= PCB_V15)
	#define pLEDOUTPUT		PAout(0)
#endif
#define pLEDOVERLOAD		PBout(7)

#define pLEDRUN				PEout(0)		//����ָʾ��
#define pLEDPAUSE			PEout(1)
#define pLEDFLIP			PEout(2)
#define pLEDSINGLETRIGGER	PEout(3)		//����

//#define pLEDVC				PCout(14)		//����

#define pLEDUNITSWITCH		PDout(13)		//����
#define pLEDSINGLE			PAout(8)
#define pLEDFREERUN			PAout(12)
#define pLEDTRAIN			PAout(11)
#define pLEDEXTBNC			PBout(9)
#define pLEDAMPL			PDout(2)
#define pLEDPULSE			PDout(3)
#define pLEDFREQUENCY		PDout(6)
#define pLEDDURATION		PDout(12)


#define pLEDBD			PEout(6)		//����ָʾ�ƣ�����ʹ��
	
#if(TEST_LED)
	void test_LED(void);
	void test_T_LED(void);
#else
	#define CTRLMODE_MANNUAL	0
	#define CTRLMODE_COMPUTER	1
	#define pCTRLMODE		PEin(6)
	void CtrlMode_Poll(void);
#endif

#define LEDSHORTON_BEGIN	2
#define LEDSHORTON_ING		1
#define LEDSHORTON_END		0
#define LEDSHORTONTIME		500				//����ʱ��[ms]
typedef struct LEDSHORTON
{
	uint8_t  fSinggleTrigger;				//0--�ر�	1--������	2--������ʼ
	uint16_t SingleTriggerCountDown;
	
	uint8_t  fVC;
	uint16_t VCCountDown;
	
	uint8_t  fUnitSwitch;
	uint16_t UnitSwitchCountDown;
}s_LedShortOn;
extern s_LedShortOn	LedShortOn;

void LED_Init(void);
void Led_ModePartOff(void);
void Led_ParamPartOff(void);
void Led_ShortOn_Poll(void);

#endif
