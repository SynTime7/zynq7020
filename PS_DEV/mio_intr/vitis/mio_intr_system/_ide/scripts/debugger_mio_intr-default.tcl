# Usage with Vitis IDE:
# In Vitis IDE create a Single Application Debug launch configuration,
# change the debug type to 'Attach to running target' and provide this 
# tcl script in 'Execute Script' option.
# Path of this script: D:\workspace\workspace_zynq\zynq7020\PS_DEV\mio_intr\vitis\mio_intr_system\_ide\scripts\debugger_mio_intr-default.tcl
# 
# 
# Usage with xsct:
# To debug using xsct, launch xsct and run below command
# source D:\workspace\workspace_zynq\zynq7020\PS_DEV\mio_intr\vitis\mio_intr_system\_ide\scripts\debugger_mio_intr-default.tcl
# 
connect -url tcp:127.0.0.1:3121
targets -set -nocase -filter {name =~"APU*"}
rst -system
after 3000
targets -set -filter {jtag_cable_name =~ "Digilent JTAG-SMT3 210357A7D00EA" && level==0 && jtag_device_ctx=="jsn-JTAG-SMT3-210357A7D00EA-23727093-0"}
fpga -file D:/workspace/workspace_zynq/zynq7020/PS_DEV/mio_intr/vitis/mio_intr/_ide/bitstream/zynq7020_wrapper.bit
targets -set -nocase -filter {name =~"APU*"}
loadhw -hw D:/workspace/workspace_zynq/zynq7020/PS_DEV/mio_intr/vitis/zynq7020_wrapper/export/zynq7020_wrapper/hw/zynq7020_wrapper.xsa -mem-ranges [list {0x40000000 0xbfffffff}] -regs
configparams force-mem-access 1
targets -set -nocase -filter {name =~"APU*"}
source D:/workspace/workspace_zynq/zynq7020/PS_DEV/mio_intr/vitis/mio_intr/_ide/psinit/ps7_init.tcl
ps7_init
ps7_post_config
targets -set -nocase -filter {name =~ "*A9*#0"}
dow D:/workspace/workspace_zynq/zynq7020/PS_DEV/mio_intr/vitis/mio_intr/Debug/mio_intr.elf
configparams force-mem-access 0
bpadd -addr &main
