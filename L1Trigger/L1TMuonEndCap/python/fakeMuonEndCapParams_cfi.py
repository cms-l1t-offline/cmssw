import FWCore.ParameterSet.Config as cms

emtfParamsSource = cms.ESSource(
	"EmptyESSource",
	recordName = cms.string('L1TMuonEndcapParamsRcd'),
	iovIsRunNotTime = cms.bool(True),
	firstValid = cms.vuint32(1)
)

##EMTF ESProducer. Fills CondFormats from XML files.
emtfParams = cms.ESProducer(
	"L1TMuonEndcapParamsESProducer",
   PtAssignVersion = cms.int32(1),
   St1MatchWindow = cms.int32(15),
   St2MatchWindow = cms.int32(15),
   St3MatchWindow = cms.int32(7),
   St4MatchWindow = cms.int32(7)
)




