set part xcku115-flvd1517-2-i
set clock 300MHz

open_project -reset HLS_Histogrammer
open_solution -reset KU115
#setting target device
set_part $part
create_clock -period $clock
#adding hls files
add_files -cflags "-std=c++11" "HistogramAndBuffer/HLS_Histogrammer.cpp"
add_files -cflags "-std=c++11" "HistogramAndBuffer/RegionOffsets.cpp"
# adding testbench files
add_files -cflags "-std=c++11" -tb "HistogramAndBuffer/TB_Histogrammer.cpp"
# Sets the top-level function name that is going to be starting point for synthetisation
set_top hls_histogrammer
# removing reset
config_rtl -reset none
#synthetising
csynth_design
#exporting IP
export_design -evaluate vhdl -flow impl -rtl vhdl -format ip_catalog
