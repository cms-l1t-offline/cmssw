import FWCore.ParameterSet.Config as cms

def customise(process):

    ## EMTF
    ## - see python/simEmtfDigis_cfi.py
    if hasattr(process, 'simEmtfDigis'):
        #process.simEmtfDigis.spPCParams16.ZoneBoundaries = [0,36,54,96,127]
        #process.simEmtfDigis.spPCParams16.UseNewZones    = True
        process.simEmtfDigis.DTEnable                    = True
        process.simEmtfDigis.CSCEnable                   = True
        process.simEmtfDigis.RPCEnable                   = True
        process.simEmtfDigis.GEMEnable                   = True
        process.simEmtfDigis.IRPCEnable                  = True
        process.simEmtfDigis.ME0Enable                   = True
        process.simEmtfDigis.Era                         = cms.string('Phase2_timing')
        process.simEmtfDigis.spPAParams16.PtLUTVersion   = cms.int32(7)

    ## CSCTriggerPrimitives
    ## - see L1Trigger/CSCTriggerPrimitives/python/cscTriggerPrimitiveDigis_cfi.py
    if hasattr(process, 'simCscTriggerPrimitiveDigis'):
        process.simCscTriggerPrimitiveDigis.commonParam.runME11ILT = cms.bool(False)
        process.simCscTriggerPrimitiveDigis.commonParam.runME21ILT = cms.bool(False)
    else:
        process.load('L1Trigger.CSCTriggerPrimitives.cscTriggerPrimitiveDigis_cfi')
        process.simCscTriggerPrimitiveDigis.commonParam.runME11ILT = cms.bool(False)
        process.simCscTriggerPrimitiveDigis.commonParam.runME21ILT = cms.bool(False)

    ## RPCRecHit
    if hasattr(process, 'rpcRecHits'):
        process.rpcRecHits.rpcDigiLabel = 'simMuonRPCDigis'
    else:
        process.load('RecoLocalMuon.RPCRecHit.rpcRecHits_cfi')
        process.rpcRecHits.rpcDigiLabel = 'simMuonRPCDigis'

    ## ME0TriggerDigi added in 10_5_X
    if hasattr(process, 'me0TriggerPseudoDigis105X'):
        pass
    else:
        process.load('L1Trigger.L1TMuonEndCap.me0TriggerPseudoDigis105X_cff')
    return process
