
/********************************************************************
*
*  �ļ�   ��usart2.h
*  �ܽ�   ��PA2/USART2_TX     PA3/USART2_RX
*  ����   ��
*
******************************************************************************/

#ifndef __DEBUG_UART_H
#define __DEBUG_UART_H
#include  "main.h"



//����log_info
#define debug
#ifdef debug
		//#define log_info(...)    printf(__VA_ARGS__)
		 #define log_info(...)     printf(__VA_ARGS__)
#else
		#define log_info(...)
#endif



#define USART6_REC_LEN		5
#define USART6_SEND_LEN		5
#define USART6_PCLK			36
#define USART6_BAUD			9600
#define USART6_RecOverTime	2400*16/USART6_BAUD+1		//16ms�ǲ�����2400ʱ�������ý��ճ�ʱ������Ϊ��׼

typedef struct 
{
	u16 Rflag;											//���ձ�־��bit15--�������  bit14--���յ���һ���ֽ�  bit5~0--���յ���Ч�ֽ���Ŀ��
	u8 Rtbl[USART6_REC_LEN];							//��������
	u8 Ttbl[USART6_SEND_LEN];							//��������
}sUSART6_t;

extern sUSART6_t U6;

#define EN_USART6_RX  0
void Usart6_Init(u32 bound); 
void Usart6_Tx(u8 *buf,u8 len);
#endif	 
