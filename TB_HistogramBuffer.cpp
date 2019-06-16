#include "HLS_HistogramBuffer.h"
#include "TB_HistogramBuffer.h"
#include <iostream>
#include <csignal>
#include <assert.h>
#include <fstream>
#include "HLS/Histogram2D.h"
#include "HLS/Types.h"
#include "HLS/HistogramSettings.h"
#include <cstdlib>
#include <ctime>

int main(int argc, char const *argv[])
{
  // srand(time(NULL));
  
  hls::Barrel_PfInputHistogram::TBins refBarrelBins[N_ETA_SEGMENTS_BARREL];
  hls::TPt toTestBarrelBins[N_BINS_PHI_REGION][N_ETA_BINS_BARREL_REGION * N_ETA_SEGMENTS_BARREL];

  int val = 0;
  for (unsigned int regionId = 0; regionId < N_ETA_SEGMENTS_BARREL; regionId++)
  {
    for (unsigned int y = 0; y < N_BINS_PHI_REGION; y++)
    {
      for (unsigned int x = 0; x < N_ETA_BINS_BARREL_REGION; x++)
      {
        refBarrelBins[regionId][y][x] = val;
        val++;
      }
    }
  }

  for (unsigned int regionId = 0; regionId < N_ETA_SEGMENTS_BARREL; regionId++)
  {
    hls_histogram_buffer(refBarrelBins[regionId], toTestBarrelBins[0], (regionId == 0) ? true : false);
  }
  for (unsigned int y = 1; y < N_BINS_PHI_REGION; y++)
  {
    hls_histogram_buffer(refBarrelBins[0], toTestBarrelBins[y], false);
  }

  // for (unsigned int y = 0; y < N_BINS_PHI_REGION; y++)
  // {
  //   for (unsigned int regionId = 0; regionId < N_ETA_SEGMENTS_BARREL; regionId++)
  //   {
  //     for (unsigned int x = 0; x < N_ETA_BINS_BARREL_REGION; x++)
  //     {
  //       std::cout << refBarrelBins[regionId][y][x] << "  ";
  //     }
  //   }
  //   std::cout << std::endl;
  // }

  // std::cout << std::endl;
  // std::cout << "------------------------------------" << std::endl;
  // std::cout << std::endl;

  // for (unsigned int y = 0; y < N_BINS_PHI_REGION; y++)
  // {
  //   for (unsigned int x = 0; x < N_ETA_SEGMENTS_BARREL * N_ETA_BINS_BARREL_REGION; x++)
  //   {
  //     std::cout << toTestBarrelBins[y][x] << "  ";
  //   }
  //   std::cout << std::endl;
  // }

  for (unsigned int y = 0; y < N_BINS_PHI_REGION; y++)
  {
    for (unsigned int regionId = 0; regionId < N_ETA_SEGMENTS_BARREL; regionId++)
    {
      for (unsigned int x = 0; x < N_ETA_BINS_BARREL_REGION; x++)
      {
        assert(refBarrelBins[regionId][y][x] == toTestBarrelBins[y][N_ETA_BINS_BARREL_REGION * regionId + x]);
      }
    }
  }

  return 0;
}