import FWCore.ParameterSet.Config as cms

import L1Trigger.Phase2L1ParticleFlow.pfClustersFromHGC3DClusters_cfi

pfClustersFromHGC3DClustersEM = L1Trigger.Phase2L1ParticleFlow.pfClustersFromHGC3DClusters_cfi.pfClustersFromHGC3DClusters.clone(
    emOnly = cms.bool(True),
    etMin = cms.double(0.0), 
    corrector  = cms.string("L1Trigger/Phase2L1ParticleFlow/data/emcorr_hgc.root"),
    preEmId  = cms.string(""),
    resol = cms.PSet(
            etaBins = cms.vdouble( 1.900,  2.200,  2.500,  2.800,  2.950),
            offset  = cms.vdouble( 0.566,  0.557,  0.456,  0.470,  0.324),
            scale   = cms.vdouble( 0.030,  0.024,  0.024,  0.023,  0.042),
            kind    = cms.string('calo'),
    )
)


from Configuration.Eras.Modifier_phase2_hgcalV10_cff import phase2_hgcalV10
phase2_hgcalV10.toModify(pfClustersFromHGC3DClustersEM,
    corrector = "L1Trigger/Phase2L1ParticleFlow/data/emcorr_hgc_106X.root",
    resol = cms.PSet(
        etaBins = cms.vdouble( 1.900,  2.200,  2.500,  2.800,  2.950),
        offset  = cms.vdouble( 0.714,  0.618,  0.527,  0.443,  0.304),
        scale   = cms.vdouble( 0.030,  0.025,  0.022,  0.023,  0.025),
        kind    = cms.string('calo')
    ),
) 
