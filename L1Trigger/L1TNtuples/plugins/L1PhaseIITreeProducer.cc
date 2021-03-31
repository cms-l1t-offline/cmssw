// -*- C++ -*-
//
// Package:    UserCode/L1TriggerDPG
// Class:      L1PhaseIITreeProducer
//
/**\class L1PhaseIITreeProducer L1PhaseIITreeProducer.cc UserCode/L1TriggerDPG/src/L1PhaseIITreeProducer.cc

Description: Produce L1 Extra tree

Implementation:

*/
//
// Original Author:  Alex Tapper
//         Created:
// $Id: L1PhaseIITreeProducer.cc,v 1.5 2013/01/06 21:55:55 jbrooke Exp $
//
//

// system include files
#include <memory>

// framework
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

// data formats
#include "DataFormats/L1Trigger/interface/L1EmParticleFwd.h"
#include "DataFormats/L1Trigger/interface/L1EmParticle.h"
#include "DataFormats/L1Trigger/interface/L1JetParticleFwd.h"
#include "DataFormats/L1Trigger/interface/L1JetParticle.h"
#include "DataFormats/L1Trigger/interface/L1MuonParticleFwd.h"
#include "DataFormats/L1Trigger/interface/L1MuonParticle.h"
#include "DataFormats/L1Trigger/interface/L1EtMissParticleFwd.h"
#include "DataFormats/L1Trigger/interface/L1EtMissParticle.h"
#include "DataFormats/L1TCorrelator/interface/TkMuon.h"
#include "DataFormats/L1TCorrelator/interface/TkMuonFwd.h"

#include "DataFormats/L1TCorrelator/interface/TkGlbMuon.h"
#include "DataFormats/L1TCorrelator/interface/TkGlbMuonFwd.h"
#include "DataFormats/L1TCorrelator/interface/TkPrimaryVertex.h"
#include "DataFormats/L1TCorrelator/interface/TkEtMiss.h"
#include "DataFormats/L1TCorrelator/interface/TkEtMissFwd.h"
#include "DataFormats/L1TCorrelator/interface/TkEm.h"
#include "DataFormats/L1TCorrelator/interface/TkEmFwd.h"
#include "DataFormats/L1TCorrelator/interface/TkElectron.h"
#include "DataFormats/L1TCorrelator/interface/TkElectronFwd.h"
#include "DataFormats/L1TCorrelator/interface/TkJet.h"
#include "DataFormats/L1TCorrelator/interface/TkJetFwd.h"
#include "DataFormats/L1TCorrelator/interface/TkHTMiss.h"
#include "DataFormats/L1TCorrelator/interface/TkHTMissFwd.h"

#include "DataFormats/L1TCorrelator/interface/TkTau.h"
#include "DataFormats/L1TCorrelator/interface/TkTauFwd.h"

#include "DataFormats/L1TCorrelator/interface/L1TrkTau.h"
#include "DataFormats/L1TCorrelator/interface/TkEGTau.h"
#include "DataFormats/L1TCorrelator/interface/L1CaloTkTau.h"

#include "DataFormats/L1Trigger/interface/EGamma.h"
#include "DataFormats/L1Trigger/interface/Tau.h"
#include "DataFormats/L1Trigger/interface/Jet.h"
#include "DataFormats/L1Trigger/interface/Muon.h"
#include "DataFormats/L1Trigger/interface/EtSum.h"
//#include "DataFormats/L1TVertex/interface/Vertex.h"

//#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/L1TParticleFlow/interface/PFJet.h"

//#include "DataFormats/L1Trigger/interface/PFTau.h"
#include "DataFormats/L1TParticleFlow/interface/PFCandidate.h"

#include "DataFormats/L1TParticleFlow/interface/PFTau.h"

//#include "DataFormats/Phase2L1Taus/interface/L1HPSPFTau.h"
//#include "DataFormats/Phase2L1Taus/interface/L1HPSPFTauFwd.h"

#include "DataFormats/L1TCorrelator/interface/TkBsCandidate.h"
#include "DataFormats/L1TCorrelator/interface/TkBsCandidateFwd.h"

#include "DataFormats/JetReco/interface/CaloJet.h"

//#include "DataFormats/L1TMuon/interface/BayesMuCorrelatorTrack.h"

// ROOT output stuff
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "TTree.h"

#include "L1Trigger/L1TNtuples/interface/L1AnalysisPhaseII.h"
#include "L1Trigger/L1TNtuples/interface/L1AnalysisPhaseIIDataFormat.h"

//
// class declaration
//

class L1PhaseIITreeProducer : public edm::EDAnalyzer {
public:
  explicit L1PhaseIITreeProducer(const edm::ParameterSet&);
  ~L1PhaseIITreeProducer() override;

private:
  void beginJob(void) override;
  void analyze(const edm::Event&, const edm::EventSetup&) override;
  void endJob() override;

public:
  L1Analysis::L1AnalysisPhaseII* l1Extra;
  L1Analysis::L1AnalysisPhaseIIDataFormat* l1ExtraData;

private:
  unsigned maxL1Extra_;

  // output file
  edm::Service<TFileService> fs_;

  // tree
  TTree* tree_;

  edm::EDGetTokenT<l1t::MuonBxCollection> muonToken_;

  edm::EDGetTokenT<l1t::JetBxCollection> caloJetToken_;

  edm::EDGetTokenT<l1t::EGammaBxCollection> egToken_;
  edm::EDGetTokenT<l1t::TkElectronCollection> tkEGToken_;
  //edm::EDGetTokenT<l1t::TkElectronCollection>  tkEGV2Token_;
  edm::EDGetTokenT<l1t::TkEmCollection> tkEMToken_;

  edm::EDGetTokenT<l1t::EGammaBxCollection> egTokenHGC_;
  edm::EDGetTokenT<l1t::TkElectronCollection> tkEGTokenHGC_;
  //edm::EDGetTokenT<l1t::TkElectronCollection>  tkEGV2TokenHGC_;
  edm::EDGetTokenT<l1t::TkEmCollection> tkEMTokenHGC_;

  edm::EDGetTokenT<l1t::TkMuonCollection> TkMuonToken_;
  edm::EDGetTokenT<l1t::TkGlbMuonCollection> TkGlbMuonToken_;

  //      edm::EDGetTokenT<l1t::TkMuonCollection> TkMuonStubsTokenBMTF_;
  //      edm::EDGetTokenT<l1t::TkMuonCollection> TkMuonStubsTokenEMTF_;
  //      edm::EDGetTokenT<l1t::BayesMuCorrTrackBxCollection> TkMuonStubsTokenOMTF_; // and yet another class... why not?
  //      edm::EDGetTokenT<l1t::TkMuonCollection> TkMuonStubsTokenME0_;
  //       edm::EDGetTokenT<l1t::BayesMuCorrTrackBxCollection> TkMuonStubsTokenHSCP_; // and yet another class... why not?

  edm::EDGetTokenT<l1t::TkJetCollection> tkTrackerJetToken_;
  edm::EDGetTokenT<l1t::TkEtMissCollection> tkMetToken_;

  edm::EDGetTokenT<l1t::L1TrkTauCollection> tkTauToken_;
  edm::EDGetTokenT<l1t::TkEGTauCollection> tkEGTauToken_;
  edm::EDGetTokenT<l1t::L1CaloTkTauCollection> caloTkTauToken_;

  std::vector<edm::EDGetTokenT<l1t::TkHTMissCollection>> tkMhtToken_;

  edm::EDGetTokenT<l1t::TkJetCollection> tkCaloJetToken_;
  edm::EDGetTokenT<l1t::TauBxCollection> caloTauToken_;
  edm::EDGetTokenT<float> caloJetHTTToken_;

  edm::EDGetTokenT<std::vector<l1t::PFJet>> ak4L1PF_;
  //                edm::EDGetTokenT<std::vector<l1t::PFJet>> ak4L1PFForMET_;

  edm::EDGetTokenT<l1t::RegionalMuonCandBxCollection> muonKalman_;
  edm::EDGetTokenT<l1t::RegionalMuonCandBxCollection> muonOverlap_;
  edm::EDGetTokenT<l1t::RegionalMuonCandBxCollection> muonEndcap_;

  edm::EDGetTokenT<std::vector<reco::PFMET>> l1PFMet_;

  //edm::EDGetTokenT<std::vector<reco::CaloJet> > l1pfPhase1L1TJetToken_; // why are these caloJets???

  edm::EDGetTokenT<float> z0PuppiToken_;
  //edm::EDGetTokenT<l1t::VertexCollection> l1vertextdrToken_;
  //edm::EDGetTokenT<l1t::VertexCollection> l1verticesToken_;
  edm::EDGetTokenT<l1t::TkPrimaryVertexCollection> l1TkPrimaryVertexToken_;

  //edm::EDGetTokenT<l1t::PFTauCollection> PFTauToken_;
  edm::EDGetTokenT<std::vector<l1t::PFCandidate>> l1PFCandidates_;

  edm::EDGetTokenT<l1t::PFTauCollection> L1NNTauToken_;
  edm::EDGetTokenT<l1t::PFTauCollection> L1NNTauPFToken_;

  //edm::EDGetTokenT<l1t::L1HPSPFTauCollection> L1HPSPFTauToken_;

  edm::EDGetTokenT<l1t::TkBsCandidateCollection> L1TkBsCandToken_;
  edm::EDGetTokenT<l1t::TkBsCandidateCollection> L1TkBsCandLooseToken_;
  edm::EDGetTokenT<l1t::TkBsCandidateCollection> L1TkBsCandTightToken_;
};

L1PhaseIITreeProducer::L1PhaseIITreeProducer(const edm::ParameterSet& iConfig) {
  muonToken_ = consumes<l1t::MuonBxCollection>(iConfig.getUntrackedParameter<edm::InputTag>("muonToken"));

  caloJetToken_ = consumes<l1t::JetBxCollection>(iConfig.getParameter<edm::InputTag>("caloJetToken"));
  caloTauToken_ = consumes<l1t::TauBxCollection>(iConfig.getParameter<edm::InputTag>("caloTauToken"));
  caloJetHTTToken_ = consumes<float>(iConfig.getParameter<edm::InputTag>("caloJetHTTToken"));

  egToken_ = consumes<l1t::EGammaBxCollection>(iConfig.getParameter<edm::InputTag>("egTokenBarrel"));
  egTokenHGC_ = consumes<l1t::EGammaBxCollection>(iConfig.getParameter<edm::InputTag>("egTokenHGC"));

  tkEGToken_ = consumes<l1t::TkElectronCollection>(iConfig.getParameter<edm::InputTag>("tkEGTokenBarrel"));
  //tkEGV2Token_ = consumes<l1t::TkElectronCollection>(iConfig.getParameter<edm::InputTag>("tkEGV2TokenBarrel"));
  tkEMToken_ = consumes<l1t::TkEmCollection>(iConfig.getParameter<edm::InputTag>("tkEMTokenBarrel"));

  tkEGTokenHGC_ = consumes<l1t::TkElectronCollection>(iConfig.getParameter<edm::InputTag>("tkEGTokenHGC"));
  //tkEGV2TokenHGC_ = consumes<l1t::TkElectronCollection>(iConfig.getParameter<edm::InputTag>("tkEGV2TokenHGC"));
  tkEMTokenHGC_ = consumes<l1t::TkEmCollection>(iConfig.getParameter<edm::InputTag>("tkEMTokenHGC"));

  TkMuonToken_ = consumes<l1t::TkMuonCollection>(iConfig.getParameter<edm::InputTag>("TkMuonToken"));

  TkGlbMuonToken_ = consumes<l1t::TkGlbMuonCollection>(iConfig.getParameter<edm::InputTag>("TkGlbMuonToken"));

  //TkMuonStubsTokenBMTF_ = consumes<l1t::TkMuonCollection>(iConfig.getParameter<edm::InputTag>("TkMuonStubsTokenBMTF"));
  //TkMuonStubsTokenEMTF_ = consumes<l1t::TkMuonCollection>(iConfig.getParameter<edm::InputTag>("TkMuonStubsTokenEMTF"));
  //TkMuonStubsTokenOMTF_ = consumes<l1t::BayesMuCorrTrackBxCollection>(iConfig.getParameter<edm::InputTag>("TkMuonStubsTokenOMTF"));
  //TkMuonStubsTokenME0_ = consumes<l1t::TkMuonCollection>(iConfig.getParameter<edm::InputTag>("TkMuonStubsTokenME0"));

  //TkMuonStubsTokenHSCP_ = consumes<l1t::BayesMuCorrTrackBxCollection>(iConfig.getParameter<edm::InputTag>("TkMuonStubsTokenHSCP"));

  tkTauToken_ = consumes<l1t::L1TrkTauCollection>(iConfig.getParameter<edm::InputTag>("tkTauToken"));
  caloTkTauToken_ = consumes<l1t::L1CaloTkTauCollection>(iConfig.getParameter<edm::InputTag>("caloTkTauToken"));
  tkEGTauToken_ = consumes<l1t::TkEGTauCollection>(iConfig.getParameter<edm::InputTag>("tkEGTauToken"));

  tkTrackerJetToken_ = consumes<l1t::TkJetCollection>(iConfig.getParameter<edm::InputTag>("tkTrackerJetToken"));
  tkMetToken_ = consumes<l1t::TkEtMissCollection>(iConfig.getParameter<edm::InputTag>("tkMetToken"));
  //tkMhtToken_ = consumes<l1t::TkHTMissCollection>(iConfig.getParameter<edm::InputTag>("tkMhtToken"));

  const auto& mhttokens = iConfig.getParameter<std::vector<edm::InputTag>>("tkMhtTokens");
  for (const auto& mhttoken : mhttokens) {
    tkMhtToken_.push_back(consumes<l1t::TkHTMissCollection>(mhttoken));
  }

  tkCaloJetToken_ = consumes<l1t::TkJetCollection>(iConfig.getParameter<edm::InputTag>("tkCaloJetToken"));

  ak4L1PF_ = consumes<std::vector<l1t::PFJet>>(iConfig.getParameter<edm::InputTag>("ak4L1PF"));

  //l1pfPhase1L1TJetToken_ = consumes<std::vector<reco::CaloJet> > (iConfig.getParameter<edm::InputTag>("l1pfPhase1L1TJetToken"));

  muonKalman_ = consumes<l1t::RegionalMuonCandBxCollection>(iConfig.getParameter<edm::InputTag>("muonKalman"));
  muonOverlap_ = consumes<l1t::RegionalMuonCandBxCollection>(iConfig.getParameter<edm::InputTag>("muonOverlap"));
  muonEndcap_ = consumes<l1t::RegionalMuonCandBxCollection>(iConfig.getParameter<edm::InputTag>("muonEndcap"));

  l1PFMet_ = consumes<std::vector<reco::PFMET>>(iConfig.getParameter<edm::InputTag>("l1PFMet"));

  z0PuppiToken_ = consumes<float>(iConfig.getParameter<edm::InputTag>("zoPuppi"));
  //l1vertextdrToken_ = consumes< l1t::VertexCollection> (iConfig.getParameter<edm::InputTag>("l1vertextdr"));
  //l1verticesToken_  = consumes< l1t::VertexCollection> (iConfig.getParameter<edm::InputTag>("l1vertices"));
  l1TkPrimaryVertexToken_ =
      consumes<l1t::TkPrimaryVertexCollection>(iConfig.getParameter<edm::InputTag>("l1TkPrimaryVertex"));

  l1PFCandidates_ = consumes<std::vector<l1t::PFCandidate>>(iConfig.getParameter<edm::InputTag>("l1PFCandidates"));
  //PFTauToken_ = consumes<l1t::PFTauCollection>(iConfig.getParameter<edm::InputTag>("PFTauToken"));

  L1NNTauToken_ = consumes<l1t::PFTauCollection>(iConfig.getParameter<edm::InputTag>("L1NNTauToken"));
  L1NNTauPFToken_ = consumes<l1t::PFTauCollection>(iConfig.getParameter<edm::InputTag>("L1NNTauPFToken"));

  //L1HPSPFTauToken_ = consumes<l1t::L1HPSPFTauCollection>(iConfig.getParameter<edm::InputTag>("L1HPSPFTauToken"));

  L1TkBsCandToken_ = consumes<l1t::TkBsCandidateCollection>(iConfig.getParameter<edm::InputTag>("L1TkBsCandsToken"));
  L1TkBsCandLooseToken_ =
      consumes<l1t::TkBsCandidateCollection>(iConfig.getParameter<edm::InputTag>("L1TkBsCandsLooseToken"));
  L1TkBsCandTightToken_ =
      consumes<l1t::TkBsCandidateCollection>(iConfig.getParameter<edm::InputTag>("L1TkBsCandsTightToken"));

  maxL1Extra_ = iConfig.getParameter<unsigned int>("maxL1Extra");

  l1Extra = new L1Analysis::L1AnalysisPhaseII();
  l1ExtraData = l1Extra->getData();

  // set up output
  tree_ = fs_->make<TTree>("L1PhaseIITree", "L1PhaseIITree");
  tree_->Branch("L1PhaseII", "L1Analysis::L1AnalysisPhaseIIDataFormat", &l1ExtraData, 32000, 3);
}

L1PhaseIITreeProducer::~L1PhaseIITreeProducer() {
  // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)
}

//
// member functions
//

// ------------ method called to for each event  ------------
void L1PhaseIITreeProducer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {
  l1Extra->Reset();

  edm::Handle<l1t::MuonBxCollection> muon;
  edm::Handle<l1t::TkGlbMuonCollection> TkGlbMuon;
  edm::Handle<l1t::TkMuonCollection> TkMuon;
  edm::Handle<l1t::TkMuonCollection> TkMuonStubsBMTF;
  edm::Handle<l1t::TkMuonCollection> TkMuonStubsEMTF;
  edm::Handle<l1t::TkMuonCollection> TkMuonStubsME0;

  //edm::Handle<l1t::BayesMuCorrTrackBxCollection> TkMuonStubsOMTF;

  iEvent.getByToken(muonToken_, muon);
  iEvent.getByToken(TkGlbMuonToken_, TkGlbMuon);
  iEvent.getByToken(TkMuonToken_, TkMuon);
  // iEvent.getByToken(TkMuonStubsTokenBMTF_,TkMuonStubsBMTF);
  // iEvent.getByToken(TkMuonStubsTokenEMTF_,TkMuonStubsEMTF);
  // iEvent.getByToken(TkMuonStubsTokenME0_,TkMuonStubsME0);

  // iEvent.getByToken(TkMuonStubsTokenOMTF_,TkMuonStubsOMTF);

  // edm::Handle<l1t::BayesMuCorrTrackBxCollection> TkMuonStubsHSCP;
  // iEvent.getByToken(TkMuonStubsTokenHSCP_,TkMuonStubsHSCP);

  edm::Handle<l1t::RegionalMuonCandBxCollection> muonsKalman;
  iEvent.getByToken(muonKalman_, muonsKalman);

  edm::Handle<l1t::RegionalMuonCandBxCollection> muonsOverlap;
  iEvent.getByToken(muonOverlap_, muonsOverlap);

  edm::Handle<l1t::RegionalMuonCandBxCollection> muonsEndcap;
  iEvent.getByToken(muonEndcap_, muonsEndcap);

  edm::Handle<l1t::L1TrkTauCollection> tkTau;
  iEvent.getByToken(tkTauToken_, tkTau);
  edm::Handle<l1t::L1CaloTkTauCollection> caloTkTau;
  iEvent.getByToken(caloTkTauToken_, caloTkTau);
  edm::Handle<l1t::TkEGTauCollection> tkEGTau;
  iEvent.getByToken(tkEGTauToken_, tkEGTau);

  //edm::Handle<l1t::PFTauCollection> PFTau;
  //iEvent.getByToken(PFTauToken_,PFTau);

  edm::Handle<l1t::PFTauCollection> l1NNTau;
  iEvent.getByToken(L1NNTauToken_, l1NNTau);

  edm::Handle<l1t::PFTauCollection> l1NNTauPF;
  iEvent.getByToken(L1NNTauPFToken_, l1NNTauPF);

  //edm::Handle<l1t::L1HPSPFTauCollection> l1HPSPFTau;
  //iEvent.getByToken(L1HPSPFTauToken_,l1HPSPFTau);

  edm::Handle<std::vector<l1t::PFCandidate>> l1PFCandidates;
  iEvent.getByToken(l1PFCandidates_, l1PFCandidates);

  edm::Handle<l1t::JetBxCollection> caloJet;
  iEvent.getByToken(caloJetToken_, caloJet);

  edm::Handle<l1t::TauBxCollection> caloTau;
  iEvent.getByToken(caloTauToken_, caloTau);

  edm::Handle<l1t::TkJetCollection> tkTrackerJet;
  edm::Handle<l1t::TkJetCollection> tkCaloJet;
  edm::Handle<l1t::TkEtMissCollection> tkMets;
  //edm::Handle<l1t::TkHTMissCollection> tkMhts;

  iEvent.getByToken(tkTrackerJetToken_, tkTrackerJet);
  iEvent.getByToken(tkCaloJetToken_, tkCaloJet);
  iEvent.getByToken(tkMetToken_, tkMets);
  //iEvent.getByToken(tkMhtToken_, tkMhts);

  edm::Handle<std::vector<l1t::PFJet>> ak4L1PFs;
  iEvent.getByToken(ak4L1PF_, ak4L1PFs);
  //        edm::Handle<std::vector<l1t::PFJet>> ak4L1PFForMETs;
  //        iEvent.getByToken(ak4L1PFForMET_,ak4L1PFForMETs);

  edm::Handle<std::vector<reco::PFMET>> l1PFMet;
  iEvent.getByToken(l1PFMet_, l1PFMet);

  //  edm::Handle<  std::vector<reco::CaloJet>  > l1pfPhase1L1TJet;
  //  iEvent.getByToken(l1pfPhase1L1TJetToken_,  l1pfPhase1L1TJet);

  // now also fill vertices

  edm::Handle<float> z0Puppi;
  iEvent.getByToken(z0PuppiToken_, z0Puppi);
  float Z0 = *z0Puppi;

  // edm::Handle<std::vector<l1t::Vertex> > l1vertextdr;
  // edm::Handle<std::vector<l1t::Vertex> > l1vertices;
  // iEvent.getByToken(l1vertextdrToken_,l1vertextdr);
  // iEvent.getByToken(l1verticesToken_,l1vertices);

  edm::Handle<std::vector<l1t::TkPrimaryVertex>> l1TkPrimaryVertex;
  iEvent.getByToken(l1TkPrimaryVertexToken_, l1TkPrimaryVertex);

  // Why just a value? no HTMiss? No angles?
  edm::Handle<float> caloJetHTTs;
  iEvent.getByToken(caloJetHTTToken_, caloJetHTTs);
  //float caloJetHTT=*caloJetHTTs;

  edm::Handle<std::vector<l1t::TkBsCandidate>> tkBsCands;
  iEvent.getByToken(L1TkBsCandToken_, tkBsCands);
  edm::Handle<std::vector<l1t::TkBsCandidate>> tkBsCandsLoose;
  iEvent.getByToken(L1TkBsCandLooseToken_, tkBsCandsLoose);
  edm::Handle<std::vector<l1t::TkBsCandidate>> tkBsCandsTight;
  iEvent.getByToken(L1TkBsCandTightToken_, tkBsCandsTight);

  //  float vertexTDRZ0=-999;
  //  if(l1vertextdr->size()>0) vertexTDRZ0=l1vertextdr->at(0).z0();

  //  if(l1vertices.isValid() && l1TkPrimaryVertex.isValid() &&  l1vertices->size()>0 && l1TkPrimaryVertex->size()>0){
  //        l1Extra->SetVertices(Z0,vertexTDRZ0,l1vertices,l1TkPrimaryVertex);
  //  }
  //  else {
  //          edm::LogWarning("MissingProduct") << "One of the L1TVertex collections is not valid " << std::endl;
  //          std::cout<<"Getting the vertices!"<<std::endl;
  //          std::cout<<Z0<<"   "<<l1vertextdr->size() <<"  "<< l1vertices->size() <<"   "<<  l1TkPrimaryVertex->size()<<std::endl;
  //  }

  if (caloJet.isValid() && caloJetHTTs.isValid()) {
    float caloJetHTT = *caloJetHTTs;
    l1Extra->SetCaloJet(caloJet, maxL1Extra_, caloJetHTT);
  } else {
    edm::LogWarning("MissingProduct") << "L1Upgrade caloJets not found. Branch will not be filled" << std::endl;
  }

  if (caloTau.isValid()) {
    l1Extra->SetCaloTau(caloTau, maxL1Extra_);
  } else {
    edm::LogWarning("MissingProduct") << "L1Upgrade caloTaus not found. Branch will not be filled" << std::endl;
  }

  if (muon.isValid()) {
    l1Extra->SetMuon(muon, maxL1Extra_);
  } else {
    edm::LogWarning("MissingProduct") << "L1Upgrade Muons not found. Branch will not be filled" << std::endl;
  }

  if (muonsKalman.isValid()) {
    l1Extra->SetMuonKF(muonsKalman, maxL1Extra_, 1);
  } else {
    edm::LogWarning("MissingProduct") << "L1Upgrade KBMTF Muons not found. Branch will not be filled" << std::endl;
  }

  if (muonsOverlap.isValid()) {
    l1Extra->SetMuonKF(muonsOverlap, maxL1Extra_, 2);
  } else {
    edm::LogWarning("MissingProduct") << "L1Upgrade KBMTF Muons not found. Branch will not be filled" << std::endl;
  }

  if (muonsEndcap.isValid()) {
    l1Extra->SetMuonKF(muonsEndcap, maxL1Extra_, 3);
  } else {
    edm::LogWarning("MissingProduct") << "L1Upgrade KBMTF Muons not found. Branch will not be filled" << std::endl;
  }

  edm::Handle<l1t::TkElectronCollection> tkEG;
  iEvent.getByToken(tkEGToken_, tkEG);
  edm::Handle<l1t::TkElectronCollection> tkEGHGC;
  iEvent.getByToken(tkEGTokenHGC_, tkEGHGC);

  //edm::Handle<l1t::TkElectronCollection> tkEGV2;
  //iEvent.getByToken(tkEGV2Token_, tkEGV2);
  //edm::Handle<l1t::TkElectronCollection> tkEGV2HGC;
  //iEvent.getByToken(tkEGV2TokenHGC_, tkEGV2HGC);

  if (tkEG.isValid() && tkEGHGC.isValid()) {
    l1Extra->SetTkEG(tkEG, tkEGHGC, maxL1Extra_);
  } else {
    edm::LogWarning("MissingProduct") << "L1PhaseII TkEG not found. Branch will not be filled" << std::endl;
  }
  /*
                if (tkEGV2.isValid() && tkEGV2HGC.isValid()){
                        l1Extra->SetTkEGV2(tkEGV2, tkEGV2HGC, maxL1Extra_);
                } else {
                        edm::LogWarning("MissingProduct") << "L1PhaseII tkEGV2 not found. Branch will not be filled" << std::endl;
                }
*/
  edm::Handle<l1t::EGammaBxCollection> eg;
  iEvent.getByToken(egToken_, eg);
  edm::Handle<l1t::EGammaBxCollection> egHGC;
  iEvent.getByToken(egTokenHGC_, egHGC);

  if (eg.isValid() && egHGC.isValid()) {
    l1Extra->SetEG(eg, egHGC, maxL1Extra_);
  } else {
    edm::LogWarning("MissingProduct") << "L1PhaseII Barrel EG not found. Branch will not be filled" << std::endl;
  }

  edm::Handle<l1t::TkEmCollection> tkEM;
  iEvent.getByToken(tkEMToken_, tkEM);

  edm::Handle<l1t::TkEmCollection> tkEMHGC;
  iEvent.getByToken(tkEMTokenHGC_, tkEMHGC);

  if (tkEM.isValid() && tkEMHGC.isValid()) {
    l1Extra->SetTkEM(tkEM, tkEMHGC, maxL1Extra_);
  } else {
    edm::LogWarning("MissingProduct") << "L1PhaseII  TkEM not found. Branch will not be filled" << std::endl;
  }

  if (tkTau.isValid()) {
    l1Extra->SetTrkTau(tkTau, maxL1Extra_);
  } else {
    edm::LogWarning("MissingProduct") << "L1PhaseII TrkTau not found. Branch will not be filled" << std::endl;
  }
  if (caloTkTau.isValid()) {
    l1Extra->SetCaloTkTau(caloTkTau, maxL1Extra_);
  } else {
    edm::LogWarning("MissingProduct") << "L1PhaseII caloTkTau not found. Branch will not be filled" << std::endl;
  }
  if (tkEGTau.isValid()) {
    l1Extra->SetTkEGTau(tkEGTau, maxL1Extra_);
  } else {
    edm::LogWarning("MissingProduct") << "L1PhaseII TkEGTau not found. Branch will not be filled" << std::endl;
  }

  if (tkTrackerJet.isValid()) {
    l1Extra->SetTkJet(tkTrackerJet, maxL1Extra_);
  } else {
    edm::LogWarning("MissingProduct") << "L1PhaseII tkTrackerJets not found. Branch will not be filled" << std::endl;
  }

  if (tkCaloJet.isValid()) {
    l1Extra->SetTkCaloJet(tkCaloJet, maxL1Extra_);
  } else {
    edm::LogWarning("MissingProduct") << "L1PhaseII TkCaloJets not found. Branch will not be filled" << std::endl;
  }

  //  if (l1pfPhase1L1TJet.isValid()){
  //          l1Extra->SetL1PfPhase1L1TJet(l1pfPhase1L1TJet, maxL1Extra_);
  //  } else {
  //         edm::LogWarning("MissingProduct") << "L1PhaseII l1pfPhase1L1TJets not found. Branch will not be filled" << std::endl;
  // }

  if (TkGlbMuon.isValid()) {
    l1Extra->SetTkGlbMuon(TkGlbMuon, maxL1Extra_);
  } else {
    edm::LogWarning("MissingProduct") << "L1PhaseII TkGlbMuons not found. Branch will not be filled" << std::endl;
  }
  if (TkMuon.isValid()) {
    l1Extra->SetTkMuon(TkMuon, maxL1Extra_);
    //                l1Extra->SetDiMuonTk(TkMuon,maxL1Extra_);

  } else {
    edm::LogWarning("MissingProduct") << "L1PhaseII TkMuons not found. Branch will not be filled" << std::endl;
  }
  //if (TkMuonStubsBMTF.isValid()){
  //        l1Extra->SetTkMuonStubs(TkMuonStubsBMTF, maxL1Extra_,1);
  //} else {
  //        edm::LogWarning("MissingProduct") << "L1PhaseII TkMuonStubsBMTF not found. Branch will not be filled" << std::endl;
  //}
  //if (TkMuonStubsOMTF.isValid()){
  //        l1Extra->SetTkMuonStubsOMTF(TkMuonStubsOMTF, maxL1Extra_,2);
  //} else {
  //        edm::LogWarning("MissingProduct") << "L1PhaseII TkMuonStubsOMTF not found. Branch will not be filled" << std::endl;
  //}
  //if (TkMuonStubsEMTF.isValid()){
  //        l1Extra->SetTkMuonStubs(TkMuonStubsEMTF, maxL1Extra_,3);
  //} else {
  //        edm::LogWarning("MissingProduct") << "L1PhaseII TkMuonStubsEMTF not found. Branch will not be filled" << std::endl;
  //}

  //if (TkMuonStubsME0.isValid()){
  //        l1Extra->SetTkMuonStubs(TkMuonStubsME0, maxL1Extra_,4);
  //} else {
  //        edm::LogWarning("MissingProduct") << "L1PhaseII TkMuonStubsME0 not found. Branch will not be filled" << std::endl;
  //}

  //if (TkMuonStubsHSCP.isValid()){
  //        l1Extra->SetHSCP(TkMuonStubsHSCP, maxL1Extra_);
  //} else {
  //        edm::LogWarning("MissingProduct") << "L1PhaseII TkMuonStubsHSCP not found. Branch will not be filled" << std::endl;
  //}

  if (tkMets.isValid()) {
    l1Extra->SetTkMET(tkMets);
  } else {
    edm::LogWarning("MissingProduct") << "L1PhaseII TkMET not found. Branch will not be filled" << std::endl;
  }

  for (auto& tkmhttoken : tkMhtToken_) {
    edm::Handle<l1t::TkHTMissCollection> tkMhts;
    iEvent.getByToken(tkmhttoken, tkMhts);

    if (tkMhts.isValid()) {
      l1Extra->SetTkMHT(tkMhts);
    } else {
      edm::LogWarning("MissingProduct") << "L1PhaseII TkMHT not found. Branch will not be filled" << std::endl;
    }
  }

  if (ak4L1PFs.isValid()) {
    l1Extra->SetPFJet(ak4L1PFs, maxL1Extra_);
  } else {
    edm::LogWarning("MissingProduct") << "L1PhaseII PFJets not found. Branch will not be filled" << std::endl;
  }

  /*        if (ak4L1PFForMETs.isValid()){
                l1Extra->SetPFJetForMET(ak4L1PFForMETs, maxL1Extra_);
        } else {
                edm::LogWarning("MissingProduct") << "L1PhaseII PFJetForMETs not found. Branch will not be filled" << std::endl;
        }
*/

  if (l1PFMet.isValid()) {
    l1Extra->SetL1METPF(l1PFMet);
  } else {
    edm::LogWarning("MissingProduct") << "L1PFMet missing" << std::endl;
  }

  if (l1PFCandidates.isValid()) {
    l1Extra->SetPFObjects(l1PFCandidates, maxL1Extra_);
  } else {
    edm::LogWarning("MissingProduct") << "L1PFCandidates missing, no L1PFMuons will be found" << std::endl;
  }

  //if(PFTau.isValid()){
  //        l1Extra->SetPFTaus(PFTau,maxL1Extra_);
  //} else{
  //        edm::LogWarning("MissingProduct") << "PFTaus missing"<<std::endl;
  //}

  if (l1NNTau.isValid()) {
    l1Extra->SetNNTaus(l1NNTau, maxL1Extra_);
  } else {
    edm::LogWarning("MissingProduct") << "L1NNTaus missing" << std::endl;
  }

  if (l1NNTauPF.isValid()) {
    l1Extra->SetNNTauPFs(l1NNTauPF, maxL1Extra_);
  } else {
    edm::LogWarning("MissingProduct") << "L1NNTauPFs missing" << std::endl;
  }

  //if(l1HPSPFTau.isValid()){
  //        l1Extra->SetHPSPFTaus(l1HPSPFTau,maxL1Extra_);
  //} else{
  //        edm::LogWarning("MissingProduct") << "L1HPSPFTaus missing"<<std::endl;
  //}

  if (tkBsCands.isValid()) {
    l1Extra->SetBsCands(tkBsCands, maxL1Extra_, 0);
  } else {
    edm::LogWarning("MissingProduct") << "L1TkBsCands missing " << std::endl;
  }
  if (tkBsCandsLoose.isValid()) {
    l1Extra->SetBsCands(tkBsCandsLoose, maxL1Extra_, 1);
  } else {
    edm::LogWarning("MissingProduct") << "L1TkBsCandsLoose missing " << std::endl;
  }
  if (tkBsCandsTight.isValid()) {
    l1Extra->SetBsCands(tkBsCandsTight, maxL1Extra_, 2);
  } else {
    edm::LogWarning("MissingProduct") << "L1TkBsCandsTight missing " << std::endl;
  }

  tree_->Fill();
}

// ------------ method called once each job just before starting event loop  ------------
void L1PhaseIITreeProducer::beginJob(void) {}

// ------------ method called once each job just after ending the event loop  ------------
void L1PhaseIITreeProducer::endJob() {}

//define this as a plug-in
DEFINE_FWK_MODULE(L1PhaseIITreeProducer);
