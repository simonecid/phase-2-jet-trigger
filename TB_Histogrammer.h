#ifndef __TB_PHASE1CLUSTERING_H__
#define __TB_PHASE1CLUSTERING_H__

#include "HLS_Histogrammer.h"
#include "HLS/HistogramEtaPhi.h"

bool readInputsFromFile(const std::string &filepath, Inputs inputs);
void resetInputs(Inputs inputs);
bool readBinsFromFile(const std::string &filepath, hls::TBins histogram);
#endif //__TB_PHASE1CLUSTERING_H__