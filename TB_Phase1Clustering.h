#ifndef __TB_PHASE1CLUSTERING_H__
#define __TB_PHASE1CLUSTERING_H__

#include "HLS_Phase1Clustering.h"

void cleanGrid(CaloGrid grid, pt_type value=0);
void cleanJets(Jets jets);
void test9x9();
void test5x5();
void readCaloGridBufferFromFile(const std::string &filepath, CaloGridBuffer caloGridBuffer);
void runJetFinder(const CaloGridBuffer caloGridBuffer, TMJets tmJets);

#endif //__TB_PHASE1CLUSTERING_H__