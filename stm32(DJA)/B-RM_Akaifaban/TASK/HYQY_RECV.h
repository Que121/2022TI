#ifndef _HYQY_RECV_H
#define _HYQY_RECV_H
//#include "YQYstd.h"
#include "main.h"

/**
 * 使用方法如下：
 * 发送任意长（小于RECV_BUFFER_MAX）的整数，数据间用英文逗号隔开
 * 注意保证整帧为：
 *          数据首1、数据首2 + 数据1, + 数据2 , +...+数据n, + 数据尾1 + 数据尾2
 * 例如(char)：pp12,666,266,1234,\r\n
 */

#define RECV_BUFFER_MAX     64              //接收缓冲区大小
#define RECV_BEG1           0x70            //数据首1   'p'
#define RECV_BEG2           0x70            //数据首2   'p'

#define RECV_END1           0x0D            //数据尾1   '\r'
#define RECV_END2           0x0A            //数据尾2   '\n'


typedef struct 
{
    uint8_t     UART_id;                    //记录串口几号
    uint8_t     recvBuff[RECV_BUFFER_MAX];
    uint16_t    recvNum;                    //记录接收了几个数据了
    uint16_t    recvPin_Now;                //指针，指向当前缓冲位
    uint8_t     findBegin;                  //如果找到头，置1
    uint8_t     findEnd;                    //如果找到尾，置1，头置0
    uint8_t     DataIsOK;                   //数据接收完成，解码完成
    uint32_t    recvRes[10];                //接收的成品数据
    uint8_t     LastRecv;                   //记录上次接收的字符
}HYQY_RECV_Typedef;

extern HYQY_RECV_Typedef HYQY_recvInfo[2];  

/* 初始化接收 */
void HYQY_Recv_Init(HYQY_RECV_Typedef* recv_Type, uint8_t UART_id);

/* 放入接收中断内，用于接收一次字符 */
void HYQY_Recv_Handle(HYQY_RECV_Typedef* recv_Type,uint8_t chr);

/* 对缓冲区数据解码 */
void HYQY_DeBuf(HYQY_RECV_Typedef* recv_Type,uint8_t* buf,uint16_t num);

/* 判断接收是否完成 */
uint8_t IsReceiveDataOK(HYQY_RECV_Typedef* recv_Type);

/* 展示解码完成的信息 */
void ShowDecodeResult(HYQY_RECV_Typedef* recv_Type);

/* 测试demo： */
void undefinedReceiveShowDemo_Handle(void);
void undefinedReceive_LED_Demo_Handle(void);

#endif      //_HYQY_RECV_H
