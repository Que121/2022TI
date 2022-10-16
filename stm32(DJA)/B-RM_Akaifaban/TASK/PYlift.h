#ifndef __PY_LIFT_H
#define __PY_LIFT_H

#include "main.h"																//�ڰ���#include "stm32f4xx_hal.h"

#define REMOTE_REFRESH_TIME				100						//��������Ƶ��10Hz
#define LIFT_HEIGHT_S_MAX						437.5				//�������
#define LIFT_HEIGHT_S_MIN					 	94					//�������
#define LIFT_HEIGHT_W_MAX						383.0				//ǰ������
#define LIFT_HEIGHT_W_MIN					 	12.5				//ǰ������
#define FLEX_LENGTH_W_MAX						440.0				//�е�����
#define FLEX_LENGTH_W_MIN					 	0						//�е�����

typedef struct
{
	double height_S;
	double height_W;
	double length_Flex;														//�������
	int32_t angle_S;
	int32_t angle_W;
	int32_t angle_Flex;														//����Ƕ�
	int32_t angle_Pitch;													//Pitch��ת�Ƕ�
	float angle_FanRoll;													//����Roll
	int16_t angle_VisionYaw;											//�Ӿ���̨Yaw��
	
	float FanPower;																//���̷���
	
	int32_t offset_S;															//ƫ��
	int32_t offset_W;															//ƫ��
	int32_t offset_L;															//ƫ��
	int32_t offset_A;															//ƫ�ƣ��Ƕȣ�
	uint8_t calibrationStatus;										//λ0 W��У׼��� λ1 S��У׼��� λ2 ȫ��У׼���
	
	uint32_t lastCtrlTime;												//��¼�ϴα����ı��ʱ��
	uint32_t lastCalibrateTime;										//��¼�ϴ�У׼ʱ��
}PY_Lift_t;


extern PY_Lift_t PY_Lift;												//̧�����ƽṹ��

extern void PY_Lift_Init(void);									//��ʼ��
extern void PY_Lift_Handle(void);								//���о��������ִ�У�

#endif //__PY_LIFT_H

