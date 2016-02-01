import FWCore.ParameterSet.Config as cms

process = cms.Process('L1EndcapMuonTF')


process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.Geometry.GeometryIdeal_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.load('L1Trigger.L1TMuonEndCap.L1TMuonTriggerPrimitiveProducer_cfi')
process.load('L1Trigger.L1TMuonEndCap.L1CSCTFTrackConverter_cfi')
process.load('L1Trigger.L1TMuonEndCap.L1DTTFTrackConverter_cfi')
process.load('L1Trigger.L1TMuonEndCap.L1RPCTFTrackConverter_cfi')
#process.load('L1Trigger.L1TMuonEndCap.L1TMuonSimpleDeltaEtaHitMatcher_cfi')

from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_mc', '')


process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(21000)
)

#infile = ['file:unganged2.root']
infile = ['file:MuGun_test.root']
#['file:SingleMuFlatPt_5GeVto200GeV_GEN_SIM_DIGI_L1.root']
#['file:SingleMuFlatPt_minusEta_1GeVto200GeV_GEN_SIM_DIGI_L1.root']
#infile.append('file:MuGun_test.root')


#infile.append('file:SingleMuFlatPt_plusEta_1GeVto200GeV_GEN_SIM_DIGI_L1_2.root')
#infile.append('file:SingleMuFlatPt_minusEta_1GeVto200GeV_GEN_SIM_DIGI_L1_2.root')

process.source = cms.Source(
    'PoolSource',
    fileNames = cms.untracked.vstring(infile)
    )

process.L1TMuonSeq = cms.Sequence( process.L1TMuonTriggerPrimitives +
                                   process.L1CSCTFTrackConverter    +
                                   process.L1DTTFTrackConverter     +
                                   process.L1RPCTFTrackConverters   )#+ 
                                   #process.L1TMuonSimpleDeltaEtaHitMatcher )

process.L1TMuonPath = cms.Path(process.L1TMuonSeq)

outCommands = cms.untracked.vstring('drop *')
outCommands.append('keep *_genParticles_*_*')
outCommands.append('keep *_simCsctfDigis_*_*')
outCommands.append('keep *_simDttfDigis_*_*')
outCommands.append('keep *_simRpcTriggerDigis_*_*')
outCommands.append('keep *_simMuonRPCDigis_*_*')
outCommands.append('keep *_simDtTriggerPrimitiveDigis_*_*')
outCommands.append('keep *_simCscTriggerPrimitiveDigis_*_*')
outCommands.append('keep *_L1TMuonTriggerPrimitives_*_*')
outCommands.append('keep *_*Converter_*_*')
outCommands.append('keep *_*Matcher_*_*')

process.FEVTDEBUGoutput = cms.OutputModule(
    "PoolOutputModule",
    splitLevel = cms.untracked.int32(0),
    eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
    outputCommands = outCommands,
    fileName = cms.untracked.string('L1TMuon_Out.root'),
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string(''),
        dataTier = cms.untracked.string('')
    )
)

process.outPath = cms.EndPath(process.FEVTDEBUGoutput)

process.schedule = cms.Schedule(process.L1TMuonPath,
                                process.outPath)
