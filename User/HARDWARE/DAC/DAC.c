
#include  "main.h"

/********************************************************************
*	功能	：芯片时序延时
******************************************************************************/
//static void Delay_ns(uint8_t xns)
//{
//	int i = 0;
//	for(i=0; i<xns/140+1; i++)		//1us=72nop  140ns=1nop
//		__nop();
//}


//注意，该函数的参数是输出的实际值，在该函数内部进行运算为寄存器值，优点是实现应用与驱动层的剥离。

//	当前设置下，SPI_CLK=6.72MHz

static void AD5542_WriteA( int64_t val)
{
	unsigned short reg_val;
	unsigned char i=0;
	
	//log_info("Write A=%lld\r\n",val);
		
	//reg_val=(signed short)(val*32768.0/4096.0)+0x8000;	//转换为DAC数据寄存器值	
	reg_val=(signed short)(val*8)+0x8000;
	
	DA_LDAC_H();
	DA_CS1_L();
	DA_CS2_H();
	
	for(i=0;i<16;i++)
	{
		if((reg_val<<i)&0x8000) 
		{
			DA_SDIN_H();
		}
		else
		{
			DA_SDIN_L();
		}
	        //__nop();
	        
		    DA_SCLK_L();
		    __nop();
		    
		    DA_SCLK_H();
			//__nop();
			
	}
			//__nop();
	        
	      
	       
	       DA_LDAC_L();
	       DA_CS1_H();
	       DA_CS2_H();
}

static void AD5542_WriteB(int64_t val)
{
	unsigned short reg_val;
	unsigned char i=0;
	
	//log_info("Write B=%lld\r\n",val);
	
	//reg_val=(signed short)(val*32768.0/4096.0)+0x8000;	//转换为DAC数据寄存器值	
	reg_val=(signed short)(val*8)+0x8000;
	
	DA_LDAC_H();
	DA_CS1_H();
	DA_CS2_L();
	
	for(i=0;i<16;i++)
	{
		if((reg_val<<i)&0x8000) 
		{
			DA_SDIN_H();
		}
		else
		{
			DA_SDIN_L();
		}
	        //__nop();
		
		    DA_SCLK_L();
		
		    __nop();
		
		    DA_SCLK_H();
		
			//__nop();
	}
	
	      // __nop();
	       
	       DA_LDAC_L();
	       DA_CS1_H();
	       DA_CS2_H();
}

///*static*/ void Channel_Out_T1(unsigned int T)
//{
//	uint16_t x = 0;
//	//x = 0xffff * T;
//	
////	#if(DA_OUTPUTRANGE == DA_OUTPUTRANGE_10000MV)
////		x = x / 10000;
////	#elif(DA_OUTPUTRANGE == DA_OUTPUTRANGE_10800MV)
////		x = x / 10800;
////	#elif(DA_OUTPUTRANGE == DA_OUTPUTRANGE_4096MV)
//	
//	x=(unsigned short)(T*32768.0/4096.0)+0x8000;
//	//#endif
//	
//	AD5542_WriteA(x);
//}

//static void Channel_Out_T2(unsigned int T)
//{
//	uint16_t x = 0;
//	//x = 0xffff * T;
////	#if(DA_OUTPUTRANGE == DA_OUTPUTRANGE_10000MV)
////		x = x / 10000;
////	#elif(DA_OUTPUTRANGE == DA_OUTPUTRANGE_10800MV)
////		x = x / 10800;
////	#elif(DA_OUTPUTRANGE == DA_OUTPUTRANGE_4096MV)
//	
//	 x=(unsigned short)(T*32768.0/4096.0)+0x8000;
//	//#endif
//	
//	AD5542_WriteB(x);
//}



//AD5542_Output value=mV
/*******************************************************************************
* Function Name  : AD5542_Output
* Description    : AD5542输出设置
* Input          : ch：通道选择，1选择电压通道，2选择电流通道，3两者都选择，value：DAC输出电压，mV
* Output         : None
* Return         : None
*******************************************************************************/
void AD5542_Output(uint8_t ch, int64_t value)
{
	static uint8_t  ch_pre = 0xff;
	static int64_t value_pre = 0xff;
	
	
	//log_info("ch=%d value=%lld\r\n",ch,value);
	if(ch_pre == ch && value_pre == value)
	{
		return;
	}
	else
	{
		ch_pre = ch;
		value_pre = value;
	}	
	
	if(ch == DA_CHNL_VOLT)
	{
		AD5542_WriteB(0);
		AD5542_WriteA(value);
	}
	else if(ch == DA_CHNL_CURR)								//[0，6.4V]-----[0, 32mA]			[0,6400mV]-----[0,32000uA]		1mA--0.2V	1uA--0.2mV	5uA--1mV
	{
		AD5542_WriteA(0);
		AD5542_WriteB(value);
	}
	else if(ch == DA_CHNL_ALL)
	{
		
		AD5542_WriteA(value);
		AD5542_WriteB(value);
	}
}





void AD5542_Init(void)
{	
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//使能GPIOA时钟
	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化
	
	
	/*	设置两路DAC输出为0mV  */
	
	DA_CS1_H();
	DA_CS2_H();
	DA_LDAC_H();
	DA_SCLK_H();
	DA_SDIN_L();
	
	__nop();__nop();__nop();__nop();__nop();__nop();
}
