#include "L1Trigger/L1TTrackMatch/interface/Cordic.h"

#include <cmath>
#include <memory>

using namespace L1TkEtMissEmuAlgo;

Cordic::Cordic(int aPhiScale, int aMagnitudeBits, const int aSteps, bool debug,
               bool writeLUTs)
    : mPhiScale(aPhiScale),
      mMagnitudeScale(1 << aMagnitudeBits),
      mMagnitudeBits(aMagnitudeBits),
      cordicSteps(aSteps),
      debug(debug) {
  atanLUT.reserve(aSteps);
  magNormalisationLUT.reserve(aSteps);

  if (debug) {
    edm::LogVerbatim("L1TkEtMissEmulator") << "=====atan LUT=====";
  }

  for (int i = 0; i < aSteps; i++) {
    atanLUT.push_back(
        METphi_t(round(mPhiScale * atan(pow(2, -i)) / (2 * M_PI))));
    if (debug) {
      edm::LogVerbatim("L1TkEtMissEmulator") << atanLUT[i] << " | ";
    }
  }
  if (debug) {
    edm::LogVerbatim("L1TkEtMissEmulator") << "\n=====Normalisation LUT=====";
  }

  float val = 1.0;
  for (int j = 0; j < aSteps; j++) {
    val = val / (pow(1 + pow(4, -j), 0.5));
    magNormalisationLUT.push_back(Et_t(round(mMagnitudeScale * val)));
    if (debug) {
      edm::LogVerbatim("L1TkEtMissEmulator") << magNormalisationLUT[j] << " | ";
    }
  }

  if (writeLUTs) {
    writeLUTtoFile<METphi_t>(atanLUT, "cordicatan", ",");
    writeLUTtoFile<Et_t>(magNormalisationLUT, "cordicrenorm", ",");
  }
}

EtMiss Cordic::toPolar(Et_t x, Et_t y) const {
  Et_t new_x = 0;
  Et_t new_y = 0;

  METphi_t phi = 0;
  METphi_t new_phi = 0;
  bool sign = false;

  EtMiss ret_etmiss;

  if (debug) {
    edm::LogVerbatim("L1TkEtMissEmulator") << "\n=====Cordic Steps=====";
  }

  if (x >= 0 && y >= 0) {
    phi = 0;
    sign = true;
    x = x;
    y = y;
  } else if (x < 0 && y >= 0) {
    phi = mPhiScale >> 1;
    sign = false;
    x = -x;
    y = -y;
  } else if (x < 0 && y < 0) {
    phi = mPhiScale >> 1;
    sign = true;
    x = -x;
    y = -y;
  } else {
    phi = mPhiScale;
    sign = false;
    x = x;
    y = -y;
  }

  for (int step = 0; step < cordicSteps; step++) {
    if (y < 0) {
      new_x = x - (y >> step);
      new_y = y + (x >> step);
    } else {
      new_x = x + (y >> step);
      new_y = y - (x >> step);
    }

    if ((y < 0) == sign) {
      new_phi = phi - atanLUT[step];
    } else {
      new_phi = phi + atanLUT[step];
    }

    x = new_x;
    y = new_y;
    phi = new_phi;

    if (debug) {
      edm::LogVerbatim("L1TkEtMissEmulator")
          << " Cordic x: " << x << " Cordic y: " << y << " Cordic phi: " << phi
          << "\n";
    }
  }

  // Cordic performs calculation in internal Et granularity, convert to final
  // granularity for Et word
  ret_etmiss.Et =
      (int)(x * magNormalisationLUT[cordicSteps - 1] * maxTrackPt / maxMET) >>
      (mMagnitudeBits + TTTrack_TrackWord::TrackBitWidths::kRinvSize -
       kMETSize);
  ret_etmiss.Phi = phi;
  return ret_etmiss;
}
