# 
# Usage: To re-create this platform project launch xsct with below options.
# xsct D:\workspace\workspace_zynq\zynq7020\PS_DEV\zynq_ps_code_gpio\gpio_mio\workspace_vitis\zynq7_wrapper\platform.tcl
# 
# OR launch xsct and run below command.
# source D:\workspace\workspace_zynq\zynq7020\PS_DEV\zynq_ps_code_gpio\gpio_mio\workspace_vitis\zynq7_wrapper\platform.tcl
# 
# To create the platform in a different location, modify the -out option of "platform create" command.
# -out option specifies the output directory of the platform project.

platform create -name {zynq7_wrapper}\
-hw {D:\workspace\workspace_zynq\zynq7020\PS_DEV\zynq_ps_code_gpio\gpio_mio\zynq7_wrapper.xsa}\
-out {D:/workspace/workspace_zynq/zynq7020/PS_DEV/zynq_ps_code_gpio/gpio_mio/workspace_vitis}

platform write
domain create -name {standalone_ps7_cortexa9_0} -display-name {standalone_ps7_cortexa9_0} -os {standalone} -proc {ps7_cortexa9_0} -runtime {cpp} -arch {32-bit} -support-app {empty_application}
platform generate -domains 
platform active {zynq7_wrapper}
domain active {zynq_fsbl}
domain active {standalone_ps7_cortexa9_0}
platform generate -quick
platform generate
