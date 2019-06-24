# creating project and solution
open_project -reset JetClustering
open_solution -reset KU115

#setting target device
set_part xcku115-flvd1517-2-i
create_clock -period 240MHz

#adding hls files
add_files -cflags "-std=c++11" "HLS_Phase1Clustering.cpp"

# adding testbench files
add_files -cflags "-std=c++11" -tb "TB_Phase1Clustering.cpp"

# Sets the top-level function name that is going to be starting point for synthetisation
set_top hls_jet_clustering