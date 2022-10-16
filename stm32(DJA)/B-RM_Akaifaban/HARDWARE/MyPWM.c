#include "MyPWM.h"


#include "led.h"
#include "tim.h"


void MyPWM_Init(void)													//��ʼ��PWM
{
	HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_1);		//��� Roll��
	HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_2);		//��� Yaw
	HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_3);		//δ��
	HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_4);		//δ��
	HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_1);		//���
	
	TIM2->CCR1 = 0;
	TIM2->CCR2 = 0;
	TIM2->CCR3 = 0;
	TIM2->CCR4 = 0;
	
	TIM4->CCR1 = 200;														
	
}

void FanPowerCtrl(uint8_t POWER)
{
	if(POWER > 100)return;
	if(POWER <= 10)
	{
		TIM4->CCR1 = 0;
		HAL_GPIO_WritePin(FAN_EN_GPIO_Port,FAN_EN_Pin,GPIO_PIN_RESET);
	}
	else
	{  
		HAL_GPIO_WritePin(FAN_EN_GPIO_Port,FAN_EN_Pin,GPIO_PIN_SET);
		TIM4->CCR1 = POWER * 10;
	}
}

//��ʱ������ 84-1		20000-1
void FanRollCtrl(int8_t Angle)									//-90 - 90
{
	TIM2->CCR1 = 1500 + (int16_t)(Angle/90.0*1000.0);
}

//��ʱ������ 84-1		20000-1
void VisionYawCtrl(int8_t Angle)								//-90 - 90
{
	TIM2->CCR2 = 1500 + (int16_t)(Angle/90.0*1000.0);
}


