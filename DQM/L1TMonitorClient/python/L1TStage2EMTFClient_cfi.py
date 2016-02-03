import FWCore.ParameterSet.Config as cms

l1tStage2EmtfClient = cms.EDAnalyzer(
    'L1TStage2EMTFClient',
    inputDir = cms.untracked.string('L1T2016/L1TStage2EMTF'),
    outputDir = cms.untracked.string('L1T2016/L1TStage2EMTFClient'),
)

