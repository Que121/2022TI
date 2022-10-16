#include "superCap.h"

SuperCap_t SuperCap;


void SuperCap_GetInformation(uint8_t * RAWdata)							//获取超级电容相关信息
{
	SuperCap.Vot_input = (RAWdata[1]<<8 | RAWdata[0])/100.0;	//输入电压
	SuperCap.Vot_cap 	 = (RAWdata[3]<<8 | RAWdata[2])/100.0;	//电容电压
	SuperCap.Cur_input = (RAWdata[5]<<8 | RAWdata[4])/100.0;	//输入电流
	SuperCap.Set_power = (RAWdata[7]<<8 | RAWdata[6])/100.0;	//之前设置的功率
	
	SuperCap.power = SuperCap.Vot_input * SuperCap.Cur_input;	//电压 x 电流 = 功率
}


