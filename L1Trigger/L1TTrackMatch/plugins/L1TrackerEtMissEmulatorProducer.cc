// -*- C++ -*-
//
// Package:    L1Trigger/L1TTrackMatch
// Class:      L1TrackerEtMissEmulatorProducer
//
/**\class L1TrackerEtMissEmulatorProducer L1TrackerEtMissEmulatorProducer.cc
 L1Trigger/L1TTrackMatch/plugins/L1TrackerEtMissEmulatorProducer.cc

 Description: Takes L1TTTracks and performs a integer emulation of Track-based
 missing Et, outputting a collection of EtSum 
*/
//
// Original Author:  Christopher Brown
//         Created:  Fri, 19 Feb 2021 
//
//

// system include files
#include <memory>
#include <numeric>
// user include files
#include "DataFormats/L1TrackTrigger/interface/TTTrack_TrackWord.h"
#include "DataFormats/L1TrackTrigger/interface/TTTypes.h"
#include "DataFormats/L1Trigger/interface/EtSum.h"
#include "DataFormats/L1Trigger/interface/Vertex.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/stream/EDProducer.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "L1Trigger/L1TTrackMatch/interface/Cordic.h"
#include "L1Trigger/L1TTrackMatch/interface/L1TkEtMissEmuAlgo.h"
#include "L1Trigger/L1TTrackMatch/interface/L1TkEtMissEmuTrackTransform.h"

using namespace l1t;

class L1TrackerEtMissEmulatorProducer : public edm::stream::EDProducer<> {
 public:
  typedef TTTrack<Ref_Phase2TrackerDigi_> L1TTTrackType;
  typedef std::vector<L1TTTrackType> L1TTTrackCollectionType;

  explicit L1TrackerEtMissEmulatorProducer(const edm::ParameterSet&);
  ~L1TrackerEtMissEmulatorProducer() override;

 private:
  virtual void beginJob();
  void produce(edm::Event&, const edm::EventSetup&) override;
  virtual void endJob();

  // ----------member data ---------------------------

  std::vector<L1TkEtMissEmuAlgo::global_phi_t> cosLUT_;  // Cos LUT array
  std::vector<L1TkEtMissEmuAlgo::eta_t>
      EtaRegionsLUT_;  // Various precomputed LUTs
  std::vector<TTTrack_TrackWord::z0_t> DeltaZLUT_;
  std::vector<L1TkEtMissEmuAlgo::global_phi_t> phiQuadrants_;
  std::vector<L1TkEtMissEmuAlgo::global_phi_t> phiShifts_;

  TTTrack_TrackWord::z0_t minZ0_;
  TTTrack_TrackWord::z0_t maxZ0_;
  L1TkEtMissEmuAlgo::eta_t maxEta_;
  TTTrack_TrackWord::chi2rphi_t chi2rphiMax_;
  TTTrack_TrackWord::chi2rz_t chi2rzMax_;
  TTTrack_TrackWord::bendChi2_t bendChi2Max_;
  L1TkEtMissEmuAlgo::pt_t minPt_;
  L1TkEtMissEmuAlgo::nstub_t nStubsmin_;

  int chi2Max_;

  TTTrack_TrackWord::z0_t deltaZ0_ = 0;

  int cordicSteps_;
  int debug_;
  bool cordicDebug_ = false;
  bool writeLUTs_ = false;

  bool GTTinput_ = false;
  bool VtxEmulator_ = false;

  L1TkEtMissEmuTrackTransform TrackTransform;

  std::string L1MetCollectionName_;

  const edm::EDGetTokenT<VertexCollection> pvToken_;
  const edm::EDGetTokenT<std::vector<TTTrack<Ref_Phase2TrackerDigi_>>>
      trackToken_;
};

// constructor//
L1TrackerEtMissEmulatorProducer::L1TrackerEtMissEmulatorProducer(
    const edm::ParameterSet& iConfig)
    : pvToken_(consumes<VertexCollection>(
          iConfig.getParameter<edm::InputTag>("L1VertexInputTag"))),
      trackToken_(consumes<std::vector<TTTrack<Ref_Phase2TrackerDigi_>>>(
          iConfig.getParameter<edm::InputTag>("L1TrackInputTag"))) {
  // Setup LUTs
  TrackTransform.generateLUTs();
  phiQuadrants_ = TrackTransform.getPhiQuad();
  phiShifts_ = TrackTransform.getPhiShift();

  // Get Emulator config parameters
  cordicSteps_ = (int)iConfig.getParameter<int>("nCordicSteps");
  debug_ = (int)iConfig.getParameter<int>("debug");
  writeLUTs_ = (bool)iConfig.getParameter<bool>("writeLUTs");

  GTTinput_ = (bool)iConfig.getParameter<bool>("useGTTinput");
  VtxEmulator_ = (bool)iConfig.getParameter<bool>("useVertexEmulator");

  TrackTransform.setGTTinput(GTTinput_);
  TrackTransform.setVtxEmulator(VtxEmulator_);

  // Name of output ED Product
  L1MetCollectionName_ =
      (std::string)iConfig.getParameter<std::string>("L1MetCollectionName");

  // Input parameter cuts and convert to correct integer representations
  maxZ0_ = L1TkEtMissEmuAlgo::digitizeSignedValue<TTTrack_TrackWord::z0_t>(
      (double)iConfig.getParameter<double>("maxZ0"),
      TTTrack_TrackWord::TrackBitWidths::kZ0Size, TTTrack_TrackWord::stepZ0);
  minZ0_ = (1 << TTTrack_TrackWord::TrackBitWidths::kZ0Size) - maxZ0_;

  maxEta_ = L1TkEtMissEmuAlgo::digitizeSignedValue<eta_t>(
      (double)iConfig.getParameter<double>("maxEta"),
      TTTrack_TrackWord::TrackBitWidths::kTanlSize, L1TkEtMissEmuAlgo::stepEta);

  chi2rphiMax_ = L1TkEtMissEmuAlgo::getBin(
      (double)iConfig.getParameter<double>("chi2rphidofMax"),
      TTTrack_TrackWord::chi2RPhiBins);
  chi2rzMax_ = L1TkEtMissEmuAlgo::getBin(
      (double)iConfig.getParameter<double>("chi2rzdofMax"),
      TTTrack_TrackWord::chi2RZBins);
  bendChi2Max_ = L1TkEtMissEmuAlgo::getBin(
      (double)iConfig.getParameter<double>("bendChi2Max"),
      TTTrack_TrackWord::bendChi2Bins);
  chi2Max_ = (int)iConfig.getParameter<int>("chi2Max");

  minPt_ = L1TkEtMissEmuAlgo::digitizeSignedValue<pt_t>(
      (double)iConfig.getParameter<double>("minPt"),
      TTTrack_TrackWord::TrackBitWidths::kRinvSize, L1TkEtMissEmuAlgo::stepPt);

  nStubsmin_ = (nstub_t)iConfig.getParameter<int>("nStubsmin");

  if (debug_ == 5) {
    cordicDebug_ = true;
  }

  // To have same bin spacing between 0 and pi/2 as between original phi
  // granularity
  int cosLUTbins = ceil((L1TkEtMissEmuAlgo::maxCosLUTPhi *
                         (L1TkEtMissEmuAlgo::kGlobalPhiBins - 1)) /
                        (2 * -TTTrack_TrackWord::minPhi0));

  // Compute LUTs
  cosLUT_ = L1TkEtMissEmuAlgo::generateCosLUT(cosLUTbins);
  EtaRegionsLUT_ = L1TkEtMissEmuAlgo::generateEtaRegionLUT();
  DeltaZLUT_ = L1TkEtMissEmuAlgo::generateDeltaZLUT();

  // Write Out LUTs
  if (writeLUTs_) {
    L1TkEtMissEmuAlgo::writeLUTtoFile<L1TkEtMissEmuAlgo::global_phi_t>(
        cosLUT_, "cos", ",");
    L1TkEtMissEmuAlgo::writeLUTtoFile<L1TkEtMissEmuAlgo::global_phi_t>(
        phiQuadrants_, "phiquadrants", ",");
    L1TkEtMissEmuAlgo::writeLUTtoFile<L1TkEtMissEmuAlgo::global_phi_t>(
        phiShifts_, "phishift", ",");
    L1TkEtMissEmuAlgo::writeLUTtoFile<L1TkEtMissEmuAlgo::eta_t>(
        EtaRegionsLUT_, "etaregions", ",");
    L1TkEtMissEmuAlgo::writeLUTtoFile<TTTrack_TrackWord::z0_t>(DeltaZLUT_,
                                                               "dzbins", ",");
    std::vector<unsigned int> cutlut = {
        (unsigned int)minPt_,       (unsigned int)minZ0_,
        (unsigned int)maxZ0_,       (unsigned int)maxEta_,
        (unsigned int)chi2rphiMax_, (unsigned int)chi2rzMax_,
        (unsigned int)bendChi2Max_, (unsigned int)nStubsmin_};
    L1TkEtMissEmuAlgo::writeLUTtoFile<unsigned int>(cutlut, "cuts", ",");
  }

  produces<std::vector<EtSum>>(L1MetCollectionName_);
}

L1TrackerEtMissEmulatorProducer::~L1TrackerEtMissEmulatorProducer() {}

void L1TrackerEtMissEmulatorProducer::produce(edm::Event& iEvent,
                                              const edm::EventSetup& iSetup) {
  using namespace edm;

  std::unique_ptr<std::vector<l1t::EtSum>> METCollection(
      new std::vector<l1t::EtSum>(0));

  edm::Handle<VertexCollection> L1VertexHandle;
  iEvent.getByToken(pvToken_, L1VertexHandle);

  edm::Handle<L1TTTrackCollectionType> L1TTTrackHandle;
  iEvent.getByToken(trackToken_, L1TTTrackHandle);

  // Initialize cordic class
  Cordic cordicSqrt(L1TkEtMissEmuAlgo::kMETPhiBins, L1TkEtMissEmuAlgo::kMETSize,
                    cordicSteps_, cordicDebug_, writeLUTs_);

  if (!L1VertexHandle.isValid()) {
    LogError("L1TrackerEtMissEmulatorProducer")
        << "\nWarning: VertexCollection not found in the event. Exit\n";
    return;
  }

  if (!L1TTTrackHandle.isValid()) {
    LogError("L1TrackerEtMissEmulatorProducer")
        << "\nWarning: L1TTTrackCollection not found in the event. Exit\n";
    return;
  }

  // Initialize sector sums, need 0 initialization in case a sector has no
  // tracks
  L1TkEtMissEmuAlgo::Et_t sumPx[L1TkEtMissEmuAlgo::NSector] = {0, 0, 0, 0, 0,
                                                               0, 0, 0, 0};
  L1TkEtMissEmuAlgo::Et_t sumPy[L1TkEtMissEmuAlgo::NSector] = {0, 0, 0, 0, 0,
                                                               0, 0, 0, 0};

  // Track counters
  int num_tracks{0};
  int num_assoc_tracks{0};
  int num_quality_tracks{0};

  // Get reference to first vertex in event vertex collection
  l1t::Vertex& vtx = const_cast<l1t::Vertex&>(L1VertexHandle->at(0));

  for (const auto& track : *L1TTTrackHandle) {
    num_tracks++;
    L1TTTrackType& track_ref = const_cast<L1TTTrackType&>(
        track);  // Get Reference to track to pass to TrackTransform

    // Convert to internal track representation
    InternalEtWord EtTrack = TrackTransform.transformTrack(track_ref, vtx);

    // Parameter cuts
    if (EtTrack.pt < minPt_) continue;

    // Z signed so double bound
    if (EtTrack.z0 & (1 << (TTTrack_TrackWord::TrackBitWidths::kZ0Size - 1))) { 
    // if negative
      if (EtTrack.z0 <= maxZ0_) continue;
    } else {
      if (EtTrack.z0 > minZ0_) continue;
    }
    if (EtTrack.eta > maxEta_) continue;

    // Quality Cuts
    if (EtTrack.chi2rphidof >= chi2rphiMax_) continue;

    if (EtTrack.chi2rzdof >= chi2rzMax_) continue;

    if (EtTrack.chi2rzdof + EtTrack.chi2rphidof >= chi2Max_) continue;

    if (EtTrack.bendChi2 >= bendChi2Max_) continue;

    if (EtTrack.nstubs < nStubsmin_) continue;

    num_quality_tracks++;
    // Temporary int representation to get the difference
    int tempz = L1TkEtMissEmuAlgo::unpackSignedValue(
        EtTrack.z0, TTTrack_TrackWord::TrackBitWidths::kZ0Size);
    int temppv = L1TkEtMissEmuAlgo::unpackSignedValue(
        EtTrack.pV, TTTrack_TrackWord::TrackBitWidths::kZ0Size);

    TTTrack_TrackWord::z0_t z_diff = abs(tempz - temppv);

    // Track to vertex association, adaptive z window based on eta region
    for (unsigned int reg = 0; reg < L1TkEtMissEmuAlgo::NEtaRegion; reg++) {
      if (EtTrack.eta >= EtaRegionsLUT_[reg] &&
          EtTrack.eta < EtaRegionsLUT_[reg + 1]) {
        deltaZ0_ = DeltaZLUT_[reg];
        break;
      }
    }

    if (z_diff <= deltaZ0_) {
      num_assoc_tracks++;

      if (debug_ == 2) {
        std::cout << "========================Phi "
                     "debug=================================\n";
        std::cout << "Int pT: " << EtTrack.pt << "\n";
        std::cout << "Int Phi: " << EtTrack.globalPhi
                  << " Float Phi: " << EtTrack.phi
                  << " Actual Float Cos(Phi): " << cos(EtTrack.phi)
                  << " Actual Float Sin(Phi): " << sin(EtTrack.phi) << "\n";
      }

      // Split tracks in phi quadrants and access cosLUT_, backwards iteration
      // through cosLUT_ gives sin Sum sector Et -ve when cos or sin phi are -ve
      if (EtTrack.globalPhi >= phiQuadrants_[0] &&
          EtTrack.globalPhi < phiQuadrants_[1]) {
        sumPx[EtTrack.Sector] =
            sumPx[EtTrack.Sector] + (EtTrack.pt * cosLUT_[EtTrack.globalPhi]) /
                                        L1TkEtMissEmuAlgo::kGlobalPhiBins;
        sumPy[EtTrack.Sector] =
            sumPy[EtTrack.Sector] +
            (EtTrack.pt * cosLUT_[phiQuadrants_[1] - 1 - EtTrack.globalPhi]) /
                L1TkEtMissEmuAlgo::kGlobalPhiBins;

        if (debug_ == 2) {
          std::cout << "Sector: " << EtTrack.Sector << " Quadrant: " << 1
                    << "\n";
          std::cout << "Int Phi: " << EtTrack.globalPhi
                    << " Int Cos(Phi): " << cosLUT_[EtTrack.globalPhi]
                    << " Int Sin(Phi): "
                    << cosLUT_[phiQuadrants_[1] - 1 - EtTrack.globalPhi]
                    << "\n";
          std::cout
              << "Float Phi: "
              << (float)EtTrack.globalPhi / L1TkEtMissEmuAlgo::kGlobalPhiBins
              << " Float Cos(Phi): "
              << (float)cosLUT_[EtTrack.globalPhi] /
                     L1TkEtMissEmuAlgo::kGlobalPhiBins
              << " Float Sin(Phi): "
              << (float)cosLUT_[phiQuadrants_[1] - 1 - EtTrack.globalPhi] /
                     L1TkEtMissEmuAlgo::kGlobalPhiBins
              << "\n";
        }
      } else if (EtTrack.globalPhi >= phiQuadrants_[1] &&
                 EtTrack.globalPhi < phiQuadrants_[2]) {
        sumPx[EtTrack.Sector] =
            sumPx[EtTrack.Sector] -
            (EtTrack.pt * cosLUT_[phiQuadrants_[2] - 1 - EtTrack.globalPhi]) /
                L1TkEtMissEmuAlgo::kGlobalPhiBins;
        sumPy[EtTrack.Sector] =
            sumPy[EtTrack.Sector] +
            (EtTrack.pt * cosLUT_[EtTrack.globalPhi - phiQuadrants_[1]]) /
                L1TkEtMissEmuAlgo::kGlobalPhiBins;

        if (debug_ == 2) {
          std::cout << "Sector: " << EtTrack.Sector << " Quadrant: " << 2
                    << "\n";
          std::cout << "Int Phi: " << EtTrack.globalPhi << " Int Cos(Phi): -"
                    << cosLUT_[phiQuadrants_[2] - EtTrack.globalPhi]
                    << " Int Sin(Phi): "
                    << cosLUT_[EtTrack.globalPhi - phiQuadrants_[1]] << "\n";
          std::cout
              << "Float Phi: "
              << (float)EtTrack.globalPhi / L1TkEtMissEmuAlgo::kGlobalPhiBins
              << " Float Cos(Phi): -"
              << (float)cosLUT_[phiQuadrants_[2] - 1 - EtTrack.globalPhi] /
                     L1TkEtMissEmuAlgo::kGlobalPhiBins
              << " Float Sin(Phi): "
              << (float)cosLUT_[EtTrack.globalPhi - phiQuadrants_[1]] /
                     L1TkEtMissEmuAlgo::kGlobalPhiBins
              << "\n";
        }
      } else if (EtTrack.globalPhi >= phiQuadrants_[2] &&
                 EtTrack.globalPhi < phiQuadrants_[3]) {
        sumPx[EtTrack.Sector] =
            sumPx[EtTrack.Sector] -
            (EtTrack.pt * cosLUT_[EtTrack.globalPhi - phiQuadrants_[2]]) /
                L1TkEtMissEmuAlgo::kGlobalPhiBins;
        sumPy[EtTrack.Sector] =
            sumPy[EtTrack.Sector] -
            (EtTrack.pt * cosLUT_[phiQuadrants_[3] - 1 - EtTrack.globalPhi]) /
                L1TkEtMissEmuAlgo::kGlobalPhiBins;

        if (debug_ == 2) {
          std::cout << "Sector: " << EtTrack.Sector << " Quadrant: " << 3
                    << "\n";
          std::cout << "Int Phi: " << EtTrack.globalPhi << " Int Cos(Phi): -"
                    << cosLUT_[EtTrack.globalPhi - phiQuadrants_[2]]
                    << " Int Sin(Phi): -"
                    << cosLUT_[phiQuadrants_[3] - 1 - EtTrack.globalPhi]
                    << "\n";
          std::cout
              << "Float Phi: "
              << (float)EtTrack.globalPhi / L1TkEtMissEmuAlgo::kGlobalPhiBins
              << " Float Cos(Phi): -"
              << (float)cosLUT_[EtTrack.globalPhi - phiQuadrants_[2]] /
                     L1TkEtMissEmuAlgo::kGlobalPhiBins
              << " Float Sin(Phi): -"
              << (float)cosLUT_[phiQuadrants_[3] - 1 - EtTrack.globalPhi] /
                     L1TkEtMissEmuAlgo::kGlobalPhiBins
              << "\n";
        }

      } else if (EtTrack.globalPhi >= phiQuadrants_[3] &&
                 EtTrack.globalPhi < phiQuadrants_[4]) {
        sumPx[EtTrack.Sector] =
            sumPx[EtTrack.Sector] +
            (EtTrack.pt * cosLUT_[phiQuadrants_[4] - 1 - EtTrack.globalPhi]) /
                L1TkEtMissEmuAlgo::kGlobalPhiBins;
        sumPy[EtTrack.Sector] =
            sumPy[EtTrack.Sector] -
            (EtTrack.pt * cosLUT_[EtTrack.globalPhi - phiQuadrants_[3]]) /
                L1TkEtMissEmuAlgo::kGlobalPhiBins;

        if (debug_ == 2) {
          std::cout << "Sector: " << EtTrack.Sector << " Quadrant: " << 4
                    << "\n";
          std::cout << "Int Phi: " << EtTrack.globalPhi << " Int Cos(Phi): "
                    << cosLUT_[phiQuadrants_[4] - 1 - EtTrack.globalPhi]
                    << " Int Sin(Phi): -"
                    << cosLUT_[EtTrack.globalPhi - phiQuadrants_[3]] << "\n";
          std::cout
              << "Float Phi: "
              << (float)EtTrack.globalPhi / L1TkEtMissEmuAlgo::kGlobalPhiBins
              << " Float Cos(Phi): "
              << (float)cosLUT_[phiQuadrants_[4] - 1 - EtTrack.globalPhi] /
                     L1TkEtMissEmuAlgo::kGlobalPhiBins
              << " Float Sin(Phi): -"
              << (float)cosLUT_[EtTrack.globalPhi - phiQuadrants_[3]] /
                     L1TkEtMissEmuAlgo::kGlobalPhiBins
              << "\n";
        }
      }
    }

  }  // end loop over tracks

  L1TkEtMissEmuAlgo::Et_t GlobalPx = 0;
  L1TkEtMissEmuAlgo::Et_t GlobalPy = 0;

  // Global Et sum
  for (unsigned int i = 0; i < L1TkEtMissEmuAlgo::NSector; i++) {
    GlobalPx = GlobalPx + sumPx[i];
    GlobalPy = GlobalPy + sumPy[i];
  }

  // Perform cordic sqrt, take x,y and converts to polar coordinate r,phi where
  // r=sqrt(x**2+y**2) and phi = atan(y/x)
  L1TkEtMissEmuAlgo::EtMiss EtMiss = cordicSqrt.toPolar(GlobalPx, GlobalPy);

  // Recentre phi
  L1TkEtMissEmuAlgo::METphi_t tempPhi = 0;

  if ((GlobalPx < 0) && (GlobalPy < 0))
    tempPhi = EtMiss.Phi - L1TkEtMissEmuAlgo::kMETPhiBins / 2;
  else if ((GlobalPx >= 0) && (GlobalPy >= 0))
    tempPhi = (EtMiss.Phi) + L1TkEtMissEmuAlgo::kMETPhiBins / 2;
  else if ((GlobalPx >= 0) && (GlobalPy < 0))
    tempPhi = EtMiss.Phi - L1TkEtMissEmuAlgo::kMETPhiBins / 2;
  else if ((GlobalPx < 0) && (GlobalPy >= 0))
    tempPhi = -EtMiss.Phi + 3 * L1TkEtMissEmuAlgo::kMETPhiBins / 2;

  if (debug_ == 6) {
    std::cout << "====Sector Pt====\n";
    std::cout << "Px: ";
    for (auto i : sumPx) {
      std::cout << i * L1TkEtMissEmuAlgo::stepMET << "|";
    }
    std::cout << "\nPy: ";
    for (auto i : sumPy) {
      std::cout << i * L1TkEtMissEmuAlgo::stepMET << "|";
    }
    std::cout << "\n";

    std::cout << "====Global Pt====\n";
    std::cout << "Integer Global Px: " << GlobalPx
              << "| Integer Global Py: " << GlobalPy << "\n";
    std::cout << "Float Global Px: " << GlobalPx * L1TkEtMissEmuAlgo::stepMET
              << "| Float Global Py: " << GlobalPy * L1TkEtMissEmuAlgo::stepMET
              << "\n";
  }

  if (debug_ == 6) {
    std::cout << "====MET===\n";
    std::cout << "Integer MET: " << EtMiss.Et
              << "| Integer MET phi: " << EtMiss.Phi << "\n";
    std::cout << "Float MET: " << (EtMiss.Et) * L1TkEtMissEmuAlgo::stepMET
              << "| Float MET phi: "
              << (float)tempPhi * L1TkEtMissEmuAlgo::stepMETPhi - M_PI << "\n";

    std::cout << "# Intial Tracks: " << num_tracks << "\n";
    std::cout << "# Tracks after Quality Cuts: " << num_quality_tracks << "\n";
    std::cout << "# Tracks Associated to Vertex: " << num_assoc_tracks << "\n";
    std::cout << "========================================================\n";
  }

  math::XYZTLorentzVector missingEt(-GlobalPx, -GlobalPy, 0, EtMiss.Et);
  EtSum L1EtSum(missingEt, EtSum::EtSumType::kMissingEt, (int)EtMiss.Et, 0,
                (int)tempPhi, (int)num_assoc_tracks);

  METCollection->push_back(L1EtSum);

  iEvent.put(std::move(METCollection), L1MetCollectionName_);
}  // end producer

void L1TrackerEtMissEmulatorProducer::beginJob() {}

void L1TrackerEtMissEmulatorProducer::endJob() {}

// define this as a plug-in

DEFINE_FWK_MODULE(L1TrackerEtMissEmulatorProducer);
