#include "HLS_Phase1Clustering.h"

#ifndef __SYNTHESIS__
#include <iostream>
#include <csignal>
void printBuffer(const CaloGridBuffer caloGrid)
{
  copyPhiGrid: for(TPhi iPhi = 0 ; iPhi < PHI_JET_SIZE ; iPhi++)
  {
    copyEtaGrid: for (TEta iEta = 0 ; iEta < ETA_GRID_SIZE ; iEta++)
    {
      std::cout << caloGrid[iPhi][iEta] << " " ;
    }
    std::cout << std::endl;
  }
  return;
}
#endif


TPt hls_getTowerEnergy(const CaloGridBuffer caloGrid, TEta iEta, TPhi iPhi)
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
  // if out of bounds return 0, else the value
  return ((iEta < 0) || (iEta >= ETA_GRID_SIZE)) ? kNullPt : caloGrid[iPhi][iEta];
}

void hls_copyGrid (const CaloGridBuffer inCaloGrid, CaloGridBuffer outCaloGrid)
{
  #pragma HLS inline
  #pragma HLS pipeline
  copyPhiGrid: for (TPhi iPhi = 0 ; iPhi < PHI_JET_SIZE ; iPhi++)
  {
    copyEtaGrid: for(TEta iEta = 0 ; iEta < ETA_GRID_SIZE ; iEta++)
    {
      outCaloGrid[iPhi][iEta] = inCaloGrid[iPhi][iEta];
    }
  }
}

void hls_clearGrid (CaloGridBuffer inCaloGrid)
{
  #pragma HLS inline
  #pragma HLS pipeline
  copyPhiGrid: for (TPhi iPhi = 0 ; iPhi < PHI_JET_SIZE ; iPhi++)
  {
    copyEtaGrid: for(TEta iEta = 0 ; iEta < ETA_GRID_SIZE ; iEta++)
    {
      inCaloGrid[iPhi][iEta] = 0;
    }
  }
}

void hls_shiftGridLeft (const CaloGridBuffer inCaloGrid, CaloGridBuffer outCaloGrid)
{
  #pragma HLS inline
  #pragma HLS pipeline
  copyPhiGrid: for(TPhi iPhi = 1 ; iPhi < PHI_JET_SIZE ; iPhi++)
  {
    copyEtaGrid: for (TEta iEta = 0 ; iEta < ETA_GRID_SIZE ; iEta++)
    {
      outCaloGrid[iPhi - 1][iEta] = inCaloGrid[iPhi][iEta];
    }
  }
  setEtaGrid0: for (TEta iEta = 0 ; iEta < ETA_GRID_SIZE ; iEta++)
  {
    outCaloGrid[PHI_JET_SIZE - 1][iEta] = 0;
  }
}

void hls_copyLine (const CaloGridPhiSlice caloGridPhiSlice, CaloGridBuffer outCaloGrid, TPhi phiIndex)
{
  #pragma HLS inline
  #pragma HLS pipeline
  copyPhiGrid: for (TEta iEta = 0 ; iEta < ETA_GRID_SIZE ; iEta++)
  {
    outCaloGrid[phiIndex][iEta] = caloGridPhiSlice[iEta];
  }
}

void hls_copyLine (const CaloGridPhiSlice inCaloGridPhiSlice, CaloGridPhiSlice outCaloGridPhiSlice)
{
  #pragma HLS inline
  #pragma HLS pipeline
  copyPhiGrid: for (TEta iEta = 0 ; iEta < ETA_GRID_SIZE ; iEta++)
  {
    outCaloGridPhiSlice[iEta] = inCaloGridPhiSlice[iEta];
  }
}

void hls_jet_clustering(const CaloGridPhiSlice inCaloGridPhiSlice, Links outJets, bool d0Valid) 
{
  #pragma HLS array_partition variable=inCaloGridPhiSlice complete dim=0
  #pragma HLS array_partition variable=outJets complete dim=0
  #pragma HLS array_reshape variable=outJets complete dim=2
  #pragma HLS data_pack variable=outJets
  // no valid ports for the I/O
  #pragma HLS interface ap_none port=inCaloGridPhiSlice
  // no valid ports for the I/O
  #pragma HLS interface ap_none port=outJets
  // no valid ports for the I/O
  #pragma HLS interface ap_none port=d0Valid
  // removing control bus from design
  #pragma HLS interface ap_ctrl_none port=return
  #if HLS_JET_CLUSTERING_FULLY_PIPELINED==true
  #pragma HLS pipeline
  #endif
  
  static TPhi sRegister = 0;
  bool lReset = d0Valid;
  TPhi lPhiIndex;
  static CaloGridBuffer sCaloGrid;
  #pragma HLS array_partition variable=sCaloGrid complete dim=0
  static CaloGridPhiSlice sFirstPhiSlices[PHI_JET_SIZE - 1];
  #pragma HLS array_partition variable=sFirstPhiSlices complete dim=0
  if (lReset)
  {
    sRegister = -2; // -2 because the previous stage in the next clock will receive region 1, then in the second region 2 and the jet finder will receive a row
    hls_clearGrid(sCaloGrid);
  } 
  else 
  {
    TPhi lNextRegisterValue = sRegister + 1;
    sRegister = (sRegister == (RESET_PERIOD - 1)) ? kNullPhi : lNextRegisterValue;
    lPhiIndex = sRegister;
    // #ifndef __SYNTHESIS__
    // std::cout << "sRegister: " << +sRegister << std::endl;
    // #endif
  }

  CaloGridBuffer lCaloGridTmp;
  #pragma HLS array_partition variable=lCaloGridTmp complete dim=0 
  //storing the first phi slices to analyse them later when I got the last ones
  if (lPhiIndex < PHI_JET_SIZE - 1) 
  {
    hls_copyLine(inCaloGridPhiSlice, sFirstPhiSlices[lPhiIndex]);
  }
  //storing in  the buffer
  if (lPhiIndex < PHI_GRID_SIZE) hls_copyLine(inCaloGridPhiSlice, sCaloGrid, PHI_JET_SIZE - 1);
  //if we have received all the phi slices then it is time to analyse the one I previously stored
  else if (lPhiIndex < PHI_GRID_SIZE + PHI_JET_SIZE - 1)
  {
    TPhi lLineIndex = lPhiIndex - PHI_GRID_SIZE;
    hls_copyLine(sFirstPhiSlices[lLineIndex], sCaloGrid, PHI_JET_SIZE - 1);
  }
  hls_copyGrid(sCaloGrid, lCaloGridTmp);
  hls_shiftGridLeft(sCaloGrid, sCaloGrid);
  Jets lJets; 
  hls_runJetFinders(lCaloGridTmp, lJets);
  
  //in I am buffering data before finding jets or I have analysed all the data, then I output null
  if ((lPhiIndex < PHI_JET_SIZE - 1) || (lPhiIndex >= PHI_GRID_SIZE + PHI_JET_SIZE - 1))
  {
    sendNullData:for (unsigned char jetIdx = 0 ; jetIdx < NUMBER_OF_SEEDS; jetIdx++) 
    {
      outJets[jetIdx >> 1][jetIdx % 2].pt = 0;
      outJets[jetIdx >> 1][jetIdx % 2].iPhi = 0;
      outJets[jetIdx >> 1][jetIdx % 2].iEta = 0;
    }
  } 
  else 
  {  
    copyBackJets:for (unsigned char jetIdx = 0 ; jetIdx < NUMBER_OF_SEEDS; jetIdx++) 
    {
      TPhi lOutJetsPhi = lPhiIndex - lJets[jetIdx].iPhi;
      outJets[jetIdx >> 1][jetIdx % 2].pt = lJets[jetIdx].pt;
      // handling phi periodicity
      TPhi lOutJetsPhiMinusPeriod = lOutJetsPhi - PHI_GRID_SIZE;
      outJets[jetIdx >> 1][jetIdx % 2].iPhi = (lOutJetsPhi >= PHI_GRID_SIZE) ? lOutJetsPhiMinusPeriod : lOutJetsPhi;
      outJets[jetIdx >> 1][jetIdx % 2].iEta = lJets[jetIdx].iEta;
    }
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
  jetFinderEtaLoop: for (TEta iEta = 0 ; iEta < ETA_GRID_SIZE ; iEta++) 
  {
    CaloGridBuffer lTmpCaloGrid;
    hls_copyGrid( inCaloGrid, lTmpCaloGrid );
    #pragma HLS array_partition variable=lTmpCaloGrid complete dim=0
    Jet lTmpJet;
    lTmpJet.pt = hls_findJet(inCaloGrid, iEta, PHI_JET_SIZE/2);
    // if (lTmpJet.pt > 0) std::raise(SIGINT);
    lTmpJet.iPhi = PHI_JET_SIZE/2;
    lTmpJet.iEta = iEta;
    outJets[iEta] = lTmpJet;
  }
}

TPt hls_findJet(const CaloGridBuffer caloGrid, TEta iEtaCentre, TPhi iPhiCentre) 
{
  #if INLINE_EVERYTHING==true
  #pragma HLS inline
  #endif
  #if FINDJET_PIPELINE_AND_UNROLL==true
  #pragma HLS pipeline
  #endif
  TPt centralPt = caloGrid[iPhiCentre][iEtaCentre];
  bool isLocalMaximum = (centralPt < SEED_THRESHOLD) ? false : true;
  TPt ptSum = 0;
  // Scanning through the grid centered on the seed
  checkMaximumEtaLoop: for (TEta etaIndex = 0; etaIndex < ETA_JET_SIZE; etaIndex++)
  {
    checkMaximumPhiLoop: for (TPhi phiIndex = 0; phiIndex < PHI_JET_SIZE; phiIndex++)
    {
      #if FINDJET_PIPELINE==true
      #pragma HLS pipeline
      #endif
      TPt towerEnergy = hls_getTowerEnergy(caloGrid, iEtaCentre + etaIndex - ETA_JET_SIZE/2, iPhiCentre + phiIndex - PHI_JET_SIZE/2);
      ptSum += towerEnergy;
      if ((etaIndex == ETA_JET_SIZE/2) && (phiIndex == ETA_JET_SIZE/2)) continue;
      if (centralPt < towerEnergy) {
        isLocalMaximum = false;
        continue; 
      }
      if (phiIndex > PHI_JET_SIZE/2) 
      {
        if (phiIndex >= ETA_JET_SIZE/2 - etaIndex)
        {
          isLocalMaximum = (isLocalMaximum) ? (centralPt > towerEnergy) : false;
        }
      } else 
      {
        if (phiIndex > ETA_JET_SIZE/2 - etaIndex)
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