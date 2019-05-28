#ifndef __HLS_HISTOGRAM_H__
#define __HLS_HISTOGRAM_H__

#include "ap_int.h"
#include "Types.h"
#include "HistogramSettings.h"

namespace hls
{
  //Class handling eta phi histogram
  // x=eta
  // y=phi
  class HistogramEtaPhi
  {
  private:
    TBins _histogram;
  public:
    HistogramEtaPhi();
    
    #if LUT_FILL == true
    const static TEta _xBins[ETA_GRID_SIZE + 1];
    const static TPhi _yBins[PHI_GRID_SIZE + 1];
    #endif
    // const static TEta _xBinLow;
    // const static TEta _xBinHigh;
    // const static TEta _xBinStep;
    // const static TPhi _yBinLow;
    // const static TPhi _yBinStep;
    // const static TPhi _yBinHigh;
    TBin getBin(const unsigned char x, const unsigned char y) const;
    void setBin(const unsigned char x, const unsigned char y, TBin value);
    TBin getXSize() const;
    TBin getYSize() const;
    unsigned char findXBin(TEta xPosition) const;
    unsigned char findYBin(TPhi yPosition) const;
    void addToBin(unsigned char xIdx, unsigned char yIdx, TBin value);
    void fill(TEta etaPosition, TPhi phiPosition, TBin value = 1);
    void reset(TBin value = 0);
  };
}

#endif