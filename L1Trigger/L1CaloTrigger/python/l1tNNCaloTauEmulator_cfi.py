import FWCore.ParameterSet.Config as cms

l1tNNCaloTauEmulator = cms.EDProducer("l1tNNCaloTauEmulator",
    l1CaloTowers = cms.InputTag("l1tEGammaClusterEmuProducer","L1CaloTowerCollection",""), # uncalibrated towers (same input as L1TowerCalibrator)
    hgcalTowers = cms.InputTag("l1tHGCalTowerProducer","HGCalTowerProcessor"),

    HgcalClusters = cms.InputTag("l1tHGCalBackEndLayer2Producer","HGCalBackendLayer2Processor3DClustering"),
    preEmId  = cms.string("hOverE < 0.3 && hOverE >= 0"),
    VsPuId = cms.PSet(
        isPUFilter = cms.bool(True),
        preselection = cms.string(""),
        method = cms.string("BDT"), # "" to be disabled, "BDT" to be enabled
        variables = cms.VPSet(
            cms.PSet(name = cms.string("eMax"), value = cms.string("eMax()")),
            cms.PSet(name = cms.string("eMaxOverE"), value = cms.string("eMax()/energy()")),
            cms.PSet(name = cms.string("sigmaPhiPhiTot"), value = cms.string("sigmaPhiPhiTot()")),
            cms.PSet(name = cms.string("sigmaRRTot"), value = cms.string("sigmaRRTot()")),
            cms.PSet(name = cms.string("triggerCells90percent"), value = cms.string("triggerCells90percent()")),
        ),
        weightsFile = cms.string("L1Trigger/Phase2L1ParticleFlow/data/hgcal_egID/Photon_Pion_vs_Neutrino_BDTweights_1116.xml.gz"),
        wp = cms.string("-0.10")
    ),

    EcalEtMinForClustering = cms.double(0.),
    HcalEtMinForClustering = cms.double(0.),
    EtMinForSeeding = cms.double(2.5),
    EtaRestriction = cms.double(3.0), # the TauMinator algo can go up to 3.0, but 2.4 makes it directly comparable to "Square Calo Tau" algo
    PuidThr = cms.double(-0.10),

    CNNmodel_CB_path = cms.string("L1Trigger/L1CaloTrigger/data/Phase2_NNCaloTaus/CNNmodel_CB.pb"),
    DNNident_CB_path = cms.string("L1Trigger/L1CaloTrigger/data/Phase2_NNCaloTaus/DNNident_CB.pb"),
    DNNcalib_CB_path = cms.string("L1Trigger/L1CaloTrigger/data/Phase2_NNCaloTaus/DNNcalib_CB.pb"),

    CNNmodel_CE_path = cms.string("L1Trigger/L1CaloTrigger/data/Phase2_NNCaloTaus/CNNmodel_CE.pb"),
    DNNident_CE_path = cms.string("L1Trigger/L1CaloTrigger/data/Phase2_NNCaloTaus/DNNident_CE.pb"),
    DNNcalib_CE_path = cms.string("L1Trigger/L1CaloTrigger/data/Phase2_NNCaloTaus/DNNcalib_CE.pb"),
    FeatScaler_CE_path = cms.string("L1Trigger/L1CaloTrigger/data/Phase2_NNCaloTaus/Cl3dFeatScaler_CE.json"),

    IdWp90_CB = cms.double(0.7310),
    IdWp95_CB = cms.double(0.3573),
    IdWp99_CB = cms.double(0.0410),

    IdWp90_CE = cms.double(0.6243),
    IdWp95_CE = cms.double(0.2994),
    IdWp99_CE = cms.double(0.0413),

    DEBUG = cms.bool(False)
)
