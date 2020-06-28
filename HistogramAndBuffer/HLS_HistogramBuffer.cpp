#include "HLS_HistogramBuffer.h"
#include "HLS/Histogram2D.h"
#include "RegionOffsets.h"

#ifndef __SYNTHESIS__
#include <iostream>
#endif

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
                      hls::TPt outBins[N_BINS_ETA],
                      bool d0Valid,
                      bool & outValid
                     )
{
  #pragma HLS array_partition variable=inBarrelBins dim=0
  #pragma HLS array_partition variable=outBins dim=0
  // no valid ports for the I/O
  #pragma HLS interface ap_none port=inBarrelBins
  // no valid ports for the I/O
  #pragma HLS interface ap_none port=outBins
  // no valid ports for the I/O
  #pragma HLS interface ap_none port=d0Valid
  // no valid ports for the I/O
  #pragma HLS interface ap_none port=outValid
  // removing control bus from design
  #pragma HLS interface ap_ctrl_none port=return
  
  #pragma HLS pipeline II=1

  // internal state register
  static unsigned char sRegister = 0;
  // tracks which phi line has been output
  static unsigned char sOutputLine = 0;
  unsigned char lRegionID = 0;

  bool lReset = d0Valid;

  if (lReset) {

    sRegister = -1;
  }
  else 
  {
  // id of incoming region
    sRegister = (sRegister == (RESET_PERIOD - 1)) ? 0 : sRegister + 1;
    lRegionID = sRegister;
    // #ifndef __SYNTHESIS__
    // std::cout << "sRegister: " << +sRegister << std::endl;
    // #endif
  }

  hls::Barrel_PfInputHistogram::TBins lBarrelBins;
  #pragma HLS array_partition variable=lBarrelBins dim=0 complete
  copy2D<decltype(inBarrelBins), N_ETA_BINS_BARREL_REGION, N_BINS_PHI_REGION, decltype(lBarrelBins)>
    (inBarrelBins, lBarrelBins); 
  
  static hls::TBuffer sBuffer;

  // checking if all the regions have been received
  if (lRegionID < N_ETA_SEGMENTS_BARREL * N_PHI_SEGMENTS)
  {
    unsigned char etaOffset = returnBarrelEtaOffset(lRegionID);
    unsigned char phiOffset = returnBarrelPhiOffset(lRegionID);
    copy2DToWindow<decltype(lBarrelBins), N_ETA_BINS_BARREL_REGION, N_BINS_PHI_REGION, decltype(sBuffer)>(lBarrelBins, sBuffer, etaOffset, phiOffset);
  }

  unsigned char lOutputLine = (lRegionID < N_ETA_SEGMENTS - 1) ? 0 : lRegionID - (N_ETA_SEGMENTS - 1);

  lOutputLine = (lOutputLine >= N_BINS_PHI) ? 0 : lOutputLine;
  copyOutputLoop: for (unsigned char iEta = 0; iEta < N_BINS_ETA; iEta++)
  {
    // checking if a line of regions has been received, if so we output the first phi line of the buffer
    outBins[iEta] = (lRegionID < N_ETA_SEGMENTS - 1) ? nullPt : sBuffer.getval(lOutputLine, iEta);
  }

  outValid = d0Valid; //alignment reset

}