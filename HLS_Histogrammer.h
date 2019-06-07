#ifndef __HLS_HISTOGRAMMER_H__
#define __HLS_HISTOGRAMMER_H__

#include "HLS/Types.h"
#include "HLS/Histogram2D.h"
#include "HLS/HistogramSettings.h"

void hls_histogrammer(
                      const hls::Barrel_Inputs barrel_inputs, 
                      const hls::TK_HG_Inputs tk_hg_inputs, 
                      const hls::HG_Inputs hg_inputs, 
                      const hls::HF_Inputs hf_inputs, 
                      hls::Barrel_PfInputHistogram::TBins barrel_bins,
                      hls::TK_HG_PfInputHistogram::TBins tk_hg_bins,
                      hls::HG_PfInputHistogram::TBins hg_bins,
                      hls::HF_PfInputHistogram::TBins hf_bins
                     );

template<class Inputs>
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

template <class THistogram, class Inputs>
void fillHistogramWithInputs(THistogram & histogram, Inputs inputs)
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

  fillHistogramYLoop: for (unsigned char y = 0; y < histogram.getNBinsY(); y++)
  {
    fillHistogramXLoop: for (unsigned char x = 0; x < histogram.getNBinsX(); x++)
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

#endif //__HLS_HISTOGRAMMER_H__