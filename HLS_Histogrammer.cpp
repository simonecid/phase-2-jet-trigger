#include "HLS_Histogrammer.h"
#include "HLS/HistogramEtaPhi.h"

void copyInputs (const Inputs srcInputs, Inputs destInputs)
{
  #pragma HLS inline
  #pragma HLS pipeline
  for (unsigned int x = 0; x < NUMBER_OF_INPUTS_PER_CLOCK ; x++)
  {
    destInputs[x] = srcInputs[x];
  }
}

void hls_histogrammer(Inputs *inputs, hls::TBins *bins, bool reset)
{
  #pragma HLS partition variable=inputs dim=0
  #pragma HLS partition variable=bins dim=0
  #pragma HLS pipeline


  static hls::HistogramEtaPhi caloGrid;
  #pragma HLS partition variable=caloGrid._histogram complete dim=0
  #pragma HLS partition variable=HistogramEtaPhi._xBins complete dim=0
  #pragma HLS partition variable=HistogramEtaPhi._yBins complete dim=0

  bool lReset = reset;

  Inputs lInputs;
  #pragma HLS partition variable=lInputs dim=0
  copyInputs(inputs, lInputs);

  if (lReset)
  {
    caloGrid.reset();
  }
  
  for (unsigned int x = 0; x < NUMBER_OF_INPUTS_PER_CLOCK; x++)
  {
    const Inputs & lInput = lInputs[x];
    caloGrid.fill(lInput.iEta, lInput.iPhi, lInput.pt);
  }

  for (unsigned int etaIndex = 0; etaIndex < ETA_GRID_SIZE; etaIndex++)
  {
    for (unsigned int phiIndex = 0; phiIndex < PHI_GRID_SIZE; phiIndex++)
    {
      bins[phiIndex][etaIndex] = caloGrid.getBin(etaIndex, phiIndex);
    }
  }

  return;
}