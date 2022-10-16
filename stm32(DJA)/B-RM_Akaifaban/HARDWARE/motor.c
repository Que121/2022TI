#include "motor.h"

//PID_TypeDef GimbalMotorPIDspeed[2]  		= {0};				//��̨����ٶȻ�*2
//PID_TypeDef	GimbalMotorPIDposition[2]  	= {0};				//��̨���λ�û�*2

//��
motor_Typedef ChassisMotor[4] = {0};									//���̵���ĸ�
motor_Typedef LiftMotor[4]    = {0};									//̧������ĸ�
motor_Typedef FlexMotor[2]		= {0};									//�쳤�������
motor_Typedef PitchMotor;															//Pitch�ᷭת��������̣�

void MOTOR_Init(void)																	//��ʼ�����
{
	uint8_t i = 0;
	
//----------------------------------------------���̵��---------------------------------------------------------------------

	for(i = 0; i < 4; i++)															//���̵��PID��ʼ��
	{
		ChassisMotor[i].PIDmode = speed;									//ʹ���ٶȻ�
		ChassisMotor[i].MotorType = DJI_3508;							//����ʹ�ô�3508���
		
		pid_init(&ChassisMotor[i].PID_speed);							//PID��ʼ��
		pid_init(&ChassisMotor[i].PID_position);					//PID��ʼ��
		ChassisMotor[i].PID_speed.f_param_init(&ChassisMotor[i].PID_speed,
																					PID_Speed,											//�ٶȻ�
																					CHASSIS_MOTOR_PID_OUTPUT_MAX,		//����޷�
																					CHASSIS_MOTOR_I_OUT_MAX,				//�����޷�
																					10, 														//����
																					3.8, 0.2, 0);//PID
		ChassisMotor[i].PID_position.f_param_init(&ChassisMotor[i].PID_position,
																					PID_Position,										//λ�û�
																					CHASSIS_MOTOR_PID_OUTPUT_MAX_P,	//����޷�
																					CHASSIS_MOTOR_I_OUT_MAX_P,			//�����޷�
																					10, 														//����
																					CHASSIS_MOTOR_kP_P, CHASSIS_MOTOR_kI_P, CHASSIS_MOTOR_kD_P);//PID
		
		ChassisMotor[i].PID_speed.target = 0;					//�������ٴ��޸ĳ�ʼtarget
		ChassisMotor[i].PID_position.target = 0;			//�������ٴ��޸ĳ�ʼtarget

		
	}

//----------------------------------------------̧�����---------------------------------------------------------------------
	
	for(i = 0; i < 4; i++)																								//���̵��PID��ʼ��
	{
		LiftMotor[i].PIDmode = stop;																				//Ĭ��ֹͣ״̬
		LiftMotor[i].MotorType = DJI_3508;																	//����ʹ�ô�3508���
		
		pid_init(&LiftMotor[i].PID_speed);																	//PID �ٶȻ� ��ʼ��
		pid_init(&LiftMotor[i].PID_position);																//PID λ�û� ��ʼ��
		LiftMotor[i].PID_speed.f_param_init(&LiftMotor[i].PID_speed,
																					PID_Speed,										//�ٶȻ�
																					16384,												//����޷�
																					5000,													//�����޷�
																					5, 														//����
																					0.8, 0.1, 0);									//PID  1.8, 0.2, 0
		LiftMotor[i].PID_position.f_param_init(&LiftMotor[i].PID_position,
																					PID_Position,									//λ�û�
																					16384,												//����޷�
																					5000,													//�����޷�
																					0, 														//����
																					25, 0, 0);										//PID  25, 0, 0
		
		LiftMotor[i].PID_speed.target = 0;					//�������ٴ��޸ĳ�ʼtarget
		LiftMotor[i].PID_position.target = 0;				//�������ٴ��޸ĳ�ʼtarget
	}
	

//----------------------------------------------�쳤���---------------------------------------------------------------------

	for(i = 0; i < 2; i++)																								//���̵��PID��ʼ��
	{
		FlexMotor[i].PIDmode = stop;																				//Ĭ��ֹͣ״̬
		FlexMotor[i].MotorType = DJI_3508;																	//����ʹ�ô�3508���
		
		pid_init(&FlexMotor[i].PID_speed);																	//PID �ٶȻ� ��ʼ��
		pid_init(&FlexMotor[i].PID_position);																//PID λ�û� ��ʼ��
		FlexMotor[i].PID_speed.f_param_init(&FlexMotor[i].PID_speed,
																					PID_Speed,										//�ٶȻ�
																					16384,												//����޷�
																					5000,													//�����޷�
																					15, 													//����
																					1.8, 0.2, 0);									//PID  1.8, 0.2, 0
		FlexMotor[i].PID_position.f_param_init(&FlexMotor[i].PID_position,
																					PID_Position,									//λ�û�
																					16384,												//����޷�
																					5000,													//�����޷�
																					0, 														//����
																					25, 0, 0);										//PID  25, 0, 0
		
		FlexMotor[i].PID_speed.target = 0;					//�������ٴ��޸ĳ�ʼtarget
		FlexMotor[i].PID_position.target = 0;				//�������ٴ��޸ĳ�ʼtarget
	}
	
//----------------------------------------------Pitch��ת���---------------------------------------------------------------------
	PitchMotor.PIDmode = stop;
	PitchMotor.MotorType = DJI_3508;
	
	pid_init(&PitchMotor.PID_speed);																	//PID �ٶȻ� ��ʼ��
	pid_init(&PitchMotor.PID_position);																//PID λ�û� ��ʼ��
	PitchMotor.PID_speed.f_param_init(&PitchMotor.PID_speed,
																				PID_Speed,										//�ٶȻ�
																				16384,												//����޷�
																				5000,													//�����޷�
																				15, 													//����
																				1.8, 0.3, 0);									//PID  1.8, 0.2, 0
	PitchMotor.PID_position.f_param_init(&PitchMotor.PID_position,
																				PID_Position,									//λ�û�
																				16384,												//����޷�
																				5000,													//�����޷�
																				0, 														//����
																				30, 0, 0);										//PID  25, 0, 0
	
	PitchMotor.PID_speed.target = 0;					//�������ٴ��޸ĳ�ʼtarget
	PitchMotor.PID_position.target = 0;				//�������ٴ��޸ĳ�ʼtarget
	
}


/***********************************************************************************************************************
  * @brief  ���̵����ؼ���
  *
  * @param  ��
  *
  * @retval ��
  *///******************************************************************************************************************

void ChassisMOTOR_PID_Control(void)																				//�������PID���Ѿ�������MOTOR_PID_Cal()������
{
	uint8_t i = 0;
	if(ChassisMotor[i].PIDmode == position)																	//�����λ�û�,�����λ�ü���
		for(i = 0; i < 4; i++)
		{																																			//�������ֵ���������ṹ��ָ��͵�ǰ����ת��
			ChassisMotor[i].PID_position.f_cal_pid(&ChassisMotor[i].PID_position,ChassisMotor[i].measure.total_angle/8190.0*360.0/19.0);
			ChassisMotor[i].PID_speed.target = ChassisMotor[i].PID_position.output;
			
			if((ChassisMotor[i].PID_position.err < 1) && (ChassisMotor[i].PID_position.err > -1))
				ChassisMotor[i].PID_speed.iout = 0;
		}
	for(i = 0; i < 4; i++)																									//Ĭ�����ٶȻ��������ٶȼ���
	{																																				//�������ֵ���������ṹ��ָ��͵�ǰ����ת��
		ChassisMotor[i].PID_speed.f_cal_pid(&ChassisMotor[i].PID_speed,ChassisMotor[i].measure.speed_rpm);
		if(ChassisMotor[i].PIDmode == stop)ChassisMotor[0].PID_speed.output = 0;			//���������ֹͣģʽ�������Ϊ0
	}
	
	SetChassisMotorRAW(&hcan1, 	  (int16_t)ChassisMotor[0].PID_speed.output, 					//��������ͨ��can�߷������̵��
																(int16_t)ChassisMotor[1].PID_speed.output, 
																(int16_t)ChassisMotor[2].PID_speed.output, 
																(int16_t)ChassisMotor[3].PID_speed.output);
}
void SetChassisMotorByPID(int16_t v1,int16_t v2,int16_t v3,int16_t v4)	//ͨ��PID�����Ƶ��̵��ת�٣������ĸ������Ŀ��ֵ
{
	uint8_t i = 0;
	for(i = 0; i < 4; i++)
		ChassisMotor[i].PIDmode = speed;
	
	ChassisMotor[0].PID_speed.target = (float)v1;
	ChassisMotor[1].PID_speed.target = (float)v2;
	ChassisMotor[2].PID_speed.target = (float)v3;
	ChassisMotor[3].PID_speed.target = (float)v4;
}
void SetChassisMotorByPID_Angle(int16_t a1,int16_t a2,int16_t a3,int16_t a4)	//ͨ��PID�����Ƶ��̵��ת�٣������ĸ������Ŀ��Ƕ�
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
  * @brief  ̧�������غ���
  *
  * @param  ��
  *
  * @retval ��
  *///******************************************************************************************************************
void LiftMOTOR_PID_Control_Angle(void)
{
	uint8_t i = 0;
	for(i = 0; i < 4; i++)
	{
		LiftMotor[i].PID_position.f_cal_pid(&LiftMotor[i].PID_position,LiftMotor[i].measure.total_angle / 8191.0 * 360 / 20.0);		//ע������ĽǶȣ�
		LiftMotor[i].PID_speed.target = LiftMotor[i].PID_position.output;
	}
	for(i = 0; i < 4; i++)
	{																																			//�������ֵ���������ṹ��ָ��͵�ǰ����ת��
		LiftMotor[i].PID_speed.f_cal_pid(&LiftMotor[i].PID_speed,LiftMotor[i].measure.speed_rpm);
	}
	if((LiftMotor[0].PIDmode != stop) && (LiftMotor[1].PIDmode != stop))
		SetLeftMotorRAW(				LiftMotor[0].PID_speed.output,
														LiftMotor[1].PID_speed.output,
														LiftMotor[2].PID_speed.output,
														LiftMotor[3].PID_speed.output);
}

void SetLiftMotorByPID_Angle(int16_t a1,int16_t a2,int16_t a3,int16_t a4)	//ͨ��PID������̧�����ת�٣������ĸ������Ŀ��Ƕ�
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
  * @brief  ������� + Pitch��ת�����غ���
  *
  * @param  ��
  *
  * @retval ��
  *///******************************************************************************************************************
void FlexMOTOR_PID_Control_Angle(void)
{
	uint8_t i = 0;
	for(i = 0; i < 2; i++)
	{
		FlexMotor[i].PID_position.f_cal_pid(&FlexMotor[i].PID_position,FlexMotor[i].measure.total_angle / 8191.0 * 360 / 20.0);		//ע������ĽǶȣ�
		FlexMotor[i].PID_speed.target = FlexMotor[i].PID_position.output;
	}
	PitchMotor.PID_position.f_cal_pid(&PitchMotor.PID_position, PitchMotor.measure.total_angle / 8191.0 * 360 / 20.0);
	PitchMotor.PID_speed.target = PitchMotor.PID_position.output;
	
	for(i = 0; i < 2; i++)
	{																																			//�������ֵ���������ṹ��ָ��͵�ǰ����ת��
		FlexMotor[i].PID_speed.f_cal_pid(&FlexMotor[i].PID_speed,FlexMotor[i].measure.speed_rpm);
	}
	PitchMotor.PID_speed.f_cal_pid(&PitchMotor.PID_speed,PitchMotor.measure.speed_rpm);
	if((FlexMotor[0].PIDmode != stop) && (FlexMotor[1].PIDmode != stop))
		SetFlexMotorRAW(FlexMotor[0].PID_speed.output,
										FlexMotor[1].PID_speed.output,
										PitchMotor.PID_speed.output);
}
void SetFlexMotorByPID_Angle(int16_t a1,int16_t a2, int16_t pitch)											//ͨ��PID������̧������ת�٣��������������Ŀ��Ƕ�
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

//��PID���㺯��
void MOTOR_PID_Cal(void)																								//����PID���
{
	ChassisMOTOR_PID_Control();																						//����PID
}



