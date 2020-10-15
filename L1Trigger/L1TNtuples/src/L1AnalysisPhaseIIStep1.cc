//This code is for filling the step1 menu objects, for full tree go for L1AnalysisPhaseII.c
#include "L1Trigger/L1TNtuples/interface/L1AnalysisPhaseIIStep1.h"
#include "L1Trigger/L1TMuon/interface/MicroGMTConfiguration.h"

L1Analysis::L1AnalysisPhaseIIStep1::L1AnalysisPhaseIIStep1() {}

L1Analysis::L1AnalysisPhaseIIStep1::~L1AnalysisPhaseIIStep1() {}

void L1Analysis::L1AnalysisPhaseIIStep1::SetVertices(
    float z0Puppi, const edm::Handle<std::vector<l1t::TkPrimaryVertex> > TkPrimaryVertex) {
  l1extra_.z0Puppi = z0Puppi;
  for (unsigned int i = 0; i < TkPrimaryVertex->size(); i++) {
    l1extra_.z0L1TkPV.push_back(TkPrimaryVertex->at(i).zvertex());
    l1extra_.sumL1TkPV.push_back(TkPrimaryVertex->at(i).sum());
    l1extra_.nL1TkPVs++;
  }
}

void L1Analysis::L1AnalysisPhaseIIStep1::SetCaloTau(const edm::Handle<l1t::TauBxCollection> calotau,
                                                    unsigned maxL1Extra) {
  for (int ibx = calotau->getFirstBX(); ibx <= calotau->getLastBX(); ++ibx) {
    for (l1t::TauBxCollection::const_iterator it = calotau->begin(ibx);
         it != calotau->end(ibx) && l1extra_.nCaloTaus < maxL1Extra;
         it++) {
      if (it->pt() > 0) {
        l1extra_.caloTauEt.push_back(it->et());
        l1extra_.caloTauEta.push_back(it->eta());
        l1extra_.caloTauPhi.push_back(it->phi());
        l1extra_.caloTauIEt.push_back(it->hwPt());
        l1extra_.caloTauIEta.push_back(it->hwEta());
        l1extra_.caloTauIPhi.push_back(it->hwPhi());
        l1extra_.caloTauIso.push_back(it->hwIso());
        l1extra_.caloTauBx.push_back(ibx);
        l1extra_.caloTauTowerIPhi.push_back(it->towerIPhi());
        l1extra_.caloTauTowerIEta.push_back(it->towerIEta());
        l1extra_.caloTauRawEt.push_back(it->rawEt());
        l1extra_.caloTauIsoEt.push_back(it->isoEt());
        l1extra_.caloTauNTT.push_back(it->nTT());
        l1extra_.caloTauHasEM.push_back(it->hasEM());
        l1extra_.caloTauIsMerged.push_back(it->isMerged());
        l1extra_.caloTauHwQual.push_back(it->hwQual());
        l1extra_.nCaloTaus++;
      }
    }
  }
}

//EG (seeded by Phase 2 Objects )
void L1Analysis::L1AnalysisPhaseIIStep1::SetEG(const edm::Handle<l1t::EGammaBxCollection> EG,
                                               const edm::Handle<l1t::EGammaBxCollection> EGHGC,
                                               unsigned maxL1Extra) {
  for (l1t::EGammaBxCollection::const_iterator it = EG->begin(); it != EG->end() && l1extra_.nEG < maxL1Extra; it++) {
    if (it->et() > 5) {
      l1extra_.EGEt.push_back(it->et());
      l1extra_.EGEta.push_back(it->eta());
      l1extra_.EGPhi.push_back(it->phi());
      l1extra_.EGIso.push_back(it->isoEt());
      l1extra_.EGHwQual.push_back(it->hwQual());
      l1extra_.EGBx.push_back(0);  //it->bx());
      l1extra_.EGHGC.push_back(0);
      bool quality = ((it->hwQual() >> 1) & 1) > 0;
      l1extra_.EGPassesLooseTrackID.push_back(quality);
      quality = ((it->hwQual() >> 2) & 1) > 0;
      l1extra_.EGPassesPhotonID.push_back(quality);
      l1extra_.nEG++;
    }
  }

  for (l1t::EGammaBxCollection::const_iterator it = EGHGC->begin(); it != EGHGC->end() && l1extra_.nEG < maxL1Extra;
       it++) {
    if (it->et() > 5) {
      l1extra_.EGEt.push_back(it->et());
      l1extra_.EGEta.push_back(it->eta());
      l1extra_.EGPhi.push_back(it->phi());
      l1extra_.EGIso.push_back(it->isoEt());
      l1extra_.EGHwQual.push_back(it->hwQual());
      l1extra_.EGBx.push_back(0);  //it->bx());
      l1extra_.EGHGC.push_back(1);
      bool quality = (it->hwQual() == 5);
      l1extra_.EGPassesLooseTrackID.push_back(quality);
      l1extra_.EGPassesPhotonID.push_back(quality);
      l1extra_.nEG++;
    }
  }
}

// TrkEG (seeded by Phase 2 Objects)
void L1Analysis::L1AnalysisPhaseIIStep1::SetTkEG(const edm::Handle<l1t::TkElectronCollection> tkElectron,
                                                 const edm::Handle<l1t::TkElectronCollection> tkElectronHGC,
                                                 unsigned maxL1Extra) {
  for (l1t::TkElectronCollection::const_iterator it = tkElectron->begin();
       it != tkElectron->end() && l1extra_.nTkElectrons < maxL1Extra;
       it++) {
    if (it->et() > 5) {
      l1extra_.tkElectronEt.push_back(it->et());
      l1extra_.tkElectronEta.push_back(it->eta());
      l1extra_.tkElectronPhi.push_back(it->phi());
      int chargeFromCurvature = (it->trackCurvature() > 0) ? 1 : -1;  // ThisIsACheck
      l1extra_.tkElectronChg.push_back(chargeFromCurvature);
      l1extra_.tkElectronzVtx.push_back(it->trkzVtx());
      l1extra_.tkElectronTrkIso.push_back(it->trkIsol());
      l1extra_.tkElectronHwQual.push_back(it->EGRef()->hwQual());
      l1extra_.tkElectronEGRefPt.push_back(it->EGRef()->et());
      l1extra_.tkElectronEGRefEta.push_back(it->EGRef()->eta());
      l1extra_.tkElectronEGRefPhi.push_back(it->EGRef()->phi());
      l1extra_.tkElectronBx.push_back(0);  //it->bx());
      l1extra_.tkElectronHGC.push_back(0);
      bool quality = ((it->EGRef()->hwQual() >> 1) & 1) > 0;  // LooseTrackID should be the second bit
      l1extra_.tkElectronPassesLooseTrackID.push_back(quality);
      quality = ((it->EGRef()->hwQual() >> 2) & 1) > 0;  // LooseTrackID should be the second bit
      l1extra_.tkElectronPassesPhotonID.push_back(quality);
      l1extra_.nTkElectrons++;
    }
  }

  for (l1t::TkElectronCollection::const_iterator it = tkElectronHGC->begin();
       it != tkElectronHGC->end() && l1extra_.nTkElectrons < maxL1Extra;
       it++) {
    if (it->et() > 5) {
      l1extra_.tkElectronEt.push_back(it->et());
      l1extra_.tkElectronEta.push_back(it->eta());
      l1extra_.tkElectronPhi.push_back(it->phi());
      int chargeFromCurvature = (it->trackCurvature() > 0) ? 1 : -1;  // ThisIsACheck
      l1extra_.tkElectronChg.push_back(chargeFromCurvature);
      l1extra_.tkElectronzVtx.push_back(it->trkzVtx());
      l1extra_.tkElectronTrkIso.push_back(it->trkIsol());
      l1extra_.tkElectronHwQual.push_back(it->EGRef()->hwQual());
      l1extra_.tkElectronEGRefPt.push_back(it->EGRef()->et());
      l1extra_.tkElectronEGRefEta.push_back(it->EGRef()->eta());
      l1extra_.tkElectronEGRefPhi.push_back(it->EGRef()->phi());
      l1extra_.tkElectronBx.push_back(0);  //it->bx());
      l1extra_.tkElectronHGC.push_back(1);
      bool quality = (it->EGRef()->hwQual() == 5);
      l1extra_.tkElectronPassesLooseTrackID.push_back(quality);
      l1extra_.tkElectronPassesPhotonID.push_back(quality);
      l1extra_.nTkElectrons++;
    }
  }
}

void L1Analysis::L1AnalysisPhaseIIStep1::SetTkEM(const edm::Handle<l1t::TkEmCollection> tkPhoton,
                                                 const edm::Handle<l1t::TkEmCollection> tkPhotonHGC,
                                                 unsigned maxL1Extra) {
  for (l1t::TkEmCollection::const_iterator it = tkPhoton->begin();
       it != tkPhoton->end() && l1extra_.nTkPhotons < maxL1Extra;
       it++) {
    if (it->et() > 5) {
      l1extra_.tkPhotonEt.push_back(it->et());
      l1extra_.tkPhotonEta.push_back(it->eta());
      l1extra_.tkPhotonPhi.push_back(it->phi());
      l1extra_.tkPhotonTrkIso.push_back(it->trkIsol());
      l1extra_.tkPhotonTrkIsoPV.push_back(it->trkIsolPV());
      l1extra_.tkPhotonBx.push_back(0);  //it->bx());
      l1extra_.tkPhotonHwQual.push_back(it->EGRef()->hwQual());
      l1extra_.tkPhotonEGRefPt.push_back(it->EGRef()->et());
      l1extra_.tkPhotonEGRefEta.push_back(it->EGRef()->eta());
      l1extra_.tkPhotonEGRefPhi.push_back(it->EGRef()->phi());
      l1extra_.tkPhotonHGC.push_back(0);
      bool quality = ((it->EGRef()->hwQual() >> 1) & 1) > 0;
      l1extra_.tkPhotonPassesLooseTrackID.push_back(quality);
      quality = ((it->EGRef()->hwQual() >> 2) & 1) > 0;  // Photon Id should be the third bit
      l1extra_.tkPhotonPassesPhotonID.push_back(quality);
      l1extra_.nTkPhotons++;
    }
  }
  for (l1t::TkEmCollection::const_iterator it = tkPhotonHGC->begin();
       it != tkPhotonHGC->end() && l1extra_.nTkPhotons < maxL1Extra;
       it++) {
    if (it->et() > 5) {
      l1extra_.tkPhotonEt.push_back(it->et());
      l1extra_.tkPhotonEta.push_back(it->eta());
      l1extra_.tkPhotonPhi.push_back(it->phi());
      l1extra_.tkPhotonTrkIso.push_back(it->trkIsol());
      l1extra_.tkPhotonTrkIsoPV.push_back(it->trkIsolPV());
      l1extra_.tkPhotonBx.push_back(0);  //it->bx());
      l1extra_.tkPhotonHwQual.push_back(it->EGRef()->hwQual());
      l1extra_.tkPhotonEGRefPt.push_back(it->EGRef()->et());
      l1extra_.tkPhotonEGRefEta.push_back(it->EGRef()->eta());
      l1extra_.tkPhotonEGRefPhi.push_back(it->EGRef()->phi());
      l1extra_.tkPhotonHGC.push_back(1);
      bool quality = (it->EGRef()->hwQual() == 5);
      l1extra_.tkPhotonPassesLooseTrackID.push_back(quality);
      l1extra_.tkPhotonPassesPhotonID.push_back(quality);
      l1extra_.nTkPhotons++;
    }
  }
}

void L1Analysis::L1AnalysisPhaseIIStep1::SetTkMuon(const edm::Handle<l1t::TkMuonCollection> muon, unsigned maxL1Extra) {
  for (l1t::TkMuonCollection::const_iterator it = muon->begin(); it != muon->end() && l1extra_.nTkMuons < maxL1Extra;
       it++) {
    l1extra_.tkMuonPt.push_back(it->pt());
    l1extra_.tkMuonEta.push_back(it->eta());
    l1extra_.tkMuonPhi.push_back(it->phi());
    int chargeFromCurvature = (it->trackCurvature() > 0) ? 1 : -1;  // ThisIsACheck
    l1extra_.tkMuonChg.push_back(chargeFromCurvature);
    l1extra_.tkMuonTrkIso.push_back(it->trkIsol());
    if (it->muonDetector() != 3) {
      l1extra_.tkMuonMuRefPt.push_back(it->muRef()->hwPt() * 0.5);
      l1extra_.tkMuonMuRefEta.push_back(it->muRef()->hwEta() * 0.010875);
      l1extra_.tkMuonMuRefPhi.push_back(l1t::MicroGMTConfiguration::calcGlobalPhi(it->muRef()->hwPhi(),
                                                                                  it->muRef()->trackFinderType(),
                                                                                  it->muRef()->processor()) *
                                        2 * M_PI / 576);
      l1extra_.tkMuonDRMuTrack.push_back(it->dR());
      l1extra_.tkMuonNMatchedTracks.push_back(it->nTracksMatched());
      l1extra_.tkMuonQual.push_back(it->muRef()->hwQual());
      l1extra_.tkMuonMuRefChg.push_back(pow(-1, it->muRef()->hwSign()));
    } else {
      l1extra_.tkMuonMuRefPt.push_back(-777);
      l1extra_.tkMuonMuRefEta.push_back(-777);
      l1extra_.tkMuonMuRefPhi.push_back(-777);
      l1extra_.tkMuonDRMuTrack.push_back(-777);
      l1extra_.tkMuonNMatchedTracks.push_back(0);
      l1extra_.tkMuonQual.push_back(999);
      l1extra_.tkMuonMuRefChg.push_back(0);
    }
    l1extra_.tkMuonRegion.push_back(it->muonDetector());
    l1extra_.tkMuonzVtx.push_back(it->trkzVtx());
    l1extra_.tkMuonBx.push_back(0);  //it->bx());
    l1extra_.nTkMuons++;
  }
}

/*
void L1Analysis::L1AnalysisPhaseIIStep1::SetL1PfPhase1L1TJet(const      edm::Handle< std::vector<reco::CaloJet> >  l1L1PFPhase1L1Jet,    unsigned maxL1Extra){

   double mHT30_px=0, mHT30_py=0, HT30=0;
  double mHT30_3p5_px=0, mHT30_3p5_py=0, HT30_3p5=0; 


    for (reco::CaloJetCollection::const_iterator it=l1L1PFPhase1L1Jet->begin(); it!=l1L1PFPhase1L1Jet->end() && l1extra_.nPfPhase1L1Jets<maxL1Extra; it++){
      if (it->pt() > 0){
      l1extra_.pfPhase1L1JetEt .push_back(it->et());
      l1extra_.pfPhase1L1JetEta.push_back(it->eta());
      l1extra_.pfPhase1L1JetPhi.push_back(it->phi());
//      l1extra_.pfPhase1L1JetBx .push_back(0);
      l1extra_.nPfPhase1L1Jets++;
 
    if(it->et()>30 && fabs(it->eta())<2.4) {
                  HT30+=it->et();
                  mHT30_px+=it->px();
                  mHT30_py+=it->py();
      }
    if(it->et()>30 && fabs(it->eta())<3.5) {
                  HT30_3p5+=it->et();
                  mHT30_3p5_px+=it->px();
                  mHT30_3p5_py+=it->py();
      }



   }  
  }  

  l1extra_.nPfPhase1L1MHT=2;

  l1extra_.pfPhase1L1MHTEt.push_back( sqrt(mHT30_px*mHT30_px+mHT30_py*mHT30_py) );
  l1extra_.pfPhase1L1MHTPhi.push_back( atan(mHT30_py/mHT30_px) );
  l1extra_.pfPhase1L1HT.push_back( HT30 );

  l1extra_.pfPhase1L1MHTEt.push_back( sqrt(mHT30_3p5_px*mHT30_3p5_px+mHT30_3p5_py*mHT30_3p5_py) );
  l1extra_.pfPhase1L1MHTPhi.push_back( atan(mHT30_3p5_py/mHT30_3p5_px) );
  l1extra_.pfPhase1L1HT.push_back( HT30_3p5 );


}
*/

void L1Analysis::L1AnalysisPhaseIIStep1::SetL1METPF(const edm::Handle<std::vector<reco::PFMET> > l1MetPF) {
  reco::PFMET met = l1MetPF->at(0);
  l1extra_.puppiMETEt = met.et();
  l1extra_.puppiMETPhi = met.phi();
}

void L1Analysis::L1AnalysisPhaseIIStep1::SetNNTaus(const edm::Handle<vector<l1t::PFTau> > l1nnTaus,
                                                   unsigned maxL1Extra) {
  for (unsigned int i = 0; i < l1nnTaus->size() && l1extra_.nNNTaus < maxL1Extra; i++) {
    if (l1nnTaus->at(i).pt() < 1)
      continue;
    l1extra_.nnTauEt.push_back(l1nnTaus->at(i).pt());
    l1extra_.nnTauEta.push_back(l1nnTaus->at(i).eta());
    l1extra_.nnTauPhi.push_back(l1nnTaus->at(i).phi());
    l1extra_.nnTauChg.push_back(l1nnTaus->at(i).charge());
    l1extra_.nnTauChargedIso.push_back(l1nnTaus->at(i).chargedIso());
    l1extra_.nnTauFullIso.push_back(l1nnTaus->at(i).fullIso());
    l1extra_.nnTauID.push_back(l1nnTaus->at(i).id());
    l1extra_.nnTauPassLooseNN.push_back(l1nnTaus->at(i).passLooseNN());
    l1extra_.nnTauPassLoosePF.push_back(l1nnTaus->at(i).passLoosePF());
    l1extra_.nnTauPassTightPF.push_back(l1nnTaus->at(i).passTightPF());
    l1extra_.nnTauPassTightNN.push_back(l1nnTaus->at(i).passTightNN());
    l1extra_.nNNTaus++;
  }
}
