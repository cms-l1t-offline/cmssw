import FWCore.ParameterSet.Config as cms

l1tStage2Emtf = cms.EDAnalyzer(
    'L1TStage2EMTF',
    trackSource = cms.InputTag('simEmtfDigis'),
    outputDir = cms.untracked.string('L1T2016/L1TStage2EMTF'), 
    verbose = cms.untracked.bool(False),
)

