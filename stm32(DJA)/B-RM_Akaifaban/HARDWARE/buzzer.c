/*
������ʹ��TIM12



*/

#include "buzzer.h"
#include "tim.h"

uint16_t MyVolume = 30;			//Ĭ������

void buzzer_Init(void)			//��������ʼ��
{
	HAL_TIM_PWM_Start(&htim12, TIM_CHANNEL_1);
	TIM12->CCR1 = 0;
}

void playNote(char note, uint16_t delayTime)	//����1-7   ��ʱʱ��
{
	TIM12->CCR1 = MyVolume;
	switch(note)
	{
		case '1':TIM12->ARR = 1912;break;
		case '2':TIM12->ARR = 1703;break;
		case '3':TIM12->ARR = 1517;break;
		case '4':TIM12->ARR = 1432;break;
		case '5':TIM12->ARR = 1275;break;
		case '6':TIM12->ARR = 1136;break;
		case '7':TIM12->ARR = 1012;break;
		default :TIM12->ARR = 360;break;
	}
	HAL_Delay(delayTime);
	TIM12->CCR1 = 0;
}
void playDaYu(void)
{
	playNote('3',300);
	playNote('2',300);
	playNote('3',300);
	playNote('6',300);
	playNote('3',300);
	playNote('2',300);
	playNote('3',300);
	playNote('7',400);
	playNote('3',300);
	playNote('2',300);
	playNote('3',300);
	playNote('1',300);
	playNote('7',300);
	playNote('5',400);
	HAL_Delay(200);
	playNote('3',300);
	playNote('2',300);
	playNote('3',300);
	playNote('6',300);
	playNote('3',300);
	playNote('2',300);
	playNote('3',300);
	playNote('7',300);
	playNote('5',500);
	playNote('2',300);
	playNote('1',400);
	
	HAL_Delay(200);
}

void buzzerOn(uint16_t delayTime)		//����������һ��ʱ��
{
	playNote('8',delayTime);
}
