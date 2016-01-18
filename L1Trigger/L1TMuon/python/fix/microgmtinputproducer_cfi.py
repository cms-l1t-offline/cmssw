import FWCore.ParameterSet.Config as cms

L1TMicroGMTInputProducer = cms.EDProducer('L1TMicroGMTInputProducer',
    inputFileName = cms.string("../test/test.dat")
)
