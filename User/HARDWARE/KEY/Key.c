
#include  "main.h"

KEY_T s_tBtn[KEY_NUMBER] = {0};
static KEY_FIFO_T s_tKey;

/********************************************************************
*  ����	  ��������ʼ��
*  ˵��   ���������
******************************************************************************/
void Btn_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	bsp_InitKeyVar();
	
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);//ʹ��GPIOCʱ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��ͨ���ģʽ
	//GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	//GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��
	
	
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��
	
}

/********************************************************************
*  ����   �������Ƿ���
*  ����   ��ʹ���Ա��ɹ�8*8���󰴼�FUN 1905�����ӷ�ʽ
			PC0--VER7		PC6--HOR0
			PC1--VER6		PC7--HOR1
			PC2--VER5		PC8--HOR2
			PC3--VER4		PC9--HOR3
			PC4--VER3		PC10-HOR4
			PC5--VER2		PC11-HOR5
*  ˵��   ���������
******************************************************************************/
static uint8_t IsKeyDown(uint8_t number)
{
	if(number < 6)
	{
		pBTN_C0 = 0;
		pBTN_C1 = 1;
		pBTN_C2 = 1;
		pBTN_C3 = 1;
		pBTN_C4 = 1;
		pBTN_C5 = 1;
		
		if(pBTN_L0 == 0)
		{
			return 0;
		}
		else if(pBTN_L1 == 0)
		{
			return 1;
		}
		else if(pBTN_L2 == 0)
		{
			return 2;
		}
		else if(pBTN_L3 == 0)
		{
			return 3;
		}
		else if(pBTN_L4 == 0)
		{
			return 4;
		}
		else if(pBTN_L5 == 0)
		{
			return 5;
		}
	}
	else if(number < 12)
	{
		pBTN_C0 = 1;
		pBTN_C1 = 0;
		pBTN_C2 = 1;
		pBTN_C3 = 1;
		pBTN_C4 = 1;
		pBTN_C5 = 1;
		
		if(pBTN_L0 == 0)
		{
			return 6;
		}
		else if(pBTN_L1 == 0)
		{
			return 7;
		}
		else if(pBTN_L2 == 0)
		{
			return 8;
		}
		else if(pBTN_L3 == 0)
		{
			return 9;
		}
		else if(pBTN_L4 == 0)
		{
			return 10;
		}
		else if(pBTN_L5 == 0)
		{
			return 11;
		}
	}
	else if(number < 18)
	{
		pBTN_C0 = 1;
		pBTN_C1 = 1;
		pBTN_C2 = 0;
		pBTN_C3 = 1;
		pBTN_C4 = 1;
		pBTN_C5 = 1;
		
		if(pBTN_L0 == 0)
		{
			return 12;
		}
		else if(pBTN_L1 == 0)
		{
			return 13;
		}
		else if(pBTN_L2 == 0)
		{
			return 14;
		}
		else if(pBTN_L3 == 0)
		{
			return 15;
		}
		else if(pBTN_L4 == 0)
		{
			return 16;
		}
		else if(pBTN_L5 == 0)
		{
			return 17;
		}
	}
	else if(number < 24)
	{
		pBTN_C0 = 1;
		pBTN_C1 = 1;
		pBTN_C2 = 1;
		pBTN_C3 = 0;
		pBTN_C4 = 1;
		pBTN_C5 = 1;
		
		if(pBTN_L0 == 0)
		{
			return 18;
		}
		else if(pBTN_L1 == 0)
		{
			return 19;
		}
		else if(pBTN_L2 == 0)
		{
			return 20;
		}
		else if(pBTN_L3 == 0)
		{
			return 21;
		}
		else if(pBTN_L4 == 0)
		{
			return 22;
		}
		else if(pBTN_L5 == 0)
		{
			return 23;
		}
	}
	else if(number < 30)
	{
		pBTN_C0 = 1;
		pBTN_C1 = 1;
		pBTN_C2 = 1;
		pBTN_C3 = 1;
		pBTN_C4 = 0;
		pBTN_C5 = 1;
		
		if(pBTN_L0 == 0)
		{
			return 24;
		}
		else if(pBTN_L1 == 0)
		{
			return 25;
		}
		else if(pBTN_L2 == 0)
		{
			return 26;
		}
		else if(pBTN_L3 == 0)
		{
			return 27;
		}
		else if(pBTN_L4 == 0)
		{
			return 28;
		}
		else if(pBTN_L5 == 0)
		{
			return 29;
		}
	}
	else if(number < 36)
	{
		pBTN_C0 = 1;
		pBTN_C1 = 1;
		pBTN_C2 = 1;
		pBTN_C3 = 1;
		pBTN_C4 = 1;
		pBTN_C5 = 0;
		
		if(pBTN_L0 == 0)
		{
			return 30;
		}
		else if(pBTN_L1 == 0)
		{
			return 31;
		}
		else if(pBTN_L2 == 0)
		{
			return 32;
		}
		else if(pBTN_L3 == 0)
		{
			return 33;
		}
		else if(pBTN_L4 == 0)
		{
			return 34;
		}
		else if(pBTN_L5 == 0)
		{
			return 35;
		}
	}
	
	return 0xff;
}


/********************************************************************
*  ����   ����ʼ����������
*  ˵��   ��ģ���ܣ����������󲻱ظĶ�
******************************************************************************/
void bsp_InitKeyVar(void)
{
	uint8_t i = 0;
	/* �԰���FIFO��дָ������ */
	s_tKey.Read = 0;
	s_tKey.Write = 0;
	s_tKey.Read2 = 0;
	
	for(i=0; i<KEY_NUMBER; i++)
	{
		s_tBtn[i].LongTime 	 = KEY_LONG_TIME;			/* ����ʱ�� 0 ��ʾ����ⳤ�����¼� */
		s_tBtn[i].Count      = KEY_FILTER_TIME;			/* ����������Ϊ�˲�ʱ���һ�� */
		s_tBtn[i].State       = 0;						/* ����ȱʡ״̬��0Ϊδ���� */
		s_tBtn[i].RepeatSpeed = 0;						/* �����������ٶȣ�0��ʾ��֧������ */
		s_tBtn[i].RepeatCount = 0;						/* ���������� */
	}
	
	/* �����Ҫ��������ĳ�������Ĳ����������ڴ˵������¸�ֵ */
	/* ���磬����ϣ������1���³���1����Զ��ط���ͬ��ֵ */
//	s_tBtn[KID_K0].LongTime = 100;
//	s_tBtn[KID_K0].RepeatSpeed = 5;	/* ÿ��50ms�Զ����ͼ�ֵ */

//	s_tBtn[KID_K1].LongTime = 100;
//	s_tBtn[KID_K1].RepeatSpeed = 5;	/* ÿ��50ms�Զ����ͼ�ֵ */

//	s_tBtn[KID_K2].LongTime = 100;
//	s_tBtn[KID_K2].RepeatSpeed = 5;	/* ÿ��50ms�Զ����ͼ�ֵ */
//	
//	s_tBtn[KID_K3].LongTime = 100;
//	s_tBtn[KID_K3].RepeatSpeed = 5;	/* ÿ��50ms�Զ����ͼ�ֵ */
//	
//	s_tBtn[KID_K4].LongTime = 100;
//	s_tBtn[KID_K4].RepeatSpeed = 5;	/* ÿ��50ms�Զ����ͼ�ֵ */
//	
//	s_tBtn[KID_K5].LongTime = 100;
//	s_tBtn[KID_K5].RepeatSpeed = 5;	/* ÿ��50ms�Զ����ͼ�ֵ */
//	
//	s_tBtn[KID_K6].LongTime = 100;
//	s_tBtn[KID_K6].RepeatSpeed = 5;	/* ÿ��50ms�Զ����ͼ�ֵ */
//	
//	s_tBtn[KID_K7].LongTime = 100;
//	s_tBtn[KID_K7].RepeatSpeed = 5;	/* ÿ��50ms�Զ����ͼ�ֵ */	
}

/********************************************************************
*	�� �� ��: bsp_PutKey
*	����˵��: ��1����ֵѹ�밴��FIFO��������������ģ��һ��������
*	��    ��: _KeyCode : ��������
*	˵	  ����ģ���ܣ����������󲻱ظĶ�
********************************************************************************************/
void bsp_PutKey(uint8_t _KeyCode)
{
	s_tKey.Buf[s_tKey.Write] = _KeyCode;

	if (++s_tKey.Write  >= KEY_FIFO_SIZE)
	{
		s_tKey.Write = 0;
	}
}

/**********************************************************************
*	�� �� ��: bsp_GetKey
*	����˵��: �Ӱ���FIFO��������ȡһ����ֵ��ʹ�õ�1����ָ�롣
*	�� �� ֵ: ��������
*	˵    ����ģ���ܣ����������󲻱ظĶ�
***********************************************************************************************/
uint8_t bsp_GetKey(void)
{
	uint8_t ret;

	if (s_tKey.Read == s_tKey.Write)
	{
		return KEY_NONE;
	}
	else
	{
		ret = s_tKey.Buf[s_tKey.Read];

		if (++s_tKey.Read >= KEY_FIFO_SIZE)
		{
			s_tKey.Read = 0;
		}
		return ret;
	}
}

/****************************************************************************
*	�� �� ��: bsp_GetKey2
*	����˵��: �Ӱ���FIFO��������ȡһ����ֵ��ʹ�õ�2����ָ�롣
*	�� �� ֵ: ��������
*	˵    ����ģ���ܣ����������󲻱ظĶ�
****************************************************************************************************/
uint8_t bsp_GetKey2(void)
{
	uint8_t ret;

	if (s_tKey.Read2 == s_tKey.Write)
	{
		return KEY_NONE;
	}
	else
	{
		ret = s_tKey.Buf[s_tKey.Read2];

		if (++s_tKey.Read2 >= KEY_FIFO_SIZE)
		{
			s_tKey.Read2 = 0;
		}
		return ret;
	}
}

/*******************************************************************************
*	�� �� ��: bsp_GetKeyState
*	����˵��: ��ȡ������״̬
*	��    ��: _ucKeyID : ����ID����0��ʼ
*	�� �� ֵ: 1 ��ʾ���£� 0 ��ʾδ����
*	˵    ����ģ���ܣ����������󲻱ظĶ�
*******************************************************************************************************/
uint8_t bsp_GetKeyState(KEY_ID_E _ucKeyID)
{
	return s_tBtn[_ucKeyID].State;
}

/********************************************************************************
*	�� �� ��: bsp_SetKeyParam
*	����˵��: ���ð�������
*	��    ��: _ucKeyID : ����ID����0��ʼ
*			  _LongTime : �����¼�ʱ��, ��λΪ10ms
*			  _RepeatSpeed : �����ٶ�, ��λΪ10ms
*	˵    ����ģ���ܣ����������󲻱ظĶ�
********************************************************************************************************/
void bsp_SetKeyParam(uint8_t _ucKeyID, uint16_t _LongTime, uint8_t  _RepeatSpeed)
{
	s_tBtn[_ucKeyID].LongTime    = _LongTime;							//����ʱ�� 0 ��ʾ����ⳤ�����¼�
	s_tBtn[_ucKeyID].RepeatSpeed = _RepeatSpeed;						//�����������ٶȣ�0��ʾ��֧������
	s_tBtn[_ucKeyID].RepeatCount = 0;									//����������
}

/**********************************************************************************
*	�� �� ��: bsp_ClearKey
*	����˵��: ��հ���FIFO������
*	˵    ����ģ���ܣ����������󲻱ظĶ�
********************************************************************************************************/
void bsp_ClearKey(void)
{
	s_tKey.Read = s_tKey.Write;
}

/*************************************************************************************
*	�� �� ��: bsp_DetectKey
*	����˵��: ���һ��������������״̬�����뱻�����Եĵ��á�
*	˵    �����򵥶԰������º������ж�Ӧ����
********************************************************************************************************/
static void bsp_DetectKey(uint8_t i)
{
	KEY_T *pBtn = &s_tBtn[i];
	
//	if (IsKeyDown(i) != 0xff)											//�������ڵ����������󰴼�ʱ������Ϊ���Ű���[V57]
	if(IsKeyDown(i) == i)												//�а�������
	{
		if (pBtn->Count < KEY_FILTER_TIME)								//�״μ�⵽���£�����ֵ����ʼ����
		{
			pBtn->Count = KEY_FILTER_TIME;
		}
		else if(pBtn->Count < 2 * KEY_FILTER_TIME)						//����������.....
		{
			pBtn->Count++;
		}
		else
		{
			if (pBtn->State == 0)
			{
				pBtn->State = 1;										//������ǰ״̬�����»��ǵ���
				bsp_PutKey((uint8_t)(3 * i + 1));						//���Ͱ�ť���µ���Ϣ
			}

			if (pBtn->LongTime > 0)										//֧�ֳ���
			{
				if (pBtn->LongCount < pBtn->LongTime)
				{
					if (++pBtn->LongCount == pBtn->LongTime)			//���Ͱ�ť�������µ���Ϣ
					{
						bsp_PutKey((uint8_t)(3 * i + 3));				//��ֵ���밴��FIFO
					}
				}
				else
				{
					if (pBtn->RepeatSpeed > 0)							//֧���ظ�д��FIFO
					{
						if (++pBtn->RepeatCount >= pBtn->RepeatSpeed)
						{
							pBtn->RepeatCount = 0;
							bsp_PutKey((uint8_t)(3 * i + 1));			//��������ÿ��10ms����1������
						}
					}
				}
			}
		}
	}
	else
	{
		if(pBtn->Count > KEY_FILTER_TIME)
		{
			pBtn->Count = KEY_FILTER_TIME;
		}
		else if(pBtn->Count != 0)
		{
			pBtn->Count--;
		}
		else
		{
			if (pBtn->State == 1)
			{
				pBtn->State = 0;
				bsp_PutKey((uint8_t)(3 * i + 2));						//���Ͱ�ť�������Ϣ
			}
		}

		pBtn->LongCount = 0;
		pBtn->RepeatCount = 0;
	}
}

/*************************************************************************************
*	�� �� ��: bsp_KeyScan
*	����˵��: ɨ�����а����������������ж������Եĵ���
*	˵    ����ģ���ܣ����������󲻱ظĶ�
********************************************************************************************************/
void bsp_KeyScan(void)
{
	uint8_t i;

	for (i = 0; i < KEY_NUMBER; i++)
	{
		bsp_DetectKey(i);
	}
}
