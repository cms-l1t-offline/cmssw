import FWCore.ParameterSet.Config as cms

simDisplacedGmtStage2Digis = cms.EDProducer(
    "L1TDisplacedMuonProducer",
    bmtfTag = cms.InputTag("simBmtfDigis","BMTF"),
    omtfTag = cms.InputTag("simOmtfDigis","OMTF"),
    emtfTag = cms.InputTag("simEmtfDigis","EMTF"),

    bmtfTrackTag = cms.InputTag("simBmtfDigis"),
    omtfTrackTag = cms.InputTag("simOmtfDigis"),
    emtfTrackTag = cms.InputTag("simEmtfDigis"),

    cscLctTag = cms.InputTag("simCscTriggerPrimitiveDigis","MPCSORTED"),
    me0TriggerTag = cms.InputTag("me0TriggerDigis"),
    me0SegmentTag = cms.InputTag("me0TriggerPseudoDigis"),
    padTag = cms.InputTag("simMuonGEMPadDigis"),
    copadTag = cms.InputTag("simCscTriggerPrimitiveDigis"),
    cscCompTag = cms.InputTag("simMuonCSCDigis","MuonCSCComparatorDigi"),
    ## options to use extra hit information
    useGE11 = cms.bool(False),
    useGE21 = cms.bool(False),
    useME0 = cms.bool(False),
    ## 1: position based
    ## 2: direction based
    ## 3: hybrid
    method = cms.uint32(0),
    fitComparatorDigis = cms.bool(False),
    ## match to stubs
    recoverLCT = cms.bool(False),
    recoverME0 = cms.bool(False),
)

