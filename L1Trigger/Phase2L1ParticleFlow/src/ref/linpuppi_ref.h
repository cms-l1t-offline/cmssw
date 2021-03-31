#ifndef L1Trigger_Phase2L1ParticleFlow_LINPUPPI_REF_H
#define L1Trigger_Phase2L1ParticleFlow_LINPUPPI_REF_H

#ifdef CMSSW_GIT_HASH
#include "../firmware/linpuppi.h"
#else
#include "firmware/linpuppi.h"
#endif
#include <vector>

struct linpuppi_config {
  unsigned int nTrack, nIn,
      nOut;  // nIn, nOut refer to the calorimeter clusters or neutral PF candidates as input and as output (after sorting)
  unsigned int dR2Min, dR2Max, ptMax, dzCut;
  std::vector<int> absEtaBins;
  bool invertEtaBins;
  std::vector<float> ptSlopeNe, ptSlopePh, ptZeroNe, ptZeroPh;
  std::vector<float> alphaSlope, alphaZero, alphaCrop;
  std::vector<float> priorNe, priorPh;
  std::vector<unsigned int> ptCut;

  linpuppi_config(unsigned int nTrack_,
                  unsigned int nIn_,
                  unsigned int nOut_,
                  unsigned int dR2Min_,
                  unsigned int dR2Max_,
                  unsigned int ptMax_,
                  unsigned int dzCut_,
                  float ptSlopeNe_,
                  float ptSlopePh_,
                  float ptZeroNe_,
                  float ptZeroPh_,
                  float alphaSlope_,
                  float alphaZero_,
                  float alphaCrop_,
                  float priorNe_,
                  float priorPh_,
                  unsigned int ptCut_)
      : nTrack(nTrack_),
        nIn(nIn_),
        nOut(nOut_),
        dR2Min(dR2Min_),
        dR2Max(dR2Max_),
        ptMax(ptMax_),
        dzCut(dzCut_),
        absEtaBins(),
        invertEtaBins(false),
        ptSlopeNe(1, ptSlopeNe_),
        ptSlopePh(1, ptSlopePh_),
        ptZeroNe(1, ptZeroNe_),
        ptZeroPh(1, ptZeroPh_),
        alphaSlope(1, alphaSlope_),
        alphaZero(1, alphaZero_),
        alphaCrop(1, alphaCrop_),
        priorNe(1, priorNe_),
        priorPh(1, priorPh_),
        ptCut(1, ptCut_) {}

  linpuppi_config(unsigned int nTrack_,
                  unsigned int nIn_,
                  unsigned int nOut_,
                  unsigned int dR2Min_,
                  unsigned int dR2Max_,
                  unsigned int ptMax_,
                  unsigned int dzCut_,
                  int etaCut_,
                  bool invertEtaBins_,
                  float ptSlopeNe_0,
                  float ptSlopeNe_1,
                  float ptSlopePh_0,
                  float ptSlopePh_1,
                  float ptZeroNe_0,
                  float ptZeroNe_1,
                  float ptZeroPh_0,
                  float ptZeroPh_1,
                  float alphaSlope_0,
                  float alphaSlope_1,
                  float alphaZero_0,
                  float alphaZero_1,
                  float alphaCrop_0,
                  float alphaCrop_1,
                  float priorNe_0,
                  float priorNe_1,
                  float priorPh_0,
                  float priorPh_1,
                  unsigned int ptCut_0,
                  unsigned int ptCut_1);

  linpuppi_config(unsigned int nTrack_,
                  unsigned int nIn_,
                  unsigned int nOut_,
                  unsigned int dR2Min_,
                  unsigned int dR2Max_,
                  unsigned int ptMax_,
                  unsigned int dzCut_,
                  const std::vector<int> &absEtaBins_,
                  bool invertEtaBins_,
                  const std::vector<float> &ptSlopeNe_,
                  const std::vector<float> &ptSlopePh_,
                  const std::vector<float> &ptZeroNe_,
                  const std::vector<float> &ptZeroPh_,
                  const std::vector<float> &alphaSlope_,
                  const std::vector<float> &alphaZero_,
                  const std::vector<float> &alphaCrop_,
                  const std::vector<float> &priorNe_,
                  const std::vector<float> &priorPh_,
                  const std::vector<unsigned int> &ptCut_)
      : nTrack(nTrack_),
        nIn(nIn_),
        nOut(nOut_),
        dR2Min(dR2Min_),
        dR2Max(dR2Max_),
        ptMax(ptMax_),
        dzCut(dzCut_),
        absEtaBins(absEtaBins_),
        invertEtaBins(invertEtaBins_),
        ptSlopeNe(ptSlopeNe_),
        ptSlopePh(ptSlopePh_),
        ptZeroNe(ptZeroNe_),
        ptZeroPh(ptZeroPh_),
        alphaSlope(alphaSlope_),
        alphaZero(alphaZero_),
        alphaCrop(alphaCrop_),
        priorNe(priorNe_),
        priorPh(priorPh_),
        ptCut(ptCut_) {}
};

// charged
void linpuppi_chs_ref(const linpuppi_config &cfg,
                      z0_t pvZ0,
                      const PFChargedObj pfch[/*cfg.nTrack*/],
                      PFChargedObj outallch[/*cfg.nTrack*/],
                      bool debug);

// neutrals, in the tracker
void linpuppi_flt(const linpuppi_config &cfg,
                  const TkObj track[/*cfg.nTrack*/],
                  z0_t pvZ0,
                  const PFNeutralObj pfallne[/*cfg.nIn*/],
                  PFNeutralObj outallne_nocut[/*cfg.nIn*/],
                  PFNeutralObj outallne[/*cfg.nIn*/],
                  PFNeutralObj outselne[/*cfg.nOut*/],
                  bool debug);
void linpuppi_ref(const linpuppi_config &cfg,
                  const TkObj track[/*cfg.nTrack*/],
                  z0_t pvZ0,
                  const PFNeutralObj pfallne[/*cfg.nIn*/],
                  PFNeutralObj outallne_nocut[/*cfg.nIn*/],
                  PFNeutralObj outallne[/*cfg.nIn*/],
                  PFNeutralObj outselne[/*cfg.nOut*/],
                  bool debug);

// neutrals, forward
void fwdlinpuppi_ref(const linpuppi_config &cfg,
                     const HadCaloObj caloin[/*cfg.nIn*/],
                     PFNeutralObj outallne_nocut[/*cfg.nIn*/],
                     PFNeutralObj outallne[/*cfg.nIn*/],
                     PFNeutralObj outselne[/*cfg.nOut*/],
                     bool debug);
void fwdlinpuppi_flt(const linpuppi_config &cfg,
                     const HadCaloObj caloin[/*cfg.nIn*/],
                     PFNeutralObj outallne_nocut[/*cfg.nIn*/],
                     PFNeutralObj outallne[/*cfg.nIn*/],
                     PFNeutralObj outselne[/*cfg.nOut*/],
                     bool debug);

#endif
