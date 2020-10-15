#ifndef __L1Analysis_L1AnalysisPhaseIIDataFormat_H__
#define __L1Analysis_L1AnalysisPhaseIIDataFormat_H__

//-------------------------------------------------------------------------------
// Created 20/04/2010 - E. Conte, A.C. Le Bihan
//
//
// Original code : UserCode/L1TriggerDPG/L1ExtraTreeProducer - Jim Brooke
//-------------------------------------------------------------------------------

#include <vector>

namespace L1Analysis {
  struct L1AnalysisPhaseIIDataFormat {
    L1AnalysisPhaseIIDataFormat() { Reset(); };
    ~L1AnalysisPhaseIIDataFormat(){};

    void Reset() {
      z0Puppi = 0;
      z0VertexTDR = 0;
      z0Vertices.clear();
      z0L1TkPV.clear();
      sumL1TkPV.clear();
      nL1TkPVs = 0;
      nVertices = 0;

      nCaloTaus = 0;
      caloTauEt.clear();
      caloTauEta.clear();
      caloTauPhi.clear();
      caloTauIEt.clear();
      caloTauIEta.clear();
      caloTauIPhi.clear();
      caloTauIso.clear();
      caloTauBx.clear();
      caloTauTowerIPhi.clear();
      caloTauTowerIEta.clear();
      caloTauRawEt.clear();
      caloTauIsoEt.clear();
      caloTauNTT.clear();
      caloTauHasEM.clear();
      caloTauIsMerged.clear();
      caloTauHwQual.clear();

      nCaloJets = 0;
      caloJetEt.clear();
      caloJetEta.clear();
      caloJetPhi.clear();
      caloJetBx.clear();

      nPfPhase1L1Jets = 0;
      pfPhase1L1JetEt.clear();
      pfPhase1L1JetEta.clear();
      pfPhase1L1JetPhi.clear();

      pfPhase1L1HT.clear();
      pfPhase1L1MHTEt.clear();
      pfPhase1L1MHTPhi.clear();
      nPfPhase1L1MHT = 0;

      caloJetHTDefault = 0;
      caloJetHT.clear();
      caloJetMHTEt.clear();
      caloJetMHTPhi.clear();
      nCaloJetMHT = 0;

      nGlobalMuons = 0;
      globalMuonPt.clear();
      globalMuonEta.clear();
      globalMuonPhi.clear();
      globalMuonEtaAtVtx.clear();
      globalMuonPhiAtVtx.clear();
      globalMuonIEt.clear();
      globalMuonIEta.clear();
      globalMuonIPhi.clear();
      globalMuonIEtaAtVtx.clear();
      globalMuonIPhiAtVtx.clear();
      globalMuonIDEta.clear();
      globalMuonIDPhi.clear();
      globalMuonChg.clear();
      globalMuonIso.clear();
      globalMuonQual.clear();
      globalMuonTfMuonIdx.clear();
      globalMuonBx.clear();

      nStandaloneMuons = 0;
      standaloneMuonPt.clear();
      standaloneMuonPt2.clear();
      standaloneMuonEta.clear();
      standaloneMuonPhi.clear();
      standaloneMuonChg.clear();
      standaloneMuonQual.clear();
      standaloneMuonBx.clear();
      standaloneMuonRegion.clear();
      standaloneMuonDXY.clear();

      nEG = 0;
      EGEt.clear();
      EGEta.clear();
      EGPhi.clear();
      EGBx.clear();
      EGIso.clear();
      EGzVtx.clear();
      EGHwQual.clear();
      EGHGC.clear();
      EGPassesLooseTrackID.clear();
      EGPassesPhotonID.clear();

      nTkElectrons = 0;
      tkElectronEt.clear();
      tkElectronEta.clear();
      tkElectronPhi.clear();
      tkElectronChg.clear();
      tkElectronBx.clear();
      tkElectronTrkIso.clear();
      tkElectronzVtx.clear();
      tkElectronHwQual.clear();
      tkElectronEGRefPt.clear();
      tkElectronEGRefEta.clear();
      tkElectronEGRefPhi.clear();
      tkElectronHGC.clear();
      tkElectronPassesLooseTrackID.clear();
      tkElectronPassesPhotonID.clear();

      /*
      nTkElectronsV2 = 0;
      tkElectronV2Et.clear();
      tkElectronV2Eta.clear();
      tkElectronV2Phi.clear();
      tkElectronV2Chg.clear();
      tkElectronV2Bx.clear();
      tkElectronV2TrkIso.clear();
      tkElectronV2zVtx.clear();
      tkElectronV2HwQual.clear();
      tkElectronV2EGRefPt.clear();
      tkElectronV2EGRefEta.clear();
      tkElectronV2EGRefPhi.clear();
      tkElectronV2HGC.clear();
      tkElectronV2PassesLooseTrackID.clear();         
      tkElectronV2PassesPhotonID.clear();
*/

      nTkPhotons = 0;
      tkPhotonEt.clear();
      tkPhotonEta.clear();
      tkPhotonPhi.clear();
      tkPhotonBx.clear();
      tkPhotonTrkIso.clear();
      tkPhotonTrkIsoPV.clear();
      tkPhotonzVtx.clear();
      tkPhotonHwQual.clear();
      tkPhotonEGRefPt.clear();
      tkPhotonEGRefEta.clear();
      tkPhotonEGRefPhi.clear();
      tkPhotonHGC.clear();
      tkPhotonPassesLooseTrackID.clear();
      tkPhotonPassesPhotonID.clear();

      // TkTaus
      nTkTau = 0;
      tkTauEt.clear();
      tkTauEta.clear();
      tkTauPhi.clear();
      tkTauBx.clear();
      tkTauTrkIso.clear();
      tkTauzVtx.clear();

      nCaloTkTau = 0;
      caloTkTauEt.clear();
      caloTkTauEta.clear();
      caloTkTauPhi.clear();
      caloTkTauBx.clear();
      caloTkTauTrkIso.clear();
      caloTkTauzVtx.clear();

      nTkEGTau = 0;
      tkEGTauEt.clear();
      tkEGTauEta.clear();
      tkEGTauPhi.clear();
      tkEGTauBx.clear();
      tkEGTauTrkIso.clear();
      tkEGTauzVtx.clear();

      // TkJets
      nTrackerJets = 0;
      trackerJetEt.clear();
      trackerJetEta.clear();
      trackerJetPhi.clear();
      trackerJetBx.clear();
      trackerJetzVtx.clear();

      // TkCaloJets
      nTkCaloJets = 0;
      tkCaloJetEt.clear();
      tkCaloJetEta.clear();
      tkCaloJetPhi.clear();
      tkCaloJetBx.clear();

      // tkTkGlbMuons
      nTkGlbMuons = 0;
      tkGlbMuonPt.clear();
      tkGlbMuonEta.clear();
      tkGlbMuonPhi.clear();
      tkGlbMuonChg.clear();
      tkGlbMuonTrkIso.clear();
      tkGlbMuonBx.clear();
      tkGlbMuonQual.clear();
      tkGlbMuonzVtx.clear();
      tkGlbMuonMuRefPt.clear();
      tkGlbMuonTrkRefPt.clear();
      tkGlbMuonMuRefPhi.clear();
      tkGlbMuonMuRefEta.clear();
      tkGlbMuonDRMuTrack.clear();
      tkGlbMuonNMatchedTracks.clear();

      nTkMuons = 0;
      tkMuonPt.clear();
      tkMuonEta.clear();
      tkMuonPhi.clear();
      tkMuonChg.clear();
      tkMuonTrkIso.clear();
      tkMuonBx.clear();
      tkMuonQual.clear();
      tkMuonzVtx.clear();
      tkMuonMuRefPt.clear();
      tkMuonTrkRefPt.clear();
      tkMuonMuRefPhi.clear();
      tkMuonMuRefEta.clear();
      tkMuonDRMuTrack.clear();
      tkMuonNMatchedTracks.clear();
      tkMuonMuRefChg.clear();
      tkMuonRegion.clear();

      nTkMuonStubs = 0;
      tkMuonStubsPt.clear();
      tkMuonStubsEta.clear();
      tkMuonStubsPhi.clear();
      tkMuonStubsChg.clear();
      tkMuonStubsTrkIso.clear();
      tkMuonStubsBx.clear();
      tkMuonStubsQual.clear();
      tkMuonStubszVtx.clear();
      tkMuonStubsBarrelStubs.clear();
      tkMuonStubsRegion.clear();

      nTkHSCPs = 0;
      tkHSCPsPt.clear();
      tkHSCPsEta.clear();
      tkHSCPsPhi.clear();
      tkHSCPsChg.clear();
      tkHSCPsBx.clear();
      tkHSCPszVtx.clear();

      // TrackerMet
      nTrackerMet = 0;
      trackerMetSumEt.clear();
      trackerMetEt.clear();
      trackerMetPhi.clear();
      trackerMetBx.clear();

      //trackerMHT
      nTrackerMHT = 0;
      trackerHT.clear();
      trackerMHT.clear();
      trackerMHTPhi.clear();

      // New Jet Collections
      nPuppiJets = 0;
      puppiJetEt.clear();
      puppiJetEta.clear();
      puppiJetPhi.clear();
      puppiJetBx.clear();
      puppiJetzVtx.clear();
      puppiJetEtUnCorr.clear();

      /*      nPuppiJetForMETs = 0;
      puppiJetForMETEt.clear();
      puppiJetForMETEta.clear();
      puppiJetForMETPhi.clear();
      puppiJetForMETBx.clear();
      puppiJetForMETzVtx.clear();
      puppiJetForMETEtUnCorr.clear();
*/

      puppiMETEt = 0;
      puppiMETPhi = 0;
      puppiHT.clear();
      puppiMHTEt.clear();
      puppiMHTPhi.clear();
      nPuppiMHT = 0;

      nPFMuons = 0;
      pfMuonPt.clear();
      pfMuonEta.clear();
      pfMuonPhi.clear();
      pfMuonzVtx.clear();
      pfMuonChg.clear();

      nPFCands = 0;
      pfCandId.clear();
      pfCandEt.clear();
      pfCandEta.clear();
      pfCandPhi.clear();
      pfCandzVtx.clear();
      pfCandChg.clear();

      nPFTaus = 0;
      pfTauEt.clear();
      pfTauEta.clear();
      pfTauPhi.clear();
      pfTauChargedIso.clear();
      pfTauType.clear();
      pfTauIsoFlag.clear();
      pfTauRelIsoFlag.clear();
      pfTauPassesMediumIso.clear();
      pfTauChg.clear();
      pfTauZ0.clear();

      nNNTaus = 0;
      nnTauEt.clear();
      nnTauEta.clear();
      nnTauPhi.clear();
      nnTauChg.clear();
      nnTauChargedIso.clear();
      nnTauFullIso.clear();
      nnTauID.clear();
      nnTauPassLooseNN.clear();
      nnTauPassLoosePF.clear();
      nnTauPassTightPF.clear();
      nnTauPassTightNN.clear();

      nNNTauPFs = 0;
      nnTauPFEt.clear();
      nnTauPFEta.clear();
      nnTauPFPhi.clear();
      nnTauPFChg.clear();
      nnTauPFChargedIso.clear();
      nnTauPFFullIso.clear();
      nnTauPFID.clear();
      nnTauPFPassLooseNN.clear();
      nnTauPFPassLoosePF.clear();
      nnTauPFPassTightPF.clear();
      nnTauPFPassTightNN.clear();

      nHPSTaus = 0;
      hpsTauEt.clear();
      hpsTauEta.clear();
      hpsTauPhi.clear();
      hpsTauChg.clear();
      hpsTauPassTightRelIso.clear();
      hpsTauType.clear();
      hpsTauZ0.clear();

      nTkBsCands = 0;
      tkBsCandPt.clear();
      tkBsCandEta.clear();
      tkBsCandPhi.clear();
      tkBsCandMass.clear();
      tkBsCandPhi1Pt.clear();
      tkBsCandPhi2Pt.clear();
      tkBsCandPhi1Eta.clear();
      tkBsCandPhi2Eta.clear();
      tkBsCandPhi1Phi.clear();
      tkBsCandPhi2Phi.clear();
      tkBsCandPhi1Mass.clear();
      tkBsCandPhi2Mass.clear();
      tkBsCandDRPhiPair.clear();
      tkBsCandDxyPhiPair.clear();
      tkBsCandDzPhiPair.clear();
      tkBsCandKind.clear();
    }

    double z0Puppi;
    double z0VertexTDR;
    unsigned short int nVertices;
    std::vector<double> z0Vertices;
    unsigned short int nL1TkPVs;
    std::vector<double> z0L1TkPV;
    std::vector<double> sumL1TkPV;

    unsigned short int nCaloTaus;
    std::vector<double> caloTauEt;
    std::vector<double> caloTauEta;
    std::vector<double> caloTauPhi;
    std::vector<short int> caloTauIEt;
    std::vector<short int> caloTauIEta;
    std::vector<short int> caloTauIPhi;
    std::vector<short int> caloTauIso;
    std::vector<short int> caloTauBx;
    std::vector<short int> caloTauTowerIPhi;
    std::vector<short int> caloTauTowerIEta;
    std::vector<short int> caloTauRawEt;
    std::vector<short int> caloTauIsoEt;
    std::vector<short int> caloTauNTT;
    std::vector<short int> caloTauHasEM;
    std::vector<short int> caloTauIsMerged;
    std::vector<short int> caloTauHwQual;

    unsigned short int nCaloJets;
    std::vector<double> caloJetEt;
    std::vector<double> caloJetEta;
    std::vector<double> caloJetPhi;
    std::vector<short int> caloJetBx;

    unsigned short int nPfPhase1L1Jets;
    std::vector<double> pfPhase1L1JetEt;
    std::vector<double> pfPhase1L1JetEta;
    std::vector<double> pfPhase1L1JetPhi;

    std::vector<double> pfPhase1L1HT;
    std::vector<double> pfPhase1L1MHTEt;
    std::vector<double> pfPhase1L1MHTPhi;
    unsigned int nPfPhase1L1MHT;

    float caloJetHTDefault;
    std::vector<double> caloJetHT;
    std::vector<double> caloJetMHTEt;
    std::vector<double> caloJetMHTPhi;
    unsigned int nCaloJetMHT;

    unsigned short int nGlobalMuons;
    std::vector<double> globalMuonPt;
    std::vector<double> globalMuonEta;
    std::vector<double> globalMuonPhi;
    std::vector<double> globalMuonEtaAtVtx;
    std::vector<double> globalMuonPhiAtVtx;
    std::vector<short int> globalMuonIEt;
    std::vector<short int> globalMuonIEta;
    std::vector<short int> globalMuonIPhi;
    std::vector<short int> globalMuonIEtaAtVtx;
    std::vector<short int> globalMuonIPhiAtVtx;
    std::vector<short int> globalMuonIDEta;
    std::vector<short int> globalMuonIDPhi;
    std::vector<short int> globalMuonChg;
    std::vector<unsigned short int> globalMuonIso;
    std::vector<unsigned short int> globalMuonQual;
    std::vector<unsigned short int> globalMuonTfMuonIdx;
    std::vector<short int> globalMuonBx;

    unsigned short int nStandaloneMuons;
    std::vector<double> standaloneMuonPt;
    std::vector<double> standaloneMuonPt2;
    std::vector<double> standaloneMuonEta;
    std::vector<double> standaloneMuonPhi;
    std::vector<short int> standaloneMuonChg;
    std::vector<unsigned short int> standaloneMuonQual;
    std::vector<double> standaloneMuonDXY;
    std::vector<short int> standaloneMuonBx;
    std::vector<unsigned int> standaloneMuonRegion;

    unsigned int nEG;
    std::vector<double> EGEt;
    std::vector<double> EGEta;
    std::vector<double> EGPhi;
    std::vector<int> EGBx;
    std::vector<double> EGIso;
    std::vector<double> EGzVtx;
    std::vector<int> EGHwQual;
    std::vector<unsigned int> EGHGC;
    std::vector<unsigned int> EGPassesLooseTrackID;
    std::vector<unsigned int> EGPassesPhotonID;

    unsigned int nTkElectrons;
    std::vector<double> tkElectronEt;
    std::vector<double> tkElectronEta;
    std::vector<double> tkElectronPhi;
    std::vector<int> tkElectronChg;
    std::vector<int> tkElectronBx;
    std::vector<double> tkElectronTrkIso;
    std::vector<double> tkElectronzVtx;
    std::vector<double> tkElectronHwQual;
    std::vector<double> tkElectronEGRefPt;
    std::vector<double> tkElectronEGRefEta;
    std::vector<double> tkElectronEGRefPhi;
    std::vector<unsigned int> tkElectronHGC;
    std::vector<unsigned int> tkElectronPassesLooseTrackID;
    std::vector<unsigned int> tkElectronPassesPhotonID;

    /*
    unsigned int nTkElectronsV2;
    std::vector<double> tkElectronV2Et;
    std::vector<double> tkElectronV2Eta;
    std::vector<double> tkElectronV2Phi;
    std::vector<double> tkElectronV2Chg;
    std::vector<int>    tkElectronV2Bx;
    std::vector<double> tkElectronV2TrkIso;
    std::vector<double> tkElectronV2zVtx;
    std::vector<double> tkElectronV2HwQual;
    std::vector<double>   tkElectronV2EGRefPt;
    std::vector<double>   tkElectronV2EGRefEta;
    std::vector<double>   tkElectronV2EGRefPhi;
    std::vector<unsigned int> tkElectronV2HGC;
    std::vector<unsigned int> tkElectronV2PassesLooseTrackID;
    std::vector<unsigned int> tkElectronV2PassesPhotonID;
*/

    unsigned int nTkPhotons;
    std::vector<double> tkPhotonEt;
    std::vector<double> tkPhotonEta;
    std::vector<double> tkPhotonPhi;
    std::vector<int> tkPhotonBx;
    std::vector<double> tkPhotonTrkIso;
    std::vector<double> tkPhotonTrkIsoPV;
    std::vector<double> tkPhotonzVtx;
    std::vector<double> tkPhotonHwQual;
    std::vector<double> tkPhotonEGRefPt;
    std::vector<double> tkPhotonEGRefEta;
    std::vector<double> tkPhotonEGRefPhi;
    std::vector<unsigned int> tkPhotonHGC;
    std::vector<unsigned int> tkPhotonPassesLooseTrackID;
    std::vector<unsigned int> tkPhotonPassesPhotonID;

    unsigned int nTkTau;
    std::vector<double> tkTauEt;
    std::vector<double> tkTauEta;
    std::vector<double> tkTauPhi;
    std::vector<int> tkTauBx;
    std::vector<double> tkTauTrkIso;
    std::vector<double> tkTauzVtx;

    unsigned int nCaloTkTau;
    std::vector<double> caloTkTauEt;
    std::vector<double> caloTkTauEta;
    std::vector<double> caloTkTauPhi;
    std::vector<int> caloTkTauBx;
    std::vector<double> caloTkTauTrkIso;
    std::vector<double> caloTkTauzVtx;

    unsigned int nTkEGTau;
    std::vector<double> tkEGTauEt;
    std::vector<double> tkEGTauEta;
    std::vector<double> tkEGTauPhi;
    std::vector<int> tkEGTauBx;
    std::vector<double> tkEGTauTrkIso;
    std::vector<double> tkEGTauzVtx;

    unsigned int nTrackerJets;
    std::vector<double> trackerJetEt;
    std::vector<double> trackerJetEta;
    std::vector<double> trackerJetPhi;
    std::vector<int> trackerJetBx;
    std::vector<double> trackerJetzVtx;

    unsigned int nTkCaloJets;
    std::vector<double> tkCaloJetEt;
    std::vector<double> tkCaloJetEta;
    std::vector<double> tkCaloJetPhi;
    std::vector<int> tkCaloJetBx;
    std::vector<double> tkCaloJetzVtx;

    unsigned int nTkGlbMuons;
    std::vector<double> tkGlbMuonPt;
    std::vector<double> tkGlbMuonEta;
    std::vector<double> tkGlbMuonPhi;
    std::vector<int> tkGlbMuonChg;
    std::vector<unsigned int> tkGlbMuonIso;
    std::vector<double> tkGlbMuonTrkIso;
    std::vector<int> tkGlbMuonBx;
    std::vector<unsigned int> tkGlbMuonQual;
    std::vector<double> tkGlbMuonzVtx;
    std::vector<double> tkGlbMuonMuRefPt;
    std::vector<double> tkGlbMuonTrkRefPt;
    std::vector<double> tkGlbMuonMuRefPhi;
    std::vector<double> tkGlbMuonMuRefEta;
    std::vector<double> tkGlbMuonDRMuTrack;
    std::vector<double> tkGlbMuonNMatchedTracks;

    unsigned int nTkMuons;
    std::vector<double> tkMuonPt;
    std::vector<double> tkMuonEta;
    std::vector<double> tkMuonPhi;
    std::vector<int> tkMuonChg;
    std::vector<unsigned int> tkMuonIso;
    std::vector<double> tkMuonTrkIso;
    std::vector<unsigned int> tkMuonFwd;
    std::vector<unsigned int> tkMuonMip;
    std::vector<unsigned int> tkMuonRPC;
    std::vector<int> tkMuonBx;
    std::vector<unsigned int> tkMuonQual;
    std::vector<double> tkMuonzVtx;
    std::vector<double> tkMuonMuRefPt;
    std::vector<double> tkMuonTrkRefPt;
    std::vector<double> tkMuonMuRefPhi;
    std::vector<double> tkMuonMuRefEta;
    std::vector<double> tkMuonDRMuTrack;
    std::vector<double> tkMuonNMatchedTracks;
    std::vector<int> tkMuonMuRefChg;
    std::vector<unsigned int> tkMuonRegion;

    unsigned int nTkMuonStubs;
    std::vector<double> tkMuonStubsPt;
    std::vector<double> tkMuonStubsEta;
    std::vector<double> tkMuonStubsPhi;
    std::vector<int> tkMuonStubsChg;
    std::vector<int> tkMuonStubsBx;
    std::vector<double> tkMuonStubsTrkIso;
    std::vector<unsigned int> tkMuonStubsQual;
    std::vector<double> tkMuonStubszVtx;
    std::vector<double> tkMuonStubsBarrelStubs;
    std::vector<unsigned int> tkMuonStubsRegion;

    unsigned int nTkHSCPs;
    std::vector<double> tkHSCPsPt;
    std::vector<double> tkHSCPsEta;
    std::vector<double> tkHSCPsPhi;
    std::vector<int> tkHSCPsChg;
    std::vector<int> tkHSCPsBx;
    std::vector<double> tkHSCPszVtx;

    unsigned int nTrackerMet;
    std::vector<double> trackerMetSumEt;
    std::vector<double> trackerMetEt;
    std::vector<double> trackerMetPhi;
    std::vector<double> trackerMetBx;

    unsigned int nTrackerMHT;
    std::vector<double> trackerHT;
    std::vector<double> trackerMHT;
    std::vector<double> trackerMHTPhi;

    unsigned int nPuppiJets;
    std::vector<double> puppiJetEt;
    std::vector<double> puppiJetEta;
    std::vector<double> puppiJetPhi;
    std::vector<int> puppiJetBx;
    std::vector<double> puppiJetzVtx;
    std::vector<double> puppiJetEtUnCorr;

    /*
    unsigned int nPuppiJetForMETs;
    std::vector<double> puppiJetForMETEt;
    std::vector<double> puppiJetForMETEta;
    std::vector<double> puppiJetForMETPhi;
    std::vector<int>    puppiJetForMETBx;
    std::vector<double> puppiJetForMETzVtx;
    std::vector<double> puppiJetForMETEtUnCorr;
*/

    double puppiMETEt;
    double puppiMETPhi;

    std::vector<double> puppiHT;
    std::vector<double> puppiMHTEt;
    std::vector<double> puppiMHTPhi;
    unsigned int nPuppiMHT;

    unsigned int nPFMuons;
    std::vector<double> pfMuonPt;
    std::vector<double> pfMuonEta;
    std::vector<double> pfMuonPhi;
    std::vector<double> pfMuonzVtx;
    std::vector<int> pfMuonChg;

    unsigned int nPFCands;
    std::vector<int> pfCandId;
    std::vector<double> pfCandEt;
    std::vector<double> pfCandEta;
    std::vector<double> pfCandPhi;
    std::vector<double> pfCandzVtx;
    std::vector<int> pfCandChg;

    unsigned int nPFTaus;
    std::vector<double> pfTauEt;
    std::vector<double> pfTauEta;
    std::vector<double> pfTauPhi;
    std::vector<double> pfTauType;
    std::vector<double> pfTauChargedIso;
    std::vector<unsigned int> pfTauIsoFlag;
    std::vector<unsigned int> pfTauRelIsoFlag;
    std::vector<unsigned int> pfTauPassesMediumIso;
    std::vector<int> pfTauChg;
    std::vector<double> pfTauZ0;

    unsigned int nNNTaus;
    std::vector<double> nnTauEt;
    std::vector<double> nnTauEta;
    std::vector<double> nnTauPhi;
    std::vector<int> nnTauChg;
    std::vector<double> nnTauChargedIso;
    std::vector<double> nnTauFullIso;
    std::vector<unsigned int> nnTauID;
    std::vector<unsigned int> nnTauPassLooseNN;
    std::vector<unsigned int> nnTauPassLoosePF;
    std::vector<unsigned int> nnTauPassTightPF;
    std::vector<unsigned int> nnTauPassTightNN;

    unsigned int nNNTauPFs;
    std::vector<double> nnTauPFEt;
    std::vector<double> nnTauPFEta;
    std::vector<double> nnTauPFPhi;
    std::vector<int> nnTauPFChg;
    std::vector<double> nnTauPFChargedIso;
    std::vector<double> nnTauPFFullIso;
    std::vector<unsigned int> nnTauPFID;
    std::vector<unsigned int> nnTauPFPassLooseNN;
    std::vector<unsigned int> nnTauPFPassLoosePF;
    std::vector<unsigned int> nnTauPFPassTightPF;
    std::vector<unsigned int> nnTauPFPassTightNN;

    unsigned int nHPSTaus;
    std::vector<double> hpsTauEt;
    std::vector<double> hpsTauEta;
    std::vector<double> hpsTauPhi;
    std::vector<int> hpsTauChg;
    std::vector<double> hpsTauPassTightRelIso;
    std::vector<unsigned int> hpsTauType;
    std::vector<double> hpsTauZ0;

    unsigned int nTkBsCands;
    std::vector<double> tkBsCandPt;
    std::vector<double> tkBsCandEta;
    std::vector<double> tkBsCandPhi;
    std::vector<double> tkBsCandMass;
    std::vector<double> tkBsCandPhi1Pt;
    std::vector<double> tkBsCandPhi2Pt;
    std::vector<double> tkBsCandPhi1Eta;
    std::vector<double> tkBsCandPhi2Eta;
    std::vector<double> tkBsCandPhi1Phi;
    std::vector<double> tkBsCandPhi2Phi;
    std::vector<double> tkBsCandPhi1Mass;
    std::vector<double> tkBsCandPhi2Mass;
    std::vector<double> tkBsCandDRPhiPair;
    std::vector<double> tkBsCandDxyPhiPair;
    std::vector<double> tkBsCandDzPhiPair;
    std::vector<int> tkBsCandKind;
  };
}  // namespace L1Analysis
#endif
