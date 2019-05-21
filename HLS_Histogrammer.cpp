#include "HLS_Histogrammer.h"
#include "HLS/HistogramEtaPhi.h"

void copyInputs (const hls::Inputs srcInputs, hls::Inputs destInputs)
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

void hls_histogrammer(const hls::Inputs inputs, hls::TBins bins)
{
  #pragma HLS array_partition variable=inputs dim=0
  #pragma HLS array_partition variable=bins dim=0
  #pragma HLS data_pack variable=inputs
  #pragma HLS pipeline

  hls::HistogramEtaPhi caloGrid;

  hls::Inputs lInputs;
  #pragma HLS array_partition variable=lInputs dim=0
  #pragma HLS data_pack variable=lInputs 
  copyInputs(inputs, lInputs);
  caloGrid.reset();
  
  // unsigned char lXIndices[NUMBER_OF_INPUTS_PER_CLOCK];
  // unsigned char lYIndices[NUMBER_OF_INPUTS_PER_CLOCK];
  // #pragma HLS array_partition variable=lXIndices dim=0
  // #pragma HLS array_partition variable=lYIndices dim=0

  // fillHistogramLoop: for (unsigned int x = 0; x < NUMBER_OF_INPUTS_PER_CLOCK; x++)
  // {
  //   #pragma HLS unroll
  //   lXIndices[x] = caloGrid.findXBin(lInputs[x].iEta);
  //   lYIndices[x] = caloGrid.findYBin(lInputs[x].iPhi);
  //   caloGrid.addToBin(lXIndices[x], lYIndices[x], lInputs[x].pt);
  //   caloGrid.fill(lInputs[x].iEta, lInputs[x].iPhi, lInputs[x].pt);
  // }

  caloGrid.fill(lInputs);
  
  outputBinsLoops: for (unsigned int etaIndex = 0; etaIndex < ETA_GRID_SIZE; etaIndex++)
  {
    for (unsigned int phiIndex = 0; phiIndex < PHI_GRID_SIZE; phiIndex++)
    {
      bins[phiIndex][etaIndex] = caloGrid.getBin(etaIndex, phiIndex);
    }
  }

  return;
}