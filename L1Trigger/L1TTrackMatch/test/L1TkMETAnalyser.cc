#include <cmath>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/L1TCorrelator/interface/TkEtMiss.h"
#include "DataFormats/L1TCorrelator/interface/TkEtMissFwd.h"
#include "DataFormats/L1TrackTrigger/interface/TTTypes.h"
#include "DataFormats/L1Trigger/interface/EtSum.h"
#include "DataFormats/L1Trigger/interface/Vertex.h"
#include "DataFormats/Phase2TrackerDigi/interface/Phase2TrackerDigi.h"
#include "DataFormats/TrackerCommon/interface/TrackerTopology.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
#include "L1Trigger/L1TTrackMatch/interface/L1TkEtMissEmuAlgo.h"
#include "TEfficiency.h"
#include "TGraphAsymmErrors.h"
#include "TGraphErrors.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TPad.h"
#include "TProfile.h"

using namespace std;

class L1TkMETAnalyser : public edm::EDAnalyzer {
 public:
  explicit L1TkMETAnalyser(const edm::ParameterSet& iConfig);
  ~L1TkMETAnalyser() override;

 private:
  void beginJob() override;
  void analyze(const edm::Event& iEvent,
               const edm::EventSetup& iSetup) override;
  void endJob() override;

  edm::ParameterSet config;

  edm::InputTag TrackMETSimInputTag;
  edm::InputTag TrackMETEmuInputTag;
  edm::InputTag TrackMETHWInputTag;

  edm::EDGetTokenT<std::vector<l1t::TkEtMiss>> TrackMETSimToken_;
  edm::EDGetTokenT<std::vector<l1t::EtSum>> TrackMETEmuToken_;
  edm::EDGetTokenT<std::vector<l1t::EtSum>> TrackMETHWToken_;

  float EtScale;
  float EtphiScale;

  bool available_;

  edm::Service<TFileService> fs_;

  bool HW_analysis_;

  TH1F* hisTkSimMET_;
  TH1F* hisTkEmuMET_;
  TH1F* hisTkHWMET_;

  TH1F* hisTkSimPhi_;
  TH1F* hisTkEmuPhi_;
  TH1F* hisTkHWPhi_;

  TH1F* hisTkSimNtrk_;
  TH1F* hisTkEmuNtrk_;
  TH1F* hisTkHWNtrk_;

  TH1F* hisMETResidual_;
  TH1F* hisPhiResidual_;
  TH1F* hisNtrkResidual_;
};

L1TkMETAnalyser::L1TkMETAnalyser(edm::ParameterSet const& iConfig)
    : config(iConfig) {
  HW_analysis_ = iConfig.getParameter<bool>("HW_Analysis");

  TrackMETSimInputTag = iConfig.getParameter<edm::InputTag>("TrackMETInputTag");
  TrackMETEmuInputTag =
      iConfig.getParameter<edm::InputTag>("TrackMETEmuInputTag");
  if (HW_analysis_) {
    TrackMETHWInputTag =
        iConfig.getParameter<edm::InputTag>("TrackMETHWInputTag");
  }
  TrackMETSimToken_ = consumes<std::vector<l1t::TkEtMiss>>(TrackMETSimInputTag);
  TrackMETEmuToken_ = consumes<std::vector<l1t::EtSum>>(TrackMETEmuInputTag);
  if (HW_analysis_) {
    TrackMETHWToken_ = consumes<std::vector<l1t::EtSum>>(TrackMETHWInputTag);
  }
}

/////////////
// DESTRUCTOR
L1TkMETAnalyser::~L1TkMETAnalyser() {}

void L1TkMETAnalyser::beginJob() {
  TFileDirectory inputDir = fs_->mkdir("TkMETAnalysis");
  available_ = fs_.isAvailable();
  if (not available_) return;  // No ROOT file open.

  hisTkSimMET_ =
      inputDir.make<TH1F>("hisTkSimMET_", "sim TkMET [GeV]", 101, 0, 500);
  hisTkEmuMET_ =
      inputDir.make<TH1F>("hisTkEmuMET_", "emu TkMET [GeV]", 101, 0, 500);

  hisTkSimPhi_ =
      inputDir.make<TH1F>("hisTkSimPhi_", "sim phi [rad]", 101, -M_PI, M_PI);
  hisTkEmuPhi_ =
      inputDir.make<TH1F>("hisTkEmuPhi_", "emu phi [rad]", 101, -M_PI, M_PI);

  hisTkSimNtrk_ =
      inputDir.make<TH1F>("hisTkSimNtrk_", "sim ntrks", 101, 0, 256);
  hisTkEmuNtrk_ =
      inputDir.make<TH1F>("hisTkEmuNtrk_", "emu ntrks", 101, 0, 256);

  if (!HW_analysis_) {
    hisMETResidual_ = inputDir.make<TH1F>(
        "hisMETResidual_", "sim - emu TkMET [GeV]", 101, -100, 100);
    hisPhiResidual_ = inputDir.make<TH1F>("hisPhiResidual_",
                                          "sim - emu phi [rad]", 101, -1, 1);
    hisNtrkResidual_ = inputDir.make<TH1F>("hisNtrkResidual_",
                                           "sim - emu ntrks", 101, -10, 10);
  }

  if (HW_analysis_) {
    hisTkHWMET_ =
        inputDir.make<TH1F>("hisTkHWMET_", "hw TkMET [GeV]", 101, 0, 500);
    hisTkHWPhi_ =
        inputDir.make<TH1F>("hisTkHWPhi_", "hw phi [rad]", 101, -M_PI, M_PI);
    hisTkHWNtrk_ =
        inputDir.make<TH1F>("hisTkEmuNtrk_", "hw ntrks", 101, 0, 256);

    hisMETResidual_ = inputDir.make<TH1F>(
        "hisMETResidual_", "emu - hw TkMET [GeV]", 101, -100, 100);
    hisPhiResidual_ = inputDir.make<TH1F>("hisPhiResidual_",
                                          "emu - hw phi [rad]", 101, -1, 1);
    hisNtrkResidual_ =
        inputDir.make<TH1F>("hisNtrkResidual_", "emu - hw ntrks", 101, -10, 10);
  }
}

void L1TkMETAnalyser::analyze(const edm::Event& iEvent,
                              const edm::EventSetup& iSetup) {
  if (not available_) return;  // No ROOT file open.

  edm::Handle<std::vector<l1t::TkEtMiss>> L1TkMETSimHandle;
  edm::Handle<std::vector<l1t::EtSum>> L1TkMETEmuHandle;

  iEvent.getByToken(TrackMETSimToken_, L1TkMETSimHandle);
  iEvent.getByToken(TrackMETEmuToken_, L1TkMETEmuHandle);

  float SimEtmiss = L1TkMETSimHandle->begin()->etMiss();
  float EmuEtmiss =
      L1TkMETEmuHandle->begin()->hwPt() * L1TkEtMissEmuAlgo::stepMET;

  float SimEtPhi = L1TkMETSimHandle->begin()->etPhi();
  float EmuEtPhi =
      L1TkMETEmuHandle->begin()->hwPhi() * L1TkEtMissEmuAlgo::stepMETPhi - M_PI;

  int SimEtNtrk = L1TkMETSimHandle->begin()->etQual();
  int EmuEtNtrk = L1TkMETEmuHandle->begin()->hwQual();

  if (!HW_analysis_) {
    hisMETResidual_->Fill(EmuEtmiss - SimEtmiss);
    hisPhiResidual_->Fill(EmuEtPhi - SimEtPhi);
    hisNtrkResidual_->Fill(EmuEtNtrk - SimEtNtrk);
  }

  hisTkSimMET_->Fill(SimEtmiss);
  hisTkEmuMET_->Fill(EmuEtmiss);

  hisTkSimPhi_->Fill(SimEtPhi);
  hisTkEmuPhi_->Fill(EmuEtPhi);

  hisTkSimNtrk_->Fill(SimEtNtrk);
  hisTkEmuNtrk_->Fill(EmuEtNtrk);

  if (HW_analysis_) {
    edm::Handle<std::vector<l1t::EtSum>> L1TkMETHWHandle;
    iEvent.getByToken(TrackMETHWToken_, L1TkMETHWHandle);
    float HWEtmiss = L1TkMETHWHandle->begin()->hwPt();
    float HWEtPhi = L1TkMETHWHandle->begin()->hwPhi();
    int HWEtNtrk = L1TkMETHWHandle->begin()->hwQual();

    hisTkHWMET_->Fill(HWEtmiss);
    hisTkHWPhi_->Fill(HWEtPhi);
    hisTkHWNtrk_->Fill(HWEtNtrk);

    hisMETResidual_->Fill(EmuEtmiss - HWEtmiss);
    hisPhiResidual_->Fill(EmuEtPhi - HWEtPhi);
    hisNtrkResidual_->Fill(EmuEtNtrk - HWEtNtrk);
  }
}

//////////
// END JOB
void L1TkMETAnalyser::endJob() {
  // things to be done at the exit of the event Loop
  // edm::LogInfo("L1TkMETAnalyser")
  std::cout << "analyzer::==================== TkMET RECONSTRUCTION "
               "======================\n"
            << "MET Residual Bias: " << hisMETResidual_->GetMean() << " GeV\n"
            << "MET Resolution: " << hisMETResidual_->GetStdDev() << " GeV\n"
            << "Phi Residual Bias: " << hisPhiResidual_->GetMean() << " rad\n"
            << "Phi Resolution: " << hisPhiResidual_->GetStdDev() << " rad\n"
            << "NTrk Residual Bias: " << hisNtrkResidual_->GetMean()
            << " Tracks\n"
            << "Ntrk Resolution: " << hisNtrkResidual_->GetStdDev()
            << " Tracks\n";
}

///////////////////////////
// DEFINE THIS AS A PLUG-IN
DEFINE_FWK_MODULE(L1TkMETAnalyser);