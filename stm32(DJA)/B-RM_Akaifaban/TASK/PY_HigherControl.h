#ifndef _PY_HIGHER_CONTROL_H
#define _PY_HIGHER_CONTROL_H

#include <math.h>
#include "main.h"
#include "DR16.h"
#include "motor.h"



/*   �ڲ�����   */

void VisionMotorCtrl_Handle(void);		//ͼ����̨�����߼�
void hydrocooling_Handle(void);				//ˮ�����(����Ҫ�����ӵ��߼�����������ӣ�)
void carBackingRadar_Handle(void);		//�����״� ���н�ģʽ�ر�


/*   �ⲿ����   */

extern void PY_PowerOff(void);						//�����ر����������
extern void SafeLock(void);								//��ȫ��
extern void HigherCtrl_Hnadle(void);			//�߼����ƾ�����ڲ�������������������������ŵ�main.c����

#endif	//_PY_HIGHER_CONTROL_H


