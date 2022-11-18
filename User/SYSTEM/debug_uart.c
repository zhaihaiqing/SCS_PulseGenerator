
#include "sys.h"
#include "debug_uart.h"
#include <stdio.h>


//ʹ�ô���6��Ϊdebug uart
//PG9=Uart_rx
//PG14=Uart_tx

//���ڲ�������
sUSART6_t U6 = {0, {0}, {0}};

              
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
	while((USART6->SR&0X40)==0);
	USART6->DR = (u8) ch;      
	return ch;
}


/********************************************************************
*  ����   �����������ݴ洢
******************************************************************************/
void USART6_IRQHandler(void)
{
	u8 res;
	
	if(USART6->SR&(1<<5))
	{		
		res = USART2->DR;
		if((U6.Rflag & 0x8000) == 0)
		{
			if((U6.Rflag & 0x3fff) < USART6_REC_LEN)
			{
				if(U6.Rflag == 0)
				{
					U6.Rflag |= 0X4000;
				}
				//T6.U2RxTimeOutCnt = 0;
				U6.Rtbl[U6.Rflag & 0X3FFF] = res;
				U6.Rflag++;
			}			
		}
	}			
}
									 

///********************************************************************
//*  ����   ����ʼ������
//******************************************************************************/
//void Uart2_Init(u32 pclk1,u32 bound)
//{  	 
//	float temp;
//	u16 mantissa;
//	u16 fraction;	   
//	temp=(float)(pclk1*1000000)/(bound*16);
//	mantissa=temp;				 
//	fraction=(temp-mantissa)*16; 	 
//    mantissa<<=4;
//	mantissa+=fraction; 
//	RCC->APB2ENR|=1<<2;  
//	RCC->APB1ENR|=1<<17;  
//	GPIOA->CRL&=0XFFFF00FF; 
//	GPIOA->CRL|=0X00008B00;
//	GPIOA->ODR|=0X000C;
//		  
//	RCC->APB1RSTR|=1<<17;   
//	RCC->APB1RSTR&=~(1<<17);
// 	USART2->BRR=mantissa; 
//	USART2->CR1|=0X200C; 
//	 
//#ifdef EN_USART2_RX		  
//	USART2->CR1|=1<<8;    
//	USART2->CR1|=1<<5;    	    	
//	MY_NVIC_Init(2,2,USART2_IRQn,2);
//	while((USART2->SR&0X40)==0); 
//#endif
//}



//��ʼ��IO ����6
//bound:������
void Usart6_Init(u32 bound)
{
	//GPIO�˿�����
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG,ENABLE); //ʹ��GPIOAʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6,ENABLE);//ʹ��USART1ʱ��
 
	//����6��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOG,GPIO_PinSource9,GPIO_AF_USART6); //GPIOG9����ΪUSART6 RX
	GPIO_PinAFConfig(GPIOG,GPIO_PinSource14,GPIO_AF_USART6); //GPIOG14����ΪUSART6 TX
	
	//USART6�˿�����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_14; //GPIOA9��GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOG,&GPIO_InitStructure); //��ʼ��PA9��PA10

	//USART1 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	USART_Init(USART6, &USART_InitStructure); //��ʼ������1
	
	USART_Cmd(USART6, ENABLE);  //ʹ�ܴ���1 
	
	//USART_ClearFlag(USART6, USART_FLAG_TC);
	
#if EN_USART6_RX	
	USART_ITConfig(USART6, USART_IT_RXNE, ENABLE);//��������ж�

	//Usart1 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART6_IRQn;//����1�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����

#endif
	
}



/********************************************************************
*  ����   �����ڷ��ͺ���
******************************************************************************/
void Usart6_Tx(u8 *buf,u8 len)
{
	u8 i;
	
	for(i=0; i<len; i++)
	{
		USART6->DR  = buf[i];
		while((USART6->SR&0x40) == 0);
	}
}
