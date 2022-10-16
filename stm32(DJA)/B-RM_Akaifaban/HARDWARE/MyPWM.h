#ifndef __MY_PWM_H
#define __MY_PWM_H


#include "main.h"


extern void MyPWM_Init(void);														//初始化PWM

extern void FanPowerCtrl(uint8_t POWER);								//如果POWER为0，直接自动关闭使能脚 POWER 0-100
extern void FanRollCtrl(int8_t Angle);									//-90 - 90
extern void VisionYawCtrl(int8_t Angle);								//-90 - 90

#endif	//__MY_PWM_H

