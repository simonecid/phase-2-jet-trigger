#include "Histogram2D.h"
#include <csignal>

#include "Types.h"

#ifndef __SYNTHESIS__
#include <iostream>
#endif

namespace hls
{
    template <
    class TBin, //type of the bin
    class TXAxis, //x-axis element type
    TBin xLow, //x-axis low edge
    TBin xHigh, //x-axis high edge
    TBin nBinsX, //number of bins in x
    class TYAxis, //y-axis element type
    TBin yLow,  //x-axis low edge
    TBin yHigh,   //x-axis high edge
    TBin nBinsY //number of bins in y
  >
  Histogram2D::Histogram2D()
  {
    #pragma HLS inline
    #pragma HLS array_partition variable=_histogram complete dim=0
    // #if LUT_FILL == true
    // #pragma HLS array_partition variable=_xBins complete dim=0
    // #pragma HLS array_partition variable=_yBins complete dim=0
    // #endif 
  }

  // #if LUT_FILL == true
  // const TEta Histogram2D::_xBins[ETA_GRID_SIZE + 1] = ETA_BINS;
  // const TPhi Histogram2D::_yBins[PHI_GRID_SIZE + 1] = PHI_BINS;
  // #endif 

  TBin Histogram2D::getBin(const unsigned char x, const unsigned char y) const 
  {
    #pragma HLS inline
    return this -> _histogram[y][x];
  };

  void Histogram2D::setBin(const unsigned char x, const unsigned char y, TBin value)
  {
    #pragma HLS inline
    this -> _histogram[y][x] = value;
    return;
  };

  TBin Histogram2D::getXSize() const
  {
    #pragma HLS inline
    return ETA_GRID_SIZE;
  };

  TBin Histogram2D::getYSize() const
  {
    #pragma HLS inline
    return PHI_GRID_SIZE;
  };
  
  unsigned char Histogram2D::findXBin(TXAxis xPosition) const;
  {
    #pragma HLS inline
    #pragma HLS pipeline

    // #if DSP_FILL == true
    TXAxis xDiff = xPosition - xLow;
    constexpr TXAxis xBinStep = (xHigh - xLow) / nBinsX;
    TXAxis xIdx = xDiff / xBinStep;
    // #endif
    
    // #if LUT_FILL == true    
    // unsigned char xIdx = 0;
    // for (unsigned char x = 0; x < ETA_GRID_SIZE; x++)
    // {
    //   if ((xPosition >= hls::Histogram2D::_xBins[x]) && (xPosition < hls::Histogram2D::_xBins[x + 1]))
    //   {
    //     xIdx = x;
    //   }
    // }
    // #endif
    return xIdx;
  };

  unsigned char Histogram2D::findYBin(TYAxis yPosition) const;
  {
    #pragma HLS inline
    #pragma HLS pipeline

    // #if DSP_FILL == true
    TYAxis yDiff = yPosition - yLow;
    constexpr TYAxis yBinStep = (yHigh - yLow) / nBinsY;
    TYAxis yIdx = yDiff / yBinStep;
    // #endif
    
    // #if LUT_FILL == true    
    // unsigned char yIdx = 0;
    // for (unsigned char y = 0; y < PHI_GRID_SIZE; y++)
    // {
    //   if ((yPosition >= hls::Histogram2D::_yBins[y]) && (yPosition < hls::Histogram2D::_yBins[y + 1]))
    //   {
    //     yIdx = y;
    //   }
    // }
    // #endif
    return yIdx;
  };

  void Histogram2D::reset(TBin value) 
  {
    #pragma HLS inline
    #pragma HLS pipeline
    resetHistogramYLoop: for (unsigned char y = 0; y < nBinsY; y++)
    {
      resetHistogramXLoop: for (unsigned char x = 0; x < nBinsX; x++)
      {
        this -> _histogram[y][x] = value;
      }
    }
  };

  void Histogram2D::addToBin(unsigned char xIdx, unsigned char yIdx, TBin value)
  {
    #pragma HLS inline
    this -> _histogram[yIdx][xIdx] += value;
  };

  void Histogram2D::fill(TXAxis etaPosition, TYAxis phiPosition, TBin value)
  {
    #pragma HLS inline
    #pragma HLS pipeline
    unsigned char xIdx = this -> findXBin(etaPosition);
    unsigned char yIdx = this -> findYBin(phiPosition);
    this -> addToBin(xIdx, yIdx, value);
  };
}