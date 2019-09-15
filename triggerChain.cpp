#include "JetClustering/HLS_Phase1Clustering.h"
#include "HistogramAndBuffer/HLS_Histogrammer.h"
#include "HistogramAndBuffer/HLS_HistogramBuffer.h"
#include "HistogramAndBuffer/HLS/Types.h"

#include <assert.h>
#include <vector>

#define ITERATIONS RESET_PERIOD*3

template<class TBinSlice>
void printSlice(TBinSlice binSlice)
{
  for (unsigned int x = 0; x < N_ETA_BINS_BARREL_REGION; x++)
  {
    std::cout << binSlice[x] << " " ;
  }
  std::cout << std::endl;
}

typedef hls::TPt BarrelPhiSlice[N_ETA_BINS_BARREL_REGION * N_ETA_SEGMENTS_BARREL];

int main(int argc, char const *argv[])
{
  hls::Barrel_Inputs inputs[ITERATIONS];
  hls::Barrel_PfInputHistogram::TBins barrel_bins[ITERATIONS];
  BarrelPhiSlice barrelPhiSlices[ITERATIONS];
  Links jets[ITERATIONS];

  for (unsigned int iteration = 0; iteration < ITERATIONS; iteration++)
  {
    for (unsigned int inputIdx = 0; inputIdx < NUMBER_OF_INPUTS_PER_CLOCK ; inputIdx++)
    {
      inputs[iteration][inputIdx].pt = 0;
      inputs[iteration][inputIdx].iPhi = 0;
      inputs[iteration][inputIdx].iEta = 0;
    }
  }

  std::vector<TPt> ptVector_0 = {4.25, 5.75, 9.75, 3.25, 4.5, 8.25, 12, 1.25, 2, 2.5, 6.75, 15.25, 6.25, 2.75};
  std::vector<TEta> etaVector_0 = {87, 107, 108, 113, 114, 120, 125, 126, 126, 128, 133, 139, 139, 160};
  std::vector<TPhi> phiVector_0 = {126, 142, 156, 129, 9, 156, 155, 154, 111, 153, 149, 12, 4, 52};

  std::vector<TPt> ptVector_1 = {8.25, 1.25, 1.75, 1.75, 3.5, 4, 4.75, 27, 2.5, 12.5, 6.25, 10.5, 3};
  std::vector<TEta> etaVector_1 = {13, 14, 17, 22, 33, 40, 52, 57, 58, 59, 68, 70, 78};
  std::vector<TPhi> phiVector_1 = {28, 34, 12, 13, 10, 57, 13, 25, 32, 20, 17, 21, 25};

  for (int x = 0; x < ptVector_0.size(); x++)
  {
    inputs[RESET_PERIOD][x].pt = ptVector_0[x];
    inputs[RESET_PERIOD][x].iEta = etaVector_0[x];
    inputs[RESET_PERIOD][x].iPhi = phiVector_0[x];
  }
  for (int x = 0; x < ptVector_1.size(); x++)
  {
    inputs[RESET_PERIOD + 1][x].pt = ptVector_1[x];
    inputs[RESET_PERIOD + 1][x].iEta = etaVector_1[x];
    inputs[RESET_PERIOD + 1][x].iPhi = phiVector_1[x];
  }

  bool inReset = true;
  bool outReset1, outReset2;
  hls_histogrammer(inputs[0], barrel_bins[0], inReset, outReset1);
  hls_histogram_buffer(barrel_bins[0], barrelPhiSlices[0], outReset1, outReset2);
  hls_jet_clustering(barrelPhiSlices[0], jets[0], outReset2);

  for (unsigned int iteration = 0; iteration < ITERATIONS; iteration++)
  {
    std::cout << "Running cycle: " << iteration << ": \n";
    bool inReset = false;
    bool outReset1, outReset2;
    // if (iteration == 24) std::raise(SIGINT);
    hls_histogrammer(inputs[iteration], barrel_bins[iteration], inReset, outReset1);
    hls_histogram_buffer(barrel_bins[iteration], barrelPhiSlices[iteration], outReset1, outReset2);
    hls_jet_clustering(barrelPhiSlices[iteration], jets[iteration], outReset2);

    std::cout << "PhiSlice: ";
    for (hls::TEta etaIndex = 0; etaIndex < ETA_GRID_SIZE; etaIndex++)
    {
      std::cout << barrelPhiSlices[iteration][etaIndex] << " ";
    }
    std::cout << std::endl;
    
    for (unsigned int jetIdx = 0; jetIdx < NUMBER_OF_SEEDS ; jetIdx++)
    {
      if (jets[iteration][jetIdx >> 1][jetIdx % 2].pt > 0) 
      {
        std::cout << "Jet: " << jets[iteration][jetIdx >> 1][jetIdx % 2].pt * 0.25 << "\t" << jets[iteration][jetIdx >> 1][jetIdx % 2].iEta << "\t" << jets[iteration][jetIdx >> 1][jetIdx % 2].iPhi << std::endl;
      }
    }

  }

  std::raise(SIGINT);

  // assert(jets[11+RESET_PERIOD][0 / 2][0 % 2].pt == 100);
  // assert(jets[11+RESET_PERIOD][0 / 2][0 % 2].iEta == 0);
  // assert(jets[11+RESET_PERIOD][0 / 2][0 % 2].iPhi == 0);
  // assert(jets[11+RESET_PERIOD][9 / 2][9 % 2].pt == 100);
  // assert(jets[11+RESET_PERIOD][9 / 2][9 % 2].iEta == 9);
  // assert(jets[11+RESET_PERIOD][9 / 2][9 % 2].iPhi == 0);

  return 0;
}
