#include "sys.h"
#include "power.h"
#include "delay.h"

u8	PowerDir = 0;//0:停止，1：向前，2：向后

void Power_Init(u16 arr,u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);  //使能GPIO外设时钟使能
	                                                                     	

   //设置该引脚为复用输出功能,输出TIM3 CH3的PWM脉冲波形
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; //TIM5_CH1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 80K
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值  不分频
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_Pulse = 0; //设置待装入捕获比较寄存器的脉冲值
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	TIM_OC2Init(TIM5, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx

	TIM_Cmd(TIM5, ENABLE);  //使能TIM5

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  	//使能GPIO外设时钟使能
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9; 	//TIM5_CH1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  		//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA,GPIO_Pin_8);
	GPIO_SetBits(GPIOA,GPIO_Pin_9);
}


//设置电磁铁电压值得大小
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

