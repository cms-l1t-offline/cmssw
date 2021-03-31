#ifndef L1Trigger_L1TTrackMatch_L1TkEtMissEmuAlgo_HH
#define L1Trigger_L1TTrackMatch_L1TkEtMissEmuAlgo_HH

#include <ap_int.h>

#include <cmath>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <numeric>

#include "DataFormats/L1TrackTrigger/interface/TTTrack_TrackWord.h"
#include "DataFormats/L1TrackTrigger/interface/TTTypes.h"

using namespace std;
// Namespace that defines constants and types used by the EtMiss Emulation
// Includes functions for writing LUTs and converting to integer representations
namespace L1TkEtMissEmuAlgo {

const unsigned int kGlobalPhiSize{11}; 
const unsigned int kGlobalPhiExtra{3}; 
// Extra bits needed by global phi to span full range
const unsigned int kEtExtra{8};  
// Extra room for Et sums

const unsigned int kMETSize{15};     // For output Magnitude
const unsigned int kMETPhiSize{14};  // For Output Phi

typedef ap_uint<3> nstub_t;

typedef ap_uint<kGlobalPhiSize + kGlobalPhiExtra> global_phi_t;

typedef ap_uint<TTTrack_TrackWord::TrackBitWidths::kRinvSize> pt_t;
typedef ap_uint<TTTrack_TrackWord::TrackBitWidths::kTanlSize> eta_t;
// For internal Et representation, sums become larger than initial pt
// representation
typedef ap_int<TTTrack_TrackWord::TrackBitWidths::kRinvSize + kEtExtra> Et_t;

typedef ap_uint<kMETSize> MET_t;
// Cordic means this is evaluated between 0 and 2Pi rather than -pi to pi so
// unsigned
typedef ap_uint<kMETPhiSize> METphi_t;

const unsigned int kGlobalPhiBins = 1 << kGlobalPhiSize;
const unsigned int kMETBins = 1 << kMETSize;
const unsigned int kMETPhiBins = 1 << kMETPhiSize;

const unsigned int NEtaRegion{6};
const unsigned int NSector{9};
const unsigned int NQuadrants{4};

const float maxTrackPt{1024};  // TODO calculate from GTTinput module
const float maxTrackEta{4};    // TODO calculate from GTTinput module

const double stepPt = (std::abs(maxTrackPt)) /
                      (1 << TTTrack_TrackWord::TrackBitWidths::kRinvSize);
const double stepEta = (2 * std::abs(maxTrackEta)) /
                       (1 << TTTrack_TrackWord::TrackBitWidths::kTanlSize);

const float maxMET{4000};  // 4 TeV
const float maxMETPhi{2 * M_PI};

const double stepMET =
    (L1TkEtMissEmuAlgo::maxMET / L1TkEtMissEmuAlgo::kMETBins);
const double stepMETPhi =
    (L1TkEtMissEmuAlgo::maxMETPhi / L1TkEtMissEmuAlgo::kMETPhiBins);

const float EtaRegionBins[NEtaRegion + 1] = {0, 0.7, 1.0, 1.2, 1.6, 2.0, 2.4};
const float DeltaZBins[NEtaRegion] = {0.4, 0.6, 0.76, 1.0, 1.7, 2.2};

// Enough symmetry in cos and sin between 0 and pi/2 to get all possible values
// of cos and sin phi
const float maxCosLUTPhi{M_PI / 2};

const string LUTdir{"LUTs/"};

// Simple struct used for ouput of cordic
struct EtMiss {
  MET_t Et;
  METphi_t Phi;
};

std::vector<global_phi_t> generateCosLUT(unsigned int size);
std::vector<eta_t> generateEtaRegionLUT();
std::vector<TTTrack_TrackWord::z0_t> generateDeltaZLUT();

template <typename T>
T digitizeSignedValue(double value, unsigned int nBits, double lsb) {
  T digitized_value = std::floor(std::abs(value) / lsb);
  T digitized_maximum =
      (1 << (nBits - 1)) -
      1;  // The remove 1 bit from nBits to account for the sign
  if (digitized_value > digitized_maximum) digitized_value = digitized_maximum;
  if (value < 0)
    digitized_value =
        (1 << nBits) - digitized_value;  // two's complement encoding
  return digitized_value;
}

template <typename T>
unsigned int getBin(double value, const T& bins) {
  auto up = std::upper_bound(bins.begin(), bins.end(), value);
  return (up - bins.begin() - 1);
}

int unpackSignedValue(unsigned int bits, unsigned int nBits);

template <typename T>
void writeLUTtoFile(vector<T>(&LUT), const string& filename,
                    const string& delimiter) {
  int fail = system((string("mkdir -p ") + L1TkEtMissEmuAlgo::LUTdir).c_str());
  if (fail)
    throw cms::Exception("BadDir")
        << __FILE__ << " " << __LINE__ << " could not create directory "
        << L1TkEtMissEmuAlgo::LUTdir;

  const string fname = L1TkEtMissEmuAlgo::LUTdir + filename + ".tab";
  ofstream outstream(fname);
  if (outstream.fail())
    throw cms::Exception("BadFile")
        << __FILE__ << " " << __LINE__ << " could not create file " << fname;

  outstream << "{" << endl;
  for (unsigned int i = 0; i < LUT.size(); i++) {
    if (i != 0) outstream << delimiter << endl;
    outstream << LUT[i];
  }
  outstream << endl << "};" << endl;
  outstream.close();
}

}  // namespace L1TkEtMissEmuAlgo
#endif
