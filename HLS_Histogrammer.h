#ifndef __HLS_PHASE1CLUSTERING_H__
#define __HLS_PHASE1CLUSTERING_H__

/*
The code has an internal buffer which is PHI_GRID_SIZE * ETA_GRID_SIZE big
Each clock cycle I receive PHI_GRID_SIZE bins (all in the same eta).
I can run the jet finder once I received data able to cover PHI_GRID_SIZE*ETA_JET_SIZE region
*/

// size of the buffer holding the calogrid
#define PHI_GRID_SIZE 72
#define ETA_GRID_SIZE 96
#define ETA_JET_SIZE 5

#define PHI_JET_SIZE ETA_JET_SIZE
#define NUMBER_OF_SEEDS PHI_GRID_SIZE
//threshold for seeding
#define SEED_THRESHOLD 5

//controls what is pipelined, HLS_MAIN_FULLY_PIPELINED sets everything to true
#define FINDJET_PIPELINE false
#define FINDJET_PIPELINE_AND_UNROLL false
#define PHI_SCAN_PIPELINE_ONLY false
#define PHI_SCAN_PIPELINE_AND_UNROLL false
#define HLS_MAIN_FULLY_PIPELINED true
#define INLINE_EVERYTHING true

#if PHI_GRID_SIZE>=128
#error "PHI_GRID_SIZE is >= 128, this is not going to work"
#endif
#if ETA_GRID_SIZE>=128
#error "ETA_GRID_SIZE is >= 128, this is not going to work"
#endif

#include "ap_int.h"

typedef ap_uint<10> pt_type;
typedef unsigned char eta_type;
typedef unsigned char phi_type;


typedef struct {
  pt_type pt;
  phi_type iPhi;
  eta_type iEta;
} Jet;

typedef pt_type CaloGrid[ETA_GRID_SIZE][PHI_GRID_SIZE];
typedef pt_type CaloGridBuffer[ETA_JET_SIZE][PHI_GRID_SIZE];
typedef pt_type CaloGridPhiVector[PHI_GRID_SIZE];
typedef Jet Jets[NUMBER_OF_SEEDS];
typedef Jets TMJets[ETA_GRID_SIZE];

void hls_copyGrid (const CaloGridBuffer inCaloGrid, CaloGridBuffer outCaloGrid);
void hls_main(CaloGridPhiVector inCaloGridPhiSlice, Jets outJets, bool reset);
pt_type hls_getTowerEnergy(const CaloGridBuffer caloGrid, char iEta, char iPhi);
pt_type hls_findJet(const CaloGridBuffer caloGrid, unsigned char iEtaCentre, unsigned char iPhiCentre);
void hls_runJetFinders(const CaloGridBuffer inCaloGrid, Jets outJets);
void hls_copyLine (const CaloGridPhiVector caloGridPhiSlice, CaloGridBuffer outCaloGrid, unsigned char etaIndex);
void hls_shiftGridLeft (const CaloGridBuffer inCaloGrid, CaloGridBuffer outCaloGrid);
void hls_clearGrid(CaloGridBuffer grid);
void hls_clearJets(TMJets jets);

#ifndef __SYNTHESIS__
void printCaloGridBuffer(const CaloGridBuffer caloGrid);
#endif

#endif //__HLS_PHASE1CLUSTERING_H__