#ifndef __HISTOGRAMSETTINGS_H__
#define __HISTOGRAMSETTINGS_H__

// #define PHI_GRID_SIZE 5
#define PHI_GRID_SIZE 10
// #define ETA_GRID_SIZE 5
#define ETA_GRID_SIZE PHI_GRID_SIZE


#define LUT_FILL false
// #define PHI_BINS {0, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100}
#define PHI_BINS {0, 10, 20, 30, 40, 50, 60, 70}
// #define ETA_BINS {0, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100}
#define ETA_BINS {0, 10, 20, 30, 40, 50, 60, 70}

#define DSP_FILL true
#define XBINLOW 0
#define XBINHIGH 100
#define XBINSTEP 10
#define YBINLOW 0
#define YBINHIGH 100
#define YBINSTEP 10

#ifndef __SYNTHESIS__
#define NUMBER_OF_INPUTS_PER_CLOCK 48
#endif
#ifdef __SYNTHESIS__
#define NUMBER_OF_INPUTS_PER_CLOCK 48
#endif

#endif