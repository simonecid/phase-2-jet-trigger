#include "HLS_Histogrammer.h"
#include "HLS/Histogram2D.h"

template <class THistogram, class TBins, class TConstInputs, class TInputs>
void histogramInputs(TConstInputs inputs, TBins bins)
{
  #pragma HLS inline
  #pragma HLS pipeline

  THistogram lHistogram;

  lHistogram.setXLow(0);
  lHistogram.setYLow(0);
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
                      hls::Barrel_PfInputHistogram::TBins barrel_bins
                     )
{
  #pragma HLS data_pack variable=barrel_inputs
  #pragma HLS array_partition variable=barrel_inputs dim=0
  #pragma HLS array_partition variable=barrel_bins dim=0
  #pragma HLS pipeline

  histogramInputs<hls::Barrel_PfInputHistogram, hls::Barrel_PfInputHistogram::TBins, const hls::Barrel_Inputs, hls::Barrel_Inputs>
    (barrel_inputs, barrel_bins);

  return;
}
