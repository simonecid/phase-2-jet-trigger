#define PHI_GRID_SIZE 72
#define ETA_GRID_SIZE 9
#define PHI_JET_SIZE 9
#define ETA_JET_SIZE 9
#define NUMBER_OF_SEEDS 128
#define SEED_THRESHOLD 5

#ifndef __HLS_PHASE1CLUSTERING_H__
#define __HLS_PHASE1CLUSTERING_H__


typedef unsigned short int CaloGrid[ETA_GRID_SIZE][PHI_GRID_SIZE];

typedef struct {
  unsigned short int pt;
  char iPhi;
  char iEta;
} Jet;

void hls_main(const CaloGrid caloGrid, const char centralIEta, 
              Jet jets[NUMBER_OF_SEEDS], unsigned char *numberOfJetsFound);
unsigned short int getTowerEnergy(const CaloGrid caloGrid, char iEta, char iPhi);
void buildJetFromSeed(const CaloGrid caloGrid, Jet* jet);
void buildJets(const CaloGrid caloGrid, Jet seeds[NUMBER_OF_SEEDS], unsigned char* numberOfSeedsFound, char inEtaShift);

#endif //__HLS_PHASE1CLUSTERING_H__