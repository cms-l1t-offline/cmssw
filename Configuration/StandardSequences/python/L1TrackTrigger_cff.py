import FWCore.ParameterSet.Config as cms

from L1Trigger.TrackTrigger.TrackTrigger_cff import *
from SimTracker.TrackTriggerAssociation.TrackTriggerAssociator_cff import *
from L1Trigger.TrackerDTC.ProducerED_cff import *

L1TrackTrigger=cms.Sequence(TrackTriggerClustersStubs*TrackTriggerAssociatorClustersStubs*TrackerDTCProducer*TrackTriggerTTTracks*TrackTriggerAssociatorTracks)
#L1TrackTrigger=cms.Sequence(TrackTriggerClustersStubs*TrackTriggerAssociatorClustersStubs*TrackerDTCProducer)

TTStubAlgorithm_official_Phase2TrackerDigi_.zMatchingPS = cms.bool(True)
