#include "HLS_Histogrammer.h"
#include "HLS/Histogram2D.h"

void hls_histogrammer(
                      const hls::Barrel_Inputs barrel_inputs, 
                      const hls::TK_HG_Inputs tk_hg_inputs, 
                      const hls::HG_Inputs hg_inputs, 
                      const hls::HF_Inputs hf_inputs, 
                      hls::Barrel_PfInputHistogram::TBins barrel_bins,
                      hls::TK_HG_PfInputHistogram::TBins tk_hg_bins,
                      hls::HG_PfInputHistogram::TBins hg_bins,
                      hls::HF_PfInputHistogram::TBins hf_bins
                     )
{
  #pragma HLS array_partition variable=barrel_inputs dim=0
  #pragma HLS array_partition variable=tk_hg_inputs dim=0
  #pragma HLS array_partition variable=hg_inputs dim=0
  #pragma HLS array_partition variable=hf_inputs dim=0
  #pragma HLS array_partition variable=barrel_bins dim=0
  #pragma HLS array_partition variable=tk_hg_bins dim=0
  #pragma HLS array_partition variable=hg_bins dim=0
  #pragma HLS array_partition variable=hf_bins dim=0
  #pragma HLS data_pack variable=barrel_inputs
  #pragma HLS data_pack variable=tk_hg_inputs
  #pragma HLS data_pack variable=hg_inputs
  #pragma HLS data_pack variable=hf_inputs
  #pragma HLS pipeline

  hls::Barrel_PfInputHistogram barrel_Histogram;
  hls::TK_HG_PfInputHistogram tk_hg_Histogram;
  hls::HG_PfInputHistogram hg_Histogram;
  hls::HF_PfInputHistogram hf_Histogram;

  barrel_Histogram.setXLow(500);
  barrel_Histogram.setYLow(0);
  tk_hg_Histogram.setXLow(300);
  tk_hg_Histogram.setYLow(0);
  hg_Histogram.setXLow(100);
  hg_Histogram.setYLow(0);
  hf_Histogram.setXLow(0);
  hf_Histogram.setYLow(0);
  
  barrel_Histogram.reset();
  tk_hg_Histogram.reset();
  hg_Histogram.reset();
  hf_Histogram.reset();

  fillHistogramWithInputs(barrel_Histogram, barrel_inputs);
  barrel_outputBinsEtaLoop: for (unsigned char etaIndex = 0; etaIndex < BARREL_ETA_N_BINS; etaIndex++)
  {
    barrel_outputBinsPhiLoop: for (unsigned char phiIndex = 0; phiIndex < PHI_N_BINS; phiIndex++)
    {
      barrel_bins[phiIndex][etaIndex] = barrel_Histogram.getBin(etaIndex, phiIndex);
    }
  }

  // fillHistogramWithInputs(tk_hg_Histogram, tk_hg_inputs);
  // tk_hg_outputBinsEtaLoop: for (unsigned char etaIndex = 0; etaIndex < TK_HG_ETA_N_BINS; etaIndex++)
  // {
  //   tk_hg_outputBinsPhiLoop: for (unsigned char phiIndex = 0; phiIndex < PHI_N_BINS; phiIndex++)
  //   {
  //     tk_hg_bins[phiIndex][etaIndex] = tk_hg_Histogram.getBin(etaIndex, phiIndex);
  //   }
  // }

  // fillHistogramWithInputs(hg_Histogram, hg_inputs);
  // hg_outputBinsEtaLoop: for (unsigned char etaIndex = 0; etaIndex < HG_ETA_N_BINS; etaIndex++)
  // {
  //   hg_outputBinsPhiLoop: for (unsigned char phiIndex = 0; phiIndex < PHI_N_BINS; phiIndex++)
  //   {
  //     hg_bins[phiIndex][etaIndex] = hg_Histogram.getBin(etaIndex, phiIndex);
  //   }
  // }

  // fillHistogramWithInputs(hf_Histogram, hf_inputs);
  // hf_outputBinsEtaLoop: for (unsigned char etaIndex = 0; etaIndex < HF_ETA_N_BINS; etaIndex++)
  // {
  //   hf_outputBinsPhiLoop: for (unsigned char phiIndex = 0; phiIndex < PHI_N_BINS; phiIndex++)
  //   {
  //     hf_bins[phiIndex][etaIndex] = hf_Histogram.getBin(etaIndex, phiIndex);
  //   }
  // }

  return;
}
