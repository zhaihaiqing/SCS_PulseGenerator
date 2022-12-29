
#include  "main.h"

/********************************************************************
*	����	�������ѹ/����
*	��ϵ	��
				��ѹ-----����0-4V �����0-100V
				����-----����0-4V�����0-32mA
*	��ע	��	
				ampl��λ��uV/nA,�����1Vʱ������amplֵΪ1000000
******************************************************************************/
void Output_VorC(uint8_t select_vc, int64_t ampl, uint8_t IsEnable)
{	
	if(IsEnable == OUTPUT_ENABLE)
	{
		if(select_vc == SELECT_VC_V)
		{			
			if(UserOperation.bPhase == UO_PHASE_UNIPHASE)						//������		
			{
				//log_info("OUTPUT_ENABLE  Voltage amp=%lld\r\n",ampl);
				AD5542_Output(DA_CHNL_VOLT, ampl / 25 / (VALUE_EXPAND_105 / 1000));					//(value/VALUE_EXPAND_105--V)*1000--DAC REGISTER����������˳�򣬷�������������������
			}
			else
			{
				AD5542_Output(DA_CHNL_VOLT, 0 - ampl / 25 / (VALUE_EXPAND_105 / 1000));	//���������
			}
		}
		else if(select_vc == SELECT_VC_C)
		{		
			if(UserOperation.bPhase == UO_PHASE_UNIPHASE)					
			{
				AD5542_Output(DA_CHNL_CURR, (ampl / (VALUE_EXPAND_105 / 1000)) / 8);					//((value/1000000--mA)/5--V)*1000--DAC REGISTER	���������
			}
			else
			{
				AD5542_Output(DA_CHNL_CURR, 0 - (ampl / (VALUE_EXPAND_105 / 1000)) / 8); //���������
			}		
		}
		
//		if(ampl > 0)
//		{
//			pTRIGGER_OUT = 0;
//			pLEDOUTPUT = LED_DIRECTLY_ON;
//			//if(BEEPCTRL == BEEPCTRL_EN)
//			if(1 == BEEPCTRL_EN)
//			{
//				BEEP = BEEP_ON;
//			}
//			else
//			{
//				BEEP = BEEP_OFF;
//			}
//		}
//		else
//		{
//			pTRIGGER_OUT = 1;
//			pLEDOUTPUT = LED_DIRECTLY_OFF;
//			BEEP = BEEP_OFF;
//		}
	}
	else if(IsEnable == OUTPUT_DISABLE)
	{
		if(select_vc == SELECT_VC_V)
		{
			AD5542_Output(DA_CHNL_VOLT, 0);
		}
		else if(select_vc == SELECT_VC_C)
		{
			AD5542_Output(DA_CHNL_CURR, 0);
		}		
		
		
		//pTRIGGER_OUT = 1;
		pLEDOUTPUT = LED_DIRECTLY_OFF;
		BEEP = BEEP_OFF;
	}
}



//__IO unsigned char EXIT2_Egde_Count=0;
//void EXTI2_IRQHandler(void)
//{
//	if(EXTI_GetITStatus(EXTI_Line2) != RESET) 	//��⵽PB2�ж�
//	{
//		
//		EXIT2_Egde_Count++;	//��¼���ظ���
//				
//		if(Wave_Type == 0)	//�����Բ���
//		{
//			if(EXIT2_Egde_Count == 1)	//��һ������
//			{
//				//log_info("1\r\n");
//				//GPIOG->ODR ^= GPIO_Pin_10;
//				Output_VorC(UserOperation.bVC, pPwmArrayParam[DO_TIM4]->Ampl, OUTPUT_ENABLE);
//			}
//			else 	//�ڶ�������
//			{
//				//log_info("2\r\n");
//				//GPIOG->ODR ^= GPIO_Pin_10;
//				EXIT2_Egde_Count=0;
//				Output_VorC(UserOperation.bVC, 0, OUTPUT_ENABLE);
//			}
//		}
//		else if(Wave_Type == 1)	//�����Բ���
//		{
//			if(EXIT2_Egde_Count == 1)	//
//			{
//				//log_info("3\r\n");
//				Output_VorC(UserOperation.bVC, (int64_t)(0-(int64_t)pPwmArrayParam[DO_TIM4]->Ampl), OUTPUT_ENABLE);
//			}
//			else 	//�½���
//			{
//				//log_info("4\r\n");
//				EXIT2_Egde_Count=0;
//				Output_VorC(UserOperation.bVC, 0, OUTPUT_ENABLE);
//			}
//		}
//		else if(Wave_Type == 2)							//˫���Բ���,������
//		{			
//			if(EXIT2_Egde_Count == 1)
//			{
//				//log_info("5\r\n");
//				Output_VorC(UserOperation.bVC, pPwmArrayParam[DO_TIM4]->Ampl, OUTPUT_ENABLE);
//			}
//			else if(EXIT2_Egde_Count == 2)
//			{
//				//log_info("6\r\n");
//				Output_VorC(UserOperation.bVC, (int64_t)(0-(int64_t)pPwmArrayParam[DO_TIM4]->Ampl), OUTPUT_ENABLE);
//			}
//			else 
//			{
//				//log_info("7\r\n");
//				EXIT2_Egde_Count=0;
//				Output_VorC(UserOperation.bVC, 0, OUTPUT_ENABLE);
//				
//			}			
//		}
//		else				//˫���Բ���,�ȸ�����
//		{
//			if(EXIT2_Egde_Count == 1)
//			{
//				//log_info("8\r\n");
//				Output_VorC(UserOperation.bVC, (int64_t)(0-(int64_t)pPwmArrayParam[DO_TIM4]->Ampl), OUTPUT_ENABLE);
//			}
//			else if(EXIT2_Egde_Count == 2)
//			{
//				//log_info("9\r\n");
//				Output_VorC(UserOperation.bVC, pPwmArrayParam[DO_TIM4]->Ampl, OUTPUT_ENABLE);
//			}
//			else 
//			{
//				//log_info("10\r\n");
//				EXIT2_Egde_Count=0;
//				Output_VorC(UserOperation.bVC, 0, OUTPUT_ENABLE);
//			}
//		}		
//		EXTI_ClearITPendingBit(EXTI_Line2);
//	}
//}



///********************************************************************
//*	����	��	��ʼ��
//******************************************************************************/
//void EXIT2_Init(void)
//{
//	GPIO_InitTypeDef  GPIO_InitStructure;
//	NVIC_InitTypeDef   NVIC_InitStructure;
//	EXTI_InitTypeDef   EXTI_InitStructure;

//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//ʹ��GPIOBʱ��
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//ʹ��SYSCFGʱ��
//	

//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��ͨ���ģʽ
//	//GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
//	//GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//����
//	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��
//	
//	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource2);//PB2 ���ӵ��ж���2
//	
//	 /* ����EXTI_Line2 */
//	EXTI_InitStructure.EXTI_Line = EXTI_Line2;//LINE2
//	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//�ж��¼�
//	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling; //�����ش��� 
//	EXTI_InitStructure.EXTI_LineCmd = ENABLE;//ʹ��LINE0
//	EXTI_Init(&EXTI_InitStructure);//����
//  
//	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;//�ⲿ�ж�4
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;//��ռ���ȼ�3
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;//�����ȼ�3
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//ʹ���ⲿ�ж�ͨ��
//	NVIC_Init(&NVIC_InitStructure);//����
//	
//	EXTI_ClearITPendingBit(EXTI_Line2);//����жϱ�־λ
//}
















































/********************************************************************
*	����	��	���Ժ���
******************************************************************************/
#if(TEST_OUTPUT)
	void test_OUTPUT(uint8_t mode)
	{
		static uint8_t testconf = 0;																//���Ժ�������һ�β�����Ȼ�����Ч��
		if(testconf == 0)
		{
//			testconf = 1;
			if(mode == TEST_VOLT_L)
			{
				//��ѹ����			
//				pSELECT_VC = SELECT_VC_V;
//				pSELECT_VHL = SELECT_VHL_L;
//				pSELECT_FLIP = SELECT_FLIP_UN;
				
				Switch.ModeCur = 0;
				Switch.ModeCur = (SELECT_VC_V << MODE_BIT_VC) | (SELECT_FLIP_UN << MODE_BIT_FLIP) | (SELECT_VHL_L<< MODE_BIT_VHL);
				
				UserOperation.bVC	= SELECT_VC_V;
				UserOperation.fMode	= UO_MODE_FREERUN;
				UserOperation.V_ModeFreeRun.Param[UO_PARAM_PULSE]	= 1000 * VALUE_EXPAND_105;		//1000ms
				
				/**	AMPL VALUE TEST	--	BEGIN										*/
//				UserOperation.V_ModeFreeRun.Ampl	= 5 * VALUE_EXPAND_105 / 10;	//0.5V
//				UserOperation.V_ModeFreeRun.Ampl	= 3 * VALUE_EXPAND_105 / 10;
//				UserOperation.V_ModeFreeRun.Ampl	= 3 * VALUE_EXPAND_105;			//3V
				UserOperation.V_ModeFreeRun.Ampl	= 5 * VALUE_EXPAND_105;			//5V
//				UserOperation.V_ModeFreeRun.Ampl	= 8 * VALUE_EXPAND_105;			//8V
				/**	AMPL VALUE TEST	--	END											*/
				
				UserOperation.V_ModeFreeRun.Param[UO_PARAM_FREQ]	= 1 * VALUE_EXPAND_102;			//1Hz
			}
			else if(mode == TEST_VOLT_H)
			{
				//��ѹ����
//				pSELECT_VC = SELECT_VC_V;
//				pSELECT_VHL = SELECT_VHL_H;
//				pSELECT_FLIP = SELECT_FLIP_UN;
				
				Switch.ModeCur = 0;
				Switch.ModeCur = (SELECT_VC_V << MODE_BIT_VC) | (SELECT_FLIP_UN << MODE_BIT_FLIP) | (SELECT_VHL_H<< MODE_BIT_VHL);
				
				UserOperation.bVC	= SELECT_VC_V;
				UserOperation.fMode	= UO_MODE_FREERUN;
				UserOperation.V_ModeFreeRun.Param[UO_PARAM_PULSE]	= 1000 * VALUE_EXPAND_105;		//1000ms
				
				/**	AMPL VALUE TEST	--	BEGIN										*/
//				UserOperation.V_ModeFreeRun.Ampl	=  7 * VALUE_EXPAND_105;
//				UserOperation.V_ModeFreeRun.Ampl	= 12 * VALUE_EXPAND_105;		//12V
				UserOperation.V_ModeFreeRun.Ampl	= 18 * VALUE_EXPAND_105;		//18V
//				UserOperation.V_ModeFreeRun.Ampl	= 25 * VALUE_EXPAND_105;		//25V
//				UserOperation.V_ModeFreeRun.Ampl	= 30 * VALUE_EXPAND_105;		//30V
//				UserOperation.V_ModeFreeRun.Ampl	= 50 * VALUE_EXPAND_105;		//50V
//				UserOperation.V_ModeFreeRun.Ampl	= 75 * VALUE_EXPAND_105;		//75V
//				UserOperation.V_ModeFreeRun.Ampl	= 90 * VALUE_EXPAND_105;		//90V
				/**	AMPL VALUE TEST	--	END											*/
				
				UserOperation.V_ModeFreeRun.Param[UO_PARAM_FREQ]	= 1 * VALUE_EXPAND_102;			//1Hz
			}
			else if(mode == TEST_CURR)
			{
				//��������
//				pSELECT_VC = SELECT_VC_C;
//				pSELECT_VHL = SELECT_VHL_L;
//				pSELECT_FLIP = SELECT_FLIP_UN;
				
				Switch.ModeCur = 0;
				Switch.ModeCur = (SELECT_VC_C << MODE_BIT_VC) | (SELECT_FLIP_UN << MODE_BIT_FLIP) | (SELECT_VHL_L<< MODE_BIT_VHL);
				
				UserOperation.bVC	= SELECT_VC_C;
				UserOperation.fMode	= UO_MODE_FREERUN;
				UserOperation.C_ModeFreeRun.Param[UO_PARAM_PULSE]	= 1000 * VALUE_EXPAND_105;		//1000ms
//				UserOperation.C_ModeFreeRun.Param[UO_PARAM_PULSE]	= 2 * VALUE_EXPAND_105;			//2ms / 1000ms
				
				/**	AMPL VALUE TEST	--	BEGIN										*/		//current(ma)		volt-input XTR111
//--------------------------------------------------------uA----------------------------------------------------------------------//
//				UserOperation.C_ModeFreeRun.Ampl	=   5 * VALUE_EXPAND_102;			//uA--[0.021,0.024]		--2.7mV  3.1mv				
//				UserOperation.C_ModeFreeRun.Ampl	=   6 * VALUE_EXPAND_102;			//uA--[0.023,0.024]		--3.1mv
//				UserOperation.C_ModeFreeRun.Ampl	=   7 * VALUE_EXPAND_102;			//uA--[0.019,0.019]		--2.7mv
//				UserOperation.C_ModeFreeRun.Ampl	=   8 * VALUE_EXPAND_102;			//uA--[0.024,0.024]		--3.1
//				UserOperation.C_ModeFreeRun.Ampl	=   9 * VALUE_EXPAND_102;			//uA--[0.024,0.024]		--3.1
//				UserOperation.C_ModeFreeRun.Ampl	=  10 * VALUE_EXPAND_102;			//uA--[0.025,0.026]0.028--4.1
//				UserOperation.C_ModeFreeRun.Ampl	=  11 * VALUE_EXPAND_102;			//uA--[0.028,0.029]		--4.1
//				UserOperation.C_ModeFreeRun.Ampl	=  12 * VALUE_EXPAND_102;			//uA--[0.029,0.029]		--4.1
//				UserOperation.C_ModeFreeRun.Ampl	=  13 * VALUE_EXPAND_102;			//uA--[0.027,0.028]		--4.1
//				UserOperation.C_ModeFreeRun.Ampl	=  14 * VALUE_EXPAND_102;			//uA--[0.028,0.028]		--4.1
//				UserOperation.C_ModeFreeRun.Ampl	=  15 * VALUE_EXPAND_102;			//uA--[0.032,0.032]		--5.0
//				UserOperation.C_ModeFreeRun.Ampl	=  16 * VALUE_EXPAND_102;			//uA--[0.032,0.032]		--5.0
//				UserOperation.C_ModeFreeRun.Ampl	=  17 * VALUE_EXPAND_102;			//uA--[0.032,0.032]		--5.0
//				UserOperation.C_ModeFreeRun.Ampl	=  18 * VALUE_EXPAND_102;			//uA--[0.032,0.032]		--5.0
//				UserOperation.C_ModeFreeRun.Ampl	=  19 * VALUE_EXPAND_102;			//uA--[0.032,0.032]		--5.0
//				UserOperation.C_ModeFreeRun.Ampl	=  20 * VALUE_EXPAND_102;			//uA--[0.035,0.036]		--6.0
//				UserOperation.C_ModeFreeRun.Ampl	=  30 * VALUE_EXPAND_102;			//uA--[0.043,0.042]0.046--8.0
//				UserOperation.C_ModeFreeRun.Ampl	=  40 * VALUE_EXPAND_102;			//uA--[0.053,0.054]		--9.9
//				UserOperation.C_ModeFreeRun.Ampl	=  50 * VALUE_EXPAND_102;			//uA--[0.063,0.064]		--11.9
//				UserOperation.C_ModeFreeRun.Ampl	=  80 * VALUE_EXPAND_102;			//uA--[0.091,0.092]		--17.8
//				UserOperation.C_ModeFreeRun.Ampl	= 100 * VALUE_EXPAND_102;			//uA--[0.108,0.109]		--21.9
//				UserOperation.C_ModeFreeRun.Ampl	= 150 * VALUE_EXPAND_102;			//uA--[0.157,0.158]0.160--31.8
//				UserOperation.C_ModeFreeRun.Ampl	= 200 * VALUE_EXPAND_102;			//uA--[0.209,0.210]0.212--41.9
//--------------------------------------------------------mA----------------------------------------------------------------------//
//				UserOperation.C_ModeFreeRun.Ampl	=   1 * VALUE_EXPAND_105;			//mA--[1.008,1.009]1.013--202mV
//				UserOperation.C_ModeFreeRun.Ampl	=   3 * VALUE_EXPAND_105;
				UserOperation.C_ModeFreeRun.Ampl	=   5 * VALUE_EXPAND_105;			//mA--[4.98X,4.99X]		--1002mV
//				UserOperation.C_ModeFreeRun.Ampl	=  15 * VALUE_EXPAND_105;
//				UserOperation.C_ModeFreeRun.Ampl	=  25 * VALUE_EXPAND_105;
				/**	AMPL VALUE TEST	--	END											*/
				
				UserOperation.C_ModeFreeRun.Param[UO_PARAM_FREQ]	= 1 * VALUE_EXPAND_102;			//1Hz
			}
			UserOperation.fRun = UO_FRUN_START;
		}
	}
#endif
