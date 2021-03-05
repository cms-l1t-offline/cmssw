import FWCore.ParameterSet.Config as cms

simEmtfShowersMC = cms.EDProducer("L1TMuonEndCapShowerProducer",
    # Verbosity level
    verbosity = cms.untracked.int32(0),

    CSCShowerInput = cms.InputTag('simCscTriggerPrimitiveDigis'),
    spShowerParams = cms.PSet(
        ## these are different shower selections that can be enabled
        enableOneNominal = cms.bool(True),
        enableTwoLoose = cms.bool(True),
    )
)

## this producer would use unpacked CSC Showers
## not available yet
simEmtfShowersData = simEmtfShowersMC.clone()

simEmtfShowers = simEmtfShowersMC.clone()
