//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//�о�԰����
//���̵�ַ��http://shop73023976.taobao.com/?spm=2013.1.0.0.M4PqC2
//
//  �� �� ��   : main.c
//  �� �� ��   : v2.0
//  ��    ��   : Evk123
//  ��������   : 2014-0101
//  ����޸�   : 
//  ��������   : 0.69��OLED �ӿ���ʾ����(STM32F103ZEϵ��IIC)
//              ˵��: 
//              ----------------------------------------------------------------
//              GND   ��Դ��
//              VCC   ��5V��3.3v��Դ
//              SCL   ��PD6��SCL��
//              SDA   ��PD7��SDA��            
//              ----------------------------------------------------------------
//Copyright(C) �о�԰����2014/3/16
//All rights reserved
//////////////////////////////////////////////////////////////////////////////////?

#include "oled.h"

#include "oledfont.h"  	 

#include "stm32f4xx_hal.h"			   
extern I2C_HandleTypeDef hi2c1;

uint8_t OLED_1_data[8][128]={0};
uint8_t OLED_2_data[8][128]={0};
uint8_t CMD_Data[]={
0xAE, 0x00, 0x10, 0x40, 0xB0, 0x81, 0xFF, 0xA1, 0xA6, 0xA8, 0x3F,
					
0xC8, 0xD3, 0x00, 0xD5, 0x80, 0xD8, 0x05, 0xD9, 0xF1, 0xDA, 0x12,
					
0xDB, 0x30, 0x8D, 0x14, 0xAF};
void WriteCmd(uint8_t ADDRESS)
{
 uint8_t i=0;
 for(i=0; i<27; i++)
	{
 HAL_I2C_Mem_Write(&hi2c1 ,0x78,0x00,I2C_MEMADD_SIZE_8BIT,CMD_Data+i,1,0x100);
  }
}


//���豸д����
void OLED_Refreash(void)
{
	static char oled_refresh = 0;
	
	if(oled_refresh == 0)
	{
		//HAL_I2C_Mem_Write(&hi2c1 ,0x78,0x40,I2C_MEMADD_SIZE_8BIT,&data,1,0xff);
		HAL_I2C_Mem_Write_DMA(&hi2c1,OLED_1,0x40,I2C_MEMADD_SIZE_8BIT,OLED_1_data[0],1024);
		oled_refresh = 1;
	}
	else
	{
		//HAL_I2C_Mem_Write(&hi2c1 ,0x78,0x40,I2C_MEMADD_SIZE_8BIT,&data,1,0xff);
		HAL_I2C_Mem_Write_DMA(&hi2c1,OLED_2,0x40,I2C_MEMADD_SIZE_8BIT,OLED_2_data[0],1024);
		oled_refresh = 0;
	}
}

//��ʼ��oled��Ļ
void OLED_Init(void)
{ 	
	HAL_Delay(200);//�������ʱ����Ҫ
	WriteCmd(OLED_1);
	HAL_Delay(200);//�������ʱ����Ҫ
	WriteCmd(OLED_2);
}
//����
void OLED_Clear(uint8_t ADDRESS)
{
	uint8_t i,n;
	
	if(ADDRESS == OLED_1)
	{
		for(i=0;i<8;i++)  
		{  
			for(n=0;n<128;n++)
			OLED_1_data[i][n]=0;
		}
	}
	else if(ADDRESS == OLED_2)
	{
		for(i=0;i<8;i++)  
		{  
			for(n=0;n<128;n++)
			OLED_2_data[i][n]=0;
		}
	}
}

unsigned int oled_pow(uint8_t m,uint8_t n)
{
	unsigned int result=1;	 
	while(n--)result*=m;    
	return result;
}
//��ʾ2������
//x,y :�������	 
//len :���ֵ�λ��
//size:�����С
//mode:ģʽ	0,���ģʽ;1,����ģʽ
//num:��ֵ(0~4294967295);	 		  
void OLED_ShowNum(uint8_t ADDRESS,uint8_t x,uint8_t y,unsigned int num,uint8_t len,uint8_t size2)
{         	
	uint8_t t,temp;
	uint8_t enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/oled_pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(ADDRESS,x+(size2/2)*t,y,' ',size2,0);
				continue;
			}else enshow=1; 
		 	 
		}
	 	OLED_ShowChar(ADDRESS,x+(size2/2)*t,y,temp+'0',size2,0); 
	}
} 
//��ָ��λ����ʾһ���ַ�,���������ַ�
//x:0~127
//y:0~63
//mode:0,������ʾ;1,������ʾ				 
//size:ѡ������ 16/12 
void OLED_ShowChar(uint8_t ADDRESS,uint8_t x,uint8_t y,char chr,uint8_t Char_Size,uint8_t Is_Reverse)
{      	
	unsigned char c=0,i=0;
	
	c=chr-' ';//�õ�ƫ�ƺ��ֵ
	
	if(x>128-1)
	{
		x=0;
		y=y+2;
	}
	
	if(ADDRESS == OLED_1)
	{
		if(Char_Size ==16)
		{
			if(Is_Reverse==0)
			{
				for(i=0;i<8;i++)
				{
					OLED_1_data[y][x+i] = F8X16[c*16+i];		
					OLED_1_data[y+1][x+i] = F8X16[c*16+i+8];	
				}
			}
			else
			{
				OLED_1_data[y][x+i] = ~F8X16[c*16+i];		
				OLED_1_data[y+1][x+i] = ~F8X16[c*16+i+8];				
			}
		}
		else
		{
			if(Is_Reverse==0)
			{
				for(i=0;i<6;i++)
				OLED_1_data[y][x+i] = F6x8[c][i];
			}
			else
			{
				for(i=0;i<6;i++)
				OLED_1_data[y][x+i] = ~F6x8[c][i];
			}
		}
	}
	else if(ADDRESS == OLED_2)
	{
		if(Char_Size ==16)
		{
			if(Is_Reverse==0)
			{
				for(i=0;i<8;i++)
				{
					OLED_2_data[y][x+i] = F8X16[c*16+i];		
					OLED_2_data[y+1][x+i] = F8X16[c*16+i+8];	
				}
			}
			else
			{
				OLED_2_data[y][x+i] = ~F8X16[c*16+i];		
				OLED_2_data[y+1][x+i] = ~F8X16[c*16+i+8];				
			}
		}
		else
		{
			if(Is_Reverse==0)
			{
				for(i=0;i<6;i++)
				OLED_2_data[y][x+i] = F6x8[c][i];
			}
			else
			{
				for(i=0;i<6;i++)
				OLED_2_data[y][x+i] = ~F6x8[c][i];
			}
		}
	}
}
 
//��ʾһ���ַ��Ŵ�
void OLED_ShowString(uint8_t ADDRESS,uint8_t x,uint8_t y,char chr[],uint8_t Char_Size)
{
	unsigned char j=0;
	while (chr[j]!='\0')
	{		OLED_ShowChar(ADDRESS,x,y,chr[j],Char_Size,0);
			x+=8;
		if(x>120){x=0;y+=2;}
			j++;
	}
}
//��ʾ����
//hzk ��ȡģ����ó�������
void OLED_ShowCHinese(uint8_t ADDRESS,uint8_t x,uint8_t y,uint8_t no)
{      			    
	uint8_t t;
	
	if(ADDRESS == OLED_1)
	{
    for(t=0;t<16;t++)
		{
			OLED_1_data[y][x+t]=Hzk[2*no][t];
    }
    for(t=0;t<16;t++)
		{
			OLED_1_data[y+1][x+t]=Hzk[2*no+1][t];
    }
	}
	else if(ADDRESS == OLED_2)
	{
    for(t=0;t<16;t++)
		{
			OLED_2_data[y][x+t]=Hzk[2*no][t];
    }
    for(t=0;t<16;t++)
		{
			OLED_2_data[y+1][x+t]=Hzk[2*no+1][t];
    }
	}
}
