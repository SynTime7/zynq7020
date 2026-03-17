# Usage with Vitis IDE:
# In Vitis IDE create a Single Application Debug launch configuration,
# change the debug type to 'Attach to running target' and provide this 
# tcl script in 'Execute Script' option.
# Path of this script: D:\workspace\workspace_zynq\zynq7020\PS_DEV\zynq_ps_code_gpio\gpio_mio\workspace_vitis\gpio_mio_system\_ide\scripts\systemdebugger_gpio_mio_system_standalone.tcl
# 
# 
# Usage with xsct:
# To debug using xsct, launch xsct and run below command
# source D:\workspace\workspace_zynq\zynq7020\PS_DEV\zynq_ps_code_gpio\gpio_mio\workspace_vitis\gpio_mio_system\_ide\scripts\systemdebugger_gpio_mio_system_standalone.tcl
# 
connect -url tcp:127.0.0.1:3121
targets -set -nocase -filter {name =~"APU*"}
rst -system
after 3000
targets -set -nocase -filter {name =~"APU*"}
loadhw -hw D:/workspace/workspace_zynq/zynq7020/PS_DEV/zynq_ps_code_gpio/gpio_mio/workspace_vitis/zynq7_wrapper/export/zynq7_wrapper/hw/zynq7_wrapper.xsa -mem-ranges [list {0x40000000 0xbfffffff}] -regs
configparams force-mem-access 1
targets -set -nocase -filter {name =~"APU*"}
targets -set -nocase -filter {name =~ "*A9*#0"}
rst -processor
dow D:/workspace/workspace_zynq/zynq7020/PS_DEV/zynq_ps_code_gpio/gpio_mio/workspace_vitis/zynq7_wrapper/export/zynq7_wrapper/sw/zynq7_wrapper/boot/fsbl.elf
set bp_59_49_fsbl_bp [bpadd -addr &FsblHandoffJtagExit]
con -block -timeout 60
bpremove $bp_59_49_fsbl_bp
targets -set -nocase -filter {name =~ "*A9*#0"}
dow D:/workspace/workspace_zynq/zynq7020/PS_DEV/zynq_ps_code_gpio/gpio_mio/workspace_vitis/gpio_mio/Debug/gpio_mio.elf
configparams force-mem-access 0
bpadd -addr &main
