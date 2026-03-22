#ifndef UART_H
#define UART_H

#ifdef __cplusplus
extern "C"
{
#endif

// 串口说法缓冲区
#define UART_BUFFER_SIZE 1024

    typedef struct
    {
        u8 *SendBuffer;    // 发送缓冲区指针
        u8 *ReceiveBuffer; // 接收缓冲区指针
        int SendLength;    // 发送数据长度
        int ReceiveLength; // 接收数据长度
    } UartBuffers;

    typedef struct
    {
        XUartPs *pUartInstance; // UART实例
        UartBuffers Buffers;    // UART缓冲区结构体
    } UartContext;

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // UART_H