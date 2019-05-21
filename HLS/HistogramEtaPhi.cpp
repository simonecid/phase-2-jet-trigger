#include "HistogramEtaPhi.h"
#include <csignal>

#include "Types.h"

#ifndef __SYNTHESIS__
#include <iostream>
#endif

namespace hls
{
  HistogramEtaPhi::HistogramEtaPhi()
  {
    #pragma HLS array_partition variable=_histogram complete dim=0
    #if LUT_FILL == true
    #pragma HLS array_partition variable=_xBins complete dim=0
    #pragma HLS array_partition variable=_yBins complete dim=0
    #endif 
  }

  #if LUT_FILL == true
  const TEta HistogramEtaPhi::_xBins[ETA_GRID_SIZE + 1] = ETA_BINS;
  const TPhi HistogramEtaPhi::_yBins[PHI_GRID_SIZE + 1] = PHI_BINS;
  #endif 

  TBin HistogramEtaPhi::getBin(const unsigned char x, const unsigned char y) const 
  {
    // #pragma HLS inline
    return this -> _histogram[y][x];
  };

  void HistogramEtaPhi::setBin(const unsigned char x, const unsigned char y, TBin value)
  {
    #pragma HLS inline
    this -> _histogram[y][x] = value;
    return;
  };

  TBin HistogramEtaPhi::getXSize() const
  {
    #pragma HLS inline
    return ETA_GRID_SIZE;
  };

  TBin HistogramEtaPhi::getYSize() const
  {
    #pragma HLS inline
    return PHI_GRID_SIZE;
  };
  
  unsigned char HistogramEtaPhi::findXBin(TEta xPosition) const
  {
    #pragma HLS inline
    #pragma HLS pipeline

    #if DSP_FILL == true
    unsigned char xDiff = xPosition - XBINLOW;
    unsigned char xIdx = xDiff / XBINSTEP;
    #endif
    
    #if LUT_FILL == true    
    unsigned char xIdx = 0;
    for (unsigned char x = 0; x < ETA_GRID_SIZE; x++)
    {
      if ((xPosition >= hls::HistogramEtaPhi::_xBins[x]) && (xPosition < hls::HistogramEtaPhi::_xBins[x + 1]))
      {
        xIdx = x;
      }
    }
    #endif
    return xIdx;
  };

  unsigned char HistogramEtaPhi::findYBin(TPhi yPosition) const
  {
    #pragma HLS inline
    #pragma HLS pipeline

    #if DSP_FILL == true
    unsigned char yDiff = yPosition - YBINLOW;
    unsigned char yIdx = yDiff / YBINSTEP;
    #endif
    
    #if LUT_FILL == true    
    unsigned char yIdx = 0;
    for (unsigned char y = 0; y < PHI_GRID_SIZE; y++)
    {
      if ((yPosition >= hls::HistogramEtaPhi::_yBins[y]) && (yPosition < hls::HistogramEtaPhi::_yBins[y + 1]))
      {
        yIdx = y;
      }
    }
    #endif
    return yIdx;
  };

  void HistogramEtaPhi::reset(TBin value) 
  {
    // #pragma HLS inline
    #pragma HLS pipeline
    resetHistogramLoops: for (unsigned char x = 0; x < ETA_GRID_SIZE; x++)
    {
      for (unsigned char y = 0; y < PHI_GRID_SIZE; y++)
      {
        this -> _histogram[y][x] = value;
      }
    }
  };

  void HistogramEtaPhi::addToBin(unsigned char xIdx, unsigned char yIdx, TBin value)
  {
    #pragma HLS inline
    this -> _histogram[yIdx][xIdx] += value;
  };

  void HistogramEtaPhi::fill(TEta etaPosition, TPhi phiPosition, TBin value)
  {
    #pragma HLS inline
    #pragma HLS pipeline
    unsigned char xIdx = this -> findXBin(etaPosition);
    unsigned char yIdx = this -> findYBin(phiPosition);
    this -> addToBin(xIdx, yIdx, value);
  };

  void HistogramEtaPhi::fill(const Inputs inputs)
  {
    #pragma HLS inline    
    #pragma HLS pipeline 

    unsigned char lXIndices[NUMBER_OF_INPUTS_PER_CLOCK];
    unsigned char lYIndices[NUMBER_OF_INPUTS_PER_CLOCK];

    fillHistogramLoop: for (unsigned int x = 0; x < NUMBER_OF_INPUTS_PER_CLOCK; x++)
    {
      lXIndices[x] = this -> findXBin(inputs[x].iEta);
      lYIndices[x] = this -> findYBin(inputs[x].iPhi);
      this -> addToBin(lXIndices[x], lYIndices[x], inputs[x].pt);
      // caloGrid.fill(lInputs[x].iEta, lInputs[x].iPhi, lInputs[x].pt);
    }
  };

}