#ifndef HEART_H
#define HEART_H
#include "sys.h"

void Heart_Init(u16 arr,u16 psc);//心率，速度模块的初始化
void TIM2_Init(u16 arr,u16 psc);//定时器“线程”初始化，注意不可做超时的工作

#endif



