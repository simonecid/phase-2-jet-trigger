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
  srand(time(NULL));
  
  hls::Barrel_PfInputHistogram::TBins refBarrelBins;
  hls::Barrel_PfInputHistogram::TBins toTestBarrelBins;

  for (unsigned int y = 0; y < PHI_N_BINS; y++)
  {
    for (unsigned int x = 0; x < BARREL_ETA_N_BINS; x++)
    {
      refBarrelBins[y][x] = rand();
    }
  }

  for (unsigned int y = 0; y < PHI_N_BINS; y++)
  {
    hls_histogram_buffer(refBarrelBins, toTestBarrelBins[y], (y == 0) ? true : false);
  }

  for (unsigned int y = 0; y < PHI_N_BINS; y++)
  {
    for (unsigned int x = 0; x < BARREL_ETA_N_BINS; x++)
    {
      std::cout << refBarrelBins[y][x] << "  ";
    }
    std::cout << std::endl;
  }

    std::cout << std::endl;
    std::cout << "------------------------------------" << std::endl;
    std::cout << std::endl;

  for (unsigned int y = 0; y < PHI_N_BINS; y++)
  {
    for (unsigned int x = 0; x < BARREL_ETA_N_BINS; x++)
    {
      std::cout << toTestBarrelBins[y][x] << "  ";
    }
    std::cout << std::endl;
  }

  for (unsigned int y = 0; y < PHI_N_BINS; y++)
  {
    for (unsigned int x = 0; x < BARREL_ETA_N_BINS; x++)
    {
      assert(refBarrelBins[y][x] == toTestBarrelBins[y][x]);
    }
  }

  return 0;
}