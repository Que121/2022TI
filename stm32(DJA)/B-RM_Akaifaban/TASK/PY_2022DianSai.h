#ifndef __RM_2022_Diansai
#define __RM_2022_Diansai

#include "main.h"

extern uint8_t TotalStep;

typedef struct
{
	double leftAng;
	double rightAng;
	float turnAng;
	
	double lastDis;
	double TargetDis;
	uint32_t lastGoTime;
	uint8_t YQY_GoShouldDo;																		//是否应该走
	uint8_t YQY_GoStep;																				//剩余步数
	double map[10][2];																				//路径库
	uint8_t directionFlag;
	uint32_t lastCtrlTime;
	
	uint8_t remoteSwitchLeft;		//记录遥控器上次的数值
}PY_2022_t;

extern PY_2022_t PY_2022;

void DianSai2022_Init(void);																//初始化需要的硬件

void SetMotorByPID_Position(int32_t left,int32_t right);		//设置电机位置
void SetMotorByPID_Speed   (int16_t left,int16_t right);		//设置电机速度

void Dian2022GetCtrlInfo(void);															//获取控制量
void Dian2022MotorCtrl_Handle(void);												//PID控制句柄，放到main.c
void Dian2022Ctrl_Handle(void);															//总控制句柄

void turnServoCtrl(int8_t Angle);
void ServoSpark(void);

void YQY_Go(double distance, float ang);										//行走距离，转弯角度		单次运行
void YQY_GoAddMap(double distance, float ang);							//行走距离，转弯角度		添加地图
void YQY_GoDecodeMap_Handle(void);													//解码地图
void YQY_Go_Handle(void);																		//行走句柄

void AutoRunHandle(void);																		//自动巡线
void AutoRunTotalHandle(void);															//总自动控制
void PowerOff(void);


#endif
