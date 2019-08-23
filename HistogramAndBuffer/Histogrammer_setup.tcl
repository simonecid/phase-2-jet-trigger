# setup: opens a project and a solution
#


# creating project and solution
open_project HLS_Histogrammer
open_solution KU115

#setting target device
set_part xcku115-flvd1517-2-i
create_clock -period 300MHz

#adding hls files
add_files -cflags "-std=c++11" "HLS_Histogrammer.cpp"

# adding testbench files
add_files -cflags "-std=c++11" -tb "TB_Histogrammer.cpp"

# removing reset
config_rtl -reset none

# Sets the top-level function name that is going to be starting point for synthetisation
set_top hls_histogrammer