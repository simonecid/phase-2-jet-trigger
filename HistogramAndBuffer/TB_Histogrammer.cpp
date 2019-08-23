#include "HLS_Histogrammer.h"
#include "TB_Histogrammer.h"
#include <iostream>
#include <csignal>
#include <assert.h>
#include <fstream>
#include "HLS/Histogram2D.h"
#include "HLS/Types.h"
#include "HLS/HistogramSettings.h"

template<class Inputs>
bool readInputsFromFile(const std::string &filepath, Inputs inputs)
{
  std::ifstream inFile(filepath);
  if (inFile.is_open())
  {
    for (uint x = 0; x < NUMBER_OF_INPUTS_PER_CLOCK; x++)
    {
      unsigned int pt;
      inFile >> pt;
      inputs[x].pt = pt;
      unsigned int iEta;
      inFile >> iEta;
      inputs[x].iEta = iEta;
      unsigned int iPhi;
      inFile >> iPhi;
      inputs[x].iPhi = iPhi;
    }
    inFile.close();
    return true;
  } else {
    std::cout << "ERROR: Opening file \"" << filepath << "\" failed." << std::endl;
    exit(1);
  }
}

template <class TBins>
bool readBinsFromFile(const std::string &filepath, TBins histogram)
{
  std::ifstream inFile(filepath);
  if (inFile.is_open())
  {
    for (uint x = 0; x < N_ETA_BINS_BARREL_REGION; x++)
    {
      for(uint y = 0; y < N_BINS_PHI_REGION; y++)
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


template <class Inputs>
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

int main(int argc, char const *argv[])
{

  hls::Barrel_Inputs lBarrel_Inputs;
  hls::Barrel_PfInputHistogram::TBins Barrel_outputBins;
  hls::Barrel_PfInputHistogram::TBins referenceBins;

  std::cout << "Testing one bin fill" << std::endl;
  resetInputs<decltype(lBarrel_Inputs)>(lBarrel_Inputs);

  lBarrel_Inputs[0].pt = 100;
  lBarrel_Inputs[0].iEta = 15;
  lBarrel_Inputs[0].iPhi = 25;

  bool inReset, outReset;
  inReset = true;
  hls_histogrammer(lBarrel_Inputs, Barrel_outputBins, inReset, outReset);
  assert(outReset == inReset);

  inReset = false;
  hls_histogrammer(lBarrel_Inputs, Barrel_outputBins, inReset, outReset);
  assert(outReset == inReset);
  for (unsigned char y = 0; y < N_BINS_PHI_REGION; y++)
  {
    for (unsigned char x = 0; x < N_ETA_BINS_BARREL_REGION; x++)
    {
      hls::TBin lBinRef = 0;
      if ((x == 1) && (y == 2)) lBinRef = 100;
      hls::TBin lBin = Barrel_outputBins[y][x];
      assert(lBin == lBinRef);
    }
  }

  std::cout << "Testing adding to single bin" << std::endl;
  resetInputs<decltype(lBarrel_Inputs)>(lBarrel_Inputs);

  lBarrel_Inputs[0].pt = 100;
  lBarrel_Inputs[0].iEta = 10;
  lBarrel_Inputs[0].iPhi = 20;
  lBarrel_Inputs[2].pt = 100;
  lBarrel_Inputs[2].iEta = 10;
  lBarrel_Inputs[2].iPhi = 20;

  hls_histogrammer(lBarrel_Inputs, Barrel_outputBins, inReset, outReset);
  assert(outReset == inReset);
  for (unsigned char y = 0; y < N_BINS_PHI_REGION; y++)
  {
    for (unsigned char x = 0; x < N_ETA_BINS_BARREL_REGION; x++)
    {
      hls::TBin lBinRef = 0;
      if ((x == 1) && (y == 2)) lBinRef = 200;
      hls::TBin lBin = Barrel_outputBins[y][x];
      assert(lBin == lBinRef);
    }
  }
  
  std::cout << "Testing two bin fill" << std::endl;
  resetInputs<decltype(lBarrel_Inputs)>(lBarrel_Inputs);

  lBarrel_Inputs[0].pt = 100;
  lBarrel_Inputs[0].iEta = 12;
  lBarrel_Inputs[0].iPhi = 20;
  lBarrel_Inputs[2].pt = 100;
  lBarrel_Inputs[2].iEta = 02;
  lBarrel_Inputs[2].iPhi = 19;

  hls_histogrammer(lBarrel_Inputs, Barrel_outputBins, inReset, outReset);
  assert(outReset == inReset);
  for (unsigned char y = 0; y < N_BINS_PHI_REGION; y++)
  {
    for (unsigned char x = 0; x < N_ETA_BINS_BARREL_REGION; x++)
    {
      hls::TBin lBinRef = 0;
      if ((x == 1) && (y == 2)) lBinRef = 100;
      if ((x == 0) && (y == 1)) lBinRef = 100;
      hls::TBin lBin = Barrel_outputBins[y][x];
      assert(lBin == lBinRef);
    }
  }
  
  std::cout << "Testing fill with random values" << std::endl;

  for (unsigned int x = 0; x < 100; x++)
  {
    std::cout << "Processing sample " << x << std::endl;
    std::string appendix = std::to_string(x);
    std::string input_filename = "/home/sb17498/Documents/Projects/HLS/phase-2-jet-trigger-chain/HistogramAndBuffer/inputs_";
    input_filename += appendix;
    input_filename += ".dat";
    std::string histogram_filename = "/home/sb17498/Documents/Projects/HLS/phase-2-jet-trigger-chain/HistogramAndBuffer/inputs_histogram_";
    histogram_filename += appendix;
    histogram_filename += ".dat";  

    readInputsFromFile(input_filename, lBarrel_Inputs);
    readBinsFromFile(histogram_filename, referenceBins);

    hls_histogrammer(lBarrel_Inputs, Barrel_outputBins, inReset, outReset);

    // std::raise(SIGINT);
    
    for (unsigned char y = 0; y < N_BINS_PHI_REGION; y++)
    {
      for (unsigned char x = 0; x < N_ETA_BINS_BARREL_REGION; x++)
      {
        hls::TBin lBin = Barrel_outputBins[y][x];
        std::cout << lBin << " ";
      }
      std::cout << std::endl;
    }
    std::cout << "------------" << std::endl;
    for (unsigned char y = 0; y < N_BINS_PHI_REGION; y++)
    {
      for (unsigned char x = 0; x < N_ETA_BINS_BARREL_REGION; x++)
      {
        hls::TBin lBinRef = referenceBins[y][x];
        std::cout << lBinRef << " ";
      }
      std::cout << std::endl;
    }

    for (unsigned char x = 0; x < N_ETA_BINS_BARREL_REGION; x++)
    {
      for (unsigned char y = 0; y < N_BINS_PHI_REGION; y++)
      {
        hls::TBin lBinRef = referenceBins[y][x];
        hls::TBin lBin = Barrel_outputBins[y][x];
        assert(lBin == lBinRef);
      }
    }
  }

  std::cout << "All tests have been passed!" << std::endl;


  return 0;
}