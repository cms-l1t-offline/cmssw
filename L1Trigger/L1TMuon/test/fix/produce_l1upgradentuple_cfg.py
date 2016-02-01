import FWCore.ParameterSet.Config as cms
process = cms.Process("L1TMuonEmulation")
import os
import sys
import commands

process.load("FWCore.MessageLogger.MessageLogger_cfi")


VERBOSE = False
SAMPLE = "zmumu"  # "relval"##"minbias"
EDM_OUT = True
# min bias: 23635 => 3477 passed L1TMuonFilter (~6.7%), zmumu ~84%
NEVENTS = 50
if VERBOSE:
    process.MessageLogger = cms.Service("MessageLogger",
                                        suppressInfo=cms.untracked.vstring('AfterSource', 'PostModule'),
                                        destinations=cms.untracked.vstring('detailedInfo', 'critical', 'cout'),
                                        categories=cms.untracked.vstring(
                                            'CondDBESSource', 'EventSetupDependency', 'Geometry', 'MuonGeom', 'GetManyWithoutRegistration', 'GetByLabelWithoutRegistration', 'Alignment', 'SiStripBackPlaneCorrectionDepESProducer', 'SiStripLorentzAngleDepESProducer', 'SiStripQualityESProducer', 'TRACKER', 'HCAL'
                                        ),
                                        critical=cms.untracked.PSet(
                                            threshold=cms.untracked.string('ERROR')
                                        ),
                                        detailedInfo=cms.untracked.PSet(
                                            threshold=cms.untracked.string('INFO'),
                                            CondDBESSource=cms.untracked.PSet(limit=cms.untracked.int32(0)),
                                            EventSetupDependency=cms.untracked.PSet(limit=cms.untracked.int32(0)),
                                            Geometry=cms.untracked.PSet(limit=cms.untracked.int32(0)),
                                            MuonGeom=cms.untracked.PSet(limit=cms.untracked.int32(0)),
                                            Alignment=cms.untracked.PSet(limit=cms.untracked.int32(0)),
                                            GetManyWithoutRegistration=cms.untracked.PSet(limit=cms.untracked.int32(0)),
                                            GetByLabelWithoutRegistration=cms.untracked.PSet(limit=cms.untracked.int32(0))

                                        ),
                                        cout=cms.untracked.PSet(
                                            threshold=cms.untracked.string('WARNING'),
                                            CondDBESSource=cms.untracked.PSet(limit=cms.untracked.int32(0)),
                                            EventSetupDependency=cms.untracked.PSet(limit=cms.untracked.int32(0)),
                                            Geometry=cms.untracked.PSet(limit=cms.untracked.int32(0)),
                                            MuonGeom=cms.untracked.PSet(limit=cms.untracked.int32(0)),
                                            Alignment=cms.untracked.PSet(limit=cms.untracked.int32(0)),
                                            GetManyWithoutRegistration=cms.untracked.PSet(limit=cms.untracked.int32(0)),
                                            GetByLabelWithoutRegistration=cms.untracked.PSet(limit=cms.untracked.int32(0))
                                        ),
                                        )

if not VERBOSE:
    process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(1000)

process.options = cms.untracked.PSet(wantSummary=cms.untracked.bool(True))

fnames = ['/store/relval/CMSSW_7_5_0_pre1/RelValSingleMuPt10_UP15/GEN-SIM-DIGI-RAW-HLTDEBUG/MCRUN2_74_V7-v1/00000/16BF2D14-83E3-E411-B212-003048FFD756.root',
          '/store/relval/CMSSW_7_5_0_pre1/RelValSingleMuPt10_UP15/GEN-SIM-DIGI-RAW-HLTDEBUG/MCRUN2_74_V7-v1/00000/26833213-83E3-E411-9238-0025905B8590.root',
          '/store/relval/CMSSW_7_5_0_pre1/RelValSingleMuPt10_UP15/GEN-SIM-DIGI-RAW-HLTDEBUG/MCRUN2_74_V7-v1/00000/5E967412-83E3-E411-9DA0-003048FFD756.root',
          '/store/relval/CMSSW_7_5_0_pre1/RelValSingleMuPt10_UP15/GEN-SIM-DIGI-RAW-HLTDEBUG/MCRUN2_74_V7-v1/00000/686FB705-83E3-E411-A8FC-003048FF9AC6.root',
          '/store/relval/CMSSW_7_5_0_pre1/RelValSingleMuPt10_UP15/GEN-SIM-DIGI-RAW-HLTDEBUG/MCRUN2_74_V7-v1/00000/8E6F7913-83E3-E411-B72F-0025905A48BB.root']

if SAMPLE == "zmumu":
#    fnames = ['root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/20000/B61E1FCD-A077-E311-8B65-001E673974EA.root',
#              'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/10000/0023D81B-2980-E311-85A1-001E67398C0F.root',
#              'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/10000/248FB042-3080-E311-A346-001E67397D00.root']
    fnames = ['/store/mc/Phys14DR/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/PU20bx25_tsg_castor_PHYS14_25_V1-v1/10000/044B58B4-9D75-E411-AB6C-002590A83218.root',
              '/store/mc/Phys14DR/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/PU20bx25_tsg_castor_PHYS14_25_V1-v1/10000/045570BC-9175-E411-A06A-002590A887F0.root',
              '/store/mc/Phys14DR/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/PU20bx25_tsg_castor_PHYS14_25_V1-v1/10000/0C8C76BC-9775-E411-882E-002481E0DCD8.root',]
elif SAMPLE == "minbias":
    fnames = ['root://xrootd.unl.edu//store/mc/Fall13dr/Neutrino_Pt-2to20_gun/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/00000/00276D94-AA88-E311-9C90-0025905A6060.root',
              'root://xrootd.unl.edu//store/mc/Fall13dr/Neutrino_Pt-2to20_gun/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/00000/004F8058-6F88-E311-B971-0025905A6094.root',
              'root://xrootd.unl.edu//store/mc/Fall13dr/Neutrino_Pt-2to20_gun/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/00000/005C8F98-C288-E311-ADF1-0026189438BD.root',
              'root://xrootd.unl.edu//store/mc/Fall13dr/Neutrino_Pt-2to20_gun/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/00000/006A1FB8-7D88-E311-B61B-0025905A60A0.root']

process.source = cms.Source(
    'PoolSource',
    fileNames=cms.untracked.vstring(
        fnames
    )
)

process.maxEvents = cms.untracked.PSet(input=cms.untracked.int32(NEVENTS))

# PostLS1 geometry used
process.load('Configuration.Geometry.GeometryExtended2015Reco_cff')
process.load('Configuration.Geometry.GeometryExtended2015_cff')
############################

process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')
from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_mc', '')
process.load('L1Trigger.L1TMuonEndCap.L1TMuonTriggerPrimitiveProducer_cfi')

path = "L1Trigger/L1TMuonOverlap/data/"
# OMTF emulator configuration
process.load('L1Trigger.L1TMuonOverlap.OMTFProducer_cff')

process.L1TMuonEndcapTrackFinder = cms.EDProducer(
    'L1TMuonEndCapTrackProducer',
    CSCInput = cms.InputTag('simCscTriggerPrimitiveDigis',''),
    primitiveSrcs = cms.VInputTag(
    cms.InputTag('L1TMuonTriggerPrimitives', 'CSC'),
    cms.InputTag('L1TMuonTriggerPrimitives', 'DT'),
    cms.InputTag('L1TMuonTriggerPrimitives', 'RPC')
    ),
)

# TwinMux Emulator
process.load('L1Trigger.L1TMuonBarrel.L1TTwinMuxProducer_cfi')

# BMTF Emulator
process.load('L1Trigger.L1TMuonBarrel.l1tmbtfparamsproducer_cfi')
process.load('L1Trigger.L1TMuonBarrel.bmtfDigis_cfi')
process.bmtfDigis.DTDigi_Source=cms.InputTag("L1TTwinMuxProducer")

process.MicroGMTCaloInputProducer = cms.EDProducer("L1TMuonCaloSumProducer",
                                               caloStage2Layer2Label=cms.InputTag("caloStage2Layer1Digis"),
)
# WORKAROUNDS FOR WRONG SCALES / MISSING COLLECTIONS:
#process.bmtfConverter = cms.EDProducer("L1TBMTFConverter",
#                                       barrelTFInput = cms.InputTag("bmtfDigis", "BM"))

# Adjust input tags if running on GEN-SIM-RAW (have to re-digi)
if SAMPLE == "zmumu" or SAMPLE == "minbias":
    process.L1TMuonTriggerPrimitives.CSC.src = cms.InputTag('simCscTriggerPrimitiveDigis')

process.L1MuonFilter = cms.EDFilter("SelectL1Muons",)
process.GenMuonFilter = cms.EDFilter("SelectGenMuons",)

process.load("L1TriggerDPG.L1Ntuples.l1NtupleProducer_cfi")
process.load("L1TriggerDPG.L1Ntuples.l1RecoTreeProducer_cfi")
process.load("L1TriggerDPG.L1Ntuples.l1ExtraTreeProducer_cfi")
process.load("L1TriggerDPG.L1Ntuples.l1MuonRecoTreeProducer_cfi")
process.load("L1TriggerDPG.L1Ntuples.l1MuonUpgradeTreeProducer_cfi")

# uGMT emulator
process.load("L1Trigger.L1TMuon.l1tmicrogmtproducer_cfi")

process.microGMTEmulator.overlapTFInput = cms.InputTag("omtfEmulator", "OMTF")
process.l1MuonUpgradeTreeProducer.omtfTag = cms.InputTag("omtfEmulator", "OMTF")
process.microGMTEmulator.forwardTFInput = cms.InputTag("L1TMuonEndcapTrackFinder", "EMUTF")
process.l1MuonUpgradeTreeProducer.emtfTag = cms.InputTag("L1TMuonEndcapTrackFinder", "EMUTF")
#process.microGMTEmulator.barrelTFInput = cms.InputTag("bmtfConverter", "ConvBMTFMuons")
#process.l1MuonUpgradeTreeProducer.bmtfTag = cms.InputTag("bmtfConverter", "ConvBMTFMuons")
process.microGMTEmulator.barrelTFInput = cms.InputTag("bmtfDigis", "BM")
process.l1MuonUpgradeTreeProducer.bmtfTag = cms.InputTag("bmtfDigis", "BM")
process.microGMTEmulator.triggerTowerInput = cms.InputTag("MicroGMTCaloInputProducer", "TriggerTowerSums")
process.l1MuonUpgradeTreeProducer.calo2x2Tag = cms.InputTag("MicroGMTCaloInputProducer", "TriggerTower2x2s")
process.l1MuonUpgradeTreeProducer.caloTag = cms.InputTag("caloStage2Layer1Digis")
process.l1MuonUpgradeTreeProducer.caloRecoTag = cms.InputTag("none")

# output file
process.TFileService = cms.Service("TFileService",
                                   fileName=cms.string(
                                       '/afs/cern.ch/work/t/treis/private/l1ntuples_upgrade/l1ntuple_{sample}_n.root'.format(sample=SAMPLE))
                                   )

process.load('Configuration.StandardSequences.SimL1Emulator_cff')
process.load("Configuration.StandardSequences.RawToDigi_cff")
from SLHCUpgradeSimulations.Configuration.muonCustoms import customise_csc_PostLS1
process = customise_csc_PostLS1(process)

# upgrade calo stage 2
process.load('L1Trigger.L1TCalorimeter.L1TCaloStage2_PPFromRaw_cff')

# L1TMicroGMTESProducer
process.load('L1Trigger.L1TMuon.l1tmicrogmtparamsesproducer_cfi')
# reset LUT paths to trigger CMSSW internal LUT generation
#process.l1tGMTParamsESProducer.AbsIsoCheckMemLUTPath = cms.string('')
#process.l1tGMTParamsESProducer.RelIsoCheckMemLUTPath = cms.string('')
#process.l1tGMTParamsESProducer.IdxSelMemPhiLUTPath = cms.string('')
#process.l1tGMTParamsESProducer.IdxSelMemEtaLUTPath = cms.string('')
process.l1tGMTParamsESProducer.BrlSingleMatchQualLUTPath = cms.string('')
process.l1tGMTParamsESProducer.FwdPosSingleMatchQualLUTPath = cms.string('')
process.l1tGMTParamsESProducer.FwdNegSingleMatchQualLUTPath = cms.string('')
process.l1tGMTParamsESProducer.OvlPosSingleMatchQualLUTPath = cms.string('')
process.l1tGMTParamsESProducer.OvlNegSingleMatchQualLUTPath = cms.string('')
process.l1tGMTParamsESProducer.BOPosMatchQualLUTPath = cms.string('')
process.l1tGMTParamsESProducer.BONegMatchQualLUTPath = cms.string('')
process.l1tGMTParamsESProducer.FOPosMatchQualLUTPath = cms.string('')
process.l1tGMTParamsESProducer.FONegMatchQualLUTPath = cms.string('')
#process.l1tGMTParamsESProducer.BPhiExtrapolationLUTPath = cms.string('')
#process.l1tGMTParamsESProducer.OPhiExtrapolationLUTPath = cms.string('')
#process.l1tGMTParamsESProducer.FPhiExtrapolationLUTPath = cms.string('')
#process.l1tGMTParamsESProducer.BEtaExtrapolationLUTPath = cms.string('')
#process.l1tGMTParamsESProducer.OEtaExtrapolationLUTPath = cms.string('')
#process.l1tGMTParamsESProducer.FEtaExtrapolationLUTPath = cms.string('')
#process.l1tGMTParamsESProducer.SortRankLUTPath = cms.string('')

process.esTest = cms.EDAnalyzer("EventSetupRecordDataGetter",
   toGet = cms.VPSet(
      cms.PSet(record = cms.string('L1TMuonGlobalParamsRcd'),
               data = cms.vstring('L1TMuonGlobalParams'))
                    ),
   verbose = cms.untracked.bool(True)
)

# analysis
process.l1NtupleProducer.hltSource = cms.InputTag("none")
process.l1NtupleProducer.gtSource = cms.InputTag("none")
process.l1NtupleProducer.gctCentralJetsSource = cms.InputTag("none")
process.l1NtupleProducer.gctNonIsoEmSource = cms.InputTag("none")
process.l1NtupleProducer.gctForwardJetsSource = cms.InputTag("none")
process.l1NtupleProducer.gctIsoEmSource = cms.InputTag("none")
process.l1NtupleProducer.gctEnergySumsSource = cms.InputTag("none")
process.l1NtupleProducer.gctTauJetsSource = cms.InputTag("none")
process.l1NtupleProducer.gctIsoTauJetsSource = cms.InputTag("none")
process.l1NtupleProducer.rctSource = cms.InputTag("none")
process.l1NtupleProducer.dttfSource = cms.InputTag("none")
process.l1NtupleProducer.ecalSource = cms.InputTag("none")
process.l1NtupleProducer.hcalSource = cms.InputTag("none")
process.l1NtupleProducer.csctfTrkSource = cms.InputTag("none")
process.l1NtupleProducer.csctfLCTSource = cms.InputTag("none")
process.l1NtupleProducer.csctfStatusSource = cms.InputTag("none")
process.l1NtupleProducer.generatorSource = cms.InputTag("genParticles")
process.l1NtupleProducer.csctfDTStubsSource = cms.InputTag("none")


process.L1ReEmulSeq = cms.Sequence(process.SimL1Emulator
                                   + process.ecalDigis
                                   + process.hcalDigis
                                   + process.gtDigis
                                   + process.gtEvmDigis
                                   + process.csctfDigis
                                   + process.dttfDigis
                                   )

process.L1NtupleSeq = cms.Sequence(process.l1NtupleProducer + process.l1MuonUpgradeTreeProducer)
    # +process.l1extraParticles
    # +process.l1ExtraTreeProducer
    # +process.l1GtTriggerMenuLite
    # +process.l1MenuTreeProducer
    # +process.l1RecoTreeProducer
    # +process.l1MuonRecoTreeProducer

process.L1TMuonSeq = cms.Sequence(
    process.L1TMuonTriggerPrimitives
    + process.L1TTwinMuxProducer
    + process.bmtfDigis
    #+ process.bmtfConverter
    + process.omtfEmulator
    + process.L1TMuonEndcapTrackFinder
    + process.L1TCaloStage2_PPFromRaw
    + process.MicroGMTCaloInputProducer
    + process.microGMTEmulator
)

  # type: L1MuDTChambPhContainer
  # module label: simDtTriggerPrimitiveDigis
  # product instance name: ''
  # process name: ''
process.MuonFilter = cms.Sequence()


if SAMPLE == "minbias":
    process.MuonFilter = cms.Sequence(process.L1MuonFilter)
else:
    process.MuonFilter = cms.Sequence(process.GenMuonFilter)

process.L1TMuonPath = cms.Path(process.L1ReEmulSeq + process.L1TMuonSeq + process.MuonFilter * process.L1NtupleSeq)

process.out = cms.OutputModule("PoolOutputModule",
                               outputCommands=cms.untracked.vstring(
                                   'drop *',
                                   'keep *_*_*_L1TMuonEmulation'),
                               fileName=cms.untracked.string("l1tmuon_test.root"),
                               )


process.schedule = cms.Schedule(process.L1TMuonPath)
if EDM_OUT:
    process.output_step = cms.EndPath(process.out)
    process.schedule.extend([process.output_step])
