
#include  "main.h"

sColor_t Color = {COLOR_DESKTOP, COLOR_DESKTOP};
/********************************************************************
*	����	��	������Ļ��ʾ����
******************************************************************************/
#if(TEST_LCD)
	void test_LCD(void)
	{
		static u8 x=0;
		u8 lcd_id[12];									//���LCD ID�ַ���
		sprintf((char*)lcd_id,"LCD ID:%04X",lcddev.id);	//��LCD ID��ӡ��lcd_id����
		
//		printf(" LCD ID:%x\r\n",lcddev.id);		 		//��ӡLCD ID
		
//		while(1)
		{
			switch(x)
			{
				case 0:
					LCD_Clear(WHITE);
					break;
				case 1:
					LCD_Clear(BLACK);
					break;
				case 2:
					LCD_Clear(BLUE);
					break;
				case 3:
					LCD_Clear(RED);
					break;
				case 4:
					LCD_Clear(MAGENTA);
					break;
				case 5:
					LCD_Clear(GREEN);
					break;
				case 6:
					LCD_Clear(CYAN);
					break; 
				case 7:
					LCD_Clear(YELLOW);
					break;
				case 8:
					LCD_Clear(BRRED);
					break;
				case 9:
					LCD_Clear(GRAY);
					break;
				case 10:
					LCD_Clear(LGRAY);
					break;
				case 11:
					LCD_Clear(BROWN);
					break;
			}
			if(lcddev.dir == 0)					/*�������ԣ�������࿪ʼ��ʾ*/
			{
				LCD_ShowString(200,0,210,24,24,"COLUME TEST");
				LCD_ShowString(202,40,210,24,24,"STIMULATOR ^_^"); 
				LCD_ShowString(204,70,200,16,16,"TFTLCD TEST");
				LCD_ShowString(206,90,200,16,16,"LZG@MAPPINGLAB");
				LCD_ShowString(208,110,200,16,16,lcd_id);		//��ʾLCD ID
				LCD_ShowString(208,130,200,12,12,"2018/7/19");	  
				LCD_ShowString(210,150,200,12,12,"2018/7/19");	
				LCD_ShowString(250,180,200,12,12,"2018/7/19");
				LCD_ShowString(210+260,468,200,12,12,"2018/7/19");
			}   					
			else if(lcddev.dir == 1)			/*�������ԣ�������࿪ʼ��ʾ*/
			{
				LCD_ShowString(0,0,210,24,24,"HORIZON TEST");
				LCD_ShowString(0,40,210,24,24,"STIMULATOR ^_^");
				LCD_ShowString(0,70,200,16,16,"TFTLCD TEST");
				LCD_ShowString(0,90,200,16,16,"LZG@MAPPINGLAB");
				LCD_ShowString(0,110,200,16,16,lcd_id);		//��ʾLCD ID
				LCD_ShowString(0,130,200,12,12,"2018/7/19");
			}
     			
			x++;
			if(x==12)x=0;
			delay_ms(1000);	
			delay_ms(1000);	
			WDG_Feed();
		}
	}
	
	void test_LCDSimpleLayout(void)
	{
		LCD_Clear(WHITE);
		
		LCD_ShowString(000, 100, 119, 24, 24, "SINGLE");
		LCD_ShowString(120, 108, 239, 16, 16, "TRAIN");
		LCD_ShowString(240, 108, 359, 16, 16, "FREE-RUN");
		LCD_ShowString(360, 108, 480, 16, 16, "EXT-TRI");
		
		LCD_ShowString(000, 138, 119, 16, 16, "SINGLE");
		LCD_ShowString(120, 130, 239, 24, 24, "TRAIN");
		LCD_ShowString(240, 138, 359, 16, 16, "FREE-RUN");
		LCD_ShowString(360, 138, 480, 16, 16, "EXT-TRI");
		
		LCD_ShowString(000, 168, 119, 16, 16, "SINGLE");
		LCD_ShowString(120, 168, 239, 16, 16, "TRAIN");
		LCD_ShowString(240, 160, 359, 24, 24, "FREE-RUN");
		LCD_ShowString(360, 168, 480, 16, 16, "EXT-TRI");
		
		LCD_ShowString(000, 208, 119, 16, 16, "SINGLE");
		LCD_ShowString(120, 208, 239, 16, 16, "TRAIN");
		LCD_ShowString(240, 208, 359, 16, 16, "FREE-RUN");
		LCD_ShowString(360, 200, 480, 24, 24, "EXT-TRI");
		
		LCD_ShowString(000, 230, 480, 16, 16, "SINGLE    TRAIN    FREE-RUN    EXT-TRI");
		LCD_ShowString(000, 250, 480, 12, 12, "SINGLE    TRAIN    FREE-RUN    EXT-TRI");
	}
#endif

/********************************************************************
*	����	��	���԰�����ֵ����
******************************************************************************/
#if(TEST_KEYBOARD)
	void test_KEYBOARD(void)
	{
		uint8_t keycode = 0;
		uint8_t i = 0, j = 0;
		uint8_t display[60] = {0};				//����sprintf����
	
		keycode = bsp_GetKey();
		if(keycode != KEY_NONE)
		{
			i = (keycode - 1) / 3;				//��ֵ
			j = (keycode - 1) % 3;
			
//			LCD_Clear(WHITE);
			LCD_Fill(000, 50, 480, 80, WHITE);	//����ˢ�£���С��Ļ��������
			sprintf((char*)display,"KEY_Code:%04X  KEY_i:%04X  KEY_j:%04X", keycode, i, j);
			LCD_ShowString(000, 50, 480, 24, 24, display);
		}
	}
#endif

sUserInterface_t UI = {0};
#define TWINKLE_DIMTIME		200				//500
#define TWINKLE_CYCLETIME	1000			//1000

#define TWINKLE_DISABLE		0
#define TWINKLE_ENABLE		1

/********************************************************************
*	����	��	ʵ��sprintf���ܣ�ͬʱ������ֵ��ʱ����������
*	����	��	unitsize--��λ��������λ��С��λ1000����ϵ
				p--�ַ��洢����ָ��
				value--����ֵ
				decimalnum--С���㼰�����ʾλ��
				alignlength--����λ��
******************************************************************************/
static void sprintf_value(uint8_t unitsize, char* p, uint64_t value, uint8_t decimalnum, uint8_t alignlength)
{
	uint8_t i = 0, j = 0;
	uint64_t temp = 0;
	
	if(unitsize)
	{
		temp = value / 1000000;									//��ʾֵ��������
		
		switch(decimalnum)
		{
			case 0:												//��С��
				
				while(temp)
				{
					temp /= 10;
					i++;
				}
				if(i == 0)
				{
					i = 1;
				}
				for(j = 0; j < alignlength - 0 - i; j++)
				{
					p[j] = 0x20;
				}
				
				temp = value / 1000000;
				
				p[j + i] = '\0';
				
				while(i)
				{
					--i;
					p[j + i] = temp % 10 + 0x30;
					temp /= 10;
				}
				
				break;
			
			case 1:												//С����
				
				while(temp)
				{
					temp /= 10;
					i++;
				}
				if(i == 0)
				{
					i = 1;
				}
				for(j = 0; j < alignlength - 1 - i; j++)
				{
					p[j] = 0x20;
				}
				
				temp = value / 1000000;				
				
				p[j + i] = '.';
				p[j + i + 1] = '\0';
				
				while(i)
				{
					--i;
					p[j + i] = temp % 10 + 0x30;
					temp /= 10;
				}
				
				break;
			
			case 2:												//һλС��
				
				while(temp)
				{
					temp /= 10;
					i++;
				}
				if(i == 0)
				{
					i = 1;
				}
				for(j = 0; j < alignlength - 2 - i; j++)
				{
					p[j] = 0x20;
				}
				
				temp = value / 1000000;				
				
				p[j + i] = '.';
				p[j + i + 1] = value / 100000 % 10 + 0x30;
				p[j + i + 2] = '\0';
				
				while(i)
				{
					--i;
					p[j + i] = temp % 10 + 0x30;
					temp /= 10;
				}
			
				break;
			
			case 3:												//��λС��
				
				while(temp)
				{
					temp /= 10;
					i++;
				}
				if(i == 0)
				{
					i = 1;
				}
				for(j = 0; j < alignlength - 3 - i; j++)
				{
					p[j] = 0x20;
				}
				
				temp = value / 1000000;				
				
				p[j + i] = '.';
				p[j + i + 1] = value / 100000 % 10 + 0x30;
				p[j + i + 2] = value / 10000 % 10 + 0x30;
				p[j + i + 3] = '\0';
				
				while(i)
				{
					--i;
					p[j + i] = temp % 10 + 0x30;
					temp /= 10;
				}
			
				break;
			
			case 4:												//��λС��
				
				while(temp)										//��ȡ��������λ��
				{
					temp /= 10;
					i++;
				}
				if(i == 0)										//[V180]�޸��������ֽ�Ϊ0ʱ����ʾbug
				{
					i = 1;
				}
				for(j = 0; j < alignlength - 4 - i; j++)		//�Ҷ��룬�����ո�����С���㼰�����λ����ռ��4λ
				{
					p[j] = 0x20;								//���ո�
				}
				
				temp = value / 1000000;							//�ٴλ�ȡ��ʾֵ��������
				
				p[j + i] = '.';									//���.��
				p[j + i + 1] = value / 100000 % 10 + 0x30;		//С�����һλ
				p[j + i + 2] = value / 10000 % 10 + 0x30;
				p[j + i + 3] = value / 1000 % 10 + 0x30;
				p[j + i + 4] = '\0';							//��������
				
				while(i)										//���������������ֽ��и�ֵ
				{
					--i;
					p[j + i] = temp % 10 + 0x30;
					temp /= 10;
				}
				
				break;
			
			default:
				break;
		}
	}
	else
	{
		temp = value / 1000;
		
		switch(decimalnum)
		{
			case 0:												//��С��
				
				while(temp)
				{
					temp /= 10;
					i++;
				}
				if(i == 0)
				{
					i = 1;
				}
				for(j = 0; j < alignlength - i; j++)
				{
					p[j] = 0x20;
				}
				
				temp = value / 1000;
				
				p[j + i] = '\0';
				
				while(i)
				{
					--i;
					p[j + i] = temp % 10 + 0x30;
					temp /= 10;
				}
				
				break;
			
			default:
				break;
		}
	}
}

/********************************************************************
*	����	��	���������ò���������˸����
*	����	��	unitsize--��λ��������λ��С��λ1000����ϵ
				p--�ַ��洢����ָ��
				value--����ֵ
				IsTwinkleEnable--�Ƿ�ʹ����˸�����û����ù�������˸��ʾ
******************************************************************************/
static void ValueDisplay_Control(uint8_t unitsize, char* p, uint64_t value, uint8_t IsTwinkleEnable)
{	
	if(unitsize)
	{		
		if(IsTwinkleEnable)
		{
			if(T6.RunCnt % TWINKLE_CYCLETIME < TWINKLE_DIMTIME)					//��˸
			{
				sprintf(p, 	"");
			}
			else
			{
				
				
				sprintf_value(unitsize, p, value, UserOperation.Modify.NumAfterDot, 10);
				
				if(UserOperation.Modify.fStart == 1)
				{
					sprintf_value(unitsize, p, value, 4, 10);
				}
			}			
		}
		else
		{
			sprintf_value(unitsize, p, value, 4, 10);
		}
	}
	else
	{
		if(IsTwinkleEnable && (T6.RunCnt % TWINKLE_CYCLETIME < TWINKLE_DIMTIME))
		{
			sprintf(p, 	"");
		}
		else
		{
			sprintf_value(unitsize, p, value, 0, 10);
		}
	}
}

/********************************************************************
*	����	��	��ʾ������ѯ,��ֵ
******************************************************************************/
static void UI_ContentScan(void)
{
	if(DOState.Status[DO_TIM4] != DOSTATE_STATUS_COMPLETE)
	{
		if(DOState.Status[DO_TIM4] == DOSTATE_STATUS_RUNNING)
		{
			sprintf((char*)UI.Str_PG1_Run_Cur, "Running");
		}
		else if(DOState.Status[DO_TIM4] == DOSTATE_STATUS_PAUSE)
		{
			sprintf((char*)UI.Str_PG1_Run_Cur, "Pause");
		}
		
		if(UserOperation.fMode == UO_MODE_SINGLE || UserOperation.fMode == UO_MODE_EXTBNC)									//[V111]�������߼��������ΪSINGLE��EXTBNC��ʱ��RUNָʾ�Ʋ���
		{
			pLEDRUN = LED_SN74HC240_OFF;
		}
		else
		{
			pLEDRUN = LED_SN74HC240_ON;
		}
	}
	else if(DOState.Status[DO_TIM4] == DOSTATE_STATUS_COMPLETE)
	{
		sprintf((char*)UI.Str_PG1_Run_Cur, "Stop");
		pLEDRUN = LED_SN74HC240_OFF;
		pLEDPAUSE = LED_SN74HC240_OFF;
	}
	
	if(UserOperation.bPhase == UO_PHASE_UNIPHASE)																				//[V145]������ж���ֵ�����޸�ԭ�߼��淴bug
	{
		//sprintf((char*)UI.Str_PG1_Flip_Cur, "UNIPHASE");
		//sprintf((char*)UI.Str_PG1_Flip_Cur, "_|-|_");
		pLEDFLIP = LED_SN74HC240_OFF;
	}
	else
	{
		//sprintf((char*)UI.Str_PG1_Flip_Cur, "BIOPHASE");
		//sprintf((char*)UI.Str_PG1_Flip_Cur, "-|_|-");
		pLEDFLIP = LED_SN74HC240_ON;
	}
	
	if(Wave_type == 0)
	{
		
		//sprintf((char*)UI.Str_PG1_Flip_Cur, "_|-|_");
		uint16_t i=0;
		//LCD_Fill(PG1_FLIP_START_X, PG1_FLIP_START_Y, PG1_FLIP_START_X+PG1_FLIP_AREA_WIDTH, PG1_FLIP_START_Y-PG1_FLIP_AREA_HEIGHT-1, COLOR_FLIP_AREA);			//������
		sprintf((char*)UI.Str_PG1_Flip_Cur, " ");//���������߼���������
		for(i=0;i<20;i++)
		{
			LCD_Fast_DrawPoint(PG1_FLIP_START_X+60+i ,PG1_FLIP_START_Y+21,COLOR_FLIP_FONT);
		}
		for(i=0;i<18;i++)
		{
			LCD_Fast_DrawPoint(PG1_FLIP_START_X+80 ,PG1_FLIP_START_Y+21-i,COLOR_FLIP_FONT);
		}
		for(i=0;i<40;i++)
		{
			LCD_Fast_DrawPoint(PG1_FLIP_START_X+80+i ,PG1_FLIP_START_Y+21-18,COLOR_FLIP_FONT);
		}
		for(i=0;i<18;i++)
		{
			LCD_Fast_DrawPoint(PG1_FLIP_START_X+120 ,PG1_FLIP_START_Y+21-i,COLOR_FLIP_FONT);
		}
		for(i=0;i<20;i++)
		{
			LCD_Fast_DrawPoint(PG1_FLIP_START_X+120+i ,PG1_FLIP_START_Y+21,COLOR_FLIP_FONT);
		}		
		//pLEDFLIP = LED_SN74HC240_OFF;
	}
	else if(Wave_type == 1)
	{
		//sprintf((char*)UI.Str_PG1_Flip_Cur, "-|_|-");
		uint16_t i=0;
		//LCD_Fill(PG1_FLIP_START_X, PG1_FLIP_START_Y, PG1_FLIP_START_X+PG1_FLIP_AREA_WIDTH, PG1_FLIP_START_Y-PG1_FLIP_AREA_HEIGHT-1, COLOR_FLIP_AREA);			//������
		sprintf((char*)UI.Str_PG1_Flip_Cur, "  ");//���������߼���������
		for(i=0;i<20;i++)
		{
			LCD_Fast_DrawPoint(PG1_FLIP_START_X+60+i ,PG1_FLIP_START_Y+21,COLOR_FLIP_FONT);
		}
		for(i=0;i<18;i++)
		{
			LCD_Fast_DrawPoint(PG1_FLIP_START_X+80 ,PG1_FLIP_START_Y+21+i,COLOR_FLIP_FONT);
		}
		for(i=0;i<40;i++)
		{
			LCD_Fast_DrawPoint(PG1_FLIP_START_X+80+i ,PG1_FLIP_START_Y+21+18,COLOR_FLIP_FONT);
		}
		for(i=0;i<18;i++)
		{
			LCD_Fast_DrawPoint(PG1_FLIP_START_X+120 ,PG1_FLIP_START_Y+21+i,COLOR_FLIP_FONT);
		}
		for(i=0;i<20;i++)
		{
			LCD_Fast_DrawPoint(PG1_FLIP_START_X+120+i ,PG1_FLIP_START_Y+21,COLOR_FLIP_FONT);
		}
		//pLEDFLIP = LED_SN74HC240_ON;
	}
	else if(Wave_type == 2)
	{
		//sprintf((char*)UI.Str_PG1_Flip_Cur, "_|-|_|-");
		uint16_t i=0;
		//LCD_Fill(PG1_FLIP_START_X, PG1_FLIP_START_Y, PG1_FLIP_START_X+PG1_FLIP_AREA_WIDTH, PG1_FLIP_START_Y-PG1_FLIP_AREA_HEIGHT-1, COLOR_FLIP_AREA);			//������
		sprintf((char*)UI.Str_PG1_Flip_Cur, "   ");//���������߼���������
		for(i=0;i<20;i++)
		{
			LCD_Fast_DrawPoint(PG1_FLIP_START_X+60+i ,PG1_FLIP_START_Y+21,COLOR_FLIP_FONT);			//_
		}
		for(i=0;i<18;i++)
		{
			LCD_Fast_DrawPoint(PG1_FLIP_START_X+80 ,PG1_FLIP_START_Y+21-i,COLOR_FLIP_FONT);			//|
		}
		for(i=0;i<20;i++)
		{
			LCD_Fast_DrawPoint(PG1_FLIP_START_X+80+i ,PG1_FLIP_START_Y+21-18,COLOR_FLIP_FONT);		//-
		}
		for(i=0;i<36;i++)
		{
			LCD_Fast_DrawPoint(PG1_FLIP_START_X+100 ,PG1_FLIP_START_Y+21-18+i,COLOR_FLIP_FONT);		//|
		}
		for(i=0;i<20;i++)
		{
			LCD_Fast_DrawPoint(PG1_FLIP_START_X+100+i ,PG1_FLIP_START_Y+21+18,COLOR_FLIP_FONT);		//-
		}
		for(i=0;i<18;i++)
		{
			LCD_Fast_DrawPoint(PG1_FLIP_START_X+120 ,PG1_FLIP_START_Y+21+i,COLOR_FLIP_FONT);		//|
		}
		for(i=0;i<20;i++)
		{
			LCD_Fast_DrawPoint(PG1_FLIP_START_X+120+i ,PG1_FLIP_START_Y+21,COLOR_FLIP_FONT);		//-
		}
		
		//pLEDFLIP = LED_SN74HC240_ON;
	}
	else
	{
		//sprintf((char*)UI.Str_PG1_Flip_Cur, "-|_|-|_");
		uint16_t i=0;
		//LCD_Fill(PG1_FLIP_START_X, PG1_FLIP_START_Y, PG1_FLIP_START_X+PG1_FLIP_AREA_WIDTH, PG1_FLIP_START_Y-PG1_FLIP_AREA_HEIGHT-1, COLOR_FLIP_AREA);			//������
		sprintf((char*)UI.Str_PG1_Flip_Cur, "    ");//���������߼���������
		for(i=0;i<20;i++)
		{
			LCD_Fast_DrawPoint(PG1_FLIP_START_X+60+i ,PG1_FLIP_START_Y+21,COLOR_FLIP_FONT);			//_
		}
		for(i=0;i<18;i++)
		{
			LCD_Fast_DrawPoint(PG1_FLIP_START_X+80 ,PG1_FLIP_START_Y+21+i,COLOR_FLIP_FONT);			//|
		}
		for(i=0;i<20;i++)
		{
			LCD_Fast_DrawPoint(PG1_FLIP_START_X+80+i ,PG1_FLIP_START_Y+21+18,COLOR_FLIP_FONT);		//-
		}
		for(i=0;i<36;i++)
		{
			LCD_Fast_DrawPoint(PG1_FLIP_START_X+100 ,PG1_FLIP_START_Y+21+18-i,COLOR_FLIP_FONT);		//|
		}
		for(i=0;i<20;i++)
		{
			LCD_Fast_DrawPoint(PG1_FLIP_START_X+100+i ,PG1_FLIP_START_Y+21-18,COLOR_FLIP_FONT);		//-
		}
		for(i=0;i<18;i++)
		{
			LCD_Fast_DrawPoint(PG1_FLIP_START_X+120 ,PG1_FLIP_START_Y+21-i,COLOR_FLIP_FONT);		//|
		}
		for(i=0;i<20;i++)
		{
			LCD_Fast_DrawPoint(PG1_FLIP_START_X+120+i ,PG1_FLIP_START_Y+21,COLOR_FLIP_FONT);		//-
		}
		//pLEDFLIP = LED_SN74HC240_ON;
	}
	
	if(UserOperation.bVC == SELECT_VC_V)
	{
		sprintf((char*)UI.Str_PG1_VC_Cur, "Voltage");
		
		switch(UserOperation.fMode)
		{
			case UO_MODE_SINGLE:
				
				sprintf((char*)UI.Str_PG1_Mode_Single_Cur, 	"Single");
				sprintf((char*)UI.Str_PG1_Mode_FreeRun_Cur, "FreeRun");
				sprintf((char*)UI.Str_PG1_Mode_Train_Cur, 	"Train");
				sprintf((char*)UI.Str_PG1_Mode_ExtBnc_Cur, 	"ExtBnc");
				
				sprintf((char*)UI.Str_PG1_Param_Pulse_Cur, 	"PulseWidth");
				sprintf((char*)UI.Str_PG1_Param_Amplitude_Cur, "Amplitude");
				sprintf((char*)UI.Str_PG1_Param_Frequency_Cur, "");
				sprintf((char*)UI.Str_PG1_Param_Duration_Cur, "");
				
				if(UserOperation.fUnitCur & (1 << UO_UNIT_BIT_PULSE))
				{
					if(UserOperation.fParamType == UO_PARAM_PULSE && UserOperation.Modify.fStart)																//�������ô˲���
					{
						ValueDisplay_Control(UO_UNIT_BIG, (char*)UI.Str_PG1_Value_Pulse_Cur, UserOperation.V_ModeSingle.Param[UO_PARAM_PULSE], TWINKLE_ENABLE);	//��˸����
					}
					else
					{
						ValueDisplay_Control(UO_UNIT_BIG, (char*)UI.Str_PG1_Value_Pulse_Cur, UserOperation.V_ModeSingle.Param[UO_PARAM_PULSE], TWINKLE_DISABLE);
					}
					sprintf((char*)UI.Str_PG1_Unit_Pulse_Cur, "ms");
				}
				else
				{
					if(UserOperation.fParamType == UO_PARAM_PULSE && UserOperation.Modify.fStart)																//�������ô˲���
					{
						ValueDisplay_Control(UO_UNIT_SMALL, (char*)UI.Str_PG1_Value_Pulse_Cur, UserOperation.V_ModeSingle.Param[UO_PARAM_PULSE], TWINKLE_ENABLE);	//��˸����
					}
					else
					{
						ValueDisplay_Control(UO_UNIT_SMALL, (char*)UI.Str_PG1_Value_Pulse_Cur, UserOperation.V_ModeSingle.Param[UO_PARAM_PULSE], TWINKLE_DISABLE);
					}
					sprintf((char*)UI.Str_PG1_Unit_Pulse_Cur, "~s");
				}
				
				if(UserOperation.fUnitCur & (1 << UO_UNIT_BIT_AMPL_V))
				{
					if(UserOperation.fParamType == UO_PARAM_AMPL && UserOperation.Modify.fStart)
					{
						ValueDisplay_Control(UO_UNIT_BIG, (char*)UI.Str_PG1_Value_Amplitude_Cur, UserOperation.V_ModeSingle.Param[UO_PARAM_AMPL], TWINKLE_ENABLE);
					}
					else
					{
						ValueDisplay_Control(UO_UNIT_BIG, (char*)UI.Str_PG1_Value_Amplitude_Cur, UserOperation.V_ModeSingle.Param[UO_PARAM_AMPL], TWINKLE_DISABLE);
					}
					sprintf((char*)UI.Str_PG1_Unit_Amplitude_Cur, "V");
				}
				else
				{
					if(UserOperation.fParamType == UO_PARAM_AMPL && UserOperation.Modify.fStart)
					{
						ValueDisplay_Control(UO_UNIT_SMALL, (char*)UI.Str_PG1_Value_Amplitude_Cur, UserOperation.V_ModeSingle.Param[UO_PARAM_AMPL], TWINKLE_ENABLE);
					}
					else
					{
						ValueDisplay_Control(UO_UNIT_SMALL, (char*)UI.Str_PG1_Value_Amplitude_Cur, UserOperation.V_ModeSingle.Param[UO_PARAM_AMPL], TWINKLE_DISABLE);
					}
					sprintf((char*)UI.Str_PG1_Unit_Amplitude_Cur, "mV");
				}
				
				sprintf((char*)UI.Str_PG1_Value_Frequency_Cur, "");
				sprintf((char*)UI.Str_PG1_Unit_Frequency_Cur, "");
				
				sprintf((char*)UI.Str_PG1_Value_Duration_Cur, "");
				sprintf((char*)UI.Str_PG1_Unit_Duration_Cur, "");
				
				break;
				
			case UO_MODE_FREERUN:
				
				sprintf((char*)UI.Str_PG1_Mode_Single_Cur, 	"Single");
				sprintf((char*)UI.Str_PG1_Mode_FreeRun_Cur, "FreeRun");
				sprintf((char*)UI.Str_PG1_Mode_Train_Cur, 	"Train");
				sprintf((char*)UI.Str_PG1_Mode_ExtBnc_Cur, 	"ExtBnc");
				
				sprintf((char*)UI.Str_PG1_Param_Pulse_Cur, 	"PulseWidth");
				sprintf((char*)UI.Str_PG1_Param_Amplitude_Cur, "Amplitude");
				sprintf((char*)UI.Str_PG1_Param_Frequency_Cur, "Frequency");
				sprintf((char*)UI.Str_PG1_Param_Duration_Cur, "");
				
				if(UserOperation.fUnitCur & (1 << UO_UNIT_BIT_PULSE))
				{
					if(UserOperation.fParamType == UO_PARAM_PULSE && UserOperation.Modify.fStart)
					{
						ValueDisplay_Control(UO_UNIT_BIG, (char*)UI.Str_PG1_Value_Pulse_Cur, UserOperation.V_ModeFreeRun.Param[UO_PARAM_PULSE], TWINKLE_ENABLE);
					}
					else
					{
						ValueDisplay_Control(UO_UNIT_BIG, (char*)UI.Str_PG1_Value_Pulse_Cur, UserOperation.V_ModeFreeRun.Param[UO_PARAM_PULSE], TWINKLE_DISABLE);
					}
					sprintf((char*)UI.Str_PG1_Unit_Pulse_Cur, "ms");
				}
				else
				{
					if(UserOperation.fParamType == UO_PARAM_PULSE && UserOperation.Modify.fStart)
					{
						ValueDisplay_Control(UO_UNIT_SMALL, (char*)UI.Str_PG1_Value_Pulse_Cur, UserOperation.V_ModeFreeRun.Param[UO_PARAM_PULSE], TWINKLE_ENABLE);
					}
					else
					{
						ValueDisplay_Control(UO_UNIT_SMALL, (char*)UI.Str_PG1_Value_Pulse_Cur, UserOperation.V_ModeFreeRun.Param[UO_PARAM_PULSE], TWINKLE_DISABLE);
					}					
					sprintf((char*)UI.Str_PG1_Unit_Pulse_Cur, "~s");
				}
				
				if(UserOperation.fUnitCur & (1 << UO_UNIT_BIT_AMPL_V))
				{
					if(UserOperation.fParamType == UO_PARAM_AMPL && UserOperation.Modify.fStart)
					{
						ValueDisplay_Control(UO_UNIT_BIG, (char*)UI.Str_PG1_Value_Amplitude_Cur, UserOperation.V_ModeFreeRun.Param[UO_PARAM_AMPL], TWINKLE_ENABLE);
					}
					else
					{
						ValueDisplay_Control(UO_UNIT_BIG, (char*)UI.Str_PG1_Value_Amplitude_Cur, UserOperation.V_ModeFreeRun.Param[UO_PARAM_AMPL], TWINKLE_DISABLE);
					}
					sprintf((char*)UI.Str_PG1_Unit_Amplitude_Cur, "V");
				}
				else
				{
					if(UserOperation.fParamType == UO_PARAM_AMPL && UserOperation.Modify.fStart)
					{
						ValueDisplay_Control(UO_UNIT_SMALL, (char*)UI.Str_PG1_Value_Amplitude_Cur, UserOperation.V_ModeFreeRun.Param[UO_PARAM_AMPL], TWINKLE_ENABLE);
					}
					else
					{
						ValueDisplay_Control(UO_UNIT_SMALL, (char*)UI.Str_PG1_Value_Amplitude_Cur, UserOperation.V_ModeFreeRun.Param[UO_PARAM_AMPL], TWINKLE_DISABLE);
					}
					sprintf((char*)UI.Str_PG1_Unit_Amplitude_Cur, "mV");
				}
				
				if(UserOperation.fUnitCur & (1 << UO_UNIT_BIT_FREQ))
				{
					if(UserOperation.fParamType == UO_PARAM_FREQ && UserOperation.Modify.fStart)
					{
						ValueDisplay_Control(UO_UNIT_BIG, (char*)UI.Str_PG1_Value_Frequency_Cur, UserOperation.V_ModeFreeRun.Param[UO_PARAM_FREQ], TWINKLE_ENABLE);
					}
					else
					{
						ValueDisplay_Control(UO_UNIT_BIG, (char*)UI.Str_PG1_Value_Frequency_Cur, UserOperation.V_ModeFreeRun.Param[UO_PARAM_FREQ], TWINKLE_DISABLE);
					}
					sprintf((char*)UI.Str_PG1_Unit_Frequency_Cur, "KHz");
				}
				else
				{
					if(UserOperation.fParamType == UO_PARAM_FREQ && UserOperation.Modify.fStart)
					{
						ValueDisplay_Control(UO_UNIT_BIG, (char*)UI.Str_PG1_Value_Frequency_Cur, UserOperation.V_ModeFreeRun.Param[UO_PARAM_FREQ], TWINKLE_ENABLE);
					}
					else
					{
						ValueDisplay_Control(UO_UNIT_BIG, (char*)UI.Str_PG1_Value_Frequency_Cur, UserOperation.V_ModeFreeRun.Param[UO_PARAM_FREQ], TWINKLE_DISABLE);
					}					
					sprintf((char*)UI.Str_PG1_Unit_Frequency_Cur, "Hz");
				}				

				sprintf((char*)UI.Str_PG1_Value_Duration_Cur, "");
				sprintf((char*)UI.Str_PG1_Unit_Duration_Cur, "");
			
				break;
			
			case UO_MODE_TRAIN:
				
				sprintf((char*)UI.Str_PG1_Mode_Single_Cur, 	"Single");
				sprintf((char*)UI.Str_PG1_Mode_FreeRun_Cur, "FreeRun");
				sprintf((char*)UI.Str_PG1_Mode_Train_Cur, 	"Train");
				sprintf((char*)UI.Str_PG1_Mode_ExtBnc_Cur, 	"ExtBnc");
				
				sprintf((char*)UI.Str_PG1_Param_Pulse_Cur, 	"PulseWidth");
				sprintf((char*)UI.Str_PG1_Param_Amplitude_Cur, "Amplitude");
				sprintf((char*)UI.Str_PG1_Param_Frequency_Cur, "Frequency");
				sprintf((char*)UI.Str_PG1_Param_Duration_Cur, "Duration");
			
				if(UserOperation.fUnitCur & (1 << UO_UNIT_BIT_PULSE))
				{
					if(UserOperation.fParamType == UO_PARAM_PULSE && UserOperation.Modify.fStart)
					{
						ValueDisplay_Control(UO_UNIT_BIG, (char*)UI.Str_PG1_Value_Pulse_Cur, UserOperation.V_ModeTrain.Param[UO_PARAM_PULSE], TWINKLE_ENABLE);
					}
					else
					{
						ValueDisplay_Control(UO_UNIT_BIG, (char*)UI.Str_PG1_Value_Pulse_Cur, UserOperation.V_ModeTrain.Param[UO_PARAM_PULSE], TWINKLE_DISABLE);
					}
					sprintf((char*)UI.Str_PG1_Unit_Pulse_Cur, "ms");
				}
				else
				{
					if(UserOperation.fParamType == UO_PARAM_PULSE && UserOperation.Modify.fStart)
					{
						ValueDisplay_Control(UO_UNIT_SMALL, (char*)UI.Str_PG1_Value_Pulse_Cur, UserOperation.V_ModeTrain.Param[UO_PARAM_PULSE], TWINKLE_ENABLE);
					}
					else
					{
						ValueDisplay_Control(UO_UNIT_SMALL, (char*)UI.Str_PG1_Value_Pulse_Cur, UserOperation.V_ModeTrain.Param[UO_PARAM_PULSE], TWINKLE_DISABLE);
					}
					sprintf((char*)UI.Str_PG1_Unit_Pulse_Cur, "~s");
				}
				
				if(UserOperation.fUnitCur & (1 << UO_UNIT_BIT_AMPL_V))
				{
					if(UserOperation.fParamType == UO_PARAM_AMPL && UserOperation.Modify.fStart)
					{
						ValueDisplay_Control(UO_UNIT_BIG, (char*)UI.Str_PG1_Value_Amplitude_Cur, UserOperation.V_ModeTrain.Param[UO_PARAM_AMPL], TWINKLE_ENABLE);
					}
					else
					{
						ValueDisplay_Control(UO_UNIT_BIG, (char*)UI.Str_PG1_Value_Amplitude_Cur, UserOperation.V_ModeTrain.Param[UO_PARAM_AMPL], TWINKLE_DISABLE);
					}
					sprintf((char*)UI.Str_PG1_Unit_Amplitude_Cur, "V");
				}
				else
				{
					if(UserOperation.fParamType == UO_PARAM_AMPL && UserOperation.Modify.fStart)
					{
						ValueDisplay_Control(UO_UNIT_SMALL, (char*)UI.Str_PG1_Value_Amplitude_Cur, UserOperation.V_ModeTrain.Param[UO_PARAM_AMPL], TWINKLE_ENABLE);
					}
					else
					{
						ValueDisplay_Control(UO_UNIT_SMALL, (char*)UI.Str_PG1_Value_Amplitude_Cur, UserOperation.V_ModeTrain.Param[UO_PARAM_AMPL], TWINKLE_DISABLE);
					}
					sprintf((char*)UI.Str_PG1_Unit_Amplitude_Cur, "mV");
				}
				
				if(UserOperation.fUnitCur & (1 << UO_UNIT_BIT_FREQ))
				{
					if(UserOperation.fParamType == UO_PARAM_FREQ && UserOperation.Modify.fStart)
					{
						ValueDisplay_Control(UO_UNIT_BIG, (char*)UI.Str_PG1_Value_Frequency_Cur, UserOperation.V_ModeTrain.Param[UO_PARAM_FREQ], TWINKLE_ENABLE);
					}
					else
					{
						ValueDisplay_Control(UO_UNIT_BIG, (char*)UI.Str_PG1_Value_Frequency_Cur, UserOperation.V_ModeTrain.Param[UO_PARAM_FREQ], TWINKLE_DISABLE);
					}	
					sprintf((char*)UI.Str_PG1_Unit_Frequency_Cur, "KHz");
				}
				else
				{
					if(UserOperation.fParamType == UO_PARAM_FREQ && UserOperation.Modify.fStart)
					{
						ValueDisplay_Control(UO_UNIT_BIG, (char*)UI.Str_PG1_Value_Frequency_Cur, UserOperation.V_ModeTrain.Param[UO_PARAM_FREQ], TWINKLE_ENABLE);
					}
					else
					{
						ValueDisplay_Control(UO_UNIT_BIG, (char*)UI.Str_PG1_Value_Frequency_Cur, UserOperation.V_ModeTrain.Param[UO_PARAM_FREQ], TWINKLE_DISABLE);
					}
					sprintf((char*)UI.Str_PG1_Unit_Frequency_Cur, "Hz");
				}
			
				if(UserOperation.fUnitCur & (1 << UO_UNIT_BIT_DURATION))
				{
					if(UserOperation.fParamType == UO_PARAM_DURATION && UserOperation.Modify.fStart)
					{
						ValueDisplay_Control(UO_UNIT_BIG, (char*)UI.Str_PG1_Value_Duration_Cur, UserOperation.V_ModeTrain.Param[UO_PARAM_DURATION], TWINKLE_ENABLE);
					}
					else
					{
						ValueDisplay_Control(UO_UNIT_BIG, (char*)UI.Str_PG1_Value_Duration_Cur, UserOperation.V_ModeTrain.Param[UO_PARAM_DURATION], TWINKLE_DISABLE);
					}
					sprintf((char*)UI.Str_PG1_Unit_Duration_Cur, "s");
				}
				else
				{
					if(UserOperation.fParamType == UO_PARAM_DURATION && UserOperation.Modify.fStart)
					{
						ValueDisplay_Control(UO_UNIT_SMALL, (char*)UI.Str_PG1_Value_Duration_Cur, UserOperation.V_ModeTrain.Param[UO_PARAM_DURATION], TWINKLE_ENABLE);
					}
					else
					{
						ValueDisplay_Control(UO_UNIT_SMALL, (char*)UI.Str_PG1_Value_Duration_Cur, UserOperation.V_ModeTrain.Param[UO_PARAM_DURATION], TWINKLE_DISABLE);
					}					
					sprintf((char*)UI.Str_PG1_Unit_Duration_Cur, "ms");
				}
			
				break;
			
			case UO_MODE_EXTBNC:
				
				sprintf((char*)UI.Str_PG1_Mode_Single_Cur, 	"Single");
				sprintf((char*)UI.Str_PG1_Mode_FreeRun_Cur,	"FreeRun");
				sprintf((char*)UI.Str_PG1_Mode_Train_Cur, 	"Train");
				sprintf((char*)UI.Str_PG1_Mode_ExtBnc_Cur, 	"ExtBnc");
				
				//sprintf((char*)UI.Str_PG1_Param_Pulse_Cur, 	"");
			
				sprintf((char*)UI.Str_PG1_Param_Pulse_Cur, 	"PulseWidth");
			
				sprintf((char*)UI.Str_PG1_Param_Amplitude_Cur, "Amplitude");
				sprintf((char*)UI.Str_PG1_Param_Frequency_Cur, "");
				sprintf((char*)UI.Str_PG1_Param_Duration_Cur, "");
				
				//sprintf((char*)UI.Str_PG1_Value_Pulse_Cur, 	"");
				//sprintf((char*)UI.Str_PG1_Unit_Pulse_Cur, 	"");
			
			
			
				if(UserOperation.fUnitCur & (1 << UO_UNIT_BIT_PULSE))
				{
					if(UserOperation.fParamType == UO_PARAM_PULSE && UserOperation.Modify.fStart)
					{
						ValueDisplay_Control(UO_UNIT_BIG, (char*)UI.Str_PG1_Value_Pulse_Cur, UserOperation.V_ModeExtBnc.Param[UO_PARAM_PULSE], TWINKLE_ENABLE);
					}
					else
					{
						ValueDisplay_Control(UO_UNIT_BIG, (char*)UI.Str_PG1_Value_Pulse_Cur, UserOperation.V_ModeExtBnc.Param[UO_PARAM_PULSE], TWINKLE_DISABLE);
					}
					sprintf((char*)UI.Str_PG1_Unit_Pulse_Cur, "ms");
				}
				else
				{
					if(UserOperation.fParamType == UO_PARAM_PULSE && UserOperation.Modify.fStart)
					{
						ValueDisplay_Control(UO_UNIT_SMALL, (char*)UI.Str_PG1_Value_Pulse_Cur, UserOperation.V_ModeExtBnc.Param[UO_PARAM_PULSE], TWINKLE_ENABLE);
					}
					else
					{
						ValueDisplay_Control(UO_UNIT_SMALL, (char*)UI.Str_PG1_Value_Pulse_Cur, UserOperation.V_ModeExtBnc.Param[UO_PARAM_PULSE], TWINKLE_DISABLE);
					}
					sprintf((char*)UI.Str_PG1_Unit_Pulse_Cur, "~s");
				}
				
				//log_info("Exbnc_Pluse:%lld\r\n",UserOperation.V_ModeExtBnc.Param[UO_PARAM_PULSE]);
				
				if( sAdditionalData.V_Bnc_Pulse != UserOperation.V_ModeExtBnc.Param[UO_PARAM_PULSE] )
				{
					sAdditionalData.V_Bnc_Pulse = UserOperation.V_ModeExtBnc.Param[UO_PARAM_PULSE];
					AT24CXX_Write(230, (void *)&sAdditionalData, sizeof(sAdditionalData));
				}
				
				
				if(UserOperation.fUnitCur & (1 << UO_UNIT_BIT_AMPL_V))
				{
					if(UserOperation.fParamType == UO_PARAM_AMPL && UserOperation.Modify.fStart)
					{
						ValueDisplay_Control(UO_UNIT_BIG, (char*)UI.Str_PG1_Value_Amplitude_Cur, UserOperation.V_ModeExtBnc.Param[UO_PARAM_AMPL], TWINKLE_ENABLE);
					}
					else
					{
						ValueDisplay_Control(UO_UNIT_BIG, (char*)UI.Str_PG1_Value_Amplitude_Cur, UserOperation.V_ModeExtBnc.Param[UO_PARAM_AMPL], TWINKLE_DISABLE);
					}
					sprintf((char*)UI.Str_PG1_Unit_Amplitude_Cur, "V");
				}
				else
				{
					if(UserOperation.fParamType == UO_PARAM_AMPL && UserOperation.Modify.fStart)
					{
						ValueDisplay_Control(UO_UNIT_SMALL, (char*)UI.Str_PG1_Value_Amplitude_Cur, UserOperation.V_ModeExtBnc.Param[UO_PARAM_AMPL], TWINKLE_ENABLE);
					}
					else
					{
						ValueDisplay_Control(UO_UNIT_SMALL, (char*)UI.Str_PG1_Value_Amplitude_Cur, UserOperation.V_ModeExtBnc.Param[UO_PARAM_AMPL], TWINKLE_DISABLE);
					}
					sprintf((char*)UI.Str_PG1_Unit_Amplitude_Cur, "mV");
				}
			
				sprintf((char*)UI.Str_PG1_Value_Frequency_Cur, "");
				sprintf((char*)UI.Str_PG1_Unit_Frequency_Cur, "");
				
				sprintf((char*)UI.Str_PG1_Value_Duration_Cur, "");
				sprintf((char*)UI.Str_PG1_Unit_Duration_Cur, "");
				
				break;
				
			default:
				break;
		}
	}
	else
	{
		sprintf((char*)UI.Str_PG1_VC_Cur, "Current");																									//���ѹ�������ƴ���

		switch(UserOperation.fMode)
		{
			case UO_MODE_SINGLE:
				
				sprintf((char*)UI.Str_PG1_Mode_Single_Cur, 	"Single");
				sprintf((char*)UI.Str_PG1_Mode_FreeRun_Cur, "FreeRun");
				sprintf((char*)UI.Str_PG1_Mode_Train_Cur, 	"Train");
				sprintf((char*)UI.Str_PG1_Mode_ExtBnc_Cur, 	"ExtBnc");
				
				sprintf((char*)UI.Str_PG1_Param_Pulse_Cur, 	"PulseWidth");
				sprintf((char*)UI.Str_PG1_Param_Amplitude_Cur, "Amplitude");
				sprintf((char*)UI.Str_PG1_Param_Frequency_Cur, "");
				sprintf((char*)UI.Str_PG1_Param_Duration_Cur, "");
				
				if(UserOperation.fUnitCur & (1 << UO_UNIT_BIT_PULSE))
				{
					if(UserOperation.fParamType == UO_PARAM_PULSE && UserOperation.Modify.fStart)																//�������ô˲���
					{
						ValueDisplay_Control(UO_UNIT_BIG, (char*)UI.Str_PG1_Value_Pulse_Cur, UserOperation.C_ModeSingle.Param[UO_PARAM_PULSE], TWINKLE_ENABLE);	//��˸����
					}
					else
					{
						ValueDisplay_Control(UO_UNIT_BIG, (char*)UI.Str_PG1_Value_Pulse_Cur, UserOperation.C_ModeSingle.Param[UO_PARAM_PULSE], TWINKLE_DISABLE);
					}
					sprintf((char*)UI.Str_PG1_Unit_Pulse_Cur, "ms");
				}
				else
				{
					if(UserOperation.fParamType == UO_PARAM_PULSE && UserOperation.Modify.fStart)																//�������ô˲���
					{
						ValueDisplay_Control(UO_UNIT_SMALL, (char*)UI.Str_PG1_Value_Pulse_Cur, UserOperation.C_ModeSingle.Param[UO_PARAM_PULSE], TWINKLE_ENABLE);	//��˸����
					}
					else
					{
						ValueDisplay_Control(UO_UNIT_SMALL, (char*)UI.Str_PG1_Value_Pulse_Cur, UserOperation.C_ModeSingle.Param[UO_PARAM_PULSE], TWINKLE_DISABLE);
					}
					sprintf((char*)UI.Str_PG1_Unit_Pulse_Cur, "~s");
				}
				
				if(UserOperation.fUnitCur & (1 << UO_UNIT_BIT_AMPL_C))
				{
					if(UserOperation.fParamType == UO_PARAM_AMPL && UserOperation.Modify.fStart)
					{
						ValueDisplay_Control(UO_UNIT_BIG, (char*)UI.Str_PG1_Value_Amplitude_Cur, UserOperation.C_ModeSingle.Param[UO_PARAM_AMPL], TWINKLE_ENABLE);
					}
					else
					{
						ValueDisplay_Control(UO_UNIT_BIG, (char*)UI.Str_PG1_Value_Amplitude_Cur, UserOperation.C_ModeSingle.Param[UO_PARAM_AMPL], TWINKLE_DISABLE);
					}
					sprintf((char*)UI.Str_PG1_Unit_Amplitude_Cur, "mA");
				}
				else
				{
					if(UserOperation.fParamType == UO_PARAM_AMPL && UserOperation.Modify.fStart)
					{
						ValueDisplay_Control(UO_UNIT_SMALL, (char*)UI.Str_PG1_Value_Amplitude_Cur, UserOperation.C_ModeSingle.Param[UO_PARAM_AMPL], TWINKLE_ENABLE);
					}
					else
					{
						ValueDisplay_Control(UO_UNIT_SMALL, (char*)UI.Str_PG1_Value_Amplitude_Cur, UserOperation.C_ModeSingle.Param[UO_PARAM_AMPL], TWINKLE_DISABLE);
					}
					sprintf((char*)UI.Str_PG1_Unit_Amplitude_Cur, "~A");
				}
				
				sprintf((char*)UI.Str_PG1_Value_Frequency_Cur, "");
				sprintf((char*)UI.Str_PG1_Unit_Frequency_Cur, "");
				
				sprintf((char*)UI.Str_PG1_Value_Duration_Cur, "");
				sprintf((char*)UI.Str_PG1_Unit_Duration_Cur, "");
				
				break;
				
			case UO_MODE_FREERUN:
				
				sprintf((char*)UI.Str_PG1_Mode_Single_Cur, 	"Single");
				sprintf((char*)UI.Str_PG1_Mode_FreeRun_Cur, "FreeRun");
				sprintf((char*)UI.Str_PG1_Mode_Train_Cur, 	"Train");
				sprintf((char*)UI.Str_PG1_Mode_ExtBnc_Cur, 	"ExtBnc");
				
				sprintf((char*)UI.Str_PG1_Param_Pulse_Cur, 	"PulseWidth");
				sprintf((char*)UI.Str_PG1_Param_Amplitude_Cur, "Amplitude");
				sprintf((char*)UI.Str_PG1_Param_Frequency_Cur, "Frequency");
				sprintf((char*)UI.Str_PG1_Param_Duration_Cur, "");
				
				if(UserOperation.fUnitCur & (1 << UO_UNIT_BIT_PULSE))
				{
					if(UserOperation.fParamType == UO_PARAM_PULSE && UserOperation.Modify.fStart)
					{
						ValueDisplay_Control(UO_UNIT_BIG, (char*)UI.Str_PG1_Value_Pulse_Cur, UserOperation.C_ModeFreeRun.Param[UO_PARAM_PULSE], TWINKLE_ENABLE);
					}
					else
					{
						ValueDisplay_Control(UO_UNIT_BIG, (char*)UI.Str_PG1_Value_Pulse_Cur, UserOperation.C_ModeFreeRun.Param[UO_PARAM_PULSE], TWINKLE_DISABLE);
					}
					sprintf((char*)UI.Str_PG1_Unit_Pulse_Cur, "ms");
				}
				else
				{
					if(UserOperation.fParamType == UO_PARAM_PULSE && UserOperation.Modify.fStart)
					{
						ValueDisplay_Control(UO_UNIT_SMALL, (char*)UI.Str_PG1_Value_Pulse_Cur, UserOperation.C_ModeFreeRun.Param[UO_PARAM_PULSE], TWINKLE_ENABLE);
					}
					else
					{
						ValueDisplay_Control(UO_UNIT_SMALL, (char*)UI.Str_PG1_Value_Pulse_Cur, UserOperation.C_ModeFreeRun.Param[UO_PARAM_PULSE], TWINKLE_DISABLE);
					}					
					sprintf((char*)UI.Str_PG1_Unit_Pulse_Cur, "~s");
				}
				
				if(UserOperation.fUnitCur & (1 << UO_UNIT_BIT_AMPL_C))
				{
					if(UserOperation.fParamType == UO_PARAM_AMPL && UserOperation.Modify.fStart)
					{
						ValueDisplay_Control(UO_UNIT_BIG, (char*)UI.Str_PG1_Value_Amplitude_Cur, UserOperation.C_ModeFreeRun.Param[UO_PARAM_AMPL], TWINKLE_ENABLE);
					}
					else
					{
						ValueDisplay_Control(UO_UNIT_BIG, (char*)UI.Str_PG1_Value_Amplitude_Cur, UserOperation.C_ModeFreeRun.Param[UO_PARAM_AMPL], TWINKLE_DISABLE);
					}
					sprintf((char*)UI.Str_PG1_Unit_Amplitude_Cur, "mA");
				}
				else
				{
					if(UserOperation.fParamType == UO_PARAM_AMPL && UserOperation.Modify.fStart)
					{
						ValueDisplay_Control(UO_UNIT_SMALL, (char*)UI.Str_PG1_Value_Amplitude_Cur, UserOperation.C_ModeFreeRun.Param[UO_PARAM_AMPL], TWINKLE_ENABLE);
					}
					else
					{
						ValueDisplay_Control(UO_UNIT_SMALL, (char*)UI.Str_PG1_Value_Amplitude_Cur, UserOperation.C_ModeFreeRun.Param[UO_PARAM_AMPL], TWINKLE_DISABLE);
					}
					sprintf((char*)UI.Str_PG1_Unit_Amplitude_Cur, "~A");
				}
				
				if(UserOperation.fUnitCur & (1 << UO_UNIT_BIT_FREQ))
				{
					if(UserOperation.fParamType == UO_PARAM_FREQ && UserOperation.Modify.fStart)
					{
						ValueDisplay_Control(UO_UNIT_BIG, (char*)UI.Str_PG1_Value_Frequency_Cur, UserOperation.C_ModeFreeRun.Param[UO_PARAM_FREQ], TWINKLE_ENABLE);
					}
					else
					{
						ValueDisplay_Control(UO_UNIT_BIG, (char*)UI.Str_PG1_Value_Frequency_Cur, UserOperation.C_ModeFreeRun.Param[UO_PARAM_FREQ], TWINKLE_DISABLE);
					}					
					sprintf((char*)UI.Str_PG1_Unit_Frequency_Cur, "KHz");
				}
				else
				{
					if(UserOperation.fParamType == UO_PARAM_FREQ && UserOperation.Modify.fStart)
					{
						ValueDisplay_Control(UO_UNIT_BIG, (char*)UI.Str_PG1_Value_Frequency_Cur, UserOperation.C_ModeFreeRun.Param[UO_PARAM_FREQ], TWINKLE_ENABLE);
					}
					else
					{
						ValueDisplay_Control(UO_UNIT_BIG, (char*)UI.Str_PG1_Value_Frequency_Cur, UserOperation.C_ModeFreeRun.Param[UO_PARAM_FREQ], TWINKLE_DISABLE);
					}					
					sprintf((char*)UI.Str_PG1_Unit_Frequency_Cur, "Hz");
				}				

				sprintf((char*)UI.Str_PG1_Value_Duration_Cur, "");
				sprintf((char*)UI.Str_PG1_Unit_Duration_Cur, "");
			
				break;
			
			case UO_MODE_TRAIN:
				
				sprintf((char*)UI.Str_PG1_Mode_Single_Cur, 	"Single");
				sprintf((char*)UI.Str_PG1_Mode_FreeRun_Cur, "FreeRun");
				sprintf((char*)UI.Str_PG1_Mode_Train_Cur, 	"Train");
				sprintf((char*)UI.Str_PG1_Mode_ExtBnc_Cur, 	"ExtBnc");
				
				sprintf((char*)UI.Str_PG1_Param_Pulse_Cur, 	"PulseWidth");
				sprintf((char*)UI.Str_PG1_Param_Amplitude_Cur, "Amplitude");
				sprintf((char*)UI.Str_PG1_Param_Frequency_Cur, "Frequency");
				sprintf((char*)UI.Str_PG1_Param_Duration_Cur, "Duration");
			
				if(UserOperation.fUnitCur & (1 << UO_UNIT_BIT_PULSE))
				{
					if(UserOperation.fParamType == UO_PARAM_PULSE && UserOperation.Modify.fStart)
					{
						ValueDisplay_Control(UO_UNIT_BIG, (char*)UI.Str_PG1_Value_Pulse_Cur, UserOperation.C_ModeTrain.Param[UO_PARAM_PULSE], TWINKLE_ENABLE);
					}
					else
					{
						ValueDisplay_Control(UO_UNIT_BIG, (char*)UI.Str_PG1_Value_Pulse_Cur, UserOperation.C_ModeTrain.Param[UO_PARAM_PULSE], TWINKLE_DISABLE);
					}
					sprintf((char*)UI.Str_PG1_Unit_Pulse_Cur, "ms");
				}
				else
				{
					if(UserOperation.fParamType == UO_PARAM_PULSE && UserOperation.Modify.fStart)
					{
						ValueDisplay_Control(UO_UNIT_SMALL, (char*)UI.Str_PG1_Value_Pulse_Cur, UserOperation.C_ModeTrain.Param[UO_PARAM_PULSE], TWINKLE_ENABLE);
					}
					else
					{
						ValueDisplay_Control(UO_UNIT_SMALL, (char*)UI.Str_PG1_Value_Pulse_Cur, UserOperation.C_ModeTrain.Param[UO_PARAM_PULSE], TWINKLE_DISABLE);
					}
					sprintf((char*)UI.Str_PG1_Unit_Pulse_Cur, "~s");
				}
				
				if(UserOperation.fUnitCur & (1 << UO_UNIT_BIT_AMPL_C))
				{
					if(UserOperation.fParamType == UO_PARAM_AMPL && UserOperation.Modify.fStart)
					{
						ValueDisplay_Control(UO_UNIT_BIG, (char*)UI.Str_PG1_Value_Amplitude_Cur, UserOperation.C_ModeTrain.Param[UO_PARAM_AMPL], TWINKLE_ENABLE);
					}
					else
					{
						ValueDisplay_Control(UO_UNIT_BIG, (char*)UI.Str_PG1_Value_Amplitude_Cur, UserOperation.C_ModeTrain.Param[UO_PARAM_AMPL], TWINKLE_DISABLE);
					}
					sprintf((char*)UI.Str_PG1_Unit_Amplitude_Cur, "mA");
				}
				else
				{
					if(UserOperation.fParamType == UO_PARAM_AMPL && UserOperation.Modify.fStart)
					{
						ValueDisplay_Control(UO_UNIT_SMALL, (char*)UI.Str_PG1_Value_Amplitude_Cur, UserOperation.C_ModeTrain.Param[UO_PARAM_AMPL], TWINKLE_ENABLE);
					}
					else
					{
						ValueDisplay_Control(UO_UNIT_SMALL, (char*)UI.Str_PG1_Value_Amplitude_Cur, UserOperation.C_ModeTrain.Param[UO_PARAM_AMPL], TWINKLE_DISABLE);
					}
					sprintf((char*)UI.Str_PG1_Unit_Amplitude_Cur, "~A");
				}
				
				if(UserOperation.fUnitCur & (1 << UO_UNIT_BIT_FREQ))
				{
					if(UserOperation.fParamType == UO_PARAM_FREQ && UserOperation.Modify.fStart)
					{
						ValueDisplay_Control(UO_UNIT_BIG, (char*)UI.Str_PG1_Value_Frequency_Cur, UserOperation.C_ModeTrain.Param[UO_PARAM_FREQ], TWINKLE_ENABLE);
					}
					else
					{
						ValueDisplay_Control(UO_UNIT_BIG, (char*)UI.Str_PG1_Value_Frequency_Cur, UserOperation.C_ModeTrain.Param[UO_PARAM_FREQ], TWINKLE_DISABLE);
					}	
					sprintf((char*)UI.Str_PG1_Unit_Frequency_Cur, "KHz");
				}
				else
				{
					if(UserOperation.fParamType == UO_PARAM_FREQ && UserOperation.Modify.fStart)
					{
						ValueDisplay_Control(UO_UNIT_BIG, (char*)UI.Str_PG1_Value_Frequency_Cur, UserOperation.C_ModeTrain.Param[UO_PARAM_FREQ], TWINKLE_ENABLE);
					}
					else
					{
						ValueDisplay_Control(UO_UNIT_BIG, (char*)UI.Str_PG1_Value_Frequency_Cur, UserOperation.C_ModeTrain.Param[UO_PARAM_FREQ], TWINKLE_DISABLE);
					}
					sprintf((char*)UI.Str_PG1_Unit_Frequency_Cur, "Hz");
				}
			
				if(UserOperation.fUnitCur & (1 << UO_UNIT_BIT_DURATION))
				{
					if(UserOperation.fParamType == UO_PARAM_DURATION && UserOperation.Modify.fStart)
					{
						ValueDisplay_Control(UO_UNIT_BIG, (char*)UI.Str_PG1_Value_Duration_Cur, UserOperation.C_ModeTrain.Param[UO_PARAM_DURATION], TWINKLE_ENABLE);
					}
					else
					{
						ValueDisplay_Control(UO_UNIT_BIG, (char*)UI.Str_PG1_Value_Duration_Cur, UserOperation.C_ModeTrain.Param[UO_PARAM_DURATION], TWINKLE_DISABLE);
					}
					sprintf((char*)UI.Str_PG1_Unit_Duration_Cur, "s");
				}
				else
				{
					if(UserOperation.fParamType == UO_PARAM_DURATION && UserOperation.Modify.fStart)
					{
						ValueDisplay_Control(UO_UNIT_SMALL, (char*)UI.Str_PG1_Value_Duration_Cur, UserOperation.C_ModeTrain.Param[UO_PARAM_DURATION], TWINKLE_ENABLE);
					}
					else
					{
						ValueDisplay_Control(UO_UNIT_SMALL, (char*)UI.Str_PG1_Value_Duration_Cur, UserOperation.C_ModeTrain.Param[UO_PARAM_DURATION], TWINKLE_DISABLE);
					}					
					sprintf((char*)UI.Str_PG1_Unit_Duration_Cur, "ms");
				}
			
				break;
			
			case UO_MODE_EXTBNC:
				
				sprintf((char*)UI.Str_PG1_Mode_Single_Cur, 	"Single");
				sprintf((char*)UI.Str_PG1_Mode_FreeRun_Cur,	"FreeRun");
				sprintf((char*)UI.Str_PG1_Mode_Train_Cur, 	"Train");
				sprintf((char*)UI.Str_PG1_Mode_ExtBnc_Cur, 	"ExtBnc");
				
				//sprintf((char*)UI.Str_PG1_Param_Pulse_Cur, 	"");
				sprintf((char*)UI.Str_PG1_Param_Amplitude_Cur, "Amplitude");
				sprintf((char*)UI.Str_PG1_Param_Frequency_Cur, "");
				sprintf((char*)UI.Str_PG1_Param_Duration_Cur, "");
				
//				sprintf((char*)UI.Str_PG1_Value_Pulse_Cur, 	"");
//				sprintf((char*)UI.Str_PG1_Unit_Pulse_Cur, 	"");
			
				sprintf((char*)UI.Str_PG1_Param_Pulse_Cur, 	"PulseWidth");
			
			
				if(UserOperation.fUnitCur & (1 << UO_UNIT_BIT_PULSE))
				{
					if(UserOperation.fParamType == UO_PARAM_PULSE && UserOperation.Modify.fStart)																//�������ô˲���
					{
						ValueDisplay_Control(UO_UNIT_BIG, (char*)UI.Str_PG1_Value_Pulse_Cur, UserOperation.C_ModeExtBnc.Param[UO_PARAM_PULSE], TWINKLE_ENABLE);	//��˸����
					}
					else
					{
						ValueDisplay_Control(UO_UNIT_BIG, (char*)UI.Str_PG1_Value_Pulse_Cur, UserOperation.C_ModeExtBnc.Param[UO_PARAM_PULSE], TWINKLE_DISABLE);
					}
					sprintf((char*)UI.Str_PG1_Unit_Pulse_Cur, "ms");
				}
				else
				{
					if(UserOperation.fParamType == UO_PARAM_PULSE && UserOperation.Modify.fStart)																//�������ô˲���
					{
						ValueDisplay_Control(UO_UNIT_SMALL, (char*)UI.Str_PG1_Value_Pulse_Cur, UserOperation.C_ModeExtBnc.Param[UO_PARAM_PULSE], TWINKLE_ENABLE);	//��˸����
					}
					else
					{
						ValueDisplay_Control(UO_UNIT_SMALL, (char*)UI.Str_PG1_Value_Pulse_Cur, UserOperation.C_ModeExtBnc.Param[UO_PARAM_PULSE], TWINKLE_DISABLE);
					}
					sprintf((char*)UI.Str_PG1_Unit_Pulse_Cur, "~s");
				}
				
				if( sAdditionalData.C_Bnc_Pulse != UserOperation.C_ModeExtBnc.Param[UO_PARAM_PULSE])
				{
					sAdditionalData.C_Bnc_Pulse = UserOperation.C_ModeExtBnc.Param[UO_PARAM_PULSE];
					AT24CXX_Write(230, (void *)&sAdditionalData, sizeof(sAdditionalData));
				}
				
				
				if(UserOperation.fUnitCur & (1 << UO_UNIT_BIT_AMPL_C))
				{
					if(UserOperation.fParamType == UO_PARAM_AMPL && UserOperation.Modify.fStart)
					{
						ValueDisplay_Control(UO_UNIT_BIG, (char*)UI.Str_PG1_Value_Amplitude_Cur, UserOperation.C_ModeExtBnc.Param[UO_PARAM_AMPL], TWINKLE_ENABLE);
					}
					else
					{
						ValueDisplay_Control(UO_UNIT_BIG, (char*)UI.Str_PG1_Value_Amplitude_Cur, UserOperation.C_ModeExtBnc.Param[UO_PARAM_AMPL], TWINKLE_DISABLE);
					}
					sprintf((char*)UI.Str_PG1_Unit_Amplitude_Cur, "mA");
				}
				else
				{
					if(UserOperation.fParamType == UO_PARAM_AMPL && UserOperation.Modify.fStart)
					{
						ValueDisplay_Control(UO_UNIT_SMALL, (char*)UI.Str_PG1_Value_Amplitude_Cur, UserOperation.C_ModeExtBnc.Param[UO_PARAM_AMPL], TWINKLE_ENABLE);
					}
					else
					{
						ValueDisplay_Control(UO_UNIT_SMALL, (char*)UI.Str_PG1_Value_Amplitude_Cur, UserOperation.C_ModeExtBnc.Param[UO_PARAM_AMPL], TWINKLE_DISABLE);
					}
					sprintf((char*)UI.Str_PG1_Unit_Amplitude_Cur, "~A");
				}
			
				sprintf((char*)UI.Str_PG1_Value_Frequency_Cur, "");
				sprintf((char*)UI.Str_PG1_Unit_Frequency_Cur, "");
				
				sprintf((char*)UI.Str_PG1_Value_Duration_Cur, "");
				sprintf((char*)UI.Str_PG1_Unit_Duration_Cur, "");
				
				break;
				
			default:
				break;
		}
	}
}

/********************************************************************
*	����	��	�����������ù����б���ɫˢ��
*	����	��	
				param--��ǰ���ò���
*	˵��	��	ͨ�����ò�����ʼʱ��Str_PG1_XXX_YYY_Pre��գ���Str_PG1_XXX_YYY_Cur
				�Ƚ�ʱ����ȼ�ӿ��ƶ�Ӧ����ˢ��
******************************************************************************/
void ParamEdit_RefreshPre(void)
{
	if(UserOperation.fParamTypePre != UserOperation.fParamType)
	{
		switch(UserOperation.fParamTypePre)
		{
			case UO_PARAM_AMPL:
				sprintf((char*)UI.Str_PG1_Param_Amplitude_Pre, "");
				sprintf((char*)UI.Str_PG1_Value_Amplitude_Pre, "");
				sprintf((char*)UI.Str_PG1_Unit_Amplitude_Pre, "");
				break;
		
			case UO_PARAM_PULSE:
				sprintf((char*)UI.Str_PG1_Param_Pulse_Pre, "");
				sprintf((char*)UI.Str_PG1_Value_Pulse_Pre, "");
				sprintf((char*)UI.Str_PG1_Unit_Pulse_Pre, "");
				break;
			
			case UO_PARAM_FREQ:
				sprintf((char*)UI.Str_PG1_Param_Frequency_Pre, "");
				sprintf((char*)UI.Str_PG1_Value_Frequency_Pre, "");
				sprintf((char*)UI.Str_PG1_Unit_Frequency_Pre, "");
				break;
			
			case UO_PARAM_DURATION:
				sprintf((char*)UI.Str_PG1_Param_Duration_Pre, "");
				sprintf((char*)UI.Str_PG1_Value_Duration_Pre, "");
				sprintf((char*)UI.Str_PG1_Unit_Duration_Pre, "");
				break;
			
			default:
				break;
		}
		UserOperation.fParamTypePre = UserOperation.fParamType;
	}
	switch(UserOperation.fParamType)
	{
		case UO_PARAM_AMPL:
			sprintf((char*)UI.Str_PG1_Param_Amplitude_Pre, "");
			sprintf((char*)UI.Str_PG1_Value_Amplitude_Pre, "");
			sprintf((char*)UI.Str_PG1_Unit_Amplitude_Pre, "");
			break;
		
		case UO_PARAM_PULSE:
			sprintf((char*)UI.Str_PG1_Param_Pulse_Pre, "");
			sprintf((char*)UI.Str_PG1_Value_Pulse_Pre, "");
			sprintf((char*)UI.Str_PG1_Unit_Pulse_Pre, "");
			break;
		
		case UO_PARAM_FREQ:
			sprintf((char*)UI.Str_PG1_Param_Frequency_Pre, "");
			sprintf((char*)UI.Str_PG1_Value_Frequency_Pre, "");
			sprintf((char*)UI.Str_PG1_Unit_Frequency_Pre, "");
			break;
		
		case UO_PARAM_DURATION:
			sprintf((char*)UI.Str_PG1_Param_Duration_Pre, "");
			sprintf((char*)UI.Str_PG1_Value_Duration_Pre, "");
			sprintf((char*)UI.Str_PG1_Unit_Duration_Pre, "");
			break;
		
		default:
			break;
	}
}

#define DISP_AREA_MODE			1
#define DISP_AREA_PARAM			2
#define DISP_AREA_VALUE			3
#define DISP_AREA_VALUE_EDITING	4
#define DISP_AREA_UNIT			5
#define DISP_AREA_STATUS_VC		6
#define DISP_AREA_STATUS_RUN	7
#define DISP_AREA_STATUS_FLIP	8
/********************************************************************
*	����	��	��ʾ�ַ���
*	����	��	
				disparea--ˢ��λ������
				x,y--�������
				width,height--�����С,����ʾ����Ŀ���
				fontsize--�����С
				*p--�ַ�����ʼ��ַ
				backcolor,pointcolor--������ɫ��д����ɫ
*	˵��	��	ͨ��Color�ṹ����ƴ�����ɫ����ʵ������������ɫ����
******************************************************************************/
static void PartialArea_Refresh(uint8_t disparea, u16 x, u16 y, u16 width, u16 height, u8 fontsize, u8* displaycur, u16 backcolor, u16 pointcolor)
{
	switch(disparea)
	{
		case DISP_AREA_MODE:

			LCD_Fill(x, y, x+width, y+height, backcolor);								//[V74]ע�⺯��������ƥ��
										//��ˮƽ���С�															����ֱ���С�
			LCD_ShowString_WithColor(x + (width - strlen((const char *)displaycur) * (fontsize / 2)) / 2, y + (height - fontsize) / 2, width, height, fontsize, displaycur, backcolor, pointcolor);
		
			break;
		
		case DISP_AREA_PARAM:
			
			LCD_Fill(x, y, x+width, y+height, backcolor);
			LCD_ShowString_WithColor(x+20, y+20, width, height, fontsize, displaycur, backcolor, pointcolor);
		
			break;
		
		case DISP_AREA_VALUE:
			
			LCD_Fill(x, y, x+width, y+height, backcolor);
			LCD_ShowString_WithColor(x, y+20, width, height, fontsize, displaycur, backcolor, pointcolor);
		
			break;
		
		case DISP_AREA_VALUE_EDITING:
			
			LCD_Fill(x, y, x+width, y+height, COLOR_UNIT_AREA_EDITING);
			LCD_Fill(x, y+10, x+width-40, y+height-10, backcolor);
			LCD_ShowString_WithColor(x, y+20, width, height, fontsize, displaycur, backcolor, pointcolor);
		
			break;
		
		case DISP_AREA_UNIT:
			
			LCD_Fill(x, y, x+width, y+height, backcolor);
			LCD_ShowString_WithColor(x, y+20, width, height, fontsize, displaycur, backcolor, pointcolor);
		
			break;
		
		case DISP_AREA_STATUS_VC:
			
			LCD_Fill(x, y, x+width, y+height, backcolor);
			LCD_ShowString_WithColor(x +  5, y+18, width, height, 16, (u8*)"TYPE |", backcolor, COLOR_MODE_FONT);
			LCD_ShowString_WithColor(x + 60, y+10, width, height, fontsize, displaycur, backcolor, pointcolor);
		
			break;
		
		case DISP_AREA_STATUS_RUN:
			
			LCD_Fill(x, y, x+width, y+height, backcolor);
			LCD_ShowString_WithColor(x + (width - strlen((const char *)displaycur) * (fontsize / 2)) / 2, y+10, width, height, fontsize, displaycur, backcolor, pointcolor);
		
			break;
		
		case DISP_AREA_STATUS_FLIP:
			
			LCD_Fill(x, y, x+width, y+height, backcolor);
			LCD_ShowString_WithColor(x +  5, y+18, width, height, 16, (u8*)"TYPE:", backcolor, pointcolor);
			//LCD_ShowString_WithColor(x + 68, y+10, width, height, fontsize, displaycur, backcolor, pointcolor);
		
			break;
		
		default:
			break;
	}
}

/********************************************************************
*	����	��	��ʾ����ˢ��
******************************************************************************/
static void UI_DisplayFlush(void)
{
	if(UI.fFlush == FLUSH_START)
	{
		UI.fFlush = FLUSH_ING;
	}
	
	/*		V/C	��ʾ��		*/
	if(strcmp((char*)UI.Str_PG1_VC_Cur, (char*)UI.Str_PG1_VC_Pre) || UI.fFlush == FLUSH_ING)
	{
		Color.BackColor = COLOR_VC_AREA;
		Color.PointColor = COLOR_VC_FONT;
		PartialArea_Refresh(DISP_AREA_STATUS_VC, PG1_VC_START_X, PG1_VC_START_Y, PG1_VC_AREA_WIDTH, PG1_VC_AREA_HEIGHT, FONTSIZE_STATUS_DEFAULT, UI.Str_PG1_VC_Cur, Color.BackColor, Color.PointColor);
		strcpy((char*)UI.Str_PG1_VC_Pre, (char*)UI.Str_PG1_VC_Cur);
	}
	if(strcmp((char*)UI.Str_PG1_Run_Cur, (char*)UI.Str_PG1_Run_Pre) || UI.fFlush == FLUSH_ING)
	{
		if(DOState.Status[DO_TIM4] == DOSTATE_STATUS_COMPLETE)
		{
			Color.BackColor = COLOR_RUN_AREA_STOP;
		}
		else
		{
			Color.BackColor = COLOR_RUN_AREA_RUNNING;
		}
		Color.PointColor = COLOR_RUN_FONT;
		PartialArea_Refresh(DISP_AREA_STATUS_RUN, PG1_RUN_START_X, PG1_RUN_START_Y, PG1_RUN_AREA_WIDTH, PG1_RUN_AREA_HEIGHT, FONTSIZE_STATUS_DEFAULT, UI.Str_PG1_Run_Cur, Color.BackColor, Color.PointColor);
		strcpy((char*)UI.Str_PG1_Run_Pre, (char*)UI.Str_PG1_Run_Cur);
	}
	if(strcmp((char*)UI.Str_PG1_Flip_Cur, (char*)UI.Str_PG1_Flip_Pre) || UI.fFlush == FLUSH_ING)
	{
		Color.BackColor = COLOR_FLIP_AREA;
		Color.PointColor = COLOR_FLIP_FONT;
		PartialArea_Refresh(DISP_AREA_STATUS_FLIP, PG1_FLIP_START_X, PG1_FLIP_START_Y, PG1_FLIP_AREA_WIDTH, PG1_FLIP_AREA_HEIGHT, FONTSIZE_STATUS_DEFAULT, UI.Str_PG1_Flip_Cur, Color.BackColor, Color.PointColor);
		strcpy((char*)UI.Str_PG1_Flip_Pre, (char*)UI.Str_PG1_Flip_Cur);
	}
	
	/*		MODE ��ʾ��		*/
	if(strcmp((char*)UI.Str_PG1_Mode_Single_Cur, (char*)UI.Str_PG1_Mode_Single_Pre) || UI.fFlush == FLUSH_ING)
	{
		if(UserOperation.fMode == UO_MODE_SINGLE)
		{
			Color.BackColor = COLOR_MODE_AREA_EDITING;
			Color.PointColor = COLOR_MODE_FONT;
			PartialArea_Refresh(DISP_AREA_MODE, PG1_MODE_SINGLE_START_X, PG1_MODE_SINGLE_START_Y, PG1_MODE_SINGLE_AREA_WIDTH, PG1_MODE_SINGLE_AREA_HEIGHT, FONTSIZE_MODE_EDITING, UI.Str_PG1_Mode_Single_Cur, Color.BackColor, Color.PointColor);
			Color.BackColor = COLOR_MODE_AREA_EDITING;
			LCD_Fill(PG1_GAP_MOV_START_X, PG1_MODE_SINGLE_START_Y, PG1_GAP_MOV_START_X + PG1_GAP_MOV_AREA_WIDTH, PG1_MODE_SINGLE_START_Y + PG1_GAP_MOV_AREA_HEIGHT, Color.BackColor);			
		}
		else
		{
			Color.BackColor = COLOR_MODE_AREA_DEFAULT;
			Color.PointColor = COLOR_MODE_FONT;
			PartialArea_Refresh(DISP_AREA_MODE, PG1_MODE_SINGLE_START_X, PG1_MODE_SINGLE_START_Y, PG1_MODE_SINGLE_AREA_WIDTH, PG1_MODE_SINGLE_AREA_HEIGHT, FONTSIZE_MODE_DEFAULT, UI.Str_PG1_Mode_Single_Cur, Color.BackColor, Color.PointColor);
			Color.BackColor = COLOR_DESKTOP;
			LCD_Fill(PG1_GAP_MOV_START_X, PG1_MODE_SINGLE_START_Y, PG1_GAP_MOV_START_X + PG1_GAP_MOV_AREA_WIDTH, PG1_MODE_SINGLE_START_Y + PG1_GAP_MOV_AREA_HEIGHT, Color.BackColor);
		}
		strcpy((char*)UI.Str_PG1_Mode_Single_Pre, (char*)UI.Str_PG1_Mode_Single_Cur);
	}
	if(strcmp((char*)UI.Str_PG1_Mode_FreeRun_Cur, (char*)UI.Str_PG1_Mode_FreeRun_Pre) || UI.fFlush == FLUSH_ING)
	{
		if(UserOperation.fMode == UO_MODE_FREERUN)
		{
			Color.BackColor = COLOR_MODE_AREA_EDITING;
			Color.PointColor = COLOR_MODE_FONT;
			PartialArea_Refresh(DISP_AREA_MODE, PG1_MODE_FREERUN_START_X, PG1_MODE_FREERUN_START_Y, PG1_MODE_FREERUN_AREA_WIDTH, PG1_MODE_FREERUN_AREA_HEIGHT, FONTSIZE_MODE_EDITING, UI.Str_PG1_Mode_FreeRun_Cur, Color.BackColor, Color.PointColor);
			Color.BackColor = COLOR_MODE_AREA_EDITING;
			LCD_Fill(PG1_GAP_MOV_START_X, PG1_MODE_FREERUN_START_Y, PG1_GAP_MOV_START_X + PG1_GAP_MOV_AREA_WIDTH, PG1_MODE_FREERUN_START_Y + PG1_GAP_MOV_AREA_HEIGHT, Color.BackColor);
		}
		else
		{
			Color.BackColor = COLOR_MODE_AREA_DEFAULT;
			Color.PointColor = COLOR_MODE_FONT;
			PartialArea_Refresh(DISP_AREA_MODE, PG1_MODE_FREERUN_START_X, PG1_MODE_FREERUN_START_Y, PG1_MODE_FREERUN_AREA_WIDTH, PG1_MODE_FREERUN_AREA_HEIGHT, FONTSIZE_MODE_DEFAULT, UI.Str_PG1_Mode_FreeRun_Cur, Color.BackColor, Color.PointColor);
			Color.BackColor = COLOR_DESKTOP;
			LCD_Fill(PG1_GAP_MOV_START_X, PG1_MODE_FREERUN_START_Y, PG1_GAP_MOV_START_X + PG1_GAP_MOV_AREA_WIDTH, PG1_MODE_FREERUN_START_Y + PG1_GAP_MOV_AREA_HEIGHT, Color.BackColor);
		}
		strcpy((char*)UI.Str_PG1_Mode_FreeRun_Pre, (char*)UI.Str_PG1_Mode_FreeRun_Cur);
	}
	if(strcmp((char*)UI.Str_PG1_Mode_Train_Cur, (char*)UI.Str_PG1_Mode_Train_Pre) || UI.fFlush == FLUSH_ING)
	{
		if(UserOperation.fMode == UO_MODE_TRAIN)
		{
			Color.BackColor = COLOR_MODE_AREA_EDITING;
			Color.PointColor = COLOR_MODE_FONT;
			PartialArea_Refresh(DISP_AREA_MODE, PG1_MODE_TRAIN_START_X, PG1_MODE_TRAIN_START_Y, PG1_MODE_TRAIN_AREA_WIDTH, PG1_MODE_TRAIN_AREA_HEIGHT, FONTSIZE_MODE_EDITING, UI.Str_PG1_Mode_Train_Cur, Color.BackColor, Color.PointColor);
			Color.BackColor = COLOR_MODE_AREA_EDITING;
			LCD_Fill(PG1_GAP_MOV_START_X, PG1_MODE_TRAIN_START_Y, PG1_GAP_MOV_START_X + PG1_GAP_MOV_AREA_WIDTH, PG1_MODE_TRAIN_START_Y + PG1_GAP_MOV_AREA_HEIGHT, Color.BackColor);
		}
		else
		{
			Color.BackColor = COLOR_MODE_AREA_DEFAULT;
			Color.PointColor = COLOR_MODE_FONT;
			PartialArea_Refresh(DISP_AREA_MODE, PG1_MODE_TRAIN_START_X, PG1_MODE_TRAIN_START_Y, PG1_MODE_TRAIN_AREA_WIDTH, PG1_MODE_TRAIN_AREA_HEIGHT, FONTSIZE_MODE_DEFAULT, UI.Str_PG1_Mode_Train_Cur, Color.BackColor, Color.PointColor);
			Color.BackColor = COLOR_DESKTOP;
			LCD_Fill(PG1_GAP_MOV_START_X, PG1_MODE_TRAIN_START_Y, PG1_GAP_MOV_START_X + PG1_GAP_MOV_AREA_WIDTH, PG1_MODE_TRAIN_START_Y + PG1_GAP_MOV_AREA_HEIGHT, Color.BackColor);
		}
		strcpy((char*)UI.Str_PG1_Mode_Train_Pre, (char*)UI.Str_PG1_Mode_Train_Cur);
	}
	if(strcmp((char*)UI.Str_PG1_Mode_ExtBnc_Cur, (char*)UI.Str_PG1_Mode_ExtBnc_Pre) || UI.fFlush == FLUSH_ING)
	{
		if(UserOperation.fMode == UO_MODE_EXTBNC)
		{
			Color.BackColor = COLOR_MODE_AREA_EDITING;
			Color.PointColor = COLOR_MODE_FONT;
			PartialArea_Refresh(DISP_AREA_MODE, PG1_MODE_EXTBNC_START_X, PG1_MODE_EXTBNC_START_Y, PG1_MODE_EXTBNC_AREA_WIDTH, PG1_MODE_EXTBNC_AREA_HEIGHT, FONTSIZE_MODE_EDITING, UI.Str_PG1_Mode_ExtBnc_Cur, Color.BackColor, Color.PointColor);
			Color.BackColor = COLOR_MODE_AREA_EDITING;
			LCD_Fill(PG1_GAP_MOV_START_X, PG1_MODE_EXTBNC_START_Y, PG1_GAP_MOV_START_X + PG1_GAP_MOV_AREA_WIDTH, PG1_MODE_EXTBNC_START_Y + PG1_GAP_MOV_AREA_HEIGHT, Color.BackColor);
		}
		else
		{
			Color.BackColor = COLOR_MODE_AREA_DEFAULT;
			Color.PointColor = COLOR_MODE_FONT;
			PartialArea_Refresh(DISP_AREA_MODE, PG1_MODE_EXTBNC_START_X, PG1_MODE_EXTBNC_START_Y, PG1_MODE_EXTBNC_AREA_WIDTH, PG1_MODE_EXTBNC_AREA_HEIGHT, FONTSIZE_MODE_DEFAULT, UI.Str_PG1_Mode_ExtBnc_Cur, Color.BackColor, Color.PointColor);
			Color.BackColor = COLOR_DESKTOP;
			LCD_Fill(PG1_GAP_MOV_START_X, PG1_MODE_EXTBNC_START_Y, PG1_GAP_MOV_START_X + PG1_GAP_MOV_AREA_WIDTH, PG1_MODE_EXTBNC_START_Y + PG1_GAP_MOV_AREA_HEIGHT, Color.BackColor);
		}
		strcpy((char*)UI.Str_PG1_Mode_ExtBnc_Pre, (char*)UI.Str_PG1_Mode_ExtBnc_Cur);
	}
	
	/*		PARAM ��ʾ��		*/
	if(strcmp((char*)UI.Str_PG1_Param_Pulse_Cur, (char*)UI.Str_PG1_Param_Pulse_Pre) || UI.fFlush == FLUSH_ING)
	{
		if(UserOperation.fParamType != UO_PARAM_PULSE)						//��ǰ����δ���ڱ༭����״̬��������ɫ
		{
			Color.BackColor = COLOR_PARAM_AREA;
			Color.PointColor = COLOR_PARAM_FONT;
		}
		else
		{
			Color.BackColor = COLOR_UNIT_AREA_EDITING;
			Color.PointColor = COLOR_LGRAYBLUE_G;
		}		
		PartialArea_Refresh(DISP_AREA_PARAM, PG1_PARAM_PULSE_START_X, PG1_PARAM_PULSE_START_Y, PG1_PARAM_PULSE_AREA_WIDTH, PG1_PARAM_PULSE_AREA_HEIGHT, FONTSIZE_PARAM_DEFAULT, UI.Str_PG1_Param_Pulse_Cur, Color.BackColor, Color.PointColor);
		strcpy((char*)UI.Str_PG1_Param_Pulse_Pre, (char*)UI.Str_PG1_Param_Pulse_Cur);
	}
	if(strcmp((char*)UI.Str_PG1_Param_Amplitude_Cur, (char*)UI.Str_PG1_Param_Amplitude_Pre) || UI.fFlush == FLUSH_ING)
	{
		if(UserOperation.fParamType != UO_PARAM_AMPL)
		{
			Color.BackColor = COLOR_PARAM_AREA;
			Color.PointColor = COLOR_PARAM_FONT;
		}
		else
		{
			Color.BackColor = COLOR_UNIT_AREA_EDITING;
			Color.PointColor = COLOR_LGRAYBLUE_G;
		}
		PartialArea_Refresh(DISP_AREA_PARAM, PG1_PARAM_AMPLITUDE_START_X, PG1_PARAM_AMPLITUDE_START_Y, PG1_PARAM_AMPLITUDE_AREA_WIDTH, PG1_PARAM_AMPLITUDE_AREA_HEIGHT, FONTSIZE_PARAM_DEFAULT, UI.Str_PG1_Param_Amplitude_Cur, Color.BackColor, Color.PointColor);
		strcpy((char*)UI.Str_PG1_Param_Amplitude_Pre, (char*)UI.Str_PG1_Param_Amplitude_Cur);
	}
	if(strcmp((char*)UI.Str_PG1_Param_Frequency_Cur, (char*)UI.Str_PG1_Param_Frequency_Pre) || UI.fFlush == FLUSH_ING)
	{
		if(UserOperation.fParamType != UO_PARAM_FREQ)
		{
			Color.BackColor = COLOR_PARAM_AREA;
			Color.PointColor = COLOR_PARAM_FONT;
		}
		else
		{
			Color.BackColor = COLOR_UNIT_AREA_EDITING;
			Color.PointColor = COLOR_LGRAYBLUE_G;
		}		
		PartialArea_Refresh(DISP_AREA_PARAM, PG1_PARAM_FREQUENCY_START_X, PG1_PARAM_FREQUENCY_START_Y, PG1_PARAM_FREQUENCY_AREA_WIDTH, PG1_PARAM_FREQUENCY_AREA_HEIGHT, FONTSIZE_PARAM_DEFAULT, UI.Str_PG1_Param_Frequency_Cur, Color.BackColor, Color.PointColor);
		strcpy((char*)UI.Str_PG1_Param_Frequency_Pre, (char*)UI.Str_PG1_Param_Frequency_Cur);
	}
	if(strcmp((char*)UI.Str_PG1_Param_Duration_Cur, (char*)UI.Str_PG1_Param_Duration_Pre) || UI.fFlush == FLUSH_ING)
	{
		if(UserOperation.fParamType != UO_PARAM_DURATION)
		{
			Color.BackColor = COLOR_PARAM_AREA;
			Color.PointColor = COLOR_PARAM_FONT;
		}
		else
		{
			Color.BackColor = COLOR_UNIT_AREA_EDITING;
			Color.PointColor = COLOR_LGRAYBLUE_G;
		}		
		PartialArea_Refresh(DISP_AREA_PARAM, PG1_PARAM_DURATION_START_X, PG1_PARAM_DURATION_START_Y, PG1_PARAM_DURATION_AREA_WIDTH, PG1_PARAM_DURATION_AREA_HEIGHT, FONTSIZE_PARAM_DEFAULT, UI.Str_PG1_Param_Duration_Cur, Color.BackColor, Color.PointColor);
		strcpy((char*)UI.Str_PG1_Param_Duration_Pre, (char*)UI.Str_PG1_Param_Duration_Cur);
	}
	
	/*		VALUE ��ʾ��		*/
	if(strcmp((char*)UI.Str_PG1_Value_Pulse_Cur, (char*)UI.Str_PG1_Value_Pulse_Pre) || UI.fFlush == FLUSH_ING)
	{
		if(UserOperation.fParamType != UO_PARAM_PULSE)
		{
			Color.BackColor = COLOR_VALUE_AREA;
			Color.PointColor = COLOR_VALUE_FONT;
			PartialArea_Refresh(DISP_AREA_VALUE, PG1_VALUE_PULSE_START_X, PG1_VALUE_PULSE_START_Y, PG1_VALUE_PULSE_AREA_WIDTH, PG1_VALUE_PULSE_AREA_HEIGHT, FONTSIZE_VALUE_DEFAULT, UI.Str_PG1_Value_Pulse_Cur, Color.BackColor, Color.PointColor);
		}
		else
		{
			Color.BackColor = COLOR_DESKTOP;
			Color.PointColor = COLOR_LGRAYBLUE_G;
			PartialArea_Refresh(DISP_AREA_VALUE_EDITING, PG1_VALUE_PULSE_START_X, PG1_VALUE_PULSE_START_Y, PG1_VALUE_PULSE_AREA_WIDTH, PG1_VALUE_PULSE_AREA_HEIGHT, FONTSIZE_VALUE_DEFAULT, UI.Str_PG1_Value_Pulse_Cur, Color.BackColor, Color.PointColor);
		}
		strcpy((char*)UI.Str_PG1_Value_Pulse_Pre, (char*)UI.Str_PG1_Value_Pulse_Cur);
	}
	if(strcmp((char*)UI.Str_PG1_Value_Amplitude_Cur, (char*)UI.Str_PG1_Value_Amplitude_Pre) || UI.fFlush == FLUSH_ING)
	{
		if(UserOperation.fParamType != UO_PARAM_AMPL)
		{
			Color.BackColor = COLOR_VALUE_AREA;
			Color.PointColor = COLOR_VALUE_FONT;
			PartialArea_Refresh(DISP_AREA_VALUE, PG1_VALUE_AMPLITUDE_START_X, PG1_VALUE_AMPLITUDE_START_Y, PG1_VALUE_AMPLITUDE_AREA_WIDTH, PG1_VALUE_AMPLITUDE_AREA_HEIGHT, FONTSIZE_VALUE_DEFAULT, UI.Str_PG1_Value_Amplitude_Cur, Color.BackColor, Color.PointColor);
		}
		else
		{
			Color.BackColor = COLOR_DESKTOP;
			Color.PointColor = COLOR_LGRAYBLUE_G;
			PartialArea_Refresh(DISP_AREA_VALUE_EDITING, PG1_VALUE_AMPLITUDE_START_X, PG1_VALUE_AMPLITUDE_START_Y, PG1_VALUE_AMPLITUDE_AREA_WIDTH, PG1_VALUE_AMPLITUDE_AREA_HEIGHT, FONTSIZE_VALUE_DEFAULT, UI.Str_PG1_Value_Amplitude_Cur, Color.BackColor, Color.PointColor);
		}		
		strcpy((char*)UI.Str_PG1_Value_Amplitude_Pre, (char*)UI.Str_PG1_Value_Amplitude_Cur);
	}
	if(strcmp((char*)UI.Str_PG1_Value_Frequency_Cur, (char*)UI.Str_PG1_Value_Frequency_Pre) || UI.fFlush == FLUSH_ING)
	{
		if(UserOperation.fParamType != UO_PARAM_FREQ)
		{
			Color.BackColor = COLOR_VALUE_AREA;
			Color.PointColor = COLOR_VALUE_FONT;
			PartialArea_Refresh(DISP_AREA_VALUE, PG1_VALUE_FREQUENCY_START_X, PG1_VALUE_FREQUENCY_START_Y, PG1_VALUE_FREQUENCY_AREA_WIDTH, PG1_VALUE_FREQUENCY_AREA_HEIGHT, FONTSIZE_VALUE_DEFAULT, UI.Str_PG1_Value_Frequency_Cur, Color.BackColor, Color.PointColor);
		}
		else
		{
			Color.BackColor = COLOR_DESKTOP;
			Color.PointColor = COLOR_LGRAYBLUE_G;
			PartialArea_Refresh(DISP_AREA_VALUE_EDITING, PG1_VALUE_FREQUENCY_START_X, PG1_VALUE_FREQUENCY_START_Y, PG1_VALUE_FREQUENCY_AREA_WIDTH, PG1_VALUE_FREQUENCY_AREA_HEIGHT, FONTSIZE_VALUE_DEFAULT, UI.Str_PG1_Value_Frequency_Cur, Color.BackColor, Color.PointColor);
		}
		strcpy((char*)UI.Str_PG1_Value_Frequency_Pre, (char*)UI.Str_PG1_Value_Frequency_Cur);
	}
	if(strcmp((char*)UI.Str_PG1_Value_Duration_Cur, (char*)UI.Str_PG1_Value_Duration_Pre) || UI.fFlush == FLUSH_ING)
	{
		if(UserOperation.fParamType != UO_PARAM_DURATION)
		{
			Color.BackColor = COLOR_VALUE_AREA;
			Color.PointColor = COLOR_VALUE_FONT;
			PartialArea_Refresh(DISP_AREA_VALUE, PG1_VALUE_DURATION_START_X, PG1_VALUE_DURATION_START_Y, PG1_VALUE_DURATION_AREA_WIDTH, PG1_VALUE_DURATION_AREA_HEIGHT, FONTSIZE_VALUE_DEFAULT, UI.Str_PG1_Value_Duration_Cur, Color.BackColor, Color.PointColor);
		}
		else
		{
			Color.BackColor = COLOR_DESKTOP;
			Color.PointColor = COLOR_LGRAYBLUE_G;
			PartialArea_Refresh(DISP_AREA_VALUE_EDITING, PG1_VALUE_DURATION_START_X, PG1_VALUE_DURATION_START_Y, PG1_VALUE_DURATION_AREA_WIDTH, PG1_VALUE_DURATION_AREA_HEIGHT, FONTSIZE_VALUE_DEFAULT, UI.Str_PG1_Value_Duration_Cur, Color.BackColor, Color.PointColor);
		}
		strcpy((char*)UI.Str_PG1_Value_Duration_Pre, (char*)UI.Str_PG1_Value_Duration_Cur);
	}
	
	/*		UNIT ��ʾ��			*/
	if(strcmp((char*)UI.Str_PG1_Unit_Pulse_Cur, (char*)UI.Str_PG1_Unit_Pulse_Pre) || UI.fFlush == FLUSH_ING)
	{
		if(UserOperation.fParamType != UO_PARAM_PULSE)
		{
			Color.BackColor = COLOR_UNIT_AREA;
			Color.PointColor = COLOR_UNIT_FONT;
		}
		else
		{
			Color.BackColor = COLOR_UNIT_AREA_EDITING;
			Color.PointColor = COLOR_LGRAYBLUE_G;
		}		
		PartialArea_Refresh(DISP_AREA_UNIT, PG1_UNIT_PULSE_START_X, PG1_UNIT_PULSE_START_Y, PG1_UNIT_PULSE_AREA_WIDTH, PG1_UNIT_PULSE_AREA_HEIGHT, FONTSIZE_UNIT_DEFAULT, UI.Str_PG1_Unit_Pulse_Cur, Color.BackColor, Color.PointColor);
		strcpy((char*)UI.Str_PG1_Unit_Pulse_Pre, (char*)UI.Str_PG1_Unit_Pulse_Cur);
	}
	if(strcmp((char*)UI.Str_PG1_Unit_Amplitude_Cur, (char*)UI.Str_PG1_Unit_Amplitude_Pre) || UI.fFlush == FLUSH_ING)
	{
		if(UserOperation.fParamType != UO_PARAM_AMPL)
		{
			Color.BackColor = COLOR_UNIT_AREA;
			Color.PointColor = COLOR_UNIT_FONT;
		}
		else
		{
			Color.BackColor = COLOR_UNIT_AREA_EDITING;
			Color.PointColor = COLOR_LGRAYBLUE_G;
		}
		PartialArea_Refresh(DISP_AREA_UNIT, PG1_UNIT_AMPLITUDE_START_X, PG1_UNIT_AMPLITUDE_START_Y, PG1_UNIT_AMPLITUDE_AREA_WIDTH, PG1_UNIT_AMPLITUDE_AREA_HEIGHT, FONTSIZE_UNIT_DEFAULT, UI.Str_PG1_Unit_Amplitude_Cur, Color.BackColor, Color.PointColor);
		strcpy((char*)UI.Str_PG1_Unit_Amplitude_Pre, (char*)UI.Str_PG1_Unit_Amplitude_Cur);
	}
	if(strcmp((char*)UI.Str_PG1_Unit_Frequency_Cur, (char*)UI.Str_PG1_Unit_Frequency_Pre) || UI.fFlush == FLUSH_ING)
	{
		if(UserOperation.fParamType != UO_PARAM_FREQ)
		{
			Color.BackColor = COLOR_UNIT_AREA;
			Color.PointColor = COLOR_UNIT_FONT;
		}
		else
		{
			Color.BackColor = COLOR_UNIT_AREA_EDITING;
			Color.PointColor = COLOR_LGRAYBLUE_G;
		}		
		PartialArea_Refresh(DISP_AREA_UNIT, PG1_UNIT_FREQUENCY_START_X, PG1_UNIT_FREQUENCY_START_Y, PG1_UNIT_FREQUENCY_AREA_WIDTH, PG1_UNIT_FREQUENCY_AREA_HEIGHT, FONTSIZE_UNIT_DEFAULT, UI.Str_PG1_Unit_Frequency_Cur, Color.BackColor, Color.PointColor);
		strcpy((char*)UI.Str_PG1_Unit_Frequency_Pre, (char*)UI.Str_PG1_Unit_Frequency_Cur);
	}
	if(strcmp((char*)UI.Str_PG1_Unit_Duration_Cur, (char*)UI.Str_PG1_Unit_Duration_Pre) || UI.fFlush == FLUSH_ING)
	{
		if(UserOperation.fParamType != UO_PARAM_DURATION)
		{
			Color.BackColor = COLOR_UNIT_AREA;
			Color.PointColor = COLOR_UNIT_FONT;
		}
		else
		{
			Color.BackColor = COLOR_UNIT_AREA_EDITING;
			Color.PointColor = COLOR_LGRAYBLUE_G;
		}		
		PartialArea_Refresh(DISP_AREA_UNIT, PG1_UNIT_DURATION_START_X, PG1_UNIT_DURATION_START_Y, PG1_UNIT_DURATION_AREA_WIDTH, PG1_UNIT_DURATION_AREA_HEIGHT, FONTSIZE_UNIT_DEFAULT, UI.Str_PG1_Unit_Duration_Cur, Color.BackColor, Color.PointColor);
		strcpy((char*)UI.Str_PG1_Unit_Duration_Pre, (char*)UI.Str_PG1_Unit_Duration_Cur);
	}
	
	if(UI.fFlush == FLUSH_ING)
	{
		UI.fFlush = FLUSH_END;
	}
}

/********************************************************************
*	����	��	�û���������ײ㱳����ʼ��
******************************************************************************/
void UI_DeskTopInit(void)
{
	LCD_Clear(COLOR_DESKTOP);
	
	LCD_Fill(PG1_GAP_FIX_START_X, PG1_GAP_FIX_START_Y, PG1_GAP_FIX_START_X + PG1_GAP_FIX_AREA_WIDTH, PG1_GAP_FIX_START_Y + PG1_GAP_FIX_AREA_HEIGHT, COLOR_GAPLINE);
}

/********************************************************************
*	����	��	�û���������
******************************************************************************/
void UI_Poll(void)
{
	static uint8_t firstin = 0;
	
	UI_ContentScan();					//ʵʱ���¸���ʾ����
	UI_DisplayFlush();				//���б仯���ݽ���ˢ����ʾ
	
	if(firstin == 0)
	{
		LCD_SSD_BackLightSet(100);//��������Ϊ����100
		firstin = 1;
	}
}
