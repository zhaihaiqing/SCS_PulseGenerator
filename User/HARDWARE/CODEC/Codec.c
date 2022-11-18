
#include  "main.h"

uint8_t Rotate_Increase_Cnt = 0, Rotate_Decrease_Cnt = 0;
/********************************************************************
*	����	��	��ת���������룬���趨ֵ

���յ�PB13�����ж�ʱ������PB14���ŵ�ƽ���ж�ת������

A�ź���B�ź���Ͽ����ж�ת������
	A�½���ʱ��B�ź�Ϊ�ߵ�ƽ��Ϊ˳ʱ�뷽����ת
	
	A�½���ʱ��B�ź�Ϊ�͵�ƽ��Ϊ��ʱ�뷽����ת





******************************************************************************/
void EXTI15_10_IRQHandler(void)
{
	if(EXTI->PR & (1 << 13))	//��⵽PB13�ж�
	{
		delay_us(100);
		if(PULSE_A != PULSE_B)
		{
			Rotate_Increase_Cnt++;
			if(Rotate_Decrease_Cnt)
			{
				Rotate_Decrease_Cnt--;
			}
		}
		else
		{
			Rotate_Decrease_Cnt++;
			if(Rotate_Increase_Cnt)
			{
				Rotate_Increase_Cnt--;
			}
		}
		
		if(T6.CodecRotateCnt == 0)
		{
			T6.CodecRotateCnt = 1;
		}
		
		EXTI->PR |= 1 << 13;
	}
	
	
	
	if(EXTI->PR & (1 << 15)) 	//��⵽PB15�жϣ��ź�����TIM4_CH3
	{
		//GPIOG->ODR ^= GPIO_Pin_10;
		
		if(pTRIGGER_PWM == 1)
		{
			  Output_VorC(UserOperation.bVC, pPwmArrayParam[DO_TIM4]->Ampl, OUTPUT_ENABLE);
		}
		else if(pTRIGGER_PWM == 0)
		{
			  Output_VorC(UserOperation.bVC, 0, OUTPUT_DISABLE);
		}
		
		EXTI->PR |= 1 << 15;
	}
}

/********************************************************************
*	����	��	��ʼ��
******************************************************************************/
void Codec_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//ʹ��GPIOBʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//ʹ��SYSCFGʱ��
	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��ͨ���ģʽ
	//GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	//GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��

	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource13);//PB13 ���ӵ��ж���13
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource15);//PB14 ���ӵ��ж���14
	
	
	 /* ����EXTI_Line0 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line13;//LINE0
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//�ж��¼�
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling; //�����ش��� 
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;//ʹ��LINE0
	EXTI_Init(&EXTI_InitStructure);//����
  
  
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;//�ⲿ�ж�4
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x03;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure);//����
	
	
	
	 /* ����EXTI_Line15 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line15;//LINE0
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//�ж��¼�
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling; //�����ش��� 
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;//ʹ��LINE0
	EXTI_Init(&EXTI_InitStructure);//����
  
  
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;//�ⲿ�ж�4
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x03;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure);//����
  
	
//	RCC->APB2ENR |= 1 << 3;
//	GPIOB->CRH &= 0XF00FFFFF;
//	GPIOB->CRH |= 0X08800000;
//	
//	GPIOB->ODR |= (1 << 13) | (1 << 14);
	
	//Ex_NVIC_Config(GPIO_B, 13, FTIR|RTIR);
	//MY_NVIC_Init(3, 3, EXTI15_10_IRQn, 2);
}
