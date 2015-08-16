
# these files allow to test the workflows starting from different event content

# AOD
def RelValInputFile_AOD():
    return '/store/relval/CMSSW_7_4_8_patch1/RelValProdTTbar_13/AODSIM/MCRUN2_74_V11_mulTrh-v1/00000/0A41D4BF-523C-E511-A8E1-0026189438FD.root'

# DIGI (only available in RelVal, not a realistic workflow)
def RelValInputFile_DIGI():
    return ''

# RAW (need to run RawToDigi to use this)
def RelValInputFile_RAW():
    return '/store/relval/CMSSW_7_4_8_patch1/RelValProdTTbar_13/GEN-SIM-RAW/MCRUN2_74_V11_mulTrh-v1/00000/2AEA6F95-4A3C-E511-8B2F-0025905A48FC.root'

#'/store/relval/CMSSW_7_5_0_pre1/RelValProdTTbar_13/GEN-SIM-RAW/MCRUN2_74_V7-v1/00000/0CEB1526-6CE3-E411-82B6-00261894386C.root'

