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
                      const hls::TK_HG_PfInputHistogram::TBins inTKHGBins,
                      const hls::HG_PfInputHistogram::TBins inHGBins,
                      const hls::HF_PfInputHistogram::TBins inHFBins,
                      hls::TPt outBins[N_ETA_BINS_BARREL_REGION * N_ETA_SEGMENTS_BARREL],
                      bool reset
                     )
{
  #pragma HLS array_partition variable=inBarrelBins dim=0
  #pragma HLS array_partition variable=inTKHGBins dim=0
  #pragma HLS array_partition variable=inHGBins dim=0
  #pragma HLS array_partition variable=inHFBins dim=0
  #pragma HLS array_partition variable=outBins dim=0
  
  #pragma HLS pipeline 

  bool lReset = reset;

  hls::Barrel_PfInputHistogram::TBins lBarrelBins;
  #pragma HLS array_partition variable=lBarrelBins dim=0 complete
  copy2D<decltype(inBarrelBins), N_ETA_BINS_BARREL_REGION, N_BINS_PHI_REGION, decltype(lBarrelBins)>
    (inBarrelBins, lBarrelBins); 
  hls::TK_HG_PfInputHistogram::TBins lTKHGBins;
  #pragma HLS array_partition variable=lTKHGBins dim=0
  copy2D<decltype(inTKHGBins), N_ETA_BINS_TK_HG_REGION, N_BINS_PHI_REGION, decltype(lTKHGBins)>(inTKHGBins, lTKHGBins);
  hls::HG_PfInputHistogram::TBins lHGBins;
  #pragma HLS array_partition variable=lHGBins dim=0
  copy2D<decltype(inHGBins), N_ETA_BINS_HG_REGION, N_BINS_PHI_REGION, decltype(lHGBins)>(inHGBins, lHGBins);
  hls::HF_PfInputHistogram::TBins lHFBins;
  #pragma HLS array_partition variable=lHFBins dim=0
  copy2D<decltype(inHFBins), N_ETA_BINS_HF_REGION, N_BINS_PHI_REGION, decltype(lHFBins)>(inHFBins, lHFBins);

  
  // counts how many regions have been received
  static unsigned char sNumberOfRegionsReceived = 0;
  // tracks which phi line has been output
  static unsigned char sOutputLine = 0;
  unsigned char lNumberOfRegionsReceived = (lReset) ? 0 : sNumberOfRegionsReceived;
  unsigned char lOutputLine = (lReset) ? 0 : sOutputLine;
  sNumberOfRegionsReceived = (lReset) ? 1 : sNumberOfRegionsReceived + 1;


  static TBuffer sBuffer;

  // checking if all the regions have been received
  if (lNumberOfRegionsReceived < N_ETA_SEGMENTS_BARREL * N_PHI_SEGMENTS)
  {
    unsigned char etaOffset = returnBarrelEtaOffset(lNumberOfRegionsReceived) - 42;
    // unsigned char etaOffset = returnBarrelEtaOffset(lNumberOfRegionsReceived);
    unsigned char phiOffset = returnBarrelPhiOffset(lNumberOfRegionsReceived);
    copy2DToWindow<decltype(lBarrelBins), N_ETA_BINS_BARREL_REGION, N_BINS_PHI_REGION, decltype(sBuffer)>(lBarrelBins, sBuffer, etaOffset, phiOffset);
  }
  if (lNumberOfRegionsReceived < N_ETA_SEGMENTS_TK_HG * N_PHI_SEGMENTS)
  {
    unsigned char etaOffset = returnTKHGEtaOffset(lNumberOfRegionsReceived);
    unsigned char phiOffset = returnTKHGPhiOffset(lNumberOfRegionsReceived);
    copy2DToWindow<decltype(lTKHGBins), N_ETA_BINS_TK_HG_REGION, N_BINS_PHI_REGION, decltype(sBuffer)>(lTKHGBins, sBuffer, etaOffset, phiOffset);
  }
  if (lNumberOfRegionsReceived < N_ETA_SEGMENTS_HG * N_PHI_SEGMENTS)
  {
    unsigned char etaOffset = returnHGEtaOffset(lNumberOfRegionsReceived);
    unsigned char phiOffset = returnHGPhiOffset(lNumberOfRegionsReceived);
    copy2DToWindow<decltype(lHGBins), N_ETA_BINS_HG_REGION, N_BINS_PHI_REGION, decltype(sBuffer)>(lHGBins, sBuffer, etaOffset, phiOffset);
  }
  if (lNumberOfRegionsReceived < N_ETA_SEGMENTS_HF * N_PHI_SEGMENTS)
  {
    unsigned char etaOffset = returnHFEtaOffset(lNumberOfRegionsReceived);
    unsigned char phiOffset = returnHFPhiOffset(lNumberOfRegionsReceived);
    copy2DToWindow<decltype(lHFBins), N_ETA_BINS_HF_REGION, N_BINS_PHI_REGION, decltype(sBuffer)>(lHFBins, sBuffer, etaOffset, phiOffset);
  }
  // }

  copyOutputLoop: for (unsigned char iEta = 0; iEta < N_ETA_BINS_BARREL_REGION * N_ETA_SEGMENTS_BARREL; iEta++)
  {
    // checking if a line of regions has been received, if so we output the first phi line of the buffer
    outBins[iEta] = (lNumberOfRegionsReceived < N_ETA_SEGMENTS - 1) ? nullPt : sBuffer.getval(lOutputLine, iEta);
  }

  // checking if a line of regions has been received, if not, surely the counter should stay to 0
  // if yes, we have returned one in the line before, and I can increase this counter safely
  unsigned char lNextOutputLine = (lNumberOfRegionsReceived < N_ETA_SEGMENTS - 1) ? 0 : sOutputLine + 1;
  // updating the line register if we haven't reached the end of memory yet
  sOutputLine = (sOutputLine == N_BINS_PHI_REGION * N_PHI_SEGMENTS - 1) ? sOutputLine : lNextOutputLine;

}