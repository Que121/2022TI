/*
说明：

LED判定方法：R标向上正立，LED从下往上依次为1-8

可使用函数如下：
	extern void keyTest(void);																													//测试KEY是否可用
	
可使用宏定义如下：
	#define KEY_IS_PRESSED (HAL_GPIO_ReadPin(KEY_GPIO_Port,KEY_Pin) == GPIO_PIN_SET)		//返回按键是否被按下


					2022 7 7 东莞  YQY编写

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



