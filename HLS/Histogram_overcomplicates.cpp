#ifndef __HLS_HISTOGRAM_H__
#define __HLS_HISTOGRAM_H__

namespace hls
{
  template <typename TBin, typename XBinType, typename YBinType,
            XBinType TXBinType, unsigned char x_size, TXBinType xBins[x_size+1],
            YBinType TYBinType, unsigned char y_size>
  class Histogram2D
  {
  private:
    TBin _histogram[y_size][x_size];
    #pragma HLS partition variable=_histogram complete dim=0
  public:
    Histogram();
    TBin getBin(unsigned char x, unsigned char y) const 
    {
      #pragma HLS inline
      return this -> _histogram[y][x];
    };
    void setBin(TBin value, unsigned char x, unsigned char y)
    {
      #pragma HLS inline
      this -> _histogram[y][x] = value;
      return;
    };
    TBin getXSize() const
    {
      #pragma HLS inline
      return x_size;
    };
    TBin getYSize() const
    {
      #pragma HLS inline
      return y_size;
    };

    void reset(TBin value = 0)
    {
      #pragma HLS pipeline
      for (unsigned char x = 0; x < x_size; x++)
      {
        for (unsigned char y = 0; y < y_size; y++)
        {
          this -> _histogram[y][x] = value;
        }
      }
    }; 
  };
}

#endif