#ifndef _PY_CHASSIS_H
#define _PY_CHASSIS_H

#include <math.h>
#include "main.h"
#include "DR16.h"
#include "motor.h"
#include "PY_HigherControl.h"

typedef struct
{
	s16 RVx,Vx;//实际速度，设定速度
	s16 RVy,Vy;
	s16 AngleCompensate;//超前角度补偿
	float Angle;//超前角度设定
	float SAngle,RAngle;//遥控器给定角度，实际角度值
}SpinTop_t;

typedef struct
{
	float Vx,Vy,W;			//底盘移动
	float Pitch;				//云台pitch轴 单位 度
}Remote_t;

extern Remote_t Remote;
extern void TimberWolves_chassis_Handle(void);

#endif	//_PY_CHASSIS_H
