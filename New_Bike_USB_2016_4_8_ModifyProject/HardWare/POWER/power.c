#include "sys.h"
#include "power.h"
#include "delay.h"

u8	PowerDir = 0;//0:ֹͣ��1����ǰ��2�����

void Power_Init(u16 arr,u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);  //ʹ��GPIO����ʱ��ʹ��
	                                                                     	

   //���ø�����Ϊ�����������,���TIM3 CH3��PWM���岨��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; //TIM5_CH1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 80K
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  ����Ƶ
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_Pulse = 0; //���ô�װ�벶��ȽϼĴ���������ֵ
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
	TIM_OC2Init(TIM5, &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx

	TIM_Cmd(TIM5, ENABLE);  //ʹ��TIM5

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  	//ʹ��GPIO����ʱ��ʹ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9; 	//TIM5_CH1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  		//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA,GPIO_Pin_8);
	GPIO_SetBits(GPIOA,GPIO_Pin_9);
}


//���õ������ѹֵ�ô�С
void SetPower(u16 voltage)
{
 	TIM_SetCompare2(TIM5,voltage);
}

void SetForward(void)
{
	if(PowerDir == 2)
	{
		DIR0 = 0;
		delay_ms(50);
		DIR1 = 1;	
	}
	else
	{
		DIR0 = 0;
		DIR1 = 1;
	}
	PowerDir = 1;
}

void SetBackward(void)
{
	if(PowerDir == 1)
	{
	  	DIR0 = 1;
		delay_ms(50);
		DIR1 = 0;
	}
	else
	{
	  	DIR0 = 1;
		DIR1 = 0;
	}	
	PowerDir = 2;		
}

void SetStop(void)
{
	DIR0 = 0;
	DIR1 = 0;
	PowerDir = 0;
}

