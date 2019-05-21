#ifndef __TB_PHASE1CLUSTERING_H__
#define __TB_PHASE1CLUSTERING_H__

#include "HLS_Histogrammer.h"
#include "HLS/HistogramEtaPhi.h"
#include "HLS/Types.h"
#include "HLS/HistogramSettings.h"

bool readInputsFromFile(const std::string &filepath, hls::Inputs inputs);
void resetInputs(hls::Inputs inputs);
bool readBinsFromFile(const std::string &filepath, hls::TBins histogram);
#endif //__TB_PHASE1CLUSTERING_H__