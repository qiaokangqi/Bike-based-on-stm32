#include "sys.h"
#include "heart.h"
#include "led.h"

#define TMP_SPEED_NODE   	300			//离散速度临界节点
#define MAX_MILE_DATA		5000000		//最大离散里程数据		


u16 TimerCount = 0;

u16 BikeSpeed = 0; 			//速度值
u16 TmpBikeSpeed = 0;		//速度变化值
u8 IsSpeedStart = 0;		//速度变化标志位 0：stop状态，1：启动状态，2：运行状态
u32 SpeedDataTime = 0; 		//时间采集间隔内定时器溢出次数
u16 StartSpeedTime = 0;		//速度采集起始时间点
u16 EndSpeedTime = 0;		//速度采集截止时间点	
u32 BikeTime = 0;			//速度采集时间间隔
u32 BikeTimeOld = 0;		//上一次速度采集时间间隔，为速度变化趋势状态做判断
u16 BikeCount = 0;			//速度监测计数器
u8 SpeedChangeStatus = 0;	//速度变化趋势状态 0：捕获速度，1：未捕获速度
u16 TIM_Count = 0;


u16 HeartSpeed = 0;
u16 TmpHeartSpeed = 0;
u8 IsHeartStart = 0;
u32 HeartDataTime = 0;
u16 StartHeartTime = 0;		//心率采集起始时间点
u16 EndHeartTime = 0;		//心率采集截止时间点
u32 HeartTime = 0;			//心率采集时间间隔
u32 HeartTimeOld = 0;
u16 HeartCount = 0;
u8 HeartChangeStatus = 0;	//心率变化趋势状态 0：捕获速度，1：未捕获速度 


TIM_ICInitTypeDef  TIM3_ICInitStructure;
void Heart_Init(u16 arr,u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	//使能TIM3时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOA,ENABLE);	//使能PB端口时钟

	//PB0 心率捕捉引脚
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0|GPIO_Pin_1;  						//PB0 清除之前设置  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 								//PB0 输入  
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	//初始化定时器3 TIM3	 
	TIM_TimeBaseStructure.TIM_Period = arr; 	//设定计数器自动重装值 
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 	//预分频器   
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	
	//初始化TIM3输入捕获参数
	TIM3_ICInitStructure.TIM_Channel = TIM_Channel_3; //CC1S=03 	选择输入端 IC1映射到TI1上
  	TIM3_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//上升沿捕获
  	TIM3_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //映射到TI1上
  	TIM3_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //配置输入分频,不分频 
  	TIM3_ICInitStructure.TIM_ICFilter = 0x11;					//IC1F=0000 配置输入滤波器 不滤波
  	TIM_ICInit(TIM3, &TIM3_ICInitStructure);

	//初始化TIM3输入捕获参数
	TIM3_ICInitStructure.TIM_Channel = TIM_Channel_4; //CC1S=03 	选择输入端 IC1映射到TI1上
  	TIM3_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//上升沿捕获
  	TIM3_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //映射到TI1上
  	TIM3_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //配置输入分频,不分频 
  	TIM3_ICInitStructure.TIM_ICFilter = 0x11;					//IC1F=0000 配置输入滤波器 不滤波
  	TIM_ICInit(TIM3, &TIM3_ICInitStructure);

	//中断分组初始化
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM2中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //先占优先级2级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //从优先级0级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器 
	
	TIM_ITConfig(TIM3,TIM_IT_Update|TIM_IT_CC3|TIM_IT_CC4,ENABLE);//允许更新中断 ,允许CC1IE捕获中断	
	
  	TIM_Cmd(TIM3,ENABLE ); 	//使能定时器3

}

u8  TIM3CH3_CAPTURE_STA=0;	//输入捕获状态		    				
u16	TIM3CH3_CAPTURE_VAL=0;	//输入捕获值

void TIM2_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //时钟使能

	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到5000为500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(  //使能或者失能指定的TIM中断
		TIM2, //TIM2
		TIM_IT_Update ,
		ENABLE  //使能
		);
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

	TIM_Cmd(TIM2, ENABLE);  //使能TIMx外设
}

void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
	{
		//心率平滑
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
			//速度平滑
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
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);  //清除TIMx的中断待处理位:TIM 中断源	
}


void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
	{

		if(IsHeartStart == 0)//心率停止状态
		{
			HeartDataTime = 0;
			HeartTime = 0;
			HeartTimeOld = 0;
			TmpHeartSpeed = 0;	
		}
		else if(IsHeartStart == 1)//心率启动状态
		{
			HeartCount++;
			if(HeartCount > 20000)//1s
			{
				IsHeartStart = 0;	
			}
			HeartDataTime++;	
		}
		else if(IsHeartStart == 2)//心率运行状态
		{
			HeartCount++;
			if(HeartCount > 20000)//1s
			{
				IsHeartStart = 0;	
			}
			HeartDataTime++;		
		}

		if(IsSpeedStart	== 0)//停止状态
		{
			SpeedDataTime = 0;
			BikeTime = 0;
			BikeTimeOld = 0;
			TmpBikeSpeed = 0;		
		}
		else if(IsSpeedStart == 1)//启动状态
		{
			BikeCount++;
			if(BikeCount > 10000)//超时1s，停止状态
			{
				IsSpeedStart = 0;	
			}
			SpeedDataTime++;
		}
		else if(IsSpeedStart == 2)//运行状态
		{
			BikeCount++;
			if(BikeCount > 10000)//超时2s，停止状态
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
		if(IsHeartStart == 0)//心率停止状态
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
	   	if(IsSpeedStart == 0)//停止状态
		{
			IsSpeedStart = 1;
			StartSpeedTime = TIM_GetCapture4(TIM3);		
		}
		else if(IsSpeedStart == 1)//启动状态
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
		else if(IsSpeedStart == 2)//运行状态
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
    TIM_ClearITPendingBit(TIM3, TIM_IT_CC3|TIM_IT_CC4|TIM_IT_Update); //清除中断标志位 	
}

