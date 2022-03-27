import FWCore.ParameterSet.Config as cms

L1TrackingParticleJets = cms.EDProducer("L1TrackingParticleJetProducer",
    TrackingParticleInputTag = cms.InputTag("mix", "MergedTrackTruth"),
    MCTruthStubInputTag = cms.InputTag("TTStubAssociatorFromPixelDigis", "StubAccepted"),
    DebugMode = cms.bool(False),      # printout lots of debug statements
    TP_minNStub = cms.int32(4),       # require TP to have >= X number of stubs associated with it
    TP_minNStubLayer = cms.int32(-1),  # require TP to have stubs in >= X layers/disks
    TP_minPt = cms.double(2.0),       # only save TPs with pt > X GeV
    TP_maxEta = cms.double(2.5),      # only save TPs with |eta| < X
    TP_maxZ0 = cms.double(15.0),      # only save TPs with |z0| < X cm
    coneSize=cms.double(0.4),         #cone size for anti-kt fast jet
    displaced = cms.bool(False)       # use prompt/displaced tracks
)

L1TrackingParticleJetsExtended = cms.EDProducer("L1TrackingParticleJetProducer",
    TrackingParticleInputTag = cms.InputTag("mix", "MergedTrackTruth"),
    MCTruthStubInputTag = cms.InputTag("TTStubAssociatorFromPixelDigis", "StubAccepted"),
    DebugMode = cms.bool(False),      # printout lots of debug statements
    TP_minNStub = cms.int32(4),       # require TP to have >= X number of stubs associated with it
    TP_minNStubLayer = cms.int32(-1),  # require TP to have stubs in >= X layers/disks
    TP_minPt = cms.double(2.0),       # only save TPs with pt > X GeV
    TP_maxEta = cms.double(2.5),      # only save TPs with |eta| < X
    TP_maxZ0 = cms.double(15.0),      # only save TPs with |z0| < X cm
    coneSize=cms.double(0.4),         #cone size for anti-kt fast jet
    displaced = cms.bool(True)       # use prompt/displaced tracks
)
