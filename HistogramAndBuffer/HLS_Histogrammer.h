#ifndef __HLS_HISTOGRAMMER_H__
#define __HLS_HISTOGRAMMER_H__

#include "HLS/Types.h"
#include "HLS/Histogram2D.h"
#include "HLS/HistogramSettings.h"

void hls_histogrammer(
                      const hls::Barrel_Inputs barrel_inputs, 
                      hls::Barrel_PfInputHistogram::TBins barrel_bins,
                      bool d0Valid,
                      bool & outValid
                     );

template<class TConstInputs, class TInputs>
void copyInputs (TConstInputs srcInputs, TInputs destInputs)
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

template <class THistogram, class TConstInputs, class TInputs>
void fillHistogramWithInputs(THistogram & histogram, TConstInputs inputs)
{
  #pragma HLS inline    
  #pragma HLS pipeline 

  TInputs lInputs;

  copyInputs<TConstInputs, TInputs>(inputs, lInputs);

  unsigned char lXIndices[NUMBER_OF_INPUTS_PER_CLOCK];
  unsigned char lYIndices[NUMBER_OF_INPUTS_PER_CLOCK];

  indiciseInputsLoop: for (unsigned int x = 0; x < NUMBER_OF_INPUTS_PER_CLOCK; x++)
  {
    // if the momentum of the input is null, then no need to find the bin
    lXIndices[x] = (lInputs[x].pt == 0) ? 0 : histogram.findXBin(lInputs[x].iEta);
    lYIndices[x] = (lInputs[x].pt == 0) ? 0 : histogram.findYBin(lInputs[x].iPhi);
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
        lInputs[inputIdx].pt : nullPt;
      }
      histogram.setBin(x, y, binContent);
    }
  }
}

#endif //__HLS_HISTOGRAMMER_H__