############################################################
# define basic process
############################################################


### ===> FLAT OR TILTED BARREL GEOMETRY ??? <===
flat=False


import FWCore.ParameterSet.Config as cms
import os
process = cms.Process("L1TrackNtuple")
 
 
############################################################
# import standard configurations
############################################################

process.load('Configuration.StandardSequences.Services_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')

if flat:
	print 'Assuming the flat tracker geometry'
	process.load('L1Trigger.TrackTrigger.TkOnlyFlatGeom_cff')
else:
	print 'Assuming the tilted tracker geometry'
	process.load('L1Trigger.TrackTrigger.TkOnlyTiltedGeom_cff')

process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:upgradePLS3', '')


############################################################
# input and output
############################################################

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(10))
Source_Files = cms.untracked.vstring(
    "file:PGun_example_TkOnly.root"
    )
process.source = cms.Source("PoolSource", fileNames = Source_Files)

process.TFileService = cms.Service("TFileService", fileName = cms.string('ExampleL1StubNtuple.root'), closeFileFast = cms.untracked.bool(True))


############################################################
# example ntuple
############################################################

process.L1StubsExample = cms.EDAnalyzer('L1StubsExample',
                                       L1StubInputTag = cms.InputTag("TTStubsFromPhase2TrackerDigis","StubAccepted"),
                                       MCTruthClusterInputTag = cms.InputTag("TTClusterAssociatorFromPixelDigis", "ClusterAccepted"),
                                       MCTruthStubInputTag = cms.InputTag("TTStubAssociatorFromPixelDigis", "StubAccepted"),
                                       TrackingParticleInputTag = cms.InputTag("mix", "MergedTrackTruth"),
                                       TrackingVertexInputTag = cms.InputTag("mix", "MergedTrackTruth"),
                                       )
process.ana = cms.Path(process.L1StubsExample)

process.schedule = cms.Schedule(process.ana)

