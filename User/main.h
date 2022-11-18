/**
  ******************************************************************************
  * @file    Project/STM32F4xx_StdPeriph_Templates/main.h 
  * @author  MCD Application Team
  * @version V1.7.0
  * @date    22-April-2016
  * @brief   Header for main.c module
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
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include <stdint.h>
#include "stdio.h"
#include "math.h"
#include "string.h"

#include "sys.h"
#include "delay.h"
#include "debug_uart.h"




#include "Timer6.h"
#include "Led.h"
#include "Beep.h"
#include "Key.h"
#include "DAC.h"
#include "Codec.h"
#include "Switch.h"
#include "Trigger.h"
#include "Manual.h"
#include "AT24C64IIC.h"
#include "AT24C64.h"
#include "AT24C64MemData.h"
#include "Pulse.h"
#include "Usart1.h"
#include "Malloc.h"
#include "Crc.h"
#include "Protocol.h"
#include "PulseTran.h"
#include "SSD1961.h"
#include "UI.h"
#include "Output.h"
#include "Timer2.h"
#include "Wdg.h"



/*
*��ѹ����������������ѹ�������汾��80V��100V
*/


#define PCB_V14			14			//LZG_WFC,���С����˴�����
#define PCB_V15			15
#define PCB_V16			16			//��Ľ������ֵ�汾
#define PCB_VERSION		PCB_V15

#define LZG_WFC			0xff

#define TEST_LED		1
#define TEST_AT24CXX	0
#define TEST_LCD		0
#define TEST_OUTPUT		0
#define TEST_KEYBOARD	0
#define TEST_TEMP		1

//��ȡID������A--ֱ�Ӹ�ֵ�����������ɹ�		B--��ȡ�Ĵ���������ʧ�ܣ������
#define ID_PLAN_A		0
#define ID_PLAN_B		1
#define LCD_ID_PLAN		ID_PLAN_A




#define	VOLTAGE_REF_VALUE	4.096	//��׼ֵΪ4.096V
#define DEBUG_UART_RATE     115200	//������Դ��ڲ�����=115200bps


#define LED1_ON()	GPIOE->BSRRH = GPIO_Pin_6
#define LED1_OFF()	GPIOE->BSRRL = GPIO_Pin_6

#define APOW_ON()	GPIOF->BSRRL = GPIO_Pin_3
#define APOW_OFF()	GPIOF->BSRRH = GPIO_Pin_3

#define LCD_RST_H()	GPIOE->BSRRL = GPIO_Pin_5
#define LCD_RST_L()	GPIOE->BSRRH = GPIO_Pin_5


void Delay_ms(volatile unsigned int nTime);
void delay_us(uint32_t us);


/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */


extern volatile unsigned int SysTick_Count;   //Systick����
extern volatile unsigned int SysTick_Count1;   //Systick����
extern volatile unsigned int TimingDelay;     //��ʱ��������

extern volatile unsigned char Voltage_Ploar_Flag ; 		//��ѹ����ȫ�ֱ�����0���������ԣ�1��������
extern volatile unsigned char Current_Ploar_Flag ; 		//��������ȫ�ֱ�����0���������ԣ�1��������



#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
