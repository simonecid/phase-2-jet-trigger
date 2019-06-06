#ifndef __HLS_HISTOGRAMMER_H__
#define __HLS_HISTOGRAMMER_H__

#include "HLS/Types.h"
#include "HLS/Histogram2D.h"
#include "HLS/HistogramSettings.h"

typedef hls::Histogram2DFixedBinSize<
    hls::TPt,
    hls::TEta,
    XBINSTEP,
    ETA_GRID_SIZE,
    hls::TPhi,
    YBINSTEP,
    PHI_GRID_SIZE
    > PfInputHistogram;

void hls_histogrammer(const hls::Inputs inputs, PfInputHistogram::TBins bins);
void copyInputs (const hls::Inputs srcInputs, hls::Inputs destInputs);
void fillHistogramWithInputs(PfInputHistogram & histogram, const hls::Inputs inputs);

#endif //__HLS_HISTOGRAMMER_H__