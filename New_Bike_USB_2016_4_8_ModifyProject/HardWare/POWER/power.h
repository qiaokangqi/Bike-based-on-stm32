#ifndef POWER_H
#define POWER_H
#include "sys.h"

#define DIR0 PAout(8)	// PB9
#define DIR1 PAout(9)	// PB8

void SetPower(u16 voltage);//设置阻力调节速度
void Power_Init(u16 arr,u16 psc);
void SetForward(void);//阻力调大
void SetBackward(void);//阻力调小
void SetStop(void);//停止

#endif

