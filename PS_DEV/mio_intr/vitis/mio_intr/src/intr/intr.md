# Zynq中断系统详细说明

## 概述
Zynq-7000系列SoC的中断系统基于ARM Generic Interrupt Controller (GIC) v1.0，支持多达160个中断源。中断用于处理异步事件，如外设请求、定时器到期或FPGA逻辑触发。以下详细介绍中断的分类、中断号划分、配置方法及示例。

## 中断分类
Zynq的中断分为以下三大类：

### 1. 软件生成中断 (Software Generated Interrupts, SGI)
   - **描述**：由软件触发的中断，用于CPU核心间的通信或自我中断。
   - **特点**：可通过软件写入GIC的SGI寄存器触发；支持广播到所有CPU或特定CPU。
   - **用途**：多核同步、任务调度。

### 2. 私有外设中断 (Private Peripheral Interrupts, PPI)
   - **描述**：每个CPU核心私有的中断，由处理器子系统生成。
   - **来源**：
     - CPU私有定时器 (Global Timer)
     - 看门狗定时器 (Watchdog Timer)
     - FIQ (Fast Interrupt Request)
     - 私有性能监控单元 (PMU)
   - **特点**：每个核心独立处理，不共享。

### 3. 共享外设中断 (Shared Peripheral Interrupts, SPI)
   - **描述**：所有CPU核心共享的中断，由PS外设或PL生成。
   - **来源**：
     - PS外设：UART、I2C、SPI、Ethernet、USB、SDIO、CAN、GPIO等。
     - PL中断：FPGA逻辑生成的信号，通过EMIO或AXI接口连接。
     - 系统级：DMA控制器、电源管理、时钟管理。
   - **特点**：可路由到任一CPU核心，支持负载均衡。

## 中断号划分
GIC支持256个中断ID (0-255)，但Zynq实际使用0-159。编号从低到高分配：

- **0-15**: SGI (软件生成中断)
  - 0-14: 用户定义的SGI
  - 15: 保留 (通常用于调试)

- **16-31**: PPI (私有外设中断)
  - 每个CPU核心有相同编号的PPI，但独立处理。
  - 示例：
    - 27: CPU私有定时器
    - 29: 看门狗定时器
    - 30: FIQ
    - 31: 私有PMU

- **32-159**: SPI (共享外设中断)
  - 32-63: PS外设中断 (如UART0: 50, I2C0: 51)
  - 64-95: 系统中断 (如DMA: 64-67)
  - 96-127: PL中断 (FPGA生成，0-15个信号映射到96-111)
  - 128-159: 扩展外设或保留

- **160-255**: 保留 (未使用)

中断号在设备树 (Linux) 或中断向量表 (裸机) 中配置。

## 中断优先级
Zynq的GIC支持32级可配置优先级（0-31，0为最高优先级）。优先级用于决定中断的处理顺序：
- 高优先级中断可抢占低优先级中断的执行。
- 支持嵌套中断：ISR中可被更高优先级中断打断。
- 配置方法：在裸机环境下，通过`XScuGic_SetPriorityTriggerType`函数设置；在Linux环境下，通过设备树中的`interrupt-priority`属性配置。
- 示例：设置中断ID 52的优先级为5（较高优先级）：`XScuGic_SetPriorityTriggerType(&Intc, 52, 5, 0x3);`（触发类型：上升沿）。

## 中断处理流程
1. **触发**：中断信号到达GIC，控制器检查优先级和屏蔽状态。
2. **分发**：GIC将中断路由到目标CPU核心。
3. **响应**：CPU保存上下文，执行ISR (Interrupt Service Routine)。
4. **清除**：ISR处理后，清除中断标志，恢复上下文。

## 配置方法
- **裸机环境**：
  - 使用Xilinx库：`xil_exception.h`、`xscugic.h`。
  - 初始化GIC：`XScuGic_Config *Config = XScuGic_LookupConfig(XPAR_SCUGIC_0_DEVICE_ID);`
  - 注册ISR：`XScuGic_Connect(IntcInstancePtr, InterruptId, (Xil_ExceptionHandler)Handler);`
  - 启用中断：`XScuGic_Enable(IntcInstancePtr, InterruptId);`

- **Linux环境**：
  - 通过设备树配置中断号和优先级。
  - 示例设备树节点：
    ```
    interrupt-parent = <&gic>;
    interrupts = <0 50 4>;  // 类型(0=SPI), 中断号, 触发类型
    ```

## 注意事项
- 中断优先级：0-31，0最高。避免高优先级中断阻塞系统。
- 延迟处理：ISR应简短，将复杂逻辑移到任务队列。
- 调试：使用Xilinx SDK/Vitis调试器检查中断状态。
- 性能：过多中断可能影响实时性，考虑中断合并或轮询。

此文档基于Zynq-7000技术参考手册 (UG585)。如需特定外设的中断号，请参考Xilinx文档或硬件设计。