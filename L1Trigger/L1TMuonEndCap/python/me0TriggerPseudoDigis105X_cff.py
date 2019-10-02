import FWCore.ParameterSet.Config as cms

from RecoLocalMuon.GEMRecHit.me0RecHits_cfi import me0RecHits
from RecoLocalMuon.GEMSegment.me0Segments_cfi import me0Segments
from L1Trigger.ME0Trigger.me0TriggerPseudoDigis_cfi import me0TriggerPseudoDigis as me0TriggerPseudoDigis105X

me0TriggerPseudoDigiTask105X = cms.Task(me0RecHits, me0Segments, me0TriggerPseudoDigis105X)
