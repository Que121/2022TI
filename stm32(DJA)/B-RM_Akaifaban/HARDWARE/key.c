/*
˵����

LED�ж�������R������������LED������������Ϊ1-8

��ʹ�ú������£�
	extern void keyTest(void);																													//����KEY�Ƿ����
	
��ʹ�ú궨�����£�
	#define KEY_IS_PRESSED (HAL_GPIO_ReadPin(KEY_GPIO_Port,KEY_Pin) == GPIO_PIN_SET)		//���ذ����Ƿ񱻰���


					2022 7 7 ��ݸ  YQY��д

*/

#include "key.h"
#include "led.h"


void keyTest(void)
{
	if(KEY_IS_PRESSED)
	{
		ledCtrl('1',1);
		ledCtrl('2',1);
		ledCtrl('7',0);
		ledCtrl('8',0);
	}
	else
	{
		ledCtrl('1',0);
		ledCtrl('2',0);
		ledCtrl('7',1);
		ledCtrl('8',1);
	}
}

void LimitTest(void)
{
	if     (LIMIT_IS_PRESSED_AS)
		ledCtrl('1',1);
	else if(LIMIT_IS_PRESSED_DS)
		ledCtrl('2',1);
	else if(LIMIT_IS_PRESSED_AW)
		ledCtrl('3',1);
	else if(LIMIT_IS_PRESSED_DW)
		ledCtrl('4',1);
	else 
	{
		ledCtrl('1',0);
		ledCtrl('2',0);
		ledCtrl('3',0);
		ledCtrl('4',0);
	}
}



