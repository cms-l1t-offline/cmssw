#include "L1Trigger/L1TTrackMatch/interface/L1TkEtMissEmuTrackTransform.h"

#include <cmath>

void L1TkEtMissEmuTrackTransform::generateLUTs() {
  phiQuadrants = generatePhiSliceLUT(L1TkEtMissEmuAlgo::NQuadrants);
  phiShift = generatePhiSliceLUT(L1TkEtMissEmuAlgo::NSector);
}

InternalEtWord L1TkEtMissEmuTrackTransform::transformTrack(
    TTTrack<Ref_Phase2TrackerDigi_>& track_ref, l1t::Vertex& PV) {
  InternalEtWord Outword;

  if (GTTinput_) {
    if ((track_ref.getRinvWord() &
         (1 << (TTTrack_TrackWord::TrackBitWidths::kRinvSize - 1))) != 0) {
      // Only Want Magnitude of Pt for sums so perform absolute value
      Outword.pt =
          abs((1 << (TTTrack_TrackWord::TrackBitWidths::kRinvSize - 1)) -
              track_ref.getRinvWord());
    } else {
      Outword.pt = track_ref.getRinvWord();
    }

    if ((track_ref.getTanlWord() &
         (1 << (TTTrack_TrackWord::TrackBitWidths::kTanlSize - 1))) != 0) {
      // Only Want Magnitude of Eta for cuts and track to vertex association so
      // perform absolute value
      Outword.eta = (1 << (TTTrack_TrackWord::TrackBitWidths::kTanlSize)) -
                    track_ref.getTanlWord();
    } else {
      Outword.eta = track_ref.getTanlWord();
    }

  } else {
    track_ref.setTrackWordBits();
    Outword.pt =
        digitizeSignedValue<pt_t>(track_ref.momentum().perp(),
                                  TTTrack_TrackWord::TrackBitWidths::kRinvSize,
                                  L1TkEtMissEmuAlgo::stepPt);
    Outword.eta =
        digitizeSignedValue<eta_t>(abs(track_ref.momentum().eta()),
                                   TTTrack_TrackWord::TrackBitWidths::kTanlSize,
                                   L1TkEtMissEmuAlgo::stepEta);
  }

  if (VtxEmulator_) {
    Outword.pV = PV.z0();  // Convert vertex TODO change to correct vertexing
                           // emulation output
  }

  else {
    Outword.pV = digitizeSignedValue<TTTrack_TrackWord::z0_t>(
        PV.z0(), TTTrack_TrackWord::TrackBitWidths::kZ0Size,
        TTTrack_TrackWord::stepZ0);  // Convert vertex
  }

  Outword.chi2rphidof = track_ref.getChi2RPhiWord();
  Outword.chi2rzdof = track_ref.getChi2RZWord();
  Outword.bendChi2 = track_ref.getBendChi2Word();
  Outword.nstubs = countNStub(track_ref.getHitPatternWord());

  unsigned int Sector = track_ref.phiSector();
  Outword.Sector = Sector;
  // convert to local phi
  Outword.phi = track_ref.phi();
  TTTrack_TrackWord::phi_t localPhi =
      floatGlobalPhiToSectorPhi(track_ref.phi(), Sector);
  // Convert to global phi
  Outword.globalPhi = localToGlobalPhi(localPhi, phiShift[Sector]);
  Outword.z0 = track_ref.getZ0Word();

  return Outword;
}

global_phi_t L1TkEtMissEmuTrackTransform::localToGlobalPhi(
    TTTrack_TrackWord::phi_t local_phi, global_phi_t sector_shift) {
  int PhiShift = 1 << L1TkEtMissEmuAlgo::kGlobalPhiBins - 1;
  int PhiMin = phiQuadrants.front();
  int PhiMax = phiQuadrants.back();
  int phiMultiplier = TTTrack_TrackWord::TrackBitWidths::kPhiSize -
                      L1TkEtMissEmuAlgo::kGlobalPhiSize;

  int tempPhi = 0;
  global_phi_t globalPhi = 0;

  tempPhi = (unpackSignedValue(local_phi,
                               TTTrack_TrackWord::TrackBitWidths::kPhiSize) /
             pow(2, phiMultiplier)) +
            sector_shift - PhiShift;
  if (tempPhi < PhiMin) {
    tempPhi = tempPhi + PhiMax;
  } else if (tempPhi > PhiMax) {
    tempPhi = tempPhi - PhiMax;
  } else
    tempPhi = tempPhi;

  globalPhi = global_phi_t(tempPhi);

  return globalPhi;
}

nstub_t L1TkEtMissEmuTrackTransform::countNStub(
    TTTrack_TrackWord::hit_t Hitpattern) {
  nstub_t Nstub = 0;
  for (int i = (TTTrack_TrackWord::kHitPatternSize - 1); i >= 0; i--) {
    int k = Hitpattern >> i;
    if (k & 1) Nstub++;
  }
  return Nstub;
}

TTTrack_TrackWord::phi_t L1TkEtMissEmuTrackTransform::floatGlobalPhiToSectorPhi(
    float phi, unsigned int sector) {
  float tempPhi = 0.0;
  if (sector < 4) {
    tempPhi = phi - (sector * (2 * M_PI) / 9);
  } else if (sector > 5) {
    tempPhi = phi + ((9 - sector) * (2 * M_PI) / 9);
  } else if (sector == 4) {
    if (phi > 0) {
      tempPhi = phi - (sector * (2 * M_PI) / 9);
    } else {
      tempPhi = phi + ((9 - sector) * (2 * M_PI) / 9);
    }
  } else if (sector == 5) {
    if (phi < 0) {
      tempPhi = phi + ((9 - sector) * (2 * M_PI) / 9);
    } else {
      tempPhi = phi - (sector * (2 * M_PI) / 9);
    }
  }
  return digitizeSignedValue<TTTrack_TrackWord::phi_t>(
      tempPhi, TTTrack_TrackWord::TrackBitWidths::kPhiSize,
      TTTrack_TrackWord::stepPhi0);
}

std::vector<global_phi_t> L1TkEtMissEmuTrackTransform::generatePhiSliceLUT(
    unsigned int N) {
  float sliceCentre = 0.0;
  std::vector<global_phi_t> phiLUT;
  for (unsigned int q = 0; q <= N; q++) {
    phiLUT.push_back((global_phi_t)(sliceCentre /
                                    (-2 * TTTrack_TrackWord::minPhi0 /
                                     (L1TkEtMissEmuAlgo::kGlobalPhiBins - 1))));
    sliceCentre += 2 * M_PI / N;
  }
  return phiLUT;
}
