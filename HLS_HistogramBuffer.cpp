#include "HLS_HistogramBuffer.h"
#include "HLS/Histogram2D.h"
#include "RegionOffsets.h"

template <
  class TInputData,
  unsigned char inXSize,
  unsigned char inYSize,
  class TOutputData
>
void copy2D (const TInputData bins, TOutputData buffer, const unsigned char xOffset = 0, const unsigned char yOffset = 0)
{
  #pragma HLS inline
  #pragma HLS pipeline
  copy2DLoop1: for (unsigned char y = 0; y < inYSize; y++)
  {
    #pragma HLS unroll
    copy2DLoop2: for (unsigned char x = 0; x < inXSize; x++)
    {
      #pragma HLS unroll
      const unsigned char lY = y + yOffset;
      const unsigned char lX = x + xOffset;
      buffer[lY][lX] = bins[y][x];
    }
  }
}

template <
  class TInputData,
  unsigned char inXSize,
  unsigned char inYSize,
  class hlsWindow
>
void copy2DToWindow (const TInputData bins, hlsWindow & buffer, const unsigned char xOffset = 0, const unsigned char yOffset = 0)
{
  #pragma HLS inline
  #pragma HLS pipeline
  copy2DLoop1: for (unsigned char y = 0; y < inYSize; y++)
  {
    #pragma HLS unroll
    copy2DLoop2: for (unsigned char x = 0; x < inXSize; x++)
    {
      #pragma HLS unroll
      const unsigned char lY = y + yOffset;
      const unsigned char lX = x + xOffset;
      // buffer[lY][lX] = bins[y][x];
      buffer.insert_pixel(bins[y][x], lY, lX);
    }
  }
}

const hls::TPt nullPt = 0;

void hls_histogram_buffer(
                      const hls::Barrel_PfInputHistogram::TBins inBarrelBins,
                      hls::TPt outBins[N_ETA_BINS_BARREL_REGION * N_ETA_SEGMENTS_BARREL],
                      bool reset
                     )
{
  #pragma HLS array_partition variable=inBarrelBins dim=0
  #pragma HLS array_partition variable=outBins dim=0
  
  #pragma HLS pipeline 

  bool lReset = reset;

  hls::Barrel_PfInputHistogram::TBins lBarrelBins;
  #pragma HLS array_partition variable=lBarrelBins dim=0 complete
  // #pragma HLS array_partition variable=lBarrelBins dim=1 complete
  
  // counts how many regions have been received
  static unsigned char sNumberOfRegionsReceived = 0;
  // tracks which phi line has been output
  static unsigned char sOutputLine = 0;
  unsigned char lNumberOfRegionsReceived = (lReset) ? 0 : sNumberOfRegionsReceived;
  unsigned char lOutputLine = (lReset) ? 0 : sOutputLine;
  sNumberOfRegionsReceived = (lReset) ? 0 : sNumberOfRegionsReceived + 1;

  copy2D<decltype(inBarrelBins), N_ETA_BINS_BARREL_REGION, N_BINS_PHI_REGION, decltype(lBarrelBins)>
    (inBarrelBins, lBarrelBins); 

  static TBarrelBuffer sBuffer;
  // #pragma HLS array_partition variable=sBuffer dim=0 complete
  // #pragma HLS array_partition variable=sBuffer dim=2 complete
  // #pragma HLS array_partition variable=sBuffer dim=1 block factor=2
  // #pragma HLS resource variable=sBuffer core=RAM_2P_BRAM

  // for (unsigned char lIndex = 0; lIndex < 4; lIndex ++)
  // {

  // checking if all the regions have been received
  if (lNumberOfRegionsReceived < N_ETA_SEGMENTS_BARREL * N_PHI_SEGMENTS)
  {
    // unsigned char etaOffset = returnBarrelEtaOffset(lRegionId);
    unsigned char etaOffset = returnBarrelEtaOffset(lNumberOfRegionsReceived);
    // unsigned char phiOffset = returnBarrelPhiOffset(lRegionId);
    unsigned char phiOffset = returnBarrelPhiOffset(lNumberOfRegionsReceived);
    // copy2D<decltype(lBarrelBins), N_ETA_BINS_BARREL_REGION, N_BINS_PHI_REGION, decltype(sBuffer)>(lBarrelBins, sBuffer, etaOffset, phiOffset);
    copy2DToWindow<decltype(inBarrelBins), N_ETA_BINS_BARREL_REGION, N_BINS_PHI_REGION, decltype(sBuffer)>(inBarrelBins, sBuffer, etaOffset, phiOffset);
  }
  // }

  copyOutputLoop: for (unsigned char iEta = 0; iEta < N_ETA_BINS_BARREL_REGION * N_ETA_SEGMENTS_BARREL; iEta++)
  {
    // checking if a line of regions has been received, if so we output the first phi line of the buffer
    outBins[iEta] = (lNumberOfRegionsReceived < N_ETA_SEGMENTS_BARREL) ? nullPt : sBuffer.getval(lOutputLine, iEta);
     
    // outBins[iEta] = sBuffer[lTimer][iEta];
  }

  // checking if a line of regions has been received, if not, surely the counter should stay to 0
  // if yes, we have returned one in the line before, and I can increase this counter safely
  sOutputLine = (lNumberOfRegionsReceived < N_ETA_SEGMENTS_BARREL) ? 0 : sOutputLine + 1;

}