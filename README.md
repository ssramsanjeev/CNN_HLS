# CNN_Accelerator_HLS# CNN Accelerator in HLS

A hardware implementation of a Convolutional Neural Network (CNN) using Vitis HLS targeting the Xilinx Spartan-7 FPGA.

##  Highlights
- Written in C++ for HLS, synthesized to Verilog.
- Optimized from **~2000 BRAMs → 38 BRAMs**.
- Target device: XA7S75-FGGA676-2I.
- Convolution, tiling, and memory-efficient design.

##  Synthesis Report (Vitis HLS 2025.1)
- Timing Estimate: 7.29 ns  
- Resource Utilization: BRAM: 38 | DSP: 37 | FF: 5818 | LUT: 16291
To implement a cnn in hardware using verilog by using hls

##  Next Steps
Paper under preparation – expanding accelerator to multi-layer CNN.

##  Credit
Created by **Sanjeev Ramkumar**  
License: MIT 
