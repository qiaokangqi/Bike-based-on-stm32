#ifndef POWER_H
#define POWER_H
#include "sys.h"

#define DIR0 PAout(8)	// PB9
#define DIR1 PAout(9)	// PB8

void SetPower(u16 voltage);//�������������ٶ�
void Power_Init(u16 arr,u16 psc);
void SetForward(void);//��������
void SetBackward(void);//������С
void SetStop(void);//ֹͣ

#endif

