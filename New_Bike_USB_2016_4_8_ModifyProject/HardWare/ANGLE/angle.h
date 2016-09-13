#ifndef ANGLE_H
#define ANGLE_H
#include "sys.h"

void Adc_Init(void);//方向和阻力调节采样模块初始化
u16  Get_Adc(u8 ch);//获取单次采样值 
u16 Get_Adc_Average(u8 ch,u8 times);//获取指定次数平均采样值

#endif


