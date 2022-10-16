//
//···电机与变量命名如下：
//
//						  前方（W）
//			ID 7 	（吸盘）  		ID 8	
//				+-------------+
//				|							|
//				|							|
//	(A)		|		俯视图			|		(D)
//				|							|
//				|							|
//				+-------------+
//			ID 5              ID 6
//	伸出    ID 2			ID 1  （CAN2）
//							后方（S）
//
//高度统一，向上为正，向下为负。0位置为各个滑块行程中间位置。
//行程：W轴 S轴 269mm  L轴322mm
//



#include "PYlift.h" 
#include "motor.h"																//电机控制
#include "DR16.h"																	//遥控
#include "key.h"																	//限位开关
#include "PY_HigherControl.h"
#include "MyPWM.h"
#include "math.h"																	//三角函数

#define PI 										3.1415							//Π
#define PULLEY_S_d						27.02								//链轮直径 	mm  已实测调整
#define PULLEY_W_d						31.41								//皮带轮直径 mm	已实测调整
#define GAP_W_S								225.0									//前后抬升机构间距 mm
//#define ABS(x)						((x>0)?x:-x)					//绝对值

#define PY_limit(x,y,z)			{if(y>z)y=z;if(y<x)y=x;}


PY_Lift_t PY_Lift;																//抬升控制结构体

float Xe, Ye;														//吸盘pitch轴点
void GetRemote_AutoCtrl(void)
{
	float h1, h2, h3;																//后高 前高 伸出长度
	double Ang_1;																		//倾角
	static uint8_t IsInit = 1;											//用于为变量赋初值
	
	if(IsInit)																			//初始化 计算初值
	{
		IsInit = 0;
		Ang_1 = atan((double)(PY_Lift.height_W - PY_Lift.height_S)/GAP_W_S);
		h1 = PY_Lift.height_S;
		h2 = PY_Lift.height_W;
		h3 = PY_Lift.length_Flex;
		
		Xe = (h3 + 415) * cos(Ang_1);
		Ye = h1 + (h3 + 415) * sin(Ang_1);
	}
	else																						//自动解算
	{
		Xe += remote_control.ch3 / 660.0 * 2;				//获取控制量
		Ye += remote_control.ch4 / 660.0 * 2;			
		
		h1 = -0.5 * Xe + 400;													//解算高度 这里的公式可自由更换
		Ang_1 = atan((Ye - h1)/Xe);
		h2 = 220/tan(Ang_1) + h1;
		h3 = sqrtf(Xe*Xe + (Ye - h1) * (Ye - h1)) + 45;
		
		PY_limit(94,h1,437);													//限位
		PY_limit(94,h2,437);
		PY_limit(13,h3,383);
		
		PY_Lift.height_S 		= h1;											//传递 解算结果
		PY_Lift.height_W 		= h2;
		PY_Lift.length_Flex = h3;
	}
}

void PY_Lift_GetRemote(void)
{
	if(HAL_GetTick() - PY_Lift.lastCtrlTime > 100)
	{
		if(remote_control.switch_left == Switch_Middle)	//处于升降控制模式下
		{
//			if((PY_Lift.calibrationStatus & (1 << 2)) == 0)	//校准成功
//			{
//				GetRemote_AutoCtrl();													//自动解算高度
//			}
//			else
//			{
				PY_Lift.lastCtrlTime = HAL_GetTick();					//更新记录时间
				PY_Lift.height_W 		+= remote_control.ch2 / 660.0 * 30;
				PY_Lift.height_S 		+= remote_control.ch4 / 660.0 * 30;
				PY_Lift.length_Flex += remote_control.ch3 / 660.0 * 15;
//			}
			PY_Lift.angle_Pitch += remote_control.ch1 / 660.0 * 3;
		}
		if(remote_control.switch_left == Switch_Up)
		{
			PY_Lift.angle_FanRoll += (remote_control.ch1 / 660.0 * 0.4);												//风机Roll
			if(remote_control.ch1 >  655)PY_Lift.angle_FanRoll =  90;
			if(remote_control.ch1 < -655)PY_Lift.angle_FanRoll = -90;
			
			if(remote_control.ch2 >= 0)PY_Lift.FanPower += (remote_control.ch2 / 660.0 * 1.0);		//风机风量
			else PY_Lift.FanPower = 10;
		}
		
		if((PY_Lift.calibrationStatus & 0x01) == 0)
			PY_limit(LIFT_HEIGHT_W_MIN, PY_Lift.height_W, 		LIFT_HEIGHT_W_MAX);
		if((PY_Lift.calibrationStatus & 0x02) == 0)
			PY_limit(LIFT_HEIGHT_S_MIN, PY_Lift.height_S, 		LIFT_HEIGHT_S_MAX);
		PY_limit(FLEX_LENGTH_W_MIN, PY_Lift.length_Flex, 	FLEX_LENGTH_W_MAX);
		
		PY_limit(-90, PY_Lift.angle_FanRoll, 	90);
		PY_limit(0, PY_Lift.FanPower, 	100);
		
		PY_Lift.angle_W 		= (int32_t)(PY_Lift.height_W 		/ (PULLEY_W_d * PI) * 360.0);
		PY_Lift.angle_S 		= (int32_t)(PY_Lift.height_S 		/ (PULLEY_S_d * PI) * 360.0);
		PY_Lift.angle_Flex 	= (int32_t)(PY_Lift.length_Flex / (PULLEY_S_d * PI) * 360.0);
		
		if((PY_Lift.calibrationStatus & (1 << 2)) == 0)	//校准成功，实时更新倾角
		{
			//printf("ok %f ",atan(1));
			PY_Lift.offset_A = 90 - (int32_t)((atan((double)(PY_Lift.height_W - PY_Lift.height_S)/GAP_W_S) / PI)*180);
		}
//		printf("roll:%.2f pow:%.2f\n",PY_Lift.angle_FanRoll,PY_Lift.FanPower);
//    printf("H:  %.2f  %.2f\n", PY_Lift.height_S, PY_Lift.height_W);
//		printf("Ang:  %d  %d sta %d ",  PY_Lift.angle_S + PY_Lift.offset_S, PY_Lift.angle_W + PY_Lift.offset_W, PY_Lift.calibrationStatus);
//		printf("A:  %d  %d\n", PY_Lift.angle_Pitch, PY_Lift.angle_Pitch + PY_Lift.offset_A);
	}
}

void PY_Lift_Calibrate()													//校准
{
	if(HAL_GetTick() - PY_Lift.lastCalibrateTime > 1500)
	{
		if((LIMIT_IS_PRESSED_AS || LIMIT_IS_PRESSED_DS) && ((PY_Lift.calibrationStatus & 0x02) != 0))	//S轴校准
		{
			ledCtrl('8',0);
			PY_Lift.calibrationStatus &= ~0x02;
			PY_Lift.offset_S = PY_Lift.angle_S;
			PY_Lift.height_S = LIFT_HEIGHT_S_MIN;
			PY_Lift.lastCalibrateTime = HAL_GetTick();
			PY_Lift.angle_S 		= (int32_t)(PY_Lift.height_S 		/ (PULLEY_S_d * PI) * 360.0);
			printf("校准后电机\n");
		}
		if((LIMIT_IS_PRESSED_AW || LIMIT_IS_PRESSED_DW) && ((PY_Lift.calibrationStatus & 0x01) != 0))	//W轴校准
		{
			ledCtrl('7',0);
			PY_Lift.calibrationStatus &= ~(1 << 0);
			PY_Lift.offset_W = (int32_t)((PY_Lift.height_W - LIFT_HEIGHT_W_MAX) / (PULLEY_W_d * PI) * 360.0);
			PY_Lift.height_W = LIFT_HEIGHT_W_MAX;
			PY_Lift.lastCalibrateTime = HAL_GetTick();
			PY_Lift.angle_W 		= (int32_t)(PY_Lift.height_W 		/ (PULLEY_W_d * PI) * 360.0);
			printf("校准前电机\n");
		}
		if(((PY_Lift.calibrationStatus & 0x03) == 0) && ((PY_Lift.calibrationStatus & (1<<2)) != 0))	//校准完成
		{
			PY_Lift.calibrationStatus &= ~(1 << 2);						//防止重复校准。
			PY_Lift.offset_A = 90 - (int32_t)((atan((double)(PY_Lift.height_W - PY_Lift.height_S)/GAP_W_S) / PI)*180);
			PY_Lift.angle_Pitch -= PY_Lift.offset_A;
		}
		
	}
	
}

void PY_Lift_Init(void)														//初始化
{
	PY_Lift.height_S = 0;
	PY_Lift.height_W = 0;
	PY_Lift.lastCtrlTime = 0;
	PY_Lift.angle_S = 0;
	PY_Lift.angle_W = 0;
	PY_Lift.length_Flex = 0;
	PY_Lift.angle_Flex = 0;
	PY_Lift.angle_Pitch = 0;
	
	PY_Lift.offset_A = 0;													//偏移量清零(角度)
	PY_Lift.offset_L = 0;													//偏移量清零
	PY_Lift.offset_W = 0;													//偏移量清零
	PY_Lift.offset_S = 0;													//偏移量清零
	
	PY_Lift.calibrationStatus = 0xff;							//全部放1，校准成功 按bit清零
	PY_Lift.lastCalibrateTime = HAL_GetTick();		//上次校准时间
	
	ledCtrl('7',1);
	ledCtrl('8',1);
	
	PY_Lift.angle_FanRoll = 0;										//吸盘Roll
	PY_Lift.angle_VisionYaw = 0;									//视觉云台Yaw轴
	PY_Lift.FanPower = 10;												//风机风量
}

void PWM_Ctrl(void)
{
	FanRollCtrl((int8_t)PY_Lift.angle_FanRoll);		//控制舵机
	FanPowerCtrl((uint8_t)PY_Lift.FanPower);
}
void PY_Lift_Handle(void)														//运行句柄，定期执行！
{
	if(remote_control.switch_right == Switch_Down)
	{
		PY_PowerOff();
	}
	else if(remote_control.switch_right == Switch_Middle)
	{
		PY_Lift_Calibrate();														//执行校准
		PY_Lift_GetRemote();														//获取控制量
		
		PWM_Ctrl();																			//控制舵机 和 风机
		
		SetLiftMotorByPID_Angle( -(PY_Lift.angle_S + PY_Lift.offset_S),\
															 PY_Lift.angle_S + PY_Lift.offset_S,\
															 PY_Lift.angle_W + PY_Lift.offset_W,\
														 -(PY_Lift.angle_W + PY_Lift.offset_W));			//设置 串级PID 目标值（抬升电机）
		
		LiftMOTOR_PID_Control_Angle();									//进行PID计算，控制抬升电机
		
		SetFlexMotorByPID_Angle(	PY_Lift.angle_Flex  + PY_Lift.offset_L,\
														 -(PY_Lift.angle_Flex 	+ PY_Lift.offset_L),\
															-(PY_Lift.angle_Pitch + PY_Lift.offset_A));
		
		FlexMOTOR_PID_Control_Angle();									//进行PID计算，控制伸缩电机
	}
}


