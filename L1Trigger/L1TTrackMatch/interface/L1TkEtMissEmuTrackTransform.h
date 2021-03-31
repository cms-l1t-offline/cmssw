#ifndef L1Trigger_L1TTrackMatch_L1TkEtMissEmuTrackTransform_HH
#define L1Trigger_L1TTrackMatch_L1TkEtMissEmuTrackTransform_HH

#include "DataFormats/L1TrackTrigger/interface/TTTrack_TrackWord.h"
#include "DataFormats/L1TrackTrigger/interface/TTTypes.h"
#include "DataFormats/L1Trigger/interface/Vertex.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "L1Trigger/L1TTrackMatch/interface/L1TkEtMissEmuAlgo.h"

/*
** class  : L1TkEtMissEmuTrackTransform
** author : Christopher Brown
** date   : 19/02/2021
** brief  : Converts TTrack_trackword to internal Et word including vertex

**        : 
*/

using namespace L1TkEtMissEmuAlgo;

// Internal Word used by EtMiss Emulation, producer expects this wordtype
struct InternalEtWord {
  TTTrack_TrackWord::z0_t pV;
  TTTrack_TrackWord::z0_t z0;

  pt_t pt;
  eta_t eta;
  global_phi_t globalPhi;
  nstub_t nstubs;

  TTTrack_TrackWord::bendChi2_t bendChi2;
  TTTrack_TrackWord::chi2rphi_t chi2rphidof;
  TTTrack_TrackWord::chi2rz_t chi2rzdof;

  unsigned int Sector;

  float phi;  // Used to debug cos phi LUT
};


class L1TkEtMissEmuTrackTransform {
 public:
  L1TkEtMissEmuTrackTransform() = default;
  ~L1TkEtMissEmuTrackTransform(){};

  void generateLUTs();  // Generate internal LUTs needed for track transfrom

  // Transform track and vertex
  InternalEtWord transformTrack(TTTrack<Ref_Phase2TrackerDigi_>& track_ref,
                                l1t::Vertex& PV);

  // Converts local int phi to global int phi
  global_phi_t localToGlobalPhi(TTTrack_TrackWord::phi_t local_phi,
                                global_phi_t sector_shift);

  // Function to count stubs in hitpattern
  nstub_t countNStub(TTTrack_TrackWord::hit_t Hitpattern);

  // Function to take float phi to local integer phi
  TTTrack_TrackWord::phi_t floatGlobalPhiToSectorPhi(float phi,
                                                     unsigned int sector);

  std::vector<global_phi_t> generatePhiSliceLUT(unsigned int N);

  std::vector<global_phi_t> getPhiQuad() const { return phiQuadrants; }
  std::vector<global_phi_t> getPhiShift() const { return phiShift; }

  void setGTTinput(bool input) { GTTinput_ = input; }
  void setVtxEmulator(bool vtx) { VtxEmulator_ = vtx; }

 private:
  std::vector<global_phi_t> phiQuadrants;
  std::vector<global_phi_t> phiShift;

  bool GTTinput_ = false;
  bool VtxEmulator_ = false;
};

#endif