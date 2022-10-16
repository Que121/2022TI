#include "CPXT.h"
#include "crc.h"
#include "PYlift.h"
#include "stdio.h"
#include "string.h"//C语言
#include "usart.h"
#include "math.h"																	//三角函数
#include "DR16.h"

#define PI 										3.1415							//Π
#define send_max_len    			200
#define ToPixel_raw(x)				(x * 0.43)					//mm 转像素
#define ToMM(x)								(x * 2.29)					//像素转mm
#define ToPixel_x(x)					((int16_t)(x * 0.43 + 887))		//x坐标转像素绝对值
#define ToPixel_y(y)					((int16_t)(ToPixel_raw(y)) + 165)		//x坐标转像素绝对值
unsigned char CliendTxBuffer_graphic[send_max_len];		//打包数据组
unsigned char CliendTxBuffer_number[send_max_len];		//打包数据组
unsigned char CliendTxBuffer_character[send_max_len];	//打包数据组

graphic_struct 		graphic;
character_struct  character;
void showBodyBackground(uint8_t Variable, uint8_t color)
{
	//determine_ID();//判断发送者ID和其对应的客户端ID
	/*- 帧头 -*/	
	graphic.Frame.SOF = 0xA5;
	graphic.Frame.Seq = 0;	
	graphic.Frame.DataLength = sizeof(graphic.ID) + sizeof(graphic.data);
	/*- ID数据 -*/
  graphic.CmdID = 0x0301;							//命令码
	graphic.ID.cmd_ID = 0x0104;					//数据内容ID  104  客户端绘制7个图形
	//ID已经是自动读取的了
	graphic.ID.send_ID  = 2;					//发送者的ID				101蓝英雄				
	graphic.ID.receiver_ID = 0x0102;		//客户端的ID，只能为发送者机器人对应的客户端  蓝英雄操作手

	/*- 自定义内容一 -*/	
	graphic.data[0].graphic_name[0] = 0;			//？？？
	graphic.data[0].graphic_name[1] = 0;			//？？？
	graphic.data[0].graphic_name[2] = 8;			//？？？
	graphic.data[0].operate_tpye = Variable;	//操作方式，0空1增加2修改3删除
	graphic.data[0].graphic_tpye = 1;					//0直线 1矩形 2圆 3椭圆 4圆弧
	graphic.data[0].layer = 1;								//图层（0-9可选）
	graphic.data[0].color = color;								//颜色 0红蓝 1黄 2绿 3橙 4紫 5粉 6青 7黑 8白
	graphic.data[0].start_angle = 0;					//0-360
	graphic.data[0].end_angle = 0;						//0-360
	graphic.data[0].width = 2;								//线宽
	graphic.data[0].start_x = 841;						//起点 //中点x：960 y：540
	graphic.data[0].start_y = 195;
	graphic.data[0].radius = 20; 							//字体大小或半径
	graphic.data[0].end_x = 860;
	graphic.data[0].end_y = 120;
	/*- 自定义内容二 -*/	
	graphic.data[1].graphic_name[0] = 0;
	graphic.data[1].graphic_name[1] = 0;
	graphic.data[1].graphic_name[2] = 9;
	graphic.data[1].operate_tpye = Variable;
	graphic.data[1].graphic_tpye = 1;
	graphic.data[1].layer = 1;
	graphic.data[1].color = color;
	graphic.data[1].start_angle = 0;
	graphic.data[1].end_angle = 0;
	graphic.data[1].width = 2;
	graphic.data[1].start_x = 1010;//中点x：960 y：540
	graphic.data[1].start_y = 195;
	graphic.data[1].radius = 20; 
	graphic.data[1].end_x = 1030;
	graphic.data[1].end_y = 120;	
  /*- 自定义内容三 -*/	
	graphic.data[2].graphic_name[0] = 0;
	graphic.data[2].graphic_name[1] = 0;
	graphic.data[2].graphic_name[2] = 10;
	graphic.data[2].operate_tpye = Variable;
	graphic.data[2].graphic_tpye = 0;
	graphic.data[2].layer = 1;
	graphic.data[2].color = color;
	graphic.data[2].start_angle = 0;
	graphic.data[2].end_angle = 0;
	graphic.data[2].width = 2;
	graphic.data[2].start_x = 813;//中点x：960 y：540
	graphic.data[2].start_y = 165;
	graphic.data[2].radius = 0; 
	graphic.data[2].end_x = 1059;
	graphic.data[2].end_y = 165;
	/*- 自定义内容四 -*/	
	graphic.data[3].graphic_name[0] = 0;
	graphic.data[3].graphic_name[1] = 0;
	graphic.data[3].graphic_name[2] = 11;
	graphic.data[3].operate_tpye = Variable;
	graphic.data[3].graphic_tpye = 0;
	graphic.data[3].layer = 1;
	graphic.data[3].color = color;
	graphic.data[3].start_angle = 0;
	graphic.data[3].end_angle = 0;
	graphic.data[3].width = 2;
	graphic.data[3].start_x = 887;//中点x：960 y：540
	graphic.data[3].start_y = 375;
	graphic.data[3].radius = 0; 
	graphic.data[3].end_x = 887;
	graphic.data[3].end_y = 165;
	/*- 自定义内容五 -*/	
	graphic.data[4].graphic_name[0] = 0;
	graphic.data[4].graphic_name[1] = 0;
	graphic.data[4].graphic_name[2] = 12;
	graphic.data[4].operate_tpye = Variable;
	graphic.data[4].graphic_tpye = 0;
	graphic.data[4].layer = 1;
	graphic.data[4].color = color;
	graphic.data[4].start_angle = 0;
	graphic.data[4].end_angle = 0;
	graphic.data[4].width = 2;
	graphic.data[4].start_x = 984;//中点x：960 y：540
	graphic.data[4].start_y = 375;
	graphic.data[4].radius = 0; 
	graphic.data[4].end_x = 984;
	graphic.data[4].end_y = 165;
	/*- 自定义内容六 -*/	
	graphic.data[5].graphic_name[0] = 0;
	graphic.data[5].graphic_name[1] = 0;
	graphic.data[5].graphic_name[2] = 13;
	graphic.data[5].operate_tpye = Variable;
	graphic.data[5].graphic_tpye = 0;
	graphic.data[5].layer = 1;
	graphic.data[5].color = color;
	graphic.data[5].start_angle = 0;
	graphic.data[5].end_angle = 0;
	graphic.data[5].width = 2;
	graphic.data[5].start_x = 854;//中点x：960 y：540
	graphic.data[5].start_y = 375;
	graphic.data[5].radius = 0; 
	graphic.data[5].end_x = 1015;
	graphic.data[5].end_y = 375;
	/*- 自定义内容七 -*/	
	graphic.data[6].graphic_name[0] = 0;
	graphic.data[6].graphic_name[1] = 0;
	graphic.data[6].graphic_name[2] = 14;
	graphic.data[6].operate_tpye = 3;					//倾斜摇臂暂时不显示
	graphic.data[6].graphic_tpye = 0;
	graphic.data[6].layer = 1;
	graphic.data[6].color = color;
	graphic.data[6].start_angle = 0;
	graphic.data[6].end_angle = 0;
	graphic.data[6].width = 1;
	graphic.data[6].start_x = 865;//中点x：960 y：540
	graphic.data[6].start_y = 219;
	graphic.data[6].radius = 0; 
	graphic.data[6].end_x = 1047;
	graphic.data[6].end_y = 306;
	memcpy(CliendTxBuffer_graphic,&graphic,sizeof(graphic));
	Append_CRC8_Check_Sum(CliendTxBuffer_graphic,sizeof(graphic.Frame));//******
	Append_CRC16_Check_Sum(CliendTxBuffer_graphic,sizeof(graphic));//*******
	
	//  /*- 打包写入发送 -*/
	
	HAL_UART_Transmit_DMA(&huart6,CliendTxBuffer_graphic,sizeof(CliendTxBuffer_graphic));
}

void showWorkingStatus(uint8_t Variable, uint8_t mode)		//0锁 1校准 2就绪 3关电
{
	unsigned char character_demo[30]="Status:Null";
	character.data.end_angle = 20;
	switch(mode)
	{
		case 0:
			memcpy(character_demo,"Status: Locked",15);
			character.data.end_angle = 15;
			character.data.color = 4;				//紫红色
		break;
		case 1:
			memcpy(character_demo,"Status: calibrate",18);
			character.data.end_angle = 18;
			character.data.color = 3;				//橙色
		break;
		case 2:
			memcpy(character_demo,"Status: Ready",14);
			character.data.end_angle = 14;
			character.data.color = 2;				//绿色
		break;
		case 3:
			memcpy(character_demo,"Status: Power Off",18);
			character.data.end_angle = 18;
			character.data.color = 3;				//橙色
		break;
		default:break;
	}
	/*- 帧头 -*/	
	character.Frame.SOF = 0xA5;
	character.Frame.Seq = 0;	
	character.Frame.DataLength = sizeof(character.ID) + sizeof(character.data);
	/*- ID数据 -*/
  character.CmdID = 0x0301;//命令码
	character.ID.cmd_ID = 0x0110;//数据内容ID 
	//ID已经是自动读取的了
	character.ID.send_ID  = 2;//发送者的ID
	character.ID.receiver_ID = 0x0102;//客户端的ID，只能为发送者机器人对应的客户端
	/*- 自定义内容一 -*/	
	character.data.character_name[0] = 1;
	character.data.character_name[1] = 0;
	character.data.character_name[2] = 0;
	character.data.operate_tpye = Variable;
	character.data.graphic_tpye = 7;
	character.data.layer = 2;
	//character.data.color = 5;
	character.data.start_angle = 20;
	//character.data.end_angle = 5;
	character.data.width = 2;
	character.data.start_x = 1060;
	character.data.start_y = 640;
  memcpy(character.data.data, &character_demo, sizeof(character_demo));
	/*- 自定义内容一 -*/	
	
	memcpy(CliendTxBuffer_character,&character,sizeof(character));
	Append_CRC8_Check_Sum(CliendTxBuffer_character,sizeof(character.Frame));//******
	Append_CRC16_Check_Sum(CliendTxBuffer_character,sizeof(character));//*******
	
	//  /*- 打包写入发送 -*/
	
	HAL_UART_Transmit_DMA(&huart6,CliendTxBuffer_character,sizeof(CliendTxBuffer_character));
}

void showWorkingMode(uint8_t Variable, uint8_t mode)		//0移动 1抬升
{
	unsigned char character_demo[30]="Status:Null";
	character.data.end_angle = 20;
	switch(mode)
	{
		case 0:memcpy(character_demo,"Mode: Moving",13);		break;
		case 1:memcpy(character_demo,"Mode: Lifting",14);	break;
		default:break;
	}
	/*- 帧头 -*/	
	character.Frame.SOF = 0xA5;
	character.Frame.Seq = 0;	
	character.Frame.DataLength = sizeof(character.ID) + sizeof(character.data);
	/*- ID数据 -*/
  character.CmdID = 0x0301;//命令码
	character.ID.cmd_ID = 0x0110;//数据内容ID 
	//ID已经是自动读取的了
	character.ID.send_ID  = 2;//发送者的ID
	character.ID.receiver_ID = 0x0102;//客户端的ID，只能为发送者机器人对应的客户端
	/*- 自定义内容一 -*/	
	character.data.character_name[0] = 1;
	character.data.character_name[1] = 0;
	character.data.character_name[2] = 1;
	character.data.operate_tpye = Variable;
	character.data.graphic_tpye = 7;
	character.data.layer = 2;
	character.data.color = 2;
	character.data.start_angle = 20;
	//character.data.end_angle = 5;
	character.data.width = 2;
	character.data.start_x = 1100;
	character.data.start_y = 600;
  memcpy(character.data.data, &character_demo, sizeof(character_demo));
	/*- 自定义内容一 -*/	
	
	memcpy(CliendTxBuffer_character,&character,sizeof(character));
	Append_CRC8_Check_Sum(CliendTxBuffer_character,sizeof(character.Frame));//******
	Append_CRC16_Check_Sum(CliendTxBuffer_character,sizeof(character));//*******
	
	//  /*- 打包写入发送 -*/
	
	HAL_UART_Transmit_DMA(&huart6,CliendTxBuffer_character,sizeof(CliendTxBuffer_character));
}

void showLiftPosition(uint8_t Variable, float h1, float h2, float h3, float Ang_w)		//h1是后高度 h2是前高度
{
	float Ang_1 = (float)(atan((h2-h1)/220.0));									//弧度制度
	float w			= (float)(Ang_w * PI / 180.0);										//转弧度
	int16_t Xc,Yc,Xd,Yd,Xe,Ye,Xf,Yf,Xg,Yg;							//一些点的坐标
	Xc = -45 * cos(Ang_1);
	Yc = h1 - 45 * sin(Ang_1);
	Xd = 410 * cos(Ang_1);
	Yd = h1 + 410 * sin(Ang_1);
	Xe = (h3 + 415) * cos(Ang_1);
	Ye = h1 + (h3 + 415) * sin(Ang_1);
	Xf = Xe + 119 * cos(w);
	Yf = Ye - 119 * sin(w);
	Xg = (h3 - 45) * cos(Ang_1);
	Yg = h1 + (h3 - 45) * sin(Ang_1);
	
	/*- 帧头 -*/	
	graphic.Frame.SOF = 0xA5;
	graphic.Frame.Seq = 0;	
	graphic.Frame.DataLength = sizeof(graphic.ID) + sizeof(graphic.data);
	/*- ID数据 -*/
  graphic.CmdID = 0x0301;							//命令码
	graphic.ID.cmd_ID = 0x0104;					//数据内容ID  103  客户端绘制5个图形
	//ID已经是自动读取的了
	graphic.ID.send_ID  = 2;					//发送者的ID				101蓝英雄				
	graphic.ID.receiver_ID = 0x0102;		//客户端的ID，只能为发送者机器人对应的客户端  蓝英雄操作手

	/*- 自定义内容一 -*/	
	graphic.data[0].graphic_name[0] = 0;			//？？？
	graphic.data[0].graphic_name[1] = 0;			//？？？
	graphic.data[0].graphic_name[2] = 1;			//？？？
	graphic.data[0].operate_tpye = Variable;	//操作方式，0空1增加2修改3删除
	graphic.data[0].graphic_tpye = 0;					//0直线 1矩形 2圆 3椭圆 4圆弧
	graphic.data[0].layer = 1;								//图层（0-9可选）
	graphic.data[0].color = 2;								//颜色 0红蓝 1黄 2绿 3橙 4紫 5粉 6青 7黑 8白
	graphic.data[0].start_angle = 0;					//0-360
	graphic.data[0].end_angle = 0;						//0-360
	graphic.data[0].width = 2;								//线宽
	graphic.data[0].start_x = ToPixel_x(Xc);
	graphic.data[0].start_y = ToPixel_y(Yc);
	graphic.data[0].radius = 20; 							//字体大小或半径
	graphic.data[0].end_x = ToPixel_x(Xd);
	graphic.data[0].end_y = ToPixel_y(Yd);
	/*- 自定义内容二 -*/	
	graphic.data[1].graphic_name[0] = 0;
	graphic.data[1].graphic_name[1] = 0;
	graphic.data[1].graphic_name[2] = 2;
	graphic.data[1].operate_tpye = Variable;
	graphic.data[1].graphic_tpye = 0;
	graphic.data[1].layer = 1;
	graphic.data[1].color = 4;
	graphic.data[1].start_angle = 0;
	graphic.data[1].end_angle = 0;
	graphic.data[1].width = 2;
	graphic.data[1].start_x = ToPixel_x(Xg);
	graphic.data[1].start_y = ToPixel_y(Yg);
	graphic.data[1].radius = 20; 
	graphic.data[1].end_x = ToPixel_x(Xe);
	graphic.data[1].end_y = ToPixel_y(Ye);	
  /*- 自定义内容三 -*/	
	graphic.data[2].graphic_name[0] = 0;
	graphic.data[2].graphic_name[1] = 0;
	graphic.data[2].graphic_name[2] = 3;
	graphic.data[2].operate_tpye = Variable;
	graphic.data[2].graphic_tpye = 0;
	graphic.data[2].layer = 1;
	graphic.data[2].color = 3;
	graphic.data[2].start_angle = 0;
	graphic.data[2].end_angle = 0;
	graphic.data[2].width = 2;
	graphic.data[2].start_x = ToPixel_x(Xe);//中点x：960 y：540
	graphic.data[2].start_y = ToPixel_y(Ye);
	graphic.data[2].radius = 0; 
	graphic.data[2].end_x = ToPixel_x(Xf);
	graphic.data[2].end_y = ToPixel_y(Yf);
	/*- 自定义内容四 -*/	
	graphic.data[3].graphic_name[0] = 0;
	graphic.data[3].graphic_name[1] = 0;
	graphic.data[3].graphic_name[2] = 4;
	graphic.data[3].operate_tpye = 3;				//不需要
	graphic.data[3].graphic_tpye = 0;
	graphic.data[3].layer = 1;
	graphic.data[3].color = 3;
	graphic.data[3].start_angle = 0;
	graphic.data[3].end_angle = 0;
	graphic.data[3].width = 2;
	graphic.data[3].start_x = 887;//中点x：960 y：540
	graphic.data[3].start_y = 375;
	graphic.data[3].radius = 0; 
	graphic.data[3].end_x = 887;
	graphic.data[3].end_y = 165;
	/*- 自定义内容五 -*/	
	graphic.data[4].graphic_name[0] = 0;
	graphic.data[4].graphic_name[1] = 0;
	graphic.data[4].graphic_name[2] = 5;
	graphic.data[4].operate_tpye = 3;				//不需要
	graphic.data[4].graphic_tpye = 0;
	graphic.data[4].layer = 1;
	graphic.data[4].color = 3;
	graphic.data[4].start_angle = 0;
	graphic.data[4].end_angle = 0;
	graphic.data[4].width = 2;
	graphic.data[4].start_x = 984;//中点x：960 y：540
	graphic.data[4].start_y = 375;
	graphic.data[4].radius = 0; 
	graphic.data[4].end_x = 984;
	graphic.data[4].end_y = 165;

	memcpy(CliendTxBuffer_graphic,&graphic,sizeof(graphic));
	Append_CRC8_Check_Sum(CliendTxBuffer_graphic,sizeof(graphic.Frame));//******
	Append_CRC16_Check_Sum(CliendTxBuffer_graphic,sizeof(graphic));//*******
	
	//  /*- 打包写入发送 -*/
	//printf("in: %.2f c:%d %d e:%d %d\n", h1, ToPixel_x(Xc),ToPixel_y(Yc),ToPixel_x(Xd),ToPixel_y(Yd));
	HAL_UART_Transmit_DMA(&huart6,CliendTxBuffer_graphic,sizeof(CliendTxBuffer_graphic));
}

static uint8_t WorkingStatus = 0,WorkingMode = 0;
void getWorkingInformation(void)
{
	if(PY_Lift.calibrationStatus & (1<<2))WorkingStatus = 1;							//校准中
	else WorkingStatus = 2;																								//校准完成
	if(remote_control.switch_right == Switch_Down)WorkingStatus = 3;			//显示断电，优先级最高
	
	if(remote_control.switch_left == Switch_Down)WorkingMode = 0;				//显示移动模式
	if(remote_control.switch_left == Switch_Middle)WorkingMode = 1;			//显示抬升模式
	if(remote_control.switch_left == Switch_Up)WorkingMode = 1;					//显示抬升模式
}


void CPXT_handle(void)
{
	static uint32_t time_CPTX = 0;
	static uint8_t timeIsOK = 0, step = 0;
	
	if(HAL_GetTick() - time_CPTX > 40)		//40ms
	{
		time_CPTX = HAL_GetTick();
		if((++step) >= 35)step = 0;
	}
	getWorkingInformation();							//获取显示量
	//使用状态机刷新裁判系统
	if(step == 1 ){showBodyBackground(1,3); return;}										//初始化
	if(step == 7 ){showWorkingStatus(1,0);	 return;}										//初始化
	if(step == 13){showWorkingMode(1,0);		 return;}										//初始化
	if(step == 19){showWorkingStatus(2,WorkingStatus);	 return;}				//0锁 1校准 2就绪 3关电
	if(step == 25){showWorkingMode(2,WorkingMode);		 return;}					//0移动 1抬升
	if(step == 31){showLiftPosition(1, 0, 0,0, 0); return;}
	if(step % 6 != 1){showLiftPosition(2, PY_Lift.height_S, PY_Lift.height_W,PY_Lift.length_Flex, PY_Lift.angle_Pitch); return;}
	
}
