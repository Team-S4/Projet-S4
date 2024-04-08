@echo off
REM ****************************************************************************
REM Vivado (TM) v2020.2 (64-bit)
REM
REM Filename    : compile.bat
REM Simulator   : Xilinx Vivado Simulator
REM Description : Script for compiling the simulation design source files
REM
REM Generated by Vivado on Thu Mar 28 08:40:49 -0400 2024
REM SW Build 3064766 on Wed Nov 18 09:12:45 MST 2020
REM
REM Copyright 1986-2020 Xilinx, Inc. All Rights Reserved.
REM
REM usage: compile.bat
REM
REM ****************************************************************************
REM compile Verilog/System Verilog design sources
echo "xvlog --incr --relax -L i2s_receiver_v1_0_4 -L i2s_transmitter_v1_0_4 -L axi_vip_v1_1_8 -L processing_system7_vip_v1_0_10 -L xilinx_vip -prj simul_mono_vlog.prj"
call xvlog  --incr --relax -L i2s_receiver_v1_0_4 -L i2s_transmitter_v1_0_4 -L axi_vip_v1_1_8 -L processing_system7_vip_v1_0_10 -L xilinx_vip -prj simul_mono_vlog.prj -log xvlog.log
call type xvlog.log > compile.log
REM compile VHDL design sources
echo "xvhdl --incr --relax -prj simul_mono_vhdl.prj"
call xvhdl  --incr --relax -prj simul_mono_vhdl.prj -log xvhdl.log
call type xvhdl.log >> compile.log
if "%errorlevel%"=="1" goto END
if "%errorlevel%"=="0" goto SUCCESS
:END
exit 1
:SUCCESS
exit 0
