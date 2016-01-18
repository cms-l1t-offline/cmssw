import FWCore.ParameterSet.Config as cms

###OMTF ConfFormats are loaded from sqlite file
'''
from CondCore.DBCommon.CondDBCommon_cfi import *
CondDBCommon.connect = 'sqlite_file:Patterns_ipt6_31_750_4x.db'
PoolDBESSource = cms.ESSource("PoolDBESSource",
    process.CondDBCommon,
    DumpStat=cms.untracked.bool(True),
    toGet = cms.VPSet(cms.PSet(
        record = cms.string('L1TMTFOverlapParamsRcd'),
        tag = cms.string('OMTFParams_test')
    )),
)
'''

###OMTF CondFormats ESProducer. ESSetup is filled from XML file
from L1Trigger.L1TMuonOverlap.omtfParams_cfi import *

###OMTF emulator configuration
from L1Trigger.L1TMuonOverlap.OMTFProducer_cfi import *

