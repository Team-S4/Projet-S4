# Usage with Vitis IDE:
# In Vitis IDE create a Single Application Debug launch configuration,
# change the debug type to 'Attach to running target' and provide this 
# tcl script in 'Execute Script' option.
# Path of this script: C:\Users\cbowlieu\github\Projet-S4\ZYBO\ZYBO_PROJET_S4\VITIS_WORKSPACE\fsbl_system\_ide\scripts\debugger_fsbl-default.tcl
# 
# 
# Usage with xsct:
# To debug using xsct, launch xsct and run below command
# source C:\Users\cbowlieu\github\Projet-S4\ZYBO\ZYBO_PROJET_S4\VITIS_WORKSPACE\fsbl_system\_ide\scripts\debugger_fsbl-default.tcl
# 
connect -url tcp:127.0.0.1:3121
targets -set -nocase -filter {name =~"APU*"}
rst -system
after 3000
targets -set -filter {jtag_cable_name =~ "Digilent Zybo Z7 210351A77C98A" && level==0 && jtag_device_ctx=="jsn-Zybo Z7-210351A77C98A-13722093-0"}
fpga -file C:/Users/cbowlieu/github/Projet-S4/ZYBO/ZYBO_PROJET_S4/VITIS_WORKSPACE/fsbl/_ide/bitstream/design_1_wrapper.bit
targets -set -nocase -filter {name =~"APU*"}
loadhw -hw C:/Users/cbowlieu/github/Projet-S4/ZYBO/ZYBO_PROJET_S4/VITIS_WORKSPACE/design_1_wrapper/export/design_1_wrapper/hw/design_1_wrapper.xsa -mem-ranges [list {0x40000000 0xbfffffff}] -regs
configparams force-mem-access 1
targets -set -nocase -filter {name =~"APU*"}
source C:/Users/cbowlieu/github/Projet-S4/ZYBO/ZYBO_PROJET_S4/VITIS_WORKSPACE/fsbl/_ide/psinit/ps7_init.tcl
ps7_init
ps7_post_config
configparams mdm-detect-bscan-mask 2
targets -set -nocase -filter {name =~ "*A9*#0"}
dow C:/Users/cbowlieu/github/Projet-S4/ZYBO/ZYBO_PROJET_S4/VITIS_WORKSPACE/fsbl/Debug/fsbl.elf
configparams force-mem-access 0
bpadd -addr &main
