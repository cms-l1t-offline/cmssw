import FWCore.ParameterSet.Config as cms

#Parameterset for the hadronic shower trigger for Run-3
showerPSet = cms.PSet(
    ## what kind of shower triggers the logic?
    ## 0: cathode-only (TMB/OTMB)
    ## 1: anode-only (from ALCT board)
    ## 2: cathode or anode showers
    ##    loose -> 'loose anode or loose cathode'
    ##    nominal -> 'nominal anode or nominal cathode'
    ##    tight -> 'tight anode or tight cathode'
    source  = cms.uint32(0),

    ## settings for cathode showers (counting CSCComparatorDigi)
    cathodeShower = cms.PSet(
        ## {loose, nominal, tight} thresholds for hit counters
        showerThresholds = cms.vuint32(
            # ME1/1
            98, 99, 101,
            # ME1/2
            56, 62, 73,
            # ME1/3
            30, 31, 33,
            # ME2/1
            49, 53, 55,
            # ME2/2
            42, 43, 46,
            # ME3/1
            49, 50, 52,
            # ME3/2
            35, 36, 38,
            # ME4/1
            42, 49, 51,
            # ME4/2
            31, 34, 36
        ),
        showerMinTBin = cms.uint32(6),
        showerMaxTBin = cms.uint32(8),
    ),
    ## settings for anode showers (counting CSCWireDigi)
    anodeShower = cms.PSet(
        ## {loose, nominal, tight} thresholds for hit counters
        showerThresholds = cms.vuint32(
            # ME1/1
            104, 105, 107,
            # ME1/2
            92, 100, 102,
            # ME1/3
            32, 33, 48,
            # ME2/1
            133, 134, 136,
            # ME2/2
            83, 84, 86,
            # ME3/1
            130, 131, 133,
            # ME3/2
            74, 80, 87,
            # ME4/1
            127, 128, 130,
            # ME4/2
            88, 89, 94
        ),
        showerMinTBin = cms.uint32(7),
        showerMaxTBin = cms.uint32(9),
    )
)
