#ifndef _PY_CHASSIS_H
#define _PY_CHASSIS_H

#include <math.h>
#include "main.h"
#include "DR16.h"
#include "motor.h"
#include "PY_HigherControl.h"

typedef struct
{
	s16 RVx,Vx;//ʵ���ٶȣ��趨�ٶ�
	s16 RVy,Vy;
	s16 AngleCompensate;//��ǰ�ǶȲ���
	float Angle;//��ǰ�Ƕ��趨
	float SAngle,RAngle;//ң���������Ƕȣ�ʵ�ʽǶ�ֵ
}SpinTop_t;

typedef struct
{
	float Vx,Vy,W;			//�����ƶ�
	float Pitch;				//��̨pitch�� ��λ ��
}Remote_t;

extern Remote_t Remote;
extern void TimberWolves_chassis_Handle(void);

#endif	//_PY_CHASSIS_H
