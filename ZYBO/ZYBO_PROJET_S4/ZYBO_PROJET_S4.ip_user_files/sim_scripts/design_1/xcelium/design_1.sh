#!/bin/bash -f
#*********************************************************************************************************
# Vivado (TM) v2020.2 (64-bit)
#
# Filename    : design_1.sh
# Simulator   : Cadence Xcelium Parallel Simulator
# Description : Simulation script for compiling, elaborating and verifying the project source files.
#               The script will automatically create the design libraries sub-directories in the run
#               directory, add the library logical mappings in the simulator setup file, create default
#               'do/prj' file, execute compilation, elaboration and simulation steps.
#
# Generated by Vivado on Wed Apr 10 20:46:52 -0400 2024
# SW Build 3064766 on Wed Nov 18 09:12:45 MST 2020
#
# Copyright 1986-2020 Xilinx, Inc. All Rights Reserved. 
#
# usage: design_1.sh [-help]
# usage: design_1.sh [-lib_map_path]
# usage: design_1.sh [-noclean_files]
# usage: design_1.sh [-reset_run]
#
# Prerequisite:- To compile and run simulation, you must compile the Xilinx simulation libraries using the
# 'compile_simlib' TCL command. For more information about this command, run 'compile_simlib -help' in the
# Vivado Tcl Shell. Once the libraries have been compiled successfully, specify the -lib_map_path switch
# that points to these libraries and rerun export_simulation. For more information about this switch please
# type 'export_simulation -help' in the Tcl shell.
#
# You can also point to the simulation libraries by either replacing the <SPECIFY_COMPILED_LIB_PATH> in this
# script with the compiled library directory path or specify this path with the '-lib_map_path' switch when
# executing this script. Please type 'design_1.sh -help' for more information.
#
# Additional references - 'Xilinx Vivado Design Suite User Guide:Logic simulation (UG900)'
#
#*********************************************************************************************************

# Directory path for design sources and include directories (if any) wrt this path
ref_dir="."

# Override directory with 'export_sim_ref_dir' env path value if set in the shell
if [[ (! -z "$export_sim_ref_dir") && ($export_sim_ref_dir != "") ]]; then
  ref_dir="$export_sim_ref_dir"
fi

# Set the compiled library directory
ref_lib_dir="."

# Command line options
xrun_opts="-64bit -v93 -relax -access +rwc -namemap_mixgen"

# Design libraries
design_libs=(xilinx_vip xpm microblaze_v11_0_4 xil_defaultlib generic_baseblocks_v2_1_0 axi_infrastructure_v1_1_0 axi_register_slice_v2_1_22 fifo_generator_v13_2_5 axi_data_fifo_v2_1_21 axi_crossbar_v2_1_23 axi_lite_ipif_v3_0_4 axi_intc_v4_1_15 xlconcat_v2_1_4 mdm_v3_2_19 lib_cdc_v1_0_2 proc_sys_reset_v5_0_13 i2s_receiver_v1_0_4 xlconstant_v1_1_7 lib_pkg_v1_0_2 interrupt_control_v3_1_4 axi_iic_v2_0_25 axi_vip_v1_1_8 processing_system7_vip_v1_0_10 xbip_utils_v3_0_10 axi_utils_v2_0_6 c_reg_fd_v12_0_6 xbip_dsp48_wrapper_v3_0_4 xbip_pipe_v3_0_6 xbip_dsp48_addsub_v3_0_6 xbip_addsub_v3_0_6 c_addsub_v12_0_14 c_mux_bit_v12_0_6 c_shift_ram_v12_0_14 xbip_bram18k_v3_0_6 mult_gen_v12_0_16 cmpy_v6_0_19 floating_point_v7_0_18 xfft_v9_1_5 lib_fifo_v1_0_14 axi_fifo_mm_s_v4_2_4 i2s_transmitter_v1_0_4 axis_infrastructure_v1_1_0 axis_broadcaster_v1_1_21 dist_mem_gen_v8_0_13 lib_srl_fifo_v1_0_2 axi_quad_spi_v3_2_21 axi_protocol_converter_v2_1_22 axi_clock_converter_v2_1_21 blk_mem_gen_v8_4_4 axi_dwidth_converter_v2_1_22)

# Simulation root library directory
sim_lib_dir="xcelium_lib"

# Script info
echo -e "design_1.sh - Script generated by export_simulation (Vivado v2020.2 (64-bit)-id)\n"

# Main steps
run()
{
  check_args $# $1
  setup $1 $2
  execute
}

# RUN_STEP: <execute>
execute()
{
  xrun $xrun_opts \
       -reflib "$ref_lib_dir/unisim:unisim" \
       -reflib "$ref_lib_dir/unisims_ver:unisims_ver" \
       -reflib "$ref_lib_dir/secureip:secureip" \
       -reflib "$ref_lib_dir/unimacro:unimacro" \
       -reflib "$ref_lib_dir/unimacro_ver:unimacro_ver" \
       -top xil_defaultlib.design_1 \
       -f run.f \
       -top glbl \
       glbl.v \
       +incdir+"$ref_dir/../../../../ZYBO_PROJET_S4.gen/sources_1/bd/design_1/ipshared/ec67/hdl" \
       +incdir+"$ref_dir/../../../../ZYBO_PROJET_S4.gen/sources_1/bd/design_1/ipshared/d0f7" \
       +incdir+"$ref_dir/../../../../ZYBO_PROJET_S4.gen/sources_1/bd/design_1/ipshared/34f8/hdl" \
       +incdir+"$ref_dir/../../../../ZYBO_PROJET_S4.gen/sources_1/bd/design_1/ipshared/8713/hdl" \
       +incdir+"../../../../ZYBO_PROJET_S4.gen/sources_1/bd/design_1/ipshared/ec67/hdl" \
       +incdir+"../../../../ZYBO_PROJET_S4.gen/sources_1/bd/design_1/ipshared/d0f7" \
       +incdir+"../../../../ZYBO_PROJET_S4.gen/sources_1/bd/design_1/ipshared/34f8/hdl" \
       +incdir+"../../../../ZYBO_PROJET_S4.gen/sources_1/bd/design_1/ipshared/8713/hdl" \
       +incdir+"../../../../ZYBO_PROJET_S4.gen/sources_1/bd/design_1/ipshared/1b7e/hdl/verilog" \
       +incdir+"../../../../ZYBO_PROJET_S4.gen/sources_1/bd/design_1/ipshared/122e/hdl/verilog" \
       +incdir+"../../../../ZYBO_PROJET_S4.gen/sources_1/bd/design_1/ipshared/b205/hdl/verilog" \
       +incdir+"../../../../ZYBO_PROJET_S4.gen/sources_1/bd/design_1/ipshared/c968/hdl/verilog" \
       +incdir+"C:/Xilinx/Vivado/2020.2/data/xilinx_vip/include"
}

# STEP: setup
setup()
{
  case $1 in
    "-lib_map_path" )
      if [[ ($2 == "") ]]; then
        echo -e "ERROR: Simulation library directory path not specified (type \"./design_1.sh -help\" for more information)\n"
        exit 1
      else
        ref_lib_dir=$2
      fi
    ;;
    "-reset_run" )
      reset_run
      echo -e "INFO: Simulation run files deleted.\n"
      exit 0
    ;;
    "-noclean_files" )
      # do not remove previous data
    ;;
    * )
  esac

  create_lib_dir

  # Add any setup/initialization commands here:-

  # <user specific commands>

}

# Create design library directory paths
create_lib_dir()
{
  if [[ -e $sim_lib_dir ]]; then
    rm -rf $sim_lib_dir
  fi

  for (( i=0; i<${#design_libs[*]}; i++ )); do
    lib="${design_libs[i]}"
    lib_dir="$sim_lib_dir/$lib"
    if [[ ! -e $lib_dir ]]; then
      mkdir -p $lib_dir
    fi
  done
}

# Delete generated data from the previous run
reset_run()
{
  files_to_remove=(xmsim.key xrun.key xrun.log waves.shm xrun.history .simvision xcelium.d xcelium)
  for (( i=0; i<${#files_to_remove[*]}; i++ )); do
    file="${files_to_remove[i]}"
    if [[ -e $file ]]; then
      rm -rf $file
    fi
  done

  create_lib_dir
}

# Check command line arguments
check_args()
{
  if [[ ($1 == 1 ) && ($2 != "-lib_map_path" && $2 != "-noclean_files" && $2 != "-reset_run" && $2 != "-help" && $2 != "-h") ]]; then
    echo -e "ERROR: Unknown option specified '$2' (type \"./design_1.sh -help\" for more information)\n"
    exit 1
  fi

  if [[ ($2 == "-help" || $2 == "-h") ]]; then
    usage
  fi
}

# Script usage
usage()
{
  msg="Usage: design_1.sh [-help]\n\
Usage: design_1.sh [-lib_map_path]\n\
Usage: design_1.sh [-reset_run]\n\
Usage: design_1.sh [-noclean_files]\n\n\
[-help] -- Print help information for this script\n\n\
[-lib_map_path <path>] -- Compiled simulation library directory path. The simulation library is compiled\n\
using the compile_simlib tcl command. Please see 'compile_simlib -help' for more information.\n\n\
[-reset_run] -- Recreate simulator setup files and library mappings for a clean run. The generated files\n\
from the previous run will be removed. If you don't want to remove the simulator generated files, use the\n\
-noclean_files switch.\n\n\
[-noclean_files] -- Reset previous run, but do not remove simulator generated files from the previous run.\n\n"
  echo -e $msg
  exit 1
}

# Launch script
run $1 $2
