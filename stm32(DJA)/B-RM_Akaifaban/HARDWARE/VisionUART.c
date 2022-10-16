/**************************************************************************//**
 * @file     VisionUART.c
 * @brief    与视觉组进行串口通信
 * @version  V1.0
 * @date     6. April 2022
						 2022 7 11
 ******************************************************************************/
 /*
 * 更多信息请参考头文件注释
 */


//#define UART_DEBUG_MODE						//开启调试模式，每调用一次子函数，会显示子函数内参数的数值


#include "VisionUART.h"
#include "usart.h"

#ifdef UART_DEBUG_MODE
	#include "stdio.h"							//需要使用其中的printf用于调试，记得在魔术棒里面勾上use micro lib
#endif

VisionDataSend visionDataSend;		//用于发送给视觉组的结构体
VisionDataGet  visionDataGet;			//用于接收视觉组信息的结构体

uint8_t visionRx[VISION_RX_LENGTH];


void SendVisionData_Init(void)
{
	visionDataSend.head 					 = 0x53;		//帧头 华哥规定0x45
	visionDataSend.color 					 = 0x00;		//初始颜色 0为己方红色 1为己方蓝色 2为己方红蓝双色（调试模式）
	visionDataSend.mode 					 = 0x02;		//模式 0为自瞄 1为小能量机关 2为大能量机关 3为打击哨兵模式 4为小陀螺模式 5为录像模式 6为无人机模式 7为哨兵模式 8为雷达模式 其余情况默认为自瞄模式
	visionDataSend.MyID 					 = 0x03;		//当前机器人ID 0为英雄 1为工程 2为步兵 3为无人机 4为哨兵
	visionDataSend.yaw_Angle   		 = 5.0;			//yaw轴陀螺仪角度   初始值
	visionDataSend.pitch_Angle 		 = 6.0;			//pitch轴陀螺仪角度 初始值
	visionDataSend.yaw_velocity 	 = 11;			//yaw轴速度				 初始值
	visionDataSend.pitch_velocity  = 12;			//pitch轴速度			 初始值
	visionDataSend.bullet_velocity = 66;			//弹丸发射速度			 初始值
	visionDataSend.end 						 = 0x45;		//帧尾 华哥规定0x45
#ifdef UART_DEBUG_MODE
	printf("\n---------SendVisionData_Init Start--------\n\n");
	printf("Data initialization :\n");
	SendVisionData(&visionDataSend);
	printf("\n---------SendVisionData_Init Finish-------\n\n");
#endif
}



union Send_Float_Angle_Information
{
	float angle;
	uint8_t arr_angle[4];
}Send_Float_Angle_Info;

/* 要发送给视觉组的信息 信息一共18个字符*/
void SendVisionData(VisionDataSend* RAW_Data)		//调用该函数发送视觉信息给视觉组
{
	uint8_t data_buffer[18];
	
	data_buffer[0]  = RAW_Data->head;												//帧头
	data_buffer[1]  = RAW_Data->color;											//颜色 0为己方红色 1为己方蓝色 2为己方红蓝双色（调试模式）
	data_buffer[2]  = RAW_Data->mode;												//模式 0为自瞄 1为小能量机关 2为大能量机关 3为打击哨兵模式 4为小陀螺模式 5为录像模式 6为无人机模式 7为哨兵模式 8为雷达模式 其余情况默认为自瞄模式
	data_buffer[3]  = RAW_Data->MyID;												//当前机器人ID 0为英雄 1为工程 2为步兵 3为无人机 4为哨兵
	
	Send_Float_Angle_Info.angle = RAW_Data->yaw_Angle;			//通过 共用体 将float类型拆成4个uint8_t类型 分开字节发送
	data_buffer[4]  = Send_Float_Angle_Info.arr_angle[0];
	data_buffer[5]  = Send_Float_Angle_Info.arr_angle[1];
	data_buffer[6]  = Send_Float_Angle_Info.arr_angle[2];
	data_buffer[7]  = Send_Float_Angle_Info.arr_angle[3];
	
	Send_Float_Angle_Info.angle = RAW_Data->pitch_Angle;		//通过 共用体 将float类型拆成4个uint8_t类型 分开字节发送
	data_buffer[8]  = Send_Float_Angle_Info.arr_angle[0];
	data_buffer[9]  = Send_Float_Angle_Info.arr_angle[1];
	data_buffer[10] = Send_Float_Angle_Info.arr_angle[2];
	data_buffer[11] = Send_Float_Angle_Info.arr_angle[3];
	
	data_buffer[12] = (RAW_Data->yaw_velocity & 0xff);			//yaw  的陀螺仪数据（低位优先发送）
	data_buffer[13] = (RAW_Data->yaw_velocity >> 8);
	data_buffer[14] = (RAW_Data->pitch_velocity & 0xff);		//pitch的陀螺仪数据（低位优先发送）
	data_buffer[15] = (RAW_Data->pitch_velocity >> 8);
	data_buffer[16] = RAW_Data->bullet_velocity;
	data_buffer[17] = RAW_Data->end;												//帧尾
	
	SendDataByUART(data_buffer);														//调用API发送
	
	#ifdef UART_DEBUG_MODE
	printf("\n----------SendVisionData Start-------\n\n");
	printf("head:%x  end: %x \n", RAW_Data->head, RAW_Data->end);
	printf("color:%d  mode: %d ID: %d\n", RAW_Data->color, RAW_Data->mode, RAW_Data->MyID);
	printf("yaw_Angle:%f  pitch_Angle: %f \n", RAW_Data->yaw_Angle, RAW_Data->pitch_Angle);
	printf("yaw_velocity:%d  pitch_velocity: %d \n", RAW_Data->yaw_velocity, RAW_Data->pitch_velocity);
	printf("bullet_velocity:%d  \n", RAW_Data->bullet_velocity);
	printf("\n---------SendVisionData Finish-------\n");
#endif
}


/* 从视觉组获取的信息 信息一共13个字符*/
void GetVisionData(VisionDataGet* myVisionDataGet ,uint8_t * RAW_Data)			//调用该函数获取视觉信息，视觉信息会传到 形参结构体 里
{
	myVisionDataGet->head = RAW_Data[0];
	myVisionDataGet->VisionFlag = RAW_Data[1];
	myVisionDataGet->VisionShoot = RAW_Data[2];
	myVisionDataGet->yawFlag = RAW_Data[3];
	myVisionDataGet->yaw = (RAW_Data[5]<<8)|RAW_Data[4];
	myVisionDataGet->pitchFlag = RAW_Data[6];
	myVisionDataGet->pitch = (RAW_Data[8]<<8)|RAW_Data[7];
	myVisionDataGet->depth = (RAW_Data[10]<<8)|RAW_Data[9];
	myVisionDataGet->QHH_CRC = RAW_Data[11];
	myVisionDataGet->end = RAW_Data[12];
	
	if(myVisionDataGet->yawFlag)myVisionDataGet->RES_Yaw = myVisionDataGet->yaw;
	else myVisionDataGet->RES_Yaw = -((int16_t)(myVisionDataGet->yaw));
	if(myVisionDataGet->pitchFlag)myVisionDataGet->RES_Pitch = myVisionDataGet->pitch;
	else myVisionDataGet->RES_Pitch = -((int16_t)(myVisionDataGet->pitch));
	
#ifdef UART_DEBUG_MODE
	printf("\n------------GetVisionData-----------\n\n");
	printf("head:%x  end: %x \n", myVisionDataGet->head, myVisionDataGet->end);
	printf("VisionFlag:%x  VisionShoot: %x \n", myVisionDataGet->VisionFlag, myVisionDataGet->VisionShoot);
	printf("yaw:%d  pitch: %d \n", myVisionDataGet->RES_Yaw, myVisionDataGet->RES_Pitch);
	printf("depth:%x  QHH_CRC: %x \n", myVisionDataGet->depth, myVisionDataGet->QHH_CRC);
	printf("\n---------GetVisionData Finish-------\n");
#endif
}


//将该函数放到定时器溢出中断中（溢出时间1ms）可实现定时发送。发送频率请修改头文件中SendVisionDataCircular_frequency的数值
void SendVisionDataCircular(VisionDataSend* RAWData)			
{
	static int timeCount = 0;
	if(timeCount++ >= (1000 / SendVisionDataCircular_frequency))
	{
		timeCount = 0;
		SendVisionData(RAWData);
	}
}


/*  以下为需要重映射的API  */
void SendDataByUART(uint8_t* RAWData)					//需要重映射的API，向上位机发送数据会调用该函数，向指定串口发送形参指针所指数组（连续读取18个字符）
{
	//printf("transmate start!\n");
	//HAL_UART_Transmit_DMA(&huart1, RAWData, 18);//通过DMA一次性发送数据
	HAL_UART_Transmit(&huart8, RAWData, 18, 0xff);//通过阻塞一次性发送数据
}






