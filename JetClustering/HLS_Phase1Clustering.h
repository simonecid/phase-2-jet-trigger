#ifndef __HLS_PHASE1CLUSTERING_H__
#define __HLS_PHASE1CLUSTERING_H__

/*
The code has an internal buffer which is PHI_GRID_SIZE * ETA_GRID_SIZE big
Each clock cycle I receive PHI_GRID_SIZE bins (all in the same eta).
I can run the jet finder once I received data able to cover PHI_GRID_SIZE*ETA_JET_SIZE region
*/

// size of the buffer holding the calogrid
#define PHI_GRID_SIZE 8
#define ETA_GRID_SIZE 18
#define ETA_JET_SIZE 7
#define NUMBER_OF_LINKS 72
// 1 in pt means 0.25 GeV
#define PT_LSB 0.25 // GeV
// 1 in eta means 0.0833333 
#define ETA_LSB 0.0833333 
// 1 in phi mean 0.0875
#define PHI_LSB 0.0875

//threshold for seeding a jet
#define SEED_THRESHOLD 20
//threshold for computing HT sum
#define HT_SUM_THRESHOLD 120

// after how many clock cycles the algo should reset
#define RESET_PERIOD 15

#define PHI_JET_SIZE ETA_JET_SIZE
#define NUMBER_OF_SEEDS ETA_GRID_SIZE

#if PHI_GRID_SIZE>=128
#error "PHI_GRID_SIZE is >= 128, this is not going to work"
#endif
#if ETA_GRID_SIZE>=128
#error "ETA_GRID_SIZE is >= 128, this is not going to work"
#endif

#include <ap_int.h>

//pt is represented by a 16 bit unsigned int
typedef ap_uint<16> TPt; // LSB = 0.25 GeV
// px and py is a 16-bit signed int
typedef ap_int<16> TMomentum; // LSB = 0.25 GeV
// eta of the jet is represented by a 8 bit unsigned int
typedef ap_uint<8> TEta; // LSB is eq. to 0.75/9
// phi of the jet is represented by a 8 bit unsigned int
typedef ap_uint<8> TPhi; // LSB is eq. to 0.7/8
// sin and cos are represented by a 8-bit fixed point decimal, whose integer part is 1 bit (always 0) and decimal 7 bit
// precision is rounded (not sure what it means)
typedef ap_ufixed<8, 1, AP_RND> TSinCos;

// defining how a jet is packed in a single 32-bit word
typedef struct {
  TPt pt; // bit 0-15 hold pt
  TPhi iPhi; // bit 16-23 hold phi of jet
  TEta iEta; // bit 24-31 hold eta of jet
} Jet;

// defining how sums are packed on a single link
typedef struct {
  TPt ht; // bit 0-15 hold ht
  TPt met; // bit 16-31 hold met
  TPhi metPhi; // bit 48-55 hold met phi
  ap_uint<8> _dummy; // 56-63 is empty
} SumLink;

// defining how a jet is packed on a single 64-bit link, bit 0-31 hold jet 0, bit 32-63 hold jet 1
typedef Jet JetLink[2];
// defining a collection of links for output
typedef JetLink JetLinks[NUMBER_OF_LINKS];

// data type for storing the entire calorimeter (not used)
typedef TPt CaloGrid[PHI_GRID_SIZE][ETA_GRID_SIZE];
// data type for storing the part of the calorimeter on which the jets will be found
typedef TPt CaloGridBuffer[PHI_JET_SIZE][ETA_GRID_SIZE];
// phi slice of calorimeter
typedef TPt CaloGridPhiSlice[ETA_GRID_SIZE];
// collection of all the possible jets that can be found in the buffer
typedef Jet Jets[NUMBER_OF_SEEDS];

// defining typed nil quantities for ternary operator
const TPhi kNullPhi = 0;
const TEta kNullEta = 0;
const TPt kNullPt = 0;
const TMomentum kNullMomentum = 0;

// see .cpp file for comments
void hls_copyGrid (const CaloGridBuffer inCaloGrid, CaloGridBuffer outCaloGrid);
void hls_jet_clustering(const CaloGridPhiSlice inCaloGridPhiSlice, JetLinks outJets, SumLink & outSums, bool d0Valid);
TPt hls_getTowerEnergy(const CaloGridBuffer caloGrid, TEta iEta, TPhi iPhi);
TPt hls_findJet(const CaloGridBuffer caloGrid, TEta iEtaCentre, TPhi iPhiCentre);
void hls_runJetFinders(const CaloGridBuffer inCaloGrid, Jets outJets);
void hls_copyLine (const CaloGridPhiSlice caloGridPhiSlice, CaloGridBuffer outCaloGrid, TEta etaIndex);
void hls_shiftGridLeft (const CaloGridBuffer inCaloGrid, CaloGridBuffer outCaloGrid);
void hls_clearGrid(CaloGridBuffer grid);
TPt hls_computeSumOfTowers (const CaloGridPhiSlice inCaloGridPhiSlice);

#ifndef __SYNTHESIS__
void printCaloGrid(const CaloGridBuffer caloGrid);
#endif


// function that finds the bin
template<class TValue, class TIndex, int vecSize, int offset=0, int range=vecSize>
class BinFinderWrapper
{
  public: 
  static TIndex binary_bin_finder(TValue values[vecSize], TValue x);
};

template<class TValue, class TIndex, int vecSize, int offset> 
class BinFinderWrapper<TValue, TIndex, vecSize, offset, 1>
{
  public: 
  static TIndex binary_bin_finder(TValue values[vecSize], TValue x) 
  {
    #pragma HLS inline
    return offset;
  };
};

template<class TValue, class TIndex, int vecSize>
class BinFinderWrapper<TValue, TIndex, vecSize, 0, vecSize>
{
  public: 
  static TIndex binary_bin_finder(TValue values[vecSize], TValue x) 
  {
    #pragma HLS inline
    return BinFinderWrapper<TValue, TIndex, vecSize, 0, vecSize>::binary_bin_finder(values, x);
  };
};



template<class TValue, class TIndex, int vecSize, int offset, int range> 
TIndex BinFinderWrapper<TValue, TIndex, vecSize, offset, range>::binary_bin_finder(TValue values[vecSize], TValue x)
{
  #pragma HLS inline
  constexpr TIndex lMiddleElementIndex = offset + range / 2;
  if (x >= values[lMiddleElementIndex]) 
  {
    return BinFinderWrapper<TValue, TIndex, vecSize, offset + range / 2, range / 2 + range % 2>::binary_bin_finder(values, x);
  } 
  else
  {
    return BinFinderWrapper<TValue, TIndex, vecSize, offset, range / 2>::binary_bin_finder(values, x);
  }
}

template<class TValue, class TIndex, int vecSize, int depth>
TIndex binary_bin_finder(TValue values[vecSize], TValue x)
{
  #pragma HLS inline
  TIndex lOffset = 0, lRange = vecSize;
  for (int y = 0; y < depth; y++)
  {
    TIndex lMiddleElementIndex = lOffset + lRange / 2;
    if (x >= values[lMiddleElementIndex]) 
    {
      lOffset += lRange / 2;
      lRange = lRange / 2 + lRange % 2;
    } 
    else
    {
      lRange /= 2;
    }
  }

  return lOffset;
}

#endif //__HLS_PHASE1CLUSTERING_H__