#include "HLS_Histogrammer.h"
#include "HLS/Histogram2D.h"

void hls_histogrammer(
                      const hls::Barrel_Inputs barrel_inputs, 
                      hls::Barrel_PfInputHistogram::TBins barrel_bins
                     )
{
  #pragma HLS array_partition variable=barrel_inputs dim=0
  #pragma HLS array_partition variable=barrel_bins dim=0
  #pragma HLS pipeline

  hls::Barrel_PfInputHistogram barrel_Histogram;

  barrel_Histogram.setXLow(0);
  barrel_Histogram.setYLow(0);
  barrel_Histogram.reset();

  fillHistogramWithInputs<hls::Barrel_PfInputHistogram, const hls::Barrel_Inputs, hls::Barrel_Inputs>
    (barrel_Histogram, barrel_inputs);
  barrel_outputBinsEtaLoop: for (unsigned char etaIndex = 0; etaIndex < N_ETA_BINS_BARREL_REGION; etaIndex++)
  {
    barrel_outputBinsPhiLoop: for (unsigned char phiIndex = 0; phiIndex < N_BINS_PHI_REGION; phiIndex++)
    {
      barrel_bins[phiIndex][etaIndex] = barrel_Histogram.getBin(etaIndex, phiIndex);
    }
  }

  return;
}
