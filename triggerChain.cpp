#include "JetClustering/HLS_Phase1Clustering.h"
#include "HistogramAndBuffer/HLS_Histogrammer.h"
#include "HistogramAndBuffer/HLS_HistogramBuffer.h"
#include "HistogramAndBuffer/HLS/Types.h"

#include <assert.h>

#define ITERATIONS RESET_PERIOD*2

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

  inputs[0][3].pt = 100;
  inputs[0][3].iPhi = 25;
  inputs[0][3].iEta = 15;
  inputs[RESET_PERIOD + 1][3].pt = 100;
  inputs[RESET_PERIOD + 1][3].iPhi = 25;
  inputs[RESET_PERIOD + 1][3].iEta = 25;

  bool inReset = true;
  bool outReset1, outReset2;
  hls_histogrammer(inputs[0], barrel_bins[0], inReset, outReset1);
  hls_histogram_buffer(barrel_bins[0], barrelPhiSlices[0], outReset1, outReset2);
  hls_jet_clustering(barrelPhiSlices[0], jets[0], outReset2);

  for (unsigned int iteration = 0; iteration < ITERATIONS; iteration++)
  {
    bool inReset = false;
    bool outReset1, outReset2;
    hls_histogrammer(inputs[iteration], barrel_bins[iteration], inReset, outReset1);
    hls_histogram_buffer(barrel_bins[iteration], barrelPhiSlices[iteration], outReset1, outReset2);
    hls_jet_clustering(barrelPhiSlices[iteration], jets[iteration], outReset2);
  }

  assert(jets[5][1 / 2][1 % 2].pt == 100);
  assert(jets[5][1 / 2][1 % 2].iEta == 1);
  assert(jets[5][1 / 2][1 % 2].iPhi == 2);
  assert(jets[5+RESET_PERIOD][11 / 2][11 % 2].pt == 100);
  assert(jets[5+RESET_PERIOD][11 / 2][11 % 2].iEta == 11);
  assert(jets[5+RESET_PERIOD][11 / 2][11 % 2].iPhi == 2);

  return 0;
}
