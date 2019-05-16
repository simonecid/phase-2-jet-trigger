#ifndef __HLS_HISTOGRAMMER_H__
#define __HLS_HISTOGRAMMER_H__

#ifndef __SYNTHESIS__
#define NUMBER_OF_INPUTS_PER_CLOCK 256
#endif
#ifdef __SYNTHESIS__
#define NUMBER_OF_INPUTS_PER_CLOCK 100
#endif
#define NUMBER_OF_CLOCK_CYCLES_PER_REGION 1

#include "HLS/HistogramEtaPhi.h"

typedef struct {
  hls::TPt pt;
  hls::TPhi iPhi;
  hls::TEta iEta;
} Input;
typedef Input Inputs[NUMBER_OF_INPUTS_PER_CLOCK];

void hls_histogrammer(const Inputs inputs, hls::TBins bins, const bool reset);
void copyInputs (const Inputs srcInputs, Inputs destInputs);

#endif //__HLS_HISTOGRAMMER_H__