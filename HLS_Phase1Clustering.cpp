#include "HLS_Phase1Clustering.h"

unsigned short int getTowerEnergy(const CaloGrid caloGrid, char iEta, char iPhi)
{
  // We return the pt of a certain bin in the calo grid, taking account of the phi periodicity when overflowing (e.g. phi > phiSize), and returning 0 for the eta out of bounds

  //int nBinsEta = caloGrid.GetNbinsX();
  //int nBinsPhi = caloGrid.GetNbinsY();
  while (iPhi < 0) {
    iPhi += PHI_GRID_SIZE;
  }
  while (iPhi > PHI_GRID_SIZE - 1) {
    iPhi -= PHI_GRID_SIZE;
  }
  if (iEta < 0) {
    //std::cout << "Returning (pt, ieta, iphi): " << "(" << 0 << ", " << iEta << ", " << iPhi << ")" << std::endl;
    return 0;
  }
  if (iEta > ETA_GRID_SIZE - 1) {
    //std::cout << "Returning (pt, ieta, iphi): " << "(" << 0 << ", " << iEta << ", " << iPhi << ")" << std::endl;
    return 0;
  }
    //std::cout << "Returning (pt, ieta, iphi): " << "(" << caloGrid.GetBinContent(iEta, iPhi) << ", " << iEta << ", " << iPhi << ")" << std::endl;
  return caloGrid[iEta][iPhi];
}

void findSeeds(const CaloGrid caloGrid, Jet seeds[NUMBER_OF_SEEDS], unsigned char* numberOfSeedsFound) 
{
  //int nBinsX = caloGrid.GetNbinsX();
  //int nBinsY = caloGrid.GetNbinsY();
  #if NUMBER_OF_SEEDS > 256
  #pragma message "Working with more than 256 seeds, moving to 2-byte index counter, please do not go over 65536 seeds or this may break"
  unsigned int jetIdx = 0;
  #else
  char jetIdx = 0;
  #endif

  //int etaHalfSize = (int) this -> _jetIEtaSize/2;
  //int phiHalfSize = (int) this -> _jetIPhiSize/2;

  // for each point of the grid check if it is a local maximum
  // to do so I take a point, and look if is greater than the points around it (in the 9x9 neighborhood)
  // to prevent mutual exclusion, I check greater or equal for points above and right to the one I am considering (including the top-left point)
  // to prevent mutual exclusion, I check greater for points below and left to the one I am considering (including the bottom-right point)

  #if PHI_GRID_SIZE > 256
  #error "Phi grid size is above 256, looping variable not big enough"
  #endif 
  for (char iPhi = 0; iPhi < PHI_GRID_SIZE; iPhi++)
  {
    #if ETA_GRID_SIZE > 256
    #error "Eta grid size is above 256, looping variable not big enough"
    #endif
    //Excluding eta range in which the grid falls out of bounds
    for (char iEta = ETA_JET_SIZE/2; iEta < ETA_GRID_SIZE - ETA_JET_SIZE/2; iEta++)
    {
      unsigned short int centralPt = caloGrid[iEta][iPhi]; //caloGrid.GetBinContent(iEta, iPhi);
      if (centralPt < SEED_THRESHOLD) continue;
      bool isLocalMaximum = true;

      // Scanning through the grid centered on the seed
      for (char etaIndex = -ETA_JET_SIZE/2; etaIndex <= ETA_JET_SIZE/2; etaIndex++)
      {
        for (char phiIndex = -PHI_JET_SIZE/2; phiIndex <= PHI_JET_SIZE/2; phiIndex++)
        {
          if ((etaIndex == 0) && (phiIndex == 0)) continue;
          if (phiIndex > 0) {
            if (phiIndex > -etaIndex){
              isLocalMaximum = ((isLocalMaximum) && (centralPt > getTowerEnergy(caloGrid, iEta + etaIndex, iPhi + phiIndex)));
            } else {
              isLocalMaximum = ((isLocalMaximum) && (centralPt >= getTowerEnergy(caloGrid, iEta + etaIndex, iPhi + phiIndex)));
            }
          } else {
            if (phiIndex >= -etaIndex){
              isLocalMaximum = ((isLocalMaximum) && (centralPt > getTowerEnergy(caloGrid, iEta + etaIndex, iPhi + phiIndex)));
            } else {
              isLocalMaximum = ((isLocalMaximum) && (centralPt >= getTowerEnergy(caloGrid, iEta + etaIndex, iPhi + phiIndex)));
            }
          }
        }
      }

      if (isLocalMaximum)
      {
        //emplace_back(std::make_tuple(iEta, iPhi));
        seeds[jetIdx].pt = centralPt;
        seeds[jetIdx].iEta = iEta;
        seeds[jetIdx].iPhi = iPhi;
        jetIdx++;
        if (jetIdx >= NUMBER_OF_SEEDS) return;
      }
    }
  }
  *numberOfSeedsFound = jetIdx;
  return;
}

void buildJetsFromSeeds(const CaloGrid caloGrid, Jet seeds[NUMBER_OF_SEEDS], unsigned char numberOfSeedsFound)
{

  // For each seed take a grid centered on the seed of the size specified by the user
  // Sum the pf in the grid, that will be the pt of the l1t jet. Eta and phi of the jet is taken from the seed.
  for (char x = 0; x < numberOfSeedsFound; x++)
  {
    buildJetFromSeed(caloGrid, &seeds[x]);
  }
}

void buildJetFromSeed(const CaloGrid caloGrid, Jet* jet) 
{
  //int iEta = std::get<0>(seed);
  //int iPhi = std::get<1>(seed);

  //int etaHalfSize = (int) this -> _jetIEtaSize/2;
  //int phiHalfSize = (int) this -> _jetIPhiSize/2;

  // Scanning through the grid centered on the seed

  unsigned short int ptSum;

  for (char iEtaIndex = -ETA_JET_SIZE/2; iEtaIndex <= ETA_JET_SIZE/2; iEtaIndex++)
  {
    for (char iPhiIndex = -PHI_JET_SIZE/2; iPhiIndex <= PHI_JET_SIZE/2; iPhiIndex++)
    {
      ptSum += getTowerEnergy(caloGrid, jet -> iEta + iEtaIndex, jet -> iPhi + iPhiIndex);
    }
  }

  // Creating a jet with eta phi centered on the seed and momentum equal to the sum of the pt of the components    
  //reco::Candidate::LorentzVector ptVector;
  jet -> pt = ptSum;
}

void applyEtaShift(Jet seeds[NUMBER_OF_SEEDS], unsigned char numberOfSeedsFound, char etaShift)
{
  for (char x = 0; x < numberOfSeedsFound; x++) 
  {
    seeds[x].iEta = seeds[x].iEta + etaShift;
  }
}

void hls_main(const CaloGrid inCaloGrid, const char inEtaShift, Jet inJets[NUMBER_OF_SEEDS], unsigned char *numberOfJetsFound) 
{
  CaloGrid lCaloGrid;
  for (char iEtaIndex = 0; iEtaIndex < ETA_GRID_SIZE; iEtaIndex++) 
  {
   for (char iPhiIndex = 0; iPhiIndex < PHI_GRID_SIZE; iPhiIndex++) 
   {
     lCaloGrid[iEtaIndex][iPhiIndex] = inCaloGrid[iEtaIndex][iPhiIndex];
   }
  }
  char lEtaShift = inEtaShift;
  unsigned char numberOfSeedsFound = 0;
  Jet seeds[NUMBER_OF_SEEDS];
  findSeeds(lCaloGrid, seeds, &numberOfSeedsFound);
  buildJetsFromSeeds(lCaloGrid, seeds, numberOfSeedsFound);
  applyEtaShift(seeds, numberOfSeedsFound, lEtaShift);

  for (char jetIndex = 0; jetIndex < numberOfSeedsFound; jetIndex++) 
  {
    inJets[jetIndex].pt = seeds[jetIndex].pt;
    inJets[jetIndex].iEta = seeds[jetIndex].iEta;
    inJets[jetIndex].iPhi = seeds[jetIndex].iPhi;
  }
  *numberOfJetsFound = numberOfSeedsFound;
}