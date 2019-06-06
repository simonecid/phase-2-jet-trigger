#ifndef __HLS_HISTOGRAM_FIXED_BIN_SIZE_H__
#define __HLS_HISTOGRAM_FIXED_BIN_SIZE_H__

#include "ap_int.h"
#include "Types.h"
#include "HistogramSettings.h"
#ifndef __SYNTHESIS__
#include <csignal>
#include <iostream>
#endif

namespace hls
{
  // Class handling eta phi histogram
  // Uniform binning only atm
  // x=eta
  // y=phi
  
  template <
    class TBin, //type of the bin
    class TXAxis, //x-axis element type
    TXAxis xStep, //size of bin 
    unsigned char nBinsX,
    class TYAxis, //y-axis element type
    TYAxis yStep, //size of bin 
    unsigned char nBinsY
  >
  class Histogram2DFixedBinSize
  {
    public:
      typedef TBin TBins[nBinsY][nBinsX];
      Histogram2DFixedBinSize();
      
      // #if LUT_FILL == true
      // const static TEta _xBins[nBinsX + 1];
      // const static TPhi _yBins[nBinsY + 1];
      // #endif
      TBin getBin(const unsigned char x, const unsigned char y) const;
      void setBin(const unsigned char x, const unsigned char y, TBin value);
      TBin getXLow() const { return this ->  _xLowEdge; };
      void setXLow(TXAxis lowEdge) { this ->  _xLowEdge = lowEdge; };
      TBin getYLow() const { return this ->  _yLowEdge; };
      void setYLow(TYAxis lowEdge) { this ->  _yLowEdge = lowEdge; };
      TBin getNBinsX() const;
      TBin getNBinsY() const;
      unsigned char findXBin(TXAxis xPosition) const;
      unsigned char findYBin(TYAxis yPosition) const;
      void addToBin(unsigned char xIdx, unsigned char yIdx, TBin value);
      void fill(TXAxis xPosition, TYAxis yPosition, TBin value = 1);
      void reset(TBin value = 0);
    private:
      TBins _histogram;
      TXAxis _xLowEdge;
      TYAxis _yLowEdge;
  };
};

namespace hls
{
  template <
    class TBin, //type of the bin
    class TXAxis, //x-axis element type
    TXAxis xStep, 
    unsigned char nBinsX, //number of bins in x
    class TYAxis, //y-axis element type
    TYAxis yStep,
    unsigned char nBinsY //number of bins in y
  >
  Histogram2DFixedBinSize
  <
    TBin,
    TXAxis,
    xStep,
    nBinsX,
    TYAxis,
    yStep,
    nBinsY
  >::Histogram2DFixedBinSize()
  {
    #pragma HLS inline
    #pragma HLS array_partition variable=_histogram complete dim=0
    // #if LUT_FILL == true
    // #pragma HLS array_partition variable=_xBins complete dim=0
    // #pragma HLS array_partition variable=_yBins complete dim=0
    // #endif 
  }

  // #if LUT_FILL == true
  // const TEta Histogram2DFixedBinSize::_xBins[nBinsX + 1] = ETA_BINS;
  // const TPhi Histogram2DFixedBinSize::_yBins[nBinsY + 1] = PHI_BINS;
  // #endif 

  template <
    class TBin, //type of the bin
    class TXAxis, //x-axis element type
    TXAxis xStep, 
    unsigned char nBinsX, //number of bins in x
    class TYAxis, //y-axis element type
    TYAxis yStep,
    unsigned char nBinsY //number of bins in y
  >
  TBin Histogram2DFixedBinSize
  <
    TBin,
    TXAxis,
    xStep,
    nBinsX,
    TYAxis,
    yStep,
    nBinsY
  >::getBin(const unsigned char x, const unsigned char y) const 
  {
    #pragma HLS inline
    return this -> _histogram[y][x];
  };

  template <
    class TBin, //type of the bin
    class TXAxis, //x-axis element type
    TXAxis xStep, 
    unsigned char nBinsX, //number of bins in x
    class TYAxis, //y-axis element type
    TYAxis yStep,
    unsigned char nBinsY //number of bins in y
  >
  void
  Histogram2DFixedBinSize
  <
    TBin,
    TXAxis,
    xStep,
    nBinsX,
    TYAxis,
    yStep,
    nBinsY
  >::setBin(const unsigned char x, const unsigned char y, TBin value)
  {
    #pragma HLS inline
    this -> _histogram[y][x] = value;
    return;
  };

  template <
    class TBin, //type of the bin
    class TXAxis, //x-axis element type
    TXAxis xStep, 
    unsigned char nBinsX, //number of bins in x
    class TYAxis, //y-axis element type
    TYAxis yStep,
    unsigned char nBinsY //number of bins in y
  >
  TBin
  Histogram2DFixedBinSize
  <
    TBin,
    TXAxis,
    xStep,
    nBinsX,
    TYAxis,
    yStep,
    nBinsY
  >::getNBinsX() const
  {
    #pragma HLS inline
    return nBinsX;
  };

  template <
    class TBin, //type of the bin
    class TXAxis, //x-axis element type
    TXAxis xStep, 
    unsigned char nBinsX, //number of bins in x
    class TYAxis, //y-axis element type
    TYAxis yStep,
    unsigned char nBinsY //number of bins in y
  >
  TBin
  Histogram2DFixedBinSize
  <
    TBin,
    TXAxis,
    xStep,
    nBinsX,
    TYAxis,
    yStep,
    nBinsY
  >::getNBinsY() const
  {
    #pragma HLS inline
    return nBinsY;
  };
  
  template <
    class TBin, //type of the bin
    class TXAxis, //x-axis element type
    TXAxis xStep, 
    unsigned char nBinsX, //number of bins in x
    class TYAxis, //y-axis element type
    TYAxis yStep,
    unsigned char nBinsY //number of bins in y
  >
  unsigned char
  Histogram2DFixedBinSize
  <
    TBin,
    TXAxis,
    xStep,
    nBinsX,
    TYAxis,
    yStep,
    nBinsY
  >::findXBin(TXAxis xPosition) const
  {
    #pragma HLS inline
    #pragma HLS pipeline

    // #if DSP_FILL == true
    TXAxis xDiff = xPosition - this -> _xLowEdge;
    TXAxis xIdx = xDiff / xStep;
    // #endif
    
    // #if LUT_FILL == true    
    // unsigned char xIdx = 0;
    // for (unsigned char x = 0; x < nBinsX; x++)
    // {
    //   if ((xPosition >= hls::Histogram2DFixedBinSize::_xBins[x]) && (xPosition < hls::Histogram2DFixedBinSize::_xBins[x + 1]))
    //   {
    //     xIdx = x;
    //   }
    // }
    // #endif
    return xIdx;
  };

  template <
    class TBin, //type of the bin
    class TXAxis, //x-axis element type
    TXAxis xStep, 
    unsigned char nBinsX, //number of bins in x
    class TYAxis, //y-axis element type
    TYAxis yStep,
    unsigned char nBinsY //number of bins in y
  >
  unsigned char
  Histogram2DFixedBinSize
  <
    TBin,
    TXAxis,
    xStep,
    nBinsX,
    TYAxis,
    yStep,
    nBinsY
  >::findYBin(TYAxis yPosition) const
  {
    #pragma HLS inline
    #pragma HLS pipeline

    // #if DSP_FILL == true
    TYAxis yDiff = yPosition - this -> _yLowEdge;
    TYAxis yIdx = yDiff / yStep;
    // #endif
    
    // #if LUT_FILL == true    
    // unsigned char yIdx = 0;
    // for (unsigned char y = 0; y < nBinsY; y++)
    // {
    //   if ((yPosition >= hls::Histogram2DFixedBinSize::_yBins[y]) && (yPosition < hls::Histogram2DFixedBinSize::_yBins[y + 1]))
    //   {
    //     yIdx = y;
    //   }
    // }
    // #endif
    return yIdx;
  };

  template <
    class TBin, //type of the bin
    class TXAxis, //x-axis element type
    TXAxis xStep, 
    unsigned char nBinsX, //number of bins in x
    class TYAxis, //y-axis element type
    TYAxis yStep,
    unsigned char nBinsY //number of bins in y
  >
  void
  Histogram2DFixedBinSize
  <
    TBin,
    TXAxis,
    xStep,
    nBinsX,
    TYAxis,
    yStep,
    nBinsY
  >::reset(TBin value) 
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

  template <
    class TBin, //type of the bin
    class TXAxis, //x-axis element type
    TXAxis xStep, 
    unsigned char nBinsX, //number of bins in x
    class TYAxis, //y-axis element type
    TYAxis yStep,
    unsigned char nBinsY //number of bins in y
  >
  void
  Histogram2DFixedBinSize
  <
    TBin,
    TXAxis,
    xStep,
    nBinsX,
    TYAxis,
    yStep,
    nBinsY
  >::addToBin(unsigned char xIdx, unsigned char yIdx, TBin value)
  {
    #pragma HLS inline
    this -> _histogram[yIdx][xIdx] += value;
  };

  template <
    class TBin, //type of the bin
    class TXAxis, //x-axis element type
    TXAxis xStep, 
    unsigned char nBinsX, //number of bins in x
    class TYAxis, //y-axis element type
    TYAxis yStep,
    unsigned char nBinsY //number of bins in y
  >
  void
  Histogram2DFixedBinSize
  <
    TBin,
    TXAxis,
    xStep,
    nBinsX,
    TYAxis,
    yStep,
    nBinsY
  >::fill(TXAxis etaPosition, TYAxis phiPosition, TBin value)
  {
    #pragma HLS inline
    #pragma HLS pipeline
    unsigned char xIdx = this -> findXBin(etaPosition);
    unsigned char yIdx = this -> findYBin(phiPosition);
    this -> addToBin(xIdx, yIdx, value);
  };
}

#endif