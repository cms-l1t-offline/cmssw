#include "L1Trigger/L1TTrackMatch/interface/L1TkEtMissEmuAlgo.h"

namespace L1TkEtMissEmuAlgo {
std::vector<global_phi_t> generateCosLUT(
    unsigned int size) {  // Fill cosine LUT with integer values
  float phi = 0;
  std::vector<global_phi_t> cosLUT;
  for (unsigned int LUT_idx = 0; LUT_idx <= size; LUT_idx++) {
    cosLUT.push_back((global_phi_t)(cos(phi) * (kGlobalPhiBins - 1)));
    phi += (2 * -TTTrack_TrackWord::minPhi0) / (kGlobalPhiBins - 1);
  }
  return cosLUT;
}

// Generate Eta LUT for track to vertex association
std::vector<eta_t> generateEtaRegionLUT() {
  std::vector<eta_t> LUT;
  for (unsigned int q = 0; q < NEtaRegion + 1; q++) {
    LUT.push_back(digitizeSignedValue<eta_t>(
        EtaRegionBins[q], TTTrack_TrackWord::TrackBitWidths::kTanlSize,
        2 * L1TkEtMissEmuAlgo::maxTrackEta /
            (1 << (TTTrack_TrackWord::TrackBitWidths::kTanlSize))));
  }
  return LUT;
}

std::vector<TTTrack_TrackWord::z0_t> generateDeltaZLUT() {
  std::vector<TTTrack_TrackWord::z0_t> LUT;
  for (unsigned int q = 0; q < NEtaRegion; q++) {
    LUT.push_back(digitizeSignedValue<TTTrack_TrackWord::z0_t>(
        DeltaZBins[q], TTTrack_TrackWord::TrackBitWidths::kZ0Size,
        TTTrack_TrackWord::stepZ0));
  }
  return LUT;
}

int unpackSignedValue(unsigned int bits, unsigned int nBits) {
  int isign = 1;
  unsigned int digitized_maximum = (1 << nBits) - 1;
  if (bits & (1 << (nBits - 1))) {  // check the sign
    isign = -1;
    bits = (1 << (nBits + 1)) -
           bits;  // if negative, flip everything for two's complement encoding
  }
  return (int(bits & digitized_maximum)) * isign;
}

}  // namespace L1TkEtMissEmuAlgo