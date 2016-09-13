#include "stm32f10x.h"
#include "pinDefine.h"
#include "sys.h"
#include "usart.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

extern char debug_msg[128];   				   //������

int open_switch(unsigned char index)
{
	//��
	switch (index) {
	  case 1 : GPIO_ResetBits(SWITCH_1); break;
	  case 2 : GPIO_ResetBits(SWITCH_2); break;
	  case 3 : GPIO_ResetBits(SWITCH_3); break;
	  case 4 : GPIO_ResetBits(SWITCH_4); break;
	  case 5 : GPIO_ResetBits(SWITCH_5); break;
	  case 6 : GPIO_ResetBits(SWITCH_6); break;
	  case 7 : GPIO_ResetBits(SWITCH_7); break;
	  case 8 : GPIO_ResetBits(SWITCH_8); break; 
	  case 9 : GPIO_ResetBits(SWITCH_9); break; 
	  case 10 : GPIO_ResetBits(SWITCH_10); break; 
	  case 11 : GPIO_ResetBits(SWITCH_11); break; 
	  case 12 : GPIO_ResetBits(SWITCH_12); break; 
	  case 13 : GPIO_ResetBits(SWITCH_13); break;
	  case 14 : GPIO_ResetBits(SWITCH_14); break;
	  case 15 : GPIO_ResetBits(SWITCH_15); break;
	  case 16 : GPIO_ResetBits(SWITCH_16); break;
	}
	return 1;
}

int close_switch(unsigned char index)
{
	switch (index) {
	  case 1 : GPIO_SetBits(SWITCH_1); break;
	  case 2 : GPIO_SetBits(SWITCH_2); break;
	  case 3 : GPIO_SetBits(SWITCH_3); break;
	  case 4 : GPIO_SetBits(SWITCH_4); break;
	  case 5 : GPIO_SetBits(SWITCH_5); break;
	  case 6 : GPIO_SetBits(SWITCH_6); break;
	  case 7 : GPIO_SetBits(SWITCH_7); break;
	  case 8 : GPIO_SetBits(SWITCH_8); break; 
	  case 9 : GPIO_SetBits(SWITCH_9); break; 
	  case 10 : GPIO_SetBits(SWITCH_10); break; 
	  case 11 : GPIO_SetBits(SWITCH_11); break; 
	  case 12 : GPIO_SetBits(SWITCH_12); break; 
	  case 13 : GPIO_SetBits(SWITCH_13); break;
	  case 14 : GPIO_SetBits(SWITCH_14); break;
	  case 15 : GPIO_SetBits(SWITCH_15); break;
	  case 16 : GPIO_SetBits(SWITCH_16); break;
	}
	return 1;
}




//�������� 1-18
#define GO_1    1
#define GO_2    2
#define GO_3    3
#define GO_4    4
#define GO_5    5
#define GO_6    6
#define GO_7    7
#define GO_8    8
#define GO_9 	9
#define GO_10   10
#define GO_11   11
#define GO_12   12
#define GO_13   13
#define GO_14   14
#define GO_15   15
#define GO_16   16
#define GO_17   17
#define GO_18   18



unsigned char get_all_go_status(unsigned char go_type,unsigned char *buf)
{
	unsigned char status = 0;
	buf[0]  = GPIO_ReadInputDataBit(GO_IN_1);
	buf[1]  = GPIO_ReadInputDataBit(GO_IN_2);
	buf[2]  = GPIO_ReadInputDataBit(GO_IN_3);
	buf[3]  = GPIO_ReadInputDataBit(GO_IN_4);
	buf[4]  = GPIO_ReadInputDataBit(GO_IN_5);
	buf[5]  = GPIO_ReadInputDataBit(GO_IN_6);
	buf[6]  = GPIO_ReadInputDataBit(GO_IN_7);
	buf[7]  = GPIO_ReadInputDataBit(GO_IN_8);
	buf[8]  = GPIO_ReadInputDataBit(GO_IN_9);
	buf[9]  = GPIO_ReadInputDataBit(GO_IN_10);
	buf[10] = GPIO_ReadInputDataBit(GO_IN_11);
	buf[11] = GPIO_ReadInputDataBit(GO_IN_12);
	buf[12] = GPIO_ReadInputDataBit(GO_IN_13);
	buf[13] = GPIO_ReadInputDataBit(GO_IN_14);
	buf[14] = GPIO_ReadInputDataBit(GO_IN_15);
	buf[15] = GPIO_ReadInputDataBit(GO_IN_16);
	buf[16] = GPIO_ReadInputDataBit(GO_IN_17);
	buf[17] = GPIO_ReadInputDataBit(GO_IN_18);

	
	
	//g_go_type = go_type;   //���ù�������
	return status;
}

void check_go(unsigned char go_type)
{
	unsigned char char1 = 0,char2 = 0;
	//sprintf(debug_msg,"go_type = %d \r\n\0",(int)go_type);
	//uart1_send(debug_msg,strlen(debug_msg));
	STATUS_1_LED = LED_OFF;
	STATUS_2_LED = LED_OFF;
	switch (go_type) {		
	  case  1 : char1 = GPIO_ReadInputDataBit(GO_IN_1);
	  			char2 = GPIO_ReadInputDataBit(GO_IN_2);
				//sprintf(debug_msg,"1 char1=%d,char2=%d \r\n\0",(int)char1,(int)char2);
				//uart1_send(debug_msg,strlen(debug_msg));				
	  			if (char1 == 0)	  //������ ���̹��� ����
	              STATUS_1_LED = LED_ON;	
		        if (char2 == 0)
	        	  STATUS_2_LED = LED_ON;	
	    		break;
	  case 2 :  char1 = GPIO_ReadInputDataBit(GO_IN_3);
	  			//sprintf(debug_msg,"2 char1=%d,char2=%d \r\n\0",(int)char1,(int)char2);
				//uart1_send(debug_msg,strlen(debug_msg));				
	  			if (char1)	 //����ۿ���
	  			  STATUS_1_LED = LED_ON;
				break;
	  case 3 :  char1 = GPIO_ReadInputDataBit(GO_IN_4);
	  			if (char1)  // ��΢�� ����
	  			  STATUS_1_LED = LED_ON;
				break;
	  case 4 :  char1 = GPIO_ReadInputDataBit(GO_IN_5);
	  			if (char1)  // ��΢�� ǰ��
	  			  STATUS_1_LED = LED_ON;
				break;	
	  case 5 :  char1 = GPIO_ReadInputDataBit(GO_IN_6);
	  			if (char1)  // ע��������
	  			  STATUS_1_LED = LED_ON;
				break;				 					   
	  case 6 :  char1 = GPIO_ReadInputDataBit(GO_IN_7);
	  			char2 = GPIO_ReadInputDataBit(GO_IN_8);
	  			if (char1 == 0)  // �Լ��� ���� ����
	  			  STATUS_1_LED = LED_ON;
				if (char2 == 0)  
	  			  STATUS_2_LED = LED_ON;
				break; 
	  case 7 :  char1 = GPIO_ReadInputDataBit(GO_IN_9);
	  			if (char1)  //   ������ ����
	  			  STATUS_1_LED = LED_ON;
				break; 
	  case 8 :  char1 = GPIO_ReadInputDataBit(GO_IN_10);
	  			if (char1 == 0)  //  ������ ����
	  			  STATUS_1_LED = LED_ON;
				break; 	
	  case 9 :  char1 = GPIO_ReadInputDataBit(GO_IN_11);
	  			if (char1)  //  �˿���
	  			  STATUS_1_LED = LED_ON;
				break; 		  	
	  case 10:  char1 = GPIO_ReadInputDataBit(GO_IN_12);
	  			if (char1)  //  �ƿ���
	  			  STATUS_1_LED = LED_ON;
				break; 		  	
	  case 11:  char1 = GPIO_ReadInputDataBit(GO_IN_13);
	  			if (char1)  //  �Լ���
	  			  STATUS_1_LED = LED_ON;
				break; 		  	
	}
}

//��ȡ����״̬
unsigned char get_go_status(unsigned char index)
{
	unsigned char status = 0;
	switch (index) {
	  case GO_1  : status = GPIO_ReadInputDataBit(GO_IN_1);
	  case GO_2  : status = GPIO_ReadInputDataBit(GO_IN_2);	
	  case GO_3  : status = GPIO_ReadInputDataBit(GO_IN_3);
	  case GO_4  : status = GPIO_ReadInputDataBit(GO_IN_4);
	  case GO_5  : status = GPIO_ReadInputDataBit(GO_IN_5);	
	  case GO_6  : status = GPIO_ReadInputDataBit(GO_IN_6);
	  case GO_7  : status = GPIO_ReadInputDataBit(GO_IN_7);
	  case GO_8  : status = GPIO_ReadInputDataBit(GO_IN_8);	
	  case GO_9  : status = GPIO_ReadInputDataBit(GO_IN_9);
	  case GO_10 : status = GPIO_ReadInputDataBit(GO_IN_10);
	  case GO_11 : status = GPIO_ReadInputDataBit(GO_IN_11);	
	  case GO_12 : status = GPIO_ReadInputDataBit(GO_IN_12);	
	  case GO_13 : status = GPIO_ReadInputDataBit(GO_IN_13);
	  case GO_14 : status = GPIO_ReadInputDataBit(GO_IN_14);	
	  case GO_15 : status = GPIO_ReadInputDataBit(GO_IN_15);
	  case GO_16 : status = GPIO_ReadInputDataBit(GO_IN_16);
	  case GO_17 : status = GPIO_ReadInputDataBit(GO_IN_17);	
	  case GO_18 : status = GPIO_ReadInputDataBit(GO_IN_18);
	}
	return status;
}

//IN 1 - 6

#define FUZI_FEIYE    1	  //��Һ    ����
#define FUZI_XISHIYE  2   //ϡ��Һ  ����
#define FUZI_QINGXIYE 3   //��ϴҺ	����

//��ȡ ��Һ ���� ״̬
unsigned char get_feiye_fuzi_status()
{
	return GPIO_ReadInputDataBit(KEY_FEIYE_FUZI);	
}

//��ȡ ϡ��Һ ���� ״̬
unsigned char get_xishiye_fuzi_status()
{
	return GPIO_ReadInputDataBit(KEY_XISHIYE_FUZI);	
}

//��ȡ ��ϴҺ ���� ״̬
unsigned char get_qingxiye_fuzi_status()
{
	return GPIO_ReadInputDataBit(KEY_QINGXIYE_FUZI);	
}

unsigned char get_input_status(unsigned char index)
{
  unsigned char status = 0;
  switch (index) {
    case FUZI_FEIYE 	: status = get_feiye_fuzi_status(); break;   	//��� ��Һ ���� ״̬
	case FUZI_XISHIYE   : status = get_xishiye_fuzi_status(); break;   	//��� ϡ��Һ ���� ״̬
	case FUZI_QINGXIYE  : status = get_qingxiye_fuzi_status(); break;  	//��� ��ϴҺ ���� ״̬	
  }
  return status;
}

void init_switch()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	unsigned char index = 0;
    
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    //GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);  //��ȫʧ��JTAG/SWD
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);//�ر�jtag������swd
	
	//���� ���� ��� io			
	//����			 	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);																		   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);																		   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_InitStructure);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);																		   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF,ENABLE);																		   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOF,&GPIO_InitStructure);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG,ENABLE);																		   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOG,&GPIO_InitStructure);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);																		   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOE,&GPIO_InitStructure);
		
	
	//6������� 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);																		   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOE,&GPIO_InitStructure);				 


	//���� ��� ��� io [����]  
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_5 |GPIO_Pin_4 | GPIO_Pin_3 | GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_2 |GPIO_Pin_1 | GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_InitStructure);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF,ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 |GPIO_Pin_4 | GPIO_Pin_3 | GPIO_Pin_2 | GPIO_Pin_1 | GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOF,&GPIO_InitStructure);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOE,&GPIO_InitStructure);

	//�ر����еĿ���
	for (index = 1; index <= 16; index++)
	  close_switch(index);

    //���� ��� ���io ���
	// motor 1
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 |GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);

	//motor2
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_4 |GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
}
