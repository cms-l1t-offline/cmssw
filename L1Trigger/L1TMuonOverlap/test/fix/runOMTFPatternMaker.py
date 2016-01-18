import FWCore.ParameterSet.Config as cms
process = cms.Process("OMTFEmulation")
import os
import sys
import commands

verbose = False

process.load("FWCore.MessageLogger.MessageLogger_cfi")

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
    process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(50000)
process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(True))

process.source = cms.Source(
    'PoolSource',
    fileNames = cms.untracked.vstring('file:/home/akalinow/scratch/CMS/OverlapTrackFinder/Crab/SingleMuFullEtaTestSample/750_FullEta_v2/data/SingleMu_12_m_1_1_hh0.root')
    #fileNames = cms.untracked.vstring('file:/home/akalinow/scratch/CMS/OverlapTrackFinder/Crab/WToMuNu_Tune4C_13TeV-pythia8/Fall13dr-tsg_PU40bx50_POSTLS162_V2-v1/GEN-SIM-RAW/data/WToMuNu_Tune4C_13TeV-pythia8_10_1_9XA.root')
)

##Use all available events in a single job.
##Only for making the connections maps.
process.source.fileNames =  cms.untracked.vstring()
path = "/home/akalinow/scratch/CMS/OverlapTrackFinder/Crab/SingleMuFullEta/721_FullEta_v4/data/"
command = "ls "+path+"/SingleMu_25_?_9{1,2}*"
fileList = commands.getoutput(command).split("\n")
process.source.fileNames =  cms.untracked.vstring()
for aFile in fileList:
    process.source.fileNames.append('file:'+aFile)


process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(10000))

###TEST
'''
process.source = cms.Source(
    'PoolSource',
    fileNames = cms.untracked.vstring('file:/home/akalinow/scratch/CMS/OverlapTrackFinder/Crab/SingleMuFullEtaTestSample/720_FullEta_v1/data/SingleMu_16_p_1_2_TWz.root'),
    #fileNames = cms.untracked.vstring('file:/home/akalinow/scratch/CMS/OverlapTrackFinder/Crab/JPsi_21kEvents.root')
    #eventsToProcess = cms.untracked.VEventRange('16:8')
    )
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1000))
'''
#######

###PostLS1 geometry used
process.load('Configuration.Geometry.GeometryExtended2015_cff')
process.load('Configuration.Geometry.GeometryExtended2015Reco_cff')
############################
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')
from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_mc', '')

###OMTF pattern maker configuration
process.omtfPatternMaker = cms.EDAnalyzer("OMTFPatternMaker",
                                          srcDTPh = cms.InputTag('simDtTriggerPrimitiveDigis'),
                                          srcDTTh = cms.InputTag('simDtTriggerPrimitiveDigis'),
                                          srcCSC = cms.InputTag('simCscTriggerPrimitiveDigis','MPCSORTED'),
                                          srcRPC = cms.InputTag('simMuonRPCDigis'),                                              
                                          g4SimTrackSrc = cms.InputTag('g4SimHits'),
                                          makeGoldenPatterns = cms.bool(False),                                     
                                          makeConnectionsMaps = cms.bool(True),                                      
                                          dropRPCPrimitives = cms.bool(False),                                    
                                          dropDTPrimitives = cms.bool(True),                                    
                                          dropCSCPrimitives = cms.bool(True),   
                                          ptCode = cms.int32(16),
                                          charge = cms.int32(1),
                                          omtf = cms.PSet(
                                              configFromXML = cms.bool(True),   
                                              patternsXMLFiles = cms.VPSet(                                       
                                                  cms.PSet(patternsXMLFile = cms.FileInPath("L1Trigger/L1TMuonOverlap/data/Patterns_ipt4_31_750.xml")),
                                              ),
                                              configXMLFile = cms.FileInPath("L1Trigger/L1TMuonOverlap/data/hwToLogicLayer_750.xml"),
                                          )
)

process.L1TMuonSeq = cms.Sequence(process.omtfPatternMaker)

process.L1TMuonPath = cms.Path(process.L1TMuonSeq)

process.schedule = cms.Schedule(process.L1TMuonPath)
