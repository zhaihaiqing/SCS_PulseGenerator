
/********************************************************************
*
*	�ļ�	��	UI.h

*	�ܽ�	��	

*	����	��	�û�����������ʾ����[UserInterface]

*	����	��	(000,000)					-					(480,000)


				|													|
				
				
				(000,272)					-					(480, 272)
				
*	����	��	��Ҫʹ����������1206/1608/2412�е�1608���壬��16�����ظߡ�8�����ؿ�
				�����ַ����򡢿�ʼ�ַ�������Ǵ����Ͻǿ�ʼ�����½ǽ���
				
*	����	��	VERSION_01--��һ�汾������ѡ���ַ���
							X_COL_1			2				3			4
				Y_LINE_1	V/C
				2			SINGLE		|	PULSE			VALUE		UNIT
				3			FREERUN		|	AMPLITUDE		VALUE		UNIT
				4			TRAIN		|	FREQUENCY		VALUE		UNIT
				5			EXTBNC		|	DURATION		VALUE		UNIT
				
				VERSION_02--�ڶ��汾������ѡ���ַ���
							[MODE��]		[PARAM��]		[VALUE��]	[UNIT��]
							X_COL_1			2				3			4
				Y_LINE_1	SINGLE			PULSE			VALUE		UNIT
				2			FREERUN			AMPLITUDE		VALUE		UNIT
				3			TRAIN			FREQUENCY		VALUE		UNIT
				4			EXTBNC			DURATION		VALUE		UNIT
				5			V/C				RUN/STOP		FLIP
*
******************************************************************************/
#ifndef __UI_H
#define __UI_H
#include "main.h"

#if(TEST_LCD)
	void test_LCD(void);
	void test_LCDSimpleLayout(void);
#endif

#if(TEST_KEYBOARD)
	void test_KEYBOARD(void);
#endif

/*	��������ɫ��	*/
#define COLOR_WHITE_G      					0xFFFF						//GOOD
#define COLOR_BLACK         				0x0000
#define COLOR_BLUE         					0x001F  
#define COLOR_BRED          				0XF81F
#define COLOR_GRED 							0XFFE0
#define COLOR_GBLUE							0X07FF
#define COLOR_RED           				0xF800
#define COLOR_MAGENTA       				0xF81F
#define COLOR_GREEN         				0x07E0
#define COLOR_CYAN          				0x7FFF
#define COLOR_YELLOW        				0xFFE0
#define COLOR_BROWN 						0XBC40 						//��ɫ
#define COLOR_BRRED 						0XFC07 						//�غ�ɫ
#define COLOR_GRAY  						0X8430 						//��ɫ
#define COLOR_DARKBLUE      				0X01CF						//����ɫ
#define COLOR_LIGHTBLUE      				0X7D7C						//ǳ��ɫ
#define COLOR_GRAYBLUE       				0X5458 						//����ɫ
#define COLOR_LIGHTGREEN     				0X841F 						//ǳ��ɫ
#define COLOR_LIGHTGRAY        				0XEF5B 						//ǳ��ɫ(PANNEL)
#define COLOR_LGRAY_G 						0XC618 						//ǳ��ɫ(PANNEL),���屳��ɫ
#define COLOR_LGRAYBLUE_G      				0XA651 						//ǳ����ɫ(�м����ɫ)
#define COLOR_LBBLUE          				0X2B12 						//ǳ����ɫ(ѡ����Ŀ�ķ�ɫ)
#define COLOR_GUESS_1_G						0XB635
#define COLOR_GUESS_2						0X9618
#define COLOR_GUESS_3_G						0XD635
#define COLOR_GUESS_4						0XE587
#define COLOR_GUESS_5						0XEEEE

/*	�������ɫ��	*/
#define ColorConvert(R, G, B)				(((uint32_t)((R/8)<<11)) + ((uint32_t)((G/4)<<5)) + ((uint32_t)B/8))//((uint32_t)(R*32/256)<<11 + (uint32_t)(G*64/256)<<5 + (uint32_t)(B*32/256)<<0)
#define COLOR_MODE_AREA_DEFAULT				ColorConvert(170, 170, 170)	//Ĭ��MODE���򱳾���ɫ		 86	86	86	--	55	50	40	--	170	170	170
#define COLOR_MODE_AREA_EDITING				ColorConvert(80, 211, 7)	//ѡ��MODE���򱳾���ɫ		167	211	7	--	80	211	7
#define COLOR_MODE_FONT						ColorConvert(0, 0, 0)		//MODE����������ɫ			0	0	0
#define COLOR_PARAM_AREA					COLOR_MODE_AREA_DEFAULT
#define COLOR_PARAM_FONT					ColorConvert(0, 0, 0)		//							 28	28	28	--	5	5	5	--	0	0	0
#define COLOR_VALUE_AREA					COLOR_MODE_AREA_DEFAULT
#define COLOR_VALUE_FONT					COLOR_PARAM_FONT//COLOR_MODE_AREA_EDITING
#define COLOR_UNIT_AREA						COLOR_MODE_AREA_DEFAULT
#define COLOR_UNIT_AREA_EDITING				ColorConvert(70, 70, 70)	//�޸Ĳ���ֵʱ�󱳾�ǳ��ɫ	142 142 142	--	70	70	70
#define COLOR_UNIT_FONT						COLOR_PARAM_FONT//COLOR_MODE_AREA_EDITING
#define COLOR_VC_AREA						ColorConvert(100, 120, 30)	//							 95 132   6	--	100	120	30
#define COLOR_VC_FONT						COLOR_MODE_FONT
#define COLOR_RUN_AREA_STOP					ColorConvert(204, 0, 0)		//							204   0   0	--	204	0	0
#define COLOR_RUN_AREA_RUNNING				COLOR_MODE_AREA_EDITING
#define COLOR_RUN_FONT						COLOR_VC_FONT
#define COLOR_FLIP_AREA						ColorConvert(244, 220, 60)	//							244 199  27	--	244	220	60
#define COLOR_FLIP_FONT						COLOR_VC_FONT
#define COLOR_GAPLINE						COLOR_MODE_AREA_EDITING
#define COLOR_SHORTLINE						COLOR_MODE_FONT
#define COLOR_DESKTOP						ColorConvert(35, 35, 35)	//							 51  51  51	--	35	35	35

/*	��Ӧģʽ��ɫʹ�ú궨��	*/
//#define BACKCOLOR_MODE_DEFAULT			COLOR_GRAY
//#define BACKCOLOR_MODE_SINGLE				COLOR_GUESS_1_G
//#define BACKCOLOR_MODE_FREERUN			COLOR_GUESS_3_G
//#define BACKCOLOR_MODE_TRAIN				COLOR_LGRAY_G
//#define BACKCOLOR_MODE_EXTBNC				COLOR_WHITE_G

//#define POINTCOLOR_MODE_DEFAULT			COLOR_BLACK
//#define POINTCOLOR_MODE_SINGLE			COLOR_BLACK
//#define POINTCOLOR_MODE_FREERUN			COLOR_BLACK
//#define POINTCOLOR_MODE_TRAIN				COLOR_BLACK
//#define POINTCOLOR_MODE_EXTBNC			COLOR_BLACK

typedef struct COLORSTRUCT
{
	uint16_t BackColor;													//��ǰ����ģʽ������ɫ��������ǰ����ģʽ���͡����������ơ�������ֵ��������λ������
	uint16_t PointColor;												//��ǰ����ģʽ�ַ���ɫ��������ǰ����ģʽ���͡����������ơ�������ֵ��������λ���ַ�
}sColor_t;
extern sColor_t Color;

/*		��������ַ���ʾ���ֺ�			*/
#define PG1_Y_LINE_1						0							//��һ����ʼY����
#define PG1_Y_LINE_2						54
#define PG1_Y_LINE_3						108
#define PG1_Y_LINE_4						162
#define PG1_Y_LINE_5						216
#define PG1_Y_LINE_END						272-1

#define PG1_X_COL_1							0							//��һ����ʼX����
#define PG1_X_COL_GAP						21
#define PG1_X_COL_2							158
#define PG1_X_COL_3							270
#define PG1_X_COL_4							400
#define PG1_X_COL_END						480-1

#define PG1_MODE_AREA_WIDTH					(PG1_X_COL_2 - PG1_X_COL_GAP - PG1_X_COL_1 - 1)//��0��ʼ������ߴ�ע���1��������ܻ����ˢ�����򳬹�Ԥ��1�񣬵��½�ϴ���������
#define PG1_PARAM_AREA_WIDTH				(PG1_X_COL_3 - PG1_X_COL_2 - 1)
#define PG1_VALUE_AREA_WIDTH				(PG1_X_COL_4 - PG1_X_COL_3 - 1)
#define PG1_UNIT_AREA_WIDTH					(PG1_X_COL_END - PG1_X_COL_4 - 1)

#define PG1_MODE_AREA_HEIGHT				(PG1_Y_LINE_2 - PG1_Y_LINE_1 - 2)	//ȡֵΪ2ʱ�м���һ�������߼����Ϊ1ʱ�޷�Խӣ�Ϊ0ʱ������һ�����ؽ�������
#define PG1_PARAM_AREA_HEIGHT				(PG1_Y_LINE_2 - PG1_Y_LINE_1 - 2)
#define PG1_VALUE_AREA_HEIGHT				(PG1_Y_LINE_2 - PG1_Y_LINE_1 - 2)
#define PG1_UNIT_AREA_HEIGHT				(PG1_Y_LINE_2 - PG1_Y_LINE_1 - 2)

/*		STATUS��ʾ��	*/
#define PG1_VC_AREA_WIDTH					158
#define PG1_VC_AREA_HEIGHT					42
#define PG1_VC_START_X						0
#define PG1_VC_START_Y						(PG1_Y_LINE_END - PG1_VC_AREA_HEIGHT)

#define PG1_RUN_AREA_WIDTH					158
#define PG1_RUN_AREA_HEIGHT					42
#define PG1_RUN_START_X						PG1_VC_START_X + PG1_VC_AREA_WIDTH + 6
#define PG1_RUN_START_Y						(PG1_Y_LINE_END - PG1_VC_AREA_HEIGHT)

#define PG1_FLIP_AREA_WIDTH					158											//���
#define PG1_FLIP_AREA_HEIGHT				42											//�߶�
#define PG1_FLIP_START_X					(PG1_RUN_START_X + PG1_RUN_AREA_WIDTH + 6)	//��ʼλ��X
#define PG1_FLIP_START_Y					(PG1_Y_LINE_END - PG1_VC_AREA_HEIGHT)		//��ʼλ��Y


/*		MODE ��ʾ��		*/
#define PG1_MODE_SINGLE_START_X				PG1_X_COL_1
#define PG1_MODE_SINGLE_START_Y				PG1_Y_LINE_1
#define PG1_MODE_SINGLE_AREA_WIDTH			PG1_MODE_AREA_WIDTH
#define PG1_MODE_SINGLE_AREA_HEIGHT			PG1_MODE_AREA_HEIGHT

#define PG1_MODE_FREERUN_START_X			PG1_X_COL_1
#define PG1_MODE_FREERUN_START_Y			PG1_Y_LINE_2
#define PG1_MODE_FREERUN_AREA_WIDTH			PG1_MODE_AREA_WIDTH
#define PG1_MODE_FREERUN_AREA_HEIGHT		PG1_MODE_AREA_HEIGHT

#define PG1_MODE_TRAIN_START_X				PG1_X_COL_1
#define PG1_MODE_TRAIN_START_Y				PG1_Y_LINE_3
#define PG1_MODE_TRAIN_AREA_WIDTH			PG1_MODE_AREA_WIDTH
#define PG1_MODE_TRAIN_AREA_HEIGHT			PG1_MODE_AREA_HEIGHT

#define PG1_MODE_EXTBNC_START_X				PG1_X_COL_1
#define PG1_MODE_EXTBNC_START_Y				PG1_Y_LINE_4
#define PG1_MODE_EXTBNC_AREA_WIDTH			PG1_MODE_AREA_WIDTH
#define PG1_MODE_EXTBNC_AREA_HEIGHT			PG1_MODE_AREA_HEIGHT

/*		GAP �����			*/
#define PG1_GAP_FIX_START_X					PG1_X_COL_2 - PG1_X_COL_GAP / 2
#define PG1_GAP_FIX_START_Y					PG1_X_COL_1
#define PG1_GAP_FIX_AREA_WIDTH				PG1_X_COL_GAP / 2
#define PG1_GAP_FIX_AREA_HEIGHT				PG1_Y_LINE_5 - PG1_Y_LINE_1 - 2		//-2ʱ�����ƽ��

#define PG1_GAP_MOV_START_X					PG1_X_COL_2 - PG1_X_COL_GAP
//#define PG1_GAP_MOV_START_Y				PG1_Y_COL_1							//��ѡ��ģʽ�ƶ�
#define PG1_GAP_MOV_AREA_WIDTH				PG1_X_COL_GAP / 2
#define PG1_GAP_MOV_AREA_HEIGHT				PG1_MODE_AREA_HEIGHT

/*		PARAM ��ʾ��		*/
#define PG1_PARAM_AMPLITUDE_START_X			PG1_X_COL_2
#define	PG1_PARAM_AMPLITUDE_START_Y			PG1_Y_LINE_1
#define PG1_PARAM_AMPLITUDE_AREA_WIDTH		PG1_PARAM_AREA_WIDTH
#define PG1_PARAM_AMPLITUDE_AREA_HEIGHT		PG1_PARAM_AREA_HEIGHT

#define PG1_PARAM_PULSE_START_X				PG1_X_COL_2
#define	PG1_PARAM_PULSE_START_Y				PG1_Y_LINE_2
#define PG1_PARAM_PULSE_AREA_WIDTH			PG1_PARAM_AREA_WIDTH
#define PG1_PARAM_PULSE_AREA_HEIGHT			PG1_PARAM_AREA_HEIGHT

#define PG1_PARAM_FREQUENCY_START_X			PG1_X_COL_2
#define	PG1_PARAM_FREQUENCY_START_Y			PG1_Y_LINE_3
#define PG1_PARAM_FREQUENCY_AREA_WIDTH		PG1_PARAM_AREA_WIDTH
#define PG1_PARAM_FREQUENCY_AREA_HEIGHT		PG1_PARAM_AREA_HEIGHT

#define PG1_PARAM_DURATION_START_X			PG1_X_COL_2
#define	PG1_PARAM_DURATION_START_Y			PG1_Y_LINE_4
#define PG1_PARAM_DURATION_AREA_WIDTH		PG1_PARAM_AREA_WIDTH
#define PG1_PARAM_DURATION_AREA_HEIGHT		PG1_PARAM_AREA_HEIGHT

/*		VALUE ��ʾ��		*/
#define PG1_VALUE_AMPLITUDE_START_X			PG1_X_COL_3
#define	PG1_VALUE_AMPLITUDE_START_Y			PG1_Y_LINE_1
#define PG1_VALUE_AMPLITUDE_AREA_WIDTH		PG1_VALUE_AREA_WIDTH
#define PG1_VALUE_AMPLITUDE_AREA_HEIGHT		PG1_VALUE_AREA_HEIGHT

#define PG1_VALUE_PULSE_START_X				PG1_X_COL_3
#define	PG1_VALUE_PULSE_START_Y				PG1_Y_LINE_2
#define PG1_VALUE_PULSE_AREA_WIDTH			PG1_VALUE_AREA_WIDTH
#define PG1_VALUE_PULSE_AREA_HEIGHT			PG1_VALUE_AREA_HEIGHT

#define PG1_VALUE_FREQUENCY_START_X			PG1_X_COL_3
#define	PG1_VALUE_FREQUENCY_START_Y			PG1_Y_LINE_3
#define PG1_VALUE_FREQUENCY_AREA_WIDTH		PG1_VALUE_AREA_WIDTH
#define PG1_VALUE_FREQUENCY_AREA_HEIGHT		PG1_VALUE_AREA_HEIGHT

#define PG1_VALUE_DURATION_START_X			PG1_X_COL_3
#define	PG1_VALUE_DURATION_START_Y			PG1_Y_LINE_4
#define PG1_VALUE_DURATION_AREA_WIDTH		PG1_VALUE_AREA_WIDTH
#define PG1_VALUE_DURATION_AREA_HEIGHT		PG1_VALUE_AREA_HEIGHT

/*		UNIT ��ʾ��			*/
#define PG1_UNIT_AMPLITUDE_START_X			PG1_X_COL_4
#define	PG1_UNIT_AMPLITUDE_START_Y			PG1_Y_LINE_1
#define PG1_UNIT_AMPLITUDE_AREA_WIDTH		PG1_UNIT_AREA_WIDTH
#define PG1_UNIT_AMPLITUDE_AREA_HEIGHT		PG1_UNIT_AREA_HEIGHT

#define PG1_UNIT_PULSE_START_X				PG1_X_COL_4
#define	PG1_UNIT_PULSE_START_Y				PG1_Y_LINE_2
#define PG1_UNIT_PULSE_AREA_WIDTH			PG1_UNIT_AREA_WIDTH
#define PG1_UNIT_PULSE_AREA_HEIGHT			PG1_UNIT_AREA_HEIGHT

#define PG1_UNIT_FREQUENCY_START_X			PG1_X_COL_4
#define	PG1_UNIT_FREQUENCY_START_Y			PG1_Y_LINE_3
#define PG1_UNIT_FREQUENCY_AREA_WIDTH		PG1_UNIT_AREA_WIDTH
#define PG1_UNIT_FREQUENCY_AREA_HEIGHT		PG1_UNIT_AREA_HEIGHT

#define PG1_UNIT_DURATION_START_X			PG1_X_COL_4
#define	PG1_UNIT_DURATION_START_Y			PG1_Y_LINE_4
#define PG1_UNIT_DURATION_AREA_WIDTH		PG1_UNIT_AREA_WIDTH
#define PG1_UNIT_DURATION_AREA_HEIGHT		PG1_UNIT_AREA_HEIGHT

#define FONTSIZE_MODE_DEFAULT	24										//Ĭ��1608
#define FONTSIZE_MODE_EDITING	24
#define FONTSIZE_PARAM_DEFAULT	16										//Ĭ��1608
#define FONTSIZE_VALUE_DEFAULT	16										//Ĭ��1608
#define FONTSIZE_VALUE_EDITING	24
#define FONTSIZE_UNIT_DEFAULT	16										//Ĭ��1608
#define FONTSIZE_STATUS_DEFAULT	24										//Ĭ��2412

typedef enum
{
	FLUSH_START = 1,
	FLUSH_ING = 2,
	FLUSH_END = 0,
}eFlush_t;

typedef struct USERINTERFACE
{
	uint8_t Str_PG1_VC_Cur[15];											//�ַ�����12	*	���壺1608	=	ռ��96					TYPE:Current / Voltage
	uint8_t Str_PG1_VC_Pre[15];
	
	uint8_t Str_PG1_Run_Cur[15];										//�ַ�����11	*	���壺1608	=	ռ��88					STATE:START / STOP
	uint8_t Str_PG1_Run_Pre[15];
	
	uint8_t Str_PG1_Flip_Cur[15];										//�ַ�����12	*	���壺1608	=	ռ��96					FLIP:ENABLE / DISABLE
	uint8_t Str_PG1_Flip_Pre[15];
	
	uint8_t Str_PG1_Mode_Single_Cur[8];									//�ַ�����7		*	���壺2412	=	ռ��84					SINGLE / FREERUN / TRAIN / EXTBNC
	uint8_t Str_PG1_Mode_Single_Pre[8];
	uint8_t Str_PG1_Mode_FreeRun_Cur[8];
	uint8_t Str_PG1_Mode_FreeRun_Pre[8];
	uint8_t Str_PG1_Mode_Train_Cur[8];
	uint8_t Str_PG1_Mode_Train_Pre[8];
	uint8_t Str_PG1_Mode_ExtBnc_Cur[8];
	uint8_t Str_PG1_Mode_ExtBnc_Pre[8];
	
	uint8_t Str_PG1_Param_Pulse_Cur[11];								//�ַ�����9		*	���壺1608	=	ռ��72					PULSE / AMPLITUDE / FREQUENCY / DURATION
	uint8_t Str_PG1_Param_Pulse_Pre[11];
	uint8_t Str_PG1_Param_Amplitude_Cur[11];
	uint8_t Str_PG1_Param_Amplitude_Pre[11];
	uint8_t Str_PG1_Param_Frequency_Cur[11];
	uint8_t Str_PG1_Param_Frequency_Pre[11];
	uint8_t Str_PG1_Param_Duration_Cur[11];
	uint8_t Str_PG1_Param_Duration_Pre[11];
	
	uint8_t Str_PG1_Value_Pulse_Cur[20];								//�ַ�����8		*	���壺1608	=	ռ��64					4.294s / 50.000uA / 100.000mV / 30.000Hz	/ uint32_t--4294967296us=4294967ms=4294.967s��70min
	uint8_t Str_PG1_Value_Pulse_Pre[20];
	uint8_t Str_PG1_Value_Amplitude_Cur[20];
	uint8_t Str_PG1_Value_Amplitude_Pre[20];
	uint8_t Str_PG1_Value_Frequency_Cur[20];
	uint8_t Str_PG1_Value_Frequency_Pre[20];
	uint8_t Str_PG1_Value_Duration_Cur[20];
	uint8_t Str_PG1_Value_Duration_Pre[20];
	
	uint8_t Str_PG1_Unit_Pulse_Cur[5];									//�ַ�����4		*	���壺1608	=	ռ��32					uA / mA / mV / V / usec / msec / Hz / KHz / msec / sec
	uint8_t Str_PG1_Unit_Pulse_Pre[5];
	uint8_t Str_PG1_Unit_Amplitude_Cur[5];
	uint8_t Str_PG1_Unit_Amplitude_Pre[5];
	uint8_t Str_PG1_Unit_Frequency_Cur[5];
	uint8_t Str_PG1_Unit_Frequency_Pre[5];
	uint8_t Str_PG1_Unit_Duration_Cur[5];
	uint8_t Str_PG1_Unit_Duration_Pre[5];
	
	eFlush_t fFlush;													//�л�ģʽ��ͳһˢ��һ�顣�л�ģʽ��1--��ʼˢ����2--ˢ�½�������
}sUserInterface_t;
extern sUserInterface_t UI;

void UI_DeskTopInit(void);
void ParamEdit_RefreshPre(void);
void UI_Poll(void);

#endif
