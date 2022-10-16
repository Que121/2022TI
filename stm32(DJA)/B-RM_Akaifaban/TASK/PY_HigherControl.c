#include "PY_HigherControl.h"
#include "PYchassis.h"
#include "key.h"
#include "buzzer.h"
#include "MyPWM.h"
#include "power.h"
#include "PYlift.h"
#include "CPXT.h"
#include "DR16.h"

//�����λ����
void System_Reset(void) 
{
	__set_FAULTMASK(1); //�ر������ж�
	NVIC_SystemReset(); //���������λ
}

void PY_PowerOff(void)
{
	uint8_t i = 0;
	SetChassisMotorByPID(0,0,0,0);
	SetFlexMotorRAW(0,0,0);
	SetLeftMotorRAW(0,0,0,0);
	for(i = 0; i < 4; i++)ChassisMotor[i].PIDmode = stop;
	for(i = 0; i < 4; i++)   LiftMotor[i].PIDmode = stop;
	for(i = 0; i < 2; i++)   FlexMotor[i].PIDmode = stop;
	
	FanPowerCtrl(10);		//�رշ��ȣ����ж������֮ǰλ�ò�����
	power_off_24v_ALL();
	PY_Lift.angle_FanRoll = 0;
}
void SafeLock(void)
{
	uint16_t i = 0;
	printf("Safe Mode Lock\n");
	printf("�ڰ˽���\n");
	while((remote_control.ch2 > -600) || (remote_control.ch2 > -600) || (remote_control.ch3 < 600) || (remote_control.ch4 > -600))
	{
		if(++i % 10 == 0)printf("Remaining restart time %d\n",6 - i / 10);
		if(KEY_IS_PRESSED)break;									//��ͨ�������˳���ȫģʽ
		HAL_Delay(100);
		ledToggle('1');
		if(i > 50)System_Reset();
	}
	buzzerOn(50);
	printf("UnLock_Half\n");
	while((remote_control.ch2 < -600) || (remote_control.ch2 < -600) || (remote_control.ch3 > 600) || (remote_control.ch4 < -600))
	{
		if(KEY_IS_PRESSED)
		{
			remote_control.switch_left = Switch_Down;
			break;									//��ͨ�������˳���ȫģʽ
		}
		HAL_Delay(100);
	}
	playNote('8',100);HAL_Delay(200);playNote('8',200);
	printf("UnLock_Full\n");
}

void VisionMotorCtrl_Handle(void)
{
	static uint8_t VisionMotorFlag = 0;
	static uint32_t lastRunTime = 0;
	static int8_t Angle = 0, targetAng = 0;
	
	if(remote_control.switch_left == Switch_Down)		VisionMotorFlag = 1;			//�н�ģʽ
	if(remote_control.switch_left == Switch_Middle)	VisionMotorFlag = 2;			//����ģʽ
	if(remote_control.switch_left == Switch_Up)			VisionMotorFlag = 3;			//��ȡģʽ
	
	if(HAL_GetTick() - lastRunTime > 10)	//100Hz
	{
		lastRunTime = HAL_GetTick();				//����ʱ�����
		
		switch(VisionMotorFlag)							//�жϵ�ǰ����ģʽ
		{
			case 2:
			{
				targetAng = 85;
				//VisionYawCtrl(90);
			}
				break;
			case 1:
			{
				targetAng = (int8_t)(remote_control.ch3 / 660.0 * 15.0)-5;						//��ͷ�����ƶ������㿪��
				//VisionYawCtrl((int8_t)(remote_control.ch3 / 660.0 * 15.0));
			}
				break;
			case 3:
			{
				targetAng = -75;
				//VisionYawCtrl(-75);
			}
				break;
			default:break;
		}
		if(Angle > targetAng) Angle -= 1;
		if(Angle < targetAng)	Angle += 1;
		
		VisionYawCtrl(Angle);
	}
}
void hydrocooling_Handle(void)			//ˮ�����(����Ҫ�����ӵ��߼�����������ӣ�)
{
	if(remote_control.switch_right == Switch_Down)power_off_24v('1');
	else power_on_24v('1');
}
void carBackingRadar_Handle(void)		//�����״� ���н�ģʽ�ر�
{
	if((remote_control.switch_left == Switch_Down) || (remote_control.switch_right == Switch_Down))power_off_24v('2');
	else power_on_24v('2');
}
void HigherCtrl_Hnadle(void)
{
	VisionMotorCtrl_Handle();					//ͼ����̨��������
	carBackingRadar_Handle();					//�����״� ͼ������
	hydrocooling_Handle();
}

