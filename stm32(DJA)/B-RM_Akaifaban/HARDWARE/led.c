/*
˵����

LED�ж�������R������������LED������������Ϊ1-8

��ʹ�ú������£�
	extern void ledTurnOffAll(void);								//�ر�ȫ��LED
	extern void ledCtrl(char led,uint8_t status);		//����led������ 1-8  R G  ѡ��Ҫ�����ĸ�   ����0�ص� 1����
	extern void ledToggle(char led);								//��תled������ 1-8  R G  ѡ��Ҫ�����ĸ�
	extern void led_Test(void);											//�ú���������ʱ��Ϊ��ˮ��Ч���������ʹ��


					
					2022 7 7 ��ݸ  YQY��д

*/
#include "led.h"


void ledCtrl(char led,uint8_t status)		//����led������ 1-8  R G L(����) ѡ��Ҫ�����ĸ�   ����0�ص� 1����
{
	if(status != 0)			//����
	{
		switch (led)
		{
			case '1':HAL_GPIO_WritePin(LED_1_GPIO_Port,LED_1_Pin,GPIO_PIN_RESET);break;
			case '2':HAL_GPIO_WritePin(LED_2_GPIO_Port,LED_2_Pin,GPIO_PIN_RESET);break;
			case '3':HAL_GPIO_WritePin(LED_3_GPIO_Port,LED_3_Pin,GPIO_PIN_RESET);break;
			case '4':HAL_GPIO_WritePin(LED_4_GPIO_Port,LED_4_Pin,GPIO_PIN_RESET);break;
			case '5':HAL_GPIO_WritePin(LED_5_GPIO_Port,LED_5_Pin,GPIO_PIN_RESET);break;
			case '6':HAL_GPIO_WritePin(LED_6_GPIO_Port,LED_6_Pin,GPIO_PIN_RESET);break;
			case '7':HAL_GPIO_WritePin(LED_7_GPIO_Port,LED_7_Pin,GPIO_PIN_RESET);break;
			case '8':HAL_GPIO_WritePin(LED_8_GPIO_Port,LED_8_Pin,GPIO_PIN_RESET);break;
			case 'R':HAL_GPIO_WritePin(LED_RED_GPIO_Port,LED_RED_Pin,GPIO_PIN_RESET);break;
			case 'G':HAL_GPIO_WritePin(LED_GREEN_GPIO_Port,LED_GREEN_Pin,GPIO_PIN_RESET);break;
			case 'L':HAL_GPIO_WritePin(LASTER_GPIO_Port,LASTER_Pin,GPIO_PIN_SET);break;
			default: break;
		}
	}
	else								//�ص�
	{
		switch (led)		
		{
			case '1':HAL_GPIO_WritePin(LED_1_GPIO_Port,LED_1_Pin,GPIO_PIN_SET);break;
			case '2':HAL_GPIO_WritePin(LED_2_GPIO_Port,LED_2_Pin,GPIO_PIN_SET);break;
			case '3':HAL_GPIO_WritePin(LED_3_GPIO_Port,LED_3_Pin,GPIO_PIN_SET);break;
			case '4':HAL_GPIO_WritePin(LED_4_GPIO_Port,LED_4_Pin,GPIO_PIN_SET);break;
			case '5':HAL_GPIO_WritePin(LED_5_GPIO_Port,LED_5_Pin,GPIO_PIN_SET);break;
			case '6':HAL_GPIO_WritePin(LED_6_GPIO_Port,LED_6_Pin,GPIO_PIN_SET);break;
			case '7':HAL_GPIO_WritePin(LED_7_GPIO_Port,LED_7_Pin,GPIO_PIN_SET);break;
			case '8':HAL_GPIO_WritePin(LED_8_GPIO_Port,LED_8_Pin,GPIO_PIN_SET);break;
			case 'R':HAL_GPIO_WritePin(LED_RED_GPIO_Port,LED_RED_Pin,GPIO_PIN_SET);break;
			case 'L':HAL_GPIO_WritePin(LASTER_GPIO_Port,LASTER_Pin,GPIO_PIN_RESET);break;
			default: break;
		}
	}

}

void ledToggle(char led)									//��תled������ 1-8  R G  ѡ��Ҫ�����ĸ�
{
	switch (led)		
	{
		case '1':HAL_GPIO_TogglePin(LED_1_GPIO_Port,LED_1_Pin);break;
		case '2':HAL_GPIO_TogglePin(LED_2_GPIO_Port,LED_2_Pin);break;
		case '3':HAL_GPIO_TogglePin(LED_3_GPIO_Port,LED_3_Pin);break;
		case '4':HAL_GPIO_TogglePin(LED_4_GPIO_Port,LED_4_Pin);break;
		case '5':HAL_GPIO_TogglePin(LED_5_GPIO_Port,LED_5_Pin);break;
		case '6':HAL_GPIO_TogglePin(LED_6_GPIO_Port,LED_6_Pin);break;
		case '7':HAL_GPIO_TogglePin(LED_7_GPIO_Port,LED_7_Pin);break;
		case '8':HAL_GPIO_TogglePin(LED_8_GPIO_Port,LED_8_Pin);break;
		case 'R':HAL_GPIO_TogglePin(LED_RED_GPIO_Port,LED_RED_Pin);break;
		case 'G':HAL_GPIO_TogglePin(LED_GREEN_GPIO_Port,LED_GREEN_Pin);break;
		case 'L':HAL_GPIO_TogglePin(LASTER_GPIO_Port,LASTER_Pin);break;
		default: break;
	}
}
void ledTurnOffAll(void)								//�ر�ȫ��LED
{
	uint8_t i = 0;
	for(i = '1'; i < '9'; i++)ledCtrl(i,0);ledCtrl('R',0);ledCtrl('G',0);
}
#define TEST_LED_DELAY_TIME  100
void led_Test(void)											//�ú���������ʱ��Ϊ��ˮ��Ч���������ʹ��
{
	uint8_t i = 0;
	for(i = '1'; i < '9'; i++)
	{
		ledCtrl(i,1);
		HAL_Delay(TEST_LED_DELAY_TIME);
	}
	ledCtrl('R',1);HAL_Delay(TEST_LED_DELAY_TIME);
	ledCtrl('G',1);HAL_Delay(TEST_LED_DELAY_TIME);
	for(i = '1'; i < '9'; i++)
	{
		ledCtrl(i,0);
		HAL_Delay(TEST_LED_DELAY_TIME);
	}
	ledCtrl('R',0);HAL_Delay(TEST_LED_DELAY_TIME);
	ledCtrl('G',0);HAL_Delay(TEST_LED_DELAY_TIME);
	HAL_GPIO_TogglePin(LED_GREEN_GPIO_Port,LED_GREEN_Pin);
	HAL_Delay(TEST_LED_DELAY_TIME);
	HAL_GPIO_TogglePin(LED_RED_GPIO_Port,LED_RED_Pin);
	HAL_Delay(TEST_LED_DELAY_TIME);
	HAL_GPIO_TogglePin(LED_GREEN_GPIO_Port,LED_GREEN_Pin);
	HAL_Delay(TEST_LED_DELAY_TIME);
	HAL_GPIO_TogglePin(LED_RED_GPIO_Port,LED_RED_Pin);
	HAL_Delay(TEST_LED_DELAY_TIME);
}

