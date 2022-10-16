#ifndef __SUPER_CAP_h
#define __SUPER_CAP_h

#include "main.h"		//�ڰ���#include "stm32f4xx_hal.h"

#include "bsp_can.h"//内有底层的设置功率接口
/*
超级电容模组，连接到can1总线上

YQY  2022 7 12

*/

typedef struct{
	float Vot_input;			//输入模块的电压
	float Vot_cap;				//电容组电压
	float Cur_input;			//输入电流
	float Set_power;			//我们之前设定的功率（这里是反馈回来的功率，仅做验证）
	
	float power;					//此项由上述 电压*电流计算得到！
	
}SuperCap_t;

extern SuperCap_t SuperCap;

extern void SuperCap_GetInformation(uint8_t * RAWdata);		//获取超级电容相关信息 放到can接收中断里！


#endif//__SUPER_CAP_h





