#ifndef __HLS_HISTOGRAM_H__
#define __HLS_HISTOGRAM_H__

#define PHI_GRID_SIZE 10
#define ETA_GRID_SIZE 10

#define PHI_BINS {0, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100}
#define ETA_BINS {0, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100}

#include "ap_int.h"


namespace hls
{
  typedef ap_uint<10> TPt;
  typedef ap_uint<10> TEta;
  typedef ap_uint<10> TPhi;
  typedef TPt TBin;
  typedef TBin TBins[PHI_GRID_SIZE][ETA_GRID_SIZE];

  //Class handling eta phi histogram
  // x=eta
  // y=phi
  class HistogramEtaPhi
  {
  private:
    TBins _histogram;
  public:
    HistogramEtaPhi();
    const static TEta _xBins[ETA_GRID_SIZE + 1];
    const static TPhi _yBins[PHI_GRID_SIZE + 1];
    TBin getBin(const unsigned char x, const unsigned char y) const;
    void setBin(const unsigned char x, const unsigned char y, TBin value);
    TBin getXSize() const;
    TBin getYSize() const;
    void fill(TEta etaPosition, TPhi phiPosition, TBin value = 1);
    void reset(TBin value = 0);
  };
}

#endif