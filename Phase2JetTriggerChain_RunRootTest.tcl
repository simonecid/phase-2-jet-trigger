set part xcku115-flvd1517-2-i
set clock 300MHz

open_project -reset HLS_Phase2TriggerChain
open_solution -reset KU115
#setting target device
set_part $part
create_clock -period $clock
# adding testbench files
add_files -tb "pfdump.root"

add_files -cflags "-std=c++11" "JetClustering/HLS_Phase1Clustering.cpp"
# adding testbench files
add_files -cflags "-std=c++11" "HistogramAndBuffer/HLS_Histogrammer.cpp"
# adding testbench files
add_files -cflags "-std=c++11" "HistogramAndBuffer/HLS_HistogramBuffer.cpp"
# adding testbench files
add_files -cflags "-std=c++11" "HistogramAndBuffer/RegionOffsets.cpp"
# adding testbench files
add_files -cflags "-std=c++11 -DENABLE_ROOT -pthread -m64 -I/software/sb17498/miniconda/envs/ROOT/include/root" -tb "triggerChain.cpp"

csim_design -compiler gcc -ldflags "-L/software/sb17498/miniconda/envs/ROOT/lib/root -lCore -lRIO -lNet -lHist -lGraf -lGraf3d -lGpad -lTree -lTreePlayer -lRint -lPostscript -lMatrix -lPhysics -lMathCore -lThread -Wl,-rpath,/software/sb17498/miniconda/envs/ROOT/lib/root -lm -ldl"