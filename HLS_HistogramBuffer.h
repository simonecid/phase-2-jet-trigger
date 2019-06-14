#ifndef __HLS_HISTOGRAM_BUFFER_H__
#define __HLS_HISTOGRAM_BUFFER_H__

#include "HLS/Types.h"
#include "HLS/Histogram2D.h"
#include "HLS/HistogramSettings.h"
#include <hls_video.h>

// typedef hls::TPt TBuffer[PHI_N_BINS][N_ETA_BINS_BARREL_REGION];
typedef hls::Window<N_BINS_PHI_REGION * N_PHI_SEGMENTS, N_ETA_BINS_BARREL_REGION * N_ETA_SEGMENTS_BARREL, hls::TPt> TBarrelBuffer;


void hls_histogram_buffer(
                      const hls::Barrel_PfInputHistogram::TBins inBarrelBins,
                      hls::TPt outBins[N_ETA_BINS_BARREL_REGION * N_ETA_SEGMENTS_BARREL],
                      bool reset
                     );

#endif //__HLS_HISTOGRAMMER_H__