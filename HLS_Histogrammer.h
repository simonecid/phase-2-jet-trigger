#ifndef __HLS_HISTOGRAMMER_H__
#define __HLS_HISTOGRAMMER_H__

#include "HLS/Types.h"
#include "HLS/Histogram2D.h"

typedef   hls::Histogram2D<
    hls::TPt,
    hls::TEta,
    0,
    100,
    10,
    hls::TPhi,
    0,
    100,
    10
    > myHist;

void hls_histogrammer(const hls::Inputs inputs, myHist::TBins bins);
void copyInputs (const hls::Inputs srcInputs, hls::Inputs destInputs);
void fillHistogramWithInputs(myHist & histogram, const hls::Inputs inputs);

#endif //__HLS_HISTOGRAMMER_H__