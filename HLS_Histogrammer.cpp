#include "HLS_Histogrammer.h"
#include "HLS/HistogramEtaPhi.h"

void copyInputs (const Inputs srcInputs, Inputs destInputs)
{
  #pragma HLS inline
  #pragma HLS pipeline
  copyInputsLoop: for (unsigned int x = 0; x < NUMBER_OF_INPUTS_PER_CLOCK ; x++)
  {
    destInputs[x].iEta = srcInputs[x].iEta;
    destInputs[x].iPhi = srcInputs[x].iPhi;
    destInputs[x].pt = srcInputs[x].pt;
  }
}

void hls_histogrammer(const Inputs inputs, hls::TBins bins,const bool reset)
{
  #pragma HLS array_partition variable=inputs dim=0
  #pragma HLS array_partition variable=bins dim=0
  #pragma HLS pipeline

  hls::HistogramEtaPhi caloGrid;

  bool lReset = reset;

  Inputs lInputs;
  #pragma HLS array_partition variable=lInputs dim=0
  copyInputs(inputs, lInputs);
  caloGrid.reset();
  //if (lReset)
  //{
  //  caloGrid.reset();
  //}
  
  fillHistogramLoop:for (unsigned int x = 0; x < NUMBER_OF_INPUTS_PER_CLOCK; x++)
  {
    caloGrid.fill(lInputs[x].iEta, lInputs[x].iPhi, lInputs[x].pt);
  }
  
  outputBinsLoop: for (unsigned int etaIndex = 0; etaIndex < ETA_GRID_SIZE; etaIndex++)
  {
    for (unsigned int phiIndex = 0; phiIndex < PHI_GRID_SIZE; phiIndex++)
    {
      bins[phiIndex][etaIndex] = caloGrid.getBin(etaIndex, phiIndex);
    }
  }

  return;
}