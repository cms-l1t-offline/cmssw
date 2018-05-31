#ifndef __L1Analysis_L1AnalysisPhaseIIDataFormat_H__
#define __L1Analysis_L1AnalysisPhaseIIDataFormat_H__

//-------------------------------------------------------------------------------
// Created 20/04/2010 - E. Conte, A.C. Le Bihan
// 
// 
// Original code : UserCode/L1TriggerDPG/L1ExtraTreeProducer - Jim Brooke
//-------------------------------------------------------------------------------


#include <vector>

namespace L1Analysis
{
  struct L1AnalysisPhaseIIDataFormat
  {
    L1AnalysisPhaseIIDataFormat(){Reset();};
    ~L1AnalysisPhaseIIDataFormat(){};
    
    void Reset()
    {

      // Phase I objects (Barrel only, no track match)
      nEGs = 0;
      egEt.clear();
      egEta.clear();
      egPhi.clear();
      egIEt.clear();
      egIEta.clear();
      egIPhi.clear();
      egIso.clear();
      egBx.clear();
      egTowerIPhi.clear();
      egTowerIEta.clear();
      egRawEt.clear();
      egIsoEt.clear();
      egFootprintEt.clear();
      egNTT.clear();
      egShape.clear();
      egTowerHoE.clear();

      nTaus = 0;
      tauEt.clear();
      tauEta.clear();
      tauPhi.clear(); 
      tauIEt.clear();
      tauIEta.clear();
      tauIPhi.clear(); 
      tauIso.clear();
      tauBx.clear();
      tauTowerIPhi.clear();
      tauTowerIEta.clear();
      tauRawEt.clear();
      tauIsoEt.clear();
      tauNTT.clear();
      tauHasEM.clear();
      tauIsMerged.clear();
      tauHwQual.clear();

      nJets = 0;
      jetEt.clear();
      jetEta.clear();
      jetPhi.clear();
      jetIEt.clear();
      jetIEta.clear();
      jetIPhi.clear();
      jetBx.clear();
      jetTowerIPhi.clear();
      jetTowerIEta.clear();
      jetRawEt.clear();
      jetSeedEt.clear();
      jetPUEt.clear();
      jetPUDonutEt0.clear();
      jetPUDonutEt1.clear();
      jetPUDonutEt2.clear();
      jetPUDonutEt3.clear();

      nMuons = 0;
      muonEt.clear();
      muonEta.clear();
      muonPhi.clear();
      muonEtaAtVtx.clear();
      muonPhiAtVtx.clear();
      muonIEt.clear();
      muonIEta.clear();
      muonIPhi.clear();
      muonIEtaAtVtx.clear();
      muonIPhiAtVtx.clear();
      muonIDEta.clear();
      muonIDPhi.clear();
      muonChg.clear();
      muonIso.clear();
      muonQual.clear();
      muonTfMuonIdx.clear();
      muonBx.clear();

      nSums = 0;
      sumType.clear();
      sumEt.clear();
      sumPhi.clear();
      sumIEt.clear();
      sumIPhi.clear();
      sumBx.clear(); 

      //Track-Matched EG objects seeded by PhaseI Calo objects TkEM, TkEG
      nTkEG = 0;
      tkEGEt.clear();
      tkEGEta.clear();
      tkEGPhi.clear();
      tkEGBx.clear();
      tkEGTrkIso.clear();
      tkEGzVtx.clear();

      nTkIsoEG = 0;
      tkIsoEGEt.clear();
      tkIsoEGEta.clear();
      tkIsoEGPhi.clear();
      tkIsoEGBx.clear();
      tkIsoEGTrkIso.clear();
      tkIsoEGzVtx.clear();

      nTkEM = 0;
      tkEMEt.clear();
      tkEMEta.clear();
      tkEMPhi.clear();
      tkEMBx.clear();
      tkEMTrkIso.clear();

      // Tyler's Crystal electrons (Barrel) 

      // We need to revise which branches are needed here, this is a starting point 

      nEGCrystal = 0;
      EGCrystalEt.clear();
      EGCrystalEta.clear();
      EGCrystalPhi.clear();
      EGCrystalBx.clear();
      EGCrystalIso.clear();
      EGCrystalzVtx.clear();
       
      nTkEGCrystal = 0;
      tkEGCrystalEt.clear();
      tkEGCrystalEta.clear();
      tkEGCrystalPhi.clear();
      tkEGCrystalBx.clear();
      tkEGCrystalTrkIso.clear();
      tkEGCrystalzVtx.clear();

      nTkEMCrystal = 0;
      tkEMCrystalEt.clear();
      tkEMCrystalEta.clear();
      tkEMCrystalPhi.clear();
      tkEMCrystalBx.clear();
      tkEMCrystalTrkIso.clear();
      tkEMCrystalzVtx.clear();

      // TkTaus
      nTkTau = 0;
      tkTauEt.clear();
      tkTauEta.clear();
      tkTauPhi.clear();
      tkTauBx.clear();
      tkTauTrkIso.clear();
      tkTauzVtx.clear();

      // TkJets
      nTkJets = 0;
      tkJetEt.clear();
      tkJetEta.clear();
      tkJetPhi.clear();
      tkJetBx.clear();
      tkJetzVtx.clear();

      // tkTkMuons
      nTkMuons = 0;
      tkMuonEt.clear();
      tkMuonEta.clear();
      tkMuonPhi.clear();
      tkMuonChg.clear();
      tkMuonTrkIso.clear();
      tkMuonFwd.clear();
      tkMuonMip.clear();
      tkMuonRPC.clear();
      tkMuonBx.clear();
      tkMuonQuality.clear();
      tkMuonzVtx.clear();

      // TkMet
      nTkMet = 0;
      tkEt.clear();
      tkMet.clear();
      tkMetPhi.clear();
      tkMetBx.clear();

      //TkMht
      nTkMht = 0;
      tkHt.clear();
      tkMht.clear();
      tkMhtPhi.clear();
      tkMhtBx.clear();


    }
  
    unsigned short int nEGs;
    std::vector<float> egEt;
    std::vector<float> egEta;
    std::vector<float> egPhi;
    std::vector<short int> egIEt;
    std::vector<short int> egIEta;
    std::vector<short int> egIPhi;
    std::vector<short int> egIso;
    std::vector<short int> egBx;
    std::vector<short int> egTowerIPhi;
    std::vector<short int> egTowerIEta;
    std::vector<short int> egRawEt;
    std::vector<short int> egIsoEt;
    std::vector<short int> egFootprintEt;
    std::vector<short int> egNTT;
    std::vector<short int> egShape;
    std::vector<short int> egTowerHoE;
 
    unsigned short int nTaus;
    std::vector<float> tauEt;
    std::vector<float> tauEta;
    std::vector<float> tauPhi;
    std::vector<short int> tauIEt;
    std::vector<short int> tauIEta;
    std::vector<short int> tauIPhi;
    std::vector<short int> tauIso;
    std::vector<short int> tauBx;
    std::vector<short int> tauTowerIPhi;
    std::vector<short int> tauTowerIEta;
    std::vector<short int> tauRawEt;    
    std::vector<short int> tauIsoEt;
    std::vector<short int> tauNTT;
    std::vector<short int> tauHasEM;
    std::vector<short int> tauIsMerged;
    std::vector<short int> tauHwQual;

    unsigned short int nJets;
    std::vector<float> jetEt;
    std::vector<float> jetEta;
    std::vector<float> jetPhi;
    std::vector<short int> jetIEt;
    std::vector<short int> jetIEta;
    std::vector<short int> jetIPhi;
    std::vector<short int> jetBx;
    std::vector<short int> jetTowerIPhi;
    std::vector<short int> jetTowerIEta;
    std::vector<short int> jetRawEt;    
    std::vector<short int> jetSeedEt;
    std::vector<short int> jetPUEt;
    std::vector<short int> jetPUDonutEt0;
    std::vector<short int> jetPUDonutEt1;
    std::vector<short int> jetPUDonutEt2;
    std::vector<short int> jetPUDonutEt3;

    unsigned short int nMuons;
    std::vector<float>   muonEt;
    std::vector<float>   muonEta;
    std::vector<float>   muonPhi;
    std::vector<float>   muonEtaAtVtx;
    std::vector<float>   muonPhiAtVtx;
    std::vector<short int>   muonIEt;
    std::vector<short int>   muonIEta;
    std::vector<short int>   muonIPhi;
    std::vector<short int>   muonIEtaAtVtx;
    std::vector<short int>   muonIPhiAtVtx;
    std::vector<short int>   muonIDEta;
    std::vector<short int>   muonIDPhi;
    std::vector<short int>      muonChg;
    std::vector<unsigned short int> muonIso;
    std::vector<unsigned short int> muonQual;
    std::vector<unsigned short int> muonTfMuonIdx;
    std::vector<short int>      muonBx;

    unsigned short int nSums;
    std::vector<short int> sumType;
    std::vector<float> sumEt;
    std::vector<float> sumPhi;
    std::vector<short int> sumIEt;
    std::vector<short int> sumIPhi;
    std::vector<float> sumBx;

 
    unsigned int nTkEG;
    std::vector<double> tkEGEt;
    std::vector<double> tkEGEta;
    std::vector<double> tkEGPhi;
    std::vector<int>    tkEGBx;
    std::vector<double> tkEGTrkIso;
    std::vector<double> tkEGzVtx;

    unsigned int nTkIsoEG;
    std::vector<double> tkIsoEGEt;
    std::vector<double> tkIsoEGEta;
    std::vector<double> tkIsoEGPhi;
    std::vector<int>    tkIsoEGBx;
    std::vector<double> tkIsoEGTrkIso;
    std::vector<double> tkIsoEGzVtx;

    unsigned int nTkEM;
    std::vector<double> tkEMEt;
    std::vector<double> tkEMEta;
    std::vector<double> tkEMPhi;
    std::vector<int>    tkEMBx;
    std::vector<double> tkEMTrkIso;

    unsigned int nEGCrystal;
    std::vector<double> EGCrystalEt;
    std::vector<double> EGCrystalEta;
    std::vector<double> EGCrystalPhi;
    std::vector<int>    EGCrystalBx;
    std::vector<double> EGCrystalIso;
    std::vector<double> EGCrystalzVtx;

    unsigned int nTkEGCrystal;
    std::vector<double> tkEGCrystalEt;
    std::vector<double> tkEGCrystalEta;
    std::vector<double> tkEGCrystalPhi;
    std::vector<int>    tkEGCrystalBx;
    std::vector<double> tkEGCrystalTrkIso;
    std::vector<double> tkEGCrystalzVtx;

    unsigned int nTkEMCrystal;
    std::vector<double> tkEMCrystalEt;
    std::vector<double> tkEMCrystalEta;
    std::vector<double> tkEMCrystalPhi;
    std::vector<int>    tkEMCrystalBx;
    std::vector<double> tkEMCrystalTrkIso;
    std::vector<double> tkEMCrystalzVtx;

    unsigned int nTkTau;
    std::vector<double> tkTauEt;
    std::vector<double> tkTauEta;
    std::vector<double> tkTauPhi;
    std::vector<int>    tkTauBx;
    std::vector<double> tkTauTrkIso;
    std::vector<double> tkTauzVtx;

    unsigned int nTkJets;
    std::vector<double> tkJetEt;
    std::vector<double> tkJetEta;
    std::vector<double> tkJetPhi;
    std::vector<int>    tkJetBx;
    std::vector<double> tkJetzVtx;

    unsigned int nTkMuons;
    std::vector<double>   tkMuonEt;
    std::vector<double>   tkMuonEta;
    std::vector<double>   tkMuonPhi;
    std::vector<int>      tkMuonChg;
    std::vector<unsigned int> tkMuonIso;
    std::vector<double> tkMuonTrkIso;
    std::vector<unsigned int> tkMuonFwd;
    std::vector<unsigned int> tkMuonMip;
    std::vector<unsigned int> tkMuonRPC;
    std::vector<int>      tkMuonBx;
    std::vector<unsigned int>      tkMuonQuality;
    std::vector<double>   tkMuonzVtx;

    unsigned int nTkMet;
    std::vector<double> tkEt;
    std::vector<double> tkMet;
    std::vector<double> tkMetPhi;
    std::vector<double> tkMetBx;

    unsigned int nTkMht;
    std::vector<double> tkHt;
    std::vector<double> tkMht;
    std::vector<double> tkMhtPhi;
    std::vector<double> tkMhtBx;



  }; 
}
#endif


