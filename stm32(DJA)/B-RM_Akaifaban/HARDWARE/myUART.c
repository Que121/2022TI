
#include "myUART.h"
#include "stdio.h"
#include "DR16.h"
#include "myUmpire.h"				//���ڴ������ϵͳ��ض���
#include "MyConfig.h"				//�������п����õĶ�����
#include "VisionUART.h"			//���Ӿ������ͨ��

#include "HYQY_RECV.h"

uint8_t qhh_recv[10] = {0};
uint8_t recv_buf[13] = {0};

void My_UART_Init(void)
{
	//HAL_UART_Receive_DMA(&huart6, umpireRxBuffer, UMPIRE_RX_BY_DMA_LENGTH	);	//���ڽ��� DMA���� ����ϵͳ����������
	HAL_UART_Receive_IT(&huart8, qhh_recv	, 1);									//���ڽ��� DMA���� �Ӿ��鷢��������
	HYQY_Recv_Init(&HYQY_recvInfo[0],0);
	//SendVisionData_Init();																										//��ʼ���Ӿ������Ϣ
}

//�����жϻص�����
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	/* �ж����ĸ����ڴ������ж� */
	if(huart ->Instance == USART1)			//ң����������������
	{
		DR16_callBackHandle();
	}
	else if(huart ->Instance == USART6)
	{
		//decodeUmpireData_handle();				//����ϵͳ���ݣ����н���
	}
	else if(huart ->Instance == UART8)	//�Ӿ��鷢����������
	{
		HYQY_Recv_Handle(&HYQY_recvInfo[0],qhh_recv[0]);
		HAL_UART_Receive_IT(&huart8, qhh_recv	, 1);
		//HAL_UART_Receive_IT(&huart8, recv_buf	, 8	);
		
	}
}







/*  ����Ϊ��ӳ����ز���  */



//ȡ��ARM�İ���������ģʽ
#pragma import(__use_no_semihosting)                             
struct __FILE { 
	int handle; 
}; 

FILE __stdout;          
void _sys_exit(int x) 
{ 
	x = x; 
}
//�ض���printf������7��A���ϱ�ע��7��λ�ã�
int fputc(int ch, FILE *f){      
	while((UART7 -> SR & 0X40) == 0){}
    UART7->DR = (uint8_t )ch;
	return ch;
}







