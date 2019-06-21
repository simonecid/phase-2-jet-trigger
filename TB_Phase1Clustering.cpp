#include "HLS_Phase1Clustering.h"
#include "TB_Phase1Clustering.h"
#include <iostream>
#include <csignal>
#include <assert.h>
#include <fstream>

// void printCaloGrid(const CaloGrid caloGrid)
// {
//   std::cout << "Printing grid" << std::endl;
//   for (unsigned char iPhi = 0; iPhi < PHI_GRID_SIZE; iPhi++)
//   {
//     for (unsigned char iEta = 0; iEta < ETA_GRID_SIZE; iEta++)
//     {
//       std::cout << caloGrid[iPhi][iEta] << " ";
//     }
//     std::cout << std::endl;
//   }
//   std::cout << std::endl;
// }

bool readCaloGridFromFile(const std::string &filepath, CaloGrid caloGrid)
{
  std::ifstream inFile(filepath);
  if (inFile.is_open())
  {
    for(uint y = 0; y < PHI_GRID_SIZE; y++)
    {
      for (uint x = 0; x < ETA_GRID_SIZE; x++)
      {
        inFile >> caloGrid[y][x];
      }
    }
    inFile.close();
    return true;
  } else {
    std::cout << "ERROR: File opening failed." << std::endl;
    exit(1);
  }
}

void clearJets(TMJets jets)
{
  
  for (unsigned char tmIdx = 0; tmIdx < ETA_GRID_SIZE; tmIdx++) 
  {
    for (unsigned char jetIdx = 0; jetIdx < NUMBER_OF_SEEDS; jetIdx++) 
    {
      jets[tmIdx][jetIdx].pt = 0;
      jets[tmIdx][jetIdx].iEta = 0;
      jets[tmIdx][jetIdx].iPhi = 0;
    }
  }
  return;
}

void clearGrid(CaloGrid grid, pt_type value) 
{
  for (unsigned char iEtaIndex = 0; iEtaIndex < ETA_GRID_SIZE; iEtaIndex++) 
  {
    for (unsigned char iPhiIndex = 0; iPhiIndex < PHI_GRID_SIZE; iPhiIndex++) 
    {
      grid[iPhiIndex][iEtaIndex] = value;
    }
  }
  return;
}

int main(int argc, char const *argv[])
{
  test5x5();
  CaloGrid caloGrid;
  TMJets tmJets;
  readCaloGridFromFile("/users/sb17498/FPGAClustering/event.txt", caloGrid);
  runJetFinder(caloGrid, tmJets);
  for (unsigned int tmIndex = 0; tmIndex < ETA_GRID_SIZE; tmIndex++)
  {
    for (unsigned int jetIndex = 0; jetIndex < NUMBER_OF_SEEDS; jetIndex++)
    {
      const Jet & lJet = tmJets[tmIndex][jetIndex];
      
      assert(lJet.iEta == tmIndex);
      assert(lJet.iPhi == jetIndex);
      // if (lJet.pt > 0)
      // {
      //   std::cout << "----- Found a jet -----" << std::endl;
      //   std::cout << "pt: " << +lJet.pt << std::endl;
      //   std::cout << "iEta: " << +lJet.iEta << std::endl;
      //   std::cout << "iPhi: " << +lJet.iPhi << std::endl;
      // }
    }
  }
  return 0;
}

void test5x5()
{
  CaloGrid grid;

  TMJets jets;
  //unsigned char numberOfJetsFound = 0;
  // testing seed finding
  std::cout << "TEST 1" << std::endl;
  clearJets(jets);
  clearGrid(grid);
  runJetFinder(grid, jets);
  for (unsigned int tmIndex = 0; tmIndex < PHI_GRID_SIZE; tmIndex++)
  {
    for (unsigned int jetIndex = 0; jetIndex < NUMBER_OF_SEEDS; jetIndex++)
    {
      if (jets[tmIndex][jetIndex].pt > 0)
      {
        std::cout << "(" << tmIndex << ", " << jetIndex << ")" << ":";
        std::cout << "\tpt: "<< +jets[tmIndex][jetIndex].pt;
        std::cout << "\tiEta: " << +jets[tmIndex][jetIndex].iEta;
        std::cout << "\tiPhi: " << +jets[tmIndex][jetIndex].iPhi << std::endl;
      }
      assert(jets[tmIndex][jetIndex].pt == 0);
      assert(jets[tmIndex][jetIndex].iEta == jetIndex);
      assert(jets[tmIndex][jetIndex].iPhi == tmIndex);
    }
  }

  std::cout << "TEST 2" << std::endl;
  clearJets(jets);
  grid[10][20] = 10;
  runJetFinder(grid, jets);
  for (unsigned int tmIndex = 0; tmIndex < PHI_GRID_SIZE; tmIndex++)
  {
    for (unsigned int jetIndex = 0; jetIndex < NUMBER_OF_SEEDS; jetIndex++)
    {
      if (jets[tmIndex][jetIndex].pt > 0)
      {
        std::cout << "(" << tmIndex << ", " << jetIndex << ")" << ":";
        std::cout << "\tpt: "<< +jets[tmIndex][jetIndex].pt;
        std::cout << "\tiEta: " << +jets[tmIndex][jetIndex].iEta;
        std::cout << "\tiPhi: " << +jets[tmIndex][jetIndex].iPhi << std::endl;
      }
      assert(jets[tmIndex][jetIndex].pt == ( ((tmIndex == 20) && (jetIndex == 10)) ? 10 : 0 ) );
      assert(jets[tmIndex][jetIndex].iEta == jetIndex);
      assert(jets[tmIndex][jetIndex].iPhi == tmIndex);
    }
  }

  std::cout << "TEST 3" << std::endl;
  clearGrid(grid);
  clearJets(jets);
  grid[4][5] = 10;
  grid[3][5] = 10;
  runJetFinder(grid, jets);
  for (unsigned int tmIndex = 0; tmIndex < PHI_GRID_SIZE; tmIndex++)
  {
    for (unsigned int jetIndex = 0; jetIndex < NUMBER_OF_SEEDS; jetIndex++)
    {
      if (jets[tmIndex][jetIndex].pt > 0)
      {
        std::cout << "(" << tmIndex << ", " << jetIndex << ")" << ":";
        std::cout << "\tpt: "<< +jets[tmIndex][jetIndex].pt;
        std::cout << "\tiEta: " << +jets[tmIndex][jetIndex].iEta;
        std::cout << "\tiPhi: " << +jets[tmIndex][jetIndex].iPhi << std::endl;
      }
      assert(jets[tmIndex][jetIndex].pt == ( ((tmIndex == 4) && (jetIndex == 5)) ? 20 : 0 ) );
      assert(jets[tmIndex][jetIndex].iEta == tmIndex);
      assert(jets[tmIndex][jetIndex].iPhi == jetIndex);
    }
  }

  std::cout << "TEST 4" << std::endl;
  clearGrid(grid);
  clearJets(jets);
  grid[4][5] = 10;
  grid[4][6] = 10;
  runJetFinder(grid, jets);
  for (unsigned int tmIndex = 0; tmIndex < PHI_GRID_SIZE; tmIndex++)
  {
    for (unsigned int jetIndex = 0; jetIndex < NUMBER_OF_SEEDS; jetIndex++)
    {
      if (jets[tmIndex][jetIndex].pt > 0)
      {
        std::cout << "(" << tmIndex << ", " << jetIndex << ")" << ":";
        std::cout << "\tpt: "<< +jets[tmIndex][jetIndex].pt;
        std::cout << "\tiEta: " << +jets[tmIndex][jetIndex].iEta;
        std::cout << "\tiPhi: " << +jets[tmIndex][jetIndex].iPhi << std::endl;
      }
      assert(jets[tmIndex][jetIndex].pt == ( ((tmIndex == 4) && (jetIndex == 6)) ? 20 : 0 ) );
      assert(jets[tmIndex][jetIndex].iEta == tmIndex);
      assert(jets[tmIndex][jetIndex].iPhi == jetIndex);
    }
  }
  
  std::cout << "TEST 5" << std::endl;
  clearGrid(grid);
  clearJets(jets);
  grid[4][5] = 10;
  grid[4][4] = 10;
  runJetFinder(grid, jets);
  for (unsigned int tmIndex = 0; tmIndex < PHI_GRID_SIZE; tmIndex++)
  {
    for (unsigned int jetIndex = 0; jetIndex < NUMBER_OF_SEEDS; jetIndex++)
    {
      if (jets[tmIndex][jetIndex].pt > 0)
      {
        std::cout << "(" << tmIndex << ", " << jetIndex << ")" << ":";
        std::cout << "\tpt: "<< +jets[tmIndex][jetIndex].pt;
        std::cout << "\tiEta: " << +jets[tmIndex][jetIndex].iEta;
        std::cout << "\tiPhi: " << +jets[tmIndex][jetIndex].iPhi << std::endl;
      }
      assert(jets[tmIndex][jetIndex].pt == ( ((tmIndex == 4) && (jetIndex == 5)) ? 20 : 0 ) );
      assert(jets[tmIndex][jetIndex].iEta == tmIndex);
      assert(jets[tmIndex][jetIndex].iPhi == jetIndex);
    }
  }

  std::cout << "TEST 6" << std::endl;
  clearGrid(grid, SEED_THRESHOLD - 1);
  clearJets(jets);
  runJetFinder(grid, jets);
  for (unsigned int tmIndex = 0; tmIndex < PHI_GRID_SIZE; tmIndex++)
  {
    for (unsigned int jetIndex = 0; jetIndex < NUMBER_OF_SEEDS; jetIndex++)
    {
      if (jets[tmIndex][jetIndex].pt > 0)
      {
        std::cout << "(" << tmIndex << ", " << jetIndex << ")" << ":";
        std::cout << "\tpt: "<< +jets[tmIndex][jetIndex].pt;
        std::cout << "\tiEta: " << +jets[tmIndex][jetIndex].iEta;
        std::cout << "\tiPhi: " << +jets[tmIndex][jetIndex].iPhi << std::endl;
      }
      assert(jets[tmIndex][jetIndex].pt == 0);
      assert(jets[tmIndex][jetIndex].iEta == tmIndex);
      assert(jets[tmIndex][jetIndex].iPhi == jetIndex);
    }
  }

  std::cout << "TEST 7" << std::endl;
  clearGrid(grid, SEED_THRESHOLD - 1);
  clearJets(jets);
  grid[4][5] = SEED_THRESHOLD;
  runJetFinder(grid, jets);
  for (unsigned int tmIndex = 0; tmIndex < PHI_GRID_SIZE; tmIndex++)
  {
    for (unsigned int jetIndex = 0; jetIndex < NUMBER_OF_SEEDS; jetIndex++)
    {
      if (jets[tmIndex][jetIndex].pt > 0)
      {
        std::cout << "(" << tmIndex << ", " << jetIndex << ")" << ":";
        std::cout << "\tpt: "<< +jets[tmIndex][jetIndex].pt;
        std::cout << "\tiEta: " << +jets[tmIndex][jetIndex].iEta;
        std::cout << "\tiPhi: " << +jets[tmIndex][jetIndex].iPhi << std::endl;
      }
      assert(jets[tmIndex][jetIndex].pt == ( ((tmIndex == 4) && (jetIndex == 5)) ? 101 : 0 ) );
      assert(jets[tmIndex][jetIndex].iEta == tmIndex);
      assert(jets[tmIndex][jetIndex].iPhi == jetIndex);
    }
  }
  
  std::cout << "All test have been successfully passed." << std::endl;
}

// void test9x9()
// {
//   CaloGrid grid;

//   Jet jets[NUMBER_OF_SEEDS];
//   //unsigned char numberOfJetsFound = 0;
//   char etaShift = 10;

//   // testing seed finding
//   clearGrid(grid);
//   hls_jet_clustering(grid, etaShift, jets);
//   for (int x = 0 ; x < NUMBER_OF_SEEDS ; x++)
//   {
//     assert(jets[x].pt == 0);
//     assert(jets[x].iEta == 4 + etaShift);
//     assert(jets[x].iPhi == x);
//   }

//   grid[4][5] = 10;
//   hls_jet_clustering(grid, etaShift, jets);
//   for (int x = 0 ; x < NUMBER_OF_SEEDS ; x++)
//   {

//     assert(jets[x].pt == ( (x == 5) ? 10 : 0 ) );
//     assert(jets[x].iEta == 4 + etaShift);
//     assert(jets[x].iPhi == x);
//   }
  

//   clearGrid(grid);
//   grid[4][5] = 10;
//   grid[3][5] = 10;
//   hls_jet_clustering(grid, etaShift, jets);
//   for (int x = 0 ; x < NUMBER_OF_SEEDS ; x++)
//   {
//     assert(jets[x].pt == ( (x == 5) ? 20 : 0 ) );
//     assert(jets[x].iEta == 4 + etaShift);
//     assert(jets[x].iPhi == x);
//   }

//   clearGrid(grid);
//   grid[4][5] = 10;
//   grid[4][6] = 10;
//   hls_jet_clustering(grid, etaShift, jets);
//   for (int x = 0 ; x < NUMBER_OF_SEEDS ; x++)
//   {
//     assert(jets[x].pt == ( (x == 6) ? 20 : 0 ) );
//     assert(jets[x].iEta == 4 + etaShift);
//     assert(jets[x].iPhi == x);
//   }
  
//   clearGrid(grid);
//   grid[4][5] = 10;
//   grid[4][4] = 10;
//   hls_jet_clustering(grid, etaShift, jets);
//   for (int x = 0 ; x < NUMBER_OF_SEEDS ; x++)
//   {
//     assert(jets[x].pt == ( (x == 5) ? 20 : 0 ) );
//     assert(jets[x].iEta == 4 + etaShift);
//     assert(jets[x].iPhi == x);
//   }

//   clearGrid(grid, SEED_THRESHOLD - 1);
//   hls_jet_clustering(grid, 10, jets);
//   for (int x = 0 ; x < NUMBER_OF_SEEDS ; x++)
//   {
//     assert(jets[x].pt == 0);
//     assert(jets[x].iEta == 4 + etaShift);
//     assert(jets[x].iPhi == x);
//   }

//   clearGrid(grid, 1);
//   grid[4][5] = 5;
//   hls_jet_clustering(grid, etaShift, jets);
//   for (int x = 0 ; x < NUMBER_OF_SEEDS ; x++)
//   {
//     assert(jets[x].pt == ( (x == 5) ? 85 : 0 ) );
//     assert(jets[x].iEta == 4 + etaShift);
//     assert(jets[x].iPhi == x);
//   }
//   std::cout << "All test have been successfully passed." << std::endl;
// }

void runJetFinder(const CaloGrid caloGrid, TMJets tmJets)
{
  //sending the phi slices
  CaloGridPhiSlice phiSlice;
  for (unsigned char iPhiIndex = 0; iPhiIndex < PHI_GRID_SIZE; iPhiIndex++) 
  {
    for (unsigned char iEtaIndex = 0; iEtaIndex < ETA_GRID_SIZE; iEtaIndex++) 
    {
      phiSlice[iEtaIndex] = caloGrid[iPhiIndex][iEtaIndex];
    }
    // the first phi slice must reset the algo
    if (iPhiIndex == 0) hls_jet_clustering(phiSlice, tmJets[0], true);
    // the first slices (2 if jets is 5 slice wide) do not produce any real jet
    else if (iPhiIndex < PHI_JET_SIZE/2) hls_jet_clustering(phiSlice, tmJets[0], false);
    // sending slices while receiving jets from the previous ones
    else hls_jet_clustering(phiSlice, tmJets[iPhiIndex - PHI_JET_SIZE/2], false);
  }
  //we need to process the remaining PHI_JET_SIZE/2 slices
  for (unsigned char iPhiIndex = 0; iPhiIndex < PHI_JET_SIZE/2; iPhiIndex++) 
  {
    hls_jet_clustering(phiSlice, tmJets[PHI_GRID_SIZE - PHI_JET_SIZE/2 + iPhiIndex], false);
  }
  //done
}
