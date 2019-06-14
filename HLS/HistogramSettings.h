#ifndef __HISTOGRAM_SETTINGS_H__
#define __HISTOGRAM_SETTINGS_H__

// #define N_PHI_SEGMENTS 9
#define N_PHI_SEGMENTS 1

#define N_ETA_SEGMENTS_BARREL 2
#define N_ETA_SEGMENTS_TKHG 2
#define N_ETA_SEGMENTS_HG 2
#define N_ETA_SEGMENTS_HF 8

#define N_ETA_BINS_BARREL_REGION 9
#define N_ETA_BINS_TK_HG_REGION 12 
#define N_ETA_BINS_HG_REGION 6
#define N_ETA_BINS_HF_REGION 6

#define N_BINS_PHI_REGION 8

#define ETA_BIN_SIZE 10
#define PHI_BIN_SIZE 10

// #define ETA_BARREL_ 36
// #define ETA_GRID_SIZE ETA_BARREL_SIZE
// #define ETA_GRID_SIZE 120
// #define PHI_GRID_SIZE 8

// #define N_ETA_SEGMENTS_BARREL 4


// #define ETA_BIN_SIZE 10
// #define PHI_BIN_SIZE 10
// #define PHI_N_BINS 3
// #define BARREL_ETA_N_BINS 3
// #define TK_HG_ETA_N_BINS 3
// #define HG_ETA_N_BINS 3
// #define HF_ETA_N_BINS 3

#ifndef __SYNTHESIS__
#define NUMBER_OF_INPUTS_PER_CLOCK 24
#endif
#ifdef __SYNTHESIS__
#define NUMBER_OF_INPUTS_PER_CLOCK 24
#endif

#endif