
/*
˵����

��������  KEY ������ͬʱҲ��ΪBOOT 1 ʹ�ã�

*/

#ifndef __key_h
#define __key_h

#include "main.h"		//�ڰ���#include "stm32f4xx_hal.h"


#define KEY_IS_PRESSED (HAL_GPIO_ReadPin(KEY_GPIO_Port,KEY_Pin) == GPIO_PIN_SET)

#define LIMIT_IS_PRESSED_AS  (HAL_GPIO_ReadPin(LIMIT_AS_GPIO_Port,LIMIT_AS_Pin) == GPIO_PIN_RESET)		//��λ���أ�RM-A������-W ���
#define LIMIT_IS_PRESSED_DS  (HAL_GPIO_ReadPin(LIMIT_DS_GPIO_Port,LIMIT_DS_Pin) == GPIO_PIN_RESET)		//��λ���أ�RM-A������-W �Һ�
#define LIMIT_IS_PRESSED_AW  (HAL_GPIO_ReadPin(LIMIT_AW_GPIO_Port,LIMIT_AW_Pin) == GPIO_PIN_RESET)		//��λ���أ�RM-A������-W ��ǰ
#define LIMIT_IS_PRESSED_DW  (HAL_GPIO_ReadPin(LIMIT_DW_GPIO_Port,LIMIT_DW_Pin) == GPIO_PIN_RESET)		//��λ���أ�RM-A������-W ��ǰ


extern void keyTest(void);
extern void LimitTest(void);//������λ����

#endif //__key_h
