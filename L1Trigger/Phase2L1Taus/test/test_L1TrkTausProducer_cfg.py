# Configuration file to run L1TrkTausProducer.cc
# using: 
# Revision: 1.19 
# Source: /local/reps/CMSSW/CMSSW/Configuration/Applications/python/ConfigBuilder.py,v 
import FWCore.ParameterSet.Config as cms

#from Configuration.ProcessModifiers.convertHGCalDigisSim_cff import convertHGCalDigisSim
from Configuration.StandardSequences.Eras import eras

#process = cms.Process('REPR',eras.Phase2C4_trigger) # MTD MC samples
process = cms.Process('REPR',eras.Phase2C8_trigger) # 10_6_X TDR MC samples

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
#process.load('Configuration.Geometry.GeometryExtended2023D35Reco_cff')  # MTD MC samples
#process.load('Configuration.Geometry.GeometryExtended2023D35_cff')      # MTD MC samples
process.load('Configuration.Geometry.GeometryExtended2023D41Reco_cff')  # 10_6_X TDR MC samples
process.load('Configuration.Geometry.GeometryExtended2023D41_cff')      # 10_6_X TDR MC samples
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('Configuration.StandardSequences.SimL1Emulator_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')


process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(20)
)

# Input source
process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring(
                                # -- 10_6_X TDR MC samples --
                                # "root://cms-xrd-global.cern.ch//store/mc/PhaseIITDRSpring19DR/SingleTau_PT2to150/GEN-SIM-DIGI-RAW/PU200_106X_upgrade2023_realistic_v3-v2/50000/FFA0DD2D-6ECC-A840-81E8-9E5A1F1A8476.root",
                                "root://cms-xrd-global.cern.ch//store/mc/PhaseIITDRSpring19DR/SingleTau_PT2to150/GEN-SIM-DIGI-RAW/NoPU_106X_upgrade2023_realistic_v3-v1/260000/1BF39431-3FE1-AD46-9C73-54894BF7A165.root",

                                # -- MTD samples --
                                #"/store/mc/PhaseIIMTDTDRAutumn18DR/NeutrinoGun_E_10GeV/FEVT/PU200_103X_upgrade2023_realistic_v2-v1/40002/FF63F2E7-798C-AA4B-B45B-267534B77835.root",
                                #"/store/mc/PhaseIIMTDTDRAutumn18DR/GluGluHToTauTau_M125_14TeV_powheg_pythia8/FEVT/PU200_103X_upgrade2023_realistic_v2-v1/80000/FF1F2905-A82D-3E45-B34E-E040C237018E.root",

                                # -- Fall17D samples --
                                #"/store/mc/PhaseIIFall17D/GluGluHToTauTau_M125_14TeV_powheg_pythia8/GEN-SIM-DIGI-RAW/L1TPU200_93X_upgrade2023_realistic_v5-v1/90000/00D9F890-9739-E811-A931-E0071B73B6B0.root",
                                #"/store/mc/PhaseIIFall17D/GluGluHToTauTau_M125_14TeV_powheg_pythia8/GEN-SIM-DIGI-RAW/L1TPU200_93X_upgrade2023_realistic_v5-v1/90000/08467E34-8139-E811-8059-008CFA1C6564.root",
                            ),
                            secondaryFileNames = cms.untracked.vstring()
                        )

process.options = cms.untracked.PSet(
    
)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    annotation = cms.untracked.string('repr nevts:2'),
    name = cms.untracked.string('Applications'),
    version = cms.untracked.string('$Revision: 1.19 $')
)

# Other statements
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, '103X_upgrade2023_realistic_v2', '') 

process.load('SimCalorimetry.HcalTrigPrimProducers.hcaltpdigi_cff')
process.load('CalibCalorimetry.CaloTPG.CaloTPGTranscoder_cfi') 

# Path and EndPath definitions
process.L1simulation_step = cms.Path(process.SimL1Emulator)

process.load('L1Trigger.TrackFindingTracklet.L1TrackletTracks_cff')
process.L1TrackTrigger_step = cms.Path(process.L1TrackletTracks)

# Now we produce the L1TrkTauParticles
process.load("L1Trigger.Phase2L1Taus.L1TrkTauParticleProducer_cfi")
process.pL1TrkTaus = cms.Path( process.L1TrkTaus )

# ---------------------------------------------------------------------------

process.Out = cms.OutputModule( "PoolOutputModule",
    fileName = cms.untracked.string( "l1trktausTest.root" ),
    fastCloning = cms.untracked.bool( False ),
    outputCommands = cms.untracked.vstring(
        "keep *_L1TrkTaus_*_*"
        )
)

# Path and EndPath definitions
process.end = cms.EndPath( process.Out )

# Automatic addition of the customisation function from L1Trigger.Configuration.customiseUtils
from L1Trigger.Configuration.customiseUtils import DropDepricatedProducts,L1TrackTriggerTracklet,DropOutputProducts 

#call to customisation function DropDepricatedProducts imported from L1Trigger.Configuration.customiseUtils
process = DropDepricatedProducts(process)

# Add early deletion of temporary data products to reduce peak memory need
from Configuration.StandardSequences.earlyDeleteSettings_cff import customiseEarlyDelete
process = customiseEarlyDelete(process)

#dump_file = open("dump_file.py", "w")
#dump_file.write(process.dumpPython())
