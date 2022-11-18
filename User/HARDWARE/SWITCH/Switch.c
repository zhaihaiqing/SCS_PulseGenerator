
#include  "main.h"

sSwitch_t Switch = {0x00, 0x00};
/********************************************************************
*	����	��	��ʼ��
******************************************************************************/
void Switch_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//ʹ��GPIOAʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//ʹ��GPIOBʱ��
	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;						//PA7 ��������̵���
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;						//PB12 ����ģʽ�̵���
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��
}

/********************************************************************
*	����	��	�л�����
******************************************************************************/
void Switch_Ctrl(uint8_t phase)
{
	static uint8_t bphase_pre = 0x00;
	
	if(Switch.ModeCur != Switch.ModePre || bphase_pre != phase)
	{
		if(phase == UO_PHASE_UNIPHASE)
		{
			if((Switch.ModeCur & (1 << MODE_BIT_VC)) == (SELECT_VC_V << MODE_BIT_VC))   //MODE_BIT_VC		0     SELECT_VC_V		1
			{
				//�����л�����ѹ������
				Voltage_Ploar_Flag	=	0;
				Current_Ploar_Flag	=	0;
				log_info("SW Mode to Voltage, Positive voltage!\r\n");
				
				SW_CV_MODE = 0;     //��ѹģʽ
			}
			else
			{
				//�����л�,����������
				Voltage_Ploar_Flag	=	0;
				Current_Ploar_Flag	=	0;
				log_info("SW Mode to Current, Positive current!\r\n");
				SW_CV_MODE = 1;       //����ģʽ
				
			}
		}
		else if(phase == UO_PHASE_BIPHASE)
		{
			if((Switch.ModeCur & (1 << MODE_BIT_VC)) == (SELECT_VC_V << MODE_BIT_VC))
			{
				//�����л�,��ѹ������
				Voltage_Ploar_Flag	=	1;
				Current_Ploar_Flag	=	1;
				log_info("SW Mode to Voltage, Negative voltage!\r\n");
				SW_CV_MODE = 0 ;
			}
			else
			{
				//�����л�,����������
				Voltage_Ploar_Flag	=	1;
				Current_Ploar_Flag	=	1;
				log_info("SW Mode to Current, Negative current!\r\n");
				SW_CV_MODE = 1 ;
			}
		}			
		
		Switch.ModePre = Switch.ModeCur;
		bphase_pre = phase;
	}
}
