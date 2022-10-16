#include "power.h"
#include "MyConfig.h"

void power_on_24v_ALL(void)
{
    uint8_t i = 0;
    for(i = 1; i < 5; i++)
    {
        power_on_24v(i);            								//接口上电
#if POWER_24V_SOFT_START_ENABLE											//该宏 定义在"MyConfig.h"
        HAL_Delay(POWER_24V_SOFT_START_DELAY);      //缓启动
#endif
    }
}
void power_off_24v_ALL(void)
{
    uint8_t i = 0;
    for(i = '1'; i <= '4'; i++)
    {
        power_off_24v(i);            								//接口断电
    }
}

void power_on_24v(uint8_t num)
{
    switch (num)
    {
        case 1:
        case '1':
            HAL_GPIO_WritePin(POWER1_CTRL_GPIO_Port,POWER1_CTRL_Pin,GPIO_PIN_SET);break;
        case 2:
        case '2':
            HAL_GPIO_WritePin(POWER2_CTRL_GPIO_Port,POWER2_CTRL_Pin,GPIO_PIN_SET);break;
        case 3:
        case '3':
            HAL_GPIO_WritePin(POWER3_CTRL_GPIO_Port,POWER3_CTRL_Pin,GPIO_PIN_SET);break;
        case 4:
        case '4':
            HAL_GPIO_WritePin(POWER4_CTRL_GPIO_Port,POWER4_CTRL_Pin,GPIO_PIN_SET);break;
    }
}

void power_off_24v(uint8_t num)
{
    switch (num)
    {
        case 1:
        case '1':
            HAL_GPIO_WritePin(POWER1_CTRL_GPIO_Port,POWER1_CTRL_Pin,GPIO_PIN_RESET);break;
        case 2:
        case '2':
            HAL_GPIO_WritePin(POWER2_CTRL_GPIO_Port,POWER2_CTRL_Pin,GPIO_PIN_RESET);break;
        case 3:
        case '3':
            HAL_GPIO_WritePin(POWER3_CTRL_GPIO_Port,POWER3_CTRL_Pin,GPIO_PIN_RESET);break;
        case 4:
        case '4':
            HAL_GPIO_WritePin(POWER4_CTRL_GPIO_Port,POWER4_CTRL_Pin,GPIO_PIN_RESET);break;
    }
}

