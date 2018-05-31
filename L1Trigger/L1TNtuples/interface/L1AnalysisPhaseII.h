#ifndef __L1Analysis_L1AnalysisPhaseII_H__
#define __L1Analysis_L1AnalysisPhaseII_H__

//-------------------------------------------------------------------------------
// Created 02/03/2010 - A.C. Le Bihan
// 
// 
// Original code : UserCode/L1TriggerDPG/L1ExtraTreeProducer - Jim Brooke
//-------------------------------------------------------------------------------

#include "DataFormats/L1Trigger/interface/EGamma.h"
#include "DataFormats/L1Trigger/interface/Tau.h"
#include "DataFormats/L1Trigger/interface/Jet.h"
#include "DataFormats/L1Trigger/interface/Muon.h"
#include "DataFormats/L1Trigger/interface/EtSum.h"

#include "DataFormats/L1Trigger/interface/L1EmParticleFwd.h"
#include "DataFormats/L1Trigger/interface/L1EmParticle.h"
#include "DataFormats/L1Trigger/interface/L1JetParticleFwd.h"
#include "DataFormats/L1Trigger/interface/L1JetParticle.h"
#include "DataFormats/L1Trigger/interface/L1MuonParticleFwd.h"
#include "DataFormats/L1Trigger/interface/L1MuonParticle.h"
#include "DataFormats/L1Trigger/interface/L1EtMissParticleFwd.h"
#include "DataFormats/L1Trigger/interface/L1EtMissParticle.h"
#include "DataFormats/L1Trigger/interface/L1HFRingsFwd.h"
#include "DataFormats/L1Trigger/interface/L1HFRings.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutRecord.h"

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

#include "L1Trigger/L1TNtuples/interface/L1AnalysisPhaseIIDataFormat.h"

namespace L1Analysis
{
  class L1AnalysisPhaseII 
  {
  public:
    L1AnalysisPhaseII();
    ~L1AnalysisPhaseII();
    void Reset() {l1extra_.Reset();}

    void SetEm   (const edm::Handle<l1t::EGammaBxCollection> em,   unsigned maxL1Extra);
    void SetTau  (const edm::Handle<l1t::TauBxCollection>    tau,  unsigned maxL1Extra);
    void SetJet  (const edm::Handle<l1t::JetBxCollection>    jet,  unsigned maxL1Extra);
    void SetSum  (const edm::Handle<l1t::EtSumBxCollection>  sums, unsigned maxL1Extra);
    void SetMuon (const edm::Handle<l1t::MuonBxCollection>   muon, unsigned maxL1Extra);

    // Add new standalone objects 
    void SetEGCrystal   (const edm::Handle<l1t::EGammaBxCollection> EGCrystal,   unsigned maxL1Extra);

    // Add L1TrackTriggerObjects
    void SetTkEM          (const  edm::Handle<l1t::L1TkEmParticleCollection>   tkEM,     unsigned maxL1Extra);
    void SetTkEG          (const  edm::Handle<l1t::L1TkElectronParticleCollection>   tkEG,     unsigned maxL1Extra);
    void SetTkEGCrystal   (const  edm::Handle<l1t::L1TkElectronParticleCollection>   tkEGCrystal,     unsigned maxL1Extra);
    void SetTkEMCrystal   (const  edm::Handle<l1t::L1TkEmParticleCollection>   tkEMCrystal,     unsigned maxL1Extra);
    void SetTkIsoEG(const  edm::Handle<l1t::L1TkElectronParticleCollection>   tkIsoEG,     unsigned maxL1Extra);
    void SetTkMuon (const  edm::Handle<l1t::L1TkMuonParticleCollection> tkMuon,   unsigned maxL1Extra);
    void SetTkTau  (const  edm::Handle<l1t::L1TkTauParticleCollection> tkTau, unsigned maxL1Extra);
    void SetTkJet  (const  edm::Handle<l1t::L1TkJetParticleCollection>  tkJet,    unsigned maxL1Extra);
    void SetTkMet  (const  edm::Handle<l1t::L1TkEtMissParticleCollection> tkMets);
    void SetTkMht  (const  edm::Handle<l1t::L1TkHTMissParticleCollection> tkMhts);

    L1AnalysisPhaseIIDataFormat * getData() {return &l1extra_;}

  private :
    L1AnalysisPhaseIIDataFormat l1extra_;
  }; 
}
#endif


