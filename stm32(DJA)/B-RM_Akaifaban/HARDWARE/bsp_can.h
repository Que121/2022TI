/******************************************************************************
can通信底层相关操作
	如何添加新的电机呢？去搜索 在这里添加新的电机 字符

2022 7 8 YQY
*******************************************************************************/

#ifndef __BSP_CAN
#define __BSP_CAN

#include "main.h"
#include "MyConfig.h"						//包含所有可配置的东东！
#include "mytype.h"
#include "can.h"
#include "motor.h"
/*CAN发送或是接收的ID*/
typedef enum
{
	CHASSIS_MOTOR_ALL_ID 				= 0x200,
	CHASSIS_MOTOR1_ID 					= 0x201,
	CHASSIS_MOTOR2_ID 					= 0x202,
	CHASSIS_MOTOR3_ID 					= 0x203,
	CHASSIS_MOTOR4_ID 					= 0x204,
	UP_MOTOR1_ID								= 0x205,			//CAN 1  吸盘向前，左后电机
	UP_MOTOR2_ID								= 0x206,			//CAN 1  吸盘向前，右后电机
	UP_MOTOR3_ID								= 0x207,			//CAN 1  吸盘向前，左前电机
	UP_MOTOR4_ID								= 0x208,			//CAN 1  吸盘向前，右前电机
	
	FLEX_MOTOR1_ID							= 0x201,			//CAN 2  伸长电机
	FLEX_MOTOR2_ID							= 0x202,			//CAN 2  伸长电机
	PITCH_MOTOR_ID							= 0x203,			//CAN 2  翻转电机
}CAN_Message_ID;

#define FILTER_BUF_LEN		5
/*接收到的电机的参数结构体*/
typedef struct{
	int16_t	 	speed_rpm;		//电机转速，即每分钟转几圈
  float  		real_current;	//通过CAN线读取到的电机电流
  int16_t  	given_current;//我们上次调用电机电流设置函数所给定的电流（未实现）
  uint8_t  	hall;					//不知道干啥用的
	uint16_t 	angle;				//无刷电机转子绝对角度（本次值），范围:[0,8191]映射到0-360度
	uint16_t 	last_angle;		//无刷电机转子绝对角度（上次值），范围:[0,8191]映射到0-360度
	uint16_t	offset_angle;
	int32_t		round_cnt;
	int32_t		total_angle;
	u8			buf_idx;
	u16			angle_buf[FILTER_BUF_LEN];
	u16			fited_angle;
	u32			msg_cnt;
}moto_measure_t;
/* Inside  ------------------------------------------------------------------*/
void my_can_filter_init_recv_all(void);
void can_filter_recv_special(CAN_HandleTypeDef* hcan, uint8_t filter_number, uint16_t filtered_id);

void can_receive_onetime(CAN_HandleTypeDef* _hcan, int time);
void set_moto_current(CAN_HandleTypeDef* hcan, s16 iq1, s16 iq2, s16 iq3, s16 iq4);

void getAngleOffset(moto_measure_t *ptr);

/* Extern  ------------------------------------------------------------------*/
extern moto_measure_t  moto_chassis[];
extern moto_measure_t  gimbal_chassis[];
extern void SetChassisMotorRAW(CAN_HandleTypeDef* hcan, int16_t iq1, int16_t iq2, int16_t iq3, int16_t iq4);	//设置底盘四个电机电流
extern void SetLeftMotorRAW(int16_t iq1, int16_t iq2, int16_t iq3, int16_t iq4);			//设置ID5-8抬升电机的电流
extern void SetFlexMotorRAW(int16_t iq1, int16_t iq2, int16_t pitch);																												//设置ID1-2 CAN2 伸缩电机
extern void SetGimbalMotorRAW(int16_t iq1, int16_t iq2);																											//设置云台电机（ID1-2）电流
extern void SetSuperCapPower(float);																																					//设置超级电容最大消耗功率（本模组+负载） 35-130W
extern void myCanInit_ALL(void);
#endif
