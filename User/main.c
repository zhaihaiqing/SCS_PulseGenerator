/**
  ******************************************************************************
  * @file    Project/STM32F4xx_StdPeriph_Templates/main.c 
  * @author  MCD Application Team
  * @version V1.7.0
  * @date    22-April-2016
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2016 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** @addtogroup Template_Project
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static __IO uint32_t uwTimingDelay;
RCC_ClocksTypeDef RCC_Clocks;

/* Private function prototypes -----------------------------------------------*/
static void Delay(__IO uint32_t nTime);


volatile unsigned int SysTick_Count = 0;   //Systick����
volatile unsigned int SysTick_Count1 = 0;   //Systick����
volatile unsigned int TimingDelay = 0;     //��ʱ��������

//volatile unsigned char Voltage_Ploar_Flag = 0; 		//��ѹ����ȫ�ֱ�����0���������ԣ�1��������
//volatile unsigned char Current_Ploar_Flag = 0; 		//��������ȫ�ֱ�����0���������ԣ�1��������


void IO_Init()
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//ʹ��GPIOEʱ��
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);//ʹ��GPIOEʱ��
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);//ʹ��GPIOEʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);//ʹ��GPIOEʱ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_Init(GPIOE, &GPIO_InitStructure);//��ʼ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_Init(GPIOF, &GPIO_InitStructure);//��ʼ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_15;
	GPIO_Init(GPIOG, &GPIO_InitStructure);//��ʼ��
	
	
	GPIO_SetBits(GPIOA, GPIO_Pin_2 | GPIO_Pin_3 );
	
}

void get_RCCCLOCK(void)
{
	//char str[32]={0};
	RCC_ClocksTypeDef  RCC_Clocks;
	RCC_GetClocksFreq(&RCC_Clocks);
	
	log_info("HCLK_Frequency:%.3fMHz\r\n",RCC_Clocks.HCLK_Frequency/1000000.0);
	log_info("PCLK1_Frequency:%.3fMHz\r\n",RCC_Clocks.PCLK1_Frequency/1000000.0);
	log_info("PCLK2_Frequency:%.3fMHz\r\n",RCC_Clocks.PCLK2_Frequency/1000000.0);
	log_info("SYSCLK_Frequency:%.3fMHz\r\n",RCC_Clocks.SYSCLK_Frequency/1000000.0);
}

/*******************************************************************************
* Function Name  : Delay function
* Description    : ��ʱ����������ʱ����sleepģʽ
* Input          : ��ʱʱ����ms��
* Output         : None
* Return         : None
*******************************************************************************/
void Delay_ms(volatile unsigned int nTime)
{ 
	TimingDelay = nTime;
	while(TimingDelay != 0);//__WFI;//�ȴ������н���sleepģʽ
}


void delay_us(uint32_t us)
{
    uint32_t delta;
    /* �����ʱ������ tick �� */
    us = us * (SysTick->LOAD/(1000000/1000));
    /* ��õ�ǰʱ�� */
    delta = SysTick->VAL;
    /* ѭ����õ�ǰʱ�䣬ֱ���ﵽָ����ʱ����˳�ѭ�� */
    while (delta - SysTick->VAL< us);
}

/*******************************************************************************
* Function Name  : SysTick init function
* Description    : ���ö�ʱ���ȼ�SysTick���ȼ�
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Init_SysTick(void)
{
	SysTick_Config(SystemCoreClock / 1000); //���ö�ʱ���ȣ�1ms
	NVIC_SetPriority(SysTick_IRQn, 0x0);    //SysTick�ж����ȼ�����
}

/*******************************************************************************
* Function Name  : Init_Devices function
* Description    : ���Ӳ���豸��������ģ���ʼ��
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Init_Devices(void)
{
	u8 test=0;
	u8 tbuff[5]={0};
	u16 T2_ARR=0,T2_PSC=0;
	u16 T6_ARR=0,T6_PSC=0;
	//��ɻ���Ӳ����ʼ��
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�ж����ȼ�����	
	
	Init_SysTick();//����SysTick��ʱ��
	
	WDG_Init(4,3200);
	
	IO_Init();
	Usart6_Init(DEBUG_UART_RATE);
	
	log_info("\r\n");
	log_info("****************************************************************\r\n");
	log_info("MCU Start!\r\n");
	get_RCCCLOCK();
	log_info("Debgu_Uart init OK! baud rate=%d bit/s\r\n\r\n",DEBUG_UART_RATE);
	
	
/*
*	��ʱ��ʱ��84M��16bitװ��ֵ
*	1��1000ms����ʱ����Ƶϵ��8400������84M/8400=10Khz�ļ���Ƶ�ʣ�����10000��Ϊ1000ms
*	2��100ms����ʱ����Ƶϵ��8400������84M/8400=10Khz�ļ���Ƶ�ʣ�����1000��Ϊ100ms
*	3��10ms����ʱ����Ƶϵ��840������84M/840=100Khz�ļ���Ƶ�ʣ�����1000��Ϊ10ms
*	4��1ms��ʱ����Ƶϵ��84������84M/84=1000Khz�ļ���Ƶ�ʣ�����1000��Ϊ1ms
*	5��100us��ʱ����Ƶϵ��84������84M/84=1000Khz�ļ���Ƶ�ʣ�����100��Ϊ100us
*/
	Delay_ms(100);	//�ȴ�
	APOW_ON();		//��ģ���Դ
		log_info("Anglog power has open!\r\n");
		log_info("Wait for analog power to stabilize......!\r\n");
		Delay_ms(300);	//�ȴ�ģ���Դ�ȶ�
	LED_Init();
		log_info("LED_Init OK!\r\n");
	Beep_Init();
		log_info("Beep_Init OK!\r\n");
	IIC_Init();
		log_info("IIC_Init OK!\r\n");
	Btn_Init();		//����������ͨ����ʱ������⣬ÿ��10msִ��һ�μ��
		log_info("Btn_Init OK!\r\n");
	AD5542_Init();
	AD5542_Output(DA_CHNL_ALL, 0);
		log_info("AD5542_Init OK,Set AD5542 Output to zero!\r\n");
	Codec_Init();
		log_info("Codec_Init OK!\r\n");
	TriggerExIN01_Init();
	TriggerOUT_Init();
		log_info("TriggerExIN12_Init OK!\r\n");
	AT24CXX_Init();
		log_info("AT24CXX_Init OK!\r\n");
	Uart1_Init(USART1_BAUD);
		log_info("Uart1_Init OK! baud rate=%d bit/s\r\n",USART1_BAUD);
	My_Mem_Init(SRAMIN);
	Switch_Init();
	pLEDOUTPUT=1;
	WDG_Feed();
	LCDRST_Init();
		log_info("Reset LCD!\r\n");
	Delay_ms(600);	
	WDG_Feed();
	LCD_Init();
		log_info("LCD OK! LCDDEV_ID=0x%x\r\n",lcddev.id);
	Delay_ms(400);							//��ʱ��������Ļһ�����������ٵ�������ʱ=400ms��Ļ��ʼ��ʾ�ַ��������쳣���ƣ�=500ms��Ļ��ʾ������// longzhigu:  [V194]	
	WDG_Feed();
	Memory_ConfigLoad();
	UI_DeskTopInit();
	WDG_Feed();
	T6_ARR=999;
	T6_PSC=83;
	Timer6_Init(T6_ARR,T6_PSC);		//1msһ���ж�
	log_info("Init Timer6 OK!\r\n");
	log_info("Timer6 ARR=%d,PSC=%d\r\n",T6_ARR,T6_PSC);
	log_info("Timer6 Overflow_time=%dus\r\n",(T6_ARR+1)*(T6_PSC+1)/84);
	WDG_Feed();
	
	//��ʱ��5��PWM1ģʽ��
	Timer5_Init(10000-1,41);	//�̶���Ƶ��0.5us��PWMƵ��=0.5us*(arr+1)
	
	Delay_ms(50);
	
	SW_CV_OUTPUT = 0;
	
	if((Wave_type==0) || (Wave_type==1))
	{
		TriggerExIN01_Init();
	}
	else
	{
		TriggerExIN23_Init();
	}
	WDG_Feed();

	log_info("Hardware init OK! Sys running......\r\n\r\n");
	
	

	
}



/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{	
	Init_Devices();
 
     
	/* Infinite loop */
	while (1)
	{

		Manual_Poll();
		Memory_Poll();
		UI_Poll();
		Led_ShortOn_Poll();
		WDG_Feed();
		Delay_ms(1);
	}
}


#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
