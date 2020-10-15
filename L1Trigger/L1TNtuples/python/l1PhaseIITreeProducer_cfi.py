import FWCore.ParameterSet.Config as cms

l1PhaseIITree = cms.EDAnalyzer("L1PhaseIITreeProducer",

   muonToken = cms.untracked.InputTag("simGmtStage2Digis"),

   egTokenBarrel = cms.InputTag("L1EGammaClusterEmuProducer",""),
   ######tkEGTokenBarrel = cms.InputTag("L1TkElectronsCrystal","EG"), ##REMOVED
   tkEGTokenBarrel = cms.InputTag("L1TkElectronsEllipticMatchCrystal","EG"),
   tkEMTokenBarrel = cms.InputTag("L1TkPhotonsCrystal","EG"),

   egTokenHGC = cms.InputTag("l1EGammaEEProducer","L1EGammaCollectionBXVWithCuts"),
   ########tkEGTokenHGC = cms.InputTag("L1TkElectronsHGC","EG"), ###REMOVED
   tkEGTokenHGC = cms.InputTag("L1TkElectronsEllipticMatchHGC","EG"),
   tkEMTokenHGC = cms.InputTag("L1TkPhotonsHGC","EG"),

   tkTauToken     = cms.InputTag("L1TrackerTaus","TrkTau"), # ?
   caloTkTauToken = cms.InputTag("L1TkCaloTaus","CaloTk"), # ?
   tkEGTauToken   = cms.InputTag("L1TkEGTaus","TkEG"), # ?

   TkGlbMuonToken = cms.InputTag("L1TkGlbMuons",""),
   TkMuonToken = cms.InputTag("L1TkMuons",""),

#   TkMuonStubsTokenBMTF = cms.InputTag("l1StubMatchedMuons",""),  # These are not in 11_1_2 yet
#   TkMuonStubsTokenEMTF = cms.InputTag("l1TkMuonStubEndCapS12",""),
#   TkMuonStubsTokenOMTF = cms.InputTag("l1TkMuonStubOverlap","MuonTracks"),
#   TkMuonStubsTokenME0 = cms.InputTag("l1TkMuonStubEndCap","ME0Ext"),
#   TkMuonStubsTokenHSCP = cms.InputTag("l1TkMuonStubOverlap","HscpTracks"),

   tkTrackerJetToken = cms.InputTag("TwoLayerJets", "L1TwoLayerJets"),
   tkCaloJetToken = cms.InputTag("L1TkCaloJets","L1TkCaloJets"),
   tkMetToken = cms.InputTag("L1TrackerEtMiss","trkMET"),
   tkMhtTokens = cms.VInputTag( cms.InputTag("L1TrackerHTMiss","L1TrackerHTMiss") ),

   ak4L1PF = cms.InputTag("ak4PFL1PuppiCorrected"),

#   l1pfPhase1L1TJetToken  = cms.InputTag("Phase1L1TJetCalibrator" ,   "Phase1L1TJetFromPfCandidates"), # not there yet either

   l1PFCandidates = cms.InputTag("l1pfCandidates","Puppi"),

   caloTauToken = cms.InputTag("L1CaloJetProducer","L1CaloTauCollectionBXV"),
   caloJetToken = cms.InputTag("L1CaloJetProducer","L1CaloJetCollectionBXV"),
   caloJetHTTToken = cms.InputTag("L1CaloJetHTTProducer","CaloJetHTT"),

   muonKalman = cms.InputTag("simKBmtfDigis","BMTF"),
   muonOverlap = cms.InputTag("simOmtfDigis","OMTF"),
   muonEndcap = cms.InputTag("simEmtfDigis","EMTF"),

   l1PFMet = cms.InputTag("l1PFMetPuppi"),

   zoPuppi = cms.InputTag("l1pfProducerBarrel","z0"),
   l1vertextdr = cms.InputTag("VertexProducer","l1vertextdr"),
   l1vertices = cms.InputTag("VertexProducer","l1vertices"),
   l1TkPrimaryVertex= cms.InputTag("L1TkPrimaryVertex",""),

#   L1PFTauToken = cms.InputTag("l1pfTauProducer","L1PFTaus"),  # not there
   L1NNTauToken = cms.InputTag("l1NNTauProducerPuppi","L1PFTausNN"),
   L1NNTauPFToken = cms.InputTag("l1NNTauProducer","L1PFTausNN"),

#   L1HPSPFTauToken = cms.InputTag("L1HPSPFTauProducerPF",""),

   L1TkBsCandsToken = cms.InputTag("l1TkBsCandidates"),
   L1TkBsCandsLooseToken = cms.InputTag("l1TkBsCandidatesLooseWP"),
   L1TkBsCandsTightToken = cms.InputTag("l1TkBsCandidatesTightWP"),

   maxL1Extra = cms.uint32(50)
)

#### Gen level tree

from L1Trigger.L1TNtuples.l1GeneratorTree_cfi  import l1GeneratorTree
genTree=l1GeneratorTree.clone()

runmenutree=cms.Path(l1PhaseIITree*genTree)





