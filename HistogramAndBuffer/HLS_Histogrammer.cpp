#include "HLS_Histogrammer.h"
#include "HLS/Histogram2D.h"
#include "RegionOffsets.h"

#ifndef __SYNTHESIS__
#include <iostream>
#endif

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
                      bool d0Valid,
                      bool & outValid
                     )
{
  #pragma HLS data_pack variable=barrel_inputs
  #pragma HLS array_partition variable=barrel_inputs dim=0
  #pragma HLS array_partition variable=barrel_bins dim=0
  // no valid ports for the I/O
  #pragma HLS interface ap_none port=barrel_inputs
  // no valid ports for the I/O
  #pragma HLS interface ap_none port=barrel_bins
  // no valid ports for the I/O
  #pragma HLS interface ap_none port=d0Valid
  // no valid ports for the I/O
  #pragma HLS interface ap_none port=outValid
  // removing control bus from design
  #pragma HLS interface ap_ctrl_none port=return
  #pragma HLS pipeline II=1

  // keeps track of when to reset the internal status
  static unsigned char sRegister = 0;
  unsigned char lEtaOffset = 0;
  unsigned char lPhiOffset = 0;
  unsigned char lRegionID = 0;

  // reset if we have a transition 0 -> 1 of the reset signal
  bool lReset = d0Valid;


  if (lReset)

  {
    sRegister = -1;
  }
  else 
  {
    // if we are one clock before the reset period, instead of incrementing the counter, we reset it
    sRegister = (sRegister == (RESET_PERIOD - 1)) ? 0 : sRegister + 1;
    lRegionID = sRegister;
    #ifndef __SYNTHESIS__
    std::cout << "sRegister: " << +sRegister << std::endl;
    #endif
  }

  histogramInputs<hls::Barrel_PfInputHistogram, hls::Barrel_PfInputHistogram::TBins, const hls::Barrel_Inputs, hls::Barrel_Inputs>
    (barrel_inputs, barrel_bins, lEtaOffset, lPhiOffset);
  outValid = d0Valid; //alignment`

  return;
}
