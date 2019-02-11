#include "HLS_Phase1Clustering.h"

#ifndef __SYNTHESIS__
#include <iostream>
#endif

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

void buildJets(const CaloGrid caloGrid, Jet jets[NUMBER_OF_SEEDS], unsigned char* numberOfSeedsFound, char etaShift)
{
  //int nBinsX = caloGrid.GetNbinsX();
  //int nBinsY = caloGrid.GetNbinsY();

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
  seedFinderPhiScanLoop: for (char iPhi = 0; iPhi < PHI_GRID_SIZE; iPhi++)
  //seedFinderPhiScanLoop: for (char iPhi = 0; iPhi < 1; iPhi++)
  {
    //#pragma HLS unroll
    #if ETA_GRID_SIZE > 256
    #error "Eta grid size is above 256, looping variable not big enough"
    #endif
    //Excluding eta range in which the grid falls out of bounds
    seedFinderEtaScanLoop: for (char iEta = ETA_JET_SIZE/2; iEta < ETA_GRID_SIZE - ETA_JET_SIZE/2; iEta++)
    {
      unsigned short int centralPt = caloGrid[iEta][iPhi]; //caloGrid.GetBinContent(iEta, iPhi);
      if (centralPt < SEED_THRESHOLD) continue;
      bool isLocalMaximum = true;

      // Scanning through the grid centered on the seed
      checkMaximumEtaLoop: for (char etaIndex = -ETA_JET_SIZE/2; etaIndex <= ETA_JET_SIZE/2; etaIndex++)
      {
        checkMaximumPhiLoop: for (char phiIndex = -PHI_JET_SIZE/2; phiIndex <= PHI_JET_SIZE/2; phiIndex++)
        {
          if ((etaIndex == 0) && (phiIndex == 0)) continue;
          char iEtaTmp = iEta + etaIndex;
          char iPhiTmp = iPhi + phiIndex;
          unsigned int towerEnergy = getTowerEnergy(caloGrid, iEtaTmp, iPhiTmp);
          if (centralPt < towerEnergy) continue; 
          if (phiIndex > 0) 
          {
            if (phiIndex >= -etaIndex)
            {
              isLocalMaximum = ((isLocalMaximum) && (centralPt > towerEnergy));
            }
          } else 
          {
            if (phiIndex > -etaIndex)
            {
              isLocalMaximum = ((isLocalMaximum) && (centralPt > towerEnergy));
            }
          }
        }
      }

      if (isLocalMaximum)
      {
        Jet aJet;
        aJet.iEta = iEta;
        aJet.iPhi = iPhi;
        buildJetFromSeed(caloGrid, &aJet);
        jets[jetIdx].pt = aJet.pt;
        jets[jetIdx].iEta = aJet.iEta;
        jets[jetIdx].iPhi = aJet.iPhi;
        jets[jetIdx].iEta += etaShift;
        jetIdx++;
        if (jetIdx >= NUMBER_OF_SEEDS) {
          *numberOfSeedsFound = jetIdx;
          return;
        } 
      }
    }
  }
  *numberOfSeedsFound = jetIdx;
  return;
}

void buildJetFromSeed(const CaloGrid caloGrid, Jet* jet) 
{
  //int iEta = std::get<0>(seed);
  //int iPhi = std::get<1>(seed);

  //int etaHalfSize = (int) this -> _jetIEtaSize/2;
  //int phiHalfSize = (int) this -> _jetIPhiSize/2;

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
  //reco::Candidate::LorentzVector ptVector;
  jet -> pt = ptSum;
}

void hls_main(const CaloGrid inCaloGrid, const char inEtaShift, Jet inJets[NUMBER_OF_SEEDS], unsigned char *numberOfJetsFound) 
{
  CaloGrid lCaloGrid;
  char lEtaShift = inEtaShift;
  unsigned char numberOfSeedsFound = 0;
  Jet jets[NUMBER_OF_SEEDS];
  
  // copying grid
  //gridCopyEtaLoop: for (char iEtaIndex = 0; iEtaIndex < ETA_GRID_SIZE; iEtaIndex++) 
  //{
  // gridCopyPhiLoop: for (char iPhiIndex = 0; iPhiIndex < PHI_GRID_SIZE; iPhiIndex++) 
  // {
  //   lCaloGrid[iEtaIndex][iPhiIndex] = inCaloGrid[iEtaIndex][iPhiIndex];
  // }
  //}

  // initialising internal jets
  //jetInitialisationLoop: for (unsigned char jetIndex = 0; jetIndex < NUMBER_OF_SEEDS; jetIndex++) 
  //{
  //  jets[jetIndex].pt = 0;
  //  jets[jetIndex].iEta = 0;
  //  jets[jetIndex].iPhi = 0;
  //}

  // computing jets
  buildJets(inCaloGrid, jets, &numberOfSeedsFound, lEtaShift);

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