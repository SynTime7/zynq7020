#include "../main.h"

// 重定义ID号
#define UART_DEVICE_ID XPAR_PS7_UART_0_DEVICE_ID // UART设备ID
#define UART_INT_ID XPAR_XUARTPS_0_INTR          // UART中断ID

u8 UartSendBuffer[UART_BUFFER_SIZE];    // 发送缓冲区
u8 UartReceiveBuffer[UART_BUFFER_SIZE]; // 接收缓冲区

XUartPs UartInstance; // UART实例

// 全局UART上下文
UartContext g_suart_context = {
    .pUartInstance = &UartInstance, // 指向UART实例
    .Buffers = {
        .SendBuffer = UartSendBuffer,       // 发送缓冲区指针
        .ReceiveBuffer = UartReceiveBuffer, // 接收缓冲区指针
        .SendLength = 0,                    // 发送数据长度初始化为0
        .ReceiveLength = 0                  // 接收数据长度初始化为0
    }};

/* 串口收发函数 */
void uart_intr_handler(void *call_back_ref)
{
    XUartPs *Uart_receive_ptr = (XUartPs *)call_back_ref;
    int Buffer = 0;
    int Int_flage;

    // 获取中断状态
    Int_flage = XUartPs_ReadReg(Uart_receive_ptr->Config.BaseAddress, XUARTPS_SR_OFFSET);
    Int_flage &= (XUARTPS_IXR_RXOVR | XUARTPS_IXR_RXEMPTY);

    // 处理接收数据
    if (Int_flage & XUARTPS_IXR_RXOVR)
    {
        // 将FIFO中的数据读出，清除中断标志
        while (XUartPs_IsReceiveData(Uart_receive_ptr->Config.BaseAddress))
        {
            Buffer = XUartPs_RecvByte(Uart_receive_ptr->Config.BaseAddress);

            // 存储接收到的数据
            g_suart_context.Buffers.ReceiveBuffer[g_suart_context.Buffers.ReceiveLength++] = (u8)Buffer;
        }

        // 清除中断标志
        XUartPs_WriteReg(Uart_receive_ptr->Config.BaseAddress, XUARTPS_ISR_OFFSET, XUARTPS_IXR_RXOVR);
    }

    // 将接受到的数据发送回去
    if (g_suart_context.Buffers.ReceiveLength > 0)
    {
        for (int i = 0; i < g_suart_context.Buffers.ReceiveLength; i++)
        {
            XUartPs_SendByte(Uart_receive_ptr->Config.BaseAddress, g_suart_context.Buffers.ReceiveBuffer[i]);
        }
        // 发送完成后重置接收长度
        g_suart_context.Buffers.ReceiveLength = 0;
    }
}

int uart_init()
{
    int Status;
    XUartPs_Config *Config;

    // 获取UART配置
    Config = XUartPs_LookupConfig(UART_DEVICE_ID);
    assert(Config != NULL);

    // 初始化UART实例
    Status = XUartPs_CfgInitialize(g_suart_context.pUartInstance, Config, Config->BaseAddress);
    assert(Status == XST_SUCCESS);

    // 功能自检
    Status = XUartPs_SelfTest(g_suart_context.pUartInstance);
    assert(Status == XST_SUCCESS);

    // 设置普通工作模式
    XUartPs_SetOperMode(g_suart_context.pUartInstance, XUARTPS_OPER_MODE_NORMAL);

    // 设置波特率
    Status = XUartPs_SetBaudRate(g_suart_context.pUartInstance, 115200);
    assert(Status == XST_SUCCESS);

    // 设置触发中断数据量为1，最大63
    XUartPs_SetFifoThreshold(g_suart_context.pUartInstance, 1);

    // 注册中断处理函数
    XScuGic_Connect(&InterruptController, UART_INT_ID, (Xil_InterruptHandler)uart_intr_handler, g_suart_context.pUartInstance);

    // 设置中断掩码，启用接收中断
    XUartPs_SetInterruptMask(g_suart_context.pUartInstance, XUARTPS_IXR_RXOVR);

    // 清除接收中断标志
    XUartPs_WriteReg(g_suart_context.pUartInstance->Config.BaseAddress, XUARTPS_ISR_OFFSET, XUARTPS_IXR_RXOVR);

    // 使能UART中断
    XScuGic_Enable(&InterruptController, UART_INT_ID);

    return XST_SUCCESS;
}
