#ifndef __HLS_HISTOGRAM_BUFFER_H__
#define __HLS_HISTOGRAM_BUFFER_H__

#include "HLS/Types.h"
#include "HLS/Histogram2D.h"
#include "HLS/HistogramSettings.h"
#include <hls_video.h>

// typedef hls::TPt TBuffer[PHI_N_BINS][N_ETA_BINS_BARREL_REGION];
typedef hls::Window<N_BINS_PHI, N_ETA_BINS_BARREL_REGION * N_ETA_SEGMENTS_BARREL, hls::TPt> TBarrelBuffer;
typedef hls::Window<N_BINS_PHI, N_BINS_ETA, hls::TPt> TBuffer;


void hls_histogram_buffer(
                      const hls::Barrel_PfInputHistogram::TBins inBarrelBins,
                      hls::TPt outBins[N_ETA_BINS_BARREL_REGION * N_ETA_SEGMENTS_BARREL],
                      bool d0Valid,
                      bool & outValid
                     );

#endif //__HLS_HISTOGRAMMER_H__