#include "../main.h"
#include "xscutimer.h"

/**********************************************************************
 * @brief  zynq 私有定时器初始化
 * @param  None
 **********************************************************************/
#define TIMER_DEVICE_ID XPAR_SCUTIMER_DEVICE_ID // 定时器设备IDF
#define TIMER_IRPT_INTR XPAR_SCUTIMER_INTR      // 定时器中断ID

/*
 * @brief 定时器频率 = XPAR_CPU_CORTEXA9_0_CPU_CLK_FREQ_HZ/2 = 333Mhz
 *      定时器加载值 = 定时器频率 / 1000000 = 333，即每1s中断一次
 */
#define TIMER_FREQUENCY_HZ (XPAR_CPU_CORTEXA9_0_CPU_CLK_FREQ_HZ / 2)
#define TIMER_LOAD_VALUE (TIMER_FREQUENCY_HZ - 1) // 定时器加载值，设置为每1秒中断一次

// 定时器实例
XScuTimer TimerInstance;

// 定时器中断处理函数
void TimerIntrHandler(void *CallBackRef)
{
    XScuTimer *TimerInstancePtr = (XScuTimer *)CallBackRef;

    // 清除定时器中断
    XScuTimer_ClearInterruptStatus(TimerInstancePtr);

    // 在此处执行定时器中断处理逻辑，例如更新系统时间、触发任务调度等
    printf("Timer interrupt occurred!\n");
}

// 定时器中断初始化函数
int Priv_TimerIntrInit()
{
    int Status;

    // 连接定时器中断处理函数
    Status = XScuGic_Connect(&InterruptController, TIMER_IRPT_INTR, (Xil_ExceptionHandler)TimerIntrHandler, &TimerInstance);
    assert(Status == XST_SUCCESS);

    // 使能定时器中断
    XScuGic_Enable(&InterruptController, TIMER_IRPT_INTR);
    XScuTimer_EnableInterrupt(&TimerInstance);

    return XST_SUCCESS;
}

// 定时器初始化函数
int Priv_TimerInit()
{
    int Status;
    XScuTimer_Config *TimerConfig;

    // 获取定时器配置
    TimerConfig = XScuTimer_LookupConfig(TIMER_DEVICE_ID);
    assert(TimerConfig != NULL);

    // 初始化定时器实例
    Status = XScuTimer_CfgInitialize(&TimerInstance, TimerConfig, TimerConfig->BaseAddr);
    assert(Status == XST_SUCCESS);

    // 设置重装载值
    XScuTimer_LoadTimer(&TimerInstance, TIMER_LOAD_VALUE);

    // 使能自动重装载
    XScuTimer_EnableAutoReload(&TimerInstance);

    // 配置中断
    Status = Priv_TimerIntrInit();
    assert(Status == XST_SUCCESS);

    // 启动定时器
    XScuTimer_Start(&TimerInstance);

    return XST_SUCCESS;
}

/**********************************************************************
 * @brief TTC定时器初始化
 * @param  None
 **********************************************************************/
#include "xttcps.h"

#define TTC0_0_DEVICE_ID XPAR_XTTCPS_0_DEVICE_ID // TTC0_0定时器设备ID
#define TTC0_0_IRPT_INTR XPS_TTC0_0_INT_ID       // TTC0_0定时器中断ID
#define TTC0_0_FREQUENCY_HZ (100000)             // TTC0_0定时器频率，设置为100KHz

#define TTC1_0_DEVICE_ID XPAR_XTTCPS_3_DEVICE_ID // TTC1_0定时器设备ID
#define TTC1_0_IRPT_INTR XPS_TTC1_0_INT_ID       // TTC1_0定时器中断ID
#define TTC1_0_FREQUENCY_HZ (1000)               // TTC1_0定时器频率，设置为1KHz

// TTC定时器实例
XTtcPs TtcInstance[XPAR_XTTCPS_NUM_INSTANCES];

static volatile u8 ErrorCount; // 错误计数器
static volatile u32 TickCount; // 定时器中断计数器

// TTC定时器中断处理函数
void TTC_TimerIntrHandler(void *CallBackRef, u32 StatusEvent)
{
    XTtcPs *TtcInstancePtr = (XTtcPs *)CallBackRef;

    if (0 != (XTTCPS_IXR_INTERVAL_MASK & StatusEvent))
    {
        TickCount++;
        // 每当TickCount达到TTC0_0_FREQUENCY_HZ时，说明已经发生了1秒钟的定时器中断
        if (TickCount == TTC0_0_FREQUENCY_HZ)
        {
            printf("TTC Timer interrupt occurred! TickCount: %u\n", TickCount);
            TickCount = 0; // 重置计数器
        }
    }
    else
    {
        ErrorCount++;
    }
}

// TTC定时器初始化函数，用TTC0_0定时器为例
int TTC_TimerInit()
{
    int Status;
    XTtcPs_Config *TtcConfig;

    // 获取TTC定时器配置
    TtcConfig = XTtcPs_LookupConfig(TTC0_0_DEVICE_ID);
    assert(TtcConfig != NULL);

    // 初始化TTC定时器实例
    Status = XTtcPs_CfgInitialize(&TtcInstance[0], TtcConfig, TtcConfig->BaseAddress);
    assert(Status == XST_SUCCESS);

    // 设置TTC定时器选项，间隔模式，禁止波形输出
    XTtcPs_SetOptions(&TtcInstance[0], XTTCPS_OPTION_INTERVAL_MODE | XTTCPS_OPTION_WAVE_DISABLE);

    // 根据所需频率计算预分频器和间隔值 (例如设置为100KHz)
    u8 u16Prescaler;
    XInterval Interval;
    XTtcPs_CalcIntervalFromFreq(&TtcInstance[0], TTC0_0_FREQUENCY_HZ, &Interval, &u16Prescaler);

    // 配置TTC定时器参数，例如频率、模式等
    XTtcPs_SetPrescaler(&TtcInstance[0], u16Prescaler); // 设置预分频器
    XTtcPs_SetInterval(&TtcInstance[0], Interval);      // 设置定时器间隔

    // 连接TTC定时器中断处理函数
    Status = XScuGic_Connect(&InterruptController, TTC0_0_IRPT_INTR, (Xil_ExceptionHandler)XTtcPs_InterruptHandler, &TtcInstance[0]);
    assert(Status == XST_SUCCESS);
    XTtcPs_SetStatusHandler(&TtcInstance[0], &TtcInstance[0], (XTtcPs_StatusHandler)TTC_TimerIntrHandler);

    // 使能TTC定时器中断
    XScuGic_Enable(&InterruptController, TTC0_0_IRPT_INTR);
    XTtcPs_EnableInterrupts(&TtcInstance[0], XTTCPS_IXR_INTERVAL_MASK);

    // 启动TTC定时器
    XTtcPs_Start(&TtcInstance[0]);

    return XST_SUCCESS;
}