/**************************************************************************//**
 * @file     VisionUART.c
 * @brief    ���Ӿ�����д���ͨ��
 * @version  V1.0
 * @date     6. April 2022
						 2022 7 11
 ******************************************************************************/
 /*
 * ������Ϣ��ο�ͷ�ļ�ע��
 */


//#define UART_DEBUG_MODE						//��������ģʽ��ÿ����һ���Ӻ���������ʾ�Ӻ����ڲ�������ֵ


#include "VisionUART.h"
#include "usart.h"

#ifdef UART_DEBUG_MODE
	#include "stdio.h"							//��Ҫʹ�����е�printf���ڵ��ԣ��ǵ���ħ�������湴��use micro lib
#endif

VisionDataSend visionDataSend;		//���ڷ��͸��Ӿ���Ľṹ��
VisionDataGet  visionDataGet;			//���ڽ����Ӿ�����Ϣ�Ľṹ��

uint8_t visionRx[VISION_RX_LENGTH];


void SendVisionData_Init(void)
{
	visionDataSend.head 					 = 0x53;		//֡ͷ ����涨0x45
	visionDataSend.color 					 = 0x00;		//��ʼ��ɫ 0Ϊ������ɫ 1Ϊ������ɫ 2Ϊ��������˫ɫ������ģʽ��
	visionDataSend.mode 					 = 0x02;		//ģʽ 0Ϊ���� 1ΪС�������� 2Ϊ���������� 3Ϊ����ڱ�ģʽ 4ΪС����ģʽ 5Ϊ¼��ģʽ 6Ϊ���˻�ģʽ 7Ϊ�ڱ�ģʽ 8Ϊ�״�ģʽ �������Ĭ��Ϊ����ģʽ
	visionDataSend.MyID 					 = 0x03;		//��ǰ������ID 0ΪӢ�� 1Ϊ���� 2Ϊ���� 3Ϊ���˻� 4Ϊ�ڱ�
	visionDataSend.yaw_Angle   		 = 5.0;			//yaw�������ǽǶ�   ��ʼֵ
	visionDataSend.pitch_Angle 		 = 6.0;			//pitch�������ǽǶ� ��ʼֵ
	visionDataSend.yaw_velocity 	 = 11;			//yaw���ٶ�				 ��ʼֵ
	visionDataSend.pitch_velocity  = 12;			//pitch���ٶ�			 ��ʼֵ
	visionDataSend.bullet_velocity = 66;			//���跢���ٶ�			 ��ʼֵ
	visionDataSend.end 						 = 0x45;		//֡β ����涨0x45
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

/* Ҫ���͸��Ӿ������Ϣ ��Ϣһ��18���ַ�*/
void SendVisionData(VisionDataSend* RAW_Data)		//���øú��������Ӿ���Ϣ���Ӿ���
{
	uint8_t data_buffer[18];
	
	data_buffer[0]  = RAW_Data->head;												//֡ͷ
	data_buffer[1]  = RAW_Data->color;											//��ɫ 0Ϊ������ɫ 1Ϊ������ɫ 2Ϊ��������˫ɫ������ģʽ��
	data_buffer[2]  = RAW_Data->mode;												//ģʽ 0Ϊ���� 1ΪС�������� 2Ϊ���������� 3Ϊ����ڱ�ģʽ 4ΪС����ģʽ 5Ϊ¼��ģʽ 6Ϊ���˻�ģʽ 7Ϊ�ڱ�ģʽ 8Ϊ�״�ģʽ �������Ĭ��Ϊ����ģʽ
	data_buffer[3]  = RAW_Data->MyID;												//��ǰ������ID 0ΪӢ�� 1Ϊ���� 2Ϊ���� 3Ϊ���˻� 4Ϊ�ڱ�
	
	Send_Float_Angle_Info.angle = RAW_Data->yaw_Angle;			//ͨ�� ������ ��float���Ͳ��4��uint8_t���� �ֿ��ֽڷ���
	data_buffer[4]  = Send_Float_Angle_Info.arr_angle[0];
	data_buffer[5]  = Send_Float_Angle_Info.arr_angle[1];
	data_buffer[6]  = Send_Float_Angle_Info.arr_angle[2];
	data_buffer[7]  = Send_Float_Angle_Info.arr_angle[3];
	
	Send_Float_Angle_Info.angle = RAW_Data->pitch_Angle;		//ͨ�� ������ ��float���Ͳ��4��uint8_t���� �ֿ��ֽڷ���
	data_buffer[8]  = Send_Float_Angle_Info.arr_angle[0];
	data_buffer[9]  = Send_Float_Angle_Info.arr_angle[1];
	data_buffer[10] = Send_Float_Angle_Info.arr_angle[2];
	data_buffer[11] = Send_Float_Angle_Info.arr_angle[3];
	
	data_buffer[12] = (RAW_Data->yaw_velocity & 0xff);			//yaw  �����������ݣ���λ���ȷ��ͣ�
	data_buffer[13] = (RAW_Data->yaw_velocity >> 8);
	data_buffer[14] = (RAW_Data->pitch_velocity & 0xff);		//pitch�����������ݣ���λ���ȷ��ͣ�
	data_buffer[15] = (RAW_Data->pitch_velocity >> 8);
	data_buffer[16] = RAW_Data->bullet_velocity;
	data_buffer[17] = RAW_Data->end;												//֡β
	
	SendDataByUART(data_buffer);														//����API����
	
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


/* ���Ӿ����ȡ����Ϣ ��Ϣһ��13���ַ�*/
void GetVisionData(VisionDataGet* myVisionDataGet ,uint8_t * RAW_Data)			//���øú�����ȡ�Ӿ���Ϣ���Ӿ���Ϣ�ᴫ�� �βνṹ�� ��
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


//���ú����ŵ���ʱ������ж��У����ʱ��1ms����ʵ�ֶ�ʱ���͡�����Ƶ�����޸�ͷ�ļ���SendVisionDataCircular_frequency����ֵ
void SendVisionDataCircular(VisionDataSend* RAWData)			
{
	static int timeCount = 0;
	if(timeCount++ >= (1000 / SendVisionDataCircular_frequency))
	{
		timeCount = 0;
		SendVisionData(RAWData);
	}
}


/*  ����Ϊ��Ҫ��ӳ���API  */
void SendDataByUART(uint8_t* RAWData)					//��Ҫ��ӳ���API������λ���������ݻ���øú�������ָ�����ڷ����β�ָ����ָ���飨������ȡ18���ַ���
{
	//printf("transmate start!\n");
	//HAL_UART_Transmit_DMA(&huart1, RAWData, 18);//ͨ��DMAһ���Է�������
	HAL_UART_Transmit(&huart8, RAWData, 18, 0xff);//ͨ������һ���Է�������
}






