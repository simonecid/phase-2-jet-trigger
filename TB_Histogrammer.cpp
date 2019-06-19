#include "HLS_Histogrammer.h"
#include "TB_Histogrammer.h"
#include <iostream>
#include <csignal>
#include <assert.h>
#include <fstream>
#include "HLS/Histogram2D.h"
#include "HLS/Types.h"
#include "HLS/HistogramSettings.h"

// bool readInputsFromFile(const std::string &filepath, hls::Inputs inputs)
// {
//   std::ifstream inFile(filepath);
//   if (inFile.is_open())
//   {
//     for (uint x = 0; x < NUMBER_OF_INPUTS_PER_CLOCK; x++)
//     {
//       int pt;
//       inFile >> pt;
//       inputs[x].pt = pt;
//       int iEta;
//       inFile >> iEta;
//       inputs[x].iEta = iEta;
//       int iPhi;
//       inFile >> iPhi;
//       inputs[x].iPhi = iPhi;
//     }
//     inFile.close();
//     return true;
//   } else {
//     std::cout << "ERROR: Opening file \"" << filepath << "\" failed." << std::endl;
//     exit(1);
//   }
// }

// bool readBinsFromFile(const std::string &filepath, hls::TBins histogram)
// {
//   std::ifstream inFile(filepath);
//   if (inFile.is_open())
//   {
//     for (uint x = 0; x < PHI_GRID_SIZE; x++)
//     {
//       for(uint y = 0; y < ETA_GRID_SIZE; y++)
//       {
//         inFile >> histogram[y][x];
//       }
//     }
//     inFile.close();
//     return true;
//   } else {
//     std::cout << "ERROR: Opening file \"" << filepath << "\" failed." << std::endl;
//     exit(1);
//   }
// }


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
  hls::TK_HG_Inputs lTK_HG_Inputs;
  hls::HG_Inputs lHG_Inputs;
  hls::HF_Inputs lHF_Inputs;
  hls::Barrel_PfInputHistogram::TBins Barrel_outputBins;
  hls::Barrel_PfInputHistogram::TBins referenceBins;
  hls::TK_HG_PfInputHistogram::TBins TK_HG_outputBins;
  hls::HG_PfInputHistogram::TBins HG_outputBins;
  hls::HF_PfInputHistogram::TBins HF_outputBins;

  std::cout << "Testing one bin fill" << std::endl;
  resetInputs<decltype(lBarrel_Inputs)>(lBarrel_Inputs);

  lBarrel_Inputs[0].pt = 100;
  lBarrel_Inputs[0].iEta = 10;
  lBarrel_Inputs[0].iPhi = 20;

  hls_histogrammer(lBarrel_Inputs, Barrel_outputBins,
                   lTK_HG_Inputs, TK_HG_outputBins,
                   lHG_Inputs, HG_outputBins,
                   lHF_Inputs, HF_outputBins);

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

  hls_histogrammer(lBarrel_Inputs, Barrel_outputBins,
                   lTK_HG_Inputs, TK_HG_outputBins,
                   lHG_Inputs, HG_outputBins,
                   lHF_Inputs, HF_outputBins);

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

  hls_histogrammer(lBarrel_Inputs, Barrel_outputBins,
                   lTK_HG_Inputs, TK_HG_outputBins,
                   lHG_Inputs, HG_outputBins,
                   lHF_Inputs, HF_outputBins);

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
  
  // std::cout << "Testing fill with random values" << std::endl;

  // readInputsFromFile("/users/sb17498/HLS_Histogrammer/inputs.dat", lBarrel_Inputs);
  // readBinsFromFile("/users/sb17498/HLS_Histogrammer/inputs_histogram.dat", referenceBins);

  // hls_histogrammer(lBarrel_Inputs, Barrel_outputBins,
  //                  lTK_HGputs, TK_HG_outputBins,
  //                  lHG_Inputs, HG_outputBins,
  //                  lHF_Inputs, HF_outputBins);

  // for (unsigned char y = 0; y < PHI_GRID_SIZE; y++)
  // {
  //   for (unsigned char x = 0; x < ETA_GRID_SIZE; x++)
  //   {
  //     hls::TBin lBinRef = referenceBins[y][x];
  //     hls::TBin lBin = Barrel_outputBins[y][x];
  //     assert(lBin == lBinRef);
  //   }
  // }

  std::cout << "All tests have been passed!" << std::endl;

  return 0;
}