
/********************************************************************
*
*	�ļ�	��	Manual.h

*	�ܽ�	��					

*	����	��	������Ӧ�����ò�������������ȣ�
*
******************************************************************************/
#ifndef __MANUAL_H
#define __MANUAL_H
#include "main.h"

#define KBD_INPUT_0					0
#define KBD_INPUT_1					1
#define KBD_INPUT_2					2
#define	KBD_INPUT_3					3
#define KBD_INPUT_4					4
#define KBD_INPUT_5					5
#define KBD_INPUT_6					6
#define KBD_INPUT_7					7
#define KBD_INPUT_8					8
#define KBD_INPUT_9					9
#define KBD_INPUT_BACKSPACE			11
#define KBD_INPUT_CLEAR				12
#define KBD_INPUT_CODEC_INC_SLOW	13
#define KBD_INPUT_CODEC_INC_QUICK	14
#define KBD_INPUT_CODEC_DEC_SLOW	15
#define KBD_INPUT_CODEC_DEC_QUICK	16

#define VALUE_EXPAND_105	1000000		//��������и�������λĬ��С�����6λ��Ч����ÿ������������1000000��������������ʾ����
#define VALUE_EXPAND_102	1000		//��������и�����С��λĬ��С�����3λ��Ч����ÿ������������1000��������������ʾ����

/*	�������ޣ��û�����ֵ����ǿ������Ϊ����ֵ	*/
#define CEILING_V_AMPLITUDE	(uint64_t)150      *1000000		//uV,��150V
#define CEILING_C_AMPLITUDE	(uint64_t)32       *1000000		//nA,32mA	//[V193]��XTR111������96mA
#define CEILING_PULSE		(uint64_t)5*60*1000*1000000		//ns,5min
#define CEILING_FREQUENCY	(uint64_t)30       *1000000		//mHz,30KHz
#define CEILING_DURATION	(uint64_t)7*24*60  *1000000		//us,24h

extern uint64_t Ceiling[4];

typedef struct
{
	/*	������λ��Ϣ	*/
	uint8_t  fUnit;							//SINGLE  FREERUN  TRAIN  EXTBNC
	/*	NAD--Num After Dot���û�����С������Ϣ��������ť����	*/	
	uint8_t  ParamNAD[4];					//UO_PARAM_E
//	uint8_t  ParamNAD[UO_PARAM_AMPL];		//SINGLE  FREERUN  TRAIN  EXTBNC
//	uint8_t  ParamNAD[UO_PARAM_PULSE];		//SINGLE  FREERUN  TRAIN
//	uint8_t  ParamNAD[UO_PARAM_FREQ];		//        FREERUN  TRAIN
//	uint8_t  ParamNAD[UO_PARAM_DURATION];	//                 TRAIN
	/*	����ֵ	*/	
	uint64_t Param[4];
//	uint64_t Ampl;							//SINGLE  FREERUN  TRAIN  EXTBNC
//	uint64_t Pulse;							//SINGLE  FREERUN  TRAIN
//	uint64_t Freq;							//        FREERUN  TRAIN
//	uint64_t Duration;						//                 TRAIN
}sModeParam_t;

typedef struct
{										//��ȡ������Ϣ������ֱ��Ӱ�����ñ���������ͬ���жϴ���
	uint8_t  		bUnit;				//�������ò�����λ��0--С��λ	1--��λ����λΪС��λ1000����
	sModeParam_t	*pModeParam;		//�������ö�Ӧģʽ�����׵�ַ
	uint8_t			*pParamNAD;			//�������ò���С����λ��
	uint64_t		ParamValue;			//�������ò���ֵ
	uint64_t		Ceiling;			//�������ò�������ֵ������ǿ������Ϊ����
}sGetSetupData_t;

typedef struct
{										//�޸Ĳ���ֵ
	uint8_t fStart;						//0--�������  1--������ʼ״̬  2--���ý����С�������־λ����Ϊ1ʱ���û���5�����ʱ���ò���ֵΪ5���ô˱�־λΪ2����������һλ��ʼ��������
	uint8_t bUnitSwitched;				//��λ�л���־λ�����ڽ���������ù������л���λ���´����������.��λ�л���ת״̬��Ϊ1ʱ���������������������룬�����л���ָ���0
	uint8_t NumAfterDot;				//0--none	   1~6--�������õ�С�����λ��
}sModify_t;

typedef enum
{
	UO_PHASE_UNIPHASE	= 0X01,
	UO_PHASE_BIPHASE	= 0X02,
}UO_PHASE_E;

/*	UserOperation.fMode			*/
typedef enum
{
	UO_MODE_NONE 		= 0xff,
	UO_MODE_SINGLE 		= 0,
	UO_MODE_FREERUN 	= 1,
	UO_MODE_TRAIN 		= 2,
	UO_MODE_EXTBNC 		= 3,
	UO_MODE_COMPUTER 	= 4,
}UO_MODE_E;

/*	UserOperation.fParamType	*/
typedef enum
{
	UO_PARAM_NONE 		= 0xff,			//[FIX]
	UO_PARAM_AMPL		= 0,			//[F]
	UO_PARAM_PULSE		= 1,			//[F]
	UO_PARAM_FREQ		= 2,			//[F]
	UO_PARAM_DURATION	= 3,			//[F]
}UO_PARAM_E;

/*	UserOperation.fUnitCur		*/		//		0	1
#define UO_UNIT_BIT_AMPL_V		0		//[F]	mV	@V
#define UO_UNIT_BIT_AMPL_C		4		//[F]	uA	@mA
#define UO_UNIT_BIT_PULSE		1		//[F]	us	@ms
#define UO_UNIT_BIT_FREQ		2		//[F]	@Hz	KHz
#define UO_UNIT_BIT_DURATION	3		//[F]	ms	@s

#define UO_UNIT_SMALL			0		//С��λ
#define UO_UNIT_BIG				1		//��λ

/*	����update	������Ч״̬��0--��Ч����Ҫ����	1--�û�����������	2--��Ч������ֵ
		�ж���
			=0	���²�����-->��ֵ2
			=1	������
			=2	������
		�ж���
			=0	������
			=1	����ENTER��PULSE AMPL FREQ DURATION��-->��ֵ0
				����RUN SINGGLETRIGGER������SINGLE FREERUN TRAIN EXTBNC�л�ģʽ��-->��ֵ0
			=2	����PULSE AMPL FREQ DURATION���ò�����-->��ֵ1
				����RUN SINGGLETRIGGER������SINGLE FREERUN TRAIN EXTBNC�л�ģʽ��-->��ֵ0
*/
typedef enum
{
	UO_UPDATE_INVALID = 0,				//�ж�����������в���
	UO_UPDATE_SETTING,					//�ж��в��������û�������ɺ󽫱�־λ����
	UO_UPDATE_VALID,					//�ж��и������
}UO_UPDATE_E;

typedef struct
{
	UO_PHASE_E 		bPhase;				//[S]����	/	˫��
	uint8_t  		bVC;				//[S]VOL	/	CUR
	
	UO_MODE_E  		fMode;				//[S]0xff--none		0--SINGLE			1--FREERUN		2--TRAIN		3--EXTTRI			4--COMPUTER
	UO_PARAM_E  	fParamType;			//[F]0xff--none		0--Ampl				1--Pulse		2--Freq			3--Duration			�������õĲ���
	UO_PARAM_E  	fParamTypePre;		//��һ���ò������ͣ�����ˢ����һ���ò�����ʾ״̬
	uint8_t  		fUnitCur;			//[F][S]bit0--AMPL_V:mV[0]/V[1]	bit4--AMPL_A:uA[0]/mA[1]	bit1--PULSE:usec[0]/msec[1]		bit2--FREQ:Hz[0]/KHz[1]		bit3--DURATION:msec[0]/sec[1]
	
	/*	��ѹ����²���	*/
	UO_MODE_E		fMode_VPre;			//��ѹ�����ģʽ����
	sModeParam_t	V_ModeSingle;		//[S]
	sModeParam_t	V_ModeFreeRun;		//[S]
	sModeParam_t	V_ModeTrain;		//[S]
	sModeParam_t	V_ModeExtBnc;		//[S]
	
	/*	��������²���	*/
	UO_MODE_E		fMode_CPre;
	sModeParam_t	C_ModeSingle;		//[S]
	sModeParam_t	C_ModeFreeRun;		//[S]
	sModeParam_t	C_ModeTrain;		//[S]
	sModeParam_t	C_ModeExtBnc;		//[S]
	
	/*	��ǰ������ȡ�����	*/
	sGetSetupData_t	GetSetupData;		//�������ò���ֵ
	sModify_t 		Modify;				//ͨ���������ֻ��ͷ�Բ��������޸�
	UO_UPDATE_E		Update;				//��ǰ���������Ч��
}sUserOperation_t;
extern sUserOperation_t UserOperation;

void SettingInfo_Modify(uint8_t btn_value);
void Manual_Poll(void);

#endif

/*********************************************************************************
										���в���Լ��
-------------------------------------------------------------------------------------------------
	MODE		DISPLAY			PARAMETER	����					�ײ�ͳһ��λ(С�����6λ�����ֻ��ʾ3λ)
	Single		�٢�		|	��Amplitude	����      [mV/V/uA/mA]	| 0.001mV/uA-0.000001V/mA-----1uV/nA
	Free Run	�٢ڢ�		|	��Pulse		�ߵ�ƽʱ��[usec/msec]	| 0.001usec--0.000001msec-----1ns
	Train		�٢ڢۢ�	|	��Frequency	Ƶ��	  [Hz/KHz]		| 0.001Hz----0.000001KHz -----1mHz
	EXT Trigger	�ⲿBNC��	|	��Duration	���ʱ��  [msec/sec]	| 0.001msec--0.000001sec -----1us
	ע���ײ�ͳһ��λ����Ϊ�������á�������㡢������ʾ����ʣ��������˴˶���������ʹ�ã����軻�㴦��
	��������Ampl		1V�� 	��	SETVALUE = 1uV  *	1000000	= 1000000
						1mA��	��	SETVALUE = 1nA  *	1000000	= 1000000
		����Pulse		1ms��	��	SETVALUE = 1ns  *	1000000	= 1000000
		����Freq		1Hz, 	��	SETVALUE = 1mHz *	1000	= 1000
		����Duration	1ms, 	��	SETVALUE = 1us  *	1000	= 1000
**************************************************************************************************************************************/
