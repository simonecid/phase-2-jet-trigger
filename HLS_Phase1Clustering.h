#ifndef __HLS_PHASE1CLUSTERING_H__
#define __HLS_PHASE1CLUSTERING_H__

#define PHI_GRID_SIZE 72
#define ETA_GRID_SIZE 9
#define PHI_JET_SIZE 9
#define ETA_JET_SIZE 9
#define NUMBER_OF_SEEDS PHI_GRID_SIZE
#define SEED_THRESHOLD 5
// NOTE: PIPELINE_LENGTH >= 12 increases the latency from 5 to 6
#define PIPELINE_START 0
#define BUFFER_LENGTH PHI_GRID_SIZE
#define FINDJET_PIPELINE false
#define FINDJET_PIPELINE_AND_UNROLL false
#define PHI_SCAN_PIPELINE_ONLY false
#define PHI_SCAN_PIPELINE_AND_UNROLL false
#define HLS_MAIN_FULLY_PIPELINED true
#define INLINE_EVERYTHING true

#include "ap_int.h"

typedef ap_uint<10> pt_type;

typedef struct {
  pt_type pt;
  char iPhi;
  char iEta;
} Jet;

typedef pt_type CaloGrid[ETA_GRID_SIZE][PHI_GRID_SIZE];
typedef pt_type CaloGridPhiVector[PHI_GRID_SIZE];
typedef pt_type JetGrid[ETA_GRID_SIZE][PHI_GRID_SIZE];
typedef Jet Jets[NUMBER_OF_SEEDS];

void copyGrid (const CaloGrid inCaloGrid, CaloGrid outCaloGrid);
void copyJets (const Jets inJets, Jets outJets);
void hls_main(CaloGridPhiVector inCaloGridPhiSlice, Jets outJets, bool reset);
pt_type getTowerEnergy(const CaloGrid caloGrid, char iEta, char iPhi);
void buildJetFromSeed(const CaloGrid caloGrid, Jet* jet);
void buildJets(const CaloGrid caloGrid, Jet seeds[NUMBER_OF_SEEDS], char inEtaShift);
char getNormalisedPhi(char iPhi);
pt_type findJet(const CaloGrid caloGrid, char iEtaCentre, char iPhiCentre);
void pipelinedJetFinder(CaloGrid inCaloGrid, Jets outJets);
void copyLine (const CaloGridPhiVector caloGridPhiSlice, CaloGrid outCaloGrid, char etaIndex);
void shiftGridLeft (const CaloGrid inCaloGrid, CaloGrid outCaloGrid);

#endif //__HLS_PHASE1CLUSTERING_H__