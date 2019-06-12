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
  // #pragma HLS inline
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

void hls_histogram_buffer(
                      const hls::Barrel_PfInputHistogram::TBins inBarrelBins,
                      const hls::TK_HG_PfInputHistogram::TBins inTKHGBins,
                      const hls::HG_PfInputHistogram::TBins inHGBins,
                      const hls::HF_PfInputHistogram::TBins inHFBins,
                      hls::TPt outBins[ETA_GRID_SIZE],
                      bool reset
                     )
{
  #pragma HLS array_partition variable=inBarrelBins dim=0
  #pragma HLS array_partition variable=inTKHGBins dim=0
  #pragma HLS array_partition variable=inHGBins dim=0
  #pragma HLS array_partition variable=inHFBins dim=0
  #pragma HLS array_partition variable=outBins dim=0
  
  #pragma HLS pipeline II=2

  //local buffers

  hls::Barrel_PfInputHistogram::TBins lBarrelBins;
  #pragma HLS array_partition variable=lBarrelBins dim=0
  copy2D<decltype(inBarrelBins), BARREL_ETA_N_BINS, PHI_N_BINS, decltype(lBarrelBins)>(inBarrelBins, lBarrelBins);
  hls::TK_HG_PfInputHistogram::TBins lTKHGBins;
  #pragma HLS array_partition variable=lTKHGBins dim=0
  copy2D<decltype(inTKHGBins), TK_HG_ETA_N_BINS, PHI_N_BINS, decltype(lTKHGBins)>(inTKHGBins, lTKHGBins);
  hls::HG_PfInputHistogram::TBins lHGBins;
  #pragma HLS array_partition variable=lHGBins dim=0
  copy2D<decltype(inHGBins), HG_ETA_N_BINS, PHI_N_BINS, decltype(lHGBins)>(inHGBins, lHGBins);
  hls::HF_PfInputHistogram::TBins lHFBins;
  #pragma HLS array_partition variable=lHFBins dim=0
  copy2D<decltype(inHFBins), HF_ETA_N_BINS, PHI_N_BINS, decltype(lHFBins)>(inHFBins, lHFBins);
  
  static unsigned char regionId = 0;
  unsigned char lRegionId = (reset) ? 0 : regionId;
  // resetting 
  regionId = ((regionId == PHI_N_BINS - 1) || (reset)) ? 0 : regionId + 1;


  static TBuffer sBuffer;
  #pragma HLS array_partition variable=sBuffer dim=2 complete
  // #pragma HLS array_partition variable=sBuffer dim=1 block factor=2
  // #pragma HLS resource variable=sBuffer core=RAM_2P_BRAM

  regionCopy: for (unsigned lRegionIndex = 0; lRegionIndex < 72; lRegionIndex++)
  {
    if ((lRegionId < 36) && (lRegionIndex == lRegionId))
    {
      unsigned char etaOffset = returnBarrelEtaOffset(lRegionId);
      unsigned char phiOffset = returnBarrelPhiOffset(lRegionId);
      copy2D<decltype(lBarrelBins), BARREL_ETA_N_BINS, PHI_N_BINS, decltype(sBuffer)>(lBarrelBins, sBuffer, etaOffset, phiOffset);
      // copy2D<decltype(inBarrelBins), BARREL_ETA_N_BINS, PHI_N_BINS, decltype(sBuffer)>(inBarrelBins, sBuffer, etaOffset, phiOffset);
    }
    if ((lRegionId < 18) && (lRegionIndex == lRegionId))
    {
      unsigned char etaOffset = returnTKHGEtaOffset(lRegionId);
      unsigned char phiOffset = returnTKHGPhiOffset(lRegionId);
      copy2D<decltype(lTKHGBins), TK_HG_ETA_N_BINS, PHI_N_BINS, decltype(sBuffer)>(lTKHGBins, sBuffer, etaOffset, phiOffset);
      // copy2D<decltype(inTKHGBins), TK_HG_ETA_N_BINS, PHI_N_BINS, decltype(sBuffer)>(inTKHGBins, sBuffer, etaOffset, phiOffset);
    }
    if ((lRegionId < 18) && (lRegionIndex == lRegionId))
    {
      unsigned char etaOffset = returnHGEtaOffset(lRegionId);
      unsigned char phiOffset = returnHGPhiOffset(lRegionId);
      copy2D<decltype(lHGBins), HG_ETA_N_BINS, PHI_N_BINS, decltype(sBuffer)>(lHGBins, sBuffer, etaOffset, phiOffset);
      // copy2D<decltype(inHGBins), HG_ETA_N_BINS, PHI_N_BINS, decltype(sBuffer)>(inHGBins, sBuffer, etaOffset, phiOffset);
    }
    if ((lRegionId < 72) && (lRegionIndex == lRegionId))
    {
      unsigned char etaOffset = returnHFEtaOffset(lRegionId);
      unsigned char phiOffset = returnHFPhiOffset(lRegionId);
      copy2D<decltype(lHFBins), HF_ETA_N_BINS, PHI_N_BINS, decltype(sBuffer)>(lHFBins, sBuffer, etaOffset, phiOffset);
      // copy2D<decltype(inHFBins), HF_ETA_N_BINS, PHI_N_BINS, decltype(sBuffer)>(inHFBins, sBuffer, etaOffset, phiOffset);
    }
  }

  copyOutputLoop: for (unsigned char iEta = 0; iEta < ETA_GRID_SIZE; iEta++)
  {
    lRegionId = (lRegionId + 8) % 8;
    outBins[iEta] = sBuffer[regionId][iEta];
  }

}