/*
 * PY_PID.c
 *
 *  Created on: 2022年5月16日
 *      Author: 21154
 */

#include "PY_PID.h"

#define ABS(x)		((x>0)? x: -x)
PID_TypeDef My_MOTOR_pid, My_MOTOR_pos;		//串级PID，pos为位置环

int32_t  MOTOR_Fre = 0;										//通过cc1_value_2计算频率
uint32_t  MOTOR_Fre_Millis = 0;						//放上次计时的时间，如果超时就把频率清零
uint8_t MOTOR_direction = 0;
int32_t	circle_turns_Base = 0;						//底层计数（脉冲次数）
int16_t circle_turns_Angle = 0;						//上层计数（换算出来的角度）


//以下为PID参数相关计算
/*参数初始化--------------------------------------------------------------*/
static void pid_param_init(
	PID_TypeDef * pid,
	uint8_t PID_id,
	uint16_t maxout,
	uint16_t intergral_limit,
	float deadband,

	float 	kp,
	float 	ki,
	float 	kd)
{
	pid->PID_id = PID_id;

	pid->DeadBand = deadband;
	pid->IntegralLimit = intergral_limit;
	pid->MaxOutput = maxout;

	pid->kp = kp;
	pid->ki = ki;
	pid->kd = kd;

	pid->output = 0;
}

/*中途更改参数设定--------------------------------------------------------------*/
void pid_reset(PID_TypeDef * pid, float kp, float ki, float kd)
{
	pid->kp = kp;
	pid->ki = ki;
	pid->kd = kd;
}

/*pid计算-----------------------------------------------------------------------*/


static float pid_calculate(PID_TypeDef* pid, float measure)
{
//	uint32_t time,lasttime;

	pid->lasttime = pid->thistime;
	pid->thistime = HAL_GetTick();
	pid->dtime = pid->thistime-pid->lasttime;
	pid->measure = measure;
  //	pid->target = target;

	pid->last_err  = pid->err;
	pid->last_output = pid->output;

	pid->err = pid->target - pid->measure;

	//是否进入死区
	if((ABS(pid->err) >= pid->DeadBand))
	{
		pid->pout = pid->kp * pid->err;											//比例
		if(pid->err < PID_I_Err)
			pid->iout += (pid->ki * pid->err);								//积分
		pid->dout =  pid->kd * (pid->err - pid->last_err);	//微分

		//积分是否超出限制
		if(pid->iout > pid->IntegralLimit)
			pid->iout = pid->IntegralLimit;
		if(pid->iout < - pid->IntegralLimit)
			pid->iout = - pid->IntegralLimit;

		//pid输出和
		pid->output = pid->pout + pid->iout + pid->dout;
//		if(pid->output <= 0)pid->output = 0;								//YQY新增，防止正反馈（尝试修复反馈绝对值bug）

		pid->output = pid->output*0.7f + pid->last_output*0.3f;  //滤波？
		if(pid->output>pid->MaxOutput)											//输出限幅
		{
			pid->output = pid->MaxOutput;
		}
		if(pid->output < -(pid->MaxOutput))
		{
			pid->output = -(pid->MaxOutput);
		}
	}
	return pid->output;
}

/*pid结构体初始化，每一个pid参数需要调用一次-----------------------------------------------------*/
void pid_init(PID_TypeDef* pid)
{
	pid->f_param_init = pid_param_init;
	//pid->f_pid_reset = pid_reset;
	pid->f_cal_pid = pid_calculate;
}

