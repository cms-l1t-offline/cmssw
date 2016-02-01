import FWCore.ParameterSet.Config as cms

L1RPCbTFTrackConverter = cms.EDProducer(
    'L1RPCTFTrackConverter',
    RPCTrackSrc = cms.InputTag('simRpcTriggerDigis','RPCb'),
    RPCL1LinkSrc = cms.InputTag('simRpcTriggerDigis','RPCb'),
    TriggerPrimitiveSrc = cms.InputTag('L1TMuonTriggerPrimitives','')
    )

L1RPCfTFTrackConverter = cms.EDProducer(
    'L1RPCTFTrackConverter',
    RPCTrackSrc = cms.InputTag('simRpcTriggerDigis','RPCf'),
    RPCL1LinkSrc = cms.InputTag('simRpcTriggerDigis','RPCf'),
    TriggerPrimitiveSrc = cms.InputTag('L1TMuonTriggerPrimitives','')
    )

L1RPCTFTrackConverters = cms.Sequence( L1RPCbTFTrackConverter +
                                       L1RPCfTFTrackConverter   )
