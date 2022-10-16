/**************************************************************************//**
 * @file     VisionUART.h
 * @brief    与视觉组进行串口通信
 * @version  V1.0
 * @date     6. April 2022
 ******************************************************************************/
/*																文档功能说明：
 *	实现电控与视觉组的串口通信：
 *	通过中断接收视觉组传入的参数
 *	通过定时器溢出中断自动发送当前机器人数据给视觉组（频率可调）
 *
 *
 *																文档使用说明：
 * 	壹：	将 SendVisionData_Init(void); 函数放到main函数中执行，对发送结构体变量赋初值
 *				将 SendVisionDataCircular(VisionDataSend*); 函数放到1ms定时器溢出中断里，实现按规定频率发送数据
 *				将 GetVisionData(VisionDataGet*,uint8_t *); 函数放到串口接收中断内，用来解析数据，解析后的数据在形参1结构体内，形参2为传入串口接收到的字符串
 *				修改 SendDataByUART()函数（位于.c文件结尾），该函数为发送数据的底层实现方式，请根据实况自行修改。
 *
 *	贰：	修改 SendVisionDataCircular_frequency 宏定义可更改发送频率，该数值单位Hz
 *				将需要发送的信息写入visionDataSend结构体，之后自动发送
 *				视觉组发送来的数据会存放到visionDataGet结构体中
 *
 *	叁：	调试：在该头文件配套.c文件中开启//#define UART_DEBUG_MODE宏定义，使能调试模式。
 *				注意！使用该模式，请在魔术棒内勾选use micro lib
 *
 * 本文档联合编写 YQY YLX 华哥
 * Copyright (c) 2022 森林狼机器人实验室 电控组 All rights reserved.
 */
#ifndef __VISION_UART_H
#define __VISION_UART_H

#include "stm32f4xx.h"
#include "MyConfig.h"						//包含所有可配置的东东！


#define SendVisionDataCircular_frequency 1			//修改自动发送的频率 单位Hz

/* 要发送给视觉组的信息 信息一共18个字符 */
typedef struct 
{
	uint8_t head;						//bit[0]			华哥规定！开头就是0x53 （S）
	uint8_t color;					//bit[1]			颜色 0为己方红色 1为己方蓝色 2为己方红蓝双色（调试模式）
	uint8_t mode;						//bit[2]			模式 0为自瞄 1为小能量机关 2为大能量机关 3为打击哨兵模式 4为小陀螺模式 5为录像模式 6为无人机模式 7为哨兵模式 8为雷达模式 其余情况默认为自瞄模式
	uint8_t MyID;						//bit[3]			当前机器人ID 0为英雄 1为工程 2为步兵 3为无人机 4为哨兵
	float yaw_Angle;				//bit[7..4]		yaw  轴的陀螺仪角度数据（以联合体发送）
	float pitch_Angle;			//bit[11..8]	pitch轴的陀螺仪角度数据（以联合体发送）
	int16_t yaw_velocity;		//bit[13..12]	yaw  的陀螺仪数据（低位优先发送）
	int16_t pitch_velocity;	//bit[15..14]	pitch的陀螺仪数据（低位优先发送）
	uint8_t bullet_velocity;//bit[16]			当前子弹速度
	uint8_t end;						//bit[17]			华哥规定！结尾就是0x45	（E）
}VisionDataSend;


/* 从视觉组获取的信息 信息一共13个字符 */
typedef struct 
{
	uint8_t head;						//bit[0]			华哥规定！0x53 （S）
	uint8_t VisionFlag;			//bit[1]			是否识别到目标的标志 0为无目标 1为有目标
	uint8_t VisionShoot;		//bit[2]			是否射击的信号 0为停止射击 1为允许射击
	uint8_t yawFlag;				//bit[3]			yaw 轴的正负号 0为负号 1为正号
	uint16_t yaw;						//bit[5..4]		yaw 轴角度
	uint8_t pitchFlag;			//bit[6]			pitch 轴的正负号 0为负号 1为正号
	uint16_t pitch;					//bit[8..7]		pitch轴角度
	uint16_t depth;					//bit[10..9]	深度信息
	uint8_t QHH_CRC;				//bit[11]			华哥的CRC校验位
	uint8_t end;						//bit[12]			华哥规定！0x45	（E）
	
	int16_t RES_Yaw;				//合成yawFlag和yaw之后得到的结果
	int16_t RES_Pitch;			//同上
}VisionDataGet;

extern uint8_t visionRx[VISION_RX_LENGTH];								//用于存储视觉组发来的原始数据
extern VisionDataSend visionDataSend;											//外部可访问 用于存放需要发送给视觉组的数据
extern VisionDataGet  visionDataGet;											//外部可访问 用于存放视觉组发来的数据，该数据可通过void GetVisionData()更新

extern void SendVisionData_Init(void);										//调用该API以填入 帧头 帧未 等信息											(建议放到 main.c初始化部分)
extern void SendVisionData(VisionDataSend*);							//调用该函数发送视觉信息给视觉组												(建议放到定时器溢出中断里)
extern void GetVisionData(VisionDataGet*,uint8_t *);			//调用该函数获取视觉信息，视觉信息会传到 形参结构体 里		(建议放到UART接收中断里！)

extern void SendVisionDataCircular(VisionDataSend*);			//将该函数放到定时器溢出中断中（溢出时间1ms）

//需要重映射的API，向上位机发送数据会调用该函数，向指定串口发送形参指针所指数组（连续读取18个字符）
//该函数位于VisionUART.c文件内
void SendDataByUART(uint8_t*);



#endif		//__VISION_UART_H
