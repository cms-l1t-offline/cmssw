
import FWCore.ParameterSet.Config as cms
from L1Trigger.Phase2L1Taus.L1TrkTauParticleProducer_cfi import L1TrkTaus

L1TkEGTaus = cms.EDProducer("L1TkEGTauParticleProducer",
        label = cms.string("TkEG"), 	# labels the collection of L1TkEGTauParticleProducer that is produced

        # L1 Tracker Taus 
        #L1TrkTauInputTag = cms.InputTag("L1TrkTaus", "TrkTau"),
        L1TrkTauInputTag = cms.InputTag("L1TrackerTaus", "TrkTau"),
        trkTau_minEt     = cms.double( 0.0 ),
        trkTau_minEta    = cms.double( 0.0 ),
        trkTau_maxEta    = cms.double( 2.5 ),
        tk_nFitParams    = L1TrkTaus.tk_nFitParams,
                      
        # L1 EGammas
        L1EGammaInputTag      = cms.InputTag("L1EGammaClusterEmuProducer", "L1EGammaCollectionBXVEmulator"), # barrel
        L1EGammaHGCalInputTag = cms.InputTag("l1EGammaEEProducer","L1EGammaCollectionBXVWithCuts"), # endcap
        eg_minEt              = cms.double( 1.5 ),
        eg_minEta             = cms.double( 0.0 ),
        eg_maxEta             = cms.double( 2.5 ),
        eg_hwQualB            = cms.vuint32( 0, 1,  3, 4, 7 ),
        eg_hwQualE            = cms.vuint32( 2 ),

        # Shrinking Cone parameters
        shrinkCone_Constant  = cms.double( 2.5 ),
        sigCone_dRMin        = cms.double( 0.0 ), 
        #sigCone_dRMax        = cms.double( 0.15), 
        sigCone_cutoffDeltaR = cms.double( 0.15 ),
        isoCone_dRMax        = cms.double( 0.30 ),
        isoCone_useCone      = cms.bool( False ), # instead of annulus

        # EGs clustering parameters
        maxInvMass_TkEGs = cms.double( 1.77 ), # GeV
                            
        # Apply EG-Et-dependent calibration factors to the tracks+EG candidates?   
        tkEGTau_calibrateEt = cms.bool( True ), # instead of annulus
)
