#include "HLS_Test.h"

#define PHI_GRID_SIZE 82
#define ETA_GRID_SIZE 9
#define PHI_JET_SIZE 9
#define ETA_JET_SIZE 9
#define NUMBER_OF_SEEDS 64
#define NUMBER_OF_JETS 16
#define SEED_THRESHOLD 5

typedef unsigned short int CaloGrid[ETA_GRID_SIZE][PHI_GRID_SIZE];

class Jet {
  public:
  unsigned short int pt;
  char iPhi;
  char iEta;
}

unsigned short int getTowerEnergy(const CaloGrid caloGrid, int iEta, int iPhi)
{
  // We return the pt of a certain bin in the calo grid, taking account of the phi periodicity when overflowing (e.g. phi > phiSize), and returning 0 for the eta out of bounds

  //int nBinsEta = caloGrid.GetNbinsX();
  //int nBinsPhi = caloGrid.GetNbinsY();
  while (iPhi < 0) {
    iPhi += nBinsPhi;
  }
  while (iPhi > nBinsPhi - 1) {
    iPhi -= nBinsPhi;
  }
  if (iEta < 0) {
    //std::cout << "Returning (pt, ieta, iphi): " << "(" << 0 << ", " << iEta << ", " << iPhi << ")" << std::endl;
    return 0;
  }
  if (iEta > nBinsEta - 1) {
    //std::cout << "Returning (pt, ieta, iphi): " << "(" << 0 << ", " << iEta << ", " << iPhi << ")" << std::endl;
    return 0;
  }
    //std::cout << "Returning (pt, ieta, iphi): " << "(" << caloGrid.GetBinContent(iEta, iPhi) << ", " << iEta << ", " << iPhi << ")" << std::endl;
  return caloGrid[iEta][iPhi];
}

void findSeeds(const CaloGrid caloGrid, Jet seeds[NUMBER_OF_SEEDS]) 
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
      if (centralPt <= SEED_THRESHOLD) continue;
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
        jets[jetIdx].pt = centralPt;
        jets[jetIdx].iEta = iEta;
        jets[jetIdx].iPhi = iPhi;
        jetIdx++;
        if (jetIdx >= NUMBER_OF_SEEDS) return;
      }
    }
  }
  return;
}

void buildJetsFromSeeds(const CaloGrid & caloGrid, const std::vector<std::tuple<int, int>> & seeds, bool killZeroPt)
{

  // For each seed take a grid centered on the seed of the size specified by the user
  // Sum the pf in the grid, that will be the pt of the l1t jet. Eta and phi of the jet is taken from the seed.
  std::vector<reco::CaloJet> jets;
  for (const auto& seed: seeds)
  {
    reco::CaloJet jet = this -> _buildJetFromSeed(caloGrid, seed);
    this -> _subtract9x9Pileup(caloGrid, jet);
    //killing jets with 0 pt
    if ((this -> _vetoZeroPt) && (jet.pt() <= 0)) continue;
    jets.push_back(jet);
  }

  return jets;
}

char hls_main(const CaloGrid caloGrid, short int centralIEta, Jet jets[NUMBER_OF_JETS]) {
  
  //TODO: COPY calogrid TO LOCAL MEMORY ~~~ calogridlocal = memcpy(caloGrid)
  Jets seeds[NUMBER_OF_SEEDS];
  findSeeds(caloGrid, seeds);
  

}