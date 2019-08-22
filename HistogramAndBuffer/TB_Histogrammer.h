#ifndef __TB_HISTOGRAMMER_H__
#define __TB_HISTOGRAMMER_H__

#include "HLS_Histogrammer.h"
#include "HLS/Histogram2D.h"
#include "HLS/Types.h"
#include "HLS/HistogramSettings.h"

template<class Inputs>
bool readInputsFromFile(const std::string &filepath, Inputs inputs);
template <class TBins>
bool readBinsFromFile(const std::string &filepath, TBins histogram);
// bool readBinsFromFile(const std::string &filepath, PfInputHistogram::TBins histogram);
#endif //__TB_PHASE1CLUSTERING_H__