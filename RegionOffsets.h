#ifndef __REGION_OFFSET_H__
#define __REGION_OFFSET_H__

#include "HLS/HistogramSettings.h"

unsigned char returnBarrelEtaOffset(unsigned char regionId)
{
  #pragma HLS inline 
  const unsigned char barrelEtaOffsets[4] = {0, 9, 18, 27};
  return barrelEtaOffsets[regionId];

  // switch (regionId)
  // {
  //   case 0: return 0; break;
  //   case 1: return 9; break;
  //   case 2: return 18; break;
  //   case 3: return 27; break;
  //   case 0: return 42; break;
  //   case 1: return 51; break;
  //   case 2: return 60; break;
  //   case 3: return 69; break;
  //   case 4: return 42; break;
  //   case 5: return 51; break;
  //   case 6: return 60; break;
  //   case 7: return 69; break;
  //   case 8: return 42; break;
  //   case 9: return 51; break;
  //   case 10: return 60; break;
  //   case 11: return 69; break;
  //   case 12: return 42; break;
  //   case 13: return 51; break;
  //   case 14: return 60; break;
  //   case 15: return 69; break;
  //   case 16: return 42; break;
  //   case 17: return 51; break;
  //   case 18: return 60; break;
  //   case 19: return 69; break;
  //   case 20: return 42; break
  //   case 21: return 51; break;
  //   case 22: return 60; break;
  //   case 23: return 69; break;
  //   case 24: return 42; break;
  //   case 25: return 51; break;
  //   case 26: return 60; break;
  //   case 27: return 69; break;
  //   case 28: return 42; break;
  //   case 29: return 51; break;
  //   case 30: return 60; break;
  //   case 31: return 69; break;
  //   case 32: return 42; break;
  //   case 33: return 51; break;
  //   case 34: return 60; break;
  //   case 35: return 69; break;
  //   default: return 0; break;
  // }
}

unsigned char returnBarrelPhiOffset(unsigned char &regionId)
{
  #pragma HLS inline
  const unsigned char barrelPhiOffsets[4] = {0, 0, 0, 0};
  return barrelPhiOffsets[regionId];

  // switch (regionId)
  // {
  //   case 0: return 0; break;
  //   case 1: return 0; break;
  //   case 2: return 0; break;
  //   case 3: return 0; break;
  //   case 4: return 8; break;
  //   case 5: return 8; break;
  //   case 6: return 8; break;
  //   case 7: return 8; break;
  //   case 8: return 16; break;
  //   case 9: return 16; break;
  //   case 10: return 16; break;
  //   case 11: return 16; break;
  //   case 12: return 24; break;
  //   case 13: return 24; break;
  //   case 14: return 24; break;
  //   case 15: return 24; break;
  //   case 16: return 32; break;
  //   case 17: return 32; break;
  //   case 18: return 32; break;
  //   case 19: return 32; break;
  //   case 20: return 40; break;
  //   case 21: return 40; break;
  //   case 22: return 40; break;
  //   case 23: return 40; break;
  //   case 24: return 48; break;
  //   case 25: return 48; break;
  //   case 26: return 48; break;
  //   case 27: return 48; break;
  //   case 28: return 56; break;
  //   case 29: return 56; break;
  //   case 30: return 56; break;
  //   case 31: return 56; break;
  //   case 32: return 64; break;
  //   case 33: return 64; break;
  //   case 34: return 64; break;
  //   case 35: return 64; break;
  //   default: return 0; break;
  // }
}

unsigned char returnTKHGEtaOffset(unsigned char regionId)
{
  #pragma HLS inline off
  switch (regionId)
  {
    case 0: return 36; break;
    case 1: return 78; break;
    case 2: return 36; break;
    case 3: return 78; break;
    case 4: return 36; break;
    case 5: return 78; break;
    case 6: return 36; break;
    case 7: return 78; break;
    case 8: return 36; break;
    case 9: return 78; break;
    case 10: return 36; break;
    case 11: return 78; break;
    case 12: return 36; break;
    case 13: return 78; break;
    case 14: return 36; break;
    case 15: return 78; break;
    case 16: return 36; break;
    case 17: return 78; break;
    // default: return 0; break;
  }
}

unsigned char returnTKHGPhiOffset(unsigned char regionId)
{
  #pragma HLS inline off
  switch (regionId)
  {
    case 0: return 0; break;
    case 1: return 0; break;
    case 2: return 8; break;
    case 3: return 8; break;
    case 4: return 16; break;
    case 5: return 16; break;
    case 6: return 24; break;
    case 7: return 24; break;
    case 8: return 32; break;
    case 9: return 32; break;
    case 10: return 40; break;
    case 11: return 40; break;
    case 12: return 48; break;
    case 13: return 48; break;
    case 14: return 56; break;
    case 15: return 56; break;
    case 16: return 64; break;
    case 17: return 64; break;
    // default: return 0; break;
  }
}

unsigned char returnHGEtaOffset(unsigned char regionId)
{
  #pragma HLS inline off
  switch (regionId)
  {
    case 0: return 24; break;
    case 1: return 90; break;
    case 2: return 24; break;
    case 3: return 90; break;
    case 4: return 24; break;
    case 5: return 90; break;
    case 6: return 24; break;
    case 7: return 90; break;
    case 8: return 24; break;
    case 9: return 90; break;
    case 10: return 24; break;
    case 11: return 90; break;
    case 12: return 24; break;
    case 13: return 90; break;
    case 14: return 24; break;
    case 15: return 90; break;
    case 16: return 24; break;
    case 17: return 90; break;
    // default: return 0; break;
  }
}

unsigned char returnHGPhiOffset(unsigned char regionId)
{
  #pragma HLS inline off
  switch (regionId)
  {
    case 0: return 0; break;
    case 1: return 0; break;
    case 2: return 8; break;
    case 3: return 8; break;
    case 4: return 16; break;
    case 5: return 16; break;
    case 6: return 24; break;
    case 7: return 24; break;
    case 8: return 32; break;
    case 9: return 32; break;
    case 10: return 40; break;
    case 11: return 40; break;
    case 12: return 48; break;
    case 13: return 48; break;
    case 14: return 56; break;
    case 15: return 56; break;
    case 16: return 64; break;
    case 17: return 64; break;
    // default: return 0; break;
  }
}

unsigned char returnHFEtaOffset(unsigned char regionId)
{
  #pragma HLS inline off
  switch (regionId)
  {
    case 0: return 0 ; break;
    case 1: return 6 ; break;
    case 2: return 12 ; break;
    case 3: return 18 ; break;
    case 4: return 96 ; break;
    case 5: return 102 ; break;
    case 6: return 108 ; break;
    case 7: return 114 ; break;
    case 8: return 0 ; break;
    case 9: return 6 ; break;
    case 10: return 12 ; break;
    case 11: return 18 ; break;
    case 12: return 96 ; break;
    case 13: return 102 ; break;
    case 14: return 108 ; break;
    case 15: return 114 ; break;
    case 16: return 0 ; break;
    case 17: return 6 ; break;
    case 18: return 12 ; break;
    case 19: return 18 ; break;
    case 20: return 96 ; break;
    case 21: return 102 ; break;
    case 22: return 108 ; break;
    case 23: return 114 ; break;
    case 24: return 0 ; break;
    case 25: return 6 ; break;
    case 26: return 12 ; break;
    case 27: return 18 ; break;
    case 28: return 96 ; break;
    case 29: return 102 ; break;
    case 30: return 108 ; break;
    case 31: return 114 ; break;
    case 32: return 0 ; break;
    case 33: return 6 ; break;
    case 34: return 12 ; break;
    case 35: return 18 ; break;
    case 36: return 96 ; break;
    case 37: return 102 ; break;
    case 38: return 108 ; break;
    case 39: return 114 ; break;
    case 40: return 0 ; break;
    case 41: return 6 ; break;
    case 42: return 12 ; break;
    case 43: return 18 ; break;
    case 44: return 96 ; break;
    case 45: return 102 ; break;
    case 46: return 108 ; break;
    case 47: return 114 ; break;
    case 48: return 0 ; break;
    case 49: return 6 ; break;
    case 50: return 12 ; break;
    case 51: return 18 ; break;
    case 52: return 96 ; break;
    case 53: return 102 ; break;
    case 54: return 108 ; break;
    case 55: return 114 ; break;
    case 56: return 0 ; break;
    case 57: return 6 ; break;
    case 58: return 12 ; break;
    case 59: return 18 ; break;
    case 60: return 96 ; break;
    case 61: return 102 ; break;
    case 62: return 108 ; break;
    case 63: return 114 ; break;
    case 64: return 0 ; break;
    case 65: return 6 ; break;
    case 66: return 12 ; break;
    case 67: return 18 ; break;
    case 68: return 96 ; break;
    case 69: return 102 ; break;
    case 70: return 108 ; break;
    case 71: return 114 ; break;
    // default: return 0; break;
  }
}

unsigned char returnHFPhiOffset(unsigned char regionId)
{
  #pragma HLS inline off
  switch (regionId)
  {
    case 0: return 0; break;
    case 1: return 0; break;
    case 2: return 0; break;
    case 3: return 0; break;
    case 4: return 0; break;
    case 5: return 0; break;
    case 6: return 0; break;
    case 7: return 0; break;
    case 8: return 8; break;
    case 9: return 8; break;
    case 10: return 8; break;
    case 11: return 8; break;
    case 12: return 8; break;
    case 13: return 8; break;
    case 14: return 8; break;
    case 15: return 8; break;
    case 16: return 16; break;
    case 17: return 16; break;
    case 18: return 16; break;
    case 19: return 16; break;
    case 20: return 16; break;
    case 21: return 16; break;
    case 22: return 16; break;
    case 23: return 16; break;
    case 24: return 24; break;
    case 25: return 24; break;
    case 26: return 24; break;
    case 27: return 24; break;
    case 28: return 24; break;
    case 29: return 24; break;
    case 30: return 24; break;
    case 31: return 24; break;
    case 32: return 32; break;
    case 33: return 32; break;
    case 34: return 32; break;
    case 35: return 32; break;
    case 36: return 32; break;
    case 37: return 32; break;
    case 38: return 32; break;
    case 39: return 32; break;
    case 40: return 40; break;
    case 41: return 40; break;
    case 42: return 40; break;
    case 43: return 40; break;
    case 44: return 40; break;
    case 45: return 40; break;
    case 46: return 40; break;
    case 47: return 40; break;
    case 48: return 48; break;
    case 49: return 48; break;
    case 50: return 48; break;
    case 51: return 48; break;
    case 52: return 48; break;
    case 53: return 48; break;
    case 54: return 48; break;
    case 55: return 48; break;
    case 56: return 56; break;
    case 57: return 56; break;
    case 58: return 56; break;
    case 59: return 56; break;
    case 60: return 56; break;
    case 61: return 56; break;
    case 62: return 56; break;
    case 63: return 56; break;
    case 64: return 64; break;
    case 65: return 64; break;
    case 66: return 64; break;
    case 67: return 64; break;
    case 68: return 64; break;
    case 69: return 64; break;
    case 70: return 64; break;
    case 71: return 64; break;
    // default: return 0; break;
  }
}

void returnNextRegionOffset(unsigned char &etaOffset, unsigned char &phiOffset, bool reset)
{
  // #pragma HLS inline off
  // return (regionId * N_ETA_BINS_BARREL_REGION) % N_ETA_SEGMENTS_BARREL;
  // if we have overflown with the eta pointer, then move phi up and restore eta to 0
  phiOffset += (etaOffset == (N_ETA_SEGMENTS_BARREL - 1) * N_ETA_BINS_BARREL_REGION) ? phiOffset + N_BINS_PHI_REGION : phiOffset;
  etaOffset = (etaOffset == (N_ETA_SEGMENTS_BARREL - 1) * N_ETA_BINS_BARREL_REGION) ? 0 : etaOffset + N_ETA_BINS_BARREL_REGION;
  phiOffset = (reset) ? 0 : phiOffset;
  etaOffset = (reset) ? 0 : etaOffset;
  return;
}

#endif