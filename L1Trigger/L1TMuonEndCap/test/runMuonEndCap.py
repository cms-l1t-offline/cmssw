# -*- coding: utf-8 -*-
import FWCore.ParameterSet.Config as cms
process = cms.Process("L1TMuonEmulation")
import os
import sys
import commands

process.load("FWCore.MessageLogger.MessageLogger_cfi")

process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(1)
process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(False))

process.source = cms.Source('PoolSource',
 fileNames = cms.untracked.vstring(
 
 								'/cms/data/store/mc/Fall13dr/MuPlus_Pt-1to150_PositiveEndcap-gun/GEN-SIM-RAW/tsg_PU40bx25_POSTLS162_V2-v1/20000/04643A9D-F382-E311-9D3C-0017A4770C34.root'
								  
								  )
	                    )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(5))

# PostLS1 geometry used
process.load('Configuration.Geometry.GeometryExtended2016Reco_cff')
############################
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')
from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_mc', '')

process.GlobalTag.toGet = cms.VPSet(
 cms.PSet(
	 record  = cms.string("L1TMuonEndcapParamsRcd"),
	 tag	 = cms.string("L1TMuonEndcapParams_Stage2v0_hlt"),
	 connect = cms.string("frontier://FrontierPrep/CMS_CONDITIONS")
	)
) 

####Event Setup Producer
#process.load('L1Trigger.L1TMuonEndCap.fakeMuonEndCapParams_cfi')
#process.esProd = cms.EDAnalyzer("EventSetupRecordDataGetter",
#   toGet = cms.VPSet(
#	cms.PSet(record = cms.string('L1TMuonEndcapParamsRcd'),
#			data = cms.vstring('L1TMuonEndcapParams'))
#				),    
#   verbose = cms.untracked.bool(True)
#)

####OMTF Emulator
process.load('L1Trigger.L1TMuonEndCap.simMuonEndCapDigis_cfi')
process.load("Configuration.StandardSequences.RawToDigi_cff")
process.load("Configuration.StandardSequences.L1Emulator_cff")

process.L1ReEmulSeq = cms.Sequence(process.csctfDigis
                                   + process.simCscTriggerPrimitiveDigis
                                   )

process.dumpED = cms.EDAnalyzer("EventContentAnalyzer")
process.dumpES = cms.EDAnalyzer("PrintEventSetupContent")

process.L1TMuonSeq = cms.Sequence( #process.esProd +   
									       
								   process.simEmtfDigis 
                                   #+ process.dumpED
                                   #+ process.dumpES
)

process.L1TMuonPath = cms.Path(process.L1ReEmulSeq+process.L1TMuonSeq)

process.out = cms.OutputModule("PoolOutputModule", 
   fileName = cms.untracked.string("l1tomtf_superprimitives1.root")
)

#process.output_step = cms.EndPath(process.out)
process.schedule = cms.Schedule(process.L1TMuonPath)
#process.schedule.extend([process.output_step])
