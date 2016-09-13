#ifndef KEY_H
#define KEY_H

#include "sys.h"

#define KEY1  	GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_10)//��ȡ����1
#define KEY2  	GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_11)//��ȡ����2
#define KEY3   	GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_12)//��ȡ����3
#define KEY4  	GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13)//��ȡ����4
#define KEY5  	GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_14)//��ȡ����5
#define KEY6  	GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_15)//��ȡ����6

//ɨ��ģʽ�¶�ȡ��ֵ
#define KEY1_PRES	1		//KEY1  
#define KEY2_PRES	2		//KEY2 
#define KEY3_PRES	3		//KEY3  
#define KEY4_PRES	4		//KEY4
#define KEY5_PRES	5		//KEY5
#define KEY6_PRES	6		//KEY6 

void KEY_Init(void);	//����IO��ʼ��
u8 KEY_Scan(u8 mode);  	//����ɨ�躯��	


#endif

