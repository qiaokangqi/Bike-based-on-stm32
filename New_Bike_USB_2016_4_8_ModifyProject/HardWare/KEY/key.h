#ifndef KEY_H
#define KEY_H

#include "sys.h"

#define KEY1  	GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_10)//读取按键1
#define KEY2  	GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_11)//读取按键2
#define KEY3   	GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_12)//读取按键3
#define KEY4  	GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13)//读取按键4
#define KEY5  	GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_14)//读取按键5
#define KEY6  	GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_15)//读取按键6

//扫描模式下读取键值
#define KEY1_PRES	1		//KEY1  
#define KEY2_PRES	2		//KEY2 
#define KEY3_PRES	3		//KEY3  
#define KEY4_PRES	4		//KEY4
#define KEY5_PRES	5		//KEY5
#define KEY6_PRES	6		//KEY6 

void KEY_Init(void);	//按键IO初始化
u8 KEY_Scan(u8 mode);  	//按键扫描函数	


#endif

