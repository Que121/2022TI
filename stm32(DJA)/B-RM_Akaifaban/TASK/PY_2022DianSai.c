#include "PY_2022DianSai.h"
#include "PY_HigherControl.h"
#include "myUART.h"
#include "DR16.h"
#include "buzzer.h"
#include "bsp_can.h"
#include "motor.h"
#include "PYlift.h"
#include "power.h"
#include "MyPWM.h"
#include "HYQY_RECV.h"

#include "buzzer.h"
#include "YQY_Way.h"

PY_2022_t PY_2022;
uint8_t TotalStep = 0;																		//�ܽ������в���

void DianSai2022_Init(void)
{
	uint8_t i = 0;
	My_UART_Init();					//���ڽ��ճ�ʼ������������ϵͳ����
	printf("test printf %d\n",666);
	
	DR16_Init();						//��ʼ��ң�������ջ�
	buzzer_Init();					//��ʼ��������
	SafeLock();							//��ȫģʽ
	power_on_24v_ALL();
	myCanInit_ALL();				//���õײ�can��ع��ܣ������շ�
	
	for(i = 0; i < 2; i++)																								//���̵��PID��ʼ��
	{
		LiftMotor[i].PIDmode = stop;																				//Ĭ��ֹͣ״̬
		LiftMotor[i].MotorType = DJI_2006;																	//����ʹ��DJI_2006
		
		pid_init(&LiftMotor[i].PID_speed);																	//PID �ٶȻ� ��ʼ��
		pid_init(&LiftMotor[i].PID_position);																//PID λ�û� ��ʼ��
		LiftMotor[i].PID_speed.f_param_init(&LiftMotor[i].PID_speed,
																					PID_Speed,										//�ٶȻ�
																					16384,												//����޷�
																					5000,													//�����޷�
																					50, 														//����
																					0.8, 0.1, 0);									//PID  0.8, 0.1, 0
		LiftMotor[i].PID_position.f_param_init(&LiftMotor[i].PID_position,
																					PID_Position,									//λ�û�
																					16384,												//����޷�
																					5000,													//�����޷�
																					0, 														//����
																					25, 0, 0);										//PID  25, 0, 0
		
		LiftMotor[i].PID_speed.target = 0;					//�������ٴ��޸ĳ�ʼtarget
		LiftMotor[i].PID_position.target = 0;				//�������ٴ��޸ĳ�ʼtarget
	}
	
	PY_Lift_Init();
	
	MyPWM_Init();
	
	ServoSpark();
}

void SetMotorByPID_Position(int32_t left,int32_t right)
{
	double tempAngle = 0;
	uint8_t i = 0;
	for(i = 0; i < 2; i++)
		LiftMotor[i].PIDmode = position;
	
	tempAngle = left;
	LiftMotor[0].PID_position.target = tempAngle;
	tempAngle = right;
	LiftMotor[1].PID_position.target = tempAngle;
}
void SetMotorByPID_Speed   (int16_t left,int16_t right) 		//���õ���ٶ�
{
	uint8_t i = 0;
	for(i = 0; i < 2; i++)
		LiftMotor[i].PIDmode = speed;
	LiftMotor[0].PID_speed.target = left;
	LiftMotor[1].PID_speed.target = right;
}

void Dian2022MotorCtrl_Handle(void)
{
	uint8_t i = 0;
	
	for(i = 0; i < 2; i++)
	{
		LiftMotor[i].PID_position.f_cal_pid(&LiftMotor[i].PID_position,LiftMotor[i].measure.total_angle / 8191.0 * 360 / 20.0);		//ע������ĽǶȣ�
		if(LiftMotor[0].PIDmode == position)
			LiftMotor[i].PID_speed.target = LiftMotor[i].PID_position.output;
	}
	for(i = 0; i < 2; i++)
	{																																			//�������ֵ���������ṹ��ָ��͵�ǰ����ת��
		if(LiftMotor[i].PID_speed.target < 5)LiftMotor[i].PID_speed.iout = 0;
		LiftMotor[i].PID_speed.f_cal_pid(&LiftMotor[i].PID_speed,LiftMotor[i].measure.speed_rpm);
	}
	if((LiftMotor[0].PIDmode != stop) && (LiftMotor[1].PIDmode != stop))
		SetLeftMotorRAW(				LiftMotor[0].PID_speed.output,
														LiftMotor[1].PID_speed.output,
														LiftMotor[2].PID_speed.output,
														LiftMotor[3].PID_speed.output);
}

void turnServoCtrl(int8_t Angle)									//-90 - 90	PWM-S��
{
	if(Angle >= 60)Angle = 60;
	if(Angle <= -40)Angle = -40;
	
	TIM2->CCR1 = 1475 + (int16_t)(Angle/90.0*1000.0);
	//HAL_Delay(200);
}
void ServoSpark(void)
{
	turnServoCtrl(10);
	HAL_Delay(200);
	turnServoCtrl(0);
}


void Dian2022GetCtrlInfo(void)
{
	if(HAL_GetTick() - PY_2022.lastCtrlTime > 100)
	{
		if(remote_control.switch_left == Switch_Middle)							//ң��ģʽ
		{
			PY_2022.lastCtrlTime = HAL_GetTick();											//���¼�¼ʱ��
			PY_2022.leftAng += remote_control.ch2 / 660.0 * 120;			//��Ƕ�
			PY_2022.rightAng+= remote_control.ch2 / 660.0 * 120;			//�ҽǶ�
			if(!PY_2022.YQY_GoShouldDo)PY_2022.turnAng = remote_control.ch3/660.0 * 60.0;				//ת��Ƕ�
		}
//		if((remote_control.switch_left == Switch_Up) && (PY_2022.remoteSwitchLeft == Switch_Middle))
//		{
//			ServoSpark();
//			YQY_GoAddMap(600,-60);
//			YQY_GoAddMap(570,0);
//			YQY_GoAddMap(1130,60);
//		}
//		
//		PY_2022.remoteSwitchLeft = remote_control.switch_left;
		if(remote_control.switch_left == Switch_Up)
		{
			if(remote_control.switch_right == Switch_Up)
			{
				TotalStep = 15;
			}
			if(remote_control.switch_right == Switch_Middle)
			{
				TotalStep = 11;
			}
			if(remote_control.switch_right == Switch_Down)
			{
				TotalStep = 0;
			}
		}
		if(remote_control.switch_left == Switch_Down)								//�Ӿ�Ѳ��ģʽ
		{
			AutoRunTotalHandle();
			
		}
	}
	
}



void YQY_GoAddMap(double distance, float ang)									//���߾��룬ת��Ƕ�		��ӵ�ͼ
{
	uint8_t i = 0;
	PY_2022.map[PY_2022.YQY_GoStep][0] = distance;					//����·����
	PY_2022.map[PY_2022.YQY_GoStep][1] = ang;								//����·����
	PY_2022.YQY_GoStep ++;																	//·������++
//printf("\n\nAddMapSuccess!Now:\n");
//	for(i = 0; i < PY_2022.YQY_GoStep;i++)
//	{
//		printf("Num:%d,Len:%.2f,Ang:%.2f\n", i,PY_2022.map[i][0], PY_2022.map[i][1]);
//	}
}
void YQY_Go(double distance, float ang)												//���߾��룬ת��Ƕ�
{
	//��ģʽ
	PY_2022.turnAng = ang;
	PY_2022.YQY_GoShouldDo = 1;	//ʹ������·��
	PY_2022.TargetDis = distance + PY_2022.leftAng;
	if(distance < 0)PY_2022.directionFlag=1;
	else PY_2022.directionFlag=0;
}
void YQY_GoDecodeMap_Handle(void)				//�����ͼ
{
	//static uint8_t PinNow = 0;						//��ǰ����λ��
	uint8_t i = 0;
	if((!PY_2022.YQY_GoShouldDo) && (PY_2022.YQY_GoStep != 0))						//�����굱ǰ���񣬴��ڿ��У��������б�������
	{
		YQY_Go(PY_2022.map[0][0],PY_2022.map[0][1]);		//�Զ������¸�����
		
		for(i = 0; i < PY_2022.YQY_GoStep; i++)					//װ����һ������
		{
			PY_2022.map[i][0] = PY_2022.map[i+1][0];
			PY_2022.map[i][1] = PY_2022.map[i+1][1];
		}
		PY_2022.YQY_GoStep --;													//������������
	}
}
void YQY_Go_Handle(void)
{
	YQY_GoDecodeMap_Handle();
	
	if(PY_2022.YQY_GoShouldDo)						//���д˴�·��
	{
		
		if(HAL_GetTick() - PY_2022.lastGoTime > 10)
		{
			PY_2022.lastGoTime = HAL_GetTick();
			
			if(PY_2022.directionFlag==1)					//����
			{
				PY_2022.leftAng -= 10;
				PY_2022.rightAng -= 10;
				if(PY_2022.leftAng <= PY_2022.TargetDis)
				{
					if(PY_2022.YQY_GoStep==0)TotalStep++;
					//printf("finish1:Now:%.2f, tar:%.2f\n", PY_2022.leftAng,PY_2022.TargetDis);
					PY_2022.YQY_GoShouldDo = 0;
					return;
				}
			}
			else																	//ǰ��
			{
				PY_2022.leftAng += 10;
				PY_2022.rightAng += 10;
				if(PY_2022.leftAng >= PY_2022.TargetDis)
				{
					if(PY_2022.YQY_GoStep==0)TotalStep++;
					//printf("finish2:Now:%.2f, tar:%.2f\n", PY_2022.leftAng,PY_2022.TargetDis);
					PY_2022.YQY_GoShouldDo = 0;
					return;
				}
			}
		}
	}
}
void AutoRunHandle(void)
{
	static uint8_t dingFlag = 0;						//���м�⣬�ж϶���
	static uint8_t dingCishu = 0;						//��¼���˼�������
	
	uint8_t QHH_XunXian = HYQY_recvInfo[0].recvRes[1];
	
	
	PY_2022.leftAng  += 4;
	PY_2022.rightAng += 4;									//������ǰ��
	
	//printf("recv:%d,%d\n", HYQY_recvInfo[0].recvRes[1], HYQY_recvInfo[0].recvRes[2]);
		if((QHH_XunXian > 0 ) && (QHH_XunXian <= 25))
		{
			PY_2022.turnAng = 15;
			turnServoCtrl(15);
		}
		if((QHH_XunXian > 25) && (QHH_XunXian <= 35))
		{
			PY_2022.turnAng = 7;
			turnServoCtrl(7);
		}
		if((QHH_XunXian > 35) && (QHH_XunXian <= 45))
		{
			PY_2022.turnAng = 5;
			turnServoCtrl(5);
		}
		if((QHH_XunXian > 45) && (QHH_XunXian <= 55))			//��
		{
			PY_2022.turnAng = 0;
			turnServoCtrl(0);
		}
		if((QHH_XunXian > 55) && (QHH_XunXian <= 65))
		{
			PY_2022.turnAng = -5;
			turnServoCtrl(-5);
		}
		if((QHH_XunXian > 65) && (QHH_XunXian <= 75))
		{
			PY_2022.turnAng = -7;
			turnServoCtrl(-7);
		}
		if((QHH_XunXian > 75) && (QHH_XunXian <= 100))
		{
			PY_2022.turnAng = -10;
			turnServoCtrl(-10);
		}
		
		if((HYQY_recvInfo[0].recvRes[2] == 10) && (dingFlag == 0))dingFlag = 1;
		if((HYQY_recvInfo[0].recvRes[2] == 00) && (dingFlag == 1))dingFlag = 2;
		if((HYQY_recvInfo[0].recvRes[2] == 01) && (dingFlag == 2))dingFlag = 3;
		if(dingFlag == 3)
		{
			playNote('1',50);
			dingFlag = 0;
			dingCishu ++;
			TotalStep ++;																					//�����в���++
		}
	
}
void PowerOff(void)
{
	SetLeftMotorRAW(0,0,0,0);
}

void AutoRunTotalHandle(void)																//���Զ�����
{
	static uint32_t lastStepTime = 0;
	if(TotalStep == 0)AutoRunHandle();												//�Զ�Ѳ��
	if(TotalStep == 1)AutoRunHandle();												//�Զ�Ѳ��
	if(TotalStep == 2)AutoRunHandle();												//�Զ�Ѳ��
	
	if(TotalStep == 3)
	{
		PowerOff();
		playNote('7',1000);
		YQY_GoAddMap(550,-60);
		YQY_GoAddMap(-1690,60);
		YQY_GoAddMap(-654,0);
		TotalStep ++;																						//4�Ž��е���Զ����ƺ��Զ��������
	}
	if(TotalStep == 4)lastStepTime = HAL_GetTick();						//һֱ��ˢʱ��
	if(TotalStep == 5)
	{
		if(HAL_GetTick() - lastStepTime > 1500)TotalStep++;
	}
	if(TotalStep == 6)
	{
		PowerOff();
		playNote('7',1000);
		HAL_Delay(4000);
		TotalStep ++;					//������һ������
	}
	if(TotalStep == 7)			//����
	{
		ServoSpark();
		YQY_GoAddMap(880,0);
		YQY_GoAddMap(1300,60);
		YQY_GoAddMap(-934,-60);
		TotalStep ++;																						//4�Ž��е���Զ����ƺ��Զ��������
	}
	if(TotalStep == 8)lastStepTime = HAL_GetTick();						//һֱ��ˢʱ��
	if(TotalStep == 9)
	{
		if(HAL_GetTick() - lastStepTime > 1000)TotalStep++;
	}
	if(TotalStep == 10)AutoRunHandle();												//�Զ�Ѳ��
	if(TotalStep == 11)AutoRunHandle();												//�Զ�Ѳ��
	if(TotalStep == 12)AutoRunHandle();												//�Զ�Ѳ��
	if(TotalStep == 13)AutoRunHandle();												//�Զ�Ѳ��
	if(TotalStep == 14)AutoRunHandle();												//�Զ�Ѳ��
	if(TotalStep == 15)
	{
		PowerOff();
		playNote('7',1000);
		YQY_GoAddMap(650,0);
		YQY_GoAddMap(-1210,60);
		YQY_GoAddMap(-250,0);
		YQY_GoAddMap(-1000,-60);
		TotalStep ++;
	}
	if(TotalStep == 16)lastStepTime = HAL_GetTick();						//һֱ��ˢʱ��
	if(TotalStep == 17)
	{
		if(HAL_GetTick() - lastStepTime > 2000)TotalStep++;
	}
	if(TotalStep == 18)
	{
		PowerOff();
		playNote('7',2000);
		HAL_Delay(3000);
		TotalStep ++;					//������һ������
	}
	if(TotalStep == 19)			//����
	{
		//ServoSpark();
		YQY_GoAddMap(670,-60);	
		YQY_GoAddMap(550,0);
		YQY_GoAddMap(1430,30);
		TotalStep ++;																						//4�Ž��е���Զ����ƺ��Զ��������
	}
	if(TotalStep == 20)lastStepTime = HAL_GetTick();						//һֱ��ˢʱ��
	if(TotalStep == 21)
	{
		if(HAL_GetTick() - lastStepTime > 500)TotalStep++;
	}
	if(TotalStep == 22)AutoRunHandle();													//�Զ�Ѳ��
	
}

void Dian2022Ctrl_Handle(void)																//�ܿ��ƾ��
{
	Dian2022GetCtrlInfo();																			//��ȡ������
	
	turnServoCtrl(PY_2022.turnAng);															//����ת��Ƕ�
	
	YQY_Go_Handle();																						//��ͼ���Զ�����
	
	SetMotorByPID_Position(PY_2022.leftAng, -PY_2022.rightAng);	//����Ŀ��
	
	Dian2022MotorCtrl_Handle();																	//PID����
	
	printf("Li:%.2f,%.2f,%.2f,%d\n", PY_2022.leftAng, PY_2022.rightAng,PY_2022.turnAng,TotalStep);
}

