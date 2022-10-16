#include "motor.h"

//PID_TypeDef GimbalMotorPIDspeed[2]  		= {0};				//云台电机速度环*2
//PID_TypeDef	GimbalMotorPIDposition[2]  	= {0};				//云台电机位置环*2

//新
motor_Typedef ChassisMotor[4] = {0};									//底盘电机四个
motor_Typedef LiftMotor[4]    = {0};									//抬升电机四个
motor_Typedef FlexMotor[2]		= {0};									//伸长电机两个
motor_Typedef PitchMotor;															//Pitch轴翻转电机（吸盘）

void MOTOR_Init(void)																	//初始化电机
{
	uint8_t i = 0;
	
//----------------------------------------------底盘电机---------------------------------------------------------------------

	for(i = 0; i < 4; i++)															//底盘电机PID初始化
	{
		ChassisMotor[i].PIDmode = speed;									//使用速度环
		ChassisMotor[i].MotorType = DJI_3508;							//底盘使用大疆3508电机
		
		pid_init(&ChassisMotor[i].PID_speed);							//PID初始化
		pid_init(&ChassisMotor[i].PID_position);					//PID初始化
		ChassisMotor[i].PID_speed.f_param_init(&ChassisMotor[i].PID_speed,
																					PID_Speed,											//速度环
																					CHASSIS_MOTOR_PID_OUTPUT_MAX,		//输出限幅
																					CHASSIS_MOTOR_I_OUT_MAX,				//积分限幅
																					10, 														//死区
																					3.8, 0.2, 0);//PID
		ChassisMotor[i].PID_position.f_param_init(&ChassisMotor[i].PID_position,
																					PID_Position,										//位置环
																					CHASSIS_MOTOR_PID_OUTPUT_MAX_P,	//输出限幅
																					CHASSIS_MOTOR_I_OUT_MAX_P,			//积分限幅
																					10, 														//死区
																					CHASSIS_MOTOR_kP_P, CHASSIS_MOTOR_kI_P, CHASSIS_MOTOR_kD_P);//PID
		
		ChassisMotor[i].PID_speed.target = 0;					//在这里再次修改初始target
		ChassisMotor[i].PID_position.target = 0;			//在这里再次修改初始target

		
	}

//----------------------------------------------抬升电机---------------------------------------------------------------------
	
	for(i = 0; i < 4; i++)																								//底盘电机PID初始化
	{
		LiftMotor[i].PIDmode = stop;																				//默认停止状态
		LiftMotor[i].MotorType = DJI_3508;																	//底盘使用大疆3508电机
		
		pid_init(&LiftMotor[i].PID_speed);																	//PID 速度环 初始化
		pid_init(&LiftMotor[i].PID_position);																//PID 位置环 初始化
		LiftMotor[i].PID_speed.f_param_init(&LiftMotor[i].PID_speed,
																					PID_Speed,										//速度环
																					16384,												//输出限幅
																					5000,													//积分限幅
																					5, 														//死区
																					0.8, 0.1, 0);									//PID  1.8, 0.2, 0
		LiftMotor[i].PID_position.f_param_init(&LiftMotor[i].PID_position,
																					PID_Position,									//位置环
																					16384,												//输出限幅
																					5000,													//积分限幅
																					0, 														//死区
																					25, 0, 0);										//PID  25, 0, 0
		
		LiftMotor[i].PID_speed.target = 0;					//在这里再次修改初始target
		LiftMotor[i].PID_position.target = 0;				//在这里再次修改初始target
	}
	

//----------------------------------------------伸长电机---------------------------------------------------------------------

	for(i = 0; i < 2; i++)																								//底盘电机PID初始化
	{
		FlexMotor[i].PIDmode = stop;																				//默认停止状态
		FlexMotor[i].MotorType = DJI_3508;																	//底盘使用大疆3508电机
		
		pid_init(&FlexMotor[i].PID_speed);																	//PID 速度环 初始化
		pid_init(&FlexMotor[i].PID_position);																//PID 位置环 初始化
		FlexMotor[i].PID_speed.f_param_init(&FlexMotor[i].PID_speed,
																					PID_Speed,										//速度环
																					16384,												//输出限幅
																					5000,													//积分限幅
																					15, 													//死区
																					1.8, 0.2, 0);									//PID  1.8, 0.2, 0
		FlexMotor[i].PID_position.f_param_init(&FlexMotor[i].PID_position,
																					PID_Position,									//位置环
																					16384,												//输出限幅
																					5000,													//积分限幅
																					0, 														//死区
																					25, 0, 0);										//PID  25, 0, 0
		
		FlexMotor[i].PID_speed.target = 0;					//在这里再次修改初始target
		FlexMotor[i].PID_position.target = 0;				//在这里再次修改初始target
	}
	
//----------------------------------------------Pitch翻转电机---------------------------------------------------------------------
	PitchMotor.PIDmode = stop;
	PitchMotor.MotorType = DJI_3508;
	
	pid_init(&PitchMotor.PID_speed);																	//PID 速度环 初始化
	pid_init(&PitchMotor.PID_position);																//PID 位置环 初始化
	PitchMotor.PID_speed.f_param_init(&PitchMotor.PID_speed,
																				PID_Speed,										//速度环
																				16384,												//输出限幅
																				5000,													//积分限幅
																				15, 													//死区
																				1.8, 0.3, 0);									//PID  1.8, 0.2, 0
	PitchMotor.PID_position.f_param_init(&PitchMotor.PID_position,
																				PID_Position,									//位置环
																				16384,												//输出限幅
																				5000,													//积分限幅
																				0, 														//死区
																				30, 0, 0);										//PID  25, 0, 0
	
	PitchMotor.PID_speed.target = 0;					//在这里再次修改初始target
	PitchMotor.PID_position.target = 0;				//在这里再次修改初始target
	
}


/***********************************************************************************************************************
  * @brief  底盘电机相关计算
  *
  * @param  无
  *
  * @retval 无
  *///******************************************************************************************************************

void ChassisMOTOR_PID_Control(void)																				//计算底盘PID，已经包含在MOTOR_PID_Cal()函数内
{
	uint8_t i = 0;
	if(ChassisMotor[i].PIDmode == position)																	//如果是位置环,则进行位置计算
		for(i = 0; i < 4; i++)
		{																																			//计算输出值，传入电机结构体指针和当前测量转速
			ChassisMotor[i].PID_position.f_cal_pid(&ChassisMotor[i].PID_position,ChassisMotor[i].measure.total_angle/8190.0*360.0/19.0);
			ChassisMotor[i].PID_speed.target = ChassisMotor[i].PID_position.output;
			
			if((ChassisMotor[i].PID_position.err < 1) && (ChassisMotor[i].PID_position.err > -1))
				ChassisMotor[i].PID_speed.iout = 0;
		}
	for(i = 0; i < 4; i++)																									//默认是速度环，进行速度计算
	{																																				//计算输出值，传入电机结构体指针和当前测量转速
		ChassisMotor[i].PID_speed.f_cal_pid(&ChassisMotor[i].PID_speed,ChassisMotor[i].measure.speed_rpm);
		if(ChassisMotor[i].PIDmode == stop)ChassisMotor[0].PID_speed.output = 0;			//如果运行在停止模式，则输出为0
	}
	
	SetChassisMotorRAW(&hcan1, 	  (int16_t)ChassisMotor[0].PID_speed.output, 					//将计算结果通过can线发给底盘电机
																(int16_t)ChassisMotor[1].PID_speed.output, 
																(int16_t)ChassisMotor[2].PID_speed.output, 
																(int16_t)ChassisMotor[3].PID_speed.output);
}
void SetChassisMotorByPID(int16_t v1,int16_t v2,int16_t v3,int16_t v4)	//通过PID来控制底盘电机转速，传入四个电机的目标值
{
	uint8_t i = 0;
	for(i = 0; i < 4; i++)
		ChassisMotor[i].PIDmode = speed;
	
	ChassisMotor[0].PID_speed.target = (float)v1;
	ChassisMotor[1].PID_speed.target = (float)v2;
	ChassisMotor[2].PID_speed.target = (float)v3;
	ChassisMotor[3].PID_speed.target = (float)v4;
}
void SetChassisMotorByPID_Angle(int16_t a1,int16_t a2,int16_t a3,int16_t a4)	//通过PID来控制底盘电机转速，传入四个电机的目标角度
{
	float tempAngle = 0;
	uint8_t i = 0;
	for(i = 0; i < 4; i++)
		ChassisMotor[i].PIDmode = position;
	
	tempAngle = a1;
	ChassisMotor[0].PID_position.target = tempAngle;
	tempAngle = a2;
	ChassisMotor[1].PID_position.target = tempAngle;
	tempAngle = a3;
	ChassisMotor[2].PID_position.target = tempAngle;
	tempAngle = a4;
	ChassisMotor[3].PID_position.target = tempAngle;
}
/***********************************************************************************************************************
  * @brief  抬升电机相关函数
  *
  * @param  无
  *
  * @retval 无
  *///******************************************************************************************************************
void LiftMOTOR_PID_Control_Angle(void)
{
	uint8_t i = 0;
	for(i = 0; i < 4; i++)
	{
		LiftMotor[i].PID_position.f_cal_pid(&LiftMotor[i].PID_position,LiftMotor[i].measure.total_angle / 8191.0 * 360 / 20.0);		//注意这里的角度！
		LiftMotor[i].PID_speed.target = LiftMotor[i].PID_position.output;
	}
	for(i = 0; i < 4; i++)
	{																																			//计算输出值，传入电机结构体指针和当前测量转速
		LiftMotor[i].PID_speed.f_cal_pid(&LiftMotor[i].PID_speed,LiftMotor[i].measure.speed_rpm);
	}
	if((LiftMotor[0].PIDmode != stop) && (LiftMotor[1].PIDmode != stop))
		SetLeftMotorRAW(				LiftMotor[0].PID_speed.output,
														LiftMotor[1].PID_speed.output,
														LiftMotor[2].PID_speed.output,
														LiftMotor[3].PID_speed.output);
}

void SetLiftMotorByPID_Angle(int16_t a1,int16_t a2,int16_t a3,int16_t a4)	//通过PID来控制抬升电机转速，传入四个电机的目标角度
{
	float tempAngle = 0;
	uint8_t i = 0;
	for(i = 0; i < 4; i++)
		LiftMotor[i].PIDmode = position;
	
	tempAngle = a1;
	LiftMotor[0].PID_position.target = tempAngle;
	tempAngle = a2;
	LiftMotor[1].PID_position.target = tempAngle;
	tempAngle = a3;
	LiftMotor[2].PID_position.target = tempAngle;
	tempAngle = a4;
	LiftMotor[3].PID_position.target = tempAngle;
}

/***********************************************************************************************************************
  * @brief  伸缩电机 + Pitch翻转电机相关函数
  *
  * @param  无
  *
  * @retval 无
  *///******************************************************************************************************************
void FlexMOTOR_PID_Control_Angle(void)
{
	uint8_t i = 0;
	for(i = 0; i < 2; i++)
	{
		FlexMotor[i].PID_position.f_cal_pid(&FlexMotor[i].PID_position,FlexMotor[i].measure.total_angle / 8191.0 * 360 / 20.0);		//注意这里的角度！
		FlexMotor[i].PID_speed.target = FlexMotor[i].PID_position.output;
	}
	PitchMotor.PID_position.f_cal_pid(&PitchMotor.PID_position, PitchMotor.measure.total_angle / 8191.0 * 360 / 20.0);
	PitchMotor.PID_speed.target = PitchMotor.PID_position.output;
	
	for(i = 0; i < 2; i++)
	{																																			//计算输出值，传入电机结构体指针和当前测量转速
		FlexMotor[i].PID_speed.f_cal_pid(&FlexMotor[i].PID_speed,FlexMotor[i].measure.speed_rpm);
	}
	PitchMotor.PID_speed.f_cal_pid(&PitchMotor.PID_speed,PitchMotor.measure.speed_rpm);
	if((FlexMotor[0].PIDmode != stop) && (FlexMotor[1].PIDmode != stop))
		SetFlexMotorRAW(FlexMotor[0].PID_speed.output,
										FlexMotor[1].PID_speed.output,
										PitchMotor.PID_speed.output);
}
void SetFlexMotorByPID_Angle(int16_t a1,int16_t a2, int16_t pitch)											//通过PID来控制抬伸缩机转速，传入两个电机的目标角度
{
	float tempAngle = 0;
	uint8_t i = 0;
	for(i = 0; i < 4; i++)
		FlexMotor[i].PIDmode = position;
	PitchMotor.PIDmode = position;
	
	tempAngle = a1;
	FlexMotor[0].PID_position.target = tempAngle;
	tempAngle = a2;
	FlexMotor[1].PID_position.target = tempAngle;
	tempAngle = pitch;
	PitchMotor.PID_position.target = tempAngle;
}

//总PID计算函数
void MOTOR_PID_Cal(void)																								//计算PID输出
{
	ChassisMOTOR_PID_Control();																						//底盘PID
}



