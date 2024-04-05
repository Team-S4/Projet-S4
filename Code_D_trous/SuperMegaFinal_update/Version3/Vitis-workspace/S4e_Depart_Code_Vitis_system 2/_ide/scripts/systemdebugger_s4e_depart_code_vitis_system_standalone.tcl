# Usage with Vitis IDE:
# In Vitis IDE create a Single Application Debug launch configuration,
# change the debug type to 'Attach to running target' and provide this 
# tcl script in 'Execute Script' option.
# Path of this script: C:\Users\cbowlieu\APP4\Code_D_trous\SuperMegaFinal_update\Version3\Vitis-workspace\S4e_Depart_Code_Vitis_system\_ide\scripts\systemdebugger_s4e_depart_code_vitis_system_standalone.tcl
# 
# 
# Usage with xsct:
# To debug using xsct, launch xsct and run below command
# source C:\Users\cbowlieu\APP4\Code_D_trous\SuperMegaFinal_update\Version3\Vitis-workspace\S4e_Depart_Code_Vitis_system\_ide\scripts\systemdebugger_s4e_depart_code_vitis_system_standalone.tcl
# 
connect -url tcp:127.0.0.1:3121
targets -set -nocase -filter {name =~"APU*"}
rst -system
after 3000
targets -set -filter {jtag_cable_name =~ "Digilent Zybo Z7 210351B3FF7DA" && level==0 && jtag_device_ctx=="jsn-Zybo Z7-210351B3FF7DA-13722093-0"}
fpga -file C:/Users/cbowlieu/APP4/Code_D_trous/SuperMegaFinal_update/Version3/Vitis-workspace/S4e_Depart_Code_Vitis/_ide/bitstream/main_design_wrapper.bit
targets -set -nocase -filter {name =~"APU*"}
loadhw -hw C:/Users/cbowlieu/APP4/Code_D_trous/SuperMegaFinal_update/Version3/Vitis-workspace/main_design_wrapper/export/main_design_wrapper/hw/main_design_wrapper.xsa -mem-ranges [list {0x40000000 0xbfffffff}] -regs
configparams force-mem-access 1
targets -set -nocase -filter {name =~"APU*"}
source C:/Users/cbowlieu/APP4/Code_D_trous/SuperMegaFinal_update/Version3/Vitis-workspace/S4e_Depart_Code_Vitis/_ide/psinit/ps7_init.tcl
ps7_init
ps7_post_config
configparams mdm-detect-bscan-mask 2
targets -set -nocase -filter {name =~ "*microblaze*#0" && bscan=="USER2" }
dow C:/Users/cbowlieu/APP4/Code_D_trous/SuperMegaFinal_update/Version3/Vitis-workspace/S4e_Depart_Code_Vitis/Debug/S4e_Depart_Code_Vitis.elf
configparams force-mem-access 0
targets -set -nocase -filter {name =~ "*microblaze*#0" && bscan=="USER2" }
con
