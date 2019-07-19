#ifndef __HLS_PHASE1CLUSTERING_H__
#define __HLS_PHASE1CLUSTERING_H__

/*
The code has an internal buffer which is PHI_GRID_SIZE * ETA_GRID_SIZE big
Each clock cycle I receive PHI_GRID_SIZE bins (all in the same eta).
I can run the jet finder once I received data able to cover PHI_GRID_SIZE*ETA_JET_SIZE region
*/

// size of the buffer holding the calogrid
#define PHI_GRID_SIZE 9
#define ETA_GRID_SIZE 18
#define ETA_JET_SIZE 5

#define PHI_JET_SIZE ETA_JET_SIZE
#define NUMBER_OF_SEEDS ETA_GRID_SIZE
//threshold for seeding
#define SEED_THRESHOLD 5
#define RESET_PERIOD 12

//controls what is pipelined, HLS_JET_CLUSTERING_FULLY_PIPELINED sets everything to true
#define FINDJET_PIPELINE false
#define FINDJET_PIPELINE_AND_UNROLL false
#define PHI_SCAN_PIPELINE_ONLY false
#define PHI_SCAN_PIPELINE_AND_UNROLL false
#define HLS_JET_CLUSTERING_FULLY_PIPELINED true
#define INLINE_EVERYTHING true

#if PHI_GRID_SIZE>=128
#error "PHI_GRID_SIZE is >= 128, this is not going to work"
#endif
#if ETA_GRID_SIZE>=128
#error "ETA_GRID_SIZE is >= 128, this is not going to work"
#endif

#include <ap_int.h>

typedef ap_uint<10> pt_type;
typedef ap_uint<10> eta_type;
typedef ap_uint<10> phi_type;
typedef ap_uint<34> TDummy;


typedef struct {
  pt_type pt;
  phi_type iPhi;
  eta_type iEta;
  TDummy dummy; // 30 - 63
} Jet;

typedef pt_type CaloGrid[PHI_GRID_SIZE][ETA_GRID_SIZE];
typedef pt_type CaloGridBuffer[PHI_JET_SIZE][ETA_GRID_SIZE];
typedef pt_type CaloGridPhiSlice[ETA_GRID_SIZE];
typedef Jet Jets[NUMBER_OF_SEEDS];
typedef Jets TMJets[ETA_GRID_SIZE];

void hls_copyGrid (const CaloGridBuffer inCaloGrid, CaloGridBuffer outCaloGrid);
void hls_jet_clustering(CaloGridPhiSlice inCaloGridPhiSlice, Jets outJets, bool reset);
pt_type hls_getTowerEnergy(const CaloGridBuffer caloGrid, char iEta, char iPhi);
pt_type hls_findJet(const CaloGridBuffer caloGrid, unsigned char iEtaCentre, unsigned char iPhiCentre);
void hls_runJetFinders(const CaloGridBuffer inCaloGrid, Jets outJets);
void hls_copyLine (const CaloGridPhiSlice caloGridPhiSlice, CaloGridBuffer outCaloGrid, unsigned char etaIndex);
void hls_shiftGridLeft (const CaloGridBuffer inCaloGrid, CaloGridBuffer outCaloGrid);
void hls_clearGrid(CaloGridBuffer grid);
void hls_clearJets(TMJets jets);

#ifndef __SYNTHESIS__
void printCaloGridBuffer(const CaloGridBuffer caloGrid);
#endif

#endif //__HLS_PHASE1CLUSTERING_H__