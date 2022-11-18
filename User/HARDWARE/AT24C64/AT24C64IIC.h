
/********************************************************************
*
*	�ļ�	��	AT24C64IIC.h

*	�ܽ�	��	
				PB10--AT24C64_SCK
				PB11--AT24C64_SDA

*	����	��	IICЭ��
*
******************************************************************************/
#ifndef __AT24C64IIC_H
#define __AT24C64IIC_H
#include "main.h"

/*	IO��������							*/
#define SDA_IN()  {GPIOB->MODER   &= 0xff3fffff;}									//PB11
#define SDA_OUT() {GPIOB->MODER   &= 0xff3fffff;GPIOB->MODER   |= 0x00400000;}

/*	IO��������							*/
#define IIC_SCL		PBout(10)			//SCL
#define IIC_SDA_WR	PBout(11)			//дSDA
#define IIC_SDA_RD	PBin(11)			//��SDA

void IIC_Init(void);					//��ʼ��IIC��IO��				 
void IIC_Start(void);					//����IIC��ʼ�ź�
void IIC_Stop(void);					//����IICֹͣ�ź�
void IIC_Send_Byte(u8 txd);				//IIC����һ���ֽ�
u8   IIC_Read_Byte(unsigned char ack);	//IIC��ȡһ���ֽ�
u8   IIC_Wait_Ack(void);				//IIC�ȴ�ACK�ź�
void IIC_Ack(void);						//IIC����ACK�ź�
void IIC_NAck(void);					//IIC������ACK�ź�

void IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8	 IIC_Read_One_Byte(u8 daddr,u8 addr);

#endif
