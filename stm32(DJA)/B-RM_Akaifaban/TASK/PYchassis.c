#include "PYchassis.h"
#include "MyConfig.h"						//包含所有可配置的东东！
#include "motor.h"
//#define ABS(x) ((x>0)?x:-x)
#define REMOTE_VX_MAX   1000
#define REMOTE_VX_MIN  -1000
#define REMOTE_VY_MAX   1000
#define REMOTE_VY_MIN  -1000

SpinTop_t SpinTop={0};

float speed_zoom = 0.9;
Remote_t Remote;

/*////////////////////////////////////////////////////////

获得遥控信息

*/////////////////////////////////////////////////////////
void getRemoteControl(void)			//YQYMARK:调整遥控器灵敏度
{
	static float Pitch_temp;
	static uint32_t time_smooth_speed;	//用于调整灵敏度 10ms递增
	

	if(KEYBOARD_LOOSEN_ALL && (MOUSE_LOOSEN_ALL))
	{
		//使用遥控器控制
		if(remote_control.switch_left == Switch_Down)		//确保处于底盘模式
		{
			Remote.Vx  = remote_control.ch2 * 1.51;
			Remote.Vy  = remote_control.ch1 * 1.51;
			Remote.W   = remote_control.ch3 /500.0;
			Pitch_temp = remote_control.ch4 * 0.001;
		}
	}
	else
	{
		//使用电脑控制
		if(KEYBOARD_PRESSED_W && (HAL_GetTick() - time_smooth_speed > 5))Remote.Vx += 10;
		if(KEYBOARD_PRESSED_A && (HAL_GetTick() - time_smooth_speed > 5))Remote.Vy -= 10;
		if(KEYBOARD_PRESSED_S && (HAL_GetTick() - time_smooth_speed > 5))Remote.Vx -= 10;
		if(KEYBOARD_PRESSED_D && (HAL_GetTick() - time_smooth_speed > 5))Remote.Vy += 10;
		
		//快速归零
		if((!KEYBOARD_PRESSED_W)&&(!KEYBOARD_PRESSED_S))Remote.Vx = 0;
		if((!KEYBOARD_PRESSED_A)&&(!KEYBOARD_PRESSED_D))Remote.Vy = 0;
		
		//限幅
		if(Remote.Vx > REMOTE_VX_MAX)Remote.Vx = REMOTE_VX_MAX;
		if(Remote.Vx < REMOTE_VX_MIN)Remote.Vx = REMOTE_VX_MIN;
		if(Remote.Vy > REMOTE_VY_MAX)Remote.Vy = REMOTE_VY_MAX;
		if(Remote.Vy < REMOTE_VY_MIN)Remote.Vy = REMOTE_VY_MIN;
		
		Remote.W = remote_control.mouse.x * 0.05;
		Pitch_temp = -remote_control.mouse.y* 0.012;
		time_smooth_speed = HAL_GetTick();
	}
	
	
	
	
	//if(KEYBOARD_LOOSEN_WASD){Remote.Vx = 0; Remote.Vy = 0;}
	
	
	Remote.Pitch += Pitch_temp;
	if(Remote.Pitch > GIMBAL_PITCH_ANGLE_MAX)Remote.Pitch = GIMBAL_PITCH_ANGLE_MAX;
	if(Remote.Pitch < GIMBAL_PITCH_ANGLE_MIN)Remote.Pitch = GIMBAL_PITCH_ANGLE_MIN;
	
	//printf("Vx:%f Vy:%f W:%f Pitch:%f ",Remote.Vx,Remote.Vy,Remote.W,Remote.Pitch);
}


/*////////////////////////////////////////////////////////

底盘姿态最底层控制

*/////////////////////////////////////////////////////////



#define WHEEL_R		76		//轮子半径
#define CHASSIS_K	573		//K=0.5*(横轴长度+纵轴长度)mm  //计算旋转入口量
#define PI  3.1415926f
#define MOTOR_P		14		//电机减速齿比

void PY_chassis_Ctrl(float Vx,float Vy,float Wz)
{
	float w[4];		//四个轮子的实际转速rad/s
	int16_t n[4];	//转换为码盘速度的四个电机的转速
	uint8_t i=0;
	
//		SpinTop.Angle  = (motor_data[4].NowAngle - Yaw_Mid_encode + 8192) % 8192 / 22.7555556f;
//	if(SpinTop.Angle > 360)
//		SpinTop.Angle = (SpinTop.Angle - 360) * 0.0174532f;
//	else
//		SpinTop.Angle *= 0.0174532f;
	
	SpinTop.Vx = Vx * cos(SpinTop.Angle) + Vy * sin(SpinTop.Angle);
	SpinTop.Vy = -Vx * sin(SpinTop.Angle) + Vy * cos(SpinTop.Angle);
	
	//speed_zoom 功率限制系数  
	w[0] = (int16_t)(SpinTop.Vy  - SpinTop.Vx + CHASSIS_K*Wz)/WHEEL_R*speed_zoom;  //右前轮
	w[1] = (int16_t)(SpinTop.Vy  + SpinTop.Vx + CHASSIS_K*Wz)/WHEEL_R*speed_zoom;  //左前轮
	w[2] = (int16_t)(-SpinTop.Vy + SpinTop.Vx + CHASSIS_K*Wz)/WHEEL_R*speed_zoom;  //左后轮
	w[3] = (int16_t)(-SpinTop.Vy - SpinTop.Vx + CHASSIS_K*Wz)/WHEEL_R*speed_zoom;  //右后轮

	for(i=0;i<4;i++)
	  n[i] = ((float)w[i]*MOTOR_P / (2*PI)) * 60;	//转换为电机码盘速度

//	//限制斜着走的速度
//	for(i=0;i<4;i++)
//	{
//		if(n[i] > LIM_3508_SPEED)
//		{
//			uint8_t j=0;
//			float temp = (float)n[i] / LIM_3508_SPEED;	//比例
//			
//			for(j=0;j<4;j++)	n[j] = (float)n[j]/temp;		//等比例缩小
//		}
//		else if(n[i] < -LIM_3508_SPEED) 
//		{
//			uint8_t j=0;
//			float temp = -(float)n[i] / LIM_3508_SPEED;	//比例
//			
//			for(j=0;j<4;j++)	n[j] = (float)n[j]/temp;		//等比例缩小
//		}
//	}
//printf("n1-n4:%d %d %d %d\n",n[0],n[1],n[2],n[3]);
SetChassisMotorByPID(n[0],n[1],n[2],n[3]);
}






void TimberWolves_chassis_Handle(void)		//底盘控制任务  执行频率 500Hz
{
	if(remote_control.switch_right == Switch_Down)
	{
		PY_PowerOff();												//关闭所有输出
	}
	else if(remote_control.switch_right == Switch_Middle)
	{
		getRemoteControl();																									//获取遥控量
		
		
		PY_chassis_Ctrl(Remote.Vx,Remote.Vy,Remote.W);											//驱动底盘
		MOTOR_PID_Cal();																										//计算PID值
		//printf("posP:%.2f I%.2f D%.2f  angP:%.2f D%.2f\n",GimbalMotor[0].PID_speed.pout,GimbalMotor[0].PID_speed.iout,\
			GimbalMotor[0].PID_speed.dout,GimbalMotor[0].PID_position.pout,GimbalMotor[0].PID_position.dout);
	}
	
	if(remote_control.switch_left == Switch_Down)speed_zoom = 0.7;
	if(remote_control.switch_left == Switch_Middle)speed_zoom = 1.3;
	if(remote_control.switch_left == Switch_Up)speed_zoom = 1.9;
}

