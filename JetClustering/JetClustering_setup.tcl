# creating project and solution
open_project JetClusteringMHT
open_solution VU9P

#setting target device
set_part xcvu9p-flga2104-1-i
create_clock -period 240MHz

#adding hls files
add_files -cflags "-std=c++11" "HLS_Phase1Clustering.cpp"

# adding testbench files
add_files -cflags "-std=c++11" -tb "TB_Phase1Clustering.cpp"

# removing reset
config_rtl -reset none

# Sets the top-level function name that is going to be starting point for synthetisation
set_top hls_jet_clustering