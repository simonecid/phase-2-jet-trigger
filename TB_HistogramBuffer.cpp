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
  
  hls::Barrel_PfInputHistogram::TBins refBarrelBins[RESET_PERIOD];
  hls::TPt toTestBarrelBins[RESET_PERIOD][N_ETA_BINS_BARREL_REGION * N_ETA_SEGMENTS_BARREL];

  int val = 0;
  for (unsigned int y = 0; y < N_BINS_PHI_REGION; y++)
  {
    for (unsigned int regionId = 0; regionId < N_ETA_SEGMENTS_BARREL; regionId++)
    {
      for (unsigned int x = 0; x < N_ETA_BINS_BARREL_REGION; x++)
      {
        refBarrelBins[regionId][y][x] = val;
        val++;
      }
    }
  }

  bool d0Valid, outReset;

  d0Valid = true;

  hls_histogram_buffer(refBarrelBins[0], toTestBarrelBins[0], d0Valid, outReset);

  d0Valid = false;

  for (unsigned int regionId = 0; regionId < RESET_PERIOD; regionId++)
  {
    hls_histogram_buffer(refBarrelBins[regionId], toTestBarrelBins[regionId], d0Valid, outReset);
  }

  for (unsigned int y = 0; y < N_BINS_PHI_REGION; y++)
  {
    for (unsigned int regionId = 0; regionId < N_ETA_SEGMENTS_BARREL + 0; regionId++)
    {
      for (unsigned int x = 0; x < N_ETA_BINS_BARREL_REGION; x++)
      {
        assert(refBarrelBins[regionId][y][x] == toTestBarrelBins[y + N_ETA_SEGMENTS_BARREL - 1][N_ETA_BINS_BARREL_REGION * regionId + x]);
      }
    }
  }

  for (unsigned int y = 0; y < N_BINS_PHI_REGION; y++)
  {
    for (unsigned int regionId = 0; regionId < N_ETA_SEGMENTS_BARREL; regionId++)
    {
      for (unsigned int x = 0; x < N_ETA_BINS_BARREL_REGION; x++)
      {
        std::cout << refBarrelBins[regionId][y][x] << "  ";
      }
    }
    std::cout << std::endl;
  }

  std::cout << std::endl;
  std::cout << "------------------------------------" << std::endl;
  std::cout << std::endl;

  for (unsigned int y = 1; y < N_BINS_PHI_REGION + 1; y++)
  {
    for (unsigned int x = 0; x < N_ETA_SEGMENTS_BARREL * N_ETA_BINS_BARREL_REGION; x++)
    {
      std::cout << toTestBarrelBins[y][x] << "  ";
    }
    std::cout << std::endl;
  }

  //testing periodic reset
  std::cout << "Sending a new event" << std::endl;
  
  for (unsigned int regionId = 0; regionId < RESET_PERIOD; regionId++)
  {
    hls_histogram_buffer(refBarrelBins[regionId], toTestBarrelBins[regionId], d0Valid, outReset);
  }

  for (unsigned int y = 0; y < N_BINS_PHI_REGION; y++)
  {
    for (unsigned int regionId = 0; regionId < N_ETA_SEGMENTS_BARREL + 0; regionId++)
    {
      for (unsigned int x = 0; x < N_ETA_BINS_BARREL_REGION; x++)
      {
        assert(refBarrelBins[regionId][y][x] == toTestBarrelBins[y + N_ETA_SEGMENTS_BARREL - 1][N_ETA_BINS_BARREL_REGION * regionId + x]);
      }
    }
  }

  for (unsigned int y = 0; y < N_BINS_PHI_REGION; y++)
  {
    for (unsigned int regionId = 0; regionId < N_ETA_SEGMENTS_BARREL; regionId++)
    {
      for (unsigned int x = 0; x < N_ETA_BINS_BARREL_REGION; x++)
      {
        std::cout << refBarrelBins[regionId][y][x] << "  ";
      }
    }
    std::cout << std::endl;
  }

  std::cout << std::endl;
  std::cout << "------------------------------------" << std::endl;
  std::cout << std::endl;

  for (unsigned int y = 1; y < N_BINS_PHI_REGION + 1; y++)
  {
    for (unsigned int x = 0; x < N_ETA_SEGMENTS_BARREL * N_ETA_BINS_BARREL_REGION; x++)
    {
      std::cout << toTestBarrelBins[y][x] << "  ";
    }
    std::cout << std::endl;
  }

  return 0;
}