# setup: opens a project and a solution
#


# creating project and solution
open_project -reset HLS_HistogramBuffer
open_solution -reset KU115

#setting target device
set_part xcku115-flva1517-1-c
create_clock -period 240MHz

#adding hls files
add_files -cflags "-std=c++11" "HLS_HistogramBuffer.cpp"

# adding testbench files
add_files -tb "TB_HistogramBuffer.cpp"

# Sets the top-level function name that is going to be starting point for synthetisation
set_top hls_histogram_buffer