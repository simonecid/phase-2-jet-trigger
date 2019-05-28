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
    #pragma HLS inline
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
    #pragma HLS inline
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
    #pragma HLS inline
    #pragma HLS pipeline
    resetHistogramYLoop: for (unsigned char y = 0; y < PHI_GRID_SIZE; y++)
    {
      resetHistogramXLoop: for (unsigned char x = 0; x < ETA_GRID_SIZE; x++)
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
}