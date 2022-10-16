/*
说明：

LED判定方法：R标向上正立，LED从下往上依次为1-8

可使用函数如下：
	extern void ledTurnOffAll(void);								//关闭全部LED
	extern void ledCtrl(char led,uint8_t status);		//开关led：输入 1-8  R G  选择要控制哪个   输入0关灯 1开灯
	extern void ledToggle(char led);								//翻转led：输入 1-8  R G  选择要控制哪个
	extern void led_Test(void);											//该函数内有延时！为流水灯效果，请谨慎使用


					
					2022 7 7 东莞  YQY编写

*/
#include "led.h"


void ledCtrl(char led,uint8_t status)		//开关led：输入 1-8  R G L(激光) 选择要控制哪个   输入0关灯 1开灯
{
	if(status != 0)			//开灯
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
	else								//关灯
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

void ledToggle(char led)									//翻转led：输入 1-8  R G  选择要控制哪个
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
void ledTurnOffAll(void)								//关闭全部LED
{
	uint8_t i = 0;
	for(i = '1'; i < '9'; i++)ledCtrl(i,0);ledCtrl('R',0);ledCtrl('G',0);
}
#define TEST_LED_DELAY_TIME  100
void led_Test(void)											//该函数内有延时！为流水灯效果，请谨慎使用
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

