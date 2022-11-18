
#include "sys.h"
#include "debug_uart.h"
#include <stdio.h>


//使用串口6作为debug uart
//PG9=Uart_rx
//PG14=Uart_tx

//串口操作变量
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
*  功能   ：将串口数据存储
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
//*  功能   ：初始化串口
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



//初始化IO 串口6
//bound:波特率
void Usart6_Init(u32 bound)
{
	//GPIO端口设置
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG,ENABLE); //使能GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6,ENABLE);//使能USART1时钟
 
	//串口6对应引脚复用映射
	GPIO_PinAFConfig(GPIOG,GPIO_PinSource9,GPIO_AF_USART6); //GPIOG9复用为USART6 RX
	GPIO_PinAFConfig(GPIOG,GPIO_PinSource14,GPIO_AF_USART6); //GPIOG14复用为USART6 TX
	
	//USART6端口配置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_14; //GPIOA9与GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOG,&GPIO_InitStructure); //初始化PA9，PA10

	//USART1 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(USART6, &USART_InitStructure); //初始化串口1
	
	USART_Cmd(USART6, ENABLE);  //使能串口1 
	
	//USART_ClearFlag(USART6, USART_FLAG_TC);
	
#if EN_USART6_RX	
	USART_ITConfig(USART6, USART_IT_RXNE, ENABLE);//开启相关中断

	//Usart1 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART6_IRQn;//串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、

#endif
	
}



/********************************************************************
*  功能   ：串口发送函数
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
