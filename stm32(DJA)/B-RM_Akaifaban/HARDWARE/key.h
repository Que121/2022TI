
/*
说明：

按键控制  KEY 按键，同时也作为BOOT 1 使用！

*/

#ifndef __key_h
#define __key_h

#include "main.h"		//内包含#include "stm32f4xx_hal.h"


#define KEY_IS_PRESSED (HAL_GPIO_ReadPin(KEY_GPIO_Port,KEY_Pin) == GPIO_PIN_SET)

#define LIMIT_IS_PRESSED_AS  (HAL_GPIO_ReadPin(LIMIT_AS_GPIO_Port,LIMIT_AS_Pin) == GPIO_PIN_RESET)		//限位开关，RM-A开发板-W 左后
#define LIMIT_IS_PRESSED_DS  (HAL_GPIO_ReadPin(LIMIT_DS_GPIO_Port,LIMIT_DS_Pin) == GPIO_PIN_RESET)		//限位开关，RM-A开发板-W 右后
#define LIMIT_IS_PRESSED_AW  (HAL_GPIO_ReadPin(LIMIT_AW_GPIO_Port,LIMIT_AW_Pin) == GPIO_PIN_RESET)		//限位开关，RM-A开发板-W 左前
#define LIMIT_IS_PRESSED_DW  (HAL_GPIO_ReadPin(LIMIT_DW_GPIO_Port,LIMIT_DW_Pin) == GPIO_PIN_RESET)		//限位开关，RM-A开发板-W 右前


extern void keyTest(void);
extern void LimitTest(void);//测试限位开关

#endif //__key_h
