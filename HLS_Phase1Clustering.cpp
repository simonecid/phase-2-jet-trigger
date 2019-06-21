#include "HLS_Phase1Clustering.h"

#ifndef __SYNTHESIS__
#include <iostream>
#include <csignal>
#endif

pt_type hls_getTowerEnergy(const CaloGridBuffer caloGrid, char iEta, char iPhi)
{
  // #if INLINE_EVERYTHING==true
  #pragma HLS inline
  // #endif
  // // We return the pt of a certain bin in the calo grid, taking account of the phi periodicity when overflowing (e.g. phi > phiSize), and returning 0 for the eta out of bounds

  // if (iPhi < 0) 
  // {
  //   iPhi += PHI_GRID_SIZE;
  //   //return 0;
  // }
  // if (iPhi > PHI_GRID_SIZE - 1) 
  // {
  //   iPhi -= PHI_GRID_SIZE;
  //   //return 0;
  // }
  // if (iEta < 0) 
  // {
  //   return 0;
  // }
  // if (iEta > ETA_JET_SIZE - 1) 
  // {
  //   return 0;
  // }

  return caloGrid[iPhi][iEta];
}

void hls_copyGrid (const CaloGridBuffer inCaloGrid, CaloGridBuffer outCaloGrid)
{
  #pragma HLS inline
  #pragma HLS pipeline
  copyPhiGrid: for (unsigned char iPhi = 0 ; iPhi < PHI_JET_SIZE ; iPhi++)
  {
    copyEtaGrid: for(unsigned char iEta = 0 ; iEta < ETA_GRID_SIZE ; iEta++)
    {
      outCaloGrid[iPhi][iEta] = inCaloGrid[iPhi][iEta];
    }
  }
}

void hls_clearGrid (CaloGridBuffer inCaloGrid)
{
  #pragma HLS inline
  #pragma HLS pipeline
  copyPhiGrid: for (unsigned char iPhi = 0 ; iPhi < PHI_JET_SIZE ; iPhi++)
  {
    copyEtaGrid: for(unsigned char iEta = 0 ; iEta < ETA_GRID_SIZE ; iEta++)
    {
      inCaloGrid[iPhi][iEta] = 0;
    }
  }
}

void hls_shiftGridLeft (const CaloGridBuffer inCaloGrid, CaloGridBuffer outCaloGrid)
{
  #pragma HLS inline
  #pragma HLS pipeline
  copyPhiGrid: for(unsigned char iPhi = 1 ; iPhi < PHI_JET_SIZE ; iPhi++)
  {
    copyEtaGrid: for (unsigned char iEta = 0 ; iEta < ETA_GRID_SIZE ; iEta++)
    {
      outCaloGrid[iPhi - 1][iEta] = inCaloGrid[iPhi][iEta];
    }
  }
  setEtaGrid0: for (unsigned char iEta = 0 ; iEta < ETA_GRID_SIZE ; iEta++)
  {
    outCaloGrid[PHI_JET_SIZE - 1][iEta] = 0;
  }
}

void hls_copyLine (const CaloGridPhiSlice caloGridPhiSlice, CaloGridBuffer outCaloGrid, unsigned char phiIndex)
{
  #pragma HLS inline
  #pragma HLS pipeline
  copyPhiGrid: for (unsigned char iEta = 0 ; iEta < ETA_GRID_SIZE ; iEta++)
  {
    outCaloGrid[phiIndex][iEta] = caloGridPhiSlice[iEta];
  }
}

void hls_copyLine (const CaloGridPhiSlice inCaloGridPhiSlice, CaloGridPhiSlice outCaloGridPhiSlice)
{
  #pragma HLS inline
  #pragma HLS pipeline
  copyPhiGrid: for (unsigned char iEta = 0 ; iEta < ETA_GRID_SIZE ; iEta++)
  {
    outCaloGridPhiSlice[iEta] = inCaloGridPhiSlice[iEta];
  }
}

void hls_jet_clustering(CaloGridPhiSlice inCaloGridPhiSlice, Jets outJets, bool reset) 
{
  #pragma HLS array_partition variable=inCaloGridPhiSlice complete dim=0
  #pragma HLS array_partition variable=outJets complete dim=0
  #pragma HLS data_pack variable=outJets 
  #if HLS_JET_CLUSTERING_FULLY_PIPELINED==true
  #pragma HLS pipeline
  #endif
  
  static unsigned char sPhiIndex = 0;
  static CaloGridBuffer sCaloGrid;
  #pragma HLS array_partition variable=sCaloGrid complete dim=0
  static CaloGridPhiSlice sFirstPhiSlices[PHI_JET_SIZE - 1];
  #pragma HLS array_partition variable=sFirstPhiSlices complete dim=0
  if (reset)
  {
    sPhiIndex = 0;
    hls_clearGrid(sCaloGrid);
  }

  CaloGridBuffer lCaloGridTmp;
  #pragma HLS array_partition variable=lCaloGridTmp complete dim=0 
  unsigned char lPhiIndex = sPhiIndex;
  sPhiIndex++;
  //storing the first phi slices to analyse them later when I got the last ones
  if (lPhiIndex < PHI_JET_SIZE - 1) hls_copyLine(inCaloGridPhiSlice, sFirstPhiSlices[lPhiIndex]);
  //storing in  the buffer
  if (lPhiIndex < PHI_GRID_SIZE) hls_copyLine(inCaloGridPhiSlice, sCaloGrid, PHI_JET_SIZE - 1);
  //if we have received all the phi slices then it is time to analyse the one I previously stored
  else if (lPhiIndex < PHI_GRID_SIZE + PHI_JET_SIZE - 1) {
    lPhiIndex = lPhiIndex - PHI_GRID_SIZE;
    hls_copyLine(sFirstPhiSlices[lPhiIndex], sCaloGrid, PHI_JET_SIZE - 1);
  }
  hls_copyGrid(sCaloGrid, lCaloGridTmp);
  hls_shiftGridLeft(sCaloGrid, sCaloGrid);
  Jets lJets; 
  hls_runJetFinders(lCaloGridTmp, lJets);
  
  copyBackJets:for (unsigned char jetIdx = 0 ; jetIdx < NUMBER_OF_SEEDS; jetIdx++) 
  {
    outJets[jetIdx].pt = lJets[jetIdx].pt;
    outJets[jetIdx].iPhi = lPhiIndex - lJets[jetIdx].iPhi;
    outJets[jetIdx].iEta = lJets[jetIdx].iEta;
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
  jetFinderEtaLoop: for (unsigned char iEta = 0 ; iEta < ETA_GRID_SIZE ; iEta++) 
  {
    CaloGridBuffer lTmpCaloGrid;
    hls_copyGrid( inCaloGrid, lTmpCaloGrid );
    #pragma HLS array_partition variable=lTmpCaloGrid complete dim=0
    Jet lTmpJet;
    lTmpJet.pt = hls_findJet(inCaloGrid, iEta, PHI_JET_SIZE/2);
    lTmpJet.iPhi = PHI_JET_SIZE/2;
    lTmpJet.iEta = iEta;
    outJets[iEta] = lTmpJet;
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
  pt_type centralPt = caloGrid[iPhiCentre][iEtaCentre];
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