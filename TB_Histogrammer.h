#ifndef __TB_HISTOGRAMMER_H__
#define __TB_HISTOGRAMMER_H__

#include "HLS_Histogrammer.h"
#include "HLS/Histogram2D.h"
#include "HLS/Types.h"
#include "HLS/HistogramSettings.h"

bool readInputsFromFile(const std::string &filepath, hls::Barrel_Inputs inputs);
void resetInputs(hls::Barrel_Inputs inputs);
// bool readBinsFromFile(const std::string &filepath, PfInputHistogram::TBins histogram);
#endif //__TB_PHASE1CLUSTERING_H__