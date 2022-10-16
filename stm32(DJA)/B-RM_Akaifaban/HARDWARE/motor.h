/*
说明：
此文件为电机控制顶层文件，内链底层 bsp_can和PY_PID.c

*/
#ifndef __motor_h
#define __motor_h

#include "main.h"																//内包含#include "stm32f4xx_hal.h"
#include "PY_PID.h"															//包含PID底层算法
#include "bsp_can.h"														//包含电机底层控制接口
#include "MyConfig.h"														//包含所有可配置的东东！


typedef struct {
	enum
	{
		speed = 0,
		position = 1,
		stop = 2
	}PIDmode;					//电机工作模式 可选择 0 速度模式  1 位置模式  2 停止工作模式
	enum
	{
		DJI_3508 = 0,
		DJI_2006 = 1,
		DJI_6020 = 2,
		//...请自行添加
	}MotorType;					//电机型号
	
	struct{
		int16_t	 	speed_rpm;		//电机转速，即每分钟转几圈
		float  		real_current;	//通过CAN线读取到的电机电流
		int16_t  	given_current;//我们上次调用电机电流设置函数所给定的电流（未实现）
		uint16_t 	angle;				//无刷电机转子绝对角度（本次值），范围:[0,8191]映射到0-360度
		uint16_t 	last_angle;		//无刷电机转子绝对角度（上次值），范围:[0,8191]映射到0-360度
		//uint8_t  	hall;					//不知道干啥用的
		uint16_t	offset_angle;	//待完成
		int32_t		round_cnt;		//待完成
		double		total_angle;	//待完成
	}measure;
	
	PID_TypeDef PID_speed;		//PID结构体，在PY-PID.h中
	PID_TypeDef PID_position;	//PID结构体，在PY-PID.h中
}motor_Typedef;

void ChassisMOTOR_PID_Control(void);						//计算底盘PID，已经包含在MOTOR_PID_Cal()函数内
void GimbalMOTOR_PID_Control(void);							//计算云台PID

extern motor_Typedef ChassisMotor[4];						//底盘电机*4
extern motor_Typedef LiftMotor[4];							//抬升电机四个
extern motor_Typedef FlexMotor[2];							//伸缩电机两个
extern motor_Typedef PitchMotor;								//Pitch轴翻转电机（吸盘）

extern PID_TypeDef GimbalMotorPIDspeed[2];			//云台电机*2
extern PID_TypeDef	GimbalMotorPIDposition[2];	//云台电机位置环*2 串级PID

extern void MOTOR_Init(void);										//初始化电机
extern void MOTOR_PID_Cal(void);								//重新计算所有PID输出（需移植，需定时执行来更新电机速度）
extern void SetChassisMotorByPID(int16_t v1,int16_t v2,int16_t v3,int16_t v4);
extern void SetChassisMotorByPID_Angle(int16_t a1,int16_t a2,int16_t a3,int16_t a4);		//测试功能，底盘电机角度环
extern void SetLiftMotorByPID_Angle(int16_t a1,int16_t a2,int16_t a3,int16_t a4);				//抬升电机
extern void LiftMOTOR_PID_Control_Angle(void);																					//抬升电机
extern void FlexMOTOR_PID_Control_Angle(void);																					//伸缩电机
extern void SetFlexMotorByPID_Angle(int16_t a1,int16_t a2, int16_t pitch);							//伸缩电机
extern void SetChassisMotorByPID_Angle(int16_t a1,int16_t a2,int16_t a3,int16_t a4);
#endif      //__motor_h
