/*
 * PY_PID.h
 *
 *  Created on: 2022年5月16日
 *      Author: 21154
 */

#ifndef PY_PID_H_
#define PY_PID_H_

#include "main.h"
#include "MyConfig.h"						//包含所有可配置的东东！

//电机位置计数
#define circle_turns_Base_MAX   		1045.5	//底层角度计数最大脉冲次数（输出轴转一圈几个脉冲？）1045.5
#define circle_turns_Angle_MAX			360.0			//上层计数最大脉冲次数
extern int32_t	circle_turns_Base;					//底层计数（脉冲次数）
extern int16_t circle_turns_Angle;					//上层计数（换算出来的角度）
void MOTOR_recordTurn_handle(int8_t);				//存到Get_MOTOR_Fre_TIMhandle内，负责计算电机旋转角度
void MOTOR_recordTurn_Reset(void);					//重置零点

#define PID_I_Err							(1000)				//积分分离值，差值在此值以内才进行积分

//PID_id
#define 	PID_Position  			0
#define 	PID_Speed						1


typedef struct _PID_TypeDef
{
	uint8_t PID_id;

	double target;						//目标值
	float kp;									//比例
	float ki;									//积分
	float kd;									//微分

	double  measure;					//测量值
	float   err;							//误差
	float   last_err;      		//上次误差

	float pout;								//比例输出
	float iout;								//积分输出
	float dout;								//微分输出

	float output;							//本次输出
	float last_output;				//上次输出

	float MaxOutput;					//输出限幅
	float IntegralLimit;			//积分限幅
	float DeadBand;			  		//死区（绝对值）

	uint32_t thistime;
	uint32_t lasttime;
	uint8_t dtime;

	void (*f_param_init)(struct _PID_TypeDef *pid,  //PID参数初始化
				   uint8_t PID_id,
				   uint16_t maxOutput,										//输出限幅
				   uint16_t integralLimit,								//积分限幅
				   float deadband,												//死区，err太小就不管了
				   float kp,
				   float ki,
				   float kd);

//	void (*f_pid_reset)(struct _PID_TypeDef *pid, float kp,float ki, float kd);		//PID三个参数修改
	float (*f_cal_pid)(struct _PID_TypeDef *pid, float measure);   								//PID计算
}PID_TypeDef;

/*中途更改参数设定--------------------------------------------------------------*/
extern void pid_reset(PID_TypeDef * pid, float kp, float ki, float kd);
void pid_init(PID_TypeDef* pid);
extern PID_TypeDef My_MOTOR_pid,My_MOTOR_pos;		//外部
#endif /* PY_PID_H_ */
