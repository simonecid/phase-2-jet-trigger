#include "HLS_Phase1Clustering.h"

#ifndef __SYNTHESIS__
#include <iostream>
#endif

char getNormalisedPhi(char iPhi)
{
  if (iPhi < 0) 
  {
    iPhi += PHI_GRID_SIZE;
  }
  else if (iPhi > PHI_GRID_SIZE - 1) 
  {
    iPhi -= PHI_GRID_SIZE;
  } 
  return iPhi;  
}

unsigned short int getTowerEnergy(const CaloGrid caloGrid, char iEta, char iPhi)
{
  // We return the pt of a certain bin in the calo grid, taking account of the phi periodicity when overflowing (e.g. phi > phiSize), and returning 0 for the eta out of bounds

  if (iPhi < 0) 
  {
    iPhi += PHI_GRID_SIZE;
  }
  if (iPhi > PHI_GRID_SIZE - 1) 
  {
    iPhi -= PHI_GRID_SIZE;
  }
  if (iEta < 0) 
  {
    //std::cout << "Returning (pt, ieta, iphi): " << "(" << 0 << ", " << iEta << ", " << iPhi << ")" << std::endl;
    return 0;
  }
  if (iEta > ETA_GRID_SIZE - 1) 
  {
    //std::cout << "Returning (pt, ieta, iphi): " << "(" << 0 << ", " << iEta << ", " << iPhi << ")" << std::endl;
    return 0;
  }
    //std::cout << "Returning (pt, ieta, iphi): " << "(" << caloGrid.GetBinContent(iEta, iPhi) << ", " << iEta << ", " << iPhi << ")" << std::endl;
  return caloGrid[iEta][iPhi];
}

void buildJets_2(const CaloGrid caloGrid, Jet jets[NUMBER_OF_SEEDS], unsigned char* numberOfSeedsFound, char etaShift)
{
  JetGrid jetGrids[PHI_GRID_SIZE];
  #pragma HLS array_partition variable=jetGrids complete dim=3

  gridCopyEtaLoop_2: for (char iEtaIndex = 0; iEtaIndex < ETA_GRID_SIZE; iEtaIndex++) 
  {
    gridCopyPhiLoop_2: for (char iPhiIndex = 0; iPhiIndex < PHI_GRID_SIZE; iPhiIndex++) 
    {
      //lCaloGrid[iEtaIndex][iPhiIndex] = inCaloGrid[iEtaIndex][iPhiIndex];
      unsigned short int pt = caloGrid[iEtaIndex][iPhiIndex];
      #pragma HLS pipeline
      for (char jetGridIndex = -PHI_JET_SIZE/2; jetGridIndex < PHI_JET_SIZE/2; jetGridIndex++) 
      {
        char jetPhiCentre = getNormalisedPhi(iPhiIndex + jetGridIndex);
        char iPhiReindex = getNormalisedPhi(iPhiIndex - jetPhiCentre);
        jetGrids[iEtaIndex][iPhiReindex][jetPhiCentre] = pt;
      }
   }
  }

  for (char iPhiIndex = 0; iPhiIndex < PHI_GRID_SIZE; iPhiIndex++) 
  {

    #pragma HLS pipeline
    jets[iPhiIndex].pt = findJet(jetGrids[iPhiIndex]);
    jets[iPhiIndex].iEta = ETA_GRID_SIZE/2.;
    jets[iPhiIndex].iPhi = iPhiIndex;
  }

  return;
}



void buildJets(const CaloGrid caloGrid, Jet jets[NUMBER_OF_SEEDS], unsigned char* numberOfSeedsFound, char etaShift)
{
  #if NUMBER_OF_SEEDS > 256
  #pragma message "Working with more than 256 jets, moving to 2-byte index counter, please do not go over 65536 jets or this may break"
  unsigned int jetIdx = 0;
  #else
  unsigned char jetIdx = 0;
  #endif

  // for each point of the grid check if it is a local maximum
  // to do so I take a point, and look if is greater than the points around it (in the 9x9 neighborhood)
  // to prevent mutual exclusion, I check greater or equal for points above and right to the one I am considering (including the top-left point)
  // to prevent mutual exclusion, I check greater for points below and left to the one I am considering (including the bottom-right point)

  #if PHI_GRID_SIZE > 256
  #error "Phi grid size is above 256, looping variable not big enough"
  #endif 
  seedFinderPhiScanLoop: for (char iPhi = PHI_JET_SIZE/2; iPhi < PHI_GRID_SIZE - PHI_JET_SIZE/2; iPhi++)
  //seedFinderPhiScanLoop: for (char iPhi = 0; iPhi < 1; iPhi++)
  {
    //#pragma HLS unroll
    #if ETA_GRID_SIZE > 256
    #error "Eta grid size is above 256, looping variable not big enough"
    #endif
    //Excluding eta range in which the grid falls out of bounds
    seedFinderEtaScanLoop: for (char iEta = ETA_JET_SIZE/2; iEta < ETA_GRID_SIZE - ETA_JET_SIZE/2; iEta++)
    {
      //unsigned short int centralPt = caloGrid[iEta][iPhi]; //caloGrid.GetBinContent(iEta, iPhi);

      //if ((jetIdx >= NUMBER_OF_SEEDS) || (centralPt < SEED_THRESHOLD)) continue;

      //jets[jetIdx].pt = 0;
      //jets[jetIdx].iEta = iEta + etaShift;
      //jets[jetIdx].iPhi = iPhi;
      //bool isLocalMaximum = true;
      #pragma HLS pipeline
      
      JetGrid jetGrid;
      //#pragma HLS array_partition variable=jetGrid complete dim=0

      // Scanning through the grid centered on the seed
      checkMaximumEtaLoop: for (char etaIndex = -ETA_JET_SIZE/2; etaIndex <= ETA_JET_SIZE/2; etaIndex++)
      {
        checkMaximumPhiLoop: for (char phiIndex = -PHI_JET_SIZE/2; phiIndex <= PHI_JET_SIZE/2; phiIndex++)
        {
          char iEtaTmp = iEta + etaIndex;
          char iPhiTmp = iPhi + phiIndex;
          unsigned int towerEnergy = getTowerEnergy(caloGrid, iEtaTmp, iPhiTmp);
          jetGrid[etaIndex + ETA_JET_SIZE/2][phiIndex + PHI_JET_SIZE/2] = towerEnergy;

          //jets[jetIdx].pt += towerEnergy;
          //if ((etaIndex == 0) && (phiIndex == 0)) continue;
          //if (centralPt < towerEnergy) isLocalMaximum = false;
          //if (phiIndex > 0) 
          //{
          //  if (phiIndex >= -etaIndex)
          //  {
          //    isLocalMaximum = (isLocalMaximum) ? (centralPt > towerEnergy) : false;
          //  }
          //} else 
          //{
          //  if (phiIndex > -etaIndex)
          //  {
          //    isLocalMaximum = (isLocalMaximum) ? (centralPt > towerEnergy) : false;
          //  }
          //}
        }
      }

      unsigned short int jetPt = findJet(jetGrid);
      jets[jetIdx].pt = jetPt;
      jets[jetIdx].iEta = iEta + etaShift;
      jets[jetIdx].iPhi = iPhi;
      if (jetPt > 0)
      {
        jetIdx++;
      }
    }
  }
  *numberOfSeedsFound = jetIdx;
  return;
}

void buildJetFromSeed(const CaloGrid caloGrid, Jet* jet) 
{
  // Scanning through the grid centered on the seed

  unsigned short int ptSum = 0;

  ptSumEtaLoop: for (char iEtaIndex = -ETA_JET_SIZE/2; iEtaIndex <= ETA_JET_SIZE/2; iEtaIndex++)
  {
    ptSumPhiLoop: for (char iPhiIndex = -PHI_JET_SIZE/2; iPhiIndex <= PHI_JET_SIZE/2; iPhiIndex++)
    {
      ptSum += getTowerEnergy(caloGrid, jet -> iEta + iEtaIndex, jet -> iPhi + iPhiIndex);
    }
  }

  // Creating a jet with eta phi centered on the seed and momentum equal to the sum of the pt of the components    
  jet -> pt = ptSum;
}

void hls_main(const CaloGrid inCaloGrid, const char inEtaShift, Jet inJets[NUMBER_OF_SEEDS], unsigned char *numberOfJetsFound) 
{
  //#pragma HLS array_partition variable=inJets complete dim=0
  //#pragma HLS pipeline
  CaloGrid lCaloGrid;
  //#pragma HLS array_partition variable=lCaloGrid complete dim=0
  unsigned char numberOfSeedsFound = 0;
  Jet jets[NUMBER_OF_SEEDS];
  #pragma HLS array_partition variable=jets complete dim=0

  // copying grid
  gridCopyEtaLoop: for (char iEtaIndex = 0; iEtaIndex < ETA_GRID_SIZE; iEtaIndex++) 
  {
   gridCopyPhiLoop: for (char iPhiIndex = 0; iPhiIndex < PHI_GRID_SIZE; iPhiIndex++) 
   {
     lCaloGrid[iEtaIndex][iPhiIndex] = inCaloGrid[iEtaIndex][iPhiIndex];
   }
  }

  // computing jets
  //buildJets(lCaloGrid, jets, &numberOfSeedsFound, inEtaShift);
  buildJets_2(lCaloGrid, jets, &numberOfSeedsFound, inEtaShift);

  // copying back the results
  jetOutputCopyLoop: for (unsigned char jetIndex = 0; jetIndex < NUMBER_OF_SEEDS; jetIndex++) 
  {
    inJets[jetIndex].pt = jets[jetIndex].pt;
    inJets[jetIndex].iEta = jets[jetIndex].iEta;
    inJets[jetIndex].iPhi = jets[jetIndex].iPhi;
  }
  *numberOfJetsFound = numberOfSeedsFound;

  return;  
}

unsigned short int findJet(const JetGrid caloGrid) 
{
  //#pragma HLS array_partition variable=caloGrid complete dim=0
  #pragma HLS pipeline
  unsigned short int centralPt = caloGrid[ETA_JET_SIZE/2][PHI_JET_SIZE/2];
  bool isLocalMaximum = (centralPt < SEED_THRESHOLD) ? false : true;
  unsigned short int ptSum = 0;
  // Scanning through the grid centered on the seed
  checkMaximumEtaLoop: for (unsigned char etaIndex = 0; etaIndex < ETA_JET_SIZE; etaIndex++)
  {
    checkMaximumPhiLoop: for (unsigned char phiIndex = 0; phiIndex < PHI_JET_SIZE; phiIndex++)
    {
      unsigned int towerEnergy = getTowerEnergy(caloGrid, etaIndex, phiIndex);
      ptSum += towerEnergy;
      if ((etaIndex == ETA_JET_SIZE/2) && (phiIndex == PHI_JET_SIZE/2)) continue;
      if (centralPt < towerEnergy) {
        isLocalMaximum = false;
        continue; 
      }
      if (phiIndex > PHI_JET_SIZE/2) 
      {
        if (phiIndex >= -etaIndex + PHI_JET_SIZE - 1)
        {
          isLocalMaximum = (isLocalMaximum) ? (centralPt > towerEnergy) : false;
        }
      } else 
      {
        if (phiIndex > -etaIndex + PHI_JET_SIZE - 1)
        {
          isLocalMaximum = (isLocalMaximum) ? (centralPt > towerEnergy) : false;
        }
      }
    }
  }
  return isLocalMaximum ? ptSum : 0;
}