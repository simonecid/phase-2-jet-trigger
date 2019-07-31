#ifndef __TB_PHASE1CLUSTERING_H__
#define __TB_PHASE1CLUSTERING_H__

#include "HLS_Phase1Clustering.h"

void clearGrid(CaloGrid grid, pt_type value=0);
void clearJets(TMJets jets);
void test9x9();
void test5x5();
bool readCaloGridFromFile(const std::string &filepath, CaloGridBuffer caloGridBuffer);
void runJetFinder(const CaloGridBuffer caloGridBuffer, TMJets tmJets, bool reset);
void printCaloGrid(const CaloGrid caloGrid);
#endif //__TB_PHASE1CLUSTERING_H__