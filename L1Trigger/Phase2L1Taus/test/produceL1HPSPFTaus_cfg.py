
import FWCore.ParameterSet.Config as cms

from Configuration.StandardSequences.Eras import eras

from Configuration.ProcessModifiers.convertHGCalDigisSim_cff import convertHGCalDigisSim
process = cms.Process('Produce',eras.Phase2C4_trigger)

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
process.load('Configuration.Geometry.GeometryExtended2023D35Reco_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(100)
)

# Input source
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        #'/store/mc/PhaseIIMTDTDRAutumn18MiniAOD/VBFHToTauTau_M125_14TeV_powheg_pythia8_correctedGridpack/MINIAODSIM/PU200_103X_upgrade2023_realistic_v2-v1/120000/556D067D-7601-BD4D-A168-88D0A67428DB.root',
        'file:/afs/cern.ch/work/s/sbhowmik/sample/556D067D-7601-BD4D-A168-88D0A67428DB.root',
    ),
    secondaryFileNames = cms.untracked.vstring(
        #'/store/mc/PhaseIIMTDTDRAutumn18DR/VBFHToTauTau_M125_14TeV_powheg_pythia8_correctedGridpack/FEVT/PU200_103X_upgrade2023_realistic_v2-v1/120000/B12B9398-9DBA-D442-855D-BFEBCE64D269.root',
        #'/store/mc/PhaseIIMTDTDRAutumn18DR/VBFHToTauTau_M125_14TeV_powheg_pythia8_correctedGridpack/FEVT/PU200_103X_upgrade2023_realistic_v2-v1/120000/5AB0C82F-3846-4D45-A27F-A356DEB6C5DE.root',
        'file:/afs/cern.ch/work/s/sbhowmik/sample/B12B9398-9DBA-D442-855D-BFEBCE64D269.root',
        'file:/afs/cern.ch/work/s/sbhowmik/sample/5AB0C82F-3846-4D45-A27F-A356DEB6C5DE.root',
    ),
    inputCommands = cms.untracked.vstring("keep *", 
        "drop l1tEMTFHit2016Extras_simEmtfDigis_CSC_HLT",
        "drop l1tEMTFHit2016Extras_simEmtfDigis_RPC_HLT",
        "drop l1tEMTFHit2016s_simEmtfDigis__HLT",
        "drop l1tEMTFTrack2016Extras_simEmtfDigis__HLT",
        "drop l1tEMTFTrack2016s_simEmtfDigis__HLT",
        'drop l1tEMTFHit2016Extras_simEmtfDigis_CSC_HLT',
        'drop l1tEMTFHit2016Extras_simEmtfDigis_RPC_HLT',
        'drop l1tEMTFHit2016s_simEmtfDigis__HLT',
        'drop l1tEMTFTrack2016Extras_simEmtfDigis__HLT',
        'drop l1tEMTFTrack2016s_simEmtfDigis__HLT',
    ),
)

process.options = cms.untracked.PSet(

)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    annotation = cms.untracked.string('step2 nevts:1'),
    name = cms.untracked.string('Applications'),
    version = cms.untracked.string('$Revision: 1.19 $')
)

# Other statements
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:phase2_realistic', '')

# Sequence, Path and EndPath definitions
process.productionSequence = cms.Sequence()

process.load('SimCalorimetry.HcalTrigPrimProducers.hcaltpdigi_cff')
process.load('CalibCalorimetry.CaloTPG.CaloTPGTranscoder_cfi')

process.load('L1Trigger.L1THGCal.hgcalTriggerPrimitives_cff')
process.productionSequence += process.hgcalTriggerPrimitives

process.load('SimCalorimetry.EcalEBTrigPrimProducers.ecalEBTriggerPrimitiveDigis_cff')
process.productionSequence += process.simEcalEBTriggerPrimitiveDigis

process.load('L1Trigger.TrackFindingTracklet.L1TrackletTracks_cff')
process.productionSequence += process.L1TrackletTracksWithAssociators

process.load('L1Trigger.VertexFinder.VertexProducer_cff')
process.VertexProducer.l1TracksInputTag = cms.InputTag("TTTracksFromTracklet", "Level1TTTracks")

process.load('Configuration.StandardSequences.SimL1Emulator_cff')
process.productionSequence += process.SimL1Emulator

process.load("L1Trigger.Phase2L1ParticleFlow.pfTracksFromL1Tracks_cfi")
process.productionSequence += process.pfTracksFromL1Tracks

process.load("L1Trigger.Phase2L1ParticleFlow.l1ParticleFlow_cff")
process.productionSequence += process.l1ParticleFlow

process.load("L1Trigger.Phase2L1ParticleFlow.l1pfJetMet_cff")
process.productionSequence += process.l1PFJets

process.kt6L1PFJetsPF = process.ak4PFL1PF.clone(
    jetAlgorithm = cms.string("Kt"),
    rParam       = cms.double(0.6),
    doRhoFastjet = cms.bool(True),
    Rho_EtaMax   = cms.double(3.0)
)
process.productionSequence += process.kt6L1PFJetsPF
process.l1pfNeutralCandidatesPF = cms.EDFilter("L1TPFCandSelector",
    src = cms.InputTag('l1pfCandidates:PF'),                                      
    cut = cms.string("pdgId = 22"), # CV: cms.string("id = Photon") does not work (does not select any l1t::PFCandidates)
    filter = cms.bool(False)                                           
)
process.productionSequence += process.l1pfNeutralCandidatesPF
process.kt6L1PFJetsNeutralsPF = process.kt6L1PFJetsPF.clone(
    src = cms.InputTag('l1pfNeutralCandidatesPF')
) 
process.productionSequence += process.kt6L1PFJetsNeutralsPF

process.kt6L1PFJetsPuppi = process.kt6L1PFJetsPF.clone(
    src = cms.InputTag('l1pfCandidates:Puppi')
)    
process.productionSequence += process.kt6L1PFJetsPuppi
process.l1pfNeutralCandidatesPuppi = process.l1pfNeutralCandidatesPF.clone(
    src = cms.InputTag('l1pfCandidates:Puppi'),                                      
)
process.productionSequence += process.l1pfNeutralCandidatesPuppi
process.kt6L1PFJetsNeutralsPuppi = process.kt6L1PFJetsPuppi.clone(
    src = cms.InputTag('l1pfNeutralCandidatesPuppi')
) 
process.productionSequence += process.kt6L1PFJetsNeutralsPuppi

############################################################
# Generator-level (visible) hadronic taus
############################################################

process.load("PhysicsTools.JetMCAlgos.TauGenJets_cfi")
process.tauGenJets.GenParticles = cms.InputTag("prunedGenParticles")

process.load("PhysicsTools.JetMCAlgos.TauGenJetsDecayModeSelectorAllHadrons_cfi")

process.genTaus = cms.Sequence(process.tauGenJets + process.tauGenJetsSelectorAllHadrons)
process.productionSequence += process.genTaus

############################################################
# produce  L1 HPS PF Tau objects
############################################################

from L1Trigger.Phase2L1Taus.L1HPSPFTauProducerPF_cff import L1HPSPFTauProducerPF
from L1Trigger.Phase2L1Taus.L1HPSPFTauProducerPuppi_cff import L1HPSPFTauProducerPuppi
for useStrips in [ True, False ]:
    for applyPreselection in [ True, False ]:
        moduleNameBase = "L1HPSPFTauProducer"
        if useStrips and applyPreselection:
            moduleNameBase += "WithStripsAndPreselection"
        elif useStrips and not applyPreselection:
            moduleNameBase += "WithStripsWithoutPreselection"
        elif not useStrips and applyPreselection:
            moduleNameBase += "WithoutStripsWithPreselection"
        elif not useStrips and not applyPreselection:
            moduleNameBase += "WithoutStripsAndPreselection"
        else:
            raise ValueError("Invalid Combination of 'useStrips' and 'applyPreselection' Configuration parameters !!")
        
        moduleNamePF = moduleNameBase + "PF"
        modulePF = L1HPSPFTauProducerPF.clone(
            useStrips = cms.bool(useStrips),
            applyPreselection = cms.bool(applyPreselection),
            debug = cms.untracked.bool(False)
        )
        setattr(process, moduleNamePF, modulePF)
        process.productionSequence += getattr(process, moduleNamePF)

        moduleNamePuppi = moduleNameBase + "Puppi"
        modulePuppi = L1HPSPFTauProducerPuppi.clone(
            useStrips = cms.bool(useStrips),
            applyPreselection = cms.bool(applyPreselection),
            debug = cms.untracked.bool(False)
        )
        setattr(process, moduleNamePuppi, modulePuppi)
        process.productionSequence += getattr(process, moduleNamePuppi)

############################################################ 
# produce L1 Tau objects using Isobel's code 
############################################################ 

process.load("L1Trigger.Phase2L1Taus.L1PFTauProducer_cff")
process.L1PFTauProducer.debug = cms.untracked.bool(False)
process.L1PFTauProducer.L1PFObjects = cms.InputTag("l1pfCandidates:PF")
process.L1PFTauProducer.L1Neutrals = cms.InputTag("l1pfCandidates")
process.productionSequence += process.L1PFTauProducer

process.production_step = cms.Path(process.productionSequence)

############################################################ 
# write output file
############################################################ 

process.out = cms.OutputModule("PoolOutputModule",
    fileName = cms.untracked.string("NTuple_L1HPSPFTauProducer.root"),                           
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('production_step')
    ),
    outputCommands = cms.untracked.vstring(
        'drop *_*_*_*',                                 
        'keep *_l1pfCandidates_PF_*',
        'keep *_l1pfCandidates_Puppi_*',
        'keep *_l1pfProducer*_z0_*',
        'keep *_pfTracksFromL1Tracks*_*_*',
        'keep *_pfClustersFrom*_*_*',
        'keep *_TTTracksFromTracklet_*_*',
        'keep *_VertexProducer_*_*',                                
        'keep *_slimmedTaus_*_*',
        'keep *_packedPFCandidates_*_*',
        'keep *_generator_*_*',
        'keep *_caloStage2Digis_*_*',
        'keep *_L1HPSPFTauProducer*PF_*_*',                           
        'keep *_L1HPSPFTauProducer*Puppi_*_*',                            
        'keep *_prunedGenParticles_*_*',
        'keep *_tauGenJetsSelectorAllHadrons_*_*',
        'keep *_particleFlow_*_*',
        'keep *_generalTracks_*_*',
        'keep *_electronGsfTracks_*_*',
        'keep *_offlineSlimmedPrimaryVertices_*_*',                           
        'keep *_L1PFTauProducer_*_*',
        'keep *_ak4PFL1PF_*_*',
        'keep *_ak4PFL1PFCorrected_*_*',
        'keep *_kt6L1PFJetsPF_rho_*',                
        'keep *_kt6L1PFJetsNeutralsPF_rho_*',                            
        'keep *_ak4PFL1Puppi_*_*',
        'keep *_ak4PFL1PuppiCorrected_*_*',
        'keep *_kt6L1PFJetsPuppi_rho_*',                             
        'keep *_kt6L1PFJetsNeutralsPuppi_rho_*',
        'keep *_slimmedAddPileupInfo_*_*', 
    )                           
)
process.outpath = cms.EndPath(process.out)

process.endjob_step = cms.EndPath(process.endOfProcess)

# Schedule definition
process.schedule = cms.Schedule(process.production_step, process.outpath, process.endjob_step)

from PhysicsTools.PatAlgos.tools.helpers import associatePatAlgosToolsTask
associatePatAlgosToolsTask(process)

# Add early deletion of temporary data products to reduce peak memory need
from Configuration.StandardSequences.earlyDeleteSettings_cff import customiseEarlyDelete
process = customiseEarlyDelete(process)
# End adding early deletion

# Enable module run-time report
#process.options = cms.untracked.PSet(
#    wantSummary = cms.untracked.bool(True)
#)

dump_file = open('dump.py','w')
dump_file.write(process.dumpPython())

process.options.numberOfThreads = cms.untracked.uint32(2)
