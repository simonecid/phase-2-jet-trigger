#include "JetClustering/HLS_Phase1Clustering.h"
#include "HistogramAndBuffer/HLS_Histogrammer.h"
#include "HistogramAndBuffer/HLS_HistogramBuffer.h"
#include "HistogramAndBuffer/HLS/Types.h"

#include <assert.h>
#include <vector>
#include <string>

#define ITERATIONS RESET_PERIOD

#ifdef ENABLE_ROOT
#include "TTree.h"
#include "TFile.h"
#endif 

template<class TBinSlice>
void printSlice(TBinSlice binSlice)
{
  for (unsigned int x = 0; x < N_ETA_BINS_BARREL_REGION; x++)
  {
    std::cout << binSlice[x] << " " ;
  }
  std::cout << std::endl;
}

typedef struct {
  float pt; 
  float phi;
  float eta;
} FloatInput;

#ifdef ENABLE_ROOT
// browses the inputs in a specific event in the tree
// saves all the input fitting a specific eta-phi region in an array of inputs for data-emu comparison
template<int maxNumberOfInputsInRegion, int maxNumberOfInputsInEvent>
void extractRegion(TTree* inputTree, unsigned int eventNumber, float phiLow, float phiHigh, float etaLow, float etaHigh, FloatInput extractedInputs[maxNumberOfInputsInRegion], unsigned int & numberOfInputsInRegion)
{
  // set address to read from the tree
  float lPtBranch[maxNumberOfInputsInEvent];
  float lEtaBranch[maxNumberOfInputsInEvent];
  float lPhiBranch[maxNumberOfInputsInEvent];
  unsigned int lNumberOfInputsInEvent;
  unsigned int lNumberOfInputsInRegion = 0;
  inputTree->GetBranch("pt")->SetAddress(lPtBranch);
  inputTree->GetBranch("eta")->SetAddress(lEtaBranch);
  inputTree->GetBranch("phi")->SetAddress(lPhiBranch);
  inputTree->GetBranch("length")->SetAddress(&lNumberOfInputsInEvent);
  
  // loading up event
  inputTree -> GetEvent(eventNumber);

  // saves input in extracted input if it fits in the region
  for (int x = 0; x < lNumberOfInputsInEvent; x++)
  {
    // checking if input fits in region or if we have run out of space
    if 
    (
      (lPhiBranch[x] < phiLow) ||
      (lPhiBranch[x] >= phiHigh)  ||
      (lEtaBranch[x] < etaLow)||
      (lEtaBranch[x] >= etaHigh)
    ) continue;

    if (lNumberOfInputsInRegion >= maxNumberOfInputsInRegion)
    {
      std::cerr << "Event " << eventNumber << " has too many inputs." << std::endl;
      continue;
    }

    // saving to extractedInputs

    // std::cout << "Input " << x << " pt-eta-phi:\t" << lPtBranch[x] << " - " << lEtaBranch[x] << " - " << lPhiBranch[x] << std::endl;

    extractedInputs[lNumberOfInputsInRegion].pt = lPtBranch[x];
    extractedInputs[lNumberOfInputsInRegion].phi = lPhiBranch[x];
    extractedInputs[lNumberOfInputsInRegion].eta = lEtaBranch[x];

    lNumberOfInputsInRegion ++;

  }

  // if (numberOfInputsInRegion > maxNumberOfInputsInRegion)
  // {
  //   std::cerr << "@@@@ " << numberOfInputsInRegion << " <= " << maxNumberOfInputsInRegion << " @@@@" << std::endl;
  //   numberOfInputsInRegion = maxNumberOfInputsInRegion;
  // }
  //outputting number of inputs fitting the region
  numberOfInputsInRegion = lNumberOfInputsInRegion;
}
#endif

typedef hls::TPt BarrelPhiSlice[N_ETA_BINS_BARREL_REGION * N_ETA_SEGMENTS_BARREL];


void runTriggerChain(hls::Barrel_Inputs inputs[ITERATIONS], JetLinks jets[ITERATIONS], SumLink sums[ITERATIONS], bool reset)
{
  hls::Barrel_PfInputHistogram::TBins barrel_bins[ITERATIONS];
  BarrelPhiSlice barrelPhiSlices[ITERATIONS];
  
  if (reset) 
  {
    bool inReset = true;
    bool outReset1, outReset2;
    hls_histogrammer(inputs[0], barrel_bins[0], inReset, outReset1);
    hls_histogram_buffer(barrel_bins[0], barrelPhiSlices[0], outReset1, outReset2);
    hls_jet_clustering(barrelPhiSlices[0], jets[0], sums[0], outReset2);
  }

  for (unsigned int iteration = 0; iteration < ITERATIONS; iteration++)
  {
    // std::cout << "Running cycle: " << iteration << ": \n";
    bool inReset = false;
    bool outReset1, outReset2;
    // if (iteration == 24) std::raise(SIGINT);
    hls_histogrammer(inputs[iteration], barrel_bins[iteration], inReset, outReset1);
    hls_histogram_buffer(barrel_bins[iteration], barrelPhiSlices[iteration], outReset1, outReset2);
    hls_jet_clustering(barrelPhiSlices[iteration], jets[iteration], sums[iteration], outReset2);

    // std::cout << "PhiSlice: " << iteration << " :";
    // for (hls::TEta etaIndex = 0; etaIndex < ETA_GRID_SIZE; etaIndex++)
    // {
    //   std::cout << barrelPhiSlices[iteration][etaIndex] * 0.25 << " ";
    // }
    // std::cout << std::endl;
    
  }
}


int main(int argc, char const *argv[])
{
  hls::Barrel_Inputs inputs[ITERATIONS];
  SumLink sums[ITERATIONS];
  JetLinks jets[ITERATIONS];

  for (unsigned int iteration = 0; iteration < ITERATIONS; iteration++)
  {
    for (unsigned int inputIdx = 0; inputIdx < NUMBER_OF_INPUTS_PER_CLOCK ; inputIdx++)
    {
      inputs[iteration][inputIdx].pt = 0;
      inputs[iteration][inputIdx].iPhi = 0;
      inputs[iteration][inputIdx].iEta = 0;
    }
  }

  #ifdef ENABLE_ROOT
  TFile lInputFile("pfdump.root");
  TTree* lInputTree = (TTree*) lInputFile.Get("ntuplizer/PuppiCandidates");
  TFile lSumsFile("HLSSums.root","RECREATE");
  TTree *lSumsTree = new TTree("hlsSumsTree","Tree contaning sums from HLS");
  float lL1THT = 0;
  float lL1TMET = 0;
  float lL1TMETPhi = 0;
  lSumsTree->Branch("l1tHT", &lL1THT, "l1tHT/F");
  lSumsTree->Branch("l1tMET", &lL1TMET, "l1tMET/F");
  lSumsTree->Branch("l1tMETPhi", &lL1TMETPhi, "l1tMETPhi/F");
  
  unsigned int lEventNumber = 0;
  unsigned int lNumberOfEvents = 0;
  if (argc > 1)
  {
    std::string lStrEventNumber(argv[1]);
    lEventNumber = stoi(lStrEventNumber);
    lNumberOfEvents = lEventNumber + 1;
  }
  else 
  {
    lNumberOfEvents = lInputTree -> GetEntries();
  }
  bool lReset = true;
  for (; lEventNumber < lNumberOfEvents; lEventNumber++)
  {
  
    std::cout << "Processing event: " << lEventNumber << std::endl;
    for (unsigned int iteration = 0; iteration < ITERATIONS; iteration++)
    {
      for (unsigned int inputIdx = 0; inputIdx < NUMBER_OF_INPUTS_PER_CLOCK ; inputIdx++)
      {
        inputs[iteration][inputIdx].pt = 0;
        inputs[iteration][inputIdx].iPhi = 0;
        inputs[iteration][inputIdx].iEta = 0;
      }
    }
    FloatInput lInputsFromTree[NUMBER_OF_INPUTS_PER_CLOCK];
    unsigned int lNumberOfInputsInRegion;
    extractRegion<NUMBER_OF_INPUTS_PER_CLOCK, 3456>(
      lInputTree, lEventNumber, 
      0, 0.7, 
      0, 0.75,
      lInputsFromTree,  lNumberOfInputsInRegion
    );

    for (int x = 0; x < lNumberOfInputsInRegion; x++)
    {
      inputs[0][x].pt = int(lInputsFromTree[x].pt/0.25);
      inputs[0][x].iEta = int(lInputsFromTree[x].eta/0.004363323);
      inputs[0][x].iPhi = int(lInputsFromTree[x].phi/0.004363323);
    }

    extractRegion<NUMBER_OF_INPUTS_PER_CLOCK, 3456>(
      lInputTree, lEventNumber, 
      0, 0.7, 
      0.75, 1.5,
      lInputsFromTree,  lNumberOfInputsInRegion
    );

    for (int x = 0; x < lNumberOfInputsInRegion; x++)
    {
      inputs[1][x].pt = int(lInputsFromTree[x].pt/0.25);
      inputs[1][x].iEta = int((lInputsFromTree[x].eta - 0.75)/0.004363323);
      inputs[1][x].iPhi = int(lInputsFromTree[x].phi/0.004363323);
    }
    runTriggerChain(inputs, jets, sums, lReset);
    if (lReset) lReset = false;
    //std::cout << "HT-MET: " << sums[RESET_PERIOD].ht * 0.25 << "\t" << sums[RESET_PERIOD].met * 0.25 << (sums[RESET_PERIOD].metPhi + 0.5) * 0.0875 << std::endl;
    unsigned char lLastFrame = (PHI_GRID_SIZE + PHI_JET_SIZE - 1);
    lL1THT = sums[lLastFrame].ht * 0.25;
    lL1TMET = sums[lLastFrame].met * 0.25;
    lL1TMETPhi = (sums[lLastFrame].metPhi + 0.5) * 0.0875;
    // std::cout << "HT-MET-METPHI: " << sums[lLastFrame].ht * 0.25 << "\t" << sums[lLastFrame].met * 0.25 << "\t" << sums[lLastFrame].metPhi << std::endl;
    lSumsTree -> Fill();
    for (unsigned int lClockCycle = 0; lClockCycle < ITERATIONS  ; lClockCycle++)
    {
      for (unsigned int jetIdx = 0; jetIdx < NUMBER_OF_SEEDS ; jetIdx++)
      {
        if (jets[lClockCycle][jetIdx >> 1][jetIdx % 2].pt > 0) 
        {
           std::cout << "Jet: " << jets[lClockCycle][jetIdx >> 1][jetIdx % 2].pt * 0.25 << "\t" << (jets[lClockCycle][jetIdx >> 1][jetIdx % 2].iEta + 0.5) * 0.0833 << "\t" << (jets[lClockCycle][jetIdx >> 1][jetIdx % 2].iPhi + 0.5) * 0.0875 << std::endl;
 //         std::cout << "Jet: " << jets[lClockCycle][jetIdx >> 1][jetIdx % 2].pt << "\t" << (jets[lClockCycle][jetIdx >> 1][jetIdx % 2].iEta + 0) * 1 << "\t" << (jets[lClockCycle][jetIdx >> 1][jetIdx % 2].iPhi + 0) * 1 << std::endl;
        }
      }
    }
  }
  lSumsFile.Write();
  lSumsFile.Close();
  #endif

  #ifndef ENABLE_ROOT
  std::vector<float> ptVector_0 = {50};
  std::vector<float> etaVector_0 = {0.343};
  std::vector<float> phiVector_0 = {0.343};
  std::vector<float> ptVector_1 = {50};
  std::vector<float> etaVector_1 = {0.343};
  std::vector<float> phiVector_1 = {0.343};
  for (int x = 0; x < ptVector_0.size(); x++)
  {
    inputs[0][x].pt = ptVector_0[x]/0.25;
    inputs[0][x].iEta = etaVector_0[x]/0.0043633231;
    inputs[0][x].iPhi = phiVector_0[x]/0.0043633231;
  }
  for (int x = 0; x < ptVector_1.size(); x++)
  {
    inputs[0 + 1][x].pt = ptVector_1[x]/0.25;
    inputs[0 + 1][x].iEta = etaVector_1[x]/0.0043633231;
    inputs[0 + 1][x].iPhi = phiVector_1[x]/0.0043633231;
  }
  runTriggerChain(inputs, jets, sums);
  std::cout << "HT-MET: " << sums[RESET_PERIOD].ht * 0.25 << "\t" << sums[RESET_PERIOD].met * 0.25 << (sums[RESET_PERIOD].metPhi + 0.5) * 0.0875 << std::endl;
  for (unsigned int lClockCycle = RESET_PERIOD - (RESET_PERIOD - 1); lClockCycle <= RESET_PERIOD  ; lClockCycle++)
  {
    for (unsigned int jetIdx = 0; jetIdx < NUMBER_OF_SEEDS ; jetIdx++)
    {
      //std::cout << "HT-MET: " << sums[iteration].ht * 0.25 << "\t" << sums[iteration].met * 0.25 << std::endl;
      if (jets[lClockCycle][jetIdx >> 1][jetIdx % 2].pt > 0) 
      {
        std::cout << "Jet: " << jets[lClockCycle][jetIdx >> 1][jetIdx % 2].pt * 0.25 << "\t" << (jets[lClockCycle][jetIdx >> 1][jetIdx % 2].iEta + 0.5) * 0.0833 << "\t" << (jets[lClockCycle][jetIdx >> 1][jetIdx % 2].iPhi + 0.5) * 0.0875 << std::endl;
        // std::cout << "Jet: " << jets[lClockCycle][jetIdx >> 1][jetIdx % 2].pt * 0.25 << "\t" << (jets[lClockCycle][jetIdx >> 1][jetIdx % 2].iEta + 0) * 1 << "\t" << (jets[lClockCycle][jetIdx >> 1][jetIdx % 2].iPhi + 0) * 1 << std::endl;
      }
  }
  #endif

  return 0;
}
