#include "HistogramEtaPhi.h"
#include <csignal>

#ifndef __SYNTHESIS__
#include <iostream>
#endif

namespace hls
{
  HistogramEtaPhi::HistogramEtaPhi()
  {
    #pragma HLS array_partition variable=_histogram complete dim=0
    #pragma HLS array_partition variable=_xBins complete dim=0
    #pragma HLS array_partition variable=_yBins complete dim=0
  }
  const TEta HistogramEtaPhi::_xBins[ETA_GRID_SIZE + 1] = ETA_BINS;
  const TPhi HistogramEtaPhi::_yBins[PHI_GRID_SIZE + 1] = PHI_BINS;

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

  void HistogramEtaPhi::reset(TBin value) 
  {
    #pragma HLS inline
    #pragma HLS pipeline
    for (unsigned char x = 0; x < ETA_GRID_SIZE; x++)
    {
      for (unsigned char y = 0; y < PHI_GRID_SIZE; y++)
      {
        this -> _histogram[y][x] = value;
      }
    }
  };

  void HistogramEtaPhi::fill(TEta etaPosition, TPhi phiPosition, TBin value)
  {
    #pragma HLS inline
    #pragma HLS pipeline

    unsigned char xIdx = 0;
    for (unsigned char x = 0; x < ETA_GRID_SIZE; x++)
    {

      if ((etaPosition >= hls::HistogramEtaPhi::_xBins[x]) && (etaPosition < hls::HistogramEtaPhi::_xBins[x + 1]))
      {
        xIdx = x;
      }
    }

    unsigned char yIdx = 0;
    for (unsigned char y = 0; y < PHI_GRID_SIZE; y++)
    {
      if ((phiPosition >= hls::HistogramEtaPhi::_yBins[y]) && (phiPosition < hls::HistogramEtaPhi::_yBins[y + 1]))
      {
        yIdx = y;
      }
    }
    
    this -> _histogram[yIdx][xIdx] += value;
  };
}