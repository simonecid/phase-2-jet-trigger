#include "HLS_Phase1Clustering.h"
#include "TB_Phase1Clustering.h"
#include <iostream>
#include <csignal>
#include <assert.h>
#include <fstream>

void printCaloGrid(const CaloGrid caloGrid)
{
  std::cout << "Printing grid" << std::endl;
  for (unsigned char iPhi = 0; iPhi < PHI_GRID_SIZE; iPhi++)
  {
    for (unsigned char iEta = 0; iEta < ETA_GRID_SIZE; iEta++)
    {
      std::cout << caloGrid[iPhi][iEta] << " ";
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;
}

void clearSums(SumLink & sums)
{
  sums.ht = 0;
  sums.met = 0;
}


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

void clearGrid(CaloGrid grid, TPt value) 
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
  return 0;
}

// runs a set of tests
// 1) nil data should return nil jets
// 2) testing single tower seed
// 3) & 4) & 5) testing two adjacent towers with same pt 
// 6) & 7) testing seed threshold 
// 8) testing phi periodicity 
void test5x5()
{
  CaloGrid grid;
  SumLink sums;
  TMJets jets;
  //unsigned char numberOfJetsFound = 0;
  // testing seed finding
  std::cout << "TEST 1" << std::endl;
  clearJets(jets);
  clearSums(sums);
  clearGrid(grid);
  runJetFinder(grid, jets, sums, true);
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
      // assert(jets[tmIndex][jetIndex].iEta == jetIndex);
      // assert(jets[tmIndex][jetIndex].iPhi == tmIndex);
    }
  }
  assert(sums.ht == 0);
  assert(sums.met == 0);

  std::cout << "TEST 2" << std::endl;
  clearJets(jets);
  clearSums(sums);
  grid[10][20] = 40;
  runJetFinder(grid, jets, sums, false);
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
      assert(jets[tmIndex][jetIndex].pt == ( ((jets[tmIndex][jetIndex].iPhi == 10) && (jets[tmIndex][jetIndex].iEta == 20)) ? 40 : 0 ) );
      // assert(jets[tmIndex][jetIndex].iEta == jetIndex);
      // assert(jets[tmIndex][jetIndex].iPhi == tmIndex);
    }
  }
  assert(sums.ht == 0);
  std::cout << "MET is " << sums.met << " instead of 40" << std::endl;

  std::cout << "TEST 3" << std::endl;
  clearGrid(grid);
  clearJets(jets);
  clearSums(sums);
  grid[4][5] = 40;
  grid[3][5] = 40;
  runJetFinder(grid, jets, sums, false);
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
      assert(jets[tmIndex][jetIndex].pt == ( ((jets[tmIndex][jetIndex].iPhi == 4) && (jets[tmIndex][jetIndex].iEta == 5)) ? 80 : 0 ) );
      // assert(jets[tmIndex][jetIndex].iEta == jetIndex);
      // assert(jets[tmIndex][jetIndex].iPhi == tmIndex);
    }
  }

  assert(sums.ht == 0);

  std::cout << "TEST 4" << std::endl;
  clearGrid(grid);
  clearJets(jets);
  clearSums(sums);
  grid[4][5] = 60;
  grid[4][6] = 60;
  runJetFinder(grid, jets, sums, false);
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
      assert(jets[tmIndex][jetIndex].pt == ( ((jets[tmIndex][jetIndex].iPhi == 4) && (jets[tmIndex][jetIndex].iEta == 6)) ? 120 : 0 ) );
      // assert(jets[tmIndex][jetIndex].iEta == jetIndex);
      // assert(jets[tmIndex][jetIndex].iPhi == tmIndex);
    }
  }
  assert(sums.ht == 120);

  
  std::cout << "TEST 5" << std::endl;
  clearGrid(grid);
  clearJets(jets);
  clearSums(sums);
  grid[4][5] = 60;
  grid[4][4] = 60;
  runJetFinder(grid, jets, sums, false);
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
      assert(jets[tmIndex][jetIndex].pt == ( ((jets[tmIndex][jetIndex].iPhi == 4) && (jets[tmIndex][jetIndex].iEta == 5)) ? 120 : 0 ) );
      // assert(jets[tmIndex][jetIndex].iEta == jetIndex);
      // assert(jets[tmIndex][jetIndex].iPhi == tmIndex);
    }
  }
  assert(sums.ht == 120);

  std::cout << "TEST 6" << std::endl;
  clearGrid(grid, SEED_THRESHOLD - 1);
  clearJets(jets);
  clearSums(sums);
  runJetFinder(grid, jets, sums, false);
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
      // assert(jets[tmIndex][jetIndex].iEta == jetIndex);
      // assert(jets[tmIndex][jetIndex].iPhi == tmIndex);
    }
  }
  assert(sums.ht == 120);
  assert(sums.met == 120);

  std::cout << "TEST 7" << std::endl;
  clearGrid(grid, SEED_THRESHOLD - 1);
  clearJets(jets);
  clearSums(sums);
  grid[4][5] = SEED_THRESHOLD;
  runJetFinder(grid, jets, sums, false);
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
      assert(jets[tmIndex][jetIndex].pt == ( ((jets[tmIndex][jetIndex].iPhi == 4) && (jets[tmIndex][jetIndex].iEta == 5)) ? 476 : 0 ) );
      // assert(jets[tmIndex][jetIndex].iEta == jetIndex);
      // assert(jets[tmIndex][jetIndex].iPhi == tmIndex);
    }
  }
  assert(sums.ht == 476);
  

  std::cout << "TEST 8" << std::endl;
  clearGrid(grid);
  clearJets(jets);
  clearSums(sums);
  grid[0][8] = 50;
  grid[PHI_GRID_SIZE - 1][8] = 45;
  grid[PHI_GRID_SIZE - 1][7] = 45;
  grid[PHI_GRID_SIZE - 1][9] = 45;
  runJetFinder(grid, jets, sums, false);
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
      assert(jets[tmIndex][jetIndex].pt == ( ((jets[tmIndex][jetIndex].iPhi == 0) && (jets[tmIndex][jetIndex].iEta == 8)) ? 185 : 0 ) );
      // assert(jets[tmIndex][jetIndex].iEta == jetIndex);
      // assert(jets[tmIndex][jetIndex].iPhi == tmIndex);
    }
  }
  assert(sums.ht == 185);

  
  std::cout << "All test have been successfully passed." << std::endl;
}


// runs the jet finder over a calorimeter grid
// first we reset the jet finder, we run it once without any data, then we send the various phi slices through it
void runJetFinder(const CaloGrid caloGrid, TMJets tmJets, SumLink sumLink, bool reset)
{
  CaloGridPhiSlice phiSlice;
  JetLinks lLinks;
  if (reset)
  {
    hls_jet_clustering(phiSlice, lLinks, sumLink, true);
    hls_jet_clustering(phiSlice, lLinks, sumLink, false);
  }
  // sending phi slices
  for (unsigned char iPhiIndex = 0; iPhiIndex < PHI_GRID_SIZE; iPhiIndex++) 
  {
    // copying the slice 
    for (unsigned char iEtaIndex = 0; iEtaIndex < ETA_GRID_SIZE; iEtaIndex++) 
    {
      phiSlice[iEtaIndex] = caloGrid[iPhiIndex][iEtaIndex];
    }
    // Sending the PHI_JET_SIZE - 1 slices while not reading jets because they do not produce any real jet
    // otherwise I send phi slices while reading and storing jets in output
    if (iPhiIndex < PHI_JET_SIZE - 1) hls_jet_clustering(phiSlice, lLinks, sumLink, false);
    else {
      hls_jet_clustering(phiSlice, lLinks, sumLink, false);
      for (unsigned int x = 0; x < NUMBER_OF_SEEDS; x++)
      {
        //unpacking jets and storing in an array that saves jets produced at every clock cycle
        // ">>" shifts bits to the right, >> 1, it is equivalent to int(x / 2), but makes sure the division is run in an optimised manner
        tmJets[iPhiIndex - PHI_JET_SIZE + 1][x] = lLinks[x >> 1][x % 2];
      }
    }
  }
  //we need to process the remaining PHI_JET_SIZE - 1 slices to take care of the phi periodicity
  for (unsigned char iPhiIndex = 0; iPhiIndex < PHI_JET_SIZE - 1; iPhiIndex++) 
  {
    hls_jet_clustering(phiSlice, lLinks, sumLink, false);
    for (unsigned int x = 0; x < NUMBER_OF_SEEDS; x++)
    {
      //unpacking
      tmJets[PHI_GRID_SIZE - PHI_JET_SIZE + 1 + iPhiIndex][x] = lLinks[x >> 1][x % 2];
    }
  }
  //done
}
