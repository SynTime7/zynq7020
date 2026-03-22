# 
# Usage: To re-create this platform project launch xsct with below options.
# xsct D:\workspace\workspace_zynq\zynq7020\PS_DEV\mio_intr\vitis\zynq7020_wrapper\platform.tcl
# 
# OR launch xsct and run below command.
# source D:\workspace\workspace_zynq\zynq7020\PS_DEV\mio_intr\vitis\zynq7020_wrapper\platform.tcl
# 
# To create the platform in a different location, modify the -out option of "platform create" command.
# -out option specifies the output directory of the platform project.

platform create -name {zynq7020_wrapper}\
-hw {D:\workspace\workspace_zynq\zynq7020\PS_DEV\mio_intr\zynq7020_wrapper.xsa}\
-out {D:/workspace/workspace_zynq/zynq7020/PS_DEV/mio_intr/vitis}

platform write
domain create -name {standalone_ps7_cortexa9_0} -display-name {standalone_ps7_cortexa9_0} -os {standalone} -proc {ps7_cortexa9_0} -runtime {cpp} -arch {32-bit} -support-app {hello_world}
platform generate -domains 
platform active {zynq7020_wrapper}
domain active {zynq_fsbl}
domain active {standalone_ps7_cortexa9_0}
platform generate -quick
platform generate
platform generate -domains standalone_ps7_cortexa9_0 
platform active {zynq7020_wrapper}
platform config -updatehw {D:/workspace/workspace_zynq/zynq7020/PS_DEV/mio_intr/zynq7020_wrapper.xsa}
platform generate -domains 
platform generate -domains standalone_ps7_cortexa9_0,zynq_fsbl 
platform active {zynq7020_wrapper}
platform config -updatehw {D:/workspace/workspace_zynq/zynq7020/PS_DEV/mio_intr/zynq7020_wrapper.xsa}
platform generate -domains 
platform config -updatehw {D:/workspace/workspace_zynq/zynq7020/PS_DEV/mio_intr/zynq7020_wrapper.xsa}
platform clean
domain active {zynq_fsbl}
bsp reload
domain active {standalone_ps7_cortexa9_0}
bsp reload
bsp reload
platform generate
bsp reload
domain active {zynq_fsbl}
bsp reload
domain active {standalone_ps7_cortexa9_0}
bsp reload
platform generate -domains 
bsp reload
