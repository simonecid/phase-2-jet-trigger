#ifndef __HLS_HISTOGRAMMER_H__
#define __HLS_HISTOGRAMMER_H__

#include "HLS/Types.h"
#include "HLS/Histogram2D.h"
#include "HLS/HistogramSettings.h"

typedef hls::TPt TBuffer[PHI_N_BINS][ETA_GRID_SIZE];

void hls_histogram_buffer(
                      const hls::Barrel_PfInputHistogram::TBins in_barrel_bins,
                      const hls::TK_HG_PfInputHistogram::TBins in_tk_hg_bins,
                      const hls::HG_PfInputHistogram::TBins in_hg_bins,
                      const hls::HF_PfInputHistogram::TBins in_hf_bins,
                      hls::TPt out_bins[ETA_GRID_SIZE]
                     );

#endif //__HLS_HISTOGRAMMER_H__