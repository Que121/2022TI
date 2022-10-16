#ifndef __PY_LIFT_H
#define __PY_LIFT_H

#include "main.h"																//内包含#include "stm32f4xx_hal.h"

#define REMOTE_REFRESH_TIME				100						//变量更新频率10Hz
#define LIFT_HEIGHT_S_MAX						437.5				//后电机最高
#define LIFT_HEIGHT_S_MIN					 	94					//后电机最低
#define LIFT_HEIGHT_W_MAX						383.0				//前电机最高
#define LIFT_HEIGHT_W_MIN					 	12.5				//前电机最低
#define FLEX_LENGTH_W_MAX						440.0				//中电机最高
#define FLEX_LENGTH_W_MIN					 	0						//中电机最低

typedef struct
{
	double height_S;
	double height_W;
	double length_Flex;														//伸出长度
	int32_t angle_S;
	int32_t angle_W;
	int32_t angle_Flex;														//伸出角度
	int32_t angle_Pitch;													//Pitch翻转角度
	float angle_FanRoll;													//吸盘Roll
	int16_t angle_VisionYaw;											//视觉云台Yaw轴
	
	float FanPower;																//吸盘风力
	
	int32_t offset_S;															//偏移
	int32_t offset_W;															//偏移
	int32_t offset_L;															//偏移
	int32_t offset_A;															//偏移（角度）
	uint8_t calibrationStatus;										//位0 W轴校准完成 位1 S轴校准完成 位2 全部校准完成
	
	uint32_t lastCtrlTime;												//记录上次变量改变的时间
	uint32_t lastCalibrateTime;										//记录上次校准时间
}PY_Lift_t;


extern PY_Lift_t PY_Lift;												//抬升控制结构体

extern void PY_Lift_Init(void);									//初始化
extern void PY_Lift_Handle(void);								//运行句柄，定期执行！

#endif //__PY_LIFT_H

