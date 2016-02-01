import FWCore.ParameterSet.Config as cms

from L1Trigger.DTTrackFinder.dttfDigis_cfi import dttfDigis

DTBunchCrossingCleanerCfg = cms.PSet(
    bxWindowSize = cms.int32(1), #look one BX ahead and behind     
    )

L1TMuonTriggerPrimitives = cms.EDProducer(
    'L1TMuonTriggerPrimitiveProducer',
    DT   = cms.PSet( collectorType = cms.string('DTCollector'),
                     src = cms.InputTag('simDtTriggerPrimitiveDigis'),
                     BX_min = cms.int32(dttfDigis.BX_min.value()),
                     BX_max = cms.int32(dttfDigis.BX_max.value()),
                     runBunchCrossingCleaner = cms.bool(True),
                     bxCleanerCfg = DTBunchCrossingCleanerCfg ),
    
    RPC = cms.PSet( collectorType = cms.string('RPCCollector'),
                    src = cms.InputTag('simMuonRPCDigis') ),
    
    CSC  = cms.PSet( collectorType = cms.string('CSCCollector'),
                     src = cms.InputTag('simCscTriggerPrimitiveDigis',
                                        'MPCSORTED') )
    )
