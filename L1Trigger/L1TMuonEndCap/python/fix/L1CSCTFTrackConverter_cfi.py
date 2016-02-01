import FWCore.ParameterSet.Config as cms

L1CSCTFTrackConverter = cms.EDProducer(
    'L1CSCTFTrackConverter',
    CSCTrackSrc = cms.InputTag('simCsctfTrackDigis',''),
    TriggerPrimitiveSrc = cms.InputTag('L1TMuonTriggerPrimitives','')
    )
