#ifndef __POWER_H
#define __POWER_H

#include "main.h"		//�ڲ�����#include "stm32f4xx_hal.h"


/*
˵����
����4· 24v ��� ��� 20A

RM-A�Ϳ�����ʾ��ͼ������Դ�ӿ����±�ţ�

        |                   |
        |                   |
        |                   |
        |       R��־       |
        |                   |
        |                   |
        |                   |
        |                   |
        |(4)            (2) |
        |(3)            (1) |

*/

extern void power_on_24v_ALL(void);             //�ϵ绺����
extern void power_off_24v_ALL(void);            //�ر�����24V��Դ
extern void power_on_24v(uint8_t num);          //��ĳһ·��Դ
extern void power_off_24v(uint8_t num);         //�ر�ĳһ·��Դ

#endif      //__POWER_H
