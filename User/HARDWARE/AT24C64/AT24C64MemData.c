
#include  "main.h"

#define 		MEMDATA_LEN    200
static uint8_t  Tbl_MemData[MEMDATA_LEN] = {0};

/********************************************************************
*	功能	：	写入数组【4字节参数】
******************************************************************************/
static void Array_Write(uint32_t addrbase, uint64_t value, uint8_t nbytes)
{
	if(nbytes == 8)																		//PULSE参数可能会出现大于4字节情况
	{
		Tbl_MemData[addrbase]		= value / 256 / 256 / 256 / 256 / 256 / 256 / 256;
		Tbl_MemData[addrbase + 1]	= value / 256 / 256 / 256 / 256 / 256 / 256 % 256;
		Tbl_MemData[addrbase + 2]	= value / 256 / 256 / 256 / 256 / 256 % 256;
		Tbl_MemData[addrbase + 3]	= value / 256 / 256 / 256 / 256 % 256;
		Tbl_MemData[addrbase + 4]	= value / 256 / 256 / 256 % 256;
		Tbl_MemData[addrbase + 5]	= value / 256 / 256 % 256;
		Tbl_MemData[addrbase + 6]	= value / 256 % 256;
		Tbl_MemData[addrbase + 7]	= value % 256;
	}
	else if(nbytes == 4)
	{
		Tbl_MemData[addrbase]		= value / 256 / 256 / 256;
		Tbl_MemData[addrbase + 1]	= value / 256 / 256 % 256;
		Tbl_MemData[addrbase + 2]	= value / 256 % 256;
		Tbl_MemData[addrbase + 3]	= value % 256;
	}
	else if(nbytes == 2)
	{
		Tbl_MemData[addrbase]		= value / 256 % 256;
		Tbl_MemData[addrbase + 1]	= value % 256;
	}
	else if(nbytes == 1)
	{
		Tbl_MemData[addrbase]		= value % 256;
	}
}

/********************************************************************
*	功能	：	读取数组【4字节参数】
******************************************************************************/
static uint64_t Array_Read(uint32_t addrbase, uint8_t nbytes)
{
	uint64_t value = 0;	
	if(nbytes == 8)
	{
		value = (uint64_t)Tbl_MemData[addrbase] * 256 * 256 * 256 * 256 * 256 * 256 * 256 
				+ (uint64_t)Tbl_MemData[addrbase + 1] * 256 * 256 * 256 * 256 * 256 * 256 
				+ (uint64_t)Tbl_MemData[addrbase + 2] * 256 * 256 * 256 * 256 * 256 
				+ (uint64_t)Tbl_MemData[addrbase + 3] * 256 * 256 * 256 * 256
				+ (uint64_t)Tbl_MemData[addrbase + 4] * 256 * 256 * 256 
				+ (uint64_t)Tbl_MemData[addrbase + 5] * 256 * 256 
				+ (uint64_t)Tbl_MemData[addrbase + 6] * 256 
				+ (uint64_t)Tbl_MemData[addrbase + 7];
	}
	else if(nbytes == 4)
	{
		value = Tbl_MemData[addrbase] * 256 * 256 * 256 + Tbl_MemData[addrbase + 1] * 256 * 256 + Tbl_MemData[addrbase + 2] * 256 + Tbl_MemData[addrbase + 3];
	}
	else if(nbytes == 2)
	{
		value = Tbl_MemData[addrbase] * 256 + Tbl_MemData[addrbase + 1];
	}
	else if(nbytes == 1)
	{
		value = Tbl_MemData[addrbase];
	}
	
	return(value);
}

/********************************************************************
*	功能	：	超限处理
******************************************************************************/
void Memory_OverLimit_Process(uint64_t *value, uint64_t ceiling)
{
	if(*value > ceiling)
	{
		*value = 0;
	}
}

/********************************************************************
*	功能	：	加载配置
******************************************************************************/
void Memory_ConfigLoad(void)
{
	AT24CXX_Read(0, Tbl_MemData, MEMDATA_LEN);
	
	if(Tbl_MemData[ADDR_CHECKDATA] != VALUE_CHECKDATA)									//读取失败，加载默认值且存储
	{
		Switch.ModePre = 0;
		Switch.ModeCur = (SELECT_VC_V << MODE_BIT_VC) | (SELECT_FLIP_UN << MODE_BIT_FLIP);
		Switch_Ctrl(UO_PHASE_UNIPHASE);
		
		bsp_PutKey(BTN_SINGLE * 3 + 1);
		
		UserOperation.V_ModeSingle.fUnit	|= (1 << UO_UNIT_BIT_PULSE) | (1 << UO_UNIT_BIT_AMPL_V) | (1 << UO_UNIT_BIT_DURATION);
		UserOperation.V_ModeFreeRun.fUnit	|= (1 << UO_UNIT_BIT_PULSE) | (1 << UO_UNIT_BIT_AMPL_V) | (1 << UO_UNIT_BIT_DURATION);
		UserOperation.V_ModeTrain.fUnit		|= (1 << UO_UNIT_BIT_PULSE) | (1 << UO_UNIT_BIT_AMPL_V) | (1 << UO_UNIT_BIT_DURATION);
		UserOperation.V_ModeExtBnc.fUnit	|= (1 << UO_UNIT_BIT_PULSE) | (1 << UO_UNIT_BIT_AMPL_V) | (1 << UO_UNIT_BIT_DURATION);
		UserOperation.C_ModeSingle.fUnit	|= (1 << UO_UNIT_BIT_PULSE) | (1 << UO_UNIT_BIT_AMPL_C) | (1 << UO_UNIT_BIT_DURATION);
		UserOperation.C_ModeFreeRun.fUnit	|= (1 << UO_UNIT_BIT_PULSE) | (1 << UO_UNIT_BIT_AMPL_C) | (1 << UO_UNIT_BIT_DURATION);
		UserOperation.C_ModeTrain.fUnit		|= (1 << UO_UNIT_BIT_PULSE) | (1 << UO_UNIT_BIT_AMPL_C) | (1 << UO_UNIT_BIT_DURATION);
		UserOperation.C_ModeExtBnc.fUnit	|= (1 << UO_UNIT_BIT_PULSE) | (1 << UO_UNIT_BIT_AMPL_C) | (1 << UO_UNIT_BIT_DURATION);
		
		UserOperation.bPhase				= UO_PHASE_UNIPHASE;
		UserOperation.bVC					= (Switch.ModeCur >> MODE_BIT_VC) & 0x01;
		UserOperation.fMode_VPre			= UO_MODE_SINGLE;
		UserOperation.fMode_CPre			= UO_MODE_SINGLE;
		UserOperation.fMode					= UO_MODE_NONE;								//初始化为无模式后与当前模式比较处理，避免初次上电投入按键值无法正常刷新情况，因轮询执行，先后无关
		UserOperation.fUnitCur				= UserOperation.V_ModeSingle.fUnit;
		
		
		Array_Write(ADDR_CHECKDATA,					VALUE_CHECKDATA,										1);
		Array_Write(ADDR_BFLIP,						UserOperation.bPhase,									1);
		Array_Write(ADDR_BVC,						UserOperation.bVC,										1);
		Array_Write(ADDR_MODE_V,					UserOperation.fMode_VPre,								1);
		Array_Write(ADDR_MODE_C,					UserOperation.fMode_CPre,								1);
		
		Array_Write(ADDR_V_SINGLE_FUNIT_START,		UserOperation.V_ModeSingle.fUnit,						1);
		Array_Write(ADDR_V_SINGLE_PULSENAD_START,	UserOperation.V_ModeSingle.ParamNAD[UO_PARAM_PULSE],	1);
		Array_Write(ADDR_V_SINGLE_AMPLNAD_START,	UserOperation.V_ModeSingle.ParamNAD[UO_PARAM_AMPL],		1);
		Array_Write(ADDR_V_SINGLE_PULSE_START,		UserOperation.V_ModeSingle.Param[UO_PARAM_PULSE],		8);
		Array_Write(ADDR_V_SINGLE_AMPL_START,		UserOperation.V_ModeSingle.Param[UO_PARAM_AMPL],		8);
		Array_Write(ADDR_V_FREERUN_FUNIT_START,		UserOperation.V_ModeFreeRun.fUnit,						1);
		Array_Write(ADDR_V_FREERUN_PULSENAD_START,	UserOperation.V_ModeFreeRun.ParamNAD[UO_PARAM_PULSE],	1);
		Array_Write(ADDR_V_FREERUN_AMPLNAD_START,	UserOperation.V_ModeFreeRun.ParamNAD[UO_PARAM_AMPL],	1);
		Array_Write(ADDR_V_FREERUN_FREQNAD_START,	UserOperation.V_ModeFreeRun.ParamNAD[UO_PARAM_FREQ],	1);
		Array_Write(ADDR_V_FREERUN_PULSE_START,		UserOperation.V_ModeFreeRun.Param[UO_PARAM_PULSE],		8);
		Array_Write(ADDR_V_FREERUN_AMPL_START,		UserOperation.V_ModeFreeRun.Param[UO_PARAM_AMPL],		8);
		Array_Write(ADDR_V_FREERUN_FREQ_START,		UserOperation.V_ModeFreeRun.Param[UO_PARAM_FREQ],		8);
		Array_Write(ADDR_V_TRAIN_FUNIT_START,		UserOperation.V_ModeTrain.fUnit,						1);
		Array_Write(ADDR_V_TRAIN_PULSENAD_START,	UserOperation.V_ModeTrain.ParamNAD[UO_PARAM_PULSE],		1);
		Array_Write(ADDR_V_TRAIN_AMPLNAD_START,		UserOperation.V_ModeTrain.ParamNAD[UO_PARAM_AMPL],		1);
		Array_Write(ADDR_V_TRAIN_FREQNAD_START,		UserOperation.V_ModeTrain.ParamNAD[UO_PARAM_FREQ],		1);
		Array_Write(ADDR_V_TRAIN_DURATIONNAD_START,	UserOperation.V_ModeTrain.ParamNAD[UO_PARAM_DURATION],	1);
		Array_Write(ADDR_V_TRAIN_PULSE_START,		UserOperation.V_ModeTrain.Param[UO_PARAM_PULSE],		8);
		Array_Write(ADDR_V_TRAIN_AMPL_START,		UserOperation.V_ModeTrain.Param[UO_PARAM_AMPL],			8);
		Array_Write(ADDR_V_TRAIN_FREQ_START,		UserOperation.V_ModeTrain.Param[UO_PARAM_FREQ],			8);
		Array_Write(ADDR_V_TRAIN_DURATION_START,	UserOperation.V_ModeTrain.Param[UO_PARAM_DURATION],		8);
		Array_Write(ADDR_V_EXTBNC_FUNIT_START,		UserOperation.V_ModeExtBnc.fUnit,						1);
		Array_Write(ADDR_V_EXTBNC_AMPLNAD_START,	UserOperation.V_ModeExtBnc.ParamNAD[UO_PARAM_AMPL],		1);
		Array_Write(ADDR_V_EXTBNC_AMPL_START,		UserOperation.V_ModeExtBnc.Param[UO_PARAM_AMPL],		8);
		
		Array_Write(ADDR_C_SINGLE_FUNIT_START,		UserOperation.C_ModeSingle.fUnit,						1);
		Array_Write(ADDR_C_SINGLE_PULSENAD_START,	UserOperation.C_ModeSingle.ParamNAD[UO_PARAM_PULSE],	1);
		Array_Write(ADDR_C_SINGLE_AMPLNAD_START,	UserOperation.C_ModeSingle.ParamNAD[UO_PARAM_AMPL],		1);
		Array_Write(ADDR_C_SINGLE_PULSE_START,		UserOperation.C_ModeSingle.Param[UO_PARAM_PULSE],		8);
		Array_Write(ADDR_C_SINGLE_AMPL_START,		UserOperation.C_ModeSingle.Param[UO_PARAM_AMPL],		8);
		Array_Write(ADDR_C_FREERUN_FUNIT_START,		UserOperation.C_ModeFreeRun.fUnit,						1);
		Array_Write(ADDR_C_FREERUN_PULSENAD_START,	UserOperation.C_ModeFreeRun.ParamNAD[UO_PARAM_PULSE],	1);
		Array_Write(ADDR_C_FREERUN_AMPLNAD_START,	UserOperation.C_ModeFreeRun.ParamNAD[UO_PARAM_AMPL],	1);
		Array_Write(ADDR_C_FREERUN_FREQNAD_START,	UserOperation.C_ModeFreeRun.ParamNAD[UO_PARAM_FREQ],	1);
		Array_Write(ADDR_C_FREERUN_PULSE_START,		UserOperation.C_ModeFreeRun.Param[UO_PARAM_PULSE],		8);
		Array_Write(ADDR_C_FREERUN_AMPL_START,		UserOperation.C_ModeFreeRun.Param[UO_PARAM_AMPL],		8);
		Array_Write(ADDR_C_FREERUN_FREQ_START,		UserOperation.C_ModeFreeRun.Param[UO_PARAM_FREQ],		8);
		Array_Write(ADDR_C_TRAIN_FUNIT_START,		UserOperation.C_ModeTrain.fUnit,						1);
		Array_Write(ADDR_C_TRAIN_PULSENAD_START,	UserOperation.C_ModeTrain.ParamNAD[UO_PARAM_PULSE],		1);
		Array_Write(ADDR_C_TRAIN_AMPLNAD_START,		UserOperation.C_ModeTrain.ParamNAD[UO_PARAM_AMPL],		1);
		Array_Write(ADDR_C_TRAIN_FREQNAD_START,		UserOperation.C_ModeTrain.ParamNAD[UO_PARAM_FREQ],		1);
		Array_Write(ADDR_C_TRAIN_DURATIONNAD_START,	UserOperation.C_ModeTrain.ParamNAD[UO_PARAM_DURATION],	1);
		Array_Write(ADDR_C_TRAIN_PULSE_START,		UserOperation.C_ModeTrain.Param[UO_PARAM_PULSE],		8);
		Array_Write(ADDR_C_TRAIN_AMPL_START,		UserOperation.C_ModeTrain.Param[UO_PARAM_AMPL],			8);
		Array_Write(ADDR_C_TRAIN_FREQ_START,		UserOperation.C_ModeTrain.Param[UO_PARAM_FREQ],			8);
		Array_Write(ADDR_C_TRAIN_DURATION_START,	UserOperation.C_ModeTrain.Param[UO_PARAM_DURATION],		8);
		Array_Write(ADDR_C_EXTBNC_FUNIT_START,		UserOperation.C_ModeExtBnc.fUnit,						1);
		Array_Write(ADDR_C_EXTBNC_AMPLNAD_START,	UserOperation.C_ModeExtBnc.ParamNAD[UO_PARAM_AMPL],		1);
		Array_Write(ADDR_C_EXTBNC_AMPL_START,		UserOperation.C_ModeExtBnc.Param[UO_PARAM_AMPL],		8);
		
		AT24CXX_Write(0, Tbl_MemData, MEMDATA_LEN);
	}
	else																					//EEPROM 读取成功
	{
		UserOperation.bPhase					= (UO_PHASE_E)Array_Read(ADDR_BFLIP,					1);
		UserOperation.bVC						= Array_Read(ADDR_BVC,						1);
		UserOperation.fMode_VPre				= (UO_MODE_E)Array_Read(ADDR_MODE_V,		1);
		UserOperation.fMode_CPre				= (UO_MODE_E)Array_Read(ADDR_MODE_C,		1);
		
		UserOperation.V_ModeSingle.fUnit		= Array_Read(ADDR_V_SINGLE_FUNIT_START,		1);
		UserOperation.V_ModeSingle.ParamNAD[UO_PARAM_PULSE] 	= Array_Read(ADDR_V_SINGLE_PULSENAD_START,	1);
		UserOperation.V_ModeSingle.ParamNAD[UO_PARAM_AMPL] 		= Array_Read(ADDR_V_SINGLE_AMPLNAD_START,	1);
		UserOperation.V_ModeSingle.Param[UO_PARAM_PULSE]		= Array_Read(ADDR_V_SINGLE_PULSE_START,		8);
		UserOperation.V_ModeSingle.Param[UO_PARAM_AMPL]			= Array_Read(ADDR_V_SINGLE_AMPL_START,		8);
		UserOperation.V_ModeFreeRun.fUnit						= Array_Read(ADDR_V_FREERUN_FUNIT_START,	1);
		UserOperation.V_ModeFreeRun.ParamNAD[UO_PARAM_PULSE]	= Array_Read(ADDR_V_FREERUN_PULSENAD_START,	1);
		UserOperation.V_ModeFreeRun.ParamNAD[UO_PARAM_AMPL] 	= Array_Read(ADDR_V_FREERUN_AMPLNAD_START,	1);
		UserOperation.V_ModeFreeRun.ParamNAD[UO_PARAM_FREQ]		= Array_Read(ADDR_V_FREERUN_FREQNAD_START,	1);
		UserOperation.V_ModeFreeRun.Param[UO_PARAM_PULSE]		= Array_Read(ADDR_V_FREERUN_PULSE_START,	8);
		UserOperation.V_ModeFreeRun.Param[UO_PARAM_AMPL]		= Array_Read(ADDR_V_FREERUN_AMPL_START,		8);
		UserOperation.V_ModeFreeRun.Param[UO_PARAM_FREQ]		= Array_Read(ADDR_V_FREERUN_FREQ_START,		8);
		UserOperation.V_ModeTrain.fUnit							= Array_Read(ADDR_V_TRAIN_FUNIT_START,		1);
		UserOperation.V_ModeTrain.ParamNAD[UO_PARAM_PULSE] 		= Array_Read(ADDR_V_TRAIN_PULSENAD_START,	1);
		UserOperation.V_ModeTrain.ParamNAD[UO_PARAM_AMPL] 		= Array_Read(ADDR_V_TRAIN_AMPLNAD_START,	1);
		UserOperation.V_ModeTrain.ParamNAD[UO_PARAM_FREQ] 		= Array_Read(ADDR_V_TRAIN_FREQNAD_START,	1);
		UserOperation.V_ModeTrain.ParamNAD[UO_PARAM_DURATION]	= Array_Read(ADDR_V_TRAIN_DURATIONNAD_START,1);
		UserOperation.V_ModeTrain.Param[UO_PARAM_PULSE]			= Array_Read(ADDR_V_TRAIN_PULSE_START,		8);
		UserOperation.V_ModeTrain.Param[UO_PARAM_AMPL]			= Array_Read(ADDR_V_TRAIN_AMPL_START,		8);
		UserOperation.V_ModeTrain.Param[UO_PARAM_FREQ]			= Array_Read(ADDR_V_TRAIN_FREQ_START,		8);
		UserOperation.V_ModeTrain.Param[UO_PARAM_DURATION]		= Array_Read(ADDR_V_TRAIN_DURATION_START,	8);
		UserOperation.V_ModeExtBnc.fUnit						= Array_Read(ADDR_V_EXTBNC_FUNIT_START,		1);
		UserOperation.V_ModeExtBnc.ParamNAD[UO_PARAM_AMPL] 		= Array_Read(ADDR_V_EXTBNC_AMPLNAD_START,	1);
		UserOperation.V_ModeExtBnc.Param[UO_PARAM_AMPL]			= Array_Read(ADDR_V_EXTBNC_AMPL_START,		8);
		
		UserOperation.C_ModeSingle.fUnit						= Array_Read(ADDR_C_SINGLE_FUNIT_START,		1);
		UserOperation.C_ModeSingle.ParamNAD[UO_PARAM_PULSE] 	= Array_Read(ADDR_C_SINGLE_PULSENAD_START,	1);
		UserOperation.C_ModeSingle.ParamNAD[UO_PARAM_AMPL] 		= Array_Read(ADDR_C_SINGLE_AMPLNAD_START,	1);
		UserOperation.C_ModeSingle.Param[UO_PARAM_PULSE]		= Array_Read(ADDR_C_SINGLE_PULSE_START,		8);
		UserOperation.C_ModeSingle.Param[UO_PARAM_AMPL]			= Array_Read(ADDR_C_SINGLE_AMPL_START,		8);
		UserOperation.C_ModeFreeRun.fUnit						= Array_Read(ADDR_C_FREERUN_FUNIT_START,	1);
		UserOperation.C_ModeFreeRun.ParamNAD[UO_PARAM_PULSE] 	= Array_Read(ADDR_C_FREERUN_PULSENAD_START,	1);
		UserOperation.C_ModeFreeRun.ParamNAD[UO_PARAM_AMPL] 	= Array_Read(ADDR_C_FREERUN_AMPLNAD_START,	1);
		UserOperation.C_ModeFreeRun.ParamNAD[UO_PARAM_FREQ] 	= Array_Read(ADDR_C_FREERUN_FREQNAD_START,	1);
		UserOperation.C_ModeFreeRun.Param[UO_PARAM_PULSE]		= Array_Read(ADDR_C_FREERUN_PULSE_START,	8);
		UserOperation.C_ModeFreeRun.Param[UO_PARAM_AMPL]		= Array_Read(ADDR_C_FREERUN_AMPL_START,		8);
		UserOperation.C_ModeFreeRun.Param[UO_PARAM_FREQ]		= Array_Read(ADDR_C_FREERUN_FREQ_START,		8);
		UserOperation.C_ModeTrain.fUnit							= Array_Read(ADDR_C_TRAIN_FUNIT_START,		1);
		UserOperation.C_ModeTrain.ParamNAD[UO_PARAM_PULSE] 		= Array_Read(ADDR_C_TRAIN_PULSENAD_START,	1);
		UserOperation.C_ModeTrain.ParamNAD[UO_PARAM_AMPL] 		= Array_Read(ADDR_C_TRAIN_AMPLNAD_START,	1);
		UserOperation.C_ModeTrain.ParamNAD[UO_PARAM_FREQ] 		= Array_Read(ADDR_C_TRAIN_FREQNAD_START,	1);
		UserOperation.C_ModeTrain.ParamNAD[UO_PARAM_DURATION]	= Array_Read(ADDR_C_TRAIN_DURATIONNAD_START,1);
		UserOperation.C_ModeTrain.Param[UO_PARAM_PULSE]			= Array_Read(ADDR_C_TRAIN_PULSE_START,		8);
		UserOperation.C_ModeTrain.Param[UO_PARAM_AMPL]			= Array_Read(ADDR_C_TRAIN_AMPL_START,		8);
		UserOperation.C_ModeTrain.Param[UO_PARAM_FREQ]			= Array_Read(ADDR_C_TRAIN_FREQ_START,		8);
		UserOperation.C_ModeTrain.Param[UO_PARAM_DURATION]		= Array_Read(ADDR_C_TRAIN_DURATION_START,	8);
		UserOperation.C_ModeExtBnc.fUnit						= Array_Read(ADDR_C_EXTBNC_FUNIT_START,		1);
		UserOperation.C_ModeExtBnc.ParamNAD[UO_PARAM_AMPL] 		= Array_Read(ADDR_C_EXTBNC_AMPLNAD_START,	1);
		UserOperation.C_ModeExtBnc.Param[UO_PARAM_AMPL]			= Array_Read(ADDR_C_EXTBNC_AMPL_START,		8);
		
		#if 1																			//避免读取AT24C64异常导致显示错乱
			Memory_OverLimit_Process(&UserOperation.V_ModeSingle.Param[UO_PARAM_PULSE],		CEILING_PULSE);
			Memory_OverLimit_Process(&UserOperation.V_ModeSingle.Param[UO_PARAM_AMPL],		CEILING_V_AMPLITUDE);			
			Memory_OverLimit_Process(&UserOperation.V_ModeFreeRun.Param[UO_PARAM_PULSE],	CEILING_PULSE);
			Memory_OverLimit_Process(&UserOperation.V_ModeFreeRun.Param[UO_PARAM_AMPL],		CEILING_V_AMPLITUDE);
			Memory_OverLimit_Process(&UserOperation.V_ModeFreeRun.Param[UO_PARAM_FREQ],		CEILING_FREQUENCY);			
			Memory_OverLimit_Process(&UserOperation.V_ModeTrain.Param[UO_PARAM_PULSE],		CEILING_PULSE);
			Memory_OverLimit_Process(&UserOperation.V_ModeTrain.Param[UO_PARAM_AMPL],		CEILING_V_AMPLITUDE);
			Memory_OverLimit_Process(&UserOperation.V_ModeTrain.Param[UO_PARAM_FREQ],		CEILING_FREQUENCY);
			Memory_OverLimit_Process(&UserOperation.V_ModeTrain.Param[UO_PARAM_DURATION],	CEILING_DURATION);			
			Memory_OverLimit_Process(&UserOperation.V_ModeExtBnc.Param[UO_PARAM_AMPL],		CEILING_V_AMPLITUDE);
			
			Memory_OverLimit_Process(&UserOperation.C_ModeSingle.Param[UO_PARAM_PULSE],		CEILING_PULSE);
			Memory_OverLimit_Process(&UserOperation.C_ModeSingle.Param[UO_PARAM_AMPL],		CEILING_C_AMPLITUDE);
			Memory_OverLimit_Process(&UserOperation.C_ModeFreeRun.Param[UO_PARAM_PULSE],	CEILING_PULSE);
			Memory_OverLimit_Process(&UserOperation.C_ModeFreeRun.Param[UO_PARAM_AMPL],		CEILING_C_AMPLITUDE);
			Memory_OverLimit_Process(&UserOperation.C_ModeFreeRun.Param[UO_PARAM_FREQ],		CEILING_FREQUENCY);
			Memory_OverLimit_Process(&UserOperation.C_ModeTrain.Param[UO_PARAM_PULSE],		CEILING_PULSE);
			Memory_OverLimit_Process(&UserOperation.C_ModeTrain.Param[UO_PARAM_AMPL],		CEILING_C_AMPLITUDE);
			Memory_OverLimit_Process(&UserOperation.C_ModeTrain.Param[UO_PARAM_FREQ],		CEILING_FREQUENCY);
			Memory_OverLimit_Process(&UserOperation.C_ModeTrain.Param[UO_PARAM_DURATION],	CEILING_DURATION);
			Memory_OverLimit_Process(&UserOperation.C_ModeExtBnc.Param[UO_PARAM_AMPL],		CEILING_C_AMPLITUDE);
		#endif
			
		Switch.ModePre = 0;																	//读取后考虑对开关等的处理
		Switch.ModeCur = UserOperation.bVC << MODE_BIT_VC;   //MODE_BIT_VC		0
		Switch_Ctrl(UO_PHASE_UNIPHASE);

		UserOperation.fMode = UO_MODE_NONE;
		
		switch(UserOperation.bVC)															//对参数标志位进行读取
		{
			case SELECT_VC_V:
				switch(UserOperation.fMode_VPre)
				{
					case UO_MODE_SINGLE:
						UserOperation.fUnitCur = UserOperation.V_ModeSingle.fUnit;
						bsp_PutKey(BTN_SINGLE * 3 + 1);
						break;
					
					case UO_MODE_FREERUN:
						UserOperation.fUnitCur = UserOperation.V_ModeFreeRun.fUnit;
						bsp_PutKey(BTN_FREERUN * 3 + 1);
						break;
					
					case UO_MODE_TRAIN:						
						UserOperation.fUnitCur = UserOperation.V_ModeTrain.fUnit;
						bsp_PutKey(BTN_TRAIN * 3 + 1);
						break;
					
					case UO_MODE_EXTBNC:						
						UserOperation.fUnitCur = UserOperation.V_ModeExtBnc.fUnit;
						bsp_PutKey(BTN_EXTBNC * 3 + 1);
						break;
					
					default:
						break;
				}
				break;
			
			case SELECT_VC_C:
				switch(UserOperation.fMode_CPre)
				{
					case UO_MODE_SINGLE:
						UserOperation.fUnitCur = UserOperation.C_ModeSingle.fUnit;	
						bsp_PutKey(BTN_SINGLE * 3 + 1);
						break;
					
					case UO_MODE_FREERUN:
						UserOperation.fUnitCur = UserOperation.C_ModeFreeRun.fUnit;
						bsp_PutKey(BTN_FREERUN * 3 + 1);
						break;
					
					case UO_MODE_TRAIN:
						UserOperation.fUnitCur = UserOperation.C_ModeTrain.fUnit;
						bsp_PutKey(BTN_TRAIN * 3 + 1);
						break;
					
					case UO_MODE_EXTBNC:
						UserOperation.fUnitCur = UserOperation.C_ModeExtBnc.fUnit;
						bsp_PutKey(BTN_EXTBNC * 3 + 1);
						break;
					
					default:
						break;
				}
				break;
		}
	}
	
	
	AT24CXX_Read(230, (void *)&sAdditionalData, sizeof(sAdditionalData));
	
	if(sAdditionalData.V_Wave_type>3)sAdditionalData.V_Wave_type=0;
	if(sAdditionalData.C_Wave_type>3)sAdditionalData.C_Wave_type=0;
	
	if(UserOperation.bVC == SELECT_VC_V)
	{
		Wave_type = sAdditionalData.V_Wave_type;
		UserOperation.V_ModeExtBnc.Param[UO_PARAM_PULSE] = sAdditionalData.V_Bnc_Pulse;
	}
	else
	{
		Wave_type = sAdditionalData.C_Wave_type;
		UserOperation.C_ModeExtBnc.Param[UO_PARAM_PULSE] = sAdditionalData.C_Bnc_Pulse;
	}
	
	
	#if(TEST_AT24CXX)
		AT24CXX_Read(0, Tbl_MemData, MEMDATA_LEN);
		printf("Tbl_MemData[0] = %d\r\n", Tbl_MemData[0]);
		printf("Tbl_MemData[1] = %d\r\n", Tbl_MemData[1]);
		printf("Tbl_MemData[2] = %d\r\n", Tbl_MemData[2]);
	#endif
}

/********************************************************************
*	功能	：	轮询保存用户数据
******************************************************************************/
void Memory_Poll(void)
{
	if(T6.MemoryUpdateCnt >= T6_VALUE_MINUTE)												//如有数据更新则60s后刷新存储
	{
		Array_Write(ADDR_CHECKDATA,					VALUE_CHECKDATA,										1);
		Array_Write(ADDR_BFLIP,						UserOperation.bPhase,									1);
		Array_Write(ADDR_BVC,						UserOperation.bVC,										1);
		Array_Write(ADDR_MODE_V,					UserOperation.fMode_VPre,								1);
		Array_Write(ADDR_MODE_C,					UserOperation.fMode_CPre,								1);
		
		Array_Write(ADDR_V_SINGLE_FUNIT_START,		UserOperation.V_ModeSingle.fUnit,						1);
		Array_Write(ADDR_V_SINGLE_PULSENAD_START,	UserOperation.V_ModeSingle.ParamNAD[UO_PARAM_PULSE],	1);
		Array_Write(ADDR_V_SINGLE_AMPLNAD_START,	UserOperation.V_ModeSingle.ParamNAD[UO_PARAM_AMPL],		1);
		Array_Write(ADDR_V_SINGLE_PULSE_START,		UserOperation.V_ModeSingle.Param[UO_PARAM_PULSE],		8);
		Array_Write(ADDR_V_SINGLE_AMPL_START,		UserOperation.V_ModeSingle.Param[UO_PARAM_AMPL],		8);
		Array_Write(ADDR_V_FREERUN_FUNIT_START,		UserOperation.V_ModeFreeRun.fUnit,						1);
		Array_Write(ADDR_V_FREERUN_PULSENAD_START,	UserOperation.V_ModeFreeRun.ParamNAD[UO_PARAM_PULSE],	1);
		Array_Write(ADDR_V_FREERUN_AMPLNAD_START,	UserOperation.V_ModeFreeRun.ParamNAD[UO_PARAM_AMPL],	1);
		Array_Write(ADDR_V_FREERUN_FREQNAD_START,	UserOperation.V_ModeFreeRun.ParamNAD[UO_PARAM_FREQ],	1);
		Array_Write(ADDR_V_FREERUN_PULSE_START,		UserOperation.V_ModeFreeRun.Param[UO_PARAM_PULSE],		8);
		Array_Write(ADDR_V_FREERUN_AMPL_START,		UserOperation.V_ModeFreeRun.Param[UO_PARAM_AMPL],		8);
		Array_Write(ADDR_V_FREERUN_FREQ_START,		UserOperation.V_ModeFreeRun.Param[UO_PARAM_FREQ],		8);
		Array_Write(ADDR_V_TRAIN_FUNIT_START,		UserOperation.V_ModeTrain.fUnit,						1);
		Array_Write(ADDR_V_TRAIN_PULSENAD_START,	UserOperation.V_ModeTrain.ParamNAD[UO_PARAM_PULSE],		1);
		Array_Write(ADDR_V_TRAIN_AMPLNAD_START,		UserOperation.V_ModeTrain.ParamNAD[UO_PARAM_AMPL],		1);
		Array_Write(ADDR_V_TRAIN_FREQNAD_START,		UserOperation.V_ModeTrain.ParamNAD[UO_PARAM_FREQ],		1);
		Array_Write(ADDR_V_TRAIN_DURATIONNAD_START,	UserOperation.V_ModeTrain.ParamNAD[UO_PARAM_DURATION],	1);
		Array_Write(ADDR_V_TRAIN_PULSE_START,		UserOperation.V_ModeTrain.Param[UO_PARAM_PULSE],		8);
		Array_Write(ADDR_V_TRAIN_AMPL_START,		UserOperation.V_ModeTrain.Param[UO_PARAM_AMPL],			8);
		Array_Write(ADDR_V_TRAIN_FREQ_START,		UserOperation.V_ModeTrain.Param[UO_PARAM_FREQ],			8);
		Array_Write(ADDR_V_TRAIN_DURATION_START,	UserOperation.V_ModeTrain.Param[UO_PARAM_DURATION],		8);
		Array_Write(ADDR_V_EXTBNC_FUNIT_START,		UserOperation.V_ModeExtBnc.fUnit,						1);
		Array_Write(ADDR_V_EXTBNC_AMPLNAD_START,	UserOperation.V_ModeExtBnc.ParamNAD[UO_PARAM_AMPL],		1);
		Array_Write(ADDR_V_EXTBNC_AMPL_START,		UserOperation.V_ModeExtBnc.Param[UO_PARAM_AMPL],		8);
		
		Array_Write(ADDR_C_SINGLE_FUNIT_START,		UserOperation.C_ModeSingle.fUnit,						1);
		Array_Write(ADDR_C_SINGLE_PULSENAD_START,	UserOperation.C_ModeSingle.ParamNAD[UO_PARAM_PULSE],	1);
		Array_Write(ADDR_C_SINGLE_AMPLNAD_START,	UserOperation.C_ModeSingle.ParamNAD[UO_PARAM_AMPL],		1);
		Array_Write(ADDR_C_SINGLE_PULSE_START,		UserOperation.C_ModeSingle.Param[UO_PARAM_PULSE],		8);
		Array_Write(ADDR_C_SINGLE_AMPL_START,		UserOperation.C_ModeSingle.Param[UO_PARAM_AMPL],		8);
		Array_Write(ADDR_C_FREERUN_FUNIT_START,		UserOperation.C_ModeFreeRun.fUnit,						1);
		Array_Write(ADDR_C_FREERUN_PULSENAD_START,	UserOperation.C_ModeFreeRun.ParamNAD[UO_PARAM_PULSE],	1);
		Array_Write(ADDR_C_FREERUN_AMPLNAD_START,	UserOperation.C_ModeFreeRun.ParamNAD[UO_PARAM_AMPL],	1);
		Array_Write(ADDR_C_FREERUN_FREQNAD_START,	UserOperation.C_ModeFreeRun.ParamNAD[UO_PARAM_FREQ],	1);
		Array_Write(ADDR_C_FREERUN_PULSE_START,		UserOperation.C_ModeFreeRun.Param[UO_PARAM_PULSE],		8);
		Array_Write(ADDR_C_FREERUN_AMPL_START,		UserOperation.C_ModeFreeRun.Param[UO_PARAM_AMPL],		8);
		Array_Write(ADDR_C_FREERUN_FREQ_START,		UserOperation.C_ModeFreeRun.Param[UO_PARAM_FREQ],		8);
		Array_Write(ADDR_C_TRAIN_FUNIT_START,		UserOperation.C_ModeTrain.fUnit,						1);
		Array_Write(ADDR_C_TRAIN_PULSENAD_START,	UserOperation.C_ModeTrain.ParamNAD[UO_PARAM_PULSE],		1);
		Array_Write(ADDR_C_TRAIN_AMPLNAD_START,		UserOperation.C_ModeTrain.ParamNAD[UO_PARAM_AMPL],		1);
		Array_Write(ADDR_C_TRAIN_FREQNAD_START,		UserOperation.C_ModeTrain.ParamNAD[UO_PARAM_FREQ],		1);
		Array_Write(ADDR_C_TRAIN_DURATIONNAD_START,	UserOperation.C_ModeTrain.ParamNAD[UO_PARAM_DURATION],	1);
		Array_Write(ADDR_C_TRAIN_PULSE_START,		UserOperation.C_ModeTrain.Param[UO_PARAM_PULSE],		8);
		Array_Write(ADDR_C_TRAIN_AMPL_START,		UserOperation.C_ModeTrain.Param[UO_PARAM_AMPL],			8);
		Array_Write(ADDR_C_TRAIN_FREQ_START,		UserOperation.C_ModeTrain.Param[UO_PARAM_FREQ],			8);
		Array_Write(ADDR_C_TRAIN_DURATION_START,	UserOperation.C_ModeTrain.Param[UO_PARAM_DURATION],		8);
		Array_Write(ADDR_C_EXTBNC_FUNIT_START,		UserOperation.C_ModeExtBnc.fUnit,						1);
		Array_Write(ADDR_C_EXTBNC_AMPLNAD_START,	UserOperation.C_ModeExtBnc.ParamNAD[UO_PARAM_AMPL],		1);
		Array_Write(ADDR_C_EXTBNC_AMPL_START,		UserOperation.C_ModeExtBnc.Param[UO_PARAM_AMPL],		8);
		
		AT24CXX_Write(0, Tbl_MemData, MEMDATA_LEN);
		
		T6.MemoryUpdateCnt = MEMORYUPDATE_UPCNT_CLEAR;
	}
}
