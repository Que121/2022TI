#ifndef __led_h
#define __led_h

#include "main.h"		//�ڰ���#include "stm32f4xx_hal.h"


/*
˵����
LED�ж�������R������������LED������������Ϊ1-8

*/

extern void ledTurnOffAll(void);								//�ر�ȫ��LED
extern void ledCtrl(char led,uint8_t status);		//����led������ 1-8  R G  ѡ��Ҫ�����ĸ�   ����0�ص� 1����
extern void ledToggle(char led);								//��תled������ 1-8  R G  ѡ��Ҫ�����ĸ�

extern void led_Test(void);											//�ú���������ʱ��Ϊ��ˮ��Ч���������ʹ��

#endif





