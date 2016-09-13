#include "charger.h"

u8 ChargerStatus = OPEN_CHARGER_STATUS;		//充电模块状态

void Charger_Init(void)		//充电模块初始化
{
	GPIO_InitTypeDef GPIO_InitStructure;

 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);//使能PORTC时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;	 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 	GPIO_Init(GPIOC, &GPIO_InitStructure);					 //根据设定参数初始化GPIOC.8,GPIOC.9
 	GPIO_ResetBits(GPIOC,GPIO_Pin_9);						 //PC.8 输出高
}

void Open_Charger(void)				//打开充电模块开关
{
	GPIO_ResetBits(GPIOC,GPIO_Pin_9);
	ChargerStatus = OPEN_CHARGER_STATUS;	
}

void Close_Charger(void)			//关闭充电模块开关			
{
	GPIO_SetBits(GPIOC,GPIO_Pin_9);
	ChargerStatus = CLOSE_CHARGER_STATUS;		
}





