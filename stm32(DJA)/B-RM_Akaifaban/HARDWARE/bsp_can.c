/******************************************************************************
can通信底层相关操作

2022 7 8 YQY
*******************************************************************************/

#include "can.h"
#include "bsp_can.h"
#include "superCap.h"
#include "stdio.h"			//用里面的printf调试语句
#include "led.h"
moto_measure_t moto_chassis[8] 		= {0};			//4 底盘电机
moto_measure_t gimbal_chassis[2]	= {0};			//2 云台电机

void get_total_angle(moto_measure_t *p);
void get_moto_offset(moto_measure_t *ptr, CAN_HandleTypeDef* hcan);
void get_moto_measure(motor_Typedef* motor, uint8_t* YQY_Rx_Msg);
/*******************************************************************************************
  * @Func			my_can_filter_init
  * @Brief    CAN1和CAN2滤波器配置
  * @Param		CAN_HandleTypeDef* hcan
  * @Retval		None
  * @Date     2015/11/30
 *******************************************************************************************/
void my_can_filter_init_recv_all(void)
{
	CAN_FilterTypeDef  can_filter;

  can_filter.FilterBank = 0;                       // filter 0
  can_filter.FilterMode =  CAN_FILTERMODE_IDMASK;  // mask mode
  can_filter.FilterScale = CAN_FILTERSCALE_32BIT;
  can_filter.FilterIdHigh = 0;
  can_filter.FilterIdLow  = 0;
  can_filter.FilterMaskIdHigh = 0;
  can_filter.FilterMaskIdLow  = 0;                // set mask 0 to receive all can id
  can_filter.FilterFIFOAssignment = CAN_RX_FIFO0; // assign to fifo0
  can_filter.FilterActivation = ENABLE;           // enable can filter
  can_filter.SlaveStartFilterBank  = 0;          // only meaningful in dual can mode
   
  HAL_CAN_ConfigFilter(&hcan1, &can_filter);        // init can filter
  HAL_CAN_Start(&hcan1);                          // start can1
  HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING); // enable can1 rx interrupt
	
	HAL_CAN_ConfigFilter(&hcan2, &can_filter);        // init can filter
  HAL_CAN_Start(&hcan2);                          // start can1
  HAL_CAN_ActivateNotification(&hcan2, CAN_IT_RX_FIFO0_MSG_PENDING); // enable can1 rx interrupt
}

/*******************************************************************************************
  * @Func			void HAL_CAN_RxCpltCallback(CAN_HandleTypeDef* _hcan)
  * @Brief    HAL库中标准的CAN接收完成回调函数，需要在此处理通过CAN总线接收到的数据 YQY重新修订
  * @Param		
  * @Retval		None 
  * @Date     2022/3/25
 *******************************************************************************************/
uint32_t FlashTimer;
CAN_RxHeaderTypeDef   RxHeader;
uint8_t               RxData[8];

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *CanHandle)
{
	if(HAL_GetTick() - FlashTimer>300){										//用于测试，如果接收到数据，就让红色led闪烁
		//HAL_GPIO_TogglePin(LED_R_GPIO_Port,LED_R_Pin);
#if( CAN_RECEIVE_LED_GREEN_BLINK )
		ledToggle('G');
#endif
		FlashTimer = HAL_GetTick();		
	}
  if(CanHandle->Instance == CAN1)
	{
		if (HAL_CAN_GetRxMessage(CanHandle, CAN_RX_FIFO0, &RxHeader, RxData) != HAL_OK)	//读取FIFO0中的缓存数据
		{
			Error_Handler();
		}
		switch(RxHeader.StdId){																//这里处理ID 0-4的电机的返回报文，其它电机同理，往下面加就行
			case CHASSIS_MOTOR1_ID:
			case CHASSIS_MOTOR2_ID:
			case CHASSIS_MOTOR3_ID:
			case CHASSIS_MOTOR4_ID:															
			{
				uint8_t i;
				i = RxHeader.StdId - CHASSIS_MOTOR_ALL_ID-1;			//计算绝对偏移地址
				get_moto_measure(&ChassisMotor[i], RxData);				//将获取到的报文信息传入子函数进行处理
				//get_total_angle(&moto_chassis[i]);							//计算绝对角度
			}
			break;
			case UP_MOTOR1_ID:																	//抬升电机 左后
			case UP_MOTOR2_ID:																	//抬升电机 有后
			case UP_MOTOR3_ID:																	//抬升电机 左前
			case UP_MOTOR4_ID:																	//抬升电机 右前											
			{
				uint8_t i;
				i = RxHeader.StdId - UP_MOTOR1_ID;			//计算绝对偏移地址
				get_moto_measure(&LiftMotor[i], RxData);				//将获取到的报文信息传入子函数进行处理
				//get_total_angle(&moto_chassis[i]);							//计算绝对角度
			}
			break;
			default: break;
		}
	}
	else if(CanHandle->Instance == CAN2)
	{
		if (HAL_CAN_GetRxMessage(CanHandle, CAN_RX_FIFO0, &RxHeader, RxData) != HAL_OK)	//读取FIFO0中的缓存数据
		{
			Error_Handler();
		}
		//printf("id:%x\n",RxHeader.StdId);
		switch(RxHeader.StdId)
		{
			case FLEX_MOTOR1_ID:
			case FLEX_MOTOR2_ID:
			{
				uint8_t i;
				i = RxHeader.StdId - FLEX_MOTOR1_ID;						//计算绝对偏移地址
				get_moto_measure(&FlexMotor[i], RxData);				//将获取到的报文信息传入子函数进行处理
			}
			break;
			case PITCH_MOTOR_ID:
			{
				get_moto_measure(&PitchMotor, RxData);					//将获取到的报文信息传入子函数进行处理
			}
			break;
		}
	}
	//MOTOR_PID_Cal();		//计算PID值
}


///*******************************************************************************************
//  * @Func			void get_moto_measure(moto_measure_t *ptr, CAN_HandleTypeDef* hcan)
//  * @Brief    接收3508电机通过CAN发过来的信息		YQY重新修订
//  * @Param		
//  * @Retval		None
//  * @Date     2022/3/25
// *******************************************************************************************/
void get_moto_measure(motor_Typedef* motor, uint8_t* YQY_Rx_Msg)
{
	motor->measure.last_angle = motor->measure.angle;																			//保存上一次的转子角度值
	motor->measure.angle = (uint16_t)(YQY_Rx_Msg[0]<<8 | YQY_Rx_Msg[1]) ;					//解码出这次的转子角度，注意是转子
	motor->measure.speed_rpm  = (int16_t)(YQY_Rx_Msg[2]<<8 | YQY_Rx_Msg[3]);			//解码这次的速度
	motor->measure.real_current = (YQY_Rx_Msg[4]<<8 | YQY_Rx_Msg[5])*5.f/16384.f;	//解码当前的电流，注意这个电流飘的很厉害！可能得滤波一下

	//motor->measure.hall = YQY_Rx_Msg[6];																				//数据手册中位[6]是空的，不知道有啥用
	
	if(motor->measure.angle - motor->measure.last_angle > 4096)										//计算电机旋转了几圈
		motor->measure.round_cnt --;
	else if (motor->measure.angle - motor->measure.last_angle < -4096)						//计算电机旋转了几圈
		motor->measure.round_cnt ++;
	motor->measure.total_angle = motor->measure.round_cnt * 8192 + motor->measure.angle - motor->measure.offset_angle;
}

/*this function should be called after system+can init */							//YQY挖坑：该函数未使用！
void getAngleOffset(moto_measure_t *ptr)
{
	ptr->offset_angle = ptr->angle;
}


/**
*@bref 电机上电角度=0， 之后用这个函数更新3510电机的相对开机后（为0）的相对角度。
	*/
#define ABS(x)	( (x>0) ? (x) : (-x) )
void get_total_angle(moto_measure_t *p){															//放入中断更新角度
	
	int res1, res2, delta;
	if(p->angle < p->last_angle){			//可能的情况
		res1 = p->angle + 8192 - p->last_angle;	//正转，delta=+
		res2 = p->angle - p->last_angle;				//反转	delta=-
	}else{	//angle > last
		res1 = p->angle - 8192 - p->last_angle ;//反转	delta -
		res2 = p->angle - p->last_angle;				//正转	delta +
	}
	//不管正反转，肯定是转的角度小的那个是真的
	if(ABS(res1)<ABS(res2))
		delta = res1;
	else
		delta = res2;

	p->total_angle += delta;
	p->last_angle = p->angle;
}
///*******************************************************************************************
//  * @Func			void SetChassisMotorRAW(moto_measure_t *ptr, CAN_HandleTypeDef* hcan)
//  * @Brief    分别控制低四位、高四位电机电流，进而控制旋转
//  * @Param		CAN_HandleTypeDef* hcan 传入can1或者can2
//							iq1-iq4 传入电机ID 1-4的电流信息。2006电机最大10000  3508最大16384
//  * @Retval		None
//  * @Date     2022/3/24
// *******************************************************************************************/
void SetChassisMotorRAW(CAN_HandleTypeDef* hcan, int16_t iq1, int16_t iq2, int16_t iq3, int16_t iq4)						//设置底盘四个电机（ID1-4）电流
{
	uint8_t TxData[8] = {0};
	uint32_t TxMainBox = 0;
	CAN_TxHeaderTypeDef TxHeader;
	
	TxHeader.StdId = 0x200;					//低4位
	TxHeader.IDE =  CAN_ID_STD;			//标准帧
	TxHeader.RTR = CAN_RTR_DATA;		//数据
	TxHeader.DLC = 0x08;
	
	TxData[0] = (iq1 >> 8);					//电机1电流高8位
	TxData[1] = iq1;								//电机1电流低8位
	TxData[2] = (iq2 >> 8);					//由此类推...
	TxData[3] = iq2;
	TxData[4] = (iq3 >> 8);
	TxData[5] = iq3;
	TxData[6] = (iq4 >> 8);
	TxData[7] = iq4;
																	//发送can报文
	while(HAL_CAN_AddTxMessage(&hcan1, &TxHeader, TxData, &TxMainBox) != HAL_OK);
}

void SetLeftMotorRAW( int16_t iq1, int16_t iq2, int16_t iq3, int16_t iq4)							//设置ID5-8电机电流
{	
	uint8_t TxData[8] = {0};
	uint32_t TxMainBox = 0;
	CAN_TxHeaderTypeDef TxHeader;
	
	TxHeader.StdId = 0x1FF;				//高4位
	TxHeader.IDE =  CAN_ID_STD;
	TxHeader.RTR = CAN_RTR_DATA;
	TxHeader.DLC = 0x08;
	
	TxData[0] = (iq1 >> 8);
	TxData[1] = iq1;
	TxData[2] = (iq2 >> 8);
	TxData[3] = iq2;
	TxData[4] = (iq3 >> 8);
	TxData[5] = iq3;
	TxData[6] = (iq4 >> 8);
	TxData[7] = iq4;
	
	while(HAL_CAN_AddTxMessage(&hcan1, &TxHeader, TxData, &TxMainBox) != HAL_OK);
}
void SetFlexMotorRAW(int16_t iq1, int16_t iq2, int16_t pitch)
{
	uint8_t TxData[8] = {0};
	uint32_t TxMainBox = 0;
	CAN_TxHeaderTypeDef TxHeader;
	
	TxHeader.StdId = 0x200;				//高4位
	TxHeader.IDE =  CAN_ID_STD;
	TxHeader.RTR = CAN_RTR_DATA;
	TxHeader.DLC = 0x06;					//数据长度4位
	
	TxData[0] = (iq1 >> 8);				//L轴伸出电机
	TxData[1] = iq1;
	TxData[2] = (iq2 >> 8);				//L轴伸出电机
	TxData[3] = iq2;
	TxData[4] = (pitch >> 8);			//Pitch轴翻转电机
	TxData[5] = pitch;
	
	while(HAL_CAN_AddTxMessage(&hcan2, &TxHeader, TxData, &TxMainBox) != HAL_OK);
}
//旧版本步兵，云台电机分部如下：pitch轴can2 ID6  yaw轴can1 ID5
void SetGimbalMotorRAW(int16_t iq1, int16_t iq2)						//设置云台电机（ID1-2）电流
{
	uint8_t TxData[8] = {0};
	uint32_t TxMainBox = 0;
	CAN_TxHeaderTypeDef TxHeader;
	
	TxHeader.StdId = 0x2FF;					//高4位		云台电机ID 5 和 ID 6
	TxHeader.IDE =  CAN_ID_STD;			//标准帧
	TxHeader.RTR = CAN_RTR_DATA;		//数据
	TxHeader.DLC = 0x02;
	
	TxData[0] = (iq1 >> 8);					//电机1电流高8位
	TxData[1] = iq1;								//电机1电流低8位
	if(HAL_CAN_AddTxMessage(&hcan1, &TxHeader, TxData, &TxMainBox) != HAL_OK)
	{
		Error_Handler();
	}
	TxData[2] = (iq2 >> 8);					//电机1电流高8位
	TxData[3] = iq2;								//电机1电流低8位
	TxHeader.DLC = 0x04;
	while(HAL_CAN_AddTxMessage(&hcan2, &TxHeader, TxData, &TxMainBox) != HAL_OK);
}
void SetSuperCapPower(float power_W)		//超级电容，输入设定的功率，范围
{
	uint16_t tempPower = (uint16_t)(power_W*100);
	uint8_t TxData[8] = {0};
	uint32_t TxMainBox = 0;
	CAN_TxHeaderTypeDef TxHeader;
	
	TxHeader.StdId = 0x210;								//超级电容模块地址
	TxHeader.IDE =  CAN_ID_STD;						//标准帧
	TxHeader.RTR = CAN_RTR_DATA;					//数据
	TxHeader.DLC = 0x02;									//数据长度2
	
	TxData[0] = (tempPower >> 8);					//超级电容功率高8位
	TxData[1] = tempPower;								//超级电容功率低8位
																				
	while(HAL_CAN_AddTxMessage(&hcan1, &TxHeader, TxData, &TxMainBox) != HAL_OK);											//发送can报文
}
void myCanInit_ALL(void)																																				//配置过滤器
{
	my_can_filter_init_recv_all();    					 																									//配置CAN过滤器
}

