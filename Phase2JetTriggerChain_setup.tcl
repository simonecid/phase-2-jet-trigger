set part xcku115-flvd1517-2-i
set clock 300MHz

open_project -reset HLS_Phase2TriggerChain
open_solution -reset KU115
#setting target device
set_part $part
create_clock -period $clock
# adding testbench files
add_files -cflags "-std=c++11" "JetClustering/HLS_Phase1Clustering.cpp"
# adding testbench files
add_files -cflags "-std=c++11" "HistogramAndBuffer/HLS_Histogrammer.cpp"
# adding testbench files
add_files -cflags "-std=c++11" "HistogramAndBuffer/HLS_HistogramBuffer.cpp"
# adding testbench files
add_files -cflags "-std=c++11" "HistogramAndBuffer/RegionOffsets.cpp"
# adding testbench files
add_files -cflags "-std=c++11" -tb "triggerChain.cpp"