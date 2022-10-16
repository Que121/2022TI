#include "myUmpire.h"
#include "CRC.h"
#include "led.h"


uint8_t umpireRxBuffer[UMPIRE_RX_BY_DMA_LENGTH];		//接收缓冲
uint32_t umpireRxLastTime = 0;
uint8_t CRC_8=0;
uint16_t CRC_16=0;


//----------------------------------------------裁判系统数据们------------------------------------------------
//ext_game_state_t Match_status_data;//比赛状态数据
//ext_game_result_t Match_Result_data;//比赛结果数据
//ext_game_robot_HP_t Robot_survival_data;//机器人存活数据
//ext_event_data_t Site_event_data;//场地事件数据
//ext_supply_projectile_action_t Stop_action_sign;//补给站动作标识
//ext_supply_projectile_booking_t Request_the_depot_to_reload;//请求补给站补弹子弹
ext_game_robot_state_t Game_robot_state;//比赛机器人状态
ext_power_heat_data_t Umpire_PowerHeat;//实时功率热量数据
//ext_game_robot_pos_t Robot_position;//机器人位置
//ext_buff_musk_t Robot_gain;//机器人增益
//aerial_robot_energy_t Air_robot_energy_state;//空中机器人能量状态
//ext_robot_hurt_t The_damage_state;//伤害状态
//ext_shoot_data_t Real_time_shooting_information;//实时射击信息
//ext_bullet_remaining_t The_bullet_state;


static uint8_t RxPointer;						//接收缓冲指针

Frame_TypeDef Frame;

void 	decodeUmpireData(uint16_t CmdID, uint16_t DataLength);	//解码子函数，由解码函数调用，仅在本文件内
void	decodeUmpireData_handle(void)														//解码函数	，放串口传输完成中断里面
{
	if(HAL_GetTick() - umpireRxLastTime > 200)		//让LED闪烁，表示接收到了裁判系统数据
	{
		ledToggle(UMPIRE_RX_LED_BLINK_NUM);
		umpireRxLastTime = HAL_GetTick();
	}
	for(RxPointer=0;RxPointer<UMPIRE_RX_BY_DMA_LENGTH;RxPointer++)
	{
		if(umpireRxBuffer[RxPointer] == 0xA5)//检测到起始标志
		{
			Frame.DataLength = umpireRxBuffer[RxPointer+1] | (umpireRxBuffer[RxPointer+2]<<8);//数据段data长度
			Frame.Seq = umpireRxBuffer[RxPointer+3];//包序号
			Frame.CRC8 = umpireRxBuffer[RxPointer+4];//帧头检验crc8
			Frame.CmdID = umpireRxBuffer[RxPointer+5] | (umpireRxBuffer[RxPointer+6]<<8);//命令id
			CRC_8 = UNVerify_CRC8_Check_Sum((uint8_t *)&umpireRxBuffer[RxPointer],5);
			CRC_16 = UNVerify_CRC16_Check_Sum((uint8_t *)&umpireRxBuffer[RxPointer],9+Frame.DataLength);
			Frame.FrameTail = umpireRxBuffer[RxPointer+7+Frame.DataLength] | (umpireRxBuffer[RxPointer+8+Frame.DataLength]<<8);//整包检验crc16
//      Real_time_shooting_information.bullet_speed_last = Real_time_shooting_information.bullet_speed;
//			printf("%d\r\n",Length);
//			printf("%d\r\n",Frame.DataLength);
//			printf("%d\r\n",Frame.Seq);
//			printf("%d\r\n",Frame.CRC8);
//			printf("%d\r\n",Frame.CmdID);
//			printf("%d\r\n",Frame.FrameTail);
			//crc校验
//			if(Frame.DataLength + 6 > Length-RxPointer)
//			{
//				break;
//			}
			if((Verify_CRC8_Check_Sum((uint8_t *)&umpireRxBuffer[RxPointer], 5)) && (Verify_CRC16_Check_Sum((uint8_t *)&umpireRxBuffer[RxPointer], 9+Frame.DataLength)))
			{
				decodeUmpireData(Frame.CmdID,Frame.DataLength);//解析数据
#if UMPIRE_PRINT_ROBOT_INFORMATION
				printf("ID:%d level:%d\n",Game_robot_state.robot_id,Game_robot_state.robot_level);
#endif
			}
			else 
			{
				break;
			}
		}
	}
}

void 	decodeUmpireData(uint16_t CmdID, uint16_t DataLength)	//解码子函数，由解码函数调用，仅在本文件内
{
	RxPointer += 7;//偏移7，指到数据段第一个字节
	switch(CmdID)
	{
		case 0x0201:
		{
			Game_robot_state.robot_id = umpireRxBuffer[RxPointer];
			Game_robot_state.robot_level = umpireRxBuffer[RxPointer+1];
			Game_robot_state.remain_HP = umpireRxBuffer[RxPointer+2] | ((umpireRxBuffer[RxPointer+3]) << 8);
			Game_robot_state.max_HP = umpireRxBuffer[RxPointer+4] | (umpireRxBuffer[RxPointer+5] << 8);
			Game_robot_state.shooter_id1_17mm_cooling_rate = umpireRxBuffer[RxPointer+6] | ((umpireRxBuffer[RxPointer+7]) << 8);
			Game_robot_state.shooter_id1_17mm_cooling_limit = umpireRxBuffer[RxPointer+8] | ((umpireRxBuffer[RxPointer+9]) << 8);
			Game_robot_state.shooter_id1_17mm_speed_limit = umpireRxBuffer[RxPointer+10] | ((umpireRxBuffer[RxPointer+11]) << 8);
			Game_robot_state.shooter_id2_17mm_cooling_rate = umpireRxBuffer[RxPointer+12] | ((umpireRxBuffer[RxPointer+13]) << 8);
			Game_robot_state.shooter_id2_17mm_cooling_limit = umpireRxBuffer[RxPointer+14] | ((umpireRxBuffer[RxPointer+15]) << 8);
			Game_robot_state.shooter_id2_17mm_speed_limit = umpireRxBuffer[RxPointer+16] | ((umpireRxBuffer[RxPointer+17]) << 8);
			Game_robot_state.shooter_id1_42mm_cooling_rate = umpireRxBuffer[RxPointer+18] | ((umpireRxBuffer[RxPointer+19]) << 8);
			Game_robot_state.shooter_id1_42mm_cooling_limit = umpireRxBuffer[RxPointer+20] | ((umpireRxBuffer[RxPointer+21]) << 8);
			Game_robot_state.shooter_id1_42mm_speed_limit = umpireRxBuffer[RxPointer+22] | ((umpireRxBuffer[RxPointer+23]) << 8);
			Game_robot_state.chassis_power_limit = umpireRxBuffer[RxPointer+24] | ((umpireRxBuffer[RxPointer+25]) << 8);
		   break;
		}
		case 0x0202:
		{
			Umpire_PowerHeat.chassis_volt =  umpireRxBuffer[RxPointer] | ((umpireRxBuffer[RxPointer+1]) << 8);
			Umpire_PowerHeat.chassis_current = umpireRxBuffer[RxPointer+2] | ((umpireRxBuffer[RxPointer+3]) << 8) | ((umpireRxBuffer[RxPointer+4]) << 16) | ((umpireRxBuffer[RxPointer+5]) << 24);
			Umpire_PowerHeat.chassis_power = umpireRxBuffer[RxPointer+6] | ((umpireRxBuffer[RxPointer+7]) << 8);
			Umpire_PowerHeat.chassis_power_buffer = umpireRxBuffer[RxPointer+8] | ((umpireRxBuffer[RxPointer+9]) << 8);
			Umpire_PowerHeat.shooter_id1_17mm_cooling_heat = umpireRxBuffer[RxPointer+10] | ((umpireRxBuffer[RxPointer+11]) << 8);
			Umpire_PowerHeat.shooter_id2_17mm_cooling_heat = umpireRxBuffer[RxPointer+12] | ((umpireRxBuffer[RxPointer+13]) << 8);
			Umpire_PowerHeat.shooter_id1_42mm_cooling_heat = umpireRxBuffer[RxPointer+14] | ((umpireRxBuffer[RxPointer+15]) << 8);
			break;
		}
		default:
			break;
	}
}
