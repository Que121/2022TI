/*
本程序可实现DR16接收机解码，

串口配置成 100000  9-E-1
DMA回环接收

					2022 7 7		YQY部分重写
*/


#include "DR16.h"
#include "led.h"

#define DEBUG_MODE_UART6 0

uint8_t* DR16RawBuffer[20] = {0};			//用于存储接收机发来的原生数据
RC_Type remote_control;
uint32_t  Latest_Remote_Control_Pack_Time = 0;
uint32_t  LED_Flash_Timer_remote_control = 0;


void DR16_Init(void)									//DR16接收机初始化
{
	HAL_UART_Receive_DMA(&huart1,(uint8_t*)DR16RawBuffer,18);
}

void DR16_callBackHandle(void)				//放DMA传输完成中断回调函数里！
{
	//printf("success\n");
	Callback_RC_Handle(&remote_control, (uint8_t*)DR16RawBuffer);
	
	#if DEBUG_MODE_UART6
	printf("ch1-4:%d %d %d %d ",remote_control.ch1,remote_control.ch2,remote_control.ch3,remote_control.ch4);
	printf("SW1-2 %d %d\n",remote_control.switch_left,remote_control.switch_right);
	printf("DR16 ch1-4:%d-%d-%d-%d mouse %d-%d-%d\n",remote_control.ch1,remote_control.ch2,remote_control.ch3,remote_control.ch4,remote_control.mouse.x,remote_control.mouse.y,remote_control.mouse.z);
	#endif
}

/*******************************************************************************************
  * @Func		void Callback_RC_Handle(RC_Type* rc, uint8_t* buff)
  * @Brief  DR16接收机协议解码程序
  * @Param		RC_Type* rc　存储遥控器数据的结构体　　uint8_t* buff　用于解码的缓存
  * @Retval		None
  * @Date    2022 7 7		YQY部分重写
 *******************************************************************************************/
void Callback_RC_Handle(RC_Type* rc, uint8_t* buff)
{
//	rc->ch1 = (*buff | *(buff+1) << 8) & 0x07FF;	offset  = 1024
	rc->ch1 = (buff[0] | buff[1]<<8) & 0x07FF;
	rc->ch1 -= 1024;
	rc->ch2 = (buff[1]>>3 | buff[2]<<5 ) & 0x07FF;
	rc->ch2 -= 1024;
	rc->ch3 = (buff[2]>>6 | buff[3]<<2 | buff[4]<<10) & 0x07FF;
	rc->ch3 -= 1024;
	rc->ch4 = (buff[4]>>1 | buff[5]<<7) & 0x07FF;		
	rc->ch4 -= 1024;
	
	rc->switch_left = ( (buff[5] >> 4)& 0x000C ) >> 2;
	rc->switch_right =  (buff[5] >> 4)& 0x0003 ;
	
	rc->mouse.x = buff[6] | (buff[7] << 8);	//x axis
	rc->mouse.y = buff[8] | (buff[9] << 8);
	rc->mouse.z = buff[10]| (buff[11] << 8);
	
	rc->mouse.press_left 	= buff[12];	// is pressed?
	rc->mouse.press_right = buff[13];
	
	rc->keyBoard.key_code = buff[14] | buff[15] << 8; //key borad code
	
	Latest_Remote_Control_Pack_Time = HAL_GetTick();
	
	if(Latest_Remote_Control_Pack_Time - LED_Flash_Timer_remote_control>500){
			ledToggle('R');
			LED_Flash_Timer_remote_control = Latest_Remote_Control_Pack_Time;
	}
}


void DR16TestByLedBlink(void)
{
		uint8_t i = 0;
	
	if		 (KEYBOARD_PRESSED_W)			ledCtrl('1',1);
	else if(KEYBOARD_PRESSED_S)			ledCtrl('2',1);
	else if(KEYBOARD_PRESSED_A)			ledCtrl('3',1);
	else if(KEYBOARD_PRESSED_D)			ledCtrl('4',1);
	else if(KEYBOARD_PRESSED_SHIFT)	ledCtrl('5',1);
	else if(KEYBOARD_PRESSED_CTRL)	ledCtrl('6',1);
	else if(KEYBOARD_PRESSED_Q)			ledCtrl('7',1);
	else if(KEYBOARD_PRESSED_E)			ledCtrl('8',1);
	else	
	{
		for(i = 0; i < 8; i++)ledCtrl('1'+i,0);
	}
	
//	if		 (KEYBOARD_PRESSED_R)			ledCtrl('1',1);
//	else if(KEYBOARD_PRESSED_F)			ledCtrl('2',1);
//	else if(KEYBOARD_PRESSED_G)			ledCtrl('3',1);
//	else if(KEYBOARD_PRESSED_Z)			ledCtrl('4',1);
//	else if(KEYBOARD_PRESSED_X)			ledCtrl('5',1);
//	else if(KEYBOARD_PRESSED_C)			ledCtrl('6',1);
//	else if(KEYBOARD_PRESSED_V)			ledCtrl('7',1);
//	else	
//	{
//		for(i = 0; i < 8; i++)ledCtrl('1'+i,0);
//	}
	
}

