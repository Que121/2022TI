#ifndef __RM_2022_Diansai
#define __RM_2022_Diansai

#include "main.h"

extern uint8_t TotalStep;

typedef struct
{
	double leftAng;
	double rightAng;
	float turnAng;
	
	double lastDis;
	double TargetDis;
	uint32_t lastGoTime;
	uint8_t YQY_GoShouldDo;																		//�Ƿ�Ӧ����
	uint8_t YQY_GoStep;																				//ʣ�ಽ��
	double map[10][2];																				//·����
	uint8_t directionFlag;
	uint32_t lastCtrlTime;
	
	uint8_t remoteSwitchLeft;		//��¼ң�����ϴε���ֵ
}PY_2022_t;

extern PY_2022_t PY_2022;

void DianSai2022_Init(void);																//��ʼ����Ҫ��Ӳ��

void SetMotorByPID_Position(int32_t left,int32_t right);		//���õ��λ��
void SetMotorByPID_Speed   (int16_t left,int16_t right);		//���õ���ٶ�

void Dian2022GetCtrlInfo(void);															//��ȡ������
void Dian2022MotorCtrl_Handle(void);												//PID���ƾ�����ŵ�main.c
void Dian2022Ctrl_Handle(void);															//�ܿ��ƾ��

void turnServoCtrl(int8_t Angle);
void ServoSpark(void);

void YQY_Go(double distance, float ang);										//���߾��룬ת��Ƕ�		��������
void YQY_GoAddMap(double distance, float ang);							//���߾��룬ת��Ƕ�		��ӵ�ͼ
void YQY_GoDecodeMap_Handle(void);													//�����ͼ
void YQY_Go_Handle(void);																		//���߾��

void AutoRunHandle(void);																		//�Զ�Ѳ��
void AutoRunTotalHandle(void);															//���Զ�����
void PowerOff(void);


#endif
