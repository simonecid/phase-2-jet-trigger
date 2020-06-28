#include "HLS_Phase1Clustering.h"
#include <hls_dsp.h>
#include <hls_math.h>

#ifndef __SYNTHESIS__
#include <iostream>
#include <csignal>
void printBuffer(const CaloGridBuffer caloGrid)
{
  copyPhiGrid: for(TPhi iPhi = 0 ; iPhi < PHI_JET_SIZE ; iPhi++)
  {
    copyEtaGrid: for (TEta iEta = 0 ; iEta < ETA_GRID_SIZE ; iEta++)
    {
      std::cout << caloGrid[iPhi][iEta] << " " ;
    }
    std::cout << std::endl;
  }
  return;
}
#endif


// Returns the pt of a certain bin in the calo grid or 0 for eta out of bounds
TPt hls_getTowerEnergy(const CaloGridBuffer caloGrid, TEta iEta, TPhi iPhi)
{
  #pragma HLS inline
  #pragma HLS pipeline II=1
  // if out of bounds return 0, else the value
  // when possible I use ternary operators as HLS optimises them better, according to the Internet
  return ((iEta < 0) || (iEta >= ETA_GRID_SIZE)) ? kNullPt : caloGrid[iPhi][iEta];
}

// Copies a buffer into another
void hls_copyGrid (const CaloGridBuffer inCaloGrid, CaloGridBuffer outCaloGrid)
{
  #pragma HLS inline
  #pragma HLS pipeline II=1
  copyPhiGrid: for (TPhi iPhi = 0 ; iPhi < PHI_JET_SIZE ; iPhi++)
  {
    copyEtaGrid: for(TEta iEta = 0 ; iEta < ETA_GRID_SIZE ; iEta++)
    {
      outCaloGrid[iPhi][iEta] = inCaloGrid[iPhi][iEta];
    }
  }
}

// Sets the content of the buffer to 0
void hls_clearGrid (CaloGridBuffer inCaloGrid)
{
  #pragma HLS inline
  #pragma HLS pipeline II=1
  copyPhiGrid: for (TPhi iPhi = 0 ; iPhi < PHI_JET_SIZE ; iPhi++)
  {
    copyEtaGrid: for(TEta iEta = 0 ; iEta < ETA_GRID_SIZE ; iEta++)
    {
      inCaloGrid[iPhi][iEta] = 0;
    }
  }
}

// Shifts the content of the buffer towards left in phi
// The content of the phi = 0 row is lost
// The content of the phi = PHI_JET_SIZE - 1 (i.e. the right-most one) is set to 0
void hls_shiftGridLeft (const CaloGridBuffer inCaloGrid, CaloGridBuffer outCaloGrid)
{
  #pragma HLS inline
  #pragma HLS pipeline II=1
  copyPhiGrid: for(TPhi iPhi = 1 ; iPhi < PHI_JET_SIZE ; iPhi++)
  {
    copyEtaGrid: for (TEta iEta = 0 ; iEta < ETA_GRID_SIZE ; iEta++)
    {
      outCaloGrid[iPhi - 1][iEta] = inCaloGrid[iPhi][iEta];
    }
  }
  setEtaGrid0: for (TEta iEta = 0 ; iEta < ETA_GRID_SIZE ; iEta++)
  {
    outCaloGrid[PHI_JET_SIZE - 1][iEta] = 0;
  }
}


// copies a phi slice into a specific phi slice of the buffer
void hls_copyLine (const CaloGridPhiSlice caloGridPhiSlice, CaloGridBuffer outCaloGrid, TPhi phiIndex)
{
  #pragma HLS inline
  #pragma HLS pipeline II=1
  copyPhiGrid: for (TEta iEta = 0 ; iEta < ETA_GRID_SIZE ; iEta++)
  {
    outCaloGrid[phiIndex][iEta] = caloGridPhiSlice[iEta];
  }
}

// copies a phi slice into another
void hls_copyLine (const CaloGridPhiSlice inCaloGridPhiSlice, CaloGridPhiSlice outCaloGridPhiSlice)
{
  #pragma HLS inline
  #pragma HLS pipeline II=1
  copyPhiGrid: for (TEta iEta = 0 ; iEta < ETA_GRID_SIZE ; iEta++)
  {
    outCaloGridPhiSlice[iEta] = inCaloGridPhiSlice[iEta];
  }
}

// computes the sum of bins in a phi slice
TPt hls_computeSumOfTowers (const CaloGridPhiSlice inCaloGridPhiSlice)
{
  #pragma HLS inline
  #pragma HLS pipeline II=1
  TPt lSum = 0;
  computeSum: for (TEta iEta = 0 ; iEta < ETA_GRID_SIZE ; iEta++)
  {
    lSum += inCaloGridPhiSlice[iEta];
  }
  return lSum;
}

// STARTING POINT
// pipelined to run every clock cycle
// input: phi slices
//        d0Valid: reset signal, first data is expected to arrive 2 clk after the signal is applied
// outputs:
//  * outJets: 32-bit jets, packed as two 32-bit jets per 64-bit link (enforced by datapack pragma)
//  * outSums: link containing packed pt sums (enforced by datapack), 32-bit of 64-bit have been used: 16 bits for HT and 16 bits for MET. It can still hold 2 more sums by expanding the struct.

// 2 clk after the initial reset is applied, the module is ready to receive data

// JET FINDING:
// * the first PHI_JET_SIZE - 1 phi slices are stored in two buffers:
//   * the first is shift register buffer in which the jets are found, the second is the one that stores them to find jets when the final phi slices have been received (to close the detector in phi)
// * from PHI_JET_SIZE-th slice to the end of the calorimeter, data is stored in the shift register buffer only and jets are found in that buffer as the data flows through it
// * once we have received PHI_GRID_SIZE-th phi slice, i.e. the last one, we can process the first PHI_JET_SIZE - 1 phi slices the kept from the beginning, every clock cycle we inject them in the shift register buffer
// * found jets are output in a packed manner: instead of outputting pt-eta-phi of each jet on a separate link, we put two jet on a single link, bit 0-31 keeps pt-eta-phi of jet 1, bit 32-63 holds pt-eta-phi of jet 2

// MET CALCULATION:
// * For each phi slice in input, we first compute the total pt
// * The px and py of the phi slice is computed by multiplying the total pt by the sin and cos of the phi slice which is stored in LUT
// * The px and py is added to a register keeping the total px and py of the event
// * MET is computed as sqrt(px*px + py*py) with a CORDIC 
// * Phi is computed by dividing px by pt and looking up the cos phi angle
// * cos phi is monotonic over 0 - pi, so we can do a dicotomic search for phi between 0 - pi
// * if py is < 0 then phi is between pi - 2*pi and we subtract the angle found in the previous step to 2*pi

// HT CALCULATION:
// For each series of jet that has been found, we check if its pt is above the HT_THRESHOLD, if so, pt is added to the HT register

// MET/HT output is packed in a single 64-bit link, since they use 16-bit there is space for two additional sums on the same link, like MHT

void hls_jet_clustering(const CaloGridPhiSlice inCaloGridPhiSlice, JetLinks outJets, SumLink & outSums, bool d0Valid) 
{
  // split in port carrying single inputs 
  #pragma HLS array_partition variable=inCaloGridPhiSlice complete dim=0
  // split in jet links
  #pragma HLS array_partition variable=outJets complete dim=0
  // packing jet pairs in a single 64-bit word
  #pragma HLS array_reshape variable=outJets complete dim=2
  // packing the word on links
  #pragma HLS data_pack variable=outJets
  // packing sums on link
  #pragma HLS data_pack variable=outSums
  // no valid ports for the I/O
  #pragma HLS interface ap_none port=inCaloGridPhiSlice
  // no valid ports for the I/O
  #pragma HLS interface ap_none port=outJets
  #pragma HLS interface ap_none port=outSums
  // no valid ports for the I/O
  #pragma HLS interface ap_none port=d0Valid
  // removing control bus from design
  #pragma HLS interface ap_ctrl_none port=return
  #pragma HLS pipeline II=1
  

  // keeps track of the status 
  static TPhi sRegister = 0;
  // stores the total HT of the event
  static TPt sHT = 0;
  // stores the total Px of the event for the MET calculation
  static TMomentum sTotalPx = 0;
  // stores the total Py of the event for the MET calculation
  static TMomentum sTotalPy = 0;
  // LUT containing the sine and cosine of each histogram iPhi angle
  // To save resources we do not compute MET from the pf inputs, as we would need to add a sine and cosine calculation which is expensive on FPGAs
  static const TSinCos sin_iPhi[PHI_GRID_SIZE] = {0.04374, 0.13087, 0.21701, 0.30149, 0.38365, 0.46289, 0.53858, 0.61015};
  static const TSinCos cos_iPhi[PHI_GRID_SIZE] = {0.99904, 0.9914, 0.97617, 0.95347, 0.92348, 0.88642, 0.84257, 0.79229};
  // THIS IS TRUE IF WE ARE NOT COVERING PHI > PI 
  static const TSinCos monotonic_cos_iPhi[PHI_GRID_SIZE] = {0.79229, 0.84257, 0.88642, 0.92348, 0.95347, 0.97617, 0.9914, 0.99904};

  
  bool lReset = d0Valid;
  TPhi lPhiIndex;
  // stores the calorimeter windows in which the jets will be found
  static CaloGridBuffer sCaloGrid;
  #pragma HLS array_partition variable=sCaloGrid complete dim=0
  // stores the first calorimeter lines to be used later to close up the phi ring
  static CaloGridPhiSlice sFirstPhiSlices[PHI_JET_SIZE - 1];
  #pragma HLS array_partition variable=sFirstPhiSlices complete dim=0

  // reset internal registers when a reset signal is received
  // else move the phi slice counter to the next state
  bool lResetSums = false;
  if (lReset)
  {
    lResetSums = true;
    
    sRegister = -2; // -2 because the previous stage in the next clock will receive region 1, then in the second region 2 and the jet finder will receive a row
  } 
  else 
  {
    TPhi lNextRegisterValue = sRegister + 1;
    // resets the internal status every RESET_PERIOD clocks, or increase it
    sRegister = (sRegister == (RESET_PERIOD - 1)) ? kNullPhi : lNextRegisterValue;
    lResetSums = (sRegister == kNullPhi);
    lPhiIndex = sRegister;
  }

  if (lResetSums)
  {
    sHT = 0;
    sTotalPx = 0;
    sTotalPy = 0;
    sJetPx = 0;
    sJetPy = 0;
  }

  // local version of the calorimeter buffer. I am not sure it is required
  // I implemented it because I wanted to make sure I would not lock the global
  // static calorimeter memory
  CaloGridBuffer lCaloGridTmp;
  #pragma HLS array_partition variable=lCaloGridTmp complete dim=0 
  //storing the first phi slices to analyse them later when I got the last ones
  if (lPhiIndex < PHI_JET_SIZE - 1) 
  {
    hls_copyLine(inCaloGridPhiSlice, sFirstPhiSlices[lPhiIndex]);
  }

  // storing in the buffer and computing current MET
  // else if we have received all the phi slices then it is time to analyse the one I previously stored
  if (lPhiIndex < PHI_GRID_SIZE) {

    TPt lTotalPt = hls_computeSumOfTowers(inCaloGridPhiSlice);
    // px and py calculation
    sTotalPx += lTotalPt * cos_iPhi[lPhiIndex];
    sTotalPy += lTotalPt * sin_iPhi[lPhiIndex];
    
    hls_copyLine(inCaloGridPhiSlice, sCaloGrid, PHI_JET_SIZE - 1);
  }
  else if (lPhiIndex < PHI_GRID_SIZE + PHI_JET_SIZE - 1)
  {
    TPhi lLineIndex = lPhiIndex - PHI_GRID_SIZE;
    hls_copyLine(sFirstPhiSlices[lLineIndex], sCaloGrid, PHI_JET_SIZE - 1);
  }
  
  // running the jet finder on the calorimeter buffer
  hls_copyGrid(sCaloGrid, lCaloGridTmp);
  hls_shiftGridLeft(sCaloGrid, sCaloGrid);
  Jets lJets; 
  // std::cout << "Dumping buffer at lPhiIndex " << lPhiIndex << std::endl;
  // printBuffer(lCaloGridTmp);
  hls_runJetFinders(lCaloGridTmp, lJets);
  
  //if I am buffering data before finding jets or I have analysed all the data, then I output null
  // otherwise output jets
  if ((lPhiIndex < PHI_JET_SIZE - 1) || (lPhiIndex >= PHI_GRID_SIZE + PHI_JET_SIZE - 1))
  {
    sendNullData:for (unsigned char jetIdx = 0 ; jetIdx < NUMBER_OF_SEEDS; jetIdx++) 
    {
      outJets[jetIdx >> 1][jetIdx % 2].pt = 0;
      outJets[jetIdx >> 1][jetIdx % 2].iPhi = 0;
      outJets[jetIdx >> 1][jetIdx % 2].iEta = 0;
    }
  } 
  else 
  { 

    // jet position in phi
    TPhi lOutJetsPhi = lPhiIndex - PHI_JET_SIZE/2;
    // jet position in phi if the jet is out of bounds in phi
    TPhi lOutJetsPhiMinusPeriod = lOutJetsPhi - PHI_GRID_SIZE;
    // jet position to output, depends on whether lOutJetsPhi is out of bounds, if it is, we assign the corrected position
    TPhi lOutJetsPhiToOutput = (lOutJetsPhi >= PHI_GRID_SIZE) ? lOutJetsPhiMinusPeriod : lOutJetsPhi;
    TMomentum lJetPx = 0;
    TMomentum lJetPy = 0;
    TPt lJetTotalPt = 0;
    TPt lHT = 0;
    copyBackJets:for (unsigned char jetIdx = 0 ; jetIdx < NUMBER_OF_SEEDS; jetIdx++) 
    {
      //packing jets for output
      outJets[jetIdx >> 1][jetIdx % 2].pt = lJets[jetIdx].pt;
      // adding to HT if the pt of the jets is above 30 GeV
      lHT += (lJets[jetIdx].pt >= HT_SUM_THRESHOLD) ? lJets[jetIdx].pt : kNullPt;
      lJetTotalPt += lJets[jetIdx].pt >= HT_SUM_THRESHOLD ? lJets[jetIdx].pt : kNullPt;

     
      // handling phi periodicity
      outJets[jetIdx >> 1][jetIdx % 2].iPhi = lOutJetsPhiToOutput;
      outJets[jetIdx >> 1][jetIdx % 2].iEta = lJets[jetIdx].iEta;
      //if (lJets[jetIdx].pt != 0) std::raise(SIGINT);

    }
    sHT += lHT;
    lJetPx = lJetTotalPt * cos_iPhi[lOutJetsPhiToOutput]; // all jets have same iPhi in the same clock cycle
    lJetPy = lJetTotalPt * sin_iPhi[lOutJetsPhiToOutput];
    
    sJetPx += lJetPx;
    sJetPy += lJetPy;
  }

  // old cordic, running on 16 bit pt squared, which is not big enough
  // TPt lMETSquared = sTotalPx * sTotalPx + sTotalPy * sTotalPy;
  // hls::sqrt_input<16, hls::CORDIC_FORMAT_USIG_INT>::in lMETSquaredCORDIC;
  // hls::sqrt_output<9, hls::CORDIC_FORMAT_USIG_INT>::out lMETCORDIC;
  // lMETSquaredCORDIC.in = lMETSquared;
  // hls::sqrt<hls::CORDIC_FORMAT_USIG_INT, 16, 9, hls::CORDIC_ROUND_TRUNCATE>(lMETSquaredCORDIC, lMETCORDIC);

  //using CORDIC to compute sqrt of MET
  //hls_math sqrt performs horribly, so we use CORDIC
  //documented at
  // https://www.xilinx.com/support/documentation/sw_manuals/xilinx2018_2/ug902-vivado-high-level-synthesis.pdf
  // page 603
  // 32 bit because squared numbers are big
  ap_uint<32> lTotalPxSquared = sTotalPx * sTotalPx;
  ap_uint<32> lTotalPySquared = sTotalPy * sTotalPy;
  ap_uint<32> lMETSquared = lTotalPxSquared + lTotalPySquared;
  hls::sqrt_input<32, hls::CORDIC_FORMAT_USIG_INT>::in lMETSquaredCORDIC;
  hls::sqrt_output<17, hls::CORDIC_FORMAT_USIG_INT>::out lMETCORDIC;
  lMETSquaredCORDIC.in = lMETSquared;
  hls::sqrt<hls::CORDIC_FORMAT_USIG_INT, 32, 17, hls::CORDIC_ROUND_TRUNCATE>(lMETSquaredCORDIC, lMETCORDIC);

  //outputting sums
  //converting from 32 -> 16 bit
  TPt lMET = lMETCORDIC.out;

  // LOOKING UP FOR PHI:
  // computing the cosine of MET, running dicotomic search for closest value to obtain iPhi
  // preventing division by zero, which may happen while MET is not fully computed
  TSinCos lCosMET = 0;
  TPhi lMETPhi = 0;
  if (lMET != 0) {
    lCosMET = sTotalPx/lMET;
    lMETPhi = PHI_GRID_SIZE - 1 - binary_bin_finder<const TSinCos, TPhi, 8, 4>(monotonic_cos_iPhi, lCosMET);
  }
  //TODO: if py is negative then phi is in the pi - 2*pi range

  //outputting sums
  // sums should be outputted only when the last jet phi row has been analysed
  // otherwise output 0
  bool lOutputSums = (lPhiIndex == (PHI_GRID_SIZE + PHI_JET_SIZE - 2));
  outSums.met = lOutputSums ? lMET : kNullPt;
  outSums.ht = lOutputSums ? sHT : kNullPt;
  outSums.metPhi = lOutputSums ? lMETPhi : kNullPhi;
  outSums._dummy = 0;

  return;
}


// runs the jet finder over the calo slice
void hls_runJetFinders(const CaloGridBuffer inCaloGrid, Jets outJets) 
{ 
  #pragma HLS pipeline II=1
  #pragma HLS inline
  // runs a jet finder for each possible jet centre of the calo slice
  jetFinderEtaLoop: for (TEta iEta = 0 ; iEta < ETA_GRID_SIZE ; iEta++) 
  {
    CaloGridBuffer lTmpCaloGrid;
    hls_copyGrid( inCaloGrid, lTmpCaloGrid );
    #pragma HLS array_partition variable=lTmpCaloGrid complete dim=0
    Jet lTmpJet;
    lTmpJet.pt = hls_findJet(inCaloGrid, iEta, PHI_JET_SIZE/2);
    // if (lTmpJet.pt > 0) std::raise(SIGINT);
    lTmpJet.iPhi = PHI_JET_SIZE/2;
    lTmpJet.iEta = iEta;
    outJets[iEta] = lTmpJet;
  }
}


// runs the comparison mask while computing the total pt of the jet
// if the mask is not satisfied, the jet pt is set to 0
TPt hls_findJet(const CaloGridBuffer caloGrid, TEta iEtaCentre, TPhi iPhiCentre) 
{
  #pragma HLS inline
  #pragma HLS pipeline II=1
  TPt centralPt = caloGrid[iPhiCentre][iEtaCentre];
  bool isLocalMaximum = (centralPt < SEED_THRESHOLD) ? false : true;
  TPt ptSum = 0;
  // Scanning through the grid centered on the seed for the mask while computing total pt
  checkMaximumEtaLoop: for (TEta etaIndex = 0; etaIndex < ETA_JET_SIZE; etaIndex++)
  {
    checkMaximumPhiLoop: for (TPhi phiIndex = 0; phiIndex < PHI_JET_SIZE; phiIndex++)
    {
      #pragma HLS pipeline II=1
      TEta lEtaOffset = etaIndex - ETA_JET_SIZE/2;
      TPhi lPhiOffset = phiIndex - PHI_JET_SIZE/2;
      TPt towerEnergy = hls_getTowerEnergy(caloGrid, iEtaCentre + lEtaOffset, iPhiCentre + lPhiOffset);
      ptSum += towerEnergy;
      if ((etaIndex == ETA_JET_SIZE/2) && (phiIndex == ETA_JET_SIZE/2)) continue;
      if (centralPt < towerEnergy) {
        isLocalMaximum = false;
        continue; 
      }
      if (phiIndex > PHI_JET_SIZE/2) 
      {
        if (phiIndex > ETA_JET_SIZE - 1 - etaIndex)
        {
          isLocalMaximum = (isLocalMaximum) ? (centralPt > towerEnergy) : false;
        } else 
        {
          isLocalMaximum = (isLocalMaximum) ? (centralPt >= towerEnergy) : false;
        }
      } else 
      {
        if (phiIndex >= ETA_JET_SIZE - 1 - etaIndex)
        {
          isLocalMaximum = (isLocalMaximum) ? (centralPt > towerEnergy) : false;
        } else
        {
          isLocalMaximum = (isLocalMaximum) ? (centralPt >= towerEnergy) : false;
        }
      }
    }
  }

  // return the momentum only if the mask is passed
  if (isLocalMaximum) {
    return ptSum;
  }
  else
    return 0;
}