#########################
#
# Configuration file for:
#  1. rerun L1 Stage2 emulation
#  2. produce new Tracking Stubs and Clusters
#  3. produce new Tracklet TrackTriggerAssociator from new Stubs and Clusters
#  4. produce new HGCal Clusters
#
#
#
# Author: Vladimir.Rekovic@cern.ch
# Date  : 10/03/2017
#
# Script tested with release CMSSW_9_0_0_pre6
#
#########################

# Started from Auto generated configuration file
# using: 
# Revision: 1.19 
# Source: /local/reps/CMSSW/CMSSW/Configuration/Applications/python/ConfigBuilder.py,v 
# with command line options: step2 --python_filename=rerun_step2_L1_L1TrackTrigger_onMCL1_FEVTHLTDEBUG.py --no_exec -s L1,L1TrackTrigger --datatier GEN-SIM-DIGI-RAW -n 1 --era Phase2C2 --eventcontent FEVTDEBUGHLT --pileup AVE_200_BX_25ns --filein file:/afs/cern.ch/user/r/rekovic/release/CMSSW_9_0_0_pre2_patch_Phase2-L1T-MC-test-v2/src/step2_ZEE_PU200_1ev-PR_EventContent-FEVTDEBUGHLT.root --conditions 90X_upgrade2023_realistic_v1 --era Phase2C2_timing --beamspot HLLHC14TeV --geometry Extended2023D4 --fileout file:step2_ZEE_PU200_1ev_rerun-L1-L1TrackTrigger.root
import FWCore.ParameterSet.Config as cms

from Configuration.StandardSequences.Eras import eras

process = cms.Process('RERUNL1',eras.Phase2C2_timing)

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mix_POISSON_average_cfi')
process.load('Configuration.Geometry.GeometryExtended2023D4Reco_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('Configuration.StandardSequences.SimL1Emulator_cff')
process.load('Configuration.StandardSequences.L1TrackTrigger_cff')
process.load('SimCalorimetry.HcalTrigPrimProducers.hcalTTPDigis_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

# Input source
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('root://cms-xrd-global.cern.ch///store/mc/PhaseIISpring17D/SingleNeutrino/GEN-SIM-DIGI-RAW/PU200_90X_upgrade2023_realistic_v9-v1/70000/004C6E8B-5F26-E711-AE46-0242AC130002.root'
    ,'root://cms-xrd-global.cern.ch///store/mc/PhaseIISpring17D/SingleNeutrino/GEN-SIM-DIGI-RAW/PU200_90X_upgrade2023_realistic_v9-v1/70000/00676CC3-5926-E711-9B46-0242AC130004.root'
    ,'root://cms-xrd-global.cern.ch///store/mc/PhaseIISpring17D/SingleNeutrino/GEN-SIM-DIGI-RAW/PU200_90X_upgrade2023_realistic_v9-v1/70000/009A927B-5726-E711-AC20-0242AC130002.root'
    ,'root://cms-xrd-global.cern.ch///store/mc/PhaseIISpring17D/SingleNeutrino/GEN-SIM-DIGI-RAW/PU200_90X_upgrade2023_realistic_v9-v1/70000/00AF6CB4-5226-E711-BC7C-0242AC130004.root'
    ,'root://cms-xrd-global.cern.ch///store/mc/PhaseIISpring17D/SingleNeutrino/GEN-SIM-DIGI-RAW/PU200_90X_upgrade2023_realistic_v9-v1/70000/00BFEB6C-6526-E711-ABE5-0242AC130005.root'
    ,'root://cms-xrd-global.cern.ch///store/mc/PhaseIISpring17D/SingleNeutrino/GEN-SIM-DIGI-RAW/PU200_90X_upgrade2023_realistic_v9-v1/70000/00F02746-6A26-E711-9B33-0242AC130004.root'
    ,'root://cms-xrd-global.cern.ch///store/mc/PhaseIISpring17D/SingleNeutrino/GEN-SIM-DIGI-RAW/PU200_90X_upgrade2023_realistic_v9-v1/70000/020F1518-6C26-E711-A67C-0242AC130004.root'
    ,'root://cms-xrd-global.cern.ch///store/mc/PhaseIISpring17D/SingleNeutrino/GEN-SIM-DIGI-RAW/PU200_90X_upgrade2023_realistic_v9-v1/70000/022EE4F8-6026-E711-89A2-0242AC130002.root'
    ,'root://cms-xrd-global.cern.ch///store/mc/PhaseIISpring17D/SingleNeutrino/GEN-SIM-DIGI-RAW/PU200_90X_upgrade2023_realistic_v9-v1/70000/023B88FC-6026-E711-BB4C-0242AC130002.root'
    ,'root://cms-xrd-global.cern.ch///store/mc/PhaseIISpring17D/SingleNeutrino/GEN-SIM-DIGI-RAW/PU200_90X_upgrade2023_realistic_v9-v1/70000/026BAE30-6826-E711-9BCE-0242AC130002.root'),
    secondaryFileNames = cms.untracked.vstring(),
    inputCommands=cms.untracked.vstring(
      'keep *',
      'drop  *_TTClustersFromPhase2TrackerDigis_*_*',
      'drop  *_TTStubsFromPhase2TrackerDigis_*_*'
    )
)

process.options = cms.untracked.PSet(

)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    annotation = cms.untracked.string('step2 nevts:1'),
    name = cms.untracked.string('Applications'),
    version = cms.untracked.string('$Revision: 1.19 $')
)

# Output definition

process.FEVTDEBUGHLToutput = cms.OutputModule("PoolOutputModule",
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string('GEN-SIM-DIGI-RAW'),
        filterName = cms.untracked.string('')
    ),
    eventAutoFlushCompressedSize = cms.untracked.int32(10485760),
    fileName = cms.untracked.string('file:reprocess_L1T_L1TTTTracklet_HGCalTP3D_step2_TT_PhaseIISpring17D-PU200_pilot.root'),
    #fileName = cms.untracked.string('file:step2_ZEE_PU200_test.root'),
    #fileName = cms.untracked.string('file:step2_ZEE_PU200_10ev_FEVTDEBUGHLT_customHigherPtTrackParticles-RERUN_L1T_TTAssociator_EcalEBtp_HGCtp.root'),
    #fileName = cms.untracked.string('root://eoscms//eos/cms/store/group/dpg_trigger/comm_trigger/L1Trigger/rekovic/PhaseIIFall16DR82-820_backport_L1TMC_v1.2.2/step2_ZEE_PU200_100ev_FEVTDEBUGHLT_customHigherPtTrackParticles-RERUN_L1T_TTAssociator_EcalEBtp_HGCtp.root'),
    outputCommands = process.FEVTDEBUGHLTEventContent.outputCommands,
    splitLevel = cms.untracked.int32(0)
)

process.FEVTDEBUGHLToutput.outputCommands.append('keep  *_*_*_*')

# Additional output definition

#process.Timing = cms.Service("Timing")


# Other statements
process.mix.input.nbPileupEvents.averageNumber = cms.double(200.000000)
process.mix.bunchspace = cms.int32(25)
process.mix.minBunch = cms.int32(-12)
process.mix.maxBunch = cms.int32(3)
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, '90X_upgrade2023_realistic_v9', '')


process.HcalHardcodeGeometryEP = cms.ESProducer("HcalHardcodeGeometryEP",
    UseOldLoader = cms.bool(False)
)


process.HcalTPGCoderULUT = cms.ESProducer("HcalTPGCoderULUT",
    FGLUTs = cms.FileInPath('CalibCalorimetry/HcalTPGAlgos/data/HBHE_FG_LUT.dat'),
    LUTGenerationMode = cms.bool(True),
    MaskBit = cms.int32(32768),
    RCalibFile = cms.FileInPath('CalibCalorimetry/HcalTPGAlgos/data/RecHit-TPG-calib.dat'),
    inputLUTs = cms.FileInPath('CalibCalorimetry/HcalTPGAlgos/data/inputLUTcoder_physics.dat'),
    read_Ascii_LUTs = cms.bool(False),
    read_FG_LUTs = cms.bool(False),
    read_XML_LUTs = cms.bool(False)
)


process.HcalTrigTowerGeometryESProducer = cms.ESProducer("HcalTrigTowerGeometryESProducer")

process.CaloGeometryBuilder = cms.ESProducer("CaloGeometryBuilder",
    SelectedCalos = cms.vstring('HCAL', 
        'ZDC', 
        'EcalBarrel', 
        'TOWER', 
        'HGCalEESensitive', 
        'HGCalHESiliconSensitive')
)


process.CaloTPGTranscoder = cms.ESProducer("CaloTPGTranscoderULUTs",
    HFTPScaleShift = cms.PSet(
        NCT = cms.int32(1),
        RCT = cms.int32(3)
    ),
    LUTfactor = cms.vint32(1, 2, 5, 0),
    RCTLSB = cms.double(0.25),
    ZS = cms.vint32(4, 2, 1, 0),
    hcalLUT1 = cms.FileInPath('CalibCalorimetry/CaloTPG/data/outputLUTtranscoder_physics.dat'),
    hcalLUT2 = cms.FileInPath('CalibCalorimetry/CaloTPG/data/TPGcalcDecompress2.txt'),
    ietaLowerBound = cms.vint32(1, 18, 27, 29),
    ietaUpperBound = cms.vint32(17, 26, 28, 32),
    nominal_gain = cms.double(0.177),
    read_Ascii_Compression_LUTs = cms.bool(False),
    read_Ascii_RCT_LUTs = cms.bool(False)
)


process.CaloTopologyBuilder = cms.ESProducer("CaloTopologyBuilder")


process.CaloTowerHardcodeGeometryEP = cms.ESProducer("CaloTowerHardcodeGeometryEP")


process.CaloTowerTopologyEP = cms.ESProducer("CaloTowerTopologyEP")

#from SimTracker.TrackTriggerAssociation.TTTrackAssociation_cfi import *
#TrackTriggerAssociatorTracks = cms.Sequence(TTTrackAssociatorFromPixelDigis)

process.load('L1Trigger.L1THGCal.hgcalTriggerPrimitives_cff')
process.hgcl1tpg_step = cms.Path(process.hgcalTriggerPrimitives)


# load ntuplizer
#process.load('L1Trigger.L1THGCal.hgcalTriggerNtuples_cff')
#process.ntuple_step = cms.Path(process.hgcalTriggerNtuples)

process.load('SimCalorimetry.EcalEBTrigPrimProducers.ecalEBTriggerPrimitiveDigis_cff')
process.EcalEBtp_step = cms.Path(process.simEcalEBTriggerPrimitiveDigis)

# Path and EndPath definitions
process.HcalTPsimulation_step = cms.Path(process.hcalTTPSequence)
process.L1simulation_step = cms.Path(process.SimL1Emulator)

# remake track stubs 
# ===> IMPORTANT !!! stub window tuning as is by default in CMSSW is incorrect !!! <===
process.load('L1Trigger.TrackTrigger.TrackTrigger_cff')
from L1Trigger.TrackTrigger.TTStubAlgorithmRegister_cfi import *
process.TTClusterStub = cms.Path(process.TrackTriggerClustersStubs)

# L1 tracking
process.load('L1Trigger.TrackFindingTracklet.L1TrackletTracks_cff')
#process.load('SimTracker.TrackTriggerAssociation.TrackTriggerAssociator_cff')
#process.L1TrackTrigger_step = cms.Path(process.TrackTriggerAssociatorClustersStubs)
#process.L1TrackTrigger_step = cms.Path(process.TrackTriggerAssociatorClustersStubs*process.TrackTriggerAssociatorTracks)
#process.L1TrackTrigger_step = cms.Path(process.TrackTriggerAssociatorComplete)
#process.L1TrackTrigger_step = cms.Path(process.L1TrackTrigger)
process.TTClusterAssociatorFromPixelDigis.digiSimLinks          = cms.InputTag( "simSiPixelDigis","Tracker" )
process.L1TrackTrigger_step = cms.Path(process.L1TrackletTracksWithAssociators)

process.endjob_step = cms.EndPath(process.endOfProcess)
process.FEVTDEBUGHLToutput_step = cms.EndPath(process.FEVTDEBUGHLToutput)

# Schedule definition
#process.schedule = cms.Schedule(process.HcalTPsimulation_step,process.L1simulation_step,process.L1TrackTrigger_step,process.EcalEBtp_step,process.hgcl1tpg_step,process.endjob_step,process.FEVTDEBUGHLToutput_step)
process.schedule = cms.Schedule(process.L1simulation_step,process.TTClusterStub,process.L1TrackTrigger_step,process.hgcl1tpg_step,process.endjob_step,process.FEVTDEBUGHLToutput_step)


# Customisation from command line
# Automatic addition of the customisation function from L1Trigger.L1TNtuples.customiseL1Ntuple
from L1Trigger.L1TNtuples.customiseL1Ntuple import L1NtupleEMUGEN_MC

#call to customisation function L1NtupleEMUNoEventTree imported from L1Trigger.L1TNtuples.customiseL1Ntuple
#process = L1NtupleEMUNoEventTree(process)
process = L1NtupleEMUGEN_MC(process)

# Automatic addition of the customisation function from L1Trigger.Configuration.customiseUtils
#from L1Trigger.Configuration.customiseUtils import L1TGlobalSimDigisSummary 

#call to customisation function L1TGlobalSimDigisSummary imported from L1Trigger.Configuration.customiseUtils
#process = L1TGlobalSimDigisSummary(process)

# End of customisation functions

# Add early deletion of temporary data products to reduce peak memory need
from Configuration.StandardSequences.earlyDeleteSettings_cff import customiseEarlyDelete
process = customiseEarlyDelete(process)
# End adding early deletion
