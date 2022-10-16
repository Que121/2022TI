#include "PYchassis.h"
#include "MyConfig.h"						//�������п����õĶ�����
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

���ң����Ϣ

*/////////////////////////////////////////////////////////
void getRemoteControl(void)			//YQYMARK:����ң����������
{
	static float Pitch_temp;
	static uint32_t time_smooth_speed;	//���ڵ��������� 10ms����
	

	if(KEYBOARD_LOOSEN_ALL && (MOUSE_LOOSEN_ALL))
	{
		//ʹ��ң��������
		if(remote_control.switch_left == Switch_Down)		//ȷ�����ڵ���ģʽ
		{
			Remote.Vx  = remote_control.ch2 * 1.51;
			Remote.Vy  = remote_control.ch1 * 1.51;
			Remote.W   = remote_control.ch3 /500.0;
			Pitch_temp = remote_control.ch4 * 0.001;
		}
	}
	else
	{
		//ʹ�õ��Կ���
		if(KEYBOARD_PRESSED_W && (HAL_GetTick() - time_smooth_speed > 5))Remote.Vx += 10;
		if(KEYBOARD_PRESSED_A && (HAL_GetTick() - time_smooth_speed > 5))Remote.Vy -= 10;
		if(KEYBOARD_PRESSED_S && (HAL_GetTick() - time_smooth_speed > 5))Remote.Vx -= 10;
		if(KEYBOARD_PRESSED_D && (HAL_GetTick() - time_smooth_speed > 5))Remote.Vy += 10;
		
		//���ٹ���
		if((!KEYBOARD_PRESSED_W)&&(!KEYBOARD_PRESSED_S))Remote.Vx = 0;
		if((!KEYBOARD_PRESSED_A)&&(!KEYBOARD_PRESSED_D))Remote.Vy = 0;
		
		//�޷�
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

������̬��ײ����

*/////////////////////////////////////////////////////////



#define WHEEL_R		76		//���Ӱ뾶
#define CHASSIS_K	573		//K=0.5*(���᳤��+���᳤��)mm  //������ת�����
#define PI  3.1415926f
#define MOTOR_P		14		//������ٳݱ�

void PY_chassis_Ctrl(float Vx,float Vy,float Wz)
{
	float w[4];		//�ĸ����ӵ�ʵ��ת��rad/s
	int16_t n[4];	//ת��Ϊ�����ٶȵ��ĸ������ת��
	uint8_t i=0;
	
//		SpinTop.Angle  = (motor_data[4].NowAngle - Yaw_Mid_encode + 8192) % 8192 / 22.7555556f;
//	if(SpinTop.Angle > 360)
//		SpinTop.Angle = (SpinTop.Angle - 360) * 0.0174532f;
//	else
//		SpinTop.Angle *= 0.0174532f;
	
	SpinTop.Vx = Vx * cos(SpinTop.Angle) + Vy * sin(SpinTop.Angle);
	SpinTop.Vy = -Vx * sin(SpinTop.Angle) + Vy * cos(SpinTop.Angle);
	
	//speed_zoom ��������ϵ��  
	w[0] = (int16_t)(SpinTop.Vy  - SpinTop.Vx + CHASSIS_K*Wz)/WHEEL_R*speed_zoom;  //��ǰ��
	w[1] = (int16_t)(SpinTop.Vy  + SpinTop.Vx + CHASSIS_K*Wz)/WHEEL_R*speed_zoom;  //��ǰ��
	w[2] = (int16_t)(-SpinTop.Vy + SpinTop.Vx + CHASSIS_K*Wz)/WHEEL_R*speed_zoom;  //�����
	w[3] = (int16_t)(-SpinTop.Vy - SpinTop.Vx + CHASSIS_K*Wz)/WHEEL_R*speed_zoom;  //�Һ���

	for(i=0;i<4;i++)
	  n[i] = ((float)w[i]*MOTOR_P / (2*PI)) * 60;	//ת��Ϊ��������ٶ�

//	//����б���ߵ��ٶ�
//	for(i=0;i<4;i++)
//	{
//		if(n[i] > LIM_3508_SPEED)
//		{
//			uint8_t j=0;
//			float temp = (float)n[i] / LIM_3508_SPEED;	//����
//			
//			for(j=0;j<4;j++)	n[j] = (float)n[j]/temp;		//�ȱ�����С
//		}
//		else if(n[i] < -LIM_3508_SPEED) 
//		{
//			uint8_t j=0;
//			float temp = -(float)n[i] / LIM_3508_SPEED;	//����
//			
//			for(j=0;j<4;j++)	n[j] = (float)n[j]/temp;		//�ȱ�����С
//		}
//	}
//printf("n1-n4:%d %d %d %d\n",n[0],n[1],n[2],n[3]);
SetChassisMotorByPID(n[0],n[1],n[2],n[3]);
}






void TimberWolves_chassis_Handle(void)		//���̿�������  ִ��Ƶ�� 500Hz
{
	if(remote_control.switch_right == Switch_Down)
	{
		PY_PowerOff();												//�ر��������
	}
	else if(remote_control.switch_right == Switch_Middle)
	{
		getRemoteControl();																									//��ȡң����
		
		
		PY_chassis_Ctrl(Remote.Vx,Remote.Vy,Remote.W);											//��������
		MOTOR_PID_Cal();																										//����PIDֵ
		//printf("posP:%.2f I%.2f D%.2f  angP:%.2f D%.2f\n",GimbalMotor[0].PID_speed.pout,GimbalMotor[0].PID_speed.iout,\
			GimbalMotor[0].PID_speed.dout,GimbalMotor[0].PID_position.pout,GimbalMotor[0].PID_position.dout);
	}
	
	if(remote_control.switch_left == Switch_Down)speed_zoom = 0.7;
	if(remote_control.switch_left == Switch_Middle)speed_zoom = 1.3;
	if(remote_control.switch_left == Switch_Up)speed_zoom = 1.9;
}

