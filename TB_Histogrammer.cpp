#include "HLS_Histogrammer.h"
#include "TB_Histogrammer.h"
#include <iostream>
#include <csignal>
#include <assert.h>
#include <fstream>
#include "HLS/HistogramEtaPhi.h"

bool readInputsFromFile(const std::string &filepath, Inputs inputs)
{
  std::ifstream inFile(filepath);
  if (inFile.is_open())
  {
    for (uint x = 0; x < NUMBER_OF_INPUTS_PER_CLOCK; x++)
    {
      inFile >> inputs[x].pt;
      inFile >> inputs[x].iEta;
      inFile >> inputs[x].iPhi;
    }
    inFile.close();
    return true;
  } else {
    std::cout << "ERROR: Opening file \"" << filepath << "\" failed." << std::endl;
    exit(1);
  }
}

bool readBinsFromFile(const std::string &filepath, hls::TBins histogram)
{
  std::ifstream inFile(filepath);
  if (inFile.is_open())
  {
    for (uint x = 0; x < PHI_GRID_SIZE; x++)
    {
      for(uint y = 0; y < ETA_GRID_SIZE; y++)
      {
        inFile >> histogram[y][x];
      }
    }
    inFile.close();
    return true;
  } else {
    std::cout << "ERROR: Opening file \"" << filepath << "\" failed." << std::endl;
    exit(1);
  }
}


void resetInputs(Inputs inputs)
{
  for (uint x = 0; x < NUMBER_OF_INPUTS_PER_CLOCK; x++) 
  {
    std::cout << +x << " < " << NUMBER_OF_INPUTS_PER_CLOCK << "\r";
    inputs[x].pt = 0;
    inputs[x].iEta = 0;
    inputs[x].iPhi = 0;
  }
  return;
}

hls::TEta xBins[ETA_GRID_SIZE + 1] = ETA_BINS;
hls::TPhi yBins[PHI_GRID_SIZE + 1] = PHI_BINS;

int main(int argc, char const *argv[])
{

  Inputs lInputs;
  hls::TBins outputBins;
  hls::TBins referenceBins;

  std::cout << "Testing one bin fill" << std::endl;
  resetInputs(lInputs);

  lInputs[0].pt = 100;
  lInputs[0].iEta = 50;
  lInputs[0].iPhi = 20;

  hls_histogrammer(lInputs, outputBins, true);

  for (unsigned char y = 0; y < PHI_GRID_SIZE; y++)
  {
    for (unsigned char x = 0; x < ETA_GRID_SIZE; x++)
    {
      hls::TBin lBinRef = 0;
      if ((x == 5) && (y == 2)) lBinRef = 100;
      hls::TBin lBin = outputBins[y][x];
      assert(lBin == lBinRef);
    }
  }

  std::cout << "Testing adding to single bin" << std::endl;
  resetInputs(lInputs);

  lInputs[0].pt = 100;
  lInputs[0].iEta = 50;
  lInputs[0].iPhi = 20;
  lInputs[2].pt = 100;
  lInputs[2].iEta = 50;
  lInputs[2].iPhi = 20;

  hls_histogrammer(lInputs, outputBins, true);

  for (unsigned char y = 0; y < PHI_GRID_SIZE; y++)
  {
    for (unsigned char x = 0; x < ETA_GRID_SIZE; x++)
    {
      hls::TBin lBinRef = 0;
      if ((x == 5) && (y == 2)) lBinRef = 200;
      hls::TBin lBin = outputBins[y][x];
      assert(lBin == lBinRef);
    }
  }
  
  std::cout << "Testing two bin fill" << std::endl;
  resetInputs(lInputs);

  lInputs[0].pt = 100;
  lInputs[0].iEta = 50;
  lInputs[0].iPhi = 20;
  lInputs[2].pt = 100;
  lInputs[2].iEta = 22;
  lInputs[2].iPhi = 89;

  hls_histogrammer(lInputs, outputBins, true);

  for (unsigned char y = 0; y < PHI_GRID_SIZE; y++)
  {
    for (unsigned char x = 0; x < ETA_GRID_SIZE; x++)
    {
      hls::TBin lBinRef = 0;
      if ((x == 5) && (y == 2)) lBinRef = 100;
      if ((x == 2) && (y == 8)) lBinRef = 100;
      hls::TBin lBin = outputBins[y][x];
      assert(lBin == lBinRef);
    }
  }
  
  std::cout << "Testing fill with random values" << std::endl;

  readInputsFromFile("/users/sb17498/HLS_Histogrammer/inputs.dat", lInputs);
  readBinsFromFile("/users/sb17498/HLS_Histogrammer/inputs_histogram.dat", referenceBins);

  hls_histogrammer(lInputs, outputBins, true);

  for (unsigned char y = 0; y < PHI_GRID_SIZE; y++)
  {
    for (unsigned char x = 0; x < ETA_GRID_SIZE; x++)
    {
      hls::TBin lBinRef = referenceBins[y][x];
      hls::TBin lBin = outputBins[y][x];
      assert(lBin == lBinRef);
    }
  }

  std::cout << "All tests have been passed!" << std::endl;

  return 0;
}