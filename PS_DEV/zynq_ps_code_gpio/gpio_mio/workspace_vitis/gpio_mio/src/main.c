#include <stdio.h>
#include "xil_printf.h"
#include "xparameters.h"
#include "xgpiops.h"
#include "sleep.h"

#define PS_LED1 0 // ps_led1 固定为 mio0
#define PS_LED2 9 // ps_led2 固定为 mio9

int main(void)
{
    XGpioPs Gpio;
    XGpioPs_Config *Config;

    // 初始化 GPIO
    Config = XGpioPs_LookupConfig(XPAR_XGPIOPS_0_DEVICE_ID);
    if (Config == NULL) {
        xil_printf("GPIO 配置查找失败\r\n");
        return -1;
    }

    int Status = XGpioPs_CfgInitialize(&Gpio, Config, Config->BaseAddr);
    if (Status != XST_SUCCESS) {
        xil_printf("GPIO 初始化失败\r\n");
        return -1;
    }

    // 设置 LED 引脚为输出
    XGpioPs_SetDirectionPin(&Gpio, PS_LED1, 1); // 设置 ps_led1 为输出
    XGpioPs_SetDirectionPin(&Gpio, PS_LED2, 1); // 设置 ps_led2 为输出

    // 使能 LED 引脚的输出功能
    XGpioPs_SetOutputEnablePin(&Gpio, PS_LED1, 1); // 使能 ps_led1 输出
    XGpioPs_SetOutputEnablePin(&Gpio, PS_LED2, 1); // 使能 ps_led2 输出 

    while (1) {
        xil_printf("点亮 LED\r\n");

        // 点亮 LED
        XGpioPs_WritePin(&Gpio, PS_LED1, 1); // 点亮 ps_led1
        XGpioPs_WritePin(&Gpio, PS_LED2, 1); // 点亮 ps_led2
        sleep(1); // 延时 1 秒

        xil_printf("熄灭 LED\r\n");
        // 熄灭 LED
        XGpioPs_WritePin(&Gpio, PS_LED1, 0); // 熄灭 ps_led1
        XGpioPs_WritePin(&Gpio, PS_LED2, 0); // 熄灭 ps_led2
        sleep(1); // 延时 1 秒
    }

    return 0;
}
