
#include "myUART.h"
#include "stdio.h"
#include "DR16.h"
#include "myUmpire.h"				//用于处理裁判系统相关东西
#include "MyConfig.h"				//包含所有可配置的东东！
#include "VisionUART.h"			//和视觉组进行通信

#include "HYQY_RECV.h"

uint8_t qhh_recv[10] = {0};
uint8_t recv_buf[13] = {0};

void My_UART_Init(void)
{
	//HAL_UART_Receive_DMA(&huart6, umpireRxBuffer, UMPIRE_RX_BY_DMA_LENGTH	);	//串口接收 DMA搬运 裁判系统发来的数据
	HAL_UART_Receive_IT(&huart8, qhh_recv	, 1);									//串口接收 DMA搬运 视觉组发来的数据
	HYQY_Recv_Init(&HYQY_recvInfo[0],0);
	//SendVisionData_Init();																										//初始化视觉组的信息
}

//串口中断回调函数
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	/* 判断是哪个串口触发的中断 */
	if(huart ->Instance == USART1)			//遥控器发回来的数据
	{
		DR16_callBackHandle();
	}
	else if(huart ->Instance == USART6)
	{
		//decodeUmpireData_handle();				//裁判系统数据，进行解码
	}
	else if(huart ->Instance == UART8)	//视觉组发回来的数据
	{
		HYQY_Recv_Handle(&HYQY_recvInfo[0],qhh_recv[0]);
		HAL_UART_Receive_IT(&huart8, qhh_recv	, 1);
		//HAL_UART_Receive_IT(&huart8, recv_buf	, 8	);
		
	}
}







/*  以下为重映射相关操作  */



//取消ARM的半主机工作模式
#pragma import(__use_no_semihosting)                             
struct __FILE { 
	int handle; 
}; 

FILE __stdout;          
void _sys_exit(int x) 
{ 
	x = x; 
}
//重定向printf到串口7（A板上标注有7的位置）
int fputc(int ch, FILE *f){      
	while((UART7 -> SR & 0X40) == 0){}
    UART7->DR = (uint8_t )ch;
	return ch;
}







