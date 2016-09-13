#include "sys.h"
#include "heart.h"
#include "led.h"

#define TMP_SPEED_NODE   	300			//��ɢ�ٶ��ٽ�ڵ�
#define MAX_MILE_DATA		5000000		//�����ɢ�������		


u16 TimerCount = 0;

u16 BikeSpeed = 0; 			//�ٶ�ֵ
u16 TmpBikeSpeed = 0;		//�ٶȱ仯ֵ
u8 IsSpeedStart = 0;		//�ٶȱ仯��־λ 0��stop״̬��1������״̬��2������״̬
u32 SpeedDataTime = 0; 		//ʱ��ɼ�����ڶ�ʱ���������
u16 StartSpeedTime = 0;		//�ٶȲɼ���ʼʱ���
u16 EndSpeedTime = 0;		//�ٶȲɼ���ֹʱ���	
u32 BikeTime = 0;			//�ٶȲɼ�ʱ����
u32 BikeTimeOld = 0;		//��һ���ٶȲɼ�ʱ������Ϊ�ٶȱ仯����״̬���ж�
u16 BikeCount = 0;			//�ٶȼ�������
u8 SpeedChangeStatus = 0;	//�ٶȱ仯����״̬ 0�������ٶȣ�1��δ�����ٶ�
u16 TIM_Count = 0;


u16 HeartSpeed = 0;
u16 TmpHeartSpeed = 0;
u8 IsHeartStart = 0;
u32 HeartDataTime = 0;
u16 StartHeartTime = 0;		//���ʲɼ���ʼʱ���
u16 EndHeartTime = 0;		//���ʲɼ���ֹʱ���
u32 HeartTime = 0;			//���ʲɼ�ʱ����
u32 HeartTimeOld = 0;
u16 HeartCount = 0;
u8 HeartChangeStatus = 0;	//���ʱ仯����״̬ 0�������ٶȣ�1��δ�����ٶ� 


TIM_ICInitTypeDef  TIM3_ICInitStructure;
void Heart_Init(u16 arr,u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	//ʹ��TIM3ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOA,ENABLE);	//ʹ��PB�˿�ʱ��

	//PB0 ���ʲ�׽����
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0|GPIO_Pin_1;  						//PB0 ���֮ǰ����  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 								//PB0 ����  
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	//��ʼ����ʱ��3 TIM3	 
	TIM_TimeBaseStructure.TIM_Period = arr; 	//�趨�������Զ���װֵ 
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 	//Ԥ��Ƶ��   
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	
	//��ʼ��TIM3���벶�����
	TIM3_ICInitStructure.TIM_Channel = TIM_Channel_3; //CC1S=03 	ѡ������� IC1ӳ�䵽TI1��
  	TIM3_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//�����ز���
  	TIM3_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //ӳ�䵽TI1��
  	TIM3_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //���������Ƶ,����Ƶ 
  	TIM3_ICInitStructure.TIM_ICFilter = 0x11;					//IC1F=0000 ���������˲��� ���˲�
  	TIM_ICInit(TIM3, &TIM3_ICInitStructure);

	//��ʼ��TIM3���벶�����
	TIM3_ICInitStructure.TIM_Channel = TIM_Channel_4; //CC1S=03 	ѡ������� IC1ӳ�䵽TI1��
  	TIM3_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//�����ز���
  	TIM3_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //ӳ�䵽TI1��
  	TIM3_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //���������Ƶ,����Ƶ 
  	TIM3_ICInitStructure.TIM_ICFilter = 0x11;					//IC1F=0000 ���������˲��� ���˲�
  	TIM_ICInit(TIM3, &TIM3_ICInitStructure);

	//�жϷ����ʼ��
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM2�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //��ռ���ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //�����ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ��� 
	
	TIM_ITConfig(TIM3,TIM_IT_Update|TIM_IT_CC3|TIM_IT_CC4,ENABLE);//��������ж� ,����CC1IE�����ж�	
	
  	TIM_Cmd(TIM3,ENABLE ); 	//ʹ�ܶ�ʱ��3

}

u8  TIM3CH3_CAPTURE_STA=0;	//���벶��״̬		    				
u16	TIM3CH3_CAPTURE_VAL=0;	//���벶��ֵ

void TIM2_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //ʱ��ʹ��

	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 ������5000Ϊ500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  10Khz�ļ���Ƶ��  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(  //ʹ�ܻ���ʧ��ָ����TIM�ж�
		TIM2, //TIM2
		TIM_IT_Update ,
		ENABLE  //ʹ��
		);
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

	TIM_Cmd(TIM2, ENABLE);  //ʹ��TIMx����
}

void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
	{
		//����ƽ��
		TimerCount++;
		if(TimerCount >= 10)
		{
			TimerCount = 0;
			if(HeartSpeed > TmpHeartSpeed)
			{
				if(HeartSpeed > 2)
				{
					HeartSpeed = HeartSpeed - 2;	
				}
				else
				{
				 	HeartSpeed = 0;	
				}	
			}
			else if(HeartSpeed < TmpHeartSpeed)
			{
				HeartSpeed = HeartSpeed + 2;	
			}
		}

		if(HeartChangeStatus == 1)
		{
			HeartChangeStatus = 0;
			HeartTime = HeartDataTime;
			HeartDataTime = 0;
			StartHeartTime = EndHeartTime;
			TmpHeartSpeed = 850000/HeartTime;
			if(TmpHeartSpeed > 120)
				TmpHeartSpeed = (TmpHeartSpeed - 120)/4 + 120; 	
		}


		TIM_Count++;
		if(TIM_Count > 10)
		{
			TIM_Count = 0;
			//�ٶ�ƽ��
			if(BikeSpeed > TmpBikeSpeed)
			{
				if(BikeSpeed > 7)
				{
					BikeSpeed = BikeSpeed - 7;
				}
				else
				{
				 	BikeSpeed = 0;	
				}	
			}
			else if(BikeSpeed < TmpBikeSpeed)
			{
				BikeSpeed = BikeSpeed + 4;	
			}
		}
		
		if(SpeedChangeStatus == 1)
		{
			SpeedChangeStatus = 0;
			BikeTime = SpeedDataTime*5;
			SpeedDataTime = 0;
			StartSpeedTime = EndSpeedTime;
			TmpBikeSpeed = MAX_MILE_DATA/BikeTime;
			if(TmpBikeSpeed > TMP_SPEED_NODE)
				TmpBikeSpeed = TMP_SPEED_NODE + (TmpBikeSpeed - TMP_SPEED_NODE)/6;		
		}
		 
	}
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);  //���TIMx���жϴ�����λ:TIM �ж�Դ	
}


void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
	{

		if(IsHeartStart == 0)//����ֹͣ״̬
		{
			HeartDataTime = 0;
			HeartTime = 0;
			HeartTimeOld = 0;
			TmpHeartSpeed = 0;	
		}
		else if(IsHeartStart == 1)//��������״̬
		{
			HeartCount++;
			if(HeartCount > 20000)//1s
			{
				IsHeartStart = 0;	
			}
			HeartDataTime++;	
		}
		else if(IsHeartStart == 2)//��������״̬
		{
			HeartCount++;
			if(HeartCount > 20000)//1s
			{
				IsHeartStart = 0;	
			}
			HeartDataTime++;		
		}

		if(IsSpeedStart	== 0)//ֹͣ״̬
		{
			SpeedDataTime = 0;
			BikeTime = 0;
			BikeTimeOld = 0;
			TmpBikeSpeed = 0;		
		}
		else if(IsSpeedStart == 1)//����״̬
		{
			BikeCount++;
			if(BikeCount > 10000)//��ʱ1s��ֹͣ״̬
			{
				IsSpeedStart = 0;	
			}
			SpeedDataTime++;
		}
		else if(IsSpeedStart == 2)//����״̬
		{
			BikeCount++;
			if(BikeCount > 10000)//��ʱ2s��ֹͣ״̬
			{
				IsSpeedStart = 0;	
			}
			SpeedDataTime++;	
		}

	}

	if(TIM_GetITStatus(TIM3, TIM_IT_CC3) != RESET)
	{
		HeartCount = 0;
		LED0 = !LED0;
		if(IsHeartStart == 0)//����ֹͣ״̬
		{
		 	IsHeartStart = 1;
			StartHeartTime = TIM_GetCapture3(TIM3);
		}
		else if(IsHeartStart == 1)
		{
			IsHeartStart = 2;
			EndHeartTime = TIM_GetCapture3(TIM3);
			if(EndHeartTime > StartHeartTime)
			{
				HeartTimeOld = EndHeartTime - StartHeartTime;
			}
			else if(EndHeartTime <= StartHeartTime)	
			{
				HeartTimeOld = StartHeartTime - EndHeartTime;	 	
			}
			HeartChangeStatus = 1;	
		}
		else if(IsHeartStart == 2)
		{
			EndHeartTime = TIM_GetCapture3(TIM3);
			if(EndHeartTime > StartHeartTime)
			{
				HeartTimeOld = EndHeartTime - StartHeartTime;
			}
			else if(EndHeartTime <= StartHeartTime)	
			{
				HeartTimeOld = StartHeartTime - EndHeartTime;	 	
			}
			HeartChangeStatus = 1;		
		}		
	}

	if(TIM_GetITStatus(TIM3, TIM_IT_CC4) != RESET)
	{
		BikeCount = 0;
	   	if(IsSpeedStart == 0)//ֹͣ״̬
		{
			IsSpeedStart = 1;
			StartSpeedTime = TIM_GetCapture4(TIM3);		
		}
		else if(IsSpeedStart == 1)//����״̬
		{
			IsSpeedStart = 2;
			EndSpeedTime = TIM_GetCapture4(TIM3);
			if(EndSpeedTime > StartSpeedTime)
			{
				BikeTimeOld = EndSpeedTime - StartSpeedTime;
			}
			else if(EndSpeedTime <= StartSpeedTime)	
			{
				BikeTimeOld = StartSpeedTime - EndSpeedTime;	 	
			}
			SpeedChangeStatus = 1;	
		}
		else if(IsSpeedStart == 2)//����״̬
		{
			EndSpeedTime = TIM_GetCapture4(TIM3);
			if(EndSpeedTime > StartSpeedTime)
			{
				BikeTimeOld = EndSpeedTime - StartSpeedTime;
			}
			else if(EndSpeedTime <= StartSpeedTime)	
			{
				BikeTimeOld = StartSpeedTime - EndSpeedTime;	 	
			}
			SpeedChangeStatus = 1;
		} 			
	}
    TIM_ClearITPendingBit(TIM3, TIM_IT_CC3|TIM_IT_CC4|TIM_IT_Update); //����жϱ�־λ 	
}

