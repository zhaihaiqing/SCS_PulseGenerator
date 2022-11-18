
#include  "main.h"

/********************************************************************
*	����	��	��ʼ��
******************************************************************************/
void IIC_Init(void)
{	
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//ʹ��GPIOBʱ��

	//GPIOB11,E10��ʼ������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��
	
	IIC_SDA_WR = 1;
	IIC_SCL    = 1;
	
	
	
	
// 	RCC->APB2ENR |= 1<<3;		//��ʹ������IO PORTʱ��
//	GPIOB->CRH   &= 0XFFFF00FF;	//�������
//	GPIOB->CRH   |= 0X00003300;
//	GPIOB->ODR   |= 3<<10;		//�����
}

/*********************************************************************
*	����	��	����IIC��ʼ�ź�
********************************************************************************************/
void IIC_Start(void)
{
	SDA_OUT();					//sda�����
	IIC_SDA_WR = 1;
	IIC_SCL    = 1;
	delay_us(8);
 	IIC_SDA_WR = 0;				//START:when CLK is high,DATA change form high to low
	delay_us(8);
	IIC_SCL    = 0;				//ǯסI2C���ߣ�׼�����ͻ��������
}	  

/*********************************************************************
*	����	��	����IICֹͣ�ź�
********************************************************************************************/
void IIC_Stop(void)
{
	SDA_OUT();					//sda�����
	IIC_SCL    = 0;
	IIC_SDA_WR = 0;				//STOP:when CLK is high DATA change form low to high
 	delay_us(8);
	IIC_SCL    = 1;	
	delay_us(8);
	IIC_SDA_WR = 1;				//����I2C���߽����ź�
}

/*********************************************************************
*	����	��	�ȴ�Ӧ���źŵ���
*	����	��	1������Ӧ��ʧ��    
				0������Ӧ��ɹ�
********************************************************************************************/
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime = 0;
	SDA_IN();					//SDA����Ϊ����
	IIC_SDA_WR   = 1;
	delay_us(2);
	IIC_SCL      = 1;
	delay_us(2);
	while(IIC_SDA_RD)
	{
		ucErrTime++;
		if(ucErrTime > 250)
		{
			IIC_Stop();
			return 1;
		}
	}
	IIC_SCL      = 0;			//ʱ�����0
	return 0;  
} 

/*********************************************************************
*	����	��	����ACKӦ��
********************************************************************************************/
void IIC_Ack(void)
{
	IIC_SCL    = 0;
	SDA_OUT();
	IIC_SDA_WR = 0;
	delay_us(4);
	IIC_SCL    = 1;
	delay_us(4);
	IIC_SCL    = 0;
}

/*********************************************************************
*	����	��	������ACKӦ��
********************************************************************************************/
void IIC_NAck(void)
{
	IIC_SCL    = 0;
	SDA_OUT();
	IIC_SDA_WR = 1;
	delay_us(4);
	IIC_SCL    = 1;
	delay_us(4);
	IIC_SCL    = 0;
}					 	

/*********************************************************************
*	����	��	IIC����һ���ֽ�
********************************************************************************************/
void IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	SDA_OUT(); 	    
    IIC_SCL = 0;				//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {              
        IIC_SDA_WR = (txd&0x80)>>7;
        txd      <<= 1;
		delay_us(4);			//��TEA5767��������ʱ���Ǳ����
		IIC_SCL    = 1;
		delay_us(4);
		IIC_SCL    = 0;
		delay_us(4);
    }	 
} 	    

/*********************************************************************
*	����	��	��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
********************************************************************************************/
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();					//SDA����Ϊ����
    for(i=0; i<8; i++ )
	{
        IIC_SCL   = 0;
        delay_us(4);
		IIC_SCL   = 1;
        receive <<= 1;
        if(IIC_SDA_RD)
			receive++;
		delay_us(2);
    }					 
    if (!ack)
        IIC_NAck();				//����nACK
    else
        IIC_Ack();				//����ACK   
    return receive;
}
