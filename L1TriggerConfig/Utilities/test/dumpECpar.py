import FWCore.ParameterSet.Config as cms

process = cms.Process("L1ConfigWritePayloadDummy")
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cout.placeholder = cms.untracked.bool(False)
process.MessageLogger.cout.threshold = cms.untracked.string('DEBUG')
process.MessageLogger.debugModules = cms.untracked.vstring('*')

process.source = cms.Source("EmptySource")
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1) )

process.load('L1Trigger.L1TMuonEndCap.fakeEmtfParams_cff')
process.getter = cms.EDAnalyzer("EventSetupRecordDataGetter",
   toGet = cms.VPSet(
      cms.PSet(record = cms.string('L1TMuonEndCapParamsRcd'),
               data = cms.vstring('L1TMuonEndCapParams')),
      cms.PSet(record = cms.string('L1TMuonEndCapForestRcd'),
               data = cms.vstring('L1TMuonEndCapForest'))
                   ),

   verbose = cms.untracked.bool(True)
)

process.l1ecw = cms.EDAnalyzer("L1TMuonEndcapWriter")

from CondCore.DBCommon.CondDBSetup_cfi import CondDBSetup
outputDB = cms.Service("PoolDBOutputService",
                       CondDBSetup,
                       connect = cms.string('sqlite_file:l1config.db'),
                       toPut   = cms.VPSet(
                           cms.PSet(
                               record = cms.string('L1TMuonEndcapParamsRcd'),
                               tag = cms.string("dummy")
                           ),
                           cms.PSet(
                               record = cms.string('L1TMuonEndCapForestRcd'),
                               tag = cms.string("dummy2")
                           )
                       )
)
outputDB.DBParameters.authenticationPath = '.'
process.add_(outputDB)

process.p = cms.Path(process.getter + process.l1ecw)
