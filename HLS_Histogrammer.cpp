#include "HLS_Histogrammer.h"
#include "HLS/Histogram2D.h"
#include "RegionOffsets.h"

template <class THistogram, class TBins, class TConstInputs, class TInputs>
void histogramInputs(TConstInputs inputs, TBins bins, unsigned char etaOffset, unsigned char phiOffset)
{
  #pragma HLS inline
  #pragma HLS pipeline

  THistogram lHistogram;

  lHistogram.setXLow(etaOffset);
  lHistogram.setYLow(phiOffset);
  lHistogram.reset();

  fillHistogramWithInputs<THistogram, TConstInputs, TInputs>
    (lHistogram, inputs);
  outputBinsEtaLoop: for (unsigned char etaIndex = 0; etaIndex < lHistogram.getNBinsX(); etaIndex++)
  {
    outputBinsPhiLoop: for (unsigned char phiIndex = 0; phiIndex < lHistogram.getNBinsY(); phiIndex++)
    {
      bins[phiIndex][etaIndex] = lHistogram.getBin(etaIndex, phiIndex);
    }
  }
}

void hls_histogrammer(
                      const hls::Barrel_Inputs barrel_inputs, 
                      hls::Barrel_PfInputHistogram::TBins barrel_bins,
                      bool inReset,
                      bool & outReset
                     )
{
  #pragma HLS data_pack variable=barrel_inputs
  #pragma HLS array_partition variable=barrel_inputs dim=0
  #pragma HLS array_partition variable=barrel_bins dim=0
  #pragma HLS pipeline

  static unsigned char sNumberOfRegionsReceived = 0;
  unsigned char lNumberOfRegionsReceived = (inReset) ? 0 : sNumberOfRegionsReceived;
  sNumberOfRegionsReceived = (inReset) ? 1 : sNumberOfRegionsReceived + 1;

  // checking if all the regions have been received
  if (lNumberOfRegionsReceived < N_ETA_SEGMENTS_BARREL * N_PHI_SEGMENTS)
  {
    unsigned char lEtaOffset = returnBarrelEtaOffset(lNumberOfRegionsReceived);
    unsigned char lPhiOffset = returnBarrelPhiOffset(lNumberOfRegionsReceived);
    histogramInputs<hls::Barrel_PfInputHistogram, hls::Barrel_PfInputHistogram::TBins, const hls::Barrel_Inputs, hls::Barrel_Inputs>
      (barrel_inputs, barrel_bins, etaOffset, phiOffset);
  }

  outReset = inReset;

  return;
}
