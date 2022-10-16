#ifndef __led_h
#define __led_h

#include "main.h"		//内包含#include "stm32f4xx_hal.h"


/*
说明：
LED判定方法：R标向上正立，LED从下往上依次为1-8

*/

extern void ledTurnOffAll(void);								//关闭全部LED
extern void ledCtrl(char led,uint8_t status);		//开关led：输入 1-8  R G  选择要控制哪个   输入0关灯 1开灯
extern void ledToggle(char led);								//翻转led：输入 1-8  R G  选择要控制哪个

extern void led_Test(void);											//该函数内有延时！为流水灯效果，请谨慎使用

#endif





