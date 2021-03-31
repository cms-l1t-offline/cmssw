############################################################
# define basic process
############################################################

import FWCore.ParameterSet.Config as cms
import FWCore.Utilities.FileUtils as FileUtils
import os

############################################################
# edit options here
############################################################
L1TRK_INST ="L1TrackMET" ### if not in input DIGRAW then we make them in the above step
process = cms.Process(L1TRK_INST)

ReRunTracking = False
GTTInput = False
VTXEmuInput = False

############################################################
# import standard configurations
############################################################

process.load('Configuration.StandardSequences.Services_cff')
process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('Configuration.Geometry.GeometryExtended2026D49Reco_cff')
process.load('Configuration.Geometry.GeometryExtended2026D49_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:phase2_realistic', '')

############################################################
# input and output
############################################################

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(10))

readFiles = cms.untracked.vstring(
    #"/store/relval/CMSSW_11_1_0_pre2/RelValTTbar_14TeV/GEN-SIM-DIGI-RAW/PU25ns_110X_mcRun4_realistic_v2_2026D49PU200-v1/20000/F7BF4AED-51F1-9D47-B86D-6C3DDA134AB9.root"
    #"/store/relval/CMSSW_11_3_0_pre3/RelValTTbar_14TeV/GEN-SIM-DIGI-RAW/PU_113X_mcRun4_realistic_v3_2026D49PU200-v1/00000/001edbad-174e-46af-932a-6ce8e04aee1c.root"
    #'/store/relval/CMSSW_11_2_0_pre5/RelValTTbar_14TeV/GEN-SIM-DIGI-RAW/PU25ns_110X_mcRun4_realistic_v3_2026D49PU200-v1/20000/0074C44A-BBE2-6849-965D-CB73FE0C0E6C.root',
    "/store/mc/Phase2HLTTDRSummer20ReRECOMiniAOD/DisplacedSUSY_stopToBottom_M_800_500mm_TuneCP5_14TeV_pythia8/FEVT/PU200_111X_mcRun4_realistic_T15_v1-v1/250000/F3E14864-7417-C941-8430-8BD3C8E06E97.root"


)
secFiles = cms.untracked.vstring()

process.source = cms.Source ("PoolSource",
                            fileNames = readFiles,
                            secondaryFileNames = secFiles,
                            duplicateCheckMode = cms.untracked.string('noDuplicateCheck'),
                            )


process.TFileService = cms.Service("TFileService", fileName = cms.string('TrackMET_Emulation.root'), closeFileFast = cms.untracked.bool(True))

if ReRunTracking:
  process.load("L1Trigger.TrackFindingTracklet.L1HybridEmulationTracks_cff")
  process.pTTTracksEmulation = cms.Path(process.L1HybridTracks)
  process.pTTTracksEmulationWithTruth = cms.Path(process.L1HybridTracksWithAssociators)

if GTTInput:
  process.load('L1Trigger.L1TTrackMatch.L1GTTInputProducer_cfi')
  process.pGTTin = cms.Path(process.L1GTTInputProducer)

process.load('L1Trigger.VertexFinder.VertexProducer_cff')
process.load("L1Trigger.L1TTrackMatch.L1TrackerEtMissProducer_cfi")
process.load("L1Trigger.L1TTrackMatch.L1TrackerEtMissEmulatorProducer_cfi")
process.load("L1Trigger.L1TTrackMatch.L1TkMETAnalyser_cfi")

############################################################
# Primary vertex
############################################################

process.VertexProducer.VertexReconstruction.Algorithm = cms.string("FastHisto")

process.L1TrackerEmuEtMiss.useGTTinput = GTTInput
process.L1TrackerEmuEtMiss.useVertexEmulator = VTXEmuInput

if GTTInput:
  process.L1TrackerEmuEtMiss.L1TrackInputTag = cms.InputTag("L1GTTInputProducer","Level1TTTracksConverted")
else:
  process.L1TrackerEmuEtMiss.L1TrackInputTag = cms.InputTag("TTTracksFromTrackletEmulation", "Level1TTTracks")  

if VTXEmuInput:
  process.VertexProducer.VertexReconstruction.Algorithm("FastHistoEmulation")
  process.L1TrackerEmuEtMiss.L1VertexInputTag = cms.InputTag("VertexProducerFastHistoEmulation,l1vertices")


process.pPV = cms.Path(process.VertexProducer)
process.pTkMET = cms.Path(process.L1TrackerEtMiss)
process.pTkEmuMET = cms.Path(process.L1TrackerEmuEtMiss)


process.analysis = cms.Path(process.L1TkMETAnalyser)

process.out = cms.OutputModule( "PoolOutputModule",
                                fastCloning = cms.untracked.bool( False ),
                                fileName = cms.untracked.string("test.root" )
		               )
process.pOut = cms.EndPath(process.out)

if ReRunTracking:
    process.schedule = cms.Schedule(process.pTTTracksEmulation,
                                    process.pTTTracksEmulationWithTruth,
                                    process.pPV, 
                                    process.pTkMET,
                                    process.pTkEmuMET, 
                                    process.analysis)

elif GTTInput:
    process.schedule = cms.Schedule(process.pGTTin,
                                    process.pPV, 
                                    process.pTkMET,
                                    process.pTkEmuMET, 
                                    process.analysis)

else:
    process.schedule = cms.Schedule(process.pPV, 
                                    process.pTkMET,
                                    process.pTkEmuMET, 
                                    process.analysis)
