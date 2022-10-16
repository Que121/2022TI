/**************************************************************************//**
 * @file     VisionUART.h
 * @brief    ���Ӿ�����д���ͨ��
 * @version  V1.0
 * @date     6. April 2022
 ******************************************************************************/
/*																�ĵ�����˵����
 *	ʵ�ֵ�����Ӿ���Ĵ���ͨ�ţ�
 *	ͨ���жϽ����Ӿ��鴫��Ĳ���
 *	ͨ����ʱ������ж��Զ����͵�ǰ���������ݸ��Ӿ��飨Ƶ�ʿɵ���
 *
 *
 *																�ĵ�ʹ��˵����
 * 	Ҽ��	�� SendVisionData_Init(void); �����ŵ�main������ִ�У��Է��ͽṹ���������ֵ
 *				�� SendVisionDataCircular(VisionDataSend*); �����ŵ�1ms��ʱ������ж��ʵ�ְ��涨Ƶ�ʷ�������
 *				�� GetVisionData(VisionDataGet*,uint8_t *); �����ŵ����ڽ����ж��ڣ������������ݣ���������������β�1�ṹ���ڣ��β�2Ϊ���봮�ڽ��յ����ַ���
 *				�޸� SendDataByUART()������λ��.c�ļ���β�����ú���Ϊ�������ݵĵײ�ʵ�ַ�ʽ�������ʵ�������޸ġ�
 *
 *	����	�޸� SendVisionDataCircular_frequency �궨��ɸ��ķ���Ƶ�ʣ�����ֵ��λHz
 *				����Ҫ���͵���Ϣд��visionDataSend�ṹ�壬֮���Զ�����
 *				�Ӿ��鷢���������ݻ��ŵ�visionDataGet�ṹ����
 *
 *	����	���ԣ��ڸ�ͷ�ļ�����.c�ļ��п���//#define UART_DEBUG_MODE�궨�壬ʹ�ܵ���ģʽ��
 *				ע�⣡ʹ�ø�ģʽ������ħ�����ڹ�ѡuse micro lib
 *
 * ���ĵ����ϱ�д YQY YLX ����
 * Copyright (c) 2022 ɭ���ǻ�����ʵ���� ����� All rights reserved.
 */
#ifndef __VISION_UART_H
#define __VISION_UART_H

#include "stm32f4xx.h"
#include "MyConfig.h"						//�������п����õĶ�����


#define SendVisionDataCircular_frequency 1			//�޸��Զ����͵�Ƶ�� ��λHz

/* Ҫ���͸��Ӿ������Ϣ ��Ϣһ��18���ַ� */
typedef struct 
{
	uint8_t head;						//bit[0]			����涨����ͷ����0x53 ��S��
	uint8_t color;					//bit[1]			��ɫ 0Ϊ������ɫ 1Ϊ������ɫ 2Ϊ��������˫ɫ������ģʽ��
	uint8_t mode;						//bit[2]			ģʽ 0Ϊ���� 1ΪС�������� 2Ϊ���������� 3Ϊ����ڱ�ģʽ 4ΪС����ģʽ 5Ϊ¼��ģʽ 6Ϊ���˻�ģʽ 7Ϊ�ڱ�ģʽ 8Ϊ�״�ģʽ �������Ĭ��Ϊ����ģʽ
	uint8_t MyID;						//bit[3]			��ǰ������ID 0ΪӢ�� 1Ϊ���� 2Ϊ���� 3Ϊ���˻� 4Ϊ�ڱ�
	float yaw_Angle;				//bit[7..4]		yaw  ��������ǽǶ����ݣ��������巢�ͣ�
	float pitch_Angle;			//bit[11..8]	pitch��������ǽǶ����ݣ��������巢�ͣ�
	int16_t yaw_velocity;		//bit[13..12]	yaw  �����������ݣ���λ���ȷ��ͣ�
	int16_t pitch_velocity;	//bit[15..14]	pitch�����������ݣ���λ���ȷ��ͣ�
	uint8_t bullet_velocity;//bit[16]			��ǰ�ӵ��ٶ�
	uint8_t end;						//bit[17]			����涨����β����0x45	��E��
}VisionDataSend;


/* ���Ӿ����ȡ����Ϣ ��Ϣһ��13���ַ� */
typedef struct 
{
	uint8_t head;						//bit[0]			����涨��0x53 ��S��
	uint8_t VisionFlag;			//bit[1]			�Ƿ�ʶ��Ŀ��ı�־ 0Ϊ��Ŀ�� 1Ϊ��Ŀ��
	uint8_t VisionShoot;		//bit[2]			�Ƿ�������ź� 0Ϊֹͣ��� 1Ϊ�������
	uint8_t yawFlag;				//bit[3]			yaw ��������� 0Ϊ���� 1Ϊ����
	uint16_t yaw;						//bit[5..4]		yaw ��Ƕ�
	uint8_t pitchFlag;			//bit[6]			pitch ��������� 0Ϊ���� 1Ϊ����
	uint16_t pitch;					//bit[8..7]		pitch��Ƕ�
	uint16_t depth;					//bit[10..9]	�����Ϣ
	uint8_t QHH_CRC;				//bit[11]			�����CRCУ��λ
	uint8_t end;						//bit[12]			����涨��0x45	��E��
	
	int16_t RES_Yaw;				//�ϳ�yawFlag��yaw֮��õ��Ľ��
	int16_t RES_Pitch;			//ͬ��
}VisionDataGet;

extern uint8_t visionRx[VISION_RX_LENGTH];								//���ڴ洢�Ӿ��鷢����ԭʼ����
extern VisionDataSend visionDataSend;											//�ⲿ�ɷ��� ���ڴ����Ҫ���͸��Ӿ��������
extern VisionDataGet  visionDataGet;											//�ⲿ�ɷ��� ���ڴ���Ӿ��鷢�������ݣ������ݿ�ͨ��void GetVisionData()����

extern void SendVisionData_Init(void);										//���ø�API������ ֡ͷ ֡δ ����Ϣ											(����ŵ� main.c��ʼ������)
extern void SendVisionData(VisionDataSend*);							//���øú��������Ӿ���Ϣ���Ӿ���												(����ŵ���ʱ������ж���)
extern void GetVisionData(VisionDataGet*,uint8_t *);			//���øú�����ȡ�Ӿ���Ϣ���Ӿ���Ϣ�ᴫ�� �βνṹ�� ��		(����ŵ�UART�����ж��)

extern void SendVisionDataCircular(VisionDataSend*);			//���ú����ŵ���ʱ������ж��У����ʱ��1ms��

//��Ҫ��ӳ���API������λ���������ݻ���øú�������ָ�����ڷ����β�ָ����ָ���飨������ȡ18���ַ���
//�ú���λ��VisionUART.c�ļ���
void SendDataByUART(uint8_t*);



#endif		//__VISION_UART_H
