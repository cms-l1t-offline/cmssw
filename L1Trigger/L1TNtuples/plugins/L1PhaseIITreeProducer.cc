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

#include "DataFormats/L1TrackTrigger/interface/L1TkMuonParticle.h"
#include "DataFormats/L1TrackTrigger/interface/L1TkMuonParticleFwd.h"
#include "DataFormats/L1TrackTrigger/interface/L1TkPrimaryVertex.h"
#include "DataFormats/L1TrackTrigger/interface/L1TkEtMissParticle.h"
#include "DataFormats/L1TrackTrigger/interface/L1TkEtMissParticleFwd.h"
#include "DataFormats/L1TrackTrigger/interface/L1TkEmParticle.h"
#include "DataFormats/L1TrackTrigger/interface/L1TkEmParticleFwd.h"
#include "DataFormats/L1TrackTrigger/interface/L1TkElectronParticle.h"
#include "DataFormats/L1TrackTrigger/interface/L1TkElectronParticleFwd.h"
#include "DataFormats/L1TrackTrigger/interface/L1TkJetParticle.h"
#include "DataFormats/L1TrackTrigger/interface/L1TkJetParticleFwd.h"
#include "DataFormats/L1TrackTrigger/interface/L1TkTauParticle.h"
#include "DataFormats/L1TrackTrigger/interface/L1TkTauParticleFwd.h"
#include "DataFormats/L1TrackTrigger/interface/L1TkHTMissParticle.h"
#include "DataFormats/L1TrackTrigger/interface/L1TkHTMissParticleFwd.h"

#include "DataFormats/L1Trigger/interface/EGamma.h"
#include "DataFormats/L1Trigger/interface/Tau.h"
#include "DataFormats/L1Trigger/interface/Jet.h"
#include "DataFormats/L1Trigger/interface/Muon.h"
#include "DataFormats/L1Trigger/interface/EtSum.h"


// ROOT output stuff
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "TTree.h"

#include "L1Trigger/L1TNtuples/interface/L1AnalysisPhaseII.h"

//
// class declaration
//

class L1PhaseIITreeProducer : public edm::EDAnalyzer {
public:
  explicit L1PhaseIITreeProducer(const edm::ParameterSet&);
  ~L1PhaseIITreeProducer();
  
  
private:
  virtual void beginJob(void) ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob();

public:
  
  L1Analysis::L1AnalysisPhaseII* l1Extra;
  L1Analysis::L1AnalysisPhaseIIDataFormat * l1ExtraData;

private:

  unsigned maxL1Extra_;

  // output file
  edm::Service<TFileService> fs_;
  
  // tree
  TTree * tree_;

  edm::EDGetTokenT<l1t::EGammaBxCollection> egToken_;
  std::vector< edm::EDGetTokenT<l1t::TauBxCollection> > tauTokens_;
  edm::EDGetTokenT<l1t::JetBxCollection> jetToken_;
  edm::EDGetTokenT<l1t::EtSumBxCollection> sumToken_;
  edm::EDGetTokenT<l1t::MuonBxCollection> muonToken_;
  edm::EDGetTokenT<l1t::EGammaBxCollection> egCrystalToken_;

  edm::EDGetTokenT<l1t::L1TkElectronParticleCollection> tkEGToken_;
  edm::EDGetTokenT<l1t::L1TkElectronParticleCollection> tkIsoEGToken_;
  edm::EDGetTokenT<l1t::L1TkEmParticleCollection> tkEMToken_;

  edm::EDGetTokenT<l1t::L1TkElectronParticleCollection> tkEGCrystalToken_;
  edm::EDGetTokenT<l1t::L1TkEmParticleCollection> tkEMCrystalToken_;

  edm::EDGetTokenT<l1t::L1TkMuonParticleCollection> tkMuonToken_;
  edm::EDGetTokenT<l1t::L1TkTauParticleCollection> tkTauToken_;
  edm::EDGetTokenT<l1t::L1TkJetParticleCollection> tkJetToken_;
  edm::EDGetTokenT<l1t::L1TkEtMissParticleCollection> tkMetToken_;
  edm::EDGetTokenT<l1t::L1TkHTMissParticleCollection> tkMhtToken_;

};

L1PhaseIITreeProducer::L1PhaseIITreeProducer(const edm::ParameterSet& iConfig){

  
  egToken_ = consumes<l1t::EGammaBxCollection>(iConfig.getUntrackedParameter<edm::InputTag>("egToken"));
  jetToken_ = consumes<l1t::JetBxCollection>(iConfig.getUntrackedParameter<edm::InputTag>("jetToken"));
  sumToken_ = consumes<l1t::EtSumBxCollection>(iConfig.getUntrackedParameter<edm::InputTag>("sumToken"));
  muonToken_ = consumes<l1t::MuonBxCollection>(iConfig.getUntrackedParameter<edm::InputTag>("muonToken"));
         
  egCrystalToken_ = consumes<l1t::EGammaBxCollection>(iConfig.getUntrackedParameter<edm::InputTag>("egCrystalToken"));
 
  const auto& taus = iConfig.getUntrackedParameter<std::vector<edm::InputTag>>("tauTokens");
  for (const auto& tau: taus) {
      tauTokens_.push_back(consumes<l1t::TauBxCollection>(tau));
        }
  
  

  tkEGToken_ = consumes<l1t::L1TkElectronParticleCollection>(iConfig.getParameter<edm::InputTag>("tkEGToken"));
  tkIsoEGToken_ = consumes<l1t::L1TkElectronParticleCollection>(iConfig.getParameter<edm::InputTag>("tkIsoEGToken"));
  tkEMToken_ = consumes<l1t::L1TkEmParticleCollection>(iConfig.getParameter<edm::InputTag>("tkEMToken"));

  tkEGCrystalToken_ = consumes<l1t::L1TkElectronParticleCollection>(iConfig.getParameter<edm::InputTag>("tkEGCrystalToken"));
  tkEMCrystalToken_ = consumes<l1t::L1TkEmParticleCollection>(iConfig.getParameter<edm::InputTag>("tkEMCrystalToken"));

  tkMuonToken_ = consumes<l1t::L1TkMuonParticleCollection>(iConfig.getParameter<edm::InputTag>("tkMuonToken"));
  tkTauToken_ = consumes<l1t::L1TkTauParticleCollection>(iConfig.getParameter<edm::InputTag>("tkTauToken"));
  tkJetToken_ = consumes<l1t::L1TkJetParticleCollection>(iConfig.getParameter<edm::InputTag>("tkJetToken"));
  tkMetToken_ = consumes<l1t::L1TkEtMissParticleCollection>(iConfig.getParameter<edm::InputTag>("tkMetToken"));
  tkMhtToken_ = consumes<l1t::L1TkHTMissParticleCollection>(iConfig.getParameter<edm::InputTag>("tkMhtToken"));


  maxL1Extra_ = iConfig.getParameter<unsigned int>("maxL1Extra");
 
  l1Extra     = new L1Analysis::L1AnalysisPhaseII();
  l1ExtraData = l1Extra->getData();
  
  // set up output
  tree_=fs_->make<TTree>("L1PhaseIITree", "L1PhaseIITree");
  tree_->Branch("L1PhaseII", "L1Analysis::L1AnalysisPhaseIIDataFormat", &l1ExtraData, 32000, 3);

}


L1PhaseIITreeProducer::~L1PhaseIITreeProducer()
{
 
  // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to for each event  ------------
void
L1PhaseIITreeProducer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{

  
  l1Extra->Reset();

  edm::Handle<l1t::EGammaBxCollection> eg;
  edm::Handle<l1t::JetBxCollection> jet;
  edm::Handle<l1t::EtSumBxCollection> sums;
  edm::Handle<l1t::MuonBxCollection> muon;
  edm::Handle<l1t::EGammaBxCollection> egCrystal;
  

  iEvent.getByToken(egToken_,   eg);
  iEvent.getByToken(jetToken_,  jet);
  iEvent.getByToken(sumToken_, sums);
  iEvent.getByToken(muonToken_, muon);
  iEvent.getByToken(egCrystalToken_,   egCrystal);


  edm::Handle<l1t::L1TkElectronParticleCollection> tkEG;
  edm::Handle<l1t::L1TkElectronParticleCollection> tkIsoEG;
  edm::Handle<l1t::L1TkEmParticleCollection> tkEM;
  edm::Handle<l1t::L1TkMuonParticleCollection> tkMuon;
  edm::Handle<l1t::L1TkTauParticleCollection> tkTau;
  edm::Handle<l1t::L1TkJetParticleCollection> tkJet;
  edm::Handle<l1t::L1TkEtMissParticleCollection> tkMets;
  edm::Handle<l1t::L1TkHTMissParticleCollection> tkMhts;
  edm::Handle<l1t::L1TkElectronParticleCollection> tkEGCrystal;
  edm::Handle<l1t::L1TkEmParticleCollection> tkEMCrystal;

  iEvent.getByToken(tkEGToken_, tkEG);
  iEvent.getByToken(tkIsoEGToken_, tkIsoEG);
  iEvent.getByToken(tkEGCrystalToken_, tkEGCrystal);
  iEvent.getByToken(tkEMCrystalToken_, tkEMCrystal);
  iEvent.getByToken(tkEMToken_, tkEM);
  iEvent.getByToken(tkMuonToken_,tkMuon);
  iEvent.getByToken(tkTauToken_, tkTau);
  iEvent.getByToken(tkJetToken_, tkJet);
  iEvent.getByToken(tkMetToken_, tkMets);
  iEvent.getByToken(tkMhtToken_, tkMhts);

  if (eg.isValid()){ 
    l1Extra->SetEm(eg, maxL1Extra_);
  } else {
    edm::LogWarning("MissingProduct") << "L1Upgrade Em not found. Branch will not be filled" << std::endl;
  }
  if (jet.isValid()){ 
    l1Extra->SetJet(jet, maxL1Extra_);
  } else {
    edm::LogWarning("MissingProduct") << "L1Upgrade Jets not found. Branch will not be filled" << std::endl;
  }

  if (sums.isValid()){ 
    l1Extra->SetSum(sums, maxL1Extra_);  
  } else {
    edm::LogWarning("MissingProduct") << "L1Upgrade EtSums not found. Branch will not be filled" << std::endl;
  }

  if (muon.isValid()){ 
    l1Extra->SetMuon(muon, maxL1Extra_);
  } else {
    edm::LogWarning("MissingProduct") << "L1Upgrade Muons not found. Branch will not be filled" << std::endl;
  }

  for (auto & tautoken: tauTokens_){
    // keeping the format in the Run2 upgrade producer for this for consistency, even if it is a bit weird
    edm::Handle<l1t::TauBxCollection> tau;
    iEvent.getByToken(tautoken,  tau);
    if (tau.isValid()){ 
      l1Extra->SetTau(tau, maxL1Extra_);
    } else {
      edm::LogWarning("MissingProduct") << "L1Upgrade Tau not found. Branch will not be filled" << std::endl;
    }
  }


  if (tkEG.isValid()){
    l1Extra->SetTkEG(tkEG, maxL1Extra_);
  } else {
    edm::LogWarning("MissingProduct") << "L1PhaseII TkEG not found. Branch will not be filled" << std::endl;
  }

  if (tkIsoEG.isValid()){
    l1Extra->SetTkIsoEG(tkIsoEG, maxL1Extra_);
  } else {
    edm::LogWarning("MissingProduct") << "L1PhaseII TkIsoEG not found. Branch will not be filled" << std::endl;
  }

  if (tkEGCrystal.isValid()){
    l1Extra->SetTkEGCrystal(tkEGCrystal, maxL1Extra_);
  } else {
    edm::LogWarning("MissingProduct") << "L1PhaseII TkEG not found. Branch will not be filled" << std::endl;
  }

  if (egCrystal.isValid()){
    l1Extra->SetEGCrystal(egCrystal, maxL1Extra_);
  } else {
    edm::LogWarning("MissingProduct") << "L1PhaseII Barrel CrystalEG not found. Branch will not be filled" << std::endl;
  }

  if (tkEMCrystal.isValid()){
    l1Extra->SetTkEMCrystal(tkEMCrystal, maxL1Extra_);
  } else {
    edm::LogWarning("MissingProduct") << "L1PhaseII Crystal TkEM not found. Branch will not be filled" << std::endl;
  }

  if (tkEM.isValid()){
    l1Extra->SetTkEM(tkEM, maxL1Extra_);
  } else {
    edm::LogWarning("MissingProduct") << "L1PhaseII TkEM not found. Branch will not be filled" << std::endl;
  }

  if (tkTau.isValid()){
    l1Extra->SetTkTau(tkTau, maxL1Extra_);
  } else {
    edm::LogWarning("MissingProduct") << "L1PhaseII TkTau not found. Branch will not be filled" << std::endl;
  }

  if (tkJet.isValid()){
    l1Extra->SetTkJet(tkJet, maxL1Extra_);
  } else {
    edm::LogWarning("MissingProduct") << "L1PhaseII TkJets not found. Branch will not be filled" << std::endl;
  }

  if (tkMuon.isValid()){
    l1Extra->SetTkMuon(tkMuon, maxL1Extra_);
  } else {
    edm::LogWarning("MissingProduct") << "L1PhaseII TkMuons not found. Branch will not be filled" << std::endl;
  }

  if (tkMets.isValid()){
    l1Extra->SetTkMet(tkMets);
  } else {
    edm::LogWarning("MissingProduct") << "L1PhaseII TkMET not found. Branch will not be filled" << std::endl;
  }

  if (tkMhts.isValid()){
    l1Extra->SetTkMht(tkMhts);
  } else {
    edm::LogWarning("MissingProduct") << "L1PhaseII TkMHT not found. Branch will not be filled" << std::endl;
  }


  tree_->Fill();

}

// ------------ method called once each job just before starting event loop  ------------
void 
L1PhaseIITreeProducer::beginJob(void)
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
L1PhaseIITreeProducer::endJob() {
}

//define this as a plug-in
DEFINE_FWK_MODULE(L1PhaseIITreeProducer);
