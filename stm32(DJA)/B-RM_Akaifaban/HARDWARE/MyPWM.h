#ifndef __MY_PWM_H
#define __MY_PWM_H


#include "main.h"


extern void MyPWM_Init(void);														//��ʼ��PWM

extern void FanPowerCtrl(uint8_t POWER);								//���POWERΪ0��ֱ���Զ��ر�ʹ�ܽ� POWER 0-100
extern void FanRollCtrl(int8_t Angle);									//-90 - 90
extern void VisionYawCtrl(int8_t Angle);								//-90 - 90

#endif	//__MY_PWM_H

