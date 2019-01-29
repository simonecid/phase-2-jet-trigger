#include "HLS_Phase1Clustering.h"
#include <iostream>
#include <csignal>
#include <assert.h>

void cleanGrid(CaloGrid grid, unsigned short int value=0) 
{
  for (char iEtaIndex = 0; iEtaIndex < ETA_GRID_SIZE; iEtaIndex++) 
  {
    for (char iPhiIndex = 0; iPhiIndex < PHI_GRID_SIZE; iPhiIndex++) 
    {
      grid[iEtaIndex][iPhiIndex] = value;
    }
  }
  return;
}

int main(int argc, char const *argv[])
{
  CaloGrid grid;

  Jet jets[NUMBER_OF_SEEDS];
  unsigned char numberOfJetsFound = 0;
  char etaShift = 10;

  // testing seed finding
  cleanGrid(grid);
  hls_main(grid, 10, jets, &numberOfJetsFound);
  assert(numberOfJetsFound == 0);

  grid[4][10] = 10;
  hls_main(grid, 10, jets, &numberOfJetsFound);
  assert(numberOfJetsFound == 1);
  assert(jets[0].pt == 10);
  assert(jets[0].iEta == 14);
  assert(jets[0].iPhi == 10);

  cleanGrid(grid);
  grid[4][10] = 10;
  grid[5][10] = 10;
  hls_main(grid, 10, jets, &numberOfJetsFound);
  assert(numberOfJetsFound == 0);

  cleanGrid(grid);
  grid[4][10] = 10;
  grid[3][10] = 10;
  hls_main(grid, 10, jets, &numberOfJetsFound);
  assert (numberOfJetsFound == 1);
  assert(jets[0].pt == 20);
  assert(jets[0].iEta == 14);
  assert(jets[0].iPhi == 10);

  cleanGrid(grid);
  grid[4][10] = 10;
  grid[4][11] = 10;
  hls_main(grid, 10, jets, &numberOfJetsFound);
  assert(numberOfJetsFound == 1);
  assert(jets[0].pt == 20);
  assert(jets[0].iEta == 14);
  assert(jets[0].iPhi == 11);
  
  cleanGrid(grid);
  grid[4][10] = 10;
  grid[4][9] = 10;
  hls_main(grid, 10, jets, &numberOfJetsFound);
  assert(numberOfJetsFound == 1);
  assert(jets[0].pt == 20);
  assert(jets[0].iEta == 14);
  assert(jets[0].iPhi == 10);

  cleanGrid(grid, SEED_THRESHOLD - 1);
  hls_main(grid, 10, jets, &numberOfJetsFound);
  assert(numberOfJetsFound == 0);

  cleanGrid(grid, 1);
  grid[4][20] = 5;
  hls_main(grid, 10, jets, &numberOfJetsFound);
  assert(numberOfJetsFound == 1);
  assert(jets[0].pt == 85);
  assert(jets[0].iEta == 14);
  assert(jets[0].iPhi == 20);
  
  std::cout << "All test have been successfully passed." << std::endl;
  return 0;
}