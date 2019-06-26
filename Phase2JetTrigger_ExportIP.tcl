set part xcku115-flvd1517-2-i
set clock 240MHz

## STEP 1: Building and exporting to IP format the jet clustering

# creating project and solution
open_project -reset HLS_JetClustering
open_solution -reset KU115
#setting target device
set_part $part
create_clock -period $clock
#adding hls files
add_files -cflags "-std=c++11" "JetClustering/HLS_Phase1Clustering.cpp"
# adding testbench files
add_files -cflags "-std=c++11" -tb "JetClustering/TB_Phase1Clustering.cpp"
# Sets the top-level function name that is going to be starting point for synthetisation
set_top hls_jet_clustering
#synthetising
csynth_design
#exporting IP
export_design -format ip_catalog -vendor "cern-cms" -version 0.2 -description jet_clustering

## STEP 2: Building and exporting to IP format the histogrammer

# creating project and solution
open_project -reset HLS_Histogrammer
open_solution -reset KU115
#setting target device
set_part $part
create_clock -period $clock
#adding hls files
add_files -cflags "-std=c++11" "HistogramAndBuffer/HLS_Histogrammer.cpp"
# adding testbench files
add_files -cflags "-std=c++11" -tb "HistogramAndBuffer/TB_Histogrammer.cpp"
# Sets the top-level function name that is going to be starting point for synthetisation
set_top hls_histogrammer
#synthetising
csynth_design
#exporting IP
export_design -format ip_catalog -vendor "cern-cms" -version 0.2 -description histogrammer

## STEP 3: Building and exporting to IP format the buffer

# creating project and solution
open_project -reset HLS_HistogramBuffer
open_solution -reset KU115
#setting target device
set_part $part
create_clock -period $clock
#adding hls files
add_files -cflags "-std=c++11" "HistogramAndBuffer/HLS_HistogramBuffer.cpp"
# adding testbench files
add_files -cflags "-std=c++11" -tb "HistogramAndBuffer/TB_HistogramBuffer.cpp"
# Sets the top-level function name that is going to be starting point for synthetisation
set_top hls_histogram_buffer
#synthetising
csynth_design
#exporting IP
export_design -format ip_catalog -vendor "cern-cms" -version barrel-only.0.1 -description histogram_buffer