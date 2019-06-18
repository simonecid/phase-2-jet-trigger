#include "HLS_Phase1Clustering.h"
#include "hls_stream.h"

#ifndef __SYNTHESIS__
#include <iostream>
#include <csignal>

void printCaloGridBuffer(const CaloGridBuffer caloGrid)
{
  std::cout << "Printing grid" << std::endl;
  for (unsigned char iPhi = 0; iPhi < PHI_GRID_SIZE; iPhi++)
  {
    for (unsigned char iEta = 0; iEta < ETA_JET_SIZE; iEta++)
    {
      std::cout << caloGrid[iEta][iPhi] << " ";
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;
}

#endif

pt_type hls_getTowerEnergy(const CaloGridBuffer caloGrid, char iEta, char iPhi)
{
  #if INLINE_EVERYTHING==true
  #pragma HLS inline
  #endif
  // We return the pt of a certain bin in the calo grid, taking account of the phi periodicity when overflowing (e.g. phi > phiSize), and returning 0 for the eta out of bounds

  if (iPhi < 0) 
  {
    iPhi += PHI_GRID_SIZE;
    //return 0;
  }
  if (iPhi > PHI_GRID_SIZE - 1) 
  {
    iPhi -= PHI_GRID_SIZE;
    //return 0;
  }
  if (iEta < 0) 
  {
    return 0;
  }
  if (iEta > ETA_JET_SIZE - 1) 
  {
    return 0;
  }
  return caloGrid[iEta][iPhi];
}

void hls_copyGrid (const CaloGridBuffer inCaloGrid, CaloGridBuffer outCaloGrid)
{
  //#pragma HLS inline
  #pragma HLS pipeline
  copyEtaGrid: for(unsigned char iEta = 0 ; iEta < ETA_JET_SIZE ; iEta++)
  {
    copyPhiGrid: for (unsigned char iPhi = 0 ; iPhi < PHI_GRID_SIZE ; iPhi++)
    {
      outCaloGrid[iEta][iPhi] = inCaloGrid[iEta][iPhi];
    }
  }
}

void hls_clearGrid (CaloGridBuffer inCaloGrid)
{
  #pragma HLS inline
  #pragma HLS pipeline
  copyEtaGrid: for(unsigned char iEta = 0 ; iEta < ETA_JET_SIZE ; iEta++)
  {
    copyPhiGrid: for (unsigned char iPhi = 0 ; iPhi < PHI_GRID_SIZE ; iPhi++)
    {
      inCaloGrid[iEta][iPhi] = 0;
    }
  }
}

void hls_shiftGridLeft (const CaloGridBuffer inCaloGrid, CaloGridBuffer outCaloGrid)
{
  #pragma HLS inline
  #pragma HLS pipeline
  copyEtaGrid: for(unsigned char iEta = 1 ; iEta < ETA_JET_SIZE ; iEta++)
  {
    copyPhiGrid: for (unsigned char iPhi = 0 ; iPhi < PHI_GRID_SIZE ; iPhi++)
    {
      outCaloGrid[iEta - 1][iPhi] = inCaloGrid[iEta][iPhi];
    }
  }
  setPhiGrid0: for (unsigned char iPhi = 0 ; iPhi < PHI_GRID_SIZE ; iPhi++)
  {
    outCaloGrid[ETA_JET_SIZE - 1][iPhi] = 0;
  }
}

void hls_copyLine (const CaloGridPhiVector caloGridPhiSlice, CaloGridBuffer outCaloGrid, unsigned char etaIndex)
{
  #pragma HLS inline
  #pragma HLS pipeline
  copyPhiGrid: for (unsigned char iPhi = 0 ; iPhi < PHI_GRID_SIZE ; iPhi++)
  {
    outCaloGrid[etaIndex][iPhi] = caloGridPhiSlice[iPhi];
  }
}

void hls_jet_clustering(CaloGridPhiVector inCaloGridPhiSlice, Jets outJets, bool reset) 
{
  #pragma HLS array_partition variable=inCaloGridPhiSlice complete dim=0
  #pragma HLS array_partition variable=outJets complete dim=0
  #if HLS_JET_CLUSTERING_FULLY_PIPELINED==true
  #pragma HLS pipeline
  #endif
  //we use two buffers two be able to receive data for a new event while analysing the previous one
  static unsigned char sEtaIndex = 0;
  static CaloGridBuffer sCaloGrid;
  #pragma HLS array_partition variable=sCaloGrid complete dim=0
  
  if (reset)
  {
    sEtaIndex = 0;
    hls_clearGrid(sCaloGrid);
  }

  CaloGridBuffer lCaloGridTmp;
  #pragma HLS array_partition variable=lCaloGridTmp complete dim=0 
  unsigned char lEtaIndex = sEtaIndex;
  sEtaIndex++;
  if (lEtaIndex < ETA_GRID_SIZE) hls_copyLine(inCaloGridPhiSlice, sCaloGrid, ETA_JET_SIZE - 1);
  hls_copyGrid(sCaloGrid, lCaloGridTmp);
  hls_shiftGridLeft(sCaloGrid, sCaloGrid);
  Jets lJets; 
  hls_runJetFinders(lCaloGridTmp, lJets);

  copyBackJets:for (unsigned char jetIdx = 0 ; jetIdx < NUMBER_OF_SEEDS; jetIdx++) 
  {
    outJets[jetIdx].pt = lJets[jetIdx].pt;
    outJets[jetIdx].iPhi = lJets[jetIdx].iPhi;
    outJets[jetIdx].iEta = lEtaIndex - lJets[jetIdx].iEta;
  }

  return;
}

void hls_runJetFinders(const CaloGridBuffer inCaloGrid, Jets outJets) 
{ 
  #if JET_FINDER_PIPELINE==true
  #pragma HLS pipeline
  #endif
  #if INLINE_EVERYTHING==true
  #pragma HLS inline
  #endif
  jetFinderPhiLoop: for (unsigned char iPhi = 0 ; iPhi < PHI_GRID_SIZE ; iPhi++) 
  {
    CaloGridBuffer lTmpCaloGrid;
    hls_copyGrid( inCaloGrid, lTmpCaloGrid );
    #pragma HLS array_partition variable=lTmpCaloGrid complete dim=0
    Jet lTmpJet;
    lTmpJet.pt = hls_findJet(inCaloGrid, ETA_JET_SIZE/2, iPhi);
    lTmpJet.iEta = ETA_JET_SIZE/2;
    lTmpJet.iPhi = iPhi;
    
    outJets[iPhi] = lTmpJet;
  }
}

pt_type hls_findJet(const CaloGridBuffer caloGrid, unsigned char iEtaCentre, unsigned char iPhiCentre) 
{
  #if INLINE_EVERYTHING==true
  #pragma HLS inline
  #endif
  #if FINDJET_PIPELINE_AND_UNROLL==true
  #pragma HLS pipeline
  #endif
  pt_type centralPt = caloGrid[iEtaCentre][iPhiCentre];
  bool isLocalMaximum = (centralPt < SEED_THRESHOLD) ? false : true;
  pt_type ptSum = 0;
  // Scanning through the grid centered on the seed
  checkMaximumEtaLoop: for (char etaIndex = -ETA_JET_SIZE/2; etaIndex <= ETA_JET_SIZE/2; etaIndex++)
  {
    checkMaximumPhiLoop: for (char phiIndex = -PHI_JET_SIZE/2; phiIndex <= PHI_JET_SIZE/2; phiIndex++)
    {
      #if FINDJET_PIPELINE==true
      #pragma HLS pipeline
      #endif
      pt_type towerEnergy = hls_getTowerEnergy(caloGrid, iEtaCentre + etaIndex, iPhiCentre + phiIndex);
      ptSum += towerEnergy;
      if ((etaIndex == 0) && (phiIndex == 0)) continue;
      if (centralPt < towerEnergy) {
        isLocalMaximum = false;
        continue; 
      }
      if (phiIndex > 0) 
      {
        if (phiIndex >= -etaIndex)
        {
          isLocalMaximum = (isLocalMaximum) ? (centralPt > towerEnergy) : false;
        }
      } else 
      {
        if (phiIndex > -etaIndex)
        {
          isLocalMaximum = (isLocalMaximum) ? (centralPt > towerEnergy) : false;
        }
      }
    }
  }
  if (isLocalMaximum) {
    return ptSum;
  }
  else
    return 0;
}