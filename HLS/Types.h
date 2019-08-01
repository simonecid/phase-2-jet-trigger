#ifndef __TYPES_H__
#define __TYPES_H__

#include <ap_int.h>
#include "HistogramSettings.h"
#include "Histogram2D.h"

namespace hls
{
  typedef ap_uint<16> TPt; // LSB = 0.25 GeV
  // typedef unsigned int TPt;
  typedef ap_uint<10> TEta; // LSB is eq. to 0.0043633231, 1/4 of ECAL crystal eta
  // typedef unsigned int TEta;
  typedef ap_uint<10> TPhi; // LSB is eq. to 0.0043633231, 1/4 of ECAL crystal eta
  typedef ap_uint<28> TDummy;
  // typedef unsigned int TPhi;
  typedef TPt TBin;
  // typedef TBin TBins[PHI_GRID_SIZE][ETA_GRID_SIZE];
  typedef struct {
    TPt pt; //0-15
    TPhi iPhi; //16-25
    TEta iEta; //26-35
    TDummy dummy; // 35 - 63
  } Input;
  typedef Input Barrel_Inputs[NUMBER_OF_INPUTS_PER_CLOCK];
  typedef Input TK_HG_Inputs[NUMBER_OF_INPUTS_PER_CLOCK];
  typedef Input HG_Inputs[NUMBER_OF_INPUTS_PER_CLOCK];
  typedef Input HF_Inputs[NUMBER_OF_INPUTS_PER_CLOCK];
  
  typedef Histogram2DFixedBinSize<
    TPt,
    TEta,
    ETA_BIN_SIZE, // size of a eta bin
    N_ETA_BINS_BARREL_REGION, // number of bins in eta
    TPhi,
    PHI_BIN_SIZE, // size of a phi bin
    N_BINS_PHI_REGION // number of bins in phi
    > Barrel_PfInputHistogram;


typedef Histogram2DFixedBinSize<
    TPt,
    TEta,
    ETA_BIN_SIZE, // size of a eta bin
    N_ETA_BINS_TK_HG_REGION, // number of bins in eta
    TPhi,
    PHI_BIN_SIZE, // size of a phi bin
    N_BINS_PHI_REGION // number of bins in phi
    > TK_HG_PfInputHistogram;

typedef Histogram2DFixedBinSize<
    TPt,
    TEta,
    ETA_BIN_SIZE, // size of a eta bin
    N_ETA_BINS_HG_REGION, // number of bins in eta
    TPhi,
    PHI_BIN_SIZE, // size of a phi bin
    N_BINS_PHI_REGION // number of bins in phi
    > HG_PfInputHistogram;

typedef Histogram2DFixedBinSize<
    TPt,
    TEta,
    ETA_BIN_SIZE, // size of a eta bin
    N_ETA_BINS_HF_REGION, // number of bins in eta
    TPhi,
    PHI_BIN_SIZE, // size of a phi bin
    N_BINS_PHI_REGION // number of bins in phi
    > HF_PfInputHistogram;
}
#endif