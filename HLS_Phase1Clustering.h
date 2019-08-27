#ifndef __HLS_PHASE1CLUSTERING_H__
#define __HLS_PHASE1CLUSTERING_H__

/*
The code has an internal buffer which is PHI_GRID_SIZE * ETA_GRID_SIZE big
Each clock cycle I receive PHI_GRID_SIZE bins (all in the same eta).
I can run the jet finder once I received data able to cover PHI_GRID_SIZE*ETA_JET_SIZE region
*/

// size of the buffer holding the calogrid
#define PHI_GRID_SIZE 8
#define ETA_GRID_SIZE 18
#define ETA_JET_SIZE 5
#define NUMBER_OF_LINKS 72

#define PHI_JET_SIZE ETA_JET_SIZE
#define NUMBER_OF_SEEDS ETA_GRID_SIZE
//threshold for seeding
#define SEED_THRESHOLD 20
#define RESET_PERIOD 13

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

typedef ap_uint<16> TPt; // LSB = 0.25 GeV
// typedef unsigned int TPt; // LSB = 0.25 GeV
typedef ap_uint<8> TEta; // LSB is eq. to GRID_SIZE
typedef ap_uint<8> TPhi; // LSB is eq. to GRID_SIZE


typedef struct {
  TPt pt;
  TPhi iPhi;
  TEta iEta;
} Jet;

typedef Jet Link[2];
typedef Link Links[NUMBER_OF_LINKS];

typedef TPt CaloGrid[PHI_GRID_SIZE][ETA_GRID_SIZE];
typedef TPt CaloGridBuffer[PHI_JET_SIZE][ETA_GRID_SIZE];
typedef TPt CaloGridPhiSlice[ETA_GRID_SIZE];
typedef Jet Jets[NUMBER_OF_SEEDS];
typedef Jets TMJets[ETA_GRID_SIZE];

const TPhi kNullPhi = 0;
const TEta kNullEta = 0;
const TPt kNullPt = 0;

void hls_copyGrid (const CaloGridBuffer inCaloGrid, CaloGridBuffer outCaloGrid);
void hls_jet_clustering(const CaloGridPhiSlice inCaloGridPhiSlice, Links outJets, bool d0Valid);
TPt hls_getTowerEnergy(const CaloGridBuffer caloGrid, TEta iEta, TPhi iPhi);
TPt hls_findJet(const CaloGridBuffer caloGrid, TEta iEtaCentre, TPhi iPhiCentre);
void hls_runJetFinders(const CaloGridBuffer inCaloGrid, Jets outJets);
void hls_copyLine (const CaloGridPhiSlice caloGridPhiSlice, CaloGridBuffer outCaloGrid, TEta etaIndex);
void hls_shiftGridLeft (const CaloGridBuffer inCaloGrid, CaloGridBuffer outCaloGrid);
void hls_clearGrid(CaloGridBuffer grid);
void hls_clearJets(TMJets jets);

#ifndef __SYNTHESIS__
void printCaloGridBuffer(const CaloGridBuffer caloGrid);
#endif

#endif //__HLS_PHASE1CLUSTERING_H__