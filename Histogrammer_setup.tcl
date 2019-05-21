# setup: opens a project and a solution
#


# creating project and solution
open_project HLS_Histogrammer
open_solution -reset KU115

#setting target device
set_part xcku115-flva1517-1-c
create_clock -period 240MHz

#adding hls files
add_files "HLS_Histogrammer.cpp"
add_files "HLS/HistogramEtaPhi.cpp"

# adding testbench files
add_files -tb "TB_Histogrammer.cpp"

# Sets the top-level function name that is going to be starting point for synthetisation
set_top hls_histogrammer