#include "HLS/HistogramSettings.h"

unsigned char returnBarrelEtaOffset(unsigned char regionId)
{
  #pragma HLS inline 
  const unsigned char barrelEtaOffsets[4] = {0, 9, 18, 27};
  // const unsigned char barrelEtaOffsets[36] = {42, 51, 60, 69, 42, 51, 60, 69, 42, 51, 60, 69, 42, 51, 60, 69, 42, 51, 60, 69, 42, 51, 60, 69, 42, 51, 60, 69, 42, 51, 60, 69, 42, 51, 60, 69};
  return barrelEtaOffsets[regionId];
}

unsigned char returnBarrelPhiOffset(unsigned char &regionId)
{
  #pragma HLS inline
  const unsigned char barrelPhiOffsets[4] = {0, 0, 0, 0};
  // const unsigned char barrelPhiOffsets[36] = {0, 0, 0, 0, 8, 8, 8, 8, 16, 16, 16, 16, 24, 24, 24, 24, 32, 32, 32, 32, 40, 40, 40, 40, 48, 48, 48, 48, 56, 56, 56, 56, 64, 64, 64, 64};
  return barrelPhiOffsets[regionId];
}

unsigned char returnTKHGEtaOffset(unsigned char regionId)
{
  #pragma HLS inline
  const unsigned char tkhgEtaOffsets[18] = {36, 78, 36, 78, 36, 78, 36, 78, 36, 78, 36, 78, 36, 78, 36, 78, 36, 78};
  return tkhgEtaOffsets[regionId];
}

unsigned char returnTKHGPhiOffset(unsigned char regionId)
{
  #pragma HLS inline
  const unsigned char tkhgPhiOffsets[18] = {0, 0, 8, 8, 16, 16, 24, 24, 32, 32, 40, 40, 48, 48, 56, 56, 64, 64};
  return tkhgPhiOffsets[regionId];
}

unsigned char returnHGEtaOffset(unsigned char regionId)
{
  #pragma HLS inline
  const unsigned char hgEtaOffset[18] = {24, 90, 24, 90, 24, 90, 24, 90, 24, 90, 24, 90, 24, 90, 24, 90, 24, 90};
  return hgEtaOffset[regionId];
}

unsigned char returnHGPhiOffset(unsigned char regionId)
{
  #pragma HLS inline
  const unsigned char hgPhiOffset[18] = {0, 0, 8, 8, 16, 16, 24, 24, 32, 32, 40, 40, 48, 48, 56, 56, 64, 64};
  return hgPhiOffset[regionId];
}

unsigned char returnHFEtaOffset(unsigned char regionId)
{
  #pragma HLS inline
  const unsigned char hfEtaOffset[72] = {0, 6, 12, 18, 96, 102, 108, 114, 0, 6, 12, 18, 96, 102, 108, 114, 0, 6, 12, 18, 96, 102, 108, 114, 0, 6, 12, 18, 96, 102, 108, 114, 0, 6, 12, 18, 96, 102, 108, 114, 0, 6, 12, 18, 96, 102, 108, 114, 0, 6, 12, 18, 96, 102, 108, 114, 0, 6, 12, 18, 96, 102, 108, 114, 0, 6, 12, 18, 96, 102, 108, 114};
  return hfEtaOffset[regionId];
}

unsigned char returnHFPhiOffset(unsigned char regionId)
{
  #pragma HLS inline
  const unsigned char hfPhiOffset[72] = {0, 0, 0, 0, 0, 0, 0, 0, 8, 8, 8, 8, 8, 8, 8, 8, 16, 16, 16, 16, 16, 16, 16, 16, 24, 24, 24, 24, 24, 24, 24, 24, 32, 32, 32, 32, 32, 32, 32, 32, 40, 40, 40, 40, 40, 40, 40, 40, 48, 48, 48, 48, 48, 48, 48, 48, 56, 56, 56, 56, 56, 56, 56, 56, 64, 64, 64, 64, 64, 64, 64, 64};
  return hfPhiOffset[regionId];
}