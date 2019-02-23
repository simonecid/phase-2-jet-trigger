#define PHI_GRID_SIZE 15
#define ETA_GRID_SIZE 9
#define PHI_JET_SIZE 9
#define ETA_JET_SIZE 9
#define NUMBER_OF_SEEDS PHI_GRID_SIZE
#define SEED_THRESHOLD 5
#define FINDJET_PIPELINE false
#define FINDJET_PIPELINE_AND_UNROLL false
#define PHI_SCAN_PIPELINE_ONLY false
#define PHI_SCAN_PIPELINE_AND_UNROLL false
#define HLS_MAIN_FULLY_PIPELINED false

#ifndef __HLS_PHASE1CLUSTERING_H__
#define __HLS_PHASE1CLUSTERING_H__

#include "hls_stream.h"


typedef unsigned short int CaloGrid[ETA_GRID_SIZE][PHI_GRID_SIZE];
typedef unsigned short int CaloGridPhiVector[PHI_GRID_SIZE];
typedef unsigned short int JetGrid[ETA_GRID_SIZE][PHI_GRID_SIZE];

typedef struct {
  unsigned short int pt;
  char iPhi;
  char iEta;
} Jet;

void hls_main(CaloGrid inCaloGrid, const char inEtaShift, Jet outJets[NUMBER_OF_SEEDS]);
unsigned short int getTowerEnergy(const CaloGrid caloGrid, char iEta, char iPhi);
void buildJetFromSeed(const CaloGrid caloGrid, Jet* jet);
void buildJets(const CaloGrid caloGrid, Jet seeds[NUMBER_OF_SEEDS], char inEtaShift);
char getNormalisedPhi(char iPhi);
unsigned short int findJet(const CaloGrid caloGrid, char iEtaCentre, char iPhiCentre);

#endif //__HLS_PHASE1CLUSTERING_H__