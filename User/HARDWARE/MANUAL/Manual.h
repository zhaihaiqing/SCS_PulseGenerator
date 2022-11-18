
/********************************************************************
*
*	文件	：	Manual.h

*	管脚	：					

*	功能	：	按键响应（设置参数、控制输出等）
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

#define VALUE_EXPAND_105	1000000		//计算过程中各参数大单位默认小数点后6位有效，故每个参数都扩大1000000倍成整数用于显示处理
#define VALUE_EXPAND_102	1000		//计算过程中各参数小单位默认小数点后3位有效，故每个参数都扩大1000倍成整数用于显示处理

/*	设置上限，用户设置值超限强制设置为上限值	*/
#define CEILING_V_AMPLITUDE	(uint64_t)150      *1000000		//uV,±150V
#define CEILING_C_AMPLITUDE	(uint64_t)32       *1000000		//nA,32mA	//[V193]三XTR111最大电流96mA
#define CEILING_PULSE		(uint64_t)5*60*1000*1000000		//ns,5min
#define CEILING_FREQUENCY	(uint64_t)30       *1000000		//mHz,30KHz
#define CEILING_DURATION	(uint64_t)7*24*60  *1000000		//us,24h

extern uint64_t Ceiling[4];

typedef struct
{
	/*	参数单位信息	*/
	uint8_t  fUnit;							//SINGLE  FREERUN  TRAIN  EXTBNC
	/*	NAD--Num After Dot，用户设置小数点信息，用于旋钮控制	*/	
	uint8_t  ParamNAD[4];					//UO_PARAM_E
//	uint8_t  ParamNAD[UO_PARAM_AMPL];		//SINGLE  FREERUN  TRAIN  EXTBNC
//	uint8_t  ParamNAD[UO_PARAM_PULSE];		//SINGLE  FREERUN  TRAIN
//	uint8_t  ParamNAD[UO_PARAM_FREQ];		//        FREERUN  TRAIN
//	uint8_t  ParamNAD[UO_PARAM_DURATION];	//                 TRAIN
	/*	参数值	*/	
	uint64_t Param[4];
//	uint64_t Ampl;							//SINGLE  FREERUN  TRAIN  EXTBNC
//	uint64_t Pulse;							//SINGLE  FREERUN  TRAIN
//	uint64_t Freq;							//        FREERUN  TRAIN
//	uint64_t Duration;						//                 TRAIN
}sModeParam_t;

typedef struct
{										//获取参数信息，管理直接影响设置变量，减少同种判断处理
	uint8_t  		bUnit;				//正在设置参数单位，0--小单位	1--大单位（大单位为小单位1000倍）
	sModeParam_t	*pModeParam;		//正在设置对应模式参数首地址
	uint8_t			*pParamNAD;			//正在设置参数小数点位数
	uint64_t		ParamValue;			//正在设置参数值
	uint64_t		Ceiling;			//正在设置参数上限值，超限强制设置为上限
}sGetSetupData_t;

typedef struct
{										//修改参数值
	uint8_t fStart;						//0--设置完成  1--设置起始状态  2--设置进行中。参数标志位，若为1时，用户按5，则此时设置参数值为5，置此标志位为2，参数左移一位开始正常输入
	uint8_t bUnitSwitched;				//单位切换标志位，用于解决参数设置过程中切换单位导致处理混乱问题.单位切换后翻转状态，为1时若输入数字先清零再输入，参数切换后恢复置0
	uint8_t NumAfterDot;				//0--none	   1~6--即将设置的小数点后位数
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

#define UO_UNIT_SMALL			0		//小单位
#define UO_UNIT_BIG				1		//大单位

/*	关于update	参数有效状态，0--无效，需要更新	1--用户正在设置中	2--有效，最新值
		中断中
			=0	更新参数并-->赋值2
			=1	不处理
			=2	不处理
		中断外
			=0	不处理
			=1	按下ENTER或PULSE AMPL FREQ DURATION则-->赋值0
				按下RUN SINGGLETRIGGER启动或SINGLE FREERUN TRAIN EXTBNC切换模式则-->赋值0
			=2	按下PULSE AMPL FREQ DURATION设置参数则-->赋值1
				按下RUN SINGGLETRIGGER启动或SINGLE FREERUN TRAIN EXTBNC切换模式则-->赋值0
*/
typedef enum
{
	UO_UPDATE_INVALID = 0,				//中断中需更新运行参数
	UO_UPDATE_SETTING,					//中断中不处理，待用户设置完成后将标志位清零
	UO_UPDATE_VALID,					//中断中更新完成
}UO_UPDATE_E;

typedef struct
{
	UO_PHASE_E 		bPhase;				//[S]单相	/	双相
	uint8_t  		bVC;				//[S]VOL	/	CUR
	
	UO_MODE_E  		fMode;				//[S]0xff--none		0--SINGLE			1--FREERUN		2--TRAIN		3--EXTTRI			4--COMPUTER
	UO_PARAM_E  	fParamType;			//[F]0xff--none		0--Ampl				1--Pulse		2--Freq			3--Duration			正在设置的参数
	UO_PARAM_E  	fParamTypePre;		//上一设置参数类型，用于刷新上一设置参数显示状态
	uint8_t  		fUnitCur;			//[F][S]bit0--AMPL_V:mV[0]/V[1]	bit4--AMPL_A:uA[0]/mA[1]	bit1--PULSE:usec[0]/msec[1]		bit2--FREQ:Hz[0]/KHz[1]		bit3--DURATION:msec[0]/sec[1]
	
	/*	电压输出下参数	*/
	UO_MODE_E		fMode_VPre;			//电压输出下模式记忆
	sModeParam_t	V_ModeSingle;		//[S]
	sModeParam_t	V_ModeFreeRun;		//[S]
	sModeParam_t	V_ModeTrain;		//[S]
	sModeParam_t	V_ModeExtBnc;		//[S]
	
	/*	电流输出下参数	*/
	UO_MODE_E		fMode_CPre;
	sModeParam_t	C_ModeSingle;		//[S]
	sModeParam_t	C_ModeFreeRun;		//[S]
	sModeParam_t	C_ModeTrain;		//[S]
	sModeParam_t	C_ModeExtBnc;		//[S]
	
	/*	当前参数获取与调整	*/
	sGetSetupData_t	GetSetupData;		//正在设置参数值
	sModify_t 		Modify;				//通过输入数字或箭头对参数进行修改
	UO_UPDATE_E		Update;				//当前输出单数有效性
}sUserOperation_t;
extern sUserOperation_t UserOperation;

void SettingInfo_Modify(uint8_t btn_value);
void Manual_Poll(void);

#endif

/*********************************************************************************
										运行参数约定
-------------------------------------------------------------------------------------------------
	MODE		DISPLAY			PARAMETER	含义					底层统一单位(小数点后6位，面板只显示3位)
	Single		①②		|	①Amplitude	幅度      [mV/V/uA/mA]	| 0.001mV/uA-0.000001V/mA-----1uV/nA
	Free Run	①②③		|	②Pulse		高电平时长[usec/msec]	| 0.001usec--0.000001msec-----1ns
	Train		①②③④	|	③Frequency	频率	  [Hz/KHz]		| 0.001Hz----0.000001KHz -----1mHz
	EXT Trigger	外部BNC①	|	④Duration	输出时长  [msec/sec]	| 0.001msec--0.000001sec -----1us
	注：底层统一单位，作为参数设置、输出计算、参数显示三项本质，如此三项彼此独立，据需使用，无需换算处理。
	例：设置Ampl		1V， 	则	SETVALUE = 1uV  *	1000000	= 1000000
						1mA，	则	SETVALUE = 1nA  *	1000000	= 1000000
		设置Pulse		1ms，	则	SETVALUE = 1ns  *	1000000	= 1000000
		设置Freq		1Hz, 	则	SETVALUE = 1mHz *	1000	= 1000
		设置Duration	1ms, 	则	SETVALUE = 1us  *	1000	= 1000
**************************************************************************************************************************************/
