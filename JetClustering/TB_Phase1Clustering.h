#ifndef __TB_PHASE1CLUSTERING_H__
#define __TB_PHASE1CLUSTERING_H__

#include "HLS_Phase1Clustering.h"

// collection of time-multiplexed jets, used in the test-bench
typedef Jets TMJets[ETA_GRID_SIZE];

void clearGrid(CaloGrid grid, TPt value=0);
void clearJets(TMJets jets);
void clearSums(SumLink & sums);
void test5x5();
bool readCaloGridFromFile(const std::string &filepath, CaloGridBuffer caloGridBuffer);
void runJetFinder(const CaloGrid caloGrid, TMJets tmJets, SumLink sumLink, bool reset);
void printCaloGrid(const CaloGrid caloGrid);
#endif //__TB_PHASE1CLUSTERING_H__