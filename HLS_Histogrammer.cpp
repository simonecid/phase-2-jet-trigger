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
  caloGrid.reset();
  
  fillHistogramWithInputs(caloGrid, inputs);

  outputBinsEtaLoop: for (unsigned char etaIndex = 0; etaIndex < ETA_GRID_SIZE; etaIndex++)
  {
    outputBinsPhiLoop: for (unsigned char phiIndex = 0; phiIndex < PHI_GRID_SIZE; phiIndex++)
    {
      bins[phiIndex][etaIndex] = caloGrid.getBin(etaIndex, phiIndex);
    }
  }

  return;
}

void fillHistogramWithInputs(hls::HistogramEtaPhi & histogram, const hls::Inputs inputs)
{
  #pragma HLS inline    
  #pragma HLS pipeline 

  unsigned char lXIndices[NUMBER_OF_INPUTS_PER_CLOCK];
  unsigned char lYIndices[NUMBER_OF_INPUTS_PER_CLOCK];

  indiciseInputsLoop: for (unsigned int x = 0; x < NUMBER_OF_INPUTS_PER_CLOCK; x++)
  {
    lXIndices[x] = histogram.findXBin(inputs[x].iEta);
    lYIndices[x] = histogram.findYBin(inputs[x].iPhi);
  }

  fillHistogramYLoop: for (unsigned char y = 0; y < PHI_GRID_SIZE; y++)
  {
    fillHistogramXLoop: for (unsigned char x = 0; x < ETA_GRID_SIZE; x++)
    {
      hls::TPt binContent = 0;
      const hls::TPt nullPt = 0;
      fillHistogramInputsLoop: for (unsigned int inputIdx = 0; inputIdx < NUMBER_OF_INPUTS_PER_CLOCK; inputIdx++)
      {
        binContent += ((lXIndices[inputIdx] == x) && (lYIndices[inputIdx] == y)) ?
        inputs[inputIdx].pt : nullPt;
      }
      histogram.setBin(x, y, binContent);
    }
  }
}
