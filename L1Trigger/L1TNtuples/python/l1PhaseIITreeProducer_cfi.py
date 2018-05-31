import FWCore.ParameterSet.Config as cms

l1PhaseIITree = cms.EDAnalyzer("L1PhaseIITreeProducer",
   egToken = cms.untracked.InputTag("simCaloStage2Digis"),
   jetToken = cms.untracked.InputTag("simCaloStage2Digis"),
   muonLegacyToken = cms.untracked.InputTag("muonLegacyInStage2FormatDigis","legacyMuon"),
   muonToken = cms.untracked.InputTag("simGmtStage2Digis"),
   sumToken = cms.untracked.InputTag("simCaloStage2Digis"),
   tauTokens = cms.untracked.VInputTag("simCaloStage2Digis"),
   egCrystalToken = cms.untracked.InputTag("l1EGammaCrystalsProducer","L1EGammaCollectionBXVWithCuts"),

   tkEGToken = cms.InputTag("L1TkElectrons","EG"),
   tkIsoEGToken = cms.InputTag("L1TkIsoElectrons","EG"),
   tkEMToken = cms.InputTag("L1TkPhotons","EG"), 

   tkEGCrystalToken = cms.InputTag("L1TkElectronsCrystal","EG"),
   tkEMCrystalToken = cms.InputTag("L1TkPhotonsCrystal","EG"),

   tkTauToken = cms.InputTag("L1TkTauFromCalo",""), # ?
   tkMuonToken = cms.InputTag("L1TkMuons",""),                                            
   tkJetToken = cms.InputTag("L1TkJets","Central"),                                            
   tkMetToken = cms.InputTag("L1TkEtMiss","MET"),
   tkMhtToken = cms.InputTag("L1TkHTMissVtx",""),

   maxL1Extra = cms.uint32(20)
)

