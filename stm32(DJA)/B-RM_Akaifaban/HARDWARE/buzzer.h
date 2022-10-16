#ifndef __buzzer_h
#define __buzzer_h

#include "main.h"		//内包含#include "stm32f4xx_hal.h"


/*
说明：
LED判定方法：R标向上正立，LED从下往上依次为1-8

*/



extern void buzzer_Init(void);					//蜂鸣器初始化

extern void setVolume(uint16_t Volume);									//设置音量
extern void playNote(char note, uint16_t delayTime);		//传入1-7   延时时间
extern void playDaYu(void);					
extern void buzzerOn(uint16_t delayTime);		//蜂鸣器发生一定时间


#endif      //__buzzer_h

