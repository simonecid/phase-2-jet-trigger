# setup: opens a project and a solution
#


# creating project and solution
open_project -reset HLS_HistogramBuffer
open_solution -reset KU115

#setting target device
set_part xcku115-flvd1517-2-i
create_clock -period 300MHz

#adding hls files
add_files -cflags "-std=c++11" "HLS_HistogramBuffer.cpp" 
add_files -cflags "-std=c++11" "RegionOffsets.cpp"

# adding testbench files
add_files -tb "TB_HistogramBuffer.cpp"

# removing reset
config_rtl -reset none

# Sets the top-level function name that is going to be starting point for synthetisation
set_top hls_histogram_buffer