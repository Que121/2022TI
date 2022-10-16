#ifndef __POWER_H
#define __POWER_H

#include "main.h"		//内部包含#include "stm32f4xx_hal.h"


/*
说明：
板载4路 24v 输出 最大 20A

RM-A型开发板示意图：（电源接口如下标号）

        |                   |
        |                   |
        |                   |
        |       R标志       |
        |                   |
        |                   |
        |                   |
        |                   |
        |(4)            (2) |
        |(3)            (1) |

*/

extern void power_on_24v_ALL(void);             //上电缓启动
extern void power_off_24v_ALL(void);            //关闭所有24V电源
extern void power_on_24v(uint8_t num);          //打开某一路电源
extern void power_off_24v(uint8_t num);         //关闭某一路电源

#endif      //__POWER_H
