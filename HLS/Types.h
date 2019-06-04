#ifndef __TYPES_H__
#define __TYPES_H__

#include "ap_int.h"
#include "HistogramSettings.h"

namespace hls
{
  typedef ap_uint<10> TPt;
  typedef unsigned char TEta;
  typedef unsigned char TPhi;
  typedef TPt TBin;
  // typedef TBin TBins[PHI_GRID_SIZE][ETA_GRID_SIZE];
  typedef struct {
    TPt pt;
    TPhi iPhi;
    TEta iEta;
  } Input;
  typedef Input Inputs[NUMBER_OF_INPUTS_PER_CLOCK];
}
#endif