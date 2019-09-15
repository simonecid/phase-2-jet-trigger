#include "JetClustering/HLS_Phase1Clustering.h"
#include "HistogramAndBuffer/HLS_Histogrammer.h"
#include "HistogramAndBuffer/HLS_HistogramBuffer.h"
#include "HistogramAndBuffer/HLS/Types.h"

#include <assert.h>

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

  std::vector<float> ptVector = {65.0, 12.0, 34.25, 3.75, 8.5, 5.25, 4.5, 15.25, 9.5, 7.5, 5.75, 4.25, 3.25, 2.0};
  std::vector<int> etaVector = {14, 15, 30, 18, 63, 26, 26, 46, 23, 32, 46, 46, 1, 63};
  std::vector<int> phiVector = {33, 30, 118, 113, 113, 111, 111, 123, 12, 126, 99, 118, 110, 28};

  for (int x = 0; x < ptVector.size(); x++)
  {
    inputs[RESET_PERIOD][x].pt = ptVector[x] / 0.25;
    inputs[RESET_PERIOD][x].iEta = etaVector[x];
    inputs[RESET_PERIOD][x].iPhi = phiVector[x];
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
