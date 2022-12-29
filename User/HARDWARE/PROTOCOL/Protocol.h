
/********************************************************************
*
*	�ļ�	��	Protocol.h

*	�ܽ�	��	(NONE)

*	����	��	�������ݽ��ռ�Ӧ��
				�������ƶ�ͨ��Э�顢������λ������
*
******************************************************************************/
#ifndef __PROTOCOL_H
#define __PROTOCOL_H
#include "main.h"

#define PROTOCOL_FIXED_HEAD			0X36
#define PROTOCOL_FIXED_END			0X32
#define PROTOCOL_FIXED_VERSION		0X01

#define ADDR_OFFSET_HEAD			0
#define ADDR_OFFSET_NUM_H			1
#define ADDR_OFFSET_NUM_L			2
#define ADDR_OFFSET_VERSION			3
#define ADDR_OFFSET_COMMAND			4
#define ADDR_OFFSET_RESERVE			5

#define ADDR_OFFSET_CONTENT_START	6				//������֡���У��粨�����á���ͣ����ֹͣ�����ö�ʱ����Ϣ

#define COMMAND_START	0X01
#define COMMAND_STOP	0X02
#define COMMAND_QUERY	0X03
#define COMMAND_CONFIG	0X10
#define COMMAND_ALARM	0X30
#define COMMAND_PAUSE	0X40

#define BTOL( ray ) ( (((uint32_t) (ray)[0]) <<24) + (((uint32_t)(ray)[1])<<16) + (((uint32_t)(ray)[2])<<8) + (ray)[3] )
#define BTOW( ray ) ( (((uint16_t) (ray)[0]) <<8) + (ray)[1] )

typedef enum EPULSE_DO								//ͨ����,data output number
{
	PULSE_DO_0 = 0,
	PULSE_DO_1,
	PULSE_DO_2,
	PULSE_DO_3,
	PULSE_DO_4,
	PULSE_DO_5,
	PULSE_DO_6,
	PULSE_DO_7,
	PULSE_DO_ALL = 8,
	PULSE_DO_NO = 0XFF
}ePulseDO_t;

typedef enum EPULSE_TYPE							//��������
{
	PULSE_TYPE_ZERO = 0,							//���ʵ�ָ��Ӳ���
	PULSE_TYPE_ONE,
	PULSE_TYPE_TWO,
	PULSE_TYPE_THREE,
	PULSE_TYPE_FOUR,
	PULSE_TYPE_FIVE
}ePulseType_t;

#define PULSE_ZERO_TBL_MAX_NUM			1000		//ʹ�ö�̬�ڴ����ȽϺ��ʣ��������Ľ�
__packed typedef struct SPULSEZERO
{
	uint16_t IndexNum[8];							//�������
	uint32_t *TblTime[8];							//�������ʱ������(X100us)
	uint32_t *TblAmpl[8];							//�����ֵ(mV/uA)
}sPulseZero_t;

__packed typedef struct SPULSEONE
{
	uint8_t  IsLowFirst;							//1-�ȸߺ�ͣ�0-�ȵͺ��
	uint32_t MsStageFirst;							//��һ�׶γ���ʱ��
	uint32_t MsStageSecond;							//�ڶ��׶γ���ʱ��
	uint32_t Ampl;									//��ֵ(mV/uA)
}sPulseOne_t;

__packed typedef struct SPULSETWO
{	
	uint32_t Freq;									//0.01Hz
	uint32_t Duty;									//50000��֮X
	uint32_t MsDuration;							//����ʱ��
	uint32_t Ampl;									//��ֵ(mV/uA)
}sPulseTwo_t;

__packed typedef struct SPULSETHREE
{
	uint32_t Period;								//�ظ������������ms
	uint32_t HighTime;								//�̶�����ߵ�ƽʱ��
	uint32_t PulseNum;								//������������
	uint16_t Pause100ms;							//��ͣʱ��
	uint16_t MsBegin;								//�����ʼʱ��
	uint16_t MsEnd;									//�����ֹʱ��
	uint16_t MsStep;								//�������ʱ��
	uint32_t CycleNum;								//ѭ������
	uint32_t Ampl;									//��ֵ(mV/uA)
}sPulseThree_t;

__packed typedef struct SPULSEFOUR
{
	uint32_t PeriodBegin;							//��ʼ����
	uint32_t HighTime;								//�̶�����ߵ�ƽʱ��
	uint32_t PulseNum;								//��ͬ���ڲ�����������
	uint16_t Pause100ms;							//��ͣʱ��
	uint32_t PeriodEnd;								//��������
	uint32_t PeriodStep;							//��������
	uint32_t CycleNum;								//ѭ������
	uint32_t Ampl;									//��ֵ(mV/uA)
}sPulseFour_t;

#define PULSE_FIVE_TBL_MAX_NUM			1000		//ʹ�ö�̬�ڴ����ȽϺ��ʣ��������Ľ�
__packed typedef struct SPULSEFIVE
{
	uint32_t Freq;
	uint16_t Duty;									//ռ�ձȹ̶�50%������˱���������չ������
	uint32_t ConfigNum;								//��������
	uint16_t MsPause[PULSE_FIVE_TBL_MAX_NUM];		//��ͣʱ��
	uint32_t MsDuration[PULSE_FIVE_TBL_MAX_NUM];	//����ʱ��
	uint32_t Ampl;									//��ֵ(mV/uA)
}sPulseFive_t;

__packed typedef union UPULSEPARAM
{
	sPulseZero_t	PulseZero;
	sPulseOne_t		PulseOne;
	sPulseTwo_t		PulseTwo;
	sPulseThree_t	PulseThree;
	sPulseFour_t	PulseFour;
	sPulseFive_t	PulseFive;
}uPulseParam_t;


__packed typedef struct SPULSECONF					//�������ò����ṹ��
{
	ePulseDO_t			Channel;
	ePulseType_t		PulseType;					//��������
	uPulseParam_t		PulseParam;					//�û����ò������գ����ڽ���ת��
}sPulseConf_t;
extern sPulseConf_t PulseConf;


extern __IO uint32_t tim5_arr;
extern __IO uint32_t train_count;	//TRAINģʽ��������
extern __IO uint32_t train_acount;	//TRAINģʽ��Ҫ���������������磺10S��Freq=10Hz��������=10*10=100����ע�⣬ĩβ����1����ȥ

void Process_COMMAND_START(void);
void Process_COMMAND_STOP(void);
void Process_COMMAND_PAUSE(void);
void Process_COMMAND_CONTINUE(void);

#endif
