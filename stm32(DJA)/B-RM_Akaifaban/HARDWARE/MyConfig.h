//说明文档！

#ifndef __MY_CONFIG_H
#define __MY_CONFIG_H
//***********************云台限位（UART8）***********************YQYMARK:调整遥控器灵敏度
#define GIMBAL_PITCH_ANGLE_MAX						 40.0f		//pitch最大
#define GIMBAL_PITCH_ANGLE_MIN						-20.0f		//pitch最小


//***********************和视觉通信（UART8）***********************
#define VISION_RX_LENGTH											13		//视觉组一次性发来13个数据

//***********************裁判系统通信（UART6）*********************
#define UMPIRE_PRINT_ROBOT_INFORMATION				0			//是否通过打印机器人相关信息，默认只打印ID和level
#define UMPIRE_RX_LED_BLINK_NUM								'0'		//成功接收裁判系统数据，让这个灯闪烁
#define	UMPIRE_RX_BY_DMA_LENGTH								50		//串口通过DMA一次性接收多少个字符？


//***********************板载24v*4输出设置*************************
#define POWER_24V_SOFT_START_ENABLE						1			//是否开启上电缓启动，0关1开。调试时候可以关了，真正上车必须打开！
#define POWER_24V_SOFT_START_DELAY						300		//四路输出上电缓启动间隔时间，单位ms   在上面一项开启的情况下才有用！


//***********************CAN通信相关设置*********************

#define CAN_RECEIVE_LED_GREEN_BLINK						1			//当CAN接收到报文后是否闪烁板载绿色LED  1闪烁  0关闭



//***********************PID电机参数****************************
							/*底盘（二轴）电机使用GM6020 串级PID*/
#define CHASSIS_MOTOR_kP 								(3.8f	  )			//底盘电机速度PID   系数kP
#define CHASSIS_MOTOR_kI 								(0.20f	  )		//底盘电机速度PID   系数kI
#define CHASSIS_MOTOR_kD 								(-0.01f )			//底盘电机速度PID   系数kD
#define CHASSIS_MOTOR_I_OUT_MAX					(4000		)			//底盘电机积分限幅
#define CHASSIS_MOTOR_PID_OUTPUT_MAX		(15000		)		//底盘电机输出限幅	2006限幅10000 3508限幅16384

#define CHASSIS_MOTOR_kP_P 								(4.3	  )		//底盘电机速度PID   系数kP
#define CHASSIS_MOTOR_kI_P 								(0.0	  )		//底盘电机速度PID   系数kI
#define CHASSIS_MOTOR_kD_P 								(0 	)		//底盘电机速度PID   系数kD
#define CHASSIS_MOTOR_I_OUT_MAX_P					(4000		)		//底盘电机积分限幅
#define CHASSIS_MOTOR_PID_OUTPUT_MAX_P		(15000	)		//底盘电机输出限幅	2006限幅10000 3508限幅16384

							/*云台（二轴）电机使用GM6020 串级PID*/
#define GIMBAL_MOTOR_kP 									50.0f			//云台电机速度PID   系数kP		45.0f
#define GIMBAL_MOTOR_kI 									3.0f  		//云台电机速度PID   系数kI		3.0f
#define GIMBAL_MOTOR_kD 									0.0f  		//云台电机速度PID   系数kD		0.0f
#define GIMBAL_MOTOR_I_OUT_MAX						30000			//云台电机积分限幅						30000	
#define GIMBAL_MOTOR_PID_OUTPUT_MAX				30000			//云台电机输出限幅	使用6020电机 限幅30000

#define GIMBAL_MOTOR_kP_P 								0.4f			//云台电机位置PID   系数kP		0.15f
#define GIMBAL_MOTOR_kI_P 								0.0f  		//云台电机位置PID   系数kI		0.0f
#define GIMBAL_MOTOR_kD_P 								-0.01f  		//云台电机位置PID   系数kD		0.0f
#define GIMBAL_MOTOR_I_OUT_MAX_P					30000			//云台电机位置积分限幅				30000	
#define GIMBAL_MOTOR_PID_OUTPUT_MAX_P			30000			//云台电机位置输出限幅	使用6020电机 限幅30000

							/*普通电机（3508、2006）电机 串级PID参数*/


//***********************IMU参数************************		未完成
#define IMU_TEMP						9000		//目标温度  修改该参数可缩短IMU校准时间
#define TEMP_Tolerance			60			//温度误差容忍度
#define TEMP_KP							2.4f
#define TEMP_KI							1
#define TEMP_KD							1



#endif	//__MY_CONFIG_H
