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
                      hls::Barrel_PfInputHistogram::TBins barrel_bins,
                      const hls::TK_HG_Inputs tk_hg_inputs, 
                      hls::TK_HG_PfInputHistogram::TBins tk_hg_bins,
                      const hls::HG_Inputs hg_inputs, 
                      hls::HG_PfInputHistogram::TBins hg_bins,
                      const hls::HF_Inputs hf_inputs, 
                      hls::HF_PfInputHistogram::TBins hf_bins,
                      bool inReset,
                      bool & outReset
                     )
{
  #pragma HLS data_pack variable=barrel_inputs
  #pragma HLS data_pack variable=tk_hg_inputs
  #pragma HLS data_pack variable=hg_inputs
  #pragma HLS data_pack variable=hf_inputs
  #pragma HLS array_partition variable=barrel_inputs dim=0
  #pragma HLS array_partition variable=barrel_bins dim=0
  #pragma HLS array_partition variable=tk_hg_inputs dim=0
  #pragma HLS array_partition variable=tk_hg_bins dim=0
  #pragma HLS array_partition variable=hg_inputs dim=0
  #pragma HLS array_partition variable=hg_bins dim=0
  #pragma HLS array_partition variable=hf_inputs dim=0
  #pragma HLS array_partition variable=hf_bins dim=0
  #pragma HLS pipeline

  histogramInputs<hls::Barrel_PfInputHistogram, hls::Barrel_PfInputHistogram::TBins, const hls::Barrel_Inputs, hls::Barrel_Inputs>
    (barrel_inputs, barrel_bins);
  histogramInputs<hls::TK_HG_PfInputHistogram, hls::TK_HG_PfInputHistogram::TBins, const hls::TK_HG_Inputs, hls::TK_HG_Inputs>
    (tk_hg_inputs, tk_hg_bins);
  histogramInputs<hls::HG_PfInputHistogram, hls::HG_PfInputHistogram::TBins, const hls::HG_Inputs, hls::HG_Inputs>
    (hg_inputs, hg_bins);
  histogramInputs<hls::HF_PfInputHistogram, hls::HF_PfInputHistogram::TBins, const hls::HF_Inputs, hls::HF_Inputs>
    (hf_inputs, hf_bins);

  outReset = inReset;

  return;
}
