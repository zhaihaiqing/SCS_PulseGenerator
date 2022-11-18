
#include  "main.h"
#include "FONT.h"

_lcd_dev lcddev;

/********************************************************************
*	����	��	LCD�Ļ�����ɫ�ͱ���ɫ
******************************************************************************/
u16 BACK_COLOR=WHITE;  //���趨��ʾ�ַ����򱳾�ɫ 	0XFFFF
u16 POINT_COLOR=BLACK;	//������ɫ[�����ɫ] 		0X0000

/********************************************************************
*	����	��	д�Ĵ�������
*	����	��	regval--��д��ļĴ������
******************************************************************************/	 
void LCD_WR_REG(u16 regval)
{   
	LCD->LCD_REG=regval;
}
/********************************************************************
*	����	��	дLCD����
*	����	��	data--д��Ĵ�����ֵ
******************************************************************************/
void LCD_WR_DATA(u16 data)
{	 
	LCD->LCD_RAM=data;		 
}
/********************************************************************
*	����	��	��LCD����
******************************************************************************/
u16 LCD_RD_DATA(void)
{
	vu16 ram;			//��ֹ���Ż�
	ram=LCD->LCD_RAM;
	return ram;	 
}
/********************************************************************
*	����	��	д�Ĵ���
*	����	��	LCD_Reg--�Ĵ������
				LCD_RegValue--Ҫд������
******************************************************************************/
void LCD_WriteReg(u16 LCD_Reg,u16 LCD_RegValue)
{	
	LCD->LCD_REG = LCD_Reg;
	LCD->LCD_RAM = LCD_RegValue;
}
/********************************************************************
*	����	��	���Ĵ���
*	����	��	LCD_Reg--��ȡ�Ĵ������
******************************************************************************/
u16 LCD_ReadReg(u16 LCD_Reg)
{										   
	LCD_WR_REG(LCD_Reg);
	delay_us(5);		  
	return LCD_RD_DATA();
}
/********************************************************************
*	����	��	��ʼдGRAM
******************************************************************************/
void LCD_WriteRAM_Prepare(void)
{
 	LCD->LCD_REG=lcddev.wramcmd;	  
}
/********************************************************************
*	����	��	LCDдGRAM
*	����	��	RGB_Code--��ɫֵ
******************************************************************************/
void LCD_WriteRAM(u16 RGB_Code)
{							    
	LCD->LCD_RAM = RGB_Code;//дʮ��λGRAM
}
/********************************************************************
*	����	��	����������ΪGBR��ʽ����д���ʱ��ΪRGB��ʽ��ͨ���ú���ת��
*	����	��	c--GBR��ʽ��ɫֵ
******************************************************************************/
u16 LCD_BGR2RGB(u16 c)
{
	u16  r,g,b,rgb;   
	b=(c>>0)&0x1f;
	g=(c>>5)&0x3f;
	r=(c>>11)&0x1f;	 
	rgb=(b<<11)+(g<<5)+(r<<0);		 
	return(rgb);
}
/********************************************************************
*	����	��	��mdk -O1ʱ���Ż�ʱ��Ҫ���ã���ʱi
******************************************************************************/
void opt_delay(u8 i)
{
	while(i--);
}
/********************************************************************
*	����	��	��ȡ��ĳ�����ɫֵ
*	����	��	x, y--����
******************************************************************************/
u16 LCD_ReadPoint(u16 x,u16 y)
{
 	u16 r=0,g=0,b=0;
	if(x>=lcddev.width||y>=lcddev.height)return 0;	//�����˷�Χ,ֱ�ӷ���		   
	LCD_SetCursor(x,y);	    
	if(lcddev.id==0X9341||lcddev.id==0X6804||lcddev.id==0X5310||lcddev.id==0X1963)LCD_WR_REG(0X2E);//9341/6804/3510/1963 ���Ͷ�GRAMָ��
	else if(lcddev.id==0X5510)LCD_WR_REG(0X2E00);	//5510 ���Ͷ�GRAMָ��
	else LCD_WR_REG(0X22);      		 			//����IC���Ͷ�GRAMָ��
	if(lcddev.id==0X9320)opt_delay(2);				//FOR 9320,��ʱ2us	    
 	r=LCD_RD_DATA();								//dummy Read	   
	if(lcddev.id==0X1963)return r;					//1963ֱ�Ӷ��Ϳ��� 
	opt_delay(2);	  
 	r=LCD_RD_DATA();  		  						//ʵ��������ɫ
 	if(lcddev.id==0X9341||lcddev.id==0X5310||lcddev.id==0X5510)		//9341/NT35310/NT35510Ҫ��2�ζ���
 	{
		opt_delay(2);	  
		b=LCD_RD_DATA(); 
		g=r&0XFF;		//����9341/5310/5510,��һ�ζ�ȡ����RG��ֵ,R��ǰ,G�ں�,��ռ8λ
		g<<=8;
	} 
	if(lcddev.id==0X9325||lcddev.id==0X4535||lcddev.id==0X4531||lcddev.id==0XB505||lcddev.id==0XC505)return r;	//�⼸��ICֱ�ӷ�����ɫֵ
	else if(lcddev.id==0X9341||lcddev.id==0X5310||lcddev.id==0X5510)return (((r>>11)<<11)|((g>>10)<<5)|(b>>11));//ILI9341/NT35310/NT35510��Ҫ��ʽת��һ��
	else return LCD_BGR2RGB(r);						//����IC
}
/********************************************************************
*	����	��	LCD������ʾ
******************************************************************************/

void LCD_DisplayOn(void)
{					   
	if(lcddev.id==0X9341||lcddev.id==0X6804||lcddev.id==0X5310||lcddev.id==0X1963)LCD_WR_REG(0X29);	//������ʾ
	else if(lcddev.id==0X5510)LCD_WR_REG(0X2900);	//������ʾ
	else LCD_WriteReg(0X07,0x0173); 			 	//������ʾ
}
/********************************************************************
*	����	��	LCD�ر���ʾ
******************************************************************************/
void LCD_DisplayOff(void)
{	   
	if(lcddev.id==0X9341||lcddev.id==0X6804||lcddev.id==0X5310||lcddev.id==0X1963)LCD_WR_REG(0X28);	//�ر���ʾ
	else if(lcddev.id==0X5510)LCD_WR_REG(0X2800);	//�ر���ʾ
	else LCD_WriteReg(0X07,0x0);//�ر���ʾ 
}
/********************************************************************
*	����	��	���ù��λ��
*	����	��	Xpos, Ypos--����
******************************************************************************/
void LCD_SetCursor(u16 Xpos, u16 Ypos)
{	 
 	if(lcddev.id==0X1963)
	{  			 		
		if(lcddev.dir==0)//x������Ҫ�任
		{
			Xpos=lcddev.width-1-Xpos;
			LCD_WR_REG(lcddev.setxcmd); 
//			LCD_WR_DATA(0);LCD_WR_DATA(0); 		
			LCD_WR_DATA(Xpos>>8);LCD_WR_DATA(Xpos&0XFF);
			LCD_WR_DATA(Xpos>>8);LCD_WR_DATA(Xpos&0XFF);
		}else
		{
			LCD_WR_REG(lcddev.setxcmd); 
			LCD_WR_DATA(Xpos>>8);LCD_WR_DATA(Xpos&0XFF); 
//			LCD_WR_DATA(Xpos>>8);LCD_WR_DATA(Xpos&0XFF);
			LCD_WR_DATA((lcddev.width-1)>>8);LCD_WR_DATA((lcddev.width-1)&0XFF);		 	 			
		}	
		LCD_WR_REG(lcddev.setycmd);
		LCD_WR_DATA(Ypos>>8);LCD_WR_DATA(Ypos&0XFF);
//		LCD_WR_DATA(Ypos>>8);LCD_WR_DATA(Ypos&0XFF); 
		LCD_WR_DATA((lcddev.height-1)>>8);LCD_WR_DATA((lcddev.height-1)&0XFF);
	}
}
/********************************************************************
*	����	��	����LCD���Զ�ɨ�跽��
*	ע��	��	�����������ܻ��ܵ��˺������õ�Ӱ��(������9341/6804����������),
				����,һ������ΪL2R_U2D����,�������Ϊ����ɨ�跽ʽ,���ܵ�����ʾ������.
				9320/9325/9328/4531/4535/1505/b505/5408/9341/5310/5510/1963��IC�Ѿ�ʵ�ʲ���	
*	����	��	dir:0~7,����8������(���嶨���lcd.h)
******************************************************************************/
void LCD_Scan_Dir(u8 dir)
{
	u16 regval=0;
	u16 dirreg=0;
	u16 temp;  
	if((lcddev.dir==1&&lcddev.id!=0X6804&&lcddev.id!=0X1963)||(lcddev.dir==0&&lcddev.id==0X1963))//����ʱ����6804��1963���ı�ɨ�跽������ʱ1963�ı䷽��
	{			   
		switch(dir)//����ת��
		{
			case 0:dir=6;break;
			case 1:dir=7;break;
			case 2:dir=4;break;
			case 3:dir=5;break;
			case 4:dir=1;break;
			case 5:dir=0;break;
			case 6:dir=3;break;
			case 7:dir=2;break;	     
		}
	} 
	if(lcddev.id==0x9341||lcddev.id==0X6804||lcddev.id==0X5310||lcddev.id==0X5510||lcddev.id==0X1963)//9341/6804/5310/5510/1963,���⴦��
	{
		switch(dir)
		{
			case L2R_U2D://������,���ϵ���
				regval|=(0<<7)|(0<<6)|(0<<5); 
				break;
			case L2R_D2U://������,���µ���
				regval|=(1<<7)|(0<<6)|(0<<5); 
				break;
			case R2L_U2D://���ҵ���,���ϵ���
				regval|=(0<<7)|(1<<6)|(0<<5); 
				break;
			case R2L_D2U://���ҵ���,���µ���
				regval|=(1<<7)|(1<<6)|(0<<5); 
				break;	 
			case U2D_L2R://���ϵ���,������
				regval|=(0<<7)|(0<<6)|(1<<5); 
				break;
			case U2D_R2L://���ϵ���,���ҵ���
				regval|=(0<<7)|(1<<6)|(1<<5); 
				break;
			case D2U_L2R://���µ���,������
				regval|=(1<<7)|(0<<6)|(1<<5); 
				break;
			case D2U_R2L://���µ���,���ҵ���
				regval|=(1<<7)|(1<<6)|(1<<5); 
				break;	 
		}
		if(lcddev.id==0X5510)dirreg=0X3600;
		else dirreg=0X36;
 		if((lcddev.id!=0X5310)&&(lcddev.id!=0X5510)&&(lcddev.id!=0X1963))regval|=0X08;//5310/5510/1963����ҪBGR   
		if(lcddev.id==0X6804)regval|=0x02;//6804��BIT6��9341�ķ���	   
		LCD_WriteReg(dirreg,regval);
		if(lcddev.id!=0X1963)//1963�������괦��
		{
			if(regval&0X20)
			{
				if(lcddev.width<lcddev.height)//����X,Y
				{
					temp=lcddev.width;
					lcddev.width=lcddev.height;
					lcddev.height=temp;
				}
			}else  
			{
				if(lcddev.width>lcddev.height)//����X,Y
				{
					temp=lcddev.width;
					lcddev.width=lcddev.height;
					lcddev.height=temp;
				}
			}  
		}
		if(lcddev.id==0X5510)
		{
			LCD_WR_REG(lcddev.setxcmd);LCD_WR_DATA(0); 
			LCD_WR_REG(lcddev.setxcmd+1);LCD_WR_DATA(0); 
			LCD_WR_REG(lcddev.setxcmd+2);LCD_WR_DATA((lcddev.width-1)>>8); 
			LCD_WR_REG(lcddev.setxcmd+3);LCD_WR_DATA((lcddev.width-1)&0XFF); 
			LCD_WR_REG(lcddev.setycmd);LCD_WR_DATA(0); 
			LCD_WR_REG(lcddev.setycmd+1);LCD_WR_DATA(0); 
			LCD_WR_REG(lcddev.setycmd+2);LCD_WR_DATA((lcddev.height-1)>>8); 
			LCD_WR_REG(lcddev.setycmd+3);LCD_WR_DATA((lcddev.height-1)&0XFF);
		}else
		{
			LCD_WR_REG(lcddev.setxcmd); 
			LCD_WR_DATA(0);LCD_WR_DATA(0);
			LCD_WR_DATA((lcddev.width-1)>>8);LCD_WR_DATA((lcddev.width-1)&0XFF);
			LCD_WR_REG(lcddev.setycmd); 
			LCD_WR_DATA(0);LCD_WR_DATA(0);
			LCD_WR_DATA((lcddev.height-1)>>8);LCD_WR_DATA((lcddev.height-1)&0XFF);  
		}
  	}else 
	{
		switch(dir)
		{
			case L2R_U2D://������,���ϵ���
				regval|=(1<<5)|(1<<4)|(0<<3); 
				break;
			case L2R_D2U://������,���µ���
				regval|=(0<<5)|(1<<4)|(0<<3); 
				break;
			case R2L_U2D://���ҵ���,���ϵ���
				regval|=(1<<5)|(0<<4)|(0<<3);
				break;
			case R2L_D2U://���ҵ���,���µ���
				regval|=(0<<5)|(0<<4)|(0<<3); 
				break;	 
			case U2D_L2R://���ϵ���,������
				regval|=(1<<5)|(1<<4)|(1<<3); 
				break;
			case U2D_R2L://���ϵ���,���ҵ���
				regval|=(1<<5)|(0<<4)|(1<<3); 
				break;
			case D2U_L2R://���µ���,������
				regval|=(0<<5)|(1<<4)|(1<<3); 
				break;
			case D2U_R2L://���µ���,���ҵ���
				regval|=(0<<5)|(0<<4)|(1<<3); 
				break;	 
		} 
		dirreg=0X03;
		regval|=1<<12; 
		LCD_WriteReg(dirreg,regval);
	}
}
/********************************************************************
*	����	��	����
*	����	��	x,y--����
******************************************************************************/
void LCD_DrawPoint(u16 x,u16 y)
{
	LCD_SetCursor(x,y);		//���ù��λ�� 
	LCD_WriteRAM_Prepare();	//��ʼд��GRAM
//	LCD->LCD_RAM=POINT_COLOR; 
	LCD->LCD_RAM=COLOR_WHITE_G;
}
/********************************************************************
*	����	��	���ٻ���
*	����	��	x,y--����
				color--��ɫ
******************************************************************************/
void LCD_Fast_DrawPoint(u16 x,u16 y,u16 color)
{	   
	if(lcddev.id==0X1963)
	{
		if(lcddev.dir==0)x=lcddev.width-1-x;
		LCD_WR_REG(lcddev.setxcmd); 
		LCD_WR_DATA(x>>8);LCD_WR_DATA(x&0XFF);
		LCD_WR_DATA(x>>8);LCD_WR_DATA(x&0XFF);
		LCD_WR_REG(lcddev.setycmd); 
		LCD_WR_DATA(y>>8);LCD_WR_DATA(y&0XFF);
		LCD_WR_DATA(y>>8);LCD_WR_DATA(y&0XFF);
	}
	LCD->LCD_REG=lcddev.wramcmd;
	LCD->LCD_RAM=color;
}
/********************************************************************
*	����	��	SSD1963 ��������
*	����	��	pwm--����ȼ�,0~100.Խ��Խ��
******************************************************************************/
void LCD_SSD_BackLightSet(u8 pwm)
{
	LCD_WR_REG(0xBE);	//����PWM���
	LCD_WR_DATA(0x05);	//1����PWMƵ��
	LCD_WR_DATA(pwm*2.55);//2����PWMռ�ձ�
	LCD_WR_DATA(0x01);	//3����C
	LCD_WR_DATA(0xFF);	//4����D
	LCD_WR_DATA(0x00);	//5����E
	LCD_WR_DATA(0x00);	//6����F
}
/********************************************************************
*	����	��	����LCD��ʾ����
*	����	��	dir--0,������1,����
******************************************************************************/
void LCD_Display_Dir(u8 dir)
{
	if(dir==0)			//����
	{
		lcddev.dir=0;	//����
		lcddev.width=240;
		lcddev.height=320;
		if(lcddev.id==0X1963)
		{
			lcddev.wramcmd=0X2C;	//����д��GRAM��ָ�� 
			lcddev.setxcmd=0X2B;	//����дX����ָ��
			lcddev.setycmd=0X2A;	//����дY����ָ��
			lcddev.width=480;		//���ÿ��480
			lcddev.height=800;		//���ø߶�800  
		}
	}else 				//����
	{	  				
		lcddev.dir=1;	//����
		lcddev.width=320;
		lcddev.height=240;
		if(lcddev.id==0X1963)
		{
			lcddev.wramcmd=0X2C;	//����д��GRAM��ָ�� 
			lcddev.setxcmd=0X2A;	//����дX����ָ��
			lcddev.setycmd=0X2B;	//����дY����ָ��
			lcddev.width=480;		//���ÿ��800->480[V64]
			lcddev.height=272;		//���ø߶�480->272[V64]
		}
	} 
	LCD_Scan_Dir(DFT_SCAN_DIR);	//Ĭ��ɨ�跽��
}
/********************************************************************
*	����	��	���ô���,���Զ����û������굽�������Ͻ�(sx,sy)
*	����	��	sx,sy--������ʼ����(���Ͻ�)
				width,height--���ڿ�Ⱥ͸߶�,�������0!!
				�����С:width*height
******************************************************************************/
void LCD_Set_Window(u16 sx,u16 sy,u16 width,u16 height)
{    
	u8 hsareg,heareg,vsareg,veareg;
	u16 hsaval,heaval,vsaval,veaval; 
	u16 twidth,theight;
	twidth=sx+width-1;
	theight=sy+height-1;
	if(lcddev.id==0X9341||lcddev.id==0X5310||lcddev.id==0X6804||(lcddev.dir==1&&lcddev.id==0X1963))
	{
		LCD_WR_REG(lcddev.setxcmd); 
		LCD_WR_DATA(sx>>8); 
		LCD_WR_DATA(sx&0XFF);	 
		LCD_WR_DATA(twidth>>8); 
		LCD_WR_DATA(twidth&0XFF);  
		LCD_WR_REG(lcddev.setycmd); 
		LCD_WR_DATA(sy>>8); 
		LCD_WR_DATA(sy&0XFF); 
		LCD_WR_DATA(theight>>8); 
		LCD_WR_DATA(theight&0XFF); 
	}else if(lcddev.id==0X1963)//1963�������⴦��
	{
		sx=lcddev.width-width-sx; 
		height=sy+height-1; 
		LCD_WR_REG(lcddev.setxcmd); 
		LCD_WR_DATA(sx>>8); 
		LCD_WR_DATA(sx&0XFF);	 
		LCD_WR_DATA((sx+width-1)>>8); 
		LCD_WR_DATA((sx+width-1)&0XFF);  
		LCD_WR_REG(lcddev.setycmd); 
		LCD_WR_DATA(sy>>8); 
		LCD_WR_DATA(sy&0XFF); 
		LCD_WR_DATA(height>>8); 
		LCD_WR_DATA(height&0XFF); 		
	}else if(lcddev.id==0X5510)
	{
		LCD_WR_REG(lcddev.setxcmd);LCD_WR_DATA(sx>>8);  
		LCD_WR_REG(lcddev.setxcmd+1);LCD_WR_DATA(sx&0XFF);	  
		LCD_WR_REG(lcddev.setxcmd+2);LCD_WR_DATA(twidth>>8);   
		LCD_WR_REG(lcddev.setxcmd+3);LCD_WR_DATA(twidth&0XFF);   
		LCD_WR_REG(lcddev.setycmd);LCD_WR_DATA(sy>>8);   
		LCD_WR_REG(lcddev.setycmd+1);LCD_WR_DATA(sy&0XFF);  
		LCD_WR_REG(lcddev.setycmd+2);LCD_WR_DATA(theight>>8);   
		LCD_WR_REG(lcddev.setycmd+3);LCD_WR_DATA(theight&0XFF);  
	}else	//��������IC
	{
		if(lcddev.dir==1)//����
		{
			//����ֵ
			hsaval=sy;				
			heaval=theight;
			vsaval=lcddev.width-twidth-1;
			veaval=lcddev.width-sx-1;				
		}else
		{ 
			hsaval=sx;				
			heaval=twidth;
			vsaval=sy;
			veaval=theight;
		} 
		hsareg=0X50;heareg=0X51;//ˮƽ���򴰿ڼĴ���
		vsareg=0X52;veareg=0X53;//��ֱ���򴰿ڼĴ���	   							  
		//���üĴ���ֵ
		LCD_WriteReg(hsareg,hsaval);
		LCD_WriteReg(heareg,heaval);
		LCD_WriteReg(vsareg,vsaval);
		LCD_WriteReg(veareg,veaval);		
		LCD_SetCursor(sx,sy);	//���ù��λ��
	}
}
/********************************************************************
*	����	��	��������
*	����	��	color--Ҫ���������ɫ
******************************************************************************/
void LCD_Clear(u16 color)
{
	u32 index=0;      
	u32 totalpoint=lcddev.width;
	totalpoint*=lcddev.height; 			//�õ��ܵ���
	if((lcddev.id==0X6804)&&(lcddev.dir==1))//6804������ʱ�����⴦��
	{						    
 		lcddev.dir=0;	 
 		lcddev.setxcmd=0X2A;
		lcddev.setycmd=0X2B;  	 			
		LCD_SetCursor(0x00,0x0000);		//���ù��λ��
 		lcddev.dir=1;	 
  		lcddev.setxcmd=0X2B;
		lcddev.setycmd=0X2A;  	 
 	}else LCD_SetCursor(0x00,0x0000);	//���ù��λ��
	LCD_WriteRAM_Prepare();     		//��ʼд��GRAM
	for(index=0;index<totalpoint;index++)
	{
		LCD->LCD_RAM=color;	
	}
}
/********************************************************************
*	����	��	��ָ����������䵥����ɫ
*	����	��	(sx,sy),(ex,ey)--�����ζԽ�����,�����СΪ:(ex-sx+1)*(ey-sy+1) 
				color--Ҫ������ɫ
******************************************************************************/
void LCD_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 color)
{          
	u16 i,j;
	u16 xlen=0;
	
	xlen=ex-sx+1;
	for(i=sy;i<=ey;i++)
	{
		LCD_SetCursor(sx,i);      				//���ù��λ�� 
		LCD_WriteRAM_Prepare();     			//��ʼд��GRAM	  
		for(j=0;j<xlen;j++)LCD->LCD_RAM=color;	//��ʾ��ɫ 	    
	}
}
/********************************************************************
*	����	��	��ָ�����������ָ����ɫ��
*	����	��	(sx,sy),(ex,ey)--�����ζԽ�����,�����СΪ:(ex-sx+1)*(ey-sy+1) 
				color--Ҫ������ɫ
******************************************************************************/
void LCD_Color_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 *color)
{  
	u16 height,width;
	u16 i,j;
	width=ex-sx+1; 			//�õ����Ŀ��
	height=ey-sy+1;			//�߶�
 	for(i=0;i<height;i++)
	{
 		LCD_SetCursor(sx,sy+i);   	//���ù��λ�� 
		LCD_WriteRAM_Prepare();     //��ʼд��GRAM
		for(j=0;j<width;j++)LCD->LCD_RAM=color[i*width+j];//д������ 
	}		  
}
/********************************************************************
*	����	��	����
*	����	��	x1,y1--�������
				x2,y2--�յ�����
******************************************************************************/
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 
	delta_x=x2-x1; //������������ 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //���õ������� 
	else if(delta_x==0)incx=0;//��ֱ�� 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//ˮƽ�� 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //ѡȡ�������������� 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//������� 
	{  
		LCD_DrawPoint(uRow,uCol);//���� 
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
}
/********************************************************************
*	����	��	������
*	����	��	(x1,y1),(x2,y2)--���εĶԽ�����
******************************************************************************/
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2)
{
	LCD_DrawLine(x1,y1,x2,y1);
	LCD_DrawLine(x1,y1,x1,y2);
	LCD_DrawLine(x1,y2,x2,y2);
	LCD_DrawLine(x2,y1,x2,y2);
}
/********************************************************************
*	����	��	��ָ��λ�û�һ��ָ����С��Բ
*	����	��	(x,y)--���ĵ�
				r--�뾶
******************************************************************************/
void LCD_Draw_Circle(u16 x0,u16 y0,u8 r)
{
	int a,b;
	int di;
	a=0;b=r;	  
	di=3-(r<<1);             //�ж��¸���λ�õı�־
	while(a<=b)
	{
		LCD_DrawPoint(x0+a,y0-b);             //5
 		LCD_DrawPoint(x0+b,y0-a);             //0           
		LCD_DrawPoint(x0+b,y0+a);             //4               
		LCD_DrawPoint(x0+a,y0+b);             //6 
		LCD_DrawPoint(x0-a,y0+b);             //1       
 		LCD_DrawPoint(x0-b,y0+a);             
		LCD_DrawPoint(x0-a,y0-b);             //2             
  		LCD_DrawPoint(x0-b,y0-a);             //7     	         
		a++;
		//ʹ��Bresenham�㷨��Բ     
		if(di<0)di +=4*a+6;	  
		else
		{
			di+=10+4*(a-b);   
			b--;
		} 						    
	}
}
/********************************************************************
*	����	��	��ָ��λ����ʾһ���ַ�
*	����	��	x,y--��ʼ����
				num--Ҫ��ʾ���ַ�:" "--->"~"
				size--�����С 12/16/24
				mode--���ӷ�ʽ(1)���Ƿǵ��ӷ�ʽ(0)
*	ע��	��	�ǵ��ӷ�ʽ�������ʾ���򱳾�ɫ������Ļ����ˮƽ�����д�״Ӱ��
******************************************************************************/
void LCD_ShowChar(u16 x,u16 y,u8 num,u8 size,u8 mode)
{  							  
    u8 temp,t1,t;
	u16 y0=y;
	u8 csize=(size/8+((size%8)?1:0))*(size/2);		//�õ�����һ���ַ���Ӧ������ռ���ֽ���	
 	num=num-' ';//�õ�ƫ�ƺ��ֵ��ASCII�ֿ��Ǵӿո�ʼȡģ������-' '���Ƕ�Ӧ�ַ����ֿ⣩
	for(t=0;t<csize;t++)
	{   
		if(size==12)temp=asc2_1206[num][t]; 	 	//����1206����
		else if(size==16)temp=asc2_1608[num][t];	//����1608����
		else if(size==24)temp=asc2_2412[num][t];	//����2412����
		else return;								//û�е��ֿ�
		for(t1=0;t1<8;t1++)
		{			    
			if(temp&0x80)LCD_Fast_DrawPoint(x,y,POINT_COLOR);
			else if(mode==0)LCD_Fast_DrawPoint(x,y,BACK_COLOR);
			temp<<=1;
			y++;
			if(y>=lcddev.height)return;		//��������
			if((y-y0)==size)
			{
				y=y0;
				x++;
				if(x>=lcddev.width)return;	//��������
				break;
			}
		}  	 
	}  	    	   	 	  
}
/********************************************************************
*	����	��	m^n����
******************************************************************************/
u32 LCD_Pow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}
/********************************************************************
*	����	��	��ʾ����,��λΪ0,����ʾ
*	����	��	x,y --�������
				len --���ֵ�λ��
				size--�����С
				color--��ɫ
				num--��ֵ(0~4294967295);				
******************************************************************************/
void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len,u8 size)
{         	
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/LCD_Pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				LCD_ShowChar(x+(size/2)*t,y,' ',size,1);
				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_ShowChar(x+(size/2)*t,y,temp+'0',size,1); 
	}
}
/********************************************************************
*	����	��	��ʾ����,��λΪ0,������ʾ
*	����	��	x,y--�������
				num--��ֵ(0~999999999);	 
				len--����(��Ҫ��ʾ��λ��)
				size--�����С
				mode--	[7]:0,�����;1,���0.	[6:1]:����	[0]:0,�ǵ�����ʾ;1,������ʾ.
******************************************************************************/
void LCD_ShowxNum(u16 x,u16 y,u32 num,u8 len,u8 size,u8 mode)
{  
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/LCD_Pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				if(mode&0X80)LCD_ShowChar(x+(size/2)*t,y,'0',size,mode&0X01);  
				else LCD_ShowChar(x+(size/2)*t,y,' ',size,mode&0X01);  
 				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_ShowChar(x+(size/2)*t,y,temp+'0',size,mode&0X01); 
	}
}
/********************************************************************
*	����	��	��ʾ�ַ���
*	����	��	x,y--�������
				width,height--�����С
				size--�����С
				*p--�ַ�����ʼ��ַ	
******************************************************************************/
void LCD_ShowString(u16 x,u16 y,u16 width,u16 height,u8 size,u8 *p)
{         
	u8 x0=x;
	width+=x;
	height+=y;
    while((*p<='~')&&(*p>=' '))//�ж��ǲ��ǷǷ��ַ�!
    {
        if(x>=width){x=x0;y+=size;}
        if(y>=height)break;//�˳�
        LCD_ShowChar(x,y,*p,size,1);
        x+=size/2;
        p++;
    }
}

/********************************************************************
*	����	��	��ָ��λ����ʾһ���ַ�
*	����	��	x,y--��ʼ����
				num--Ҫ��ʾ���ַ�:" "--->"~"
				size--�����С 12/16/24
				mode--���ӷ�ʽ(1)���Ƿǵ��ӷ�ʽ(0)
*	ע��	��	�ǵ��ӷ�ʽ�������ʾ���򱳾�ɫ������Ļ����ˮƽ�����д�״Ӱ��
******************************************************************************/
static void LCD_ShowChar_WithColor(u16 x,u16 y,u8 num,u8 size,u8 mode, u16 backcolor, u16 pointcolor)
{  							  
    u8 temp,t1,t;
	u16 y0=y;
	u8 csize=(size/8+((size%8)?1:0))*(size/2);		//�õ�����һ���ַ���Ӧ������ռ���ֽ���	
 	num=num-' ';//�õ�ƫ�ƺ��ֵ��ASCII�ֿ��Ǵӿո�ʼȡģ������-' '���Ƕ�Ӧ�ַ����ֿ⣩
	for(t=0;t<csize;t++)
	{   
		if(size==12)temp=asc2_1206[num][t]; 	 	//����1206����
		else if(size==16)temp=asc2_1608[num][t];	//����1608����
		else if(size==24)temp=asc2_2412[num][t];	//����2412����
		else return;								//û�е��ֿ�
		for(t1=0;t1<8;t1++)
		{			    
			if(temp&0x80)LCD_Fast_DrawPoint(x,y,pointcolor);
			else if(mode==0)LCD_Fast_DrawPoint(x,y,backcolor);
			temp<<=1;
			y++;
			if(y>=lcddev.height)return;		//��������
			if((y-y0)==size)
			{
				y=y0;
				x++;
				if(x>=lcddev.width)return;	//��������
				break;
			}
		}  	 
	}  	    	   	 	  
}

/********************************************************************
*	����	��	��ʾ�ַ���
*	����	��	x,y--�������
				width,height--�����С
				size--�����С
				*p--�ַ�����ʼ��ַ	
******************************************************************************/
void LCD_ShowString_WithColor(u16 x,u16 y,u16 width,u16 height,u8 size,u8 *p, u16 backcolor, u16 pointcolor)
{         
	u8 x0=x;
	width+=x;
	height+=y;
    while((*p<='~')&&(*p>=' '))//�ж��ǲ��ǷǷ��ַ�!
    {
        if(x>=width){x=x0;y+=size;}
        if(y>=height)break;//�˳�
        LCD_ShowChar_WithColor(x, y, *p, size, 0, backcolor, pointcolor);
        x+=size/2;
        p++;
    }
}

/********************************************************************
*	����	��	��ʼ��lcd
*	ע��	��	�ó�ʼ���������Գ�ʼ������ILI93XXҺ��,�������������ǻ���ILI9320��!!!
				�������ͺŵ�����оƬ��û�в���! 

PD4		LCD_RD			//LCD��Ļ���ƣ�	6800ģʽ��ʹ���źţ�	8080ģʽ����LCD��ȡ����
PD5		LCD_WR			//��дʱ����ƣ�6800ģʽ��xxxx��		8080ģʽ����LCDд����
PD7		LCD_CS			//LCD��Ƭѡ�ź�
PD11	LCD_RS			//���ݻ�����ѡ��0����д���1����д���ݣ�
PE5		LCD_RST			//LCD����λ�����ź�

PD14	LCD_D0
PD15	LCD_D1
PD0		LCD_D2
PD1		LCD_D3
PE7		LCD_D4
PE8		LCD_D5
PE9		LCD_D6
PE10	LCD_D7
PE11	LCD_D8
PE12	LCD_D9
PE13	LCD_D10
PE14	LCD_D11
PE15	LCD_D12
PD8		LCD_D13
PD9		LCD_D14
PD10	LCD_D15





******************************************************************************/
void LCD_Init(void)
{ 
	u8 id1=0,id2=0,id3=0,id4=0;
	GPIO_InitTypeDef  GPIO_InitStructure;
	FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
	FSMC_NORSRAMTimingInitTypeDef  readWriteTiming; 
	FSMC_NORSRAMTimingInitTypeDef  writeTiming;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD|RCC_AHB1Periph_GPIOE, ENABLE);//ʹ��PD,PEʱ��  
	RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FSMC,ENABLE);//ʹ��FSMCʱ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_14 | GPIO_Pin_15;//PD0,1,4,5,8,9,10,14,15 AF OUT
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//�������
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_Init(GPIOD, &GPIO_InitStructure);//��ʼ��  

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;//PE7~15,AF OUT
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//�������
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_Init(GPIOE, &GPIO_InitStructure);//��ʼ��

	//������
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource0,GPIO_AF_FSMC);	//PD0,AF12
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource1,GPIO_AF_FSMC);	//PD1,AF12
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource4,GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource5,GPIO_AF_FSMC); 
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource8,GPIO_AF_FSMC); 
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource9,GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource10,GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource14,GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource15,GPIO_AF_FSMC);	//PD15,AF12
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource7,GPIO_AF_FSMC);	//PE7,AF12
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource8,GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource9,GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource10,GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource11,GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource12,GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource13,GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource14,GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource15,GPIO_AF_FSMC);	//PE15,AF12
	
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource7,GPIO_AF_FSMC);	//CS#   FSMC_NE1
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource11,GPIO_AF_FSMC);	//RS    FSMC_A16
	
	
	//��ʽ1��
	readWriteTiming.FSMC_AddressSetupTime = 8;	 			//��ַ����ʱ�䣨ADDSET��Ϊ16��HCLK 1/168M=6ns*16=96ns	
	readWriteTiming.FSMC_AddressHoldTime = 0x00;	 		//��ַ����ʱ�䣨ADDHLD��ģʽAδ�õ�	
	readWriteTiming.FSMC_DataSetupTime = 15;				//���ݱ���ʱ��Ϊ60��HCLK	=6*60=360ns
	readWriteTiming.FSMC_BusTurnAroundDuration = 0x00;
	readWriteTiming.FSMC_CLKDivision = 0x01;
	readWriteTiming.FSMC_DataLatency = 0x00;
	readWriteTiming.FSMC_AccessMode = FSMC_AccessMode_A;	 //ģʽA 
    
	writeTiming.FSMC_AddressSetupTime =8;	      			//��ַ����ʱ�䣨ADDSET��Ϊ9��HCLK =54ns 
	writeTiming.FSMC_AddressHoldTime = 0x00;	 			//��ַ����ʱ�䣨A		
	writeTiming.FSMC_DataSetupTime = 9;		 			//���ݱ���ʱ��Ϊ6ns*9��HCLK=54ns
	writeTiming.FSMC_BusTurnAroundDuration = 0x00;
	writeTiming.FSMC_CLKDivision = 0x00;
	writeTiming.FSMC_DataLatency = 0x00;
	writeTiming.FSMC_AccessMode = FSMC_AccessMode_A;	 	//ģʽA 


	FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM1;//  ��������ʹ��NE1��Ҳ�Ͷ�ӦBTCR[0]��
	FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable; // ���������ݵ�ַ
	FSMC_NORSRAMInitStructure.FSMC_MemoryType =FSMC_MemoryType_SRAM;// FSMC_MemoryType_SRAM;  //SRAM   
	FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;//�洢�����ݿ��Ϊ16bit   
	FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode =FSMC_BurstAccessMode_Disable;// FSMC_BurstAccessMode_Disable; 
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
	FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait=FSMC_AsynchronousWait_Disable; 
	FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;   
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;  
	FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;	//  �洢��дʹ��
	FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;   
	FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Enable; // ��дʹ�ò�ͬ��ʱ��
	FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable; 
	FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &readWriteTiming; //��дʱ��
	FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &writeTiming;  //дʱ��

	FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);  //��ʼ��FSMC����

	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE);  // ʹ��BANK1
	
	
	//��ʽ2��ֱ�����üĴ���
	//�Ĵ�������
	//bank1��NE1~4,ÿһ����һ��BCR+TCR�������ܹ��˸��Ĵ�����
	//��������ʹ��NE1 ��Ҳ�Ͷ�ӦBTCR[0],[1]��
	
	//BTCR[0]��ӦFSMC_BCR1	BTCR[1]��ӦFSMC_BTR1	BWTR[0]��ӦFSMC_BWTR1
	//BTCR[2]��ӦFSMC_BCR2	BTCR[3]��ӦFSMC_BTR2	BWTR[2]��ӦFSMC_BWTR2
	//BTCR[4]��ӦFSMC_BCR3	BTCR[5]��ӦFSMC_BTR3	BWTR[4]��ӦFSMC_BWTR3
	//BTCR[6]��ӦFSMC_BCR4	BTCR[7]��ӦFSMC_BTR4	BWTR[6]��ӦFSMC_BWTR4
		
//	FSMC_Bank1->BTCR[0]=0X00000000;
//	FSMC_Bank1->BTCR[1]=0X00000000;
//	FSMC_Bank1E->BWTR[0]=0X00000000;
//	
//	//����BCR�Ĵ���	ʹ���첽ģʽ	FSMC_BCR0
//	FSMC_Bank1->BTCR[0]|=1<<14;		//��дʹ�ò�ͬ��ʱ��
//	FSMC_Bank1->BTCR[0]|=1<<12;		//�洢��дʹ��
//	FSMC_Bank1->BTCR[0]|=1<<4; 		//�洢�����ݿ��Ϊ16bit 
//	
//	
//	//����FSMC_BTR1�Ĵ���	
//	//��ʱ����ƼĴ��� 							    
//	FSMC_Bank1->BTCR[1]|=0<<28;		//ģʽA 	 							  	 
//	FSMC_Bank1->BTCR[1]|=15<<0; 		//��ַ����ʱ�䣨ADDSET��Ϊ2��HCLK 1/36M=27ns(ʵ��>200ns)			1111=16*1/168M=6ns*16=96ns
//	//��ΪҺ������IC�Ķ����ݵ�ʱ���ٶȲ���̫�죬�����1289���IC��
//	FSMC_Bank1->BTCR[1]|=60<<8;  	//���ݱ���ʱ��Ϊ16��HCLK	 	 										0xf
//	
//	//дʱ����ƼĴ���FSMC_BWTR0  
//	FSMC_Bank1E->BWTR[0]|=0<<28; 	//ģʽA 
//	//FSMC_Bank1E->BWTR[0]|=1<<20;		//ʱ�ӷ���2��	
//	FSMC_Bank1E->BWTR[0]|=15<<0;		//��ַ����ʱ�䣨ADDSET��Ϊ1��HCLK 									0
// 	//4��HCLK��HCLK=72M����ΪҺ������IC��д�ź���������Ҳ��50ns��72M/4=24M=55ns  	 
//	FSMC_Bank1E->BWTR[0]|=15<<8; 	//���ݱ���ʱ��Ϊ4��HCLK												3
//	
//	//ʹ��BANK1,����4
//	FSMC_Bank1->BTCR[0]|=1<<0;		//ʹ��BANK1������4	
	
//	




	Delay_ms(800); 					// delay 50 ms�ܹ���ʾ����ʱ1000msҲ����ʾ���ӵ��٣����ȶ�������ʱ600ms������������ʱ800-1000ms 	
  	lcddev.id=LCD_ReadReg(0x0000);	//��ID��9320/9325/9328/4531/4535��IC��
	//log_info("lcddev.id=0x%x\r\n",lcddev.id);	
	Delay_ms(10);
	
	#if(LCD_ID_PLAN == ID_PLAN_A)		//PLAN-A
		lcddev.id = 0x1963;				//������ֱ�ӽ�SSD1961��������Ļ���Ϊ1963��ֱ�Ӹ�ֵ�϶�ȡID���Խ׶��ܸ������ʵ��������Ļ��ʾ
	#elif(LCD_ID_PLAN == ID_PLAN_B)		//PLAN-B
		if(lcddev.id<=0XFF||lcddev.id==0XFFFF||lcddev.id==0X9300)//����ID����ȷ,����lcddev.id==0X9300�жϣ���Ϊ9341��δ����λ������»ᱻ����9300	//���е��˶�ȡlcddev.id=0xff��������ֲ�������=0xff����V55��
		{	
			//����9341 ID�Ķ�ȡ		
//			LCD_WR_REG(0XD3);
//			lcddev.id=LCD_RD_DATA();	//dummy read 	
//			lcddev.id=LCD_RD_DATA();	//����0X00
//			lcddev.id=LCD_RD_DATA();   	//��ȡ93								   
//			lcddev.id<<=8;
//			lcddev.id|=LCD_RD_DATA();  	//��ȡ41 	   			   
//			if(lcddev.id!=0X9341)		//��9341,�����ǲ���6804
//			{	
//				LCD_WR_REG(0XBF);				   
//				lcddev.id=LCD_RD_DATA(); 	//dummy read 	 
//				lcddev.id=LCD_RD_DATA();   	//����0X01			   
//				lcddev.id=LCD_RD_DATA(); 	//����0XD0 			  	
//				lcddev.id=LCD_RD_DATA();	//�������0X68 
//				lcddev.id<<=8;
//				lcddev.id|=LCD_RD_DATA();	//�������0X04	  
//				if(lcddev.id!=0X6804)		//Ҳ����6804,���Կ����ǲ���NT35310
//				{ 
//					LCD_WR_REG(0XD4);				   
//					lcddev.id=LCD_RD_DATA();//dummy read  
//					lcddev.id=LCD_RD_DATA();//����0X01	 
//					lcddev.id=LCD_RD_DATA();//����0X53	
//					lcddev.id<<=8;	 
//					lcddev.id|=LCD_RD_DATA();	//�������0X10	 
//					if(lcddev.id!=0X5310)		//Ҳ����NT35310,���Կ����ǲ���NT35510
//					{
//						LCD_WR_REG(0XDA00);	
//						lcddev.id=LCD_RD_DATA();		//����0X00	 
//						LCD_WR_REG(0XDB00);	
//						lcddev.id=LCD_RD_DATA();		//����0X80
//						lcddev.id<<=8;	
//						LCD_WR_REG(0XDC00);	
//						lcddev.id|=LCD_RD_DATA();		//����0X00		
//						if(lcddev.id==0x8000)lcddev.id=0x5510;//NT35510���ص�ID��8000H,Ϊ��������,����ǿ������Ϊ5510
//						if(lcddev.id!=0X5510)			//Ҳ����NT5510,���Կ����ǲ���SSD1963
//						{
							LCD_WR_REG(0XA1);
							lcddev.id=LCD_RD_DATA();
							lcddev.id=LCD_RD_DATA();	//����0X57
							lcddev.id<<=8;	 
							lcddev.id|=LCD_RD_DATA();	//����0X61	
							if(lcddev.id==0X5761)lcddev.id=0X1963;//SSD1963���ص�ID��5761H,Ϊ��������,����ǿ������Ϊ1963
//						}
//					}
//				}
//			}  	
		}
//		printf(" LCD ID:%x\r\n",lcddev.id); //��ӡLCD ID  
	#endif

	if(lcddev.id==0X1963)
	{		
		LCD_WR_REG(0xE2);		//Set PLL with OSC = 10MHz (hardware),	Multiplier N = 35, 250MHz < VCO < 800MHz = OSC*(N+1), VCO = 300MHz
		LCD_WR_DATA(0x1D);		//����1 
		LCD_WR_DATA(0x02);		//����2 Divider M = 2, PLL = 300/(M+1) = 100MHz
		LCD_WR_DATA(0x04);		//����3 Validate M and N values  		
		delay_us(100);		
		LCD_WR_REG(0xE0);		// Start PLL command
		LCD_WR_DATA(0x01);		// enable PLL		
		Delay_ms(10);
		LCD_WR_REG(0xE0);		// Start PLL command again
		LCD_WR_DATA(0x03);		// now, use PLL output as system clock	
		Delay_ms(12);  
		LCD_WR_REG(0x01);		//��λ
		Delay_ms(10);
		Delay_ms(800);			// longzhigu:  [V194]
		
		LCD_WR_REG(0xE6);		//��������Ƶ��,33Mhz
		LCD_WR_DATA(0x2F);		//web--0x04
		LCD_WR_DATA(0xFF);		//web--0xcc
		LCD_WR_DATA(0xFF);		//web--0xc8
		
		LCD_WR_REG(0xB0);		//����LCDģʽ
		LCD_WR_DATA(0x10);		//0X20--24λģʽ		//web--0x10 18bit[V55]
		LCD_WR_DATA(0x00);		//TFT ģʽ 
	
		LCD_WR_DATA((SSD_HOR_RESOLUTION-1)>>8);//����LCDˮƽ����
		LCD_WR_DATA(SSD_HOR_RESOLUTION-1);		 
		LCD_WR_DATA((SSD_VER_RESOLUTION-1)>>8);//����LCD��ֱ����
		LCD_WR_DATA(SSD_VER_RESOLUTION-1);		 
		LCD_WR_DATA(0x00);		//RGB���� 
		
		LCD_WR_REG(0xB4);		//Set horizontal period
		LCD_WR_DATA((SSD_HT-1)>>8);
		LCD_WR_DATA(SSD_HT-1);
		LCD_WR_DATA(SSD_HPS>>8);
		LCD_WR_DATA(SSD_HPS);
		LCD_WR_DATA(SSD_HOR_PULSE_WIDTH-1);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		
		LCD_WR_REG(0xB6);		//Set vertical period
		LCD_WR_DATA((SSD_VT-1)>>8);
		LCD_WR_DATA(SSD_VT-1);
		LCD_WR_DATA(SSD_VPS>>8);
		LCD_WR_DATA(SSD_VPS);
		LCD_WR_DATA(SSD_VER_FRONT_PORCH-1);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		
		LCD_WR_REG(0xF0);	//����SSD1963��CPU�ӿ�Ϊ16bit
		LCD_WR_DATA(0x03);	//16-bit(565 format) data for 16bpp

		LCD_WR_REG(0x29);	//������ʾ
		//����PWM���  ����ͨ��ռ�ձȿɵ� 
		LCD_WR_REG(0xD0);	//�����Զ���ƽ��DBC
		LCD_WR_DATA(0x00);	//disable
	
		LCD_WR_REG(0xBE);	//����PWM���
		LCD_WR_DATA(0x05);	//1����PWMƵ��
		LCD_WR_DATA(0xFE);	//2����PWMռ�ձ�
		LCD_WR_DATA(0x01);	//3����C
		LCD_WR_DATA(0x00);	//4����D
		LCD_WR_DATA(0x00);	//5����E 
		LCD_WR_DATA(0x00);	//6����F 
		
		LCD_WR_REG(0xB8);	//����GPIO����
		LCD_WR_DATA(0x03);	//2��IO�����ó����			//web--0x0f
		LCD_WR_DATA(0x01);	//GPIOʹ��������IO����
		LCD_WR_REG(0xBA);
		LCD_WR_DATA(0X01);	//GPIO[1:0]=01,����LCD����
		
		LCD_SSD_BackLightSet(0);//��������Ϊ����100
	}		 
	LCD_Display_Dir(1);		//Ĭ��Ϊ����0
	LCD_Clear(COLOR_DESKTOP);
}
/********************************************************************
*	����	��	LCD��λ���ų�ʼ��
******************************************************************************/
void LCDRST_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);//ʹ��GPIOAʱ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_Init(GPIOE, &GPIO_InitStructure);//��ʼ��
	
	
	
	LCD_RST_L();
	Delay_ms(200);
	LCD_RST_H();
	
//	delay_us(4);
//	pLCDRST = 1;
	
}

