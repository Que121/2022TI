#ifndef _PY_HIGHER_CONTROL_H
#define _PY_HIGHER_CONTROL_H

#include <math.h>
#include "main.h"
#include "DR16.h"
#include "motor.h"



/*   内部函数   */

void VisionMotorCtrl_Handle(void);		//图传云台控制逻辑
void hydrocooling_Handle(void);				//水冷控制(如需要更复杂的逻辑可在这里添加！)
void carBackingRadar_Handle(void);		//倒车雷达 在行进模式关闭


/*   外部函数   */

extern void PY_PowerOff(void);						//立即关闭所有输出！
extern void SafeLock(void);								//安全锁
extern void HigherCtrl_Hnadle(void);			//高级控制句柄，内部轮流调用上面三个函数，请放到main.c里面

#endif	//_PY_HIGHER_CONTROL_H


