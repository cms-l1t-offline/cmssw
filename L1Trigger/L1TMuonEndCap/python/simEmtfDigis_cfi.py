import FWCore.ParameterSet.Config as cms

###EMTF emulator configuration
simEmtfDigis = cms.EDProducer("L1TMuonEndCapTrackProducer",
                              ## CSCInput = cms.InputTag('simCscTriggerPrimitiveDigis','MPCSORTED'),
                              ## isData = cms.bool(False)
                              CSCInput = cms.InputTag('csctfDigis'),
                              isData = cms.bool(True)
                              )
