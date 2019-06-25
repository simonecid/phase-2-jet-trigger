#include "JetClustering/HLS_Phase1Clustering.h"
#include "HistogramAndBuffer/HLS_Histogrammer.h"
#include "HistogramAndBuffer/HLS_HistogramBuffer.h"
#include "HistogramAndBuffer/HLS/Types.h"

#define ITERATIONS 10

int main(int argc, char const *argv[])
{
  hls::Barrel_Inputs inputs[ITERATIONS];
  hls::Barrel_Inputs::TBins barrel_bins[ITERATIONS];

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



  hls_histogrammer(inputs, barrel_bins[0]);
  hls_histogram_buffer(barrel_bins[0], );
  
  hls_histogrammer(inputs, barrel_bins[1]);
  

  return 0;
}
