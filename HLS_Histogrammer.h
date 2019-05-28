#ifndef __HLS_HISTOGRAMMER_H__
#define __HLS_HISTOGRAMMER_H__

#include "HLS/Types.h"
#include "HLS/HistogramEtaPhi.h"

void hls_histogrammer(const hls::Inputs inputs, hls::TBins bins);
void copyInputs (const hls::Inputs srcInputs, hls::Inputs destInputs);
void fillHistogramWithInputs(hls::HistogramEtaPhi & histogram, const hls::Inputs inputs);

#endif //__HLS_HISTOGRAMMER_H__