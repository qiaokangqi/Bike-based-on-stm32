#include "key.h"
#include "delay.h"

u16 KeyStatus = 0;

void KEY_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_AFIO,ENABLE);//使能PORTC时钟
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;//PC10-->KEY1,PC11-->KEY2,PC12-->KEY3,PC13-->KEY4,PC14-->KEY5,PC15-->KEY6
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //设置成下拉输入
 	GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化GPIOC

}

u8 KEY_Scan(u8 mode)
{
	static u8 key_up=1;//按键按松开标志
	if(mode)key_up=1;
	if(key_up&&(KEY1==1||KEY2==1||KEY3==1||KEY4==1||KEY5==1||KEY6==1))
	{
		delay_ms(5);
		key_up=0;
		if(KEY1==1){KeyStatus = 1;return KEY1_PRES;}
		else if(KEY2==1) {KeyStatus = 2;return KEY2_PRES;}
		else if(KEY3==1) {KeyStatus = 3;return KEY3_PRES;}
		else if(KEY4==1) {KeyStatus = 4;return KEY4_PRES;}
		else if(KEY5==1) {KeyStatus = 5;return KEY5_PRES;}
		else if(KEY6==1) {KeyStatus = 6;return KEY6_PRES;}  
	}
	else if(KEY1==0&&KEY2==0&&KEY3==0&&KEY4==0&&KEY5==0&&KEY6==0)
	{
		KeyStatus = 0;
		key_up=1;
	}
	return 0;//无按键按下
}
