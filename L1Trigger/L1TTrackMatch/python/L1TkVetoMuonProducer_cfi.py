import FWCore.ParameterSet.Config as cms

L1TkVetoMuons = cms.EDProducer(
    "L1TkVetoMuonProducer",
    ###############################################
    ############################################### common stuff
    L1BMTFInputTag  = cms.InputTag("simKBmtfDigis","BMTF"),
    L1OMTFInputTag  = cms.InputTag("simOmtfDigis","OMTF"),
    L1EMTFInputTag  = cms.InputTag("simEmtfDigis","EMTF"),
    L1TrackInputTag = cms.InputTag("TTTracksFromTracklet", "Level1TTTracks"),
    ###############################################
    ############################################### TP algo
    ETAMIN = cms.double(0),
    ETAMAX = cms.double(5.),        # no cut
    ZMAX = cms.double( 25. ),       # in cm
    CHI2MAX = cms.double( 100. ),
    PTMINTRA = cms.double( 2. ),    # in GeV
    DRmax = cms.double( 0.12 ),
    nStubsmin = cms.int32( 4 ),        # minimum number of stubs
    correctGMTPropForTkZ = cms.bool(True),
    use5ParameterFit = cms.bool(False), #use 4-pars by defaults
    ptLoose = cms.double( 4 ),
    ptMedium = cms.double( 3 ),
    ptTight = cms.double( 2 ),
)
