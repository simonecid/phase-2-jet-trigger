#ifndef __HLS_HISTOGRAMMER_H__
#define __HLS_HISTOGRAMMER_H__

#define NUMBER_OF_INPUTS_PER_CLOCK 100
#define NUMBER_OF_CLOCK_CYCLES_PER_REGION 1

#include "HLS/HistogramEtaPhi.h"

typedef struct {
  hls::TPt pt;
  hls::TPhi iPhi;
  hls::TEta iEta;
} Input;
typedef Inputs Input[NUMBER_OF_INPUTS_PER_CLOCK];

void hls_histogrammer(Inputs *inputs, hls::TBins *bins);

#endif //__HLS_HISTOGRAMMER_H__