import FWCore.ParameterSet.Config as cms

process = cms.Process('L1EMTF')
process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 1
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(10))
#process.SimpleMemoryCheck = cms.Service("SimpleMemoryCheck",ignoreTotal = cms.untracked.int32(1))
process.load("L1TriggerConfig.L1ScalesProducers.L1MuTriggerScalesConfig_cff")
process.load("L1TriggerConfig.L1ScalesProducers.L1MuTriggerPtScaleConfig_cff")
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.load('Configuration.Geometry.GeometryExtended2015Reco_cff')
process.load('Configuration.Geometry.GeometryExtended2015_cff')

from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_mc', '')


process.options = cms.untracked.PSet(
    SkipEvent = cms.untracked.vstring('ProductNotFound')
)

process.L1TMuonEndcapTrackFinder = cms.EDProducer(
    'L1TMuonEndCapTrackProducer',
    
	CSCInput = cms.InputTag('simCscTriggerPrimitiveDigis',''),
    primitiveSrcs = cms.VInputTag(
    cms.InputTag('L1TMuonTriggerPrimitives','CSC'),
    cms.InputTag('L1TMuonTriggerPrimitives','DT'),
    cms.InputTag('L1TMuonTriggerPrimitives','RPC')
    ),
   
)

process.content = cms.EDAnalyzer("EventContentAnalyzer")

process.load('Configuration.StandardSequences.SimL1Emulator_cff')
process.load("Configuration.StandardSequences.RawToDigi_cff")

process.L1ReEmulSeq = cms.Sequence(process.SimL1Emulator
                                   + process.ecalDigis
                                   + process.hcalDigis
                                   + process.gtDigis
                                   + process.gtEvmDigis
                                   + process.csctfDigis
                                   + process.dttfDigis
                                   )

infile = [

		'/store/mc/Fall13dr/MuPlus_Pt-1to150_PositiveEndcap-gun/GEN-SIM-RAW/tsg_PU40bx25_POSTLS162_V2-v1/20000/00662BFD-DC81-E311-AB4A-1CC1DE046FC0.root',

		'/store/relval/CMSSW_7_5_0_pre1/RelValSingleMuPt100_UP15/GEN-SIM-DIGI-RECO/MCRUN2_74_V7_FastSim-v1/00000/04DB6E17-72E2-E411-8311-0025905964BA.root',
       '/store/relval/CMSSW_7_5_0_pre1/RelValSingleMuPt100_UP15/GEN-SIM-DIGI-RECO/MCRUN2_74_V7_FastSim-v1/00000/24978F06-72E2-E411-8346-0025905A6084.root',
       '/store/relval/CMSSW_7_5_0_pre1/RelValSingleMuPt100_UP15/GEN-SIM-DIGI-RECO/MCRUN2_74_V7_FastSim-v1/00000/469C811A-72E2-E411-B1EF-0025905A6118.root',
       '/store/relval/CMSSW_7_5_0_pre1/RelValSingleMuPt100_UP15/GEN-SIM-DIGI-RECO/MCRUN2_74_V7_FastSim-v1/00000/AAD41A17-72E2-E411-A617-0025905A607E.root',
       '/store/relval/CMSSW_7_5_0_pre1/RelValSingleMuPt100_UP15/GEN-SIM-DIGI-RECO/MCRUN2_74_V7_FastSim-v1/00000/C4F4E747-71E2-E411-8305-0026189438AB.root' ]


process.source = cms.Source(
    'PoolSource',
    fileNames = cms.untracked.vstring(infile)
    
    )



outCommands = cms.untracked.vstring('keep *')

process.FEVTDEBUGoutput = cms.OutputModule(
    "PoolOutputModule",
    splitLevel = cms.untracked.int32(0),
    eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
    outputCommands = outCommands,
    fileName = cms.untracked.string('Emulator_EDM_Out.root'),
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string(''),
        dataTier = cms.untracked.string('')
    )
)

process.L1TMuonSequence = cms.Sequence(process.L1TMuonEndcapTrackFinder)

process.L1TMuonPath = cms.Path(process.L1ReEmulSeq+process.L1TMuonSequence)

process.outPath = cms.EndPath(process.FEVTDEBUGoutput)

process.schedule = cms.Schedule(process.L1TMuonPath,process.outPath)

from SLHCUpgradeSimulations.Configuration.muonCustoms import customise_csc_PostLS1
process = customise_csc_PostLS1(process)
