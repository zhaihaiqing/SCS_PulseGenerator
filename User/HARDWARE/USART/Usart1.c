
#include  "main.h"

#if 0              
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
 
void _sys_exit(int x) 
{ 
	x = x;
} 
 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);
	USART1->DR = (u8) ch;
	return ch;
}
#endif 

//���ڲ�������
sUART1_t U1 = {  
					0,     //Rflag 
					{0},   //Rtbl[USART1_REC_LEN]
					{0}    //Ttbl[USART1_SEND_LEN]
			  };

/********************************************************************
*	����	��	����������ʵʱ�洢��δ���н���������ȷ���ļ��鴦��
******************************************************************************/
void USART1_IRQHandler(void)
{
	u8 res = 0;
	
	if(USART1->SR&(1<<5))		//�����RXE�жϣ���ʾ���յ�1byte����
	{		
		res = USART1->DR;
		if((U1.Rflag & 0x8000) == 0)
		{
			if((U1.Rflag & 0x3fff) < USART1_REC_LEN)
			{
				if(U1.Rflag == 0)
				{
					U1.Rflag |= 0X4000;
				}
				//T6.U1RxTimeOutCnt = 0;
				U1.Rtbl[U1.Rflag & 0X3FFF] = res;
				U1.Rflag++;
			}	
		}
	}
		
	else if(USART1->SR&(1<<4))	//����ǿ����жϣ���ʾ���յ�1֡����
	{
		U1.Rflag |= 0X8000;
		//T6.U1RxTimeOutCnt = 0;
		
	}
}
									 

/********************************************************************
*	����	��	��ʼ������1
******************************************************************************/
void Uart1_Init(u32 bound)
{  	 
	
	//GPIO�˿�����
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //ʹ��GPIOAʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//ʹ��USART1ʱ��
 
	//����6��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); //GPIOG9����ΪUSART6 RX
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); //GPIOG14����ΪUSART6 TX
	
	//USART6�˿�����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; //GPIOA9��GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOA,&GPIO_InitStructure); //��ʼ��PA9��PA10

	//USART1 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	USART_Init(USART1, &USART_InitStructure); //��ʼ������1
	
	USART_Cmd(USART1, ENABLE);  //ʹ�ܴ���1 
	
	USART_ClearFlag(USART1, USART_FLAG_TC);
	
#if EN_USART1_RX	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);	//��������ж�
	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);	//���������ж�
	
	//Usart1 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//����1�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����

#endif


//	float temp;
//	u16 mantissa;
//	u16 fraction;	   
//	temp=(float)(pclk1*1000000)/(bound*16);
//	mantissa=temp;				 
//	fraction=(temp-mantissa)*16; 	 
//    mantissa<<=4;
//	mantissa+=fraction; 
//	RCC->APB2ENR|=1<<2;  
//	RCC->APB2ENR|=1<<14;  
//	GPIOA->CRH&=0XFFFFF00F; 
//	GPIOA->CRH|=0X000008B0;
//	GPIOA->ODR|=0X0600;
//		  
//	RCC->APB2RSTR|=1<<14;   
//	RCC->APB2RSTR&=~(1<<14);
// 	USART1->BRR=mantissa; 
//	USART1->CR1|=0X200C; 
//	 
//#ifdef EN_USART1_RX		  
//	USART1->CR1|=1<<8;    
//	USART1->CR1|=1<<5;    	    	
//	MY_NVIC_Init(1,1,USART1_IRQn,2);
//	while((USART1->SR&0X40)==0);
//#endif
}

/********************************************************************
*	����	��	���ڷ��ͺ���
******************************************************************************/
void Uart1_Tx(u8 *buf,u8 len)
{
	u8 i;
	
	for(i=0; i<len; i++)
	{
		USART1->DR  = buf[i];
		while((USART1->SR&0x40) == 0);
	}
}

