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
  return 0;
}