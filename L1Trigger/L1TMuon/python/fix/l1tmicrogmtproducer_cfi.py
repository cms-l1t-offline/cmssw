import FWCore.ParameterSet.Config as cms

import os

microGMTEmulator = cms.EDProducer('L1TMuonProducer',
    barrelTFInput = cms.InputTag("L1TMicroGMTInputProducer", "BarrelTFMuons"),
    overlapTFInput = cms.InputTag("L1TMicroGMTInputProducer", "OverlapTFMuons"),
    forwardTFInput = cms.InputTag("L1TMicroGMTInputProducer", "ForwardTFMuons"),
    triggerTowerInput = cms.InputTag("L1TMicroGMTInputProducer", "TriggerTowerSums"),
)

