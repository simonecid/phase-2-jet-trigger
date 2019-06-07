#ifndef __HISTOGRAMSETTINGS_H__
#define __HISTOGRAMSETTINGS_H__

#define ETA_BIN_SIZE 10
#define PHI_BIN_SIZE 10
#define PHI_N_BINS 8
#define BARREL_ETA_N_BINS 9
#define TK_HG_ETA_N_BINS 12 
#define HG_ETA_N_BINS 6
#define HF_ETA_N_BINS 6

#define ETA_BIN_SIZE 10
#define PHI_BIN_SIZE 10
#define PHI_N_BINS 3
#define BARREL_ETA_N_BINS 3
#define TK_HG_ETA_N_BINS 3
#define HG_ETA_N_BINS 3
#define HF_ETA_N_BINS 3

#ifndef __SYNTHESIS__
#define NUMBER_OF_INPUTS_PER_CLOCK 24
#endif
#ifdef __SYNTHESIS__
#define NUMBER_OF_INPUTS_PER_CLOCK 24
#endif

#endif