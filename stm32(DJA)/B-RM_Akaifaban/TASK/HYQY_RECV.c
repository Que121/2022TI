#include "HYQY_RECV.h"

#include "stdio.h"                                      //使用其中的调试功能

HYQY_RECV_Typedef HYQY_recvInfo[2];

void HYQY_Recv_Init(HYQY_RECV_Typedef* recv_Type, uint8_t UART_id)
{
    uint16_t i = 0;
    for(; i < RECV_BUFFER_MAX; i++)
    {
        recv_Type->recvBuff[i] = 0;                     //清空缓冲区  
    }
    recv_Type->UART_id = UART_id;
    recv_Type->recvNum = 0;                             //清空接收的字节个数
    recv_Type->recvPin_Now = 0;
    recv_Type->findBegin = 0;
    recv_Type->findEnd = 0;
    recv_Type->DataIsOK = 0;
    recv_Type->LastRecv = 0;
}
void HYQY_Recv_Handle(HYQY_RECV_Typedef* recv_Type,uint8_t chr)
{
    //找到头
    if((recv_Type->LastRecv == RECV_BEG1) && (chr == RECV_BEG2))   
    {
			//printf("findHead\r\n");
        recv_Type->findBegin = 1;                       //标志位置1
        recv_Type->LastRecv = 0;                        //清空上次记录
        return;                                         //直接结束本次
    }
    if(recv_Type->findBegin)                            //已经出现头
    {                                                   //将结果存到缓冲区
        recv_Type->recvBuff[recv_Type->recvPin_Now++] = chr;    
        if((recv_Type->LastRecv == RECV_END1) && (chr == RECV_END2))
        {
            recv_Type->findBegin = 0;                   //找到尾，置0
            recv_Type->findEnd   = 1;                   //找到尾，置1
            recv_Type->LastRecv  = 0;                   //清空上次的记录
        }
        else recv_Type->LastRecv = chr;                 //记录上个字符
    }
    if(recv_Type->findEnd)                              //已经出现尾巴，证明结束了
    {
        //数据解码
        HYQY_DeBuf(recv_Type, recv_Type->recvBuff, recv_Type->recvPin_Now - 2);
        //清空标志位置
        recv_Type->findEnd = 0;
        recv_Type->recvPin_Now = 0;
    }
    else recv_Type->LastRecv = chr;
}

void HYQY_DeBuf(HYQY_RECV_Typedef* recv_Type,uint8_t* buf,uint16_t num)
{
    uint8_t NumPin = 0;                                 //存储第几个数据？
    uint8_t i = 0;
    for(i = 0;i < recv_Type->recvNum + 1; i++)
        recv_Type->recvRes[i] = 0;
    for(i = 0;i < num; i++)
    {
        if(buf[i] != ',')
        {
            recv_Type->recvRes[NumPin] *= 10;
            recv_Type->recvRes[NumPin] += buf[i] - '0';
        }
        else NumPin ++;
    }
    recv_Type->recvNum = NumPin;                        //一共找到了这么多个数据
    recv_Type->DataIsOK = 1;
    //printf("-----Decode--finish-----\r\n");
}

uint8_t IsReceiveDataOK(HYQY_RECV_Typedef* recv_Type)   //判断接收是否完成
{
    if(recv_Type->DataIsOK)
    {
        recv_Type->DataIsOK = 0;
        return 1;
    }
    return 0;
}

void ShowDecodeResult(HYQY_RECV_Typedef* recv_Type)     //展示解码信息
{
    uint8_t i = 0;
    printf("\n UART %d decode %d datas:\r\n", recv_Type->UART_id, recv_Type->recvNum);
    for(i = 0; i < recv_Type->recvNum; i++)
    {
        printf("recvRes[%d]:%d\r\n", i, recv_Type->recvRes[i]);
    }
    //printf("-----finish-----\r\n");
}

//void undefinedReceiveShowDemo_Handle(void)
//{
//    YQYlib_Init();

//    UART0_Init();                           //串口0 用于收发数据
//    UART1_Init();                           //串口1 用于调试
//    HYQY_Recv_Init(&HYQY_recvInfo[0],0);    //初始化不定长接收功能
//    HYQY_Recv_Init(&HYQY_recvInfo[1],1);    //初始化不定长接收功能

//    printf("from YQY :start!\n");           //打印欢迎信息
//    while(1)                                //死循环，如果接收到数据，解码打印
//    {
//        if(IsReceiveDataOK(&HYQY_recvInfo[0]))
//            ShowDecodeResult(&HYQY_recvInfo[0]);
//        if(IsReceiveDataOK(&HYQY_recvInfo[1]))
//            ShowDecodeResult(&HYQY_recvInfo[1]);
//    }
//}

//void undefinedReceive_LED_Demo_Handle(void)
//{
//    YQYlib_Init();

//    UART1_Init();                           //串口1 用于调试
//    HYQY_Recv_Init(&HYQY_recvInfo[1],1);    //初始化不定长接收功能
//    ledInit();                              //初始化LED

//    printf("from YQY :start!\n");           //打印欢迎信息
//    while(1)                                //死循环，如果接收到数据，解码打印
//    {
//        if(HYQY_recvInfo[1].recvRes[0] == 10)
//            ledOut(HIGH,HIGH);
//        if(HYQY_recvInfo[1].recvRes[0] == 11)
//            ledOut(LOW,LOW);
//    }
//}
