# Usage with Vitis IDE:
# In Vitis IDE create a Single Application Debug launch configuration,
# change the debug type to 'Attach to running target' and provide this 
# tcl script in 'Execute Script' option.
# Path of this script: D:\workspace\workspace_zynq\zynq7020\PS_DEV\hello_world\workspace_vitis\hello_world_system\_ide\scripts\systemdebugger_hello_world_system_standalone.tcl
# 
# 
# Usage with xsct:
# To debug using xsct, launch xsct and run below command
# source D:\workspace\workspace_zynq\zynq7020\PS_DEV\hello_world\workspace_vitis\hello_world_system\_ide\scripts\systemdebugger_hello_world_system_standalone.tcl
# 
connect -url tcp:127.0.0.1:3121
targets -set -nocase -filter {name =~"APU*"}
rst -system
after 3000
targets -set -nocase -filter {name =~"APU*"}
loadhw -hw D:/workspace/workspace_zynq/zynq7020/PS_DEV/hello_world/workspace_vitis/zynq7_wrapper/export/zynq7_wrapper/hw/zynq7_wrapper.xsa -mem-ranges [list {0x40000000 0xbfffffff}] -regs
configparams force-mem-access 1
targets -set -nocase -filter {name =~"APU*"}
source D:/workspace/workspace_zynq/zynq7020/PS_DEV/hello_world/workspace_vitis/hello_world/_ide/psinit/ps7_init.tcl
ps7_init
ps7_post_config
targets -set -nocase -filter {name =~ "*A9*#0"}
dow D:/workspace/workspace_zynq/zynq7020/PS_DEV/hello_world/workspace_vitis/hello_world/Debug/hello_world.elf
configparams force-mem-access 0
bpadd -addr &main
