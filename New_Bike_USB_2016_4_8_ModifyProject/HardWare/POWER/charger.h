#ifndef CHARGER_H
#define CHARGER_H
#include "sys.h"

#define 	OPEN_CHARGER_STATUS 	0
#define 	CLOSE_CHARGER_STATUS 	1


void Charger_Init(void);//充电模块初始化
void Open_Charger(void);//打开充电模块开关
void Close_Charger(void);//关闭充电模块开关

#endif


