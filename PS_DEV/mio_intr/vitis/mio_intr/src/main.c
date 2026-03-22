#include "main.h"

#define INTC_DEVICE_ID XPAR_SCUGIC_SINGLE_DEVICE_ID // 中断控制器设备ID

// 中断控制器实例
XScuGic InterruptController;

// 初始化中断控制器
int init_interrupt_controller()
{
    int Status;
    XScuGic_Config *IntcConfig;

    // 获取中断控制器配置
    IntcConfig = XScuGic_LookupConfig(INTC_DEVICE_ID);
    assert(IntcConfig != NULL);

    // 初始化中断控制器
    Status = XScuGic_CfgInitialize(&InterruptController, IntcConfig, IntcConfig->CpuBaseAddress);
    assert(Status == XST_SUCCESS);

    // 中断异常使能
    Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT, (Xil_ExceptionHandler)XScuGic_InterruptHandler, &InterruptController);
    Xil_ExceptionEnable();

    return XST_SUCCESS;
}

int main()
{
    init_platform();

    // 初始化中断控制器
    int Status = init_interrupt_controller();
    assert(Status == XST_SUCCESS);

    // 初始化GPIO
    Status = gpio_init();
    assert(Status == XST_SUCCESS);

    // 初始化AXI GPIO
    Status = axi_gpio_init();
    assert(Status == XST_SUCCESS);

    int flag = 0;
    while (1)
    {
        flag = !flag;

        set_ps_led(PS_LED1, flag);  // 点亮PS_LED1
        set_ps_led(PS_LED2, !flag); // 点亮PS_LED2
        set_pl_led(PL_LED1, flag);  // 点亮PL_LED1

        sleep(2); // 模拟主循环，实际应用中可以执行其他任务
    }

    cleanup_platform();
    return 0;
}
