import FWCore.ParameterSet.Config as cms

from L1Trigger.DTTrackFinder.dttfDigis_cfi import dttfDigis

L1DTTFTrackConverter = cms.EDProducer(
    'L1DTTFTrackConverter',
    DTTrackSrc = cms.InputTag('simDttfDigis','DTTF'),
    TriggerPrimitiveSrc = cms.InputTag('L1TMuonTriggerPrimitives',''),
    BX_min = cms.int32(dttfDigis.BX_min.value()),
    BX_max = cms.int32(dttfDigis.BX_max.value()) 
    )
