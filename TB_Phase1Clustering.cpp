#include "HLS_Phase1Clustering.h"
#include "TB_Phase1Clustering.h"
#include <iostream>
#include <csignal>
#include <assert.h>
#include <fstream>

bool readCaloGridBufferFromFile(const std::string &filepath, CaloGrid caloGrid)
{
  std::ifstream inFile(filepath);
  if (inFile.is_open())
  {
    for (uint x = 0; x < PHI_GRID_SIZE; x++)
    {
      for(uint y = 0; y < ETA_GRID_SIZE; y++)
      {
        inFile >> caloGrid[y][x];
      }
    }
    inFile.close()
    return true;
  } else {
    return false;
  }
}

void cleanJets(Jets jets)
{
  for (unsigned char jetIdx = 0; jetIdx < NUMBER_OF_SEEDS; jetIdx++) 
  {
    jets[jetIdx].pt = 0;
    jets[jetIdx].iEta = 0;
    jets[jetIdx].iPhi = 0;
  }
  return;
}

void cleanGrid(CaloGrid grid, pt_type value) 
{
  for (unsigned char iEtaIndex = 0; iEtaIndex < ETA_GRID_SIZE; iEtaIndex++) 
  {
    for (unsigned char iPhiIndex = 0; iPhiIndex < PHI_GRID_SIZE; iPhiIndex++) 
    {
      grid[iEtaIndex][iPhiIndex] = value;
    }
  }
  return;
}

int main(int argc, char const *argv[])
{
  
  return 0;
}

void test5x5()
{
  CaloGrid grid;

  Jets jets;
  //unsigned char numberOfJetsFound = 0;
  char etaShift = 10;

  // testing seed finding
  std::cout << "TEST 1" << std::endl;
  cleanJets(jets);
  cleanGrid(grid);
  hls_main(grid, etaShift, jets);
  for (int x = 0 ; x < NUMBER_OF_SEEDS ; x++)
  {
    std::cout << x << ":";
    std::cout << "\tpt: "<< +jets[x].pt;
    std::cout << "\tiEta: " << +jets[x].iEta;
    std::cout << "\tiPhi: " << +jets[x].iPhi << std::endl;
    // assert(jets[x].pt == 0);
    // assert(jets[x].iEta == ETA_GRID_SIZE/2 + etaShift);
    // assert(jets[x].iPhi == x);
  }

  std::cout << "TEST 2" << std::endl;
  cleanJets(jets);
  grid[2][2] = 10;
  hls_main(grid, etaShift, jets);
  for (int x = 0 ; x < NUMBER_OF_SEEDS ; x++)
  {
    std::cout << x << ":";
    std::cout << "\tpt: "<< +jets[x].pt;
    std::cout << "\tiEta: " << +jets[x].iEta;
    std::cout << "\tiPhi: " << +jets[x].iPhi << std::endl;
    // assert(jets[x].pt == ( (x == 2) ? 10 : 0 ) );
    // assert(jets[x].iEta == ETA_GRID_SIZE/2 + etaShift);
    // assert(jets[x].iPhi == x);
  }

  // cleanGrid(grid);
  // grid[4][5] = 10;
  // grid[3][5] = 10;
  // hls_main(grid, etaShift, jets);
  // for (int x = 0 ; x < NUMBER_OF_SEEDS ; x++)
  // {
  //   assert(jets[x].pt == ( (x == 5) ? 20 : 0 ) );
  //   assert(jets[x].iEta == 4 + etaShift);
  //   assert(jets[x].iPhi == x);
  // }

  // cleanGrid(grid);
  // grid[4][5] = 10;
  // grid[4][6] = 10;
  // hls_main(grid, etaShift, jets);
  // for (int x = 0 ; x < NUMBER_OF_SEEDS ; x++)
  // {
  //   assert(jets[x].pt == ( (x == 6) ? 20 : 0 ) );
  //   assert(jets[x].iEta == 4 + etaShift);
  //   assert(jets[x].iPhi == x);
  // }
  
  // cleanGrid(grid);
  // grid[4][5] = 10;
  // grid[4][4] = 10;
  // hls_main(grid, etaShift, jets);
  // for (int x = 0 ; x < NUMBER_OF_SEEDS ; x++)
  // {
  //   assert(jets[x].pt == ( (x == 5) ? 20 : 0 ) );
  //   assert(jets[x].iEta == 4 + etaShift);
  //   assert(jets[x].iPhi == x);
  // }

  // cleanGrid(grid, SEED_THRESHOLD - 1);
  // hls_main(grid, 10, jets);
  // for (int x = 0 ; x < NUMBER_OF_SEEDS ; x++)
  // {
  //   assert(jets[x].pt == 0);
  //   assert(jets[x].iEta == 4 + etaShift);
  //   assert(jets[x].iPhi == x);
  // }

  // cleanGrid(grid, 1);
  // grid[4][5] = 5;
  // hls_main(grid, etaShift, jets);
  // for (int x = 0 ; x < NUMBER_OF_SEEDS ; x++)
  // {
  //   assert(jets[x].pt == ( (x == 5) ? 85 : 0 ) );
  //   assert(jets[x].iEta == 4 + etaShift);
  //   assert(jets[x].iPhi == x);
  // }
  std::cout << "All test have been successfully passed." << std::endl;
}

void test9x9()
{
  CaloGrid grid;

  Jet jets[NUMBER_OF_SEEDS];
  //unsigned char numberOfJetsFound = 0;
  char etaShift = 10;

  // testing seed finding
  cleanGrid(grid);
  hls_main(grid, etaShift, jets);
  for (int x = 0 ; x < NUMBER_OF_SEEDS ; x++)
  {
    assert(jets[x].pt == 0);
    assert(jets[x].iEta == 4 + etaShift);
    assert(jets[x].iPhi == x);
  }

  grid[4][5] = 10;
  hls_main(grid, etaShift, jets);
  for (int x = 0 ; x < NUMBER_OF_SEEDS ; x++)
  {

    assert(jets[x].pt == ( (x == 5) ? 10 : 0 ) );
    assert(jets[x].iEta == 4 + etaShift);
    assert(jets[x].iPhi == x);
  }
  

  cleanGrid(grid);
  grid[4][5] = 10;
  grid[3][5] = 10;
  hls_main(grid, etaShift, jets);
  for (int x = 0 ; x < NUMBER_OF_SEEDS ; x++)
  {
    assert(jets[x].pt == ( (x == 5) ? 20 : 0 ) );
    assert(jets[x].iEta == 4 + etaShift);
    assert(jets[x].iPhi == x);
  }

  cleanGrid(grid);
  grid[4][5] = 10;
  grid[4][6] = 10;
  hls_main(grid, etaShift, jets);
  for (int x = 0 ; x < NUMBER_OF_SEEDS ; x++)
  {
    assert(jets[x].pt == ( (x == 6) ? 20 : 0 ) );
    assert(jets[x].iEta == 4 + etaShift);
    assert(jets[x].iPhi == x);
  }
  
  cleanGrid(grid);
  grid[4][5] = 10;
  grid[4][4] = 10;
  hls_main(grid, etaShift, jets);
  for (int x = 0 ; x < NUMBER_OF_SEEDS ; x++)
  {
    assert(jets[x].pt == ( (x == 5) ? 20 : 0 ) );
    assert(jets[x].iEta == 4 + etaShift);
    assert(jets[x].iPhi == x);
  }

  cleanGrid(grid, SEED_THRESHOLD - 1);
  hls_main(grid, 10, jets);
  for (int x = 0 ; x < NUMBER_OF_SEEDS ; x++)
  {
    assert(jets[x].pt == 0);
    assert(jets[x].iEta == 4 + etaShift);
    assert(jets[x].iPhi == x);
  }

  cleanGrid(grid, 1);
  grid[4][5] = 5;
  hls_main(grid, etaShift, jets);
  for (int x = 0 ; x < NUMBER_OF_SEEDS ; x++)
  {
    assert(jets[x].pt == ( (x == 5) ? 85 : 0 ) );
    assert(jets[x].iEta == 4 + etaShift);
    assert(jets[x].iPhi == x);
  }
  std::cout << "All test have been successfully passed." << std::endl;
}

void runJetFinder(const CaloGrid caloGrid, TMJets tmJets)
{
  for (unsigned char iEtaIndex = 0; iEtaIndex < ETA_GRID_SIZE; iEtaIndex++) 
  {
    CaloGridPhiVector phiVector;
    for (unsigned char iPhiIndex = 0; iPhiIndex < PHI_GRID_SIZE; iPhiIndex++) 
    {
      phiVector[iPhiIndex] = caloGrid[iEtaIndex][iPhiIndex];
    }
    if (iEtaIndex == 0) hls_main(phiVector, tmJets[iEtaIndex], true);
    else hls_main(phiVector, jets, false);
  }
}
