#ifndef __HLS_HISTOGRAM_H__
#define __HLS_HISTOGRAM_H__

#include "ap_int.h"
#include "Types.h"
#include "HistogramSettings.h"

namespace hls
{
  // Class handling eta phi histogram
  // Uniform binning only atm
  // x=eta
  // y=phi
  
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
  class Histogram2D
  {
    typedef TBin TBins[nBinsY][nBinsX];  
  private:
    TBins _histogram;
  public:
    Histogram2D();
    
    // #if LUT_FILL == true
    // const static TEta _xBins[ETA_GRID_SIZE + 1];
    // const static TPhi _yBins[PHI_GRID_SIZE + 1];
    // #endif
    TBin getBin(const unsigned char x, const unsigned char y) const;
    void setBin(const unsigned char x, const unsigned char y, TBin value);
    TBin getXSize() const;
    TBin getYSize() const;
    unsigned char findXBin(TXAxis xPosition) const;
    unsigned char findYBin(TYAxis yPosition) const;
    void addToBin(unsigned char xIdx, unsigned char yIdx, TBin value);
    void fill(TXAxis xPosition, TYAxis yPosition, TBin value = 1);
    void reset(TBin value = 0);
  };
}

#endif