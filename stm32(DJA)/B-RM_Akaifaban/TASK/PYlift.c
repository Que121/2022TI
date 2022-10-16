//
//���������������������£�
//
//						  ǰ����W��
//			ID 7 	�����̣�  		ID 8	
//				+-------------+
//				|							|
//				|							|
//	(A)		|		����ͼ			|		(D)
//				|							|
//				|							|
//				+-------------+
//			ID 5              ID 6
//	���    ID 2			ID 1  ��CAN2��
//							�󷽣�S��
//
//�߶�ͳһ������Ϊ��������Ϊ����0λ��Ϊ���������г��м�λ�á�
//�г̣�W�� S�� 269mm  L��322mm
//



#include "PYlift.h" 
#include "motor.h"																//�������
#include "DR16.h"																	//ң��
#include "key.h"																	//��λ����
#include "PY_HigherControl.h"
#include "MyPWM.h"
#include "math.h"																	//���Ǻ���

#define PI 										3.1415							//��
#define PULLEY_S_d						27.02								//����ֱ�� 	mm  ��ʵ�����
#define PULLEY_W_d						31.41								//Ƥ����ֱ�� mm	��ʵ�����
#define GAP_W_S								225.0									//ǰ��̧��������� mm
//#define ABS(x)						((x>0)?x:-x)					//����ֵ

#define PY_limit(x,y,z)			{if(y>z)y=z;if(y<x)y=x;}


PY_Lift_t PY_Lift;																//̧�����ƽṹ��

float Xe, Ye;														//����pitch���
void GetRemote_AutoCtrl(void)
{
	float h1, h2, h3;																//��� ǰ�� �������
	double Ang_1;																		//���
	static uint8_t IsInit = 1;											//����Ϊ��������ֵ
	
	if(IsInit)																			//��ʼ�� �����ֵ
	{
		IsInit = 0;
		Ang_1 = atan((double)(PY_Lift.height_W - PY_Lift.height_S)/GAP_W_S);
		h1 = PY_Lift.height_S;
		h2 = PY_Lift.height_W;
		h3 = PY_Lift.length_Flex;
		
		Xe = (h3 + 415) * cos(Ang_1);
		Ye = h1 + (h3 + 415) * sin(Ang_1);
	}
	else																						//�Զ�����
	{
		Xe += remote_control.ch3 / 660.0 * 2;				//��ȡ������
		Ye += remote_control.ch4 / 660.0 * 2;			
		
		h1 = -0.5 * Xe + 400;													//����߶� ����Ĺ�ʽ�����ɸ���
		Ang_1 = atan((Ye - h1)/Xe);
		h2 = 220/tan(Ang_1) + h1;
		h3 = sqrtf(Xe*Xe + (Ye - h1) * (Ye - h1)) + 45;
		
		PY_limit(94,h1,437);													//��λ
		PY_limit(94,h2,437);
		PY_limit(13,h3,383);
		
		PY_Lift.height_S 		= h1;											//���� ������
		PY_Lift.height_W 		= h2;
		PY_Lift.length_Flex = h3;
	}
}

void PY_Lift_GetRemote(void)
{
	if(HAL_GetTick() - PY_Lift.lastCtrlTime > 100)
	{
		if(remote_control.switch_left == Switch_Middle)	//������������ģʽ��
		{
//			if((PY_Lift.calibrationStatus & (1 << 2)) == 0)	//У׼�ɹ�
//			{
//				GetRemote_AutoCtrl();													//�Զ�����߶�
//			}
//			else
//			{
				PY_Lift.lastCtrlTime = HAL_GetTick();					//���¼�¼ʱ��
				PY_Lift.height_W 		+= remote_control.ch2 / 660.0 * 30;
				PY_Lift.height_S 		+= remote_control.ch4 / 660.0 * 30;
				PY_Lift.length_Flex += remote_control.ch3 / 660.0 * 15;
//			}
			PY_Lift.angle_Pitch += remote_control.ch1 / 660.0 * 3;
		}
		if(remote_control.switch_left == Switch_Up)
		{
			PY_Lift.angle_FanRoll += (remote_control.ch1 / 660.0 * 0.4);												//���Roll
			if(remote_control.ch1 >  655)PY_Lift.angle_FanRoll =  90;
			if(remote_control.ch1 < -655)PY_Lift.angle_FanRoll = -90;
			
			if(remote_control.ch2 >= 0)PY_Lift.FanPower += (remote_control.ch2 / 660.0 * 1.0);		//�������
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
		
		if((PY_Lift.calibrationStatus & (1 << 2)) == 0)	//У׼�ɹ���ʵʱ�������
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

void PY_Lift_Calibrate()													//У׼
{
	if(HAL_GetTick() - PY_Lift.lastCalibrateTime > 1500)
	{
		if((LIMIT_IS_PRESSED_AS || LIMIT_IS_PRESSED_DS) && ((PY_Lift.calibrationStatus & 0x02) != 0))	//S��У׼
		{
			ledCtrl('8',0);
			PY_Lift.calibrationStatus &= ~0x02;
			PY_Lift.offset_S = PY_Lift.angle_S;
			PY_Lift.height_S = LIFT_HEIGHT_S_MIN;
			PY_Lift.lastCalibrateTime = HAL_GetTick();
			PY_Lift.angle_S 		= (int32_t)(PY_Lift.height_S 		/ (PULLEY_S_d * PI) * 360.0);
			printf("У׼����\n");
		}
		if((LIMIT_IS_PRESSED_AW || LIMIT_IS_PRESSED_DW) && ((PY_Lift.calibrationStatus & 0x01) != 0))	//W��У׼
		{
			ledCtrl('7',0);
			PY_Lift.calibrationStatus &= ~(1 << 0);
			PY_Lift.offset_W = (int32_t)((PY_Lift.height_W - LIFT_HEIGHT_W_MAX) / (PULLEY_W_d * PI) * 360.0);
			PY_Lift.height_W = LIFT_HEIGHT_W_MAX;
			PY_Lift.lastCalibrateTime = HAL_GetTick();
			PY_Lift.angle_W 		= (int32_t)(PY_Lift.height_W 		/ (PULLEY_W_d * PI) * 360.0);
			printf("У׼ǰ���\n");
		}
		if(((PY_Lift.calibrationStatus & 0x03) == 0) && ((PY_Lift.calibrationStatus & (1<<2)) != 0))	//У׼���
		{
			PY_Lift.calibrationStatus &= ~(1 << 2);						//��ֹ�ظ�У׼��
			PY_Lift.offset_A = 90 - (int32_t)((atan((double)(PY_Lift.height_W - PY_Lift.height_S)/GAP_W_S) / PI)*180);
			PY_Lift.angle_Pitch -= PY_Lift.offset_A;
		}
		
	}
	
}

void PY_Lift_Init(void)														//��ʼ��
{
	PY_Lift.height_S = 0;
	PY_Lift.height_W = 0;
	PY_Lift.lastCtrlTime = 0;
	PY_Lift.angle_S = 0;
	PY_Lift.angle_W = 0;
	PY_Lift.length_Flex = 0;
	PY_Lift.angle_Flex = 0;
	PY_Lift.angle_Pitch = 0;
	
	PY_Lift.offset_A = 0;													//ƫ��������(�Ƕ�)
	PY_Lift.offset_L = 0;													//ƫ��������
	PY_Lift.offset_W = 0;													//ƫ��������
	PY_Lift.offset_S = 0;													//ƫ��������
	
	PY_Lift.calibrationStatus = 0xff;							//ȫ����1��У׼�ɹ� ��bit����
	PY_Lift.lastCalibrateTime = HAL_GetTick();		//�ϴ�У׼ʱ��
	
	ledCtrl('7',1);
	ledCtrl('8',1);
	
	PY_Lift.angle_FanRoll = 0;										//����Roll
	PY_Lift.angle_VisionYaw = 0;									//�Ӿ���̨Yaw��
	PY_Lift.FanPower = 10;												//�������
}

void PWM_Ctrl(void)
{
	FanRollCtrl((int8_t)PY_Lift.angle_FanRoll);		//���ƶ��
	FanPowerCtrl((uint8_t)PY_Lift.FanPower);
}
void PY_Lift_Handle(void)														//���о��������ִ�У�
{
	if(remote_control.switch_right == Switch_Down)
	{
		PY_PowerOff();
	}
	else if(remote_control.switch_right == Switch_Middle)
	{
		PY_Lift_Calibrate();														//ִ��У׼
		PY_Lift_GetRemote();														//��ȡ������
		
		PWM_Ctrl();																			//���ƶ�� �� ���
		
		SetLiftMotorByPID_Angle( -(PY_Lift.angle_S + PY_Lift.offset_S),\
															 PY_Lift.angle_S + PY_Lift.offset_S,\
															 PY_Lift.angle_W + PY_Lift.offset_W,\
														 -(PY_Lift.angle_W + PY_Lift.offset_W));			//���� ����PID Ŀ��ֵ��̧�������
		
		LiftMOTOR_PID_Control_Angle();									//����PID���㣬����̧�����
		
		SetFlexMotorByPID_Angle(	PY_Lift.angle_Flex  + PY_Lift.offset_L,\
														 -(PY_Lift.angle_Flex 	+ PY_Lift.offset_L),\
															-(PY_Lift.angle_Pitch + PY_Lift.offset_A));
		
		FlexMOTOR_PID_Control_Angle();									//����PID���㣬�����������
	}
}


