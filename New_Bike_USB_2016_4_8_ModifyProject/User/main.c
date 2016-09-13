
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usart.h"
#include "usb_lib.h"
#include "hw_config.h"
#include "usbio.h"
#include "includes.h" 
#include "delay.h"
#include "sys.h"
#include "pinDefine.h"
#include "led.h"
#include "key.h"
#include "rc522.h"
#include "charger.h"
#include "leds.h"
#include "heart.h"
#include "capture.h"
#include "angle.h"
#include "power.h"
#include "bluetooth.h"


//����Ҫ�޸ĵ�λ��   Start
#define MAX_POWER     140
#define MIN_POWER	  100
//����Ҫ�޸ĵ�λ��   End


extern volatile uint32_t EP1_ReceivedCount;
extern volatile uint32_t EP2_ReceivedCount;
extern volatile uint8_t USB_Received_Flag;
unsigned char CT[2];//������
unsigned char SN[4]; //����

extern u16 HeartSpeed;
extern u16 BikeSpeed;
extern u16 TmpBikeSpeed;
extern u32 BikeTime;
extern u32 BikeTimeOld;
extern u8 IsSpeedStart;
extern u32 SpeedDataTime;
extern u8 ChargerStatus;
extern u16 KeyStatus;
extern u8 BlueToothStatus;


int main(void)
{  	   
	u8 SendBuf[32];
	u8 ReceBuf[32];	
	u8 i,status,key_status = 0,key_tran = 0,power_dir = 0;
	u16 adcx,adcy;
	u16 tempx = 2738,tempy;
	u8 Resistance = 136,Resistance_Status = 0;
	u16 LedsData;			 

	delay_init();	    	 		  //��ʱ��ʼ��	  
	NVIC_Configuration(); 	 		  //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	//��ʼ�� usb 
	USB_Interrupts_Config();
	Set_USBClock();
	USB_Init();

	LED_Init();
	KEY_Init();
	Charger_Init();
	LEDS_Init(899,0);				//TIM5 	80Khz
	Power_Init(899,0);				//TIM5 	80Khz
	Heart_Init(99,36-1);			//TIM3	20Khz
 	TIM2_Init(999,360-1);			//TIM2	200hz
	InitRc522();
	BlueTooth_Init();
	Adc_Init();
	 

	SetPower(350);
	Open_Charger();
	while(1)
	{
		if(USB_Received_Flag)
		{
			USB_Received_Flag = 0;
			USB_GetData(ENDP1,SendBuf,EP1_ReceivedCount);
//			if(SendBuf[1] < 56 && SendBuf[1] > 0)
//			{
//				Resistance = SendBuf[1]+110;
//			}
//			else
//			{
//				Resistance = 136;
//			}	
			USB_SendData(ENDP1,ReceBuf,16);
		}
		//��׽����
		i=KEY_Scan(0);
		if(i != 0 && key_status == 0)
		{
			key_status = 1;
			key_tran = i;	
		}
		if(key_status == 1 && i == 0)
		{
			key_status = 0;	
		}
		switch(i)
		{
			case KEY1_PRES:LED0 = !LED0;break;
			case KEY2_PRES:LED0 = !LED0;break;
			case KEY3_PRES:LED0 = !LED0;break;
			case KEY4_PRES:LED0 = !LED0;break;
			case KEY5_PRES:LED0 = !LED0;break;
			case KEY6_PRES:LED0 = !LED0;
							if(tempy > MAX_POWER)
							{
								power_dir = 0;
								SetForward();
								delay_ms(50);
								SetStop();
							}
							else if(tempy < MIN_POWER)
							{
								power_dir = 1;
							 	SetBackward();
								delay_ms(50);
								SetStop();
							}
							else
							{
								if(power_dir == 0)
								{
								 	SetForward();
									delay_ms(50);
									SetStop();
								}
								else if(power_dir == 1)
								{
									SetBackward();
									delay_ms(50);
									SetStop();	
								}
							}
												
							break;
			default:break;
	  	}

		//��׽ˢ��
		status = PcdRequest(PICC_REQALL,CT);/*ɨ�迨*/
		status = PcdAnticoll(SN);/*����ײ*/
		if(status == 0)
		{
			LED0 = !LED0;
			Reset_RC522();
		}
		else
		{
			for(i = 0;i < 4;i++)
				SN[i] = 0;
		}
		//��ȡ����ֵ����������ֵ
		adcx=Get_Adc_Average(ADC_Channel_12,10);
		adcy=Get_Adc_Average(ADC_Channel_13,10);
		adcx = adcx + 365;//����
		tempy = adcy*200/4096;
		
//		//�ж�����λ��
//		if(tempy > 163)//������С�ٽ��
//		{
//			Resistance_Status = 1;	
//		}
//		else if(tempy < 110)//��������ٽ��
//		{
//			Resistance_Status = 2;	
//		}
//		else
//		{
//			Resistance_Status = 0;	
//		}
//
//		if(Resistance_Status == 0)//�����ڿɵ���Χ
//		{
//			if(Resistance > tempy + 4)//����Ҫ��С
//			{
//				SetBackward();				
//			}
//			else if(Resistance + 4 < tempy)//����Ҫ����
//			{
//				SetForward();		
//			}
//			else 
//			{
//				SetStop();	
//			}	
//		}
//		else if(Resistance_Status == 1)//������С�ٽ���쳣������Ҫ����
//		{
//			SetForward();	
//		}
//		else if(Resistance_Status == 2)//��������ٽ���쳣������Ҫ��С 
//		{
//			SetBackward();
//		}

		//�ƴ����ٶ�ֵ�Ķ�Ӧ��ϵ
		if(BikeSpeed < 400)
		{
			LedsData =	899 - BikeSpeed/2;
		}
		else 
		{
			LedsData = 700;	
		}
		SetLeds(LedsData);


		ReceBuf[0] = 0x04;
		ReceBuf[1] = 0x02;
		ReceBuf[2] = SN[0];
		ReceBuf[3] = SN[1];
		ReceBuf[4] = SN[2];
		ReceBuf[5] = SN[3];
		ReceBuf[6] = BikeSpeed >> 8;
		ReceBuf[7] = BikeSpeed;
		ReceBuf[8] = HeartSpeed;
		ReceBuf[9] = (u8)(adcx>>8);
		ReceBuf[10] = (u8)(adcx);
		ReceBuf[11] = ChargerStatus;
		ReceBuf[12] = BlueToothStatus;
		if(key_status == 1)//��������
		{
		  	ReceBuf[13] = 0;
		}
		else				 //�����ɿ�����
		{
		 	ReceBuf[13] = key_tran;
			key_tran = 0;
		}
		ReceBuf[14] = Resistance_Status;
		ReceBuf[15] = 0x05;
	}
}

