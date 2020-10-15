// -*- C++ -*-
//
// Package:    UserCode/L1TriggerDPG
// Class:      L1PhaseIITreeStep1Producer
//
/**\class L1PhaseIITreeStep1Producer L1PhaseIITreeStep1Producer.cc UserCode/L1TriggerDPG/src/L1PhaseIITreeStep1Producer.cc

//This is a tree producer for L1 TDR Step 1 Menu - for the extended version, go for L1PhaseIITreeProducer.cc

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

#include "L1Trigger/L1TNtuples/interface/L1AnalysisPhaseIIStep1.h"
#include "L1Trigger/L1TNtuples/interface/L1AnalysisPhaseIIStep1DataFormat.h"

//
// class declaration
//

class L1PhaseIITreeStep1Producer : public edm::EDAnalyzer {
public:
  explicit L1PhaseIITreeStep1Producer(const edm::ParameterSet&);
  ~L1PhaseIITreeStep1Producer() override;

private:
  void beginJob(void) override;
  void analyze(const edm::Event&, const edm::EventSetup&) override;
  void endJob() override;

public:
  L1Analysis::L1AnalysisPhaseIIStep1* l1Extra;
  L1Analysis::L1AnalysisPhaseIIStep1DataFormat* l1ExtraData;

private:
  unsigned maxL1Extra_;

  // output file
  edm::Service<TFileService> fs_;

  // tree
  TTree* tree_;

  //Include only the step1 menu objects
  edm::EDGetTokenT<l1t::EGammaBxCollection> egToken_;
  edm::EDGetTokenT<l1t::TkElectronCollection> tkEGToken_;
  edm::EDGetTokenT<l1t::TkEmCollection> tkEMToken_;

  edm::EDGetTokenT<l1t::EGammaBxCollection> egTokenHGC_;
  edm::EDGetTokenT<l1t::TkElectronCollection> tkEGTokenHGC_;
  edm::EDGetTokenT<l1t::TkEmCollection> tkEMTokenHGC_;

  edm::EDGetTokenT<l1t::TkMuonCollection> TkMuonToken_;

  edm::EDGetTokenT<l1t::TauBxCollection> caloTauToken_;

  edm::EDGetTokenT<std::vector<reco::PFMET> > l1PFMet_;

  //edm::EDGetTokenT<std::vector<reco::CaloJet> > l1pfPhase1L1TJetToken_; // why are these caloJets???

  edm::EDGetTokenT<float> z0PuppiToken_;
  //edm::EDGetTokenT<l1t::VertexCollection> l1vertextdrToken_;
  //edm::EDGetTokenT<l1t::VertexCollection> l1verticesToken_;
  edm::EDGetTokenT<l1t::TkPrimaryVertexCollection> l1TkPrimaryVertexToken_;

  edm::EDGetTokenT<l1t::PFTauCollection> L1NNTauToken_;
  edm::EDGetTokenT<l1t::PFTauCollection> L1NNTauPFToken_;
};

L1PhaseIITreeStep1Producer::L1PhaseIITreeStep1Producer(const edm::ParameterSet& iConfig) {
  caloTauToken_ = consumes<l1t::TauBxCollection>(iConfig.getParameter<edm::InputTag>("caloTauToken"));

  egToken_ = consumes<l1t::EGammaBxCollection>(iConfig.getParameter<edm::InputTag>("egTokenBarrel"));
  egTokenHGC_ = consumes<l1t::EGammaBxCollection>(iConfig.getParameter<edm::InputTag>("egTokenHGC"));

  tkEGToken_ = consumes<l1t::TkElectronCollection>(iConfig.getParameter<edm::InputTag>("tkEGTokenBarrel"));
  tkEMToken_ = consumes<l1t::TkEmCollection>(iConfig.getParameter<edm::InputTag>("tkEMTokenBarrel"));

  tkEGTokenHGC_ = consumes<l1t::TkElectronCollection>(iConfig.getParameter<edm::InputTag>("tkEGTokenHGC"));
  tkEMTokenHGC_ = consumes<l1t::TkEmCollection>(iConfig.getParameter<edm::InputTag>("tkEMTokenHGC"));

  TkMuonToken_ = consumes<l1t::TkMuonCollection>(iConfig.getParameter<edm::InputTag>("TkMuonToken"));

  l1PFMet_ = consumes<std::vector<reco::PFMET> >(iConfig.getParameter<edm::InputTag>("l1PFMet"));

  //l1pfPhase1L1TJetToken_ = consumes<std::vector<reco::CaloJet> > (iConfig.getParameter<edm::InputTag>("l1pfPhase1L1TJetToken"));

  z0PuppiToken_ = consumes<float>(iConfig.getParameter<edm::InputTag>("zoPuppi"));
  //l1vertextdrToken_ = consumes< l1t::VertexCollection> (iConfig.getParameter<edm::InputTag>("l1vertextdr"));
  //l1verticesToken_  = consumes< l1t::VertexCollection> (iConfig.getParameter<edm::InputTag>("l1vertices"));
  l1TkPrimaryVertexToken_ =
      consumes<l1t::TkPrimaryVertexCollection>(iConfig.getParameter<edm::InputTag>("l1TkPrimaryVertex"));

  L1NNTauToken_ = consumes<l1t::PFTauCollection>(iConfig.getParameter<edm::InputTag>("L1NNTauToken"));
  L1NNTauPFToken_ = consumes<l1t::PFTauCollection>(iConfig.getParameter<edm::InputTag>("L1NNTauPFToken"));

  maxL1Extra_ = iConfig.getParameter<unsigned int>("maxL1Extra");

  l1Extra = new L1Analysis::L1AnalysisPhaseIIStep1();
  l1ExtraData = l1Extra->getData();

  // set up output
  tree_ = fs_->make<TTree>("L1PhaseIITree", "L1PhaseIITree");
  tree_->Branch("L1PhaseII", "L1Analysis::L1AnalysisPhaseIIStep1DataFormat", &l1ExtraData, 32000, 3);
}

L1PhaseIITreeStep1Producer::~L1PhaseIITreeStep1Producer() {
  // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)
}

//
// member functions
//

// ------------ method called to for each event  ------------
void L1PhaseIITreeStep1Producer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {
  l1Extra->Reset();

  edm::Handle<l1t::TkMuonCollection> TkMuon;
  iEvent.getByToken(TkMuonToken_, TkMuon);

  edm::Handle<l1t::PFTauCollection> l1NNTau;
  iEvent.getByToken(L1NNTauToken_, l1NNTau);

  edm::Handle<l1t::PFTauCollection> l1NNTauPF;
  iEvent.getByToken(L1NNTauPFToken_, l1NNTauPF);

  edm::Handle<l1t::TauBxCollection> caloTau;
  iEvent.getByToken(caloTauToken_, caloTau);

  edm::Handle<std::vector<reco::PFMET> > l1PFMet;
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

  edm::Handle<std::vector<l1t::TkPrimaryVertex> > l1TkPrimaryVertex;
  iEvent.getByToken(l1TkPrimaryVertexToken_, l1TkPrimaryVertex);

  //  float vertexTDRZ0=-999;
  //  if(l1vertextdr->size()>0) vertexTDRZ0=l1vertextdr->at(0).z0();
  /*
       if(l1vertices.isValid() && l1TkPrimaryVertex.isValid() &&  l1vertices->size()>0 && l1TkPrimaryVertex->size()>0){
             l1Extra->SetVertices(Z0,vertexTDRZ0,l1vertices,l1TkPrimaryVertex);
       }
       else {
                edm::LogWarning("MissingProduct") << "One of the L1TVertex collections is not valid " << std::endl;
                std::cout<<"Getting the vertices!"<<std::endl;
                std::cout<<Z0<<"   "<<l1vertextdr->size() <<"  "<< l1vertices->size() <<"   "<<  l1TkPrimaryVertex->size()<<std::endl;
        }
*/

  if (l1TkPrimaryVertex.isValid() && !l1TkPrimaryVertex->empty()) {
    l1Extra->SetVertices(Z0, l1TkPrimaryVertex);
  } else {
    edm::LogWarning("MissingProduct") << "One of the L1TVertex collections is not valid " << std::endl;
    std::cout << "Getting the vertices!" << std::endl;
    std::cout << Z0 << " " << l1TkPrimaryVertex->size() << std::endl;
  }

  if (caloTau.isValid()) {
    l1Extra->SetCaloTau(caloTau, maxL1Extra_);
  } else {
    edm::LogWarning("MissingProduct") << "L1Upgrade caloTaus not found. Branch will not be filled" << std::endl;
  }

  edm::Handle<l1t::TkElectronCollection> tkEG;
  iEvent.getByToken(tkEGToken_, tkEG);
  edm::Handle<l1t::TkElectronCollection> tkEGHGC;
  iEvent.getByToken(tkEGTokenHGC_, tkEGHGC);

  if (tkEG.isValid() && tkEGHGC.isValid()) {
    l1Extra->SetTkEG(tkEG, tkEGHGC, maxL1Extra_);
  } else {
    edm::LogWarning("MissingProduct") << "L1PhaseII TkEG not found. Branch will not be filled" << std::endl;
  }

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

  //  if (l1pfPhase1L1TJet.isValid()){
  //          l1Extra->SetL1PfPhase1L1TJet(l1pfPhase1L1TJet, maxL1Extra_);
  //  } else {
  //         edm::LogWarning("MissingProduct") << "L1PhaseII l1pfPhase1L1TJets not found. Branch will not be filled" << std::endl;
  // }

  if (TkMuon.isValid()) {
    l1Extra->SetTkMuon(TkMuon, maxL1Extra_);
    //                l1Extra->SetDiMuonTk(TkMuon,maxL1Extra_);

  } else {
    edm::LogWarning("MissingProduct") << "L1PhaseII TkMuons not found. Branch will not be filled" << std::endl;
  }

  if (l1PFMet.isValid()) {
    l1Extra->SetL1METPF(l1PFMet);
  } else {
    edm::LogWarning("MissingProduct") << "L1PFMet missing" << std::endl;
  }

  if (l1NNTau.isValid()) {
    l1Extra->SetNNTaus(l1NNTau, maxL1Extra_);
  } else {
    edm::LogWarning("MissingProduct") << "L1NNTaus missing" << std::endl;
  }

  tree_->Fill();
}

// ------------ method called once each job just before starting event loop  ------------
void L1PhaseIITreeStep1Producer::beginJob(void) {}

// ------------ method called once each job just after ending the event loop  ------------
void L1PhaseIITreeStep1Producer::endJob() {}

//define this as a plug-in
DEFINE_FWK_MODULE(L1PhaseIITreeStep1Producer);
