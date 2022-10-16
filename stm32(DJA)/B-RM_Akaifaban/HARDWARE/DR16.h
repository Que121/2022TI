
/*
说明：

遥控器接收机解码函数

*/

#ifndef __DR16_h
#define __DR16_h

#include "main.h"		//内包含#include "stm32f4xx_hal.h"
#include "usart.h"
#include "led.h"

//以下宏定义直接调用即可！
#define KEYBOARD_LOOSEN_ALL				(remote_control.keyBoard.key_code == 0x00		 )		//所有按键都松开
#define KEYBOARD_LOOSEN_WASD			(!(remote_control.keyBoard.key_code & 0x0f  ))		//松开wasd按键？？有bug？
#define KEYBOARD_PRESSED_W 				(remote_control.keyBoard.key_code & (1 <<  0))
#define KEYBOARD_PRESSED_S 				(remote_control.keyBoard.key_code & (1 <<  1))
#define KEYBOARD_PRESSED_A 				(remote_control.keyBoard.key_code & (1 <<  2))
#define KEYBOARD_PRESSED_D 				(remote_control.keyBoard.key_code & (1 <<  3))
#define KEYBOARD_PRESSED_SHIFT 		(remote_control.keyBoard.key_code & (1 <<  4))
#define KEYBOARD_PRESSED_CTRL 		(remote_control.keyBoard.key_code & (1 <<  5))
#define KEYBOARD_PRESSED_Q 				(remote_control.keyBoard.key_code & (1 <<  6))
#define KEYBOARD_PRESSED_E 				(remote_control.keyBoard.key_code & (1 <<  7))
#define KEYBOARD_PRESSED_R 				(remote_control.keyBoard.key_code & (1 <<  8))
#define KEYBOARD_PRESSED_F 				(remote_control.keyBoard.key_code & (1 <<  9))
#define KEYBOARD_PRESSED_G 				(remote_control.keyBoard.key_code & (1 << 10))
#define KEYBOARD_PRESSED_Z 				(remote_control.keyBoard.key_code & (1 << 11))
#define KEYBOARD_PRESSED_X 				(remote_control.keyBoard.key_code & (1 << 12))
#define KEYBOARD_PRESSED_C 				(remote_control.keyBoard.key_code & (1 << 13))
#define KEYBOARD_PRESSED_V 				(remote_control.keyBoard.key_code & (1 << 14))

#define MOUSE_LOOSEN_ALL					(	(remote_control.mouse.x ==0) && \
																		(remote_control.mouse.y ==0) && \
																		(remote_control.mouse.x ==0) && \
																		(remote_control.mouse.press_left==0)&&\
																		(remote_control.mouse.press_right==0))
																		

#define DR16_IS_LOOSE ((HAL_GetTick() - Latest_Remote_Control_Pack_Time) > 500 )		//判断遥控器是否离线

typedef struct {
	int16_t ch1;	//each ch value from -660 -- +660
	int16_t ch2;
	int16_t ch3;
	int16_t ch4;
	
	uint8_t switch_left;	//3 value
	uint8_t switch_right;
	
	struct {
		int16_t x;
		int16_t y;
		int16_t z;
	
		uint8_t press_left;
		uint8_t press_right;
	}mouse;
	
	struct {
		uint16_t key_code;
/**********************************************************************************
   * 键盘通道:15   14   13   12   11   10   9   8   7   6     5     4   3   2   1
   *          V    C    X	  Z    G    F    R   E   Q  CTRL  SHIFT  D   A   S   W
************************************************************************************/

	}keyBoard;
	

}RC_Type;



enum{
	Switch_Up = 1,
	Switch_Middle = 3,
	Switch_Down = 2,
};

enum{
	Key_W,
	Key_S,
	//...
};

extern RC_Type remote_control;


extern uint32_t  Latest_Remote_Control_Pack_Time ;			//保存上次接收数据的时间
void Callback_RC_Handle(RC_Type* rc, uint8_t* buff);


extern void DR16_Init(void);														//DR16接收机初始化
extern void DR16_callBackHandle(void);									//放DMA传输完成中断回调函数里！
extern void DR16TestByLedBlink(void);										//放main函数里面，可以测试
#endif //__DR16_h
