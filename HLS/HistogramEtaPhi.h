#ifndef __HLS_HISTOGRAM_H__
#define __HLS_HISTOGRAM_H__

#define PHI_GRID_SIZE 10
#define ETA_GRID_SIZE 10

#define PHI_BINS {0, 100, 200, 300, 400, 500, 600, 700, 800, 900, 1000, 1100, 1200, 1300, 1400, 1500, 1600, 1700, 1800, 1900, 2000, 2100, 2200, 2300, 2400, 2500, 2600, 2700, 2800, 2900, 3000, 3100, 3200, 3300, 3400, 3500, 3600, 3700, 3800, 3900, 4000, 4100, 4200, 4300, 4400, 4500, 4600, 4700, 4800, 4900, 5000, 5100, 5200, 5300, 5400, 5500, 5600, 5700, 5800, 5900, 6000, 6100, 6200, 6300, 6400, 6500, 6600, 6700, 6800, 6900, 7000, 7100, 7200}
#define ETA_BINS {0, 100, 200, 300, 400, 500, 600, 700, 800, 900, 1000, 1100, 1200, 1300, 1400, 1500, 1600, 1700, 1800, 1900, 2000, 2100, 2200, 2300, 2400, 2500, 2600, 2700, 2800, 2900, 3000, 3100, 3200, 3300, 3400, 3500, 3600, 3700, 3800, 3900, 4000, 4100, 4200, 4300, 4400, 4500, 4600, 4700, 4800, 4900, 5000, 5100, 5200, 5300, 5400, 5500, 5600, 5700, 5800, 5900, 6000, 6100, 6200, 6300, 6400, 6500, 6600, 6700, 6800, 6900, 7000, 7100, 7200, 7300, 7400, 7500, 7600, 7700, 7800, 7900, 8000, 8100, 8200, 8300, 8400, 8500, 8600, 8700, 8800, 8900, 9000, 9100, 9200, 9300, 9400, 9500, 9600}

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