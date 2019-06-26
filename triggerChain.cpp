#include "JetClustering/HLS_Phase1Clustering.h"
#include "HistogramAndBuffer/HLS_Histogrammer.h"
#include "HistogramAndBuffer/HLS_HistogramBuffer.h"
#include "HistogramAndBuffer/HLS/Types.h"

#define ITERATIONS 10

typedef hls::TPt BarrelPhiSlice[N_ETA_BINS_BARREL_REGION * N_ETA_SEGMENTS_BARREL];

int main(int argc, char const *argv[])
{
  hls::Barrel_Inputs inputs[ITERATIONS];
  hls::Barrel_Inputs::TBins barrel_bins[ITERATIONS];
  BarrelPhiSlice barrelPhiSlices[ITERATIONS];

  for (unsigned int iteration = 0; iteration < ITERATIONS; iteration++)
  {
    for (unsigned int inputIdx = 0; inputIdx < NUMBER_OF_INPUTS_PER_CLOCK ; inputIdx++)
    {
      inputs[inputIdx].pt = 0;
      inputs[inputIdx].iPhi = 0;
      inputs[inputIdx].iEta = 0;
    }
  }

  inputs[0][inputIdx].pt = 100;
  inputs[0][inputIdx].iPhi = 25;
  inputs[0][inputIdx].iEta = 25;

  for (unsigned int iteration = 0; iteration < ITERATIONS + ; iteration++)
  {
    bool reset = (iteration == 0);
    hls_histogrammer(inputs[iteration], barrel_bins[iteration], reset);
    hls_histogram_buffer(barrel_bins[iteration], barrelPhiSlice[ITERATIONS], reset);
    hls_jet_clustering()
  }

  

  return 0;
}
