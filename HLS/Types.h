#ifndef __TYPES_H__
#define __TYPES_H__

#include "ap_int.h"
#include "HistogramSettings.h"
#include "Histogram2D.h"

namespace hls
{
  typedef ap_uint<10> TPt;
  typedef ap_uint<10> TEta;
  typedef ap_uint<10> TPhi;
  typedef TPt TBin;
  // typedef TBin TBins[PHI_GRID_SIZE][ETA_GRID_SIZE];
  typedef struct {
    TPt pt;
    TPhi iPhi;
    TEta iEta;
  } Input;
  typedef Input Barrel_Inputs[NUMBER_OF_INPUTS_PER_CLOCK];
  typedef Input TK_HG_Inputs[NUMBER_OF_INPUTS_PER_CLOCK];
  typedef Input HG_Inputs[NUMBER_OF_INPUTS_PER_CLOCK];
  typedef Input HF_Inputs[NUMBER_OF_INPUTS_PER_CLOCK];
  
  typedef Histogram2DFixedBinSize<
    TPt,
    TEta,
    ETA_BIN_SIZE, // size of a eta bin
    BARREL_ETA_N_BINS, // number of bins in eta
    TPhi,
    PHI_BIN_SIZE, // size of a phi bin
    PHI_N_BINS // number of bins in phi
    > Barrel_PfInputHistogram;


typedef Histogram2DFixedBinSize<
    TPt,
    TEta,
    ETA_BIN_SIZE, // size of a eta bin
    TK_HG_ETA_N_BINS, // number of bins in eta
    TPhi,
    PHI_BIN_SIZE, // size of a phi bin
    PHI_N_BINS // number of bins in phi
    > TK_HG_PfInputHistogram;

typedef Histogram2DFixedBinSize<
    TPt,
    TEta,
    ETA_BIN_SIZE, // size of a eta bin
    HG_ETA_N_BINS, // number of bins in eta
    TPhi,
    PHI_BIN_SIZE, // size of a phi bin
    PHI_N_BINS // number of bins in phi
    > HG_PfInputHistogram;

typedef Histogram2DFixedBinSize<
    TPt,
    TEta,
    ETA_BIN_SIZE, // size of a eta bin
    HF_ETA_N_BINS, // number of bins in eta
    TPhi,
    PHI_BIN_SIZE, // size of a phi bin
    PHI_N_BINS // number of bins in phi
    > HF_PfInputHistogram;
}
#endif