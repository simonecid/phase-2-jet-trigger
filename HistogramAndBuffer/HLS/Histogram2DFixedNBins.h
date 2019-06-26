#ifndef __HLS_HISTOGRAM_2D_FIXED_N_BINS_H__
#define __HLS_HISTOGRAM_2D_FIXED_N_BINS_H__

#include <ap_int.h>
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
    class TBin, 
    class TXAxis, //x-axis element type
    unsigned char nBinsX, //number of bins in x
    class TYAxis, //y-axis element type
    unsigned char nBinsY //number of bins in y
  >
  class Histogram2DFixedNBins
  {
    public:
      typedef TBin TBins[nBinsY][nBinsX];
      Histogram2DFixedNBins();
      
      // #if LUT_FILL == true
      // const static TEta _xBins[nBinsX + 1];
      // const static TPhi _yBins[nBinsY + 1];
      // #endif
      void getXAxisLimits(TXAxis &xLow, TXAxis &xHigh) const;
      void setXAxisLimits(TXAxis xLow, TXAxis xHigh);
      void getYAxisLimits(TYAxis &yLow, TYAxis &yHigh) const;
      void setYAxisLimits(TYAxis yLow, TYAxis yHigh);
      TBin getBin(const unsigned char x, const unsigned char y) const;
      void setBin(const unsigned char x, const unsigned char y, TBin value);
      TBin getXSize() const;
      TBin getYSize() const;
      unsigned char findXBin(TXAxis xPosition) const;
      unsigned char findYBin(TYAxis yPosition) const;
      void addToBin(unsigned char xIdx, unsigned char yIdx, TBin value);
      void fill(TXAxis xPosition, TYAxis yPosition, TBin value = 1);
      void reset(TBin value = 0);
    private:
      TBins _histogram;
      TXAxis _xLow, _xHigh, _xStep;
      TYAxis _yLow, _yHigh, _yStep;
  };
};

namespace hls
{
  template <
    class TBin, 
    class TXAxis, //x-axis element type
    unsigned char nBinsX, //number of bins in x
    class TYAxis, //y-axis element type
    unsigned char nBinsY //number of bins in y
  >
  Histogram2DFixedNBins
  <
    TBin,
    TXAxis,
    nBinsX,
    TYAxis,
    nBinsY
  >::Histogram2DFixedNBins()
  {
    #pragma HLS inline
    #pragma HLS array_partition variable=_histogram complete dim=0
    // #if LUT_FILL == true
    // #pragma HLS array_partition variable=_xBins complete dim=0
    // #pragma HLS array_partition variable=_yBins complete dim=0
    // #endif 
  }

  // #if LUT_FILL == true
  // const TEta Histogram2DFixedNBins::_xBins[nBinsX + 1] = ETA_BINS;
  // const TPhi Histogram2DFixedNBins::_yBins[nBinsY + 1] = PHI_BINS;
  // #endif 

  template <
    class TBin, //type of the bin
    class TXAxis, //x-axis element type
    unsigned char nBinsX, //number of bins in x
    class TYAxis, //y-axis element type
    unsigned char nBinsY //number of bins in y
  >
  TBin Histogram2DFixedNBins
  <
    TBin,
    TXAxis,
    nBinsX,
    TYAxis,
    nBinsY
  >::getBin(const unsigned char x, const unsigned char y) const 
  {
    #pragma HLS inline
    return this -> _histogram[y][x];
  };

  template <
    class TBin, //type of the bin
    class TXAxis, //x-axis element type
    unsigned char nBinsX, //number of bins in x
    class TYAxis, //y-axis element type
    unsigned char nBinsY //number of bins in y
  >
  void
  Histogram2DFixedNBins
  <
    TBin,
    TXAxis,
    nBinsX,
    TYAxis,
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
    unsigned char nBinsX, //number of bins in x
    class TYAxis, //y-axis element type
    unsigned char nBinsY //number of bins in y
  >
  TBin
  Histogram2DFixedNBins
  <
    TBin,
    TXAxis,
    nBinsX,
    TYAxis,
    nBinsY
  >::getXSize() const
  {
    #pragma HLS inline
    return nBinsX;
  };

  template <
    class TBin, //type of the bin
    class TXAxis, //x-axis element type
    unsigned char nBinsX, //number of bins in x
    class TYAxis, //y-axis element type
    unsigned char nBinsY //number of bins in y
  >
  TBin
  Histogram2DFixedNBins
  <
    TBin,
    TXAxis,
    nBinsX,
    TYAxis,
    nBinsY
  >::getYSize() const
  {
    #pragma HLS inline
    return nBinsY;
  };
  
  template <
    class TBin, //type of the bin
    class TXAxis, //x-axis element type
    unsigned char nBinsX, //number of bins in x
    class TYAxis, //y-axis element type
    unsigned char nBinsY //number of bins in y
  >
  unsigned char
  Histogram2DFixedNBins
  <
    TBin,
    TXAxis,
    nBinsX,
    TYAxis,
    nBinsY
  >::findXBin(TXAxis xPosition) const
  {
    #pragma HLS inline
    #pragma HLS pipeline

    // #if DSP_FILL == true
    TXAxis xDiff = xPosition - this -> _xLow;
    TXAxis xIdx = xDiff / this -> _xStep;
    // #endif
    
    // #if LUT_FILL == true    
    // unsigned char xIdx = 0;
    // for (unsigned char x = 0; x < nBinsX; x++)
    // {
    //   if ((xPosition >= hls::Histogram2DFixedNBins::_xBins[x]) && (xPosition < hls::Histogram2DFixedNBins::_xBins[x + 1]))
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
    unsigned char nBinsX, //number of bins in x
    class TYAxis, //y-axis element type
    unsigned char nBinsY //number of bins in y
  >
  unsigned char
  Histogram2DFixedNBins
  <
    TBin,
    TXAxis,
    nBinsX,
    TYAxis,
    nBinsY
  >::findYBin(TYAxis yPosition) const
  {
    #pragma HLS inline
    #pragma HLS pipeline

    // #if DSP_FILL == true
    TYAxis yDiff = yPosition - this -> _yLow;
    TYAxis yIdx = yDiff / this -> _yStep;
    // #endif
    
    // #if LUT_FILL == true    
    // unsigned char yIdx = 0;
    // for (unsigned char y = 0; y < nBinsY; y++)
    // {
    //   if ((yPosition >= hls::Histogram2DFixedNBins::_yBins[y]) && (yPosition < hls::Histogram2DFixedNBins::_yBins[y + 1]))
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
    unsigned char nBinsX, //number of bins in x
    class TYAxis, //y-axis element type
    unsigned char nBinsY //number of bins in y
  >
  void
  Histogram2DFixedNBins
  <
    TBin,
    TXAxis,
    nBinsX,
    TYAxis,
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
    unsigned char nBinsX, //number of bins in x
    class TYAxis, //y-axis element type
    unsigned char nBinsY //number of bins in y
  >
  void
  Histogram2DFixedNBins
  <
    TBin,
    TXAxis,
    nBinsX,
    TYAxis,
    nBinsY
  >::addToBin(unsigned char xIdx, unsigned char yIdx, TBin value)
  {
    #pragma HLS inline
    this -> _histogram[yIdx][xIdx] += value;
  };

  template <
    class TBin, //type of the bin
    class TXAxis, //x-axis element type
    unsigned char nBinsX, //number of bins in x
    class TYAxis, //y-axis element type
    unsigned char nBinsY //number of bins in y
  >
  void
  Histogram2DFixedNBins
  <
    TBin,
    TXAxis,
    nBinsX,
    TYAxis,
    nBinsY
  >::fill(TXAxis etaPosition, TYAxis phiPosition, TBin value)
  {
    #pragma HLS inline
    #pragma HLS pipeline
    unsigned char xIdx = this -> findXBin(etaPosition);
    unsigned char yIdx = this -> findYBin(phiPosition);
    this -> addToBin(xIdx, yIdx, value);
  };

  template <
    class TBin, //type of the bin
    class TXAxis, //x-axis element type
    unsigned char nBinsX, //number of bins in x
    class TYAxis, //y-axis element type
    unsigned char nBinsY //number of bins in y
  >
  void
  Histogram2DFixedNBins
  <
    TBin,
    TXAxis,
    nBinsX,
    TYAxis,
    nBinsY
  >::getXAxisLimits(TXAxis &xLow, TXAxis &xHigh) const
  {
    xLow = this -> _xLow;
    xHigh = this -> _xHigh;
    return;
  }
  
  template <
    class TBin, //type of the bin
    class TXAxis, //x-axis element type
    unsigned char nBinsX, //number of bins in x
    class TYAxis, //y-axis element type
    unsigned char nBinsY //number of bins in y
  >
  void
  Histogram2DFixedNBins
  <
    TBin,
    TXAxis,
    nBinsX,
    TYAxis,
    nBinsY
  >::setXAxisLimits(TXAxis xLow, TXAxis xHigh)
  {
    this -> _xLow = xLow;
    this -> _xHigh = xHigh;
    this -> _xStep = (this -> _xHigh - this -> _xLow) / nBinsX;
  }
    
  template <
    class TBin, //type of the bin
    class TXAxis, //x-axis element type
    unsigned char nBinsX, //number of bins in x
    class TYAxis, //y-axis element type
    unsigned char nBinsY //number of bins in y
  >
  void
  Histogram2DFixedNBins
  <
    TBin,
    TXAxis,
    nBinsX,
    TYAxis,
    nBinsY
  >::getYAxisLimits(TYAxis &yLow, TYAxis &yHigh) const
  {
    yLow = this -> _yLow;
    yHigh = this -> _yHigh;
    return;
  }
    
  template <
    class TBin, //type of the bin
    class TXAxis, //x-axis element type
    unsigned char nBinsX, //number of bins in x
    class TYAxis, //y-axis element type
    unsigned char nBinsY //number of bins in y
  >
  void
  Histogram2DFixedNBins
  <
    TBin,
    TXAxis,
    nBinsX,
    TYAxis,
    nBinsY
  >::setYAxisLimits(TYAxis yLow, TYAxis yHigh)
  {
    this -> _yLow = yLow;
    this -> _yHigh = yHigh;
    this -> _yStep = (this -> _yHigh - this -> _yLow) / nBinsY;
  }

}

#endif