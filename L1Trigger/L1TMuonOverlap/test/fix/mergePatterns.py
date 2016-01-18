import FWCore.ParameterSet.Config as cms
process = cms.Process("OMTFEmulation")
import os
import sys
import commands

process.load("FWCore.MessageLogger.MessageLogger_cfi")

verbose = False

if verbose:
    process.MessageLogger = cms.Service("MessageLogger",
       suppressInfo       = cms.untracked.vstring('AfterSource', 'PostModule'),
       destinations   = cms.untracked.vstring(
                                             'detailedInfo'
                                             ,'critical'
                                             ,'cout'
                    ),
       categories = cms.untracked.vstring(
                                        'CondDBESSource'
                                        ,'EventSetupDependency'
                                        ,'Geometry'
                                        ,'MuonGeom'
                                        ,'GetManyWithoutRegistration'
                                        ,'GetByLabelWithoutRegistration'
                                        ,'Alignment'
                                        ,'SiStripBackPlaneCorrectionDepESProducer'
                                        ,'SiStripLorentzAngleDepESProducer'
                                        ,'SiStripQualityESProducer'
                                        ,'TRACKER'
                                        ,'HCAL'
        ),
       critical       = cms.untracked.PSet(
                        threshold = cms.untracked.string('ERROR') 
        ),
       detailedInfo   = cms.untracked.PSet(
                      threshold  = cms.untracked.string('INFO'), 
                      CondDBESSource  = cms.untracked.PSet (limit = cms.untracked.int32(0) ), 
                      EventSetupDependency  = cms.untracked.PSet (limit = cms.untracked.int32(0) ), 
                      Geometry  = cms.untracked.PSet (limit = cms.untracked.int32(0) ), 
                      MuonGeom  = cms.untracked.PSet (limit = cms.untracked.int32(0) ), 
                      Alignment  = cms.untracked.PSet (limit = cms.untracked.int32(0) ), 
                      GetManyWithoutRegistration  = cms.untracked.PSet (limit = cms.untracked.int32(0) ), 
                      GetByLabelWithoutRegistration  = cms.untracked.PSet (limit = cms.untracked.int32(0) ) 

       ),
       cout   = cms.untracked.PSet(
                threshold  = cms.untracked.string('INFO'), 
                CondDBESSource  = cms.untracked.PSet (limit = cms.untracked.int32(0) ), 
                EventSetupDependency  = cms.untracked.PSet (limit = cms.untracked.int32(0) ), 
                Geometry  = cms.untracked.PSet (limit = cms.untracked.int32(0) ), 
                MuonGeom  = cms.untracked.PSet (limit = cms.untracked.int32(0) ), 
                Alignment  = cms.untracked.PSet (limit = cms.untracked.int32(0) ), 
                GetManyWithoutRegistration  = cms.untracked.PSet (limit = cms.untracked.int32(0) ), 
                GetByLabelWithoutRegistration  = cms.untracked.PSet (limit = cms.untracked.int32(0) ) 
                ),
                                        )

if not verbose:
    process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(10000)

process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(True))

process.source = cms.Source(
    'PoolSource',
    fileNames = cms.untracked.vstring('file:/home/akalinow/scratch/CMS/OverlapTrackFinder/Crab/SingleMuFullEtaTestSample/720_FullEta_v1/data/SingleMu_16_p_1_2_TWz.root')
    )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(100))

###PostLS1 geometry used
process.load('Configuration.Geometry.GeometryExtended2015_cff')
process.load('Configuration.Geometry.GeometryExtended2015Reco_cff')
############################
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')
from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_mc', '')

path = "job_3_pat/7_5_0/"
patternsXMLFiles = cms.VPSet()
for ipt in xrange(4,32):
    patternsXMLFiles.append(cms.PSet(patternsXMLFile = cms.FileInPath(path+"SingleMu_"+str(ipt)+"_p/GPs.xml")))
    patternsXMLFiles.append(cms.PSet(patternsXMLFile = cms.FileInPath(path+"SingleMu_"+str(ipt)+"_m/GPs.xml")))
            
patternsXMLFiles = cms.VPSet()
patternsXMLFiles.append(cms.PSet(patternsXMLFile = cms.FileInPath("L1Trigger/L1TMuonOverlap/data/Patterns_ipt4_31_750_4x.xml")))

###OMTF emulator configuration
process.load('L1Trigger.L1TMuonOverlap.OMTFProducer_cfi')
##Load configuration directly from XML
process.omtfEmulator.omtf.configFromXML = cms.bool(True)

process.omtfEmulator.omtf.patternsXMLFiles = patternsXMLFiles
process.omtfEmulator.dumpGPToXML = cms.bool(True)  

###Gen level filter configuration
process.MuonEtaFilter = cms.EDFilter("SimTrackEtaFilter",
                                minNumber = cms.uint32(1),
                                src = cms.InputTag("g4SimHits"),
                                cut = cms.string("momentum.eta<0.9 && momentum.eta>0.83 &&  momentum.pt>1")
                                )
process.GenMuPath = cms.Path(process.MuonEtaFilter)
##########################################

process.L1TMuonSeq = cms.Sequence(process.omtfEmulator)

process.L1TMuonPath = cms.Path(process.MuonEtaFilter*process.L1TMuonSeq)


process.out = cms.OutputModule("PoolOutputModule", 
   fileName = cms.untracked.string("Test.root"),
                               SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('GenMuPath')
        )
                               )

process.output_step = cms.EndPath(process.out)

process.schedule = cms.Schedule(process.GenMuPath,process.L1TMuonPath)
#process.schedule.extend([process.output_step])
