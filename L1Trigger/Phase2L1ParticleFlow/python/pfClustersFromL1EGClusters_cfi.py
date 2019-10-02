import FWCore.ParameterSet.Config as cms

pfClustersFromL1EGClusters = cms.EDProducer("PFClusterProducerFromL1EGClusters",
    src = cms.InputTag("L1EGammaClusterEmuProducer","L1EGXtalClusterEmulator"),
    etMin = cms.double(0.5),
    corrector  = cms.string("L1Trigger/Phase2L1ParticleFlow/data/emcorr_barrel.root"),
    resol = cms.PSet(
            etaBins = cms.vdouble( 0.700,  1.200,  1.600),
            offset  = cms.vdouble( 0.873,  1.081,  1.563),
            scale   = cms.vdouble( 0.011,  0.015,  0.012),
            kind    = cms.string('calo'),
    )
)

# use phase2_hgcalV10 to customize for 106X L1TDR MC even in the barrel, since there's no other modifier for it
from Configuration.Eras.Modifier_phase2_hgcalV10_cff import phase2_hgcalV10
phase2_hgcalV10.toModify(pfClustersFromL1EGClusters,
    corrector  = "L1Trigger/Phase2L1ParticleFlow/data/emcorr_barrel_106X.root",
    resol = cms.PSet(
        etaBins = cms.vdouble( 0.700,  1.200,  1.600),
        offset  = cms.vdouble( 1.047,  1.096,  1.633),
        scale   = cms.vdouble( 0.014,  0.031,  0.019),
        kind    = cms.string('calo')
    )
)
