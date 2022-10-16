#include "myUmpire.h"
#include "CRC.h"
#include "led.h"


uint8_t umpireRxBuffer[UMPIRE_RX_BY_DMA_LENGTH];		//���ջ���
uint32_t umpireRxLastTime = 0;
uint8_t CRC_8=0;
uint16_t CRC_16=0;


//----------------------------------------------����ϵͳ������------------------------------------------------
//ext_game_state_t Match_status_data;//����״̬����
//ext_game_result_t Match_Result_data;//�����������
//ext_game_robot_HP_t Robot_survival_data;//�����˴������
//ext_event_data_t Site_event_data;//�����¼�����
//ext_supply_projectile_action_t Stop_action_sign;//����վ������ʶ
//ext_supply_projectile_booking_t Request_the_depot_to_reload;//���󲹸�վ�����ӵ�
ext_game_robot_state_t Game_robot_state;//����������״̬
ext_power_heat_data_t Umpire_PowerHeat;//ʵʱ������������
//ext_game_robot_pos_t Robot_position;//������λ��
//ext_buff_musk_t Robot_gain;//����������
//aerial_robot_energy_t Air_robot_energy_state;//���л���������״̬
//ext_robot_hurt_t The_damage_state;//�˺�״̬
//ext_shoot_data_t Real_time_shooting_information;//ʵʱ�����Ϣ
//ext_bullet_remaining_t The_bullet_state;


static uint8_t RxPointer;						//���ջ���ָ��

Frame_TypeDef Frame;

void 	decodeUmpireData(uint16_t CmdID, uint16_t DataLength);	//�����Ӻ������ɽ��뺯�����ã����ڱ��ļ���
void	decodeUmpireData_handle(void)														//���뺯��	���Ŵ��ڴ�������ж�����
{
	if(HAL_GetTick() - umpireRxLastTime > 200)		//��LED��˸����ʾ���յ��˲���ϵͳ����
	{
		ledToggle(UMPIRE_RX_LED_BLINK_NUM);
		umpireRxLastTime = HAL_GetTick();
	}
	for(RxPointer=0;RxPointer<UMPIRE_RX_BY_DMA_LENGTH;RxPointer++)
	{
		if(umpireRxBuffer[RxPointer] == 0xA5)//��⵽��ʼ��־
		{
			Frame.DataLength = umpireRxBuffer[RxPointer+1] | (umpireRxBuffer[RxPointer+2]<<8);//���ݶ�data����
			Frame.Seq = umpireRxBuffer[RxPointer+3];//�����
			Frame.CRC8 = umpireRxBuffer[RxPointer+4];//֡ͷ����crc8
			Frame.CmdID = umpireRxBuffer[RxPointer+5] | (umpireRxBuffer[RxPointer+6]<<8);//����id
			CRC_8 = UNVerify_CRC8_Check_Sum((uint8_t *)&umpireRxBuffer[RxPointer],5);
			CRC_16 = UNVerify_CRC16_Check_Sum((uint8_t *)&umpireRxBuffer[RxPointer],9+Frame.DataLength);
			Frame.FrameTail = umpireRxBuffer[RxPointer+7+Frame.DataLength] | (umpireRxBuffer[RxPointer+8+Frame.DataLength]<<8);//��������crc16
//      Real_time_shooting_information.bullet_speed_last = Real_time_shooting_information.bullet_speed;
//			printf("%d\r\n",Length);
//			printf("%d\r\n",Frame.DataLength);
//			printf("%d\r\n",Frame.Seq);
//			printf("%d\r\n",Frame.CRC8);
//			printf("%d\r\n",Frame.CmdID);
//			printf("%d\r\n",Frame.FrameTail);
			//crcУ��
//			if(Frame.DataLength + 6 > Length-RxPointer)
//			{
//				break;
//			}
			if((Verify_CRC8_Check_Sum((uint8_t *)&umpireRxBuffer[RxPointer], 5)) && (Verify_CRC16_Check_Sum((uint8_t *)&umpireRxBuffer[RxPointer], 9+Frame.DataLength)))
			{
				decodeUmpireData(Frame.CmdID,Frame.DataLength);//��������
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

void 	decodeUmpireData(uint16_t CmdID, uint16_t DataLength)	//�����Ӻ������ɽ��뺯�����ã����ڱ��ļ���
{
	RxPointer += 7;//ƫ��7��ָ�����ݶε�һ���ֽ�
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
