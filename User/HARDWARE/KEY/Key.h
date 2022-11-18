
/********************************************************************
*
*  �ļ�   ��Key.h
*  �ܽ�   ��PC0--PC11���󰴼�
*  ����   ��������������
*  ˵��   ��ɨ�������������������˲����ƣ����а���FIFO�����Լ�������¼���
*				(1) ��������
*				(2) ��������
*				(3) ������
*				(4) ����ʱ�Զ�����
*
******************************************************************************/
#ifndef __KEY_H
#define __KEY_H
#include "main.h"

#define KEY_NORMAL				36
#define KEY_SPECIAL				0
#define KEY_NUMBER				(KEY_NORMAL + KEY_SPECIAL)

/*	FP:	���Ŷ��壬���ö�Ӧ�ܽ�	*/
#define pBTN_L0		PCin(0)				//L--LINE����ѯ��⣬��������
#define pBTN_L1		PCin(1)
#define pBTN_L2		PCin(2)
#define pBTN_L3		PCin(3)
#define pBTN_L4		PCin(4)
#define pBTN_L5		PCin(5)
#define pBTN_C0		PCout(6)			//C--COLUME�����渳ֵ
#define pBTN_C1		PCout(7)
#define pBTN_C2		PCout(8)
#define pBTN_C3		PCout(9)
#define pBTN_C4		PCout(10)
#define pBTN_C5		PCout(11)

/********************************************************************
*	����	��	�������֣�ȷ�����������밴������ֵ��Ӧ��ϵ
*							��һ�沼��									*
	RUN			1			2			3
	FLIP		4			5			6
	SINTRI		7			8			9
				.			0			ENTER			��[BACKSPACE]	CLEAR
	VOL/CUR		SINGLE		TRAIN		FREERUN			EXTTRI
	UNIT		PULSE		AMPL		FREQ			DURATION
*							�ڶ��沼��									*	
	SINGLE		AMPL		1			2				3				RUN
	FREERUN		PULSE		4			5				6				PAUSE
	TRAIN		FREQ		7			8				9				FLIP
	EXTTRI		DURATION	.			0				ENTER	�� ��		SINGLETRI
	VOL/CUR		UNIT SWI
******************************************************************************/
#define BTN_RUN				0x0B
#define BTN_PAUSE			0x0A
#define BTN_PHASE			0x09					//BTN_FLIP
#define BTN_SINGLETRIGGER	0x18
#define BTN_VC				0x04
#define BTN_UNITSWITCH		0x1f
#define BTN_SINGLE			0x00
#define BTN_TRAIN			0x02
#define BTN_FREERUN			0x01
#define BTN_EXTBNC			0x03
#define BTN_PULSE			0x22
#define BTN_AMPL			0x23
#define BTN_FREQ			0x21
#define BTN_DURATION		0x20
#define BTN_1				0x1D
#define BTN_2				0x17
#define BTN_3				0x11
#define BTN_4				0x1C
#define BTN_5				0x16
#define BTN_6				0x10
#define BTN_7				0x1B
#define BTN_8				0x15
#define BTN_9				0x0F
#define BTN_0				0x13
#define BTN_DOT				0x19
#define BTN_ENTER			0x0D
#define BTN_BACKSPACE		0x07
#define BTN_CLEAR			0x1E

/* FP:	����Ӧ�ó���������Ĺ������������������꣬���ڼ�������� */
#define KEY_DOWN_K0				KEY_0_DOWN
#define KEY_UP_K0				KEY_0_UP
#define KEY_LONG_K0				KEY_0_LONG

#define KEY_DOWN_K1				KEY_1_DOWN
#define KEY_UP_K1				KEY_1_UP
#define KEY_LONG_K1				KEY_1_LONG

#define KEY_DOWN_K2				KEY_2_DOWN
#define KEY_UP_K2				KEY_2_UP
#define KEY_LONG_K2				KEY_2_LONG

#define KEY_DOWN_K3				KEY_3_DOWN
#define KEY_UP_K3				KEY_3_UP
#define KEY_LONG_K3				KEY_3_LONG

#define KEY_DOWN_K4				KEY_4_DOWN
#define KEY_UP_K4				KEY_4_UP
#define KEY_LONG_K4				KEY_4_LONG

#define KEY_DOWN_K5				KEY_5_DOWN
#define KEY_UP_K5				KEY_5_UP
#define KEY_LONG_K5				KEY_5_LONG

#define KEY_DOWN_K6				KEY_6_DOWN
#define KEY_UP_K6				KEY_6_UP
#define KEY_LONG_K6				KEY_6_LONG

#define KEY_DOWN_K7				KEY_7_DOWN
#define KEY_UP_K7				KEY_7_UP
#define KEY_LONG_K7				KEY_7_LONG

#define KEY_DOWN_K8				KEY_8_DOWN
#define KEY_UP_K8				KEY_8_UP
#define KEY_LONG_K8				KEY_8_LONG

#define KEY_DOWN_K9				KEY_9_DOWN
#define KEY_UP_K9				KEY_9_UP
#define KEY_LONG_K9				KEY_9_LONG

#define KEY_DOWN_K10			KEY_10_DOWN
#define KEY_UP_K10				KEY_10_UP
#define KEY_LONG_K10			KEY_10_LONG

#define KEY_DOWN_K11			KEY_11_DOWN
#define KEY_UP_K11				KEY_11_UP
#define KEY_LONG_K11			KEY_11_LONG

#define KEY_DOWN_K12			KEY_12_DOWN
#define KEY_UP_K12				KEY_12_UP
#define KEY_LONG_K12			KEY_12_LONG

#define KEY_DOWN_K13			KEY_13_DOWN
#define KEY_UP_K13				KEY_13_UP
#define KEY_LONG_K13			KEY_13_LONG

#define KEY_DOWN_K14			KEY_14_DOWN
#define KEY_UP_K14				KEY_14_UP
#define KEY_LONG_K14			KEY_14_LONG

#define KEY_DOWN_K15			KEY_15_DOWN
#define KEY_UP_K15				KEY_15_UP
#define KEY_LONG_K15			KEY_15_LONG

#define KEY_DOWN_K16			KEY_16_DOWN
#define KEY_UP_K16				KEY_16_UP
#define KEY_LONG_K16			KEY_16_LONG

#define KEY_DOWN_K17			KEY_17_DOWN
#define KEY_UP_K17				KEY_17_UP
#define KEY_LONG_K17			KEY_17_LONG

#define KEY_DOWN_K18			KEY_18_DOWN
#define KEY_UP_K18				KEY_18_UP
#define KEY_LONG_K18			KEY_18_LONG

#define KEY_DOWN_K19			KEY_19_DOWN
#define KEY_UP_K19				KEY_19_UP
#define KEY_LONG_K19			KEY_19_LONG

#define KEY_DOWN_K20			KEY_20_DOWN
#define KEY_UP_K20				KEY_20_UP
#define KEY_LONG_K20			KEY_20_LONG

#define KEY_DOWN_K21			KEY_21_DOWN
#define KEY_UP_K21				KEY_21_UP
#define KEY_LONG_K21			KEY_21_LONG

#define KEY_DOWN_K22			KEY_22_DOWN
#define KEY_UP_K22				KEY_22_UP
#define KEY_LONG_K22			KEY_22_LONG

#define KEY_DOWN_K23			KEY_23_DOWN
#define KEY_UP_K23				KEY_23_UP
#define KEY_LONG_K23			KEY_23_LONG

#define KEY_DOWN_K24			KEY_24_DOWN
#define KEY_UP_K24				KEY_24_UP
#define KEY_LONG_K24			KEY_24_LONG

#define KEY_DOWN_K25			KEY_25_DOWN
#define KEY_UP_K25				KEY_25_UP
#define KEY_LONG_K25			KEY_25_LONG

#define KEY_DOWN_K26			KEY_26_DOWN
#define KEY_UP_K26				KEY_26_UP
#define KEY_LONG_K26			KEY_26_LONG

#define KEY_DOWN_K27			KEY_27_DOWN
#define KEY_UP_K27				KEY_27_UP
#define KEY_LONG_K27			KEY_27_LONG

#define KEY_DOWN_K28			KEY_28_DOWN
#define KEY_UP_K28				KEY_28_UP
#define KEY_LONG_K28			KEY_28_LONG

#define KEY_DOWN_K29			KEY_29_DOWN
#define KEY_UP_K29				KEY_29_UP
#define KEY_LONG_K29			KEY_29_LONG

#define KEY_DOWN_K30			KEY_30_DOWN
#define KEY_UP_K30				KEY_30_UP
#define KEY_LONG_K30			KEY_30_LONG

#define KEY_DOWN_K31			KEY_31_DOWN
#define KEY_UP_K31				KEY_31_UP
#define KEY_LONG_K31			KEY_31_LONG

#define KEY_DOWN_K32			KEY_32_DOWN
#define KEY_UP_K32				KEY_32_UP
#define KEY_LONG_K32			KEY_32_LONG

#define KEY_DOWN_K33			KEY_33_DOWN
#define KEY_UP_K33				KEY_33_UP
#define KEY_LONG_K33			KEY_33_LONG

#define KEY_DOWN_K34			KEY_34_DOWN
#define KEY_UP_K34				KEY_34_UP
#define KEY_LONG_K34			KEY_34_LONG

#define KEY_DOWN_K35			KEY_35_DOWN
#define KEY_UP_K35				KEY_35_UP
#define KEY_LONG_K35			KEY_35_LONG

/*	FP:	����ʵ�ʰ������������ռȶ��������ӻ���ٰ�������
	�����ֵ����, ���밴���´�����ÿ�����İ��¡�����ͳ����¼�

	�Ƽ�ʹ��enum, ����#define��ԭ��
	(1) ����������ֵ,�������˳��ʹ���뿴���������
	(2) �������ɰ����Ǳ����ֵ�ظ���                          */
typedef enum
{
	KEY_NONE = 0,			/* 0 ��ʾ�����¼� */

	KEY_0_DOWN,				/* 0������ */
	KEY_0_UP,				/* 0������ */				//����0x02
	KEY_0_LONG,				/* 0������ */
	
	KEY_1_DOWN,				/* 1������ */
	KEY_1_UP,				/* 1������ */
	KEY_1_LONG,				/* 1������ */

	KEY_2_DOWN,				/* 2������ */
	KEY_2_UP,				/* 2������ */
	KEY_2_LONG,				/* 2������ */

	KEY_3_DOWN,				/* 3������ */
	KEY_3_UP,				/* 3������ */
	KEY_3_LONG,				/* 3������ */

	KEY_4_DOWN,				/* 4������ */
	KEY_4_UP,				/* 4������ */
	KEY_4_LONG,				/* 4������ */

	KEY_5_DOWN,				/* 5������ */
	KEY_5_UP,				/* 5������ */
	KEY_5_LONG,				/* 5������ */

	KEY_6_DOWN,				/* 6������ */
	KEY_6_UP,				/* 6������ */
	KEY_6_LONG,				/* 6������ */

	KEY_7_DOWN,				/* 7������ */
	KEY_7_UP,				/* 7������ */
	KEY_7_LONG,				/* 7������ */

	KEY_8_DOWN,				/* 8������ */
	KEY_8_UP,				/* 8������ */
	KEY_8_LONG,				/* 8������ */
	
	KEY_9_DOWN,				/* 9������ */
	KEY_9_UP,				/* 9������ */		//����0x20
	KEY_9_LONG,				/* 9������ */
	
	KEY_10_DOWN,			/* ���� */
	KEY_10_UP,				/* ���� */
	KEY_10_LONG,			/* ���� */
	
	KEY_11_DOWN,			/* ���� */
	KEY_11_UP,				/* ���� */
	KEY_11_LONG,			/* ���� */
	
	KEY_12_DOWN,			/* ���� */
	KEY_12_UP,				/* ���� */
	KEY_12_LONG,			/* ���� */
	
	KEY_13_DOWN,			/* ���� */
	KEY_13_UP,				/* ���� */
	KEY_13_LONG,			/* ���� */
	
	KEY_14_DOWN,			/* ���� */
	KEY_14_UP,				/* ���� */
	KEY_14_LONG,			/* ���� */
	
	KEY_15_DOWN,			/* ���� */
	KEY_15_UP,				/* ���� */
	KEY_15_LONG,			/* ���� */
	
	KEY_16_DOWN,			/* ���� */
	KEY_16_UP,				/* ���� */
	KEY_16_LONG,			/* ���� */
	
	KEY_17_DOWN,			/* ���� */
	KEY_17_UP,				/* ���� */
	KEY_17_LONG,			/* ���� */
	
	KEY_18_DOWN,			/* ���� */
	KEY_18_UP,				/* ���� */
	KEY_18_LONG,			/* ���� */
	
	KEY_19_DOWN,			/* ���� */
	KEY_19_UP,				/* ���� */
	KEY_19_LONG,			/* ���� */
	
	KEY_20_DOWN,			/* ���� */
	KEY_20_UP,				/* ���� */
	KEY_20_LONG,			/* ���� */	
	
	KEY_21_DOWN,			/* ���� */
	KEY_21_UP,				/* ���� */
	KEY_21_LONG,			/* ���� */
	
	KEY_22_DOWN,			/* ���� */
	KEY_22_UP,				/* ���� */
	KEY_22_LONG,			/* ���� */
	
	KEY_23_DOWN,			/* ���� */
	KEY_23_UP,				/* ���� */
	KEY_23_LONG,			/* ���� */
	
	KEY_24_DOWN,			/* ���� */
	KEY_24_UP,				/* ���� */
	KEY_24_LONG,			/* ���� */
	
	KEY_25_DOWN,			/* ���� */
	KEY_25_UP,				/* ���� */
	KEY_25_LONG,			/* ���� */
	
	KEY_26_DOWN,			/* ���� */
	KEY_26_UP,				/* ���� */
	KEY_26_LONG,			/* ���� */
	
	KEY_27_DOWN,			/* ���� */
	KEY_27_UP,				/* ���� */
	KEY_27_LONG,			/* ���� */
	
	KEY_28_DOWN,			/* ���� */
	KEY_28_UP,				/* ���� */
	KEY_28_LONG,			/* ���� */
	
	KEY_29_DOWN,			/* ���� */
	KEY_29_UP,				/* ���� */
	KEY_29_LONG,			/* ���� */
	
	KEY_30_DOWN,			/* ���� */
	KEY_30_UP,				/* ���� */
	KEY_30_LONG,			/* ���� */
	
	KEY_31_DOWN,			/* ���� */
	KEY_31_UP,				/* ���� */
	KEY_31_LONG,			/* ���� */
	
	KEY_32_DOWN,			/* ���� */
	KEY_32_UP,				/* ���� */
	KEY_32_LONG,			/* ���� */
	
	KEY_33_DOWN,			/* ���� */
	KEY_33_UP,				/* ���� */
	KEY_33_LONG,			/* ���� */
	
	KEY_34_DOWN,			/* ���� */
	KEY_34_UP,				/* ���� */
	KEY_34_LONG,			/* ���� */
	
	KEY_35_DOWN,			/* ���� */
	KEY_35_UP,				/* ���� */
	KEY_35_LONG,			/* ���� */	
}KEY_ENUM;					//ע�ⰴ���������з���ֵ����������ֵ-1��/3���ܽ�����ֵĬ��Ϊ����ֵ���������쳣����

/*	FP:	����ʵ�ʰ������������ռȶ��������ӻ���ٰ�������
	����ID, ��Ҫ����bsp_KeyState()��������ڲ��� 		*/
typedef enum
{
	KID_K0 = 0,
	KID_K1,
	KID_K2,
	KID_K3,
	KID_K4,
	KID_K5,
	KID_K6,
	KID_K7,
	KID_K8,
	KID_K9,
	KID_K10,
	KID_K11,
	KID_K12,
	KID_K13,
	KID_K14,
	KID_K15,
	KID_K16,
	KID_K17,
	KID_K18,
	KID_K19,
	KID_K20,
	KID_K21,
	KID_K22,
	KID_K23,
	KID_K24,
	KID_K25,
	KID_K26,
	KID_K27,
	KID_K28,
	KID_K29,
	KID_K30,
	KID_K31,
	KID_K32,
	KID_K33,
	KID_K34,
	KID_K35,	
}KEY_ID_E;

/*	FP:	�ɸ���ʵ��Ч�����е���
	ֻ��������⵽50ms״̬�������Ϊ��Ч����������Ͱ��������¼�
	��ʹ������·����Ӳ���˲������˲�����Ҳ���Ա�֤�ɿ��ؼ�⵽�����¼�  	*/
#define KEY_FILTER_TIME   5				/* ��λ10ms�� �����˲�ʱ��50ms		*/
#define KEY_LONG_TIME     100			/* ��λ10ms�� ����1�룬��Ϊ�����¼� */

/*	FP:	�����ܣ������޸�	ÿ��������Ӧ1��ȫ�ֵĽṹ�������	*/
typedef struct
{
	uint8_t  Count;						/* �˲��������� */
	uint16_t LongCount;					/* ���������� */
	uint16_t LongTime;					/* �������³���ʱ��, 0��ʾ����ⳤ��������ʱ����Ϊ���� */
	uint8_t  State;						/* ������ǰ״̬�����»��ǵ��� */
	uint8_t  RepeatSpeed;				/* ����ʶ�𰴼����ڣ�������ʱ���ٴν���������д��FIFO */
	uint8_t  RepeatCount;				/* �������������� */
}KEY_T;
extern KEY_T s_tBtn[KEY_NUMBER];

/*	FP:	�����ܣ������޸�	����FIFO�õ����� */
#define KEY_FIFO_SIZE	50
typedef struct							/* FP:	�����ܣ������޸�*/
{
	uint8_t Buf[KEY_FIFO_SIZE];			/* ��ֵ������ */
	uint8_t Read;						/* ��������ָ��1 */
	uint8_t Write;						/* ������дָ�� */
	uint8_t Read2;						/* ��������ָ��2 */
}KEY_FIFO_T;

/*	FP:	���ÿ���оƬ����	*/
void Btn_Init(void);
/*	FP:	�����޸� 	���ⲿ���õĺ���������	*/
void bsp_InitKeyVar(void);				/* ��ʼ���������� */
void bsp_KeyScan(void);					/* ��ѯ��������ʱ���е��� */
void bsp_PutKey(uint8_t _KeyCode);
uint8_t bsp_GetKey(void);				/* ��ȡ����ֵ */
//uint8_t bsp_GetKey2(void);
//uint8_t bsp_GetKeyState(KEY_ID_E _ucKeyID);
//void bsp_SetKeyParam(uint8_t _ucKeyID, uint16_t _LongTime, uint8_t  _RepeatSpeed);
//void bsp_ClearKey(void);

#endif
