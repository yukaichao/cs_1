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
//////////////////////////////////////////////////////////////////////////////////
#ifndef __OLED_H
#define __OLED_H			  	 

#include "stdlib.h"	    	

#include "main.h"
			  
//extern I2C_HandleTypeDef hi2c1;
//OLED�����ú���
void WriteCmd(void);


void OLED_Init(void);
void OLED_Clear(void);

void OLED_ShowNum(uint8_t x,uint8_t y,unsigned int num,uint8_t len,uint8_t size2);//��ʾ2������
void OLED_ShowChar(uint8_t x,uint8_t y,char chr,uint8_t Char_Size,uint8_t Is_Reverse);
void OLED_ShowString(uint8_t x,uint8_t y,char chr[],uint8_t Char_Size);//��ʾһ���ַ��Ŵ�
void OLED_ShowCHinese(uint8_t x,uint8_t y,uint8_t no);//��ʾ����
void OLED_Refreash(void);
#endif  
	 



