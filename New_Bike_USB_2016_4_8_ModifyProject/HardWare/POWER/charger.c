#include "charger.h"

u8 ChargerStatus = OPEN_CHARGER_STATUS;		//���ģ��״̬

void Charger_Init(void)		//���ģ���ʼ��
{
	GPIO_InitTypeDef GPIO_InitStructure;

 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);//ʹ��PORTCʱ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;	 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
 	GPIO_Init(GPIOC, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOC.8,GPIOC.9
 	GPIO_ResetBits(GPIOC,GPIO_Pin_9);						 //PC.8 �����
}

void Open_Charger(void)				//�򿪳��ģ�鿪��
{
	GPIO_ResetBits(GPIOC,GPIO_Pin_9);
	ChargerStatus = OPEN_CHARGER_STATUS;	
}

void Close_Charger(void)			//�رճ��ģ�鿪��			
{
	GPIO_SetBits(GPIOC,GPIO_Pin_9);
	ChargerStatus = CLOSE_CHARGER_STATUS;		
}





