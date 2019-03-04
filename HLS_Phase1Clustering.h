#ifndef __HLS_PHASE1CLUSTERING_H__
#define __HLS_PHASE1CLUSTERING_H__

#define PHI_GRID_SIZE 10
#define ETA_GRID_SIZE 5
#define PHI_JET_SIZE 5
#define ETA_JET_SIZE 5
#define NUMBER_OF_SEEDS PHI_GRID_SIZE
#define SEED_THRESHOLD 5
// NOTE: PIPELINE_LENGTH >= 12 increases the latency from 5 to 6
#define PIPELINE_START 0
#define PIPELINE_LENGTH PHI_GRID_SIZE
#define FINDJET_PIPELINE false
#define FINDJET_PIPELINE_AND_UNROLL true
#define PHI_SCAN_PIPELINE_ONLY false
#define PHI_SCAN_PIPELINE_AND_UNROLL false
#define HLS_MAIN_FULLY_PIPELINED true

typedef struct {
  unsigned short int pt;
  char iPhi;
  char iEta;
} Jet;

typedef unsigned short int CaloGrid[ETA_GRID_SIZE][PHI_GRID_SIZE];
typedef unsigned short int CaloGridPhiVector[PHI_GRID_SIZE];
typedef unsigned short int JetGrid[ETA_GRID_SIZE][PHI_GRID_SIZE];
typedef Jet Jets[NUMBER_OF_SEEDS];

void copyGrid (const CaloGrid inCaloGrid, CaloGrid outCaloGrid);
void copyJets (const Jets inJets, Jets outJets);
void hls_main(CaloGrid inCaloGrid, const char inEtaShift, Jet outJets[NUMBER_OF_SEEDS]);
unsigned short int getTowerEnergy(const CaloGrid caloGrid, char iEta, char iPhi);
void buildJetFromSeed(const CaloGrid caloGrid, Jet* jet);
void buildJets(const CaloGrid caloGrid, Jet seeds[NUMBER_OF_SEEDS], char inEtaShift);
char getNormalisedPhi(char iPhi);
unsigned short int findJet(const CaloGrid caloGrid, char iEtaCentre, char iPhiCentre);

#endif //__HLS_PHASE1CLUSTERING_H__