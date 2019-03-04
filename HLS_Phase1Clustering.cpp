#include "HLS_Phase1Clustering.h"
#include "hls_stream.h"

#ifndef __SYNTHESIS__
#include <iostream>
#include <csignal>
#endif

char getNormalisedPhi(char iPhi)
{
  if (iPhi < 0) 
  {
    iPhi += PHI_GRID_SIZE;
  }
  else if (iPhi > PHI_GRID_SIZE - 1) 
  {
    iPhi -= PHI_GRID_SIZE;
  } 
  return iPhi;  
}

unsigned short int getTowerEnergy(const CaloGrid caloGrid, char iEta, char iPhi)
{
  // We return the pt of a certain bin in the calo grid, taking account of the phi periodicity when overflowing (e.g. phi > phiSize), and returning 0 for the eta out of bounds

  if (iPhi < 0) 
  {
    //iPhi += PHI_GRID_SIZE;
    //!TODO: ADD OVERFLOW BUFFERS
    return 0;
  }
  if (iPhi > PHI_GRID_SIZE - 1) 
  {
    //iPhi -= PHI_GRID_SIZE;
    //!TODO: ADD OVERFLOW BUFFERS
    return 0;
  }
  if (iEta < 0) 
  {
    //std::cout << "Returning (pt, ieta, iphi): " << "(" << 0 << ", " << iEta << ", " << iPhi << ")" << std::endl;
    return 0;
  }
  if (iEta > ETA_GRID_SIZE - 1) 
  {
    //std::cout << "Returning (pt, ieta, iphi): " << "(" << 0 << ", " << iEta << ", " << iPhi << ")" << std::endl;
    return 0;
  }
    //std::cout << "Returning (pt, ieta, iphi): " << "(" << caloGrid.GetBinContent(iEta, iPhi) << ", " << iEta << ", " << iPhi << ")" << std::endl;
  return caloGrid[iEta][iPhi];
}

void buildJets(const CaloGrid caloGrid, Jet jets[NUMBER_OF_SEEDS], char etaShift)
{
  //#pragma HLS array_partition variable=caloGrid complete dim=0
  //#pragma HLS array_partition variable=jets complete dim=0
  #if NUMBER_OF_SEEDS > 256
  #pragma message "Working with more than 256 jets, moving to 2-byte index counter, please do not go over 65536 jets or this may break"
  unsigned int jetIdx = 0;
  #else
  char jetIdx = 0;
  #endif

  // for each point of the grid check if it is a local maximum
  // to do so I take a point, and look if is greater than the points around it (in the 9x9 neighborhood)
  // to prevent mutual exclusion, I check greater or equal for points above and right to the one I am considering (including the top-left point)
  // to prevent mutual exclusion, I check greater for points below and left to the one I am considering (including the bottom-right point)

  #if PHI_GRID_SIZE > 256
  #error "Phi grid size is above 256, looping variable not big enough"
  #endif 
  
  #if PHI_SCAN_PIPELINE_AND_UNROLL==true
  #pragma HLS pipeline
  #endif
  seedFinderPhiScanLoop: for (char iPhi = 0; iPhi < PHI_GRID_SIZE; iPhi++)
  {
    //Excluding eta range in which the grid falls out of bounds
    #if ETA_GRID_SIZE > 256
    #error "Eta grid size is above 256, looping variable not big enough"
    #endif
    #if PHI_SCAN_PIPELINE_ONLY==true
    #pragma HLS pipeline
    #endif
    seedFinderEtaScanLoop: for (char iEta = ETA_JET_SIZE/2; iEta < ETA_GRID_SIZE - ETA_JET_SIZE/2; iEta++)
    {
      jets[iPhi].pt = findJet(caloGrid, iEta, iPhi);
      jets[iPhi].iEta = iEta + etaShift;
      jets[iPhi].iPhi = iPhi;
    }
  }
  return;
}

void copyGrid (const CaloGrid inCaloGrid, CaloGrid outCaloGrid)
{
  #pragma HLS pipeline
  copyEtaGrid: for(unsigned char iEta = 0 ; iEta < ETA_GRID_SIZE ; iEta++)
  {
    copyPhiGrid: for (unsigned char iPhi = 0 ; iPhi < PHI_GRID_SIZE ; iPhi++)
    {
      outCaloGrid[iEta][iPhi] = inCaloGrid[iEta][iPhi];
    }
  }
}

void copyJets (const Jets inJets, Jets outJets)
{
  #pragma HLS pipeline
  copyJets: for(unsigned char jetIdx = 0 ; jetIdx < NUMBER_OF_SEEDS ; jetIdx++)
  {
    outJets[jetIdx] = inJets[jetIdx];
  }
}

void hls_main(CaloGrid inCaloGrid, const char inEtaShift, Jets outJets) 
{
  #pragma HLS array_partition variable=inCaloGrid complete dim=0
  #pragma HLS array_partition variable=outJets complete dim=0
  #if HLS_MAIN_FULLY_PIPELINED==true
  #pragma HLS pipeline
  #endif
  CaloGrid lCaloGrid[PIPELINE_LENGTH];
  #pragma HLS array_partition variable=lCaloGrid complete dim=0
  Jets lJets[PIPELINE_LENGTH];
  #pragma HLS array_partition variable=lJets complete dim=0
  char lEtaShift[PIPELINE_LENGTH];
  #pragma HLS array_partition variable=lEtaShift complete dim=0
  
  seedFinderPhiScanPipelineLoop: for (unsigned char iPhiPipeline = PIPELINE_START ; iPhiPipeline < PIPELINE_LENGTH ; iPhiPipeline++) 
  {
    CaloGrid lTmpCaloGrid;
    Jets lTmpJets;
    char lTmpEtaShift;
    #pragma HLS array_partition variable=lTmpCaloGrid complete dim=0
    #pragma HLS array_partition variable=lTmpJets complete dim=0

    //copying stuff from the previous stage or from input
    if (iPhiPipeline == PIPELINE_START)
    {
      copyGrid( inCaloGrid, lTmpCaloGrid );
      lTmpEtaShift = inEtaShift;
    } 
    else
    {
      copyGrid( lCaloGrid[iPhiPipeline - 1], lTmpCaloGrid );
      lTmpEtaShift = lEtaShift[iPhiPipeline - 1];
    }
    copyGrid( lTmpCaloGrid, lCaloGrid[iPhiPipeline] );
    lEtaShift[iPhiPipeline] = lTmpEtaShift;
    copyJets( lJets[iPhiPipeline], lTmpJets );
    
    lTmpJets[iPhiPipeline].pt = findJet(lTmpCaloGrid, ETA_GRID_SIZE/2, iPhiPipeline);
    lTmpJets[iPhiPipeline].iEta = ETA_GRID_SIZE/2 + lTmpEtaShift;
    lTmpJets[iPhiPipeline].iPhi = iPhiPipeline;

    //copying jets to output or to the next stage
    if (iPhiPipeline == PIPELINE_LENGTH - 1)
    {
      copyJets( lTmpJets, outJets );
    } 
    else
    {
      copyJets( lTmpJets, lJets[iPhiPipeline + 1] );
    }
  }
  return;  
}

unsigned short int findJet(const CaloGrid caloGrid, char iEtaCentre, char iPhiCentre) 
{
  #if FINDJET_PIPELINE_AND_UNROLL==true
  #pragma HLS pipeline
  #endif
  unsigned short int centralPt = caloGrid[iEtaCentre][iPhiCentre];
  bool isLocalMaximum = (centralPt < SEED_THRESHOLD) ? false : true;
  unsigned short int ptSum = 0;
  // Scanning through the grid centered on the seed
  checkMaximumEtaLoop: for (char etaIndex = -ETA_JET_SIZE/2; etaIndex <= ETA_JET_SIZE/2; etaIndex++)
  {
    checkMaximumPhiLoop: for (char phiIndex = -PHI_JET_SIZE/2; phiIndex <= PHI_JET_SIZE/2; phiIndex++)
    {
      #if FINDJET_PIPELINE==true
      #pragma HLS pipeline
      #endif
      unsigned int towerEnergy = getTowerEnergy(caloGrid, iEtaCentre + etaIndex, iPhiCentre + phiIndex);
      ptSum += towerEnergy;
      if ((etaIndex == 0) && (phiIndex == 0)) continue;
      if (centralPt < towerEnergy) {
        isLocalMaximum = false;
        continue; 
      }
      if (phiIndex > 0) 
      {
        if (phiIndex >= -etaIndex)
        {
          isLocalMaximum = (isLocalMaximum) ? (centralPt > towerEnergy) : false;
        }
      } else 
      {
        if (phiIndex > -etaIndex)
        {
          isLocalMaximum = (isLocalMaximum) ? (centralPt > towerEnergy) : false;
        }
      }
    }
  }
  return isLocalMaximum ? ptSum : 0;
}