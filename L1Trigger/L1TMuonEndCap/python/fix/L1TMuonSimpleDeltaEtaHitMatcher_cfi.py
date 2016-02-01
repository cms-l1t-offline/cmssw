import FWCore.ParameterSet.Config as cms

L1TMuonSimpleDeltaEtaHitMatcher = cms.EDProducer(
    'L1TMuonSimpleDeltaEtaHitMatcher',
    TriggerPrimitiveSrc = cms.InputTag('L1TMuonTriggerPrimitives'),
    genSrc = cms.InputTag('genParticles'),
    DetaWindowSize = cms.double(0.3)
    )
