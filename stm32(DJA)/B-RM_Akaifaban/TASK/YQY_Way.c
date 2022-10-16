#include "YQY_Way.h"
#include "buzzer.h"

#include "PY_2022DianSai.h"

void YQY_RuKu(void)
{
	//入库代码
	PowerOff();
	HAL_Delay(500);
	ServoSpark();
	YQY_GoAddMap(540,-60);
	YQY_GoAddMap(-1660,60);
	YQY_GoAddMap(-654,0);
}
void YQY_RuKuDiDi(void)
{
	PowerOff();
	playNote('4',300);
	HAL_Delay(1000);
	TotalStep ++;					//进入下一个流程
}
void YQY_ChuKu(void)
{
	ServoSpark();
	YQY_GoAddMap(820,0);
	YQY_GoAddMap(1230,60);
	YQY_GoAddMap(-811,-60);
	TotalStep ++;					//进入下一个流程
}
void YQY_RuCe(void)
{
	//侧方停车代码
}
void YQY_ChuCe(void)
{
	ServoSpark();
	YQY_GoAddMap(600,-60);
	YQY_GoAddMap(570,0);
	YQY_GoAddMap(1130,60);
}

