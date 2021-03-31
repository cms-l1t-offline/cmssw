#ifndef L1Trigger_Phase2L1ParticleFlow_FIRMWARE_LINPUPPI_H
#define L1Trigger_Phase2L1ParticleFlow_FIRMWARE_LINPUPPI_H

#include <cmath>
#ifdef CMSSW_GIT_HASH
#include "data.h"
#else
#include "../../firmware/data.h"
#endif

#if defined(PACKING_DATA_SIZE) && defined(PACKING_NCHANN)
#include "../../firmware/l1pf_encoding.h"
#endif

int dr2_int(etaphi_t eta1, etaphi_t phi1, etaphi_t eta2, etaphi_t phi2);

// charged
void linpuppi_chs(z0_t pvZ0, const PFChargedObj pfch[NTRACK], PFChargedObj outallch[NTRACK]);

// neutrals, in the tracker
void linpuppiNoCrop(const TkObj track[NTRACK],
                    z0_t pvZ0,
                    const PFNeutralObj pfallne[NALLNEUTRALS],
                    PFNeutralObj outallne[NALLNEUTRALS]);
void linpuppi(const TkObj track[NTRACK],
              z0_t pvZ0,
              const PFNeutralObj pfallne[NALLNEUTRALS],
              PFNeutralObj outselne[NNEUTRALS]);

// neutrals, forward
void fwdlinpuppi(const HadCaloObj caloin[NCALO], PFNeutralObj pfselne[NNEUTRALS]);
void fwdlinpuppiNoCrop(const HadCaloObj caloin[NCALO], PFNeutralObj pfallne[NCALO]);

#if defined(PACKING_DATA_SIZE) && defined(PACKING_NCHANN)
void packed_fwdlinpuppi(const ap_uint<PACKING_DATA_SIZE> input[PACKING_NCHANN],
                        ap_uint<PACKING_DATA_SIZE> output[PACKING_NCHANN]);
void packed_fwdlinpuppiNoCrop(const ap_uint<PACKING_DATA_SIZE> input[PACKING_NCHANN],
                              ap_uint<PACKING_DATA_SIZE> output[PACKING_NCHANN]);

void packed_linpuppi_chs(const ap_uint<PACKING_DATA_SIZE> input[PACKING_NCHANN],
                         ap_uint<PACKING_DATA_SIZE> output[PACKING_NCHANN]);
void packed_linpuppi(const ap_uint<PACKING_DATA_SIZE> input[PACKING_NCHANN],
                     ap_uint<PACKING_DATA_SIZE> output[PACKING_NCHANN]);
void packed_linpuppiNoCrop(const ap_uint<PACKING_DATA_SIZE> input[PACKING_NCHANN],
                           ap_uint<PACKING_DATA_SIZE> output[PACKING_NCHANN]);

void linpuppi_pack_in(const TkObj track[NTRACK],
                      z0_t pvZ0,
                      const PFNeutralObj pfallne[NALLNEUTRALS],
                      ap_uint<PACKING_DATA_SIZE> input[PACKING_NCHANN]);
void linpuppi_unpack_in(const ap_uint<PACKING_DATA_SIZE> input[PACKING_NCHANN],
                        TkObj track[NTRACK],
                        z0_t& pvZ0,
                        PFNeutralObj pfallne[NALLNEUTRALS]);
void linpuppi_chs_pack_in(z0_t pvZ0, const PFChargedObj pfch[NTRACK], ap_uint<PACKING_DATA_SIZE> input[PACKING_NCHANN]);
void linpuppi_chs_unpack_in(const ap_uint<PACKING_DATA_SIZE> input[PACKING_NCHANN],
                            z0_t& pvZ0,
                            PFChargedObj pfch[NTRACK]);
void linpuppi_pack_pv(z0_t pvZ0, ap_uint<PACKING_DATA_SIZE>& word);
void linpuppi_unpack_pv(ap_uint<PACKING_DATA_SIZE> word, z0_t& pvZ0);
#endif

void linpuppi_set_debug(bool debug);

#define LINPUPPI_ptLSB 0.25
#define LINPUPPI_DR2LSB 1.9e-5
#define LINPUPPI_dzLSB 0.05
#define LINPUPPI_pt2LSB LINPUPPI_ptLSB* LINPUPPI_ptLSB
#define LINPUPPI_pt2DR2_scale LINPUPPI_ptLSB* LINPUPPI_ptLSB / LINPUPPI_DR2LSB

#define LINPUPPI_sum_bitShift 15
#define LINPUPPI_x2_bits 6          // decimal bits the discriminator values
#define LINPUPPI_alpha_bits 5       // decimal bits of the alpha values
#define LINPUPPI_alphaSlope_bits 5  // decimal bits of the alphaSlope values
#define LINPUPPI_ptSlope_bits 6     // decimal bits of the ptSlope values
#define LINPUPPI_weight_bits 8

//=================================================
#if defined(REG_Barrel)

#define LINPUPPI_DR2MAX 4727  // 0.3 cone
#define LINPUPPI_DR2MIN 257   // 0.07 cone
#define LINPUPPI_dzCut 10
#define LINPUPPI_ptMax 200  // 50.0/LINPUPPI_ptLSB

#define LINPUPPI_ptSlopeNe 0.3
#define LINPUPPI_ptSlopePh 0.3
#define LINPUPPI_ptZeroNe 4.0
#define LINPUPPI_ptZeroPh 2.5
#define LINPUPPI_alphaSlope 0.7
#define LINPUPPI_alphaZero 6.0
#define LINPUPPI_alphaCrop 4.0
#define LINPUPPI_priorNe 5.0
#define LINPUPPI_priorPh 1.0

#define LINPUPPI_ptCut 4  // 1.0/LINPUPPI_ptLSB

//=================================================
#elif defined(REG_HGCal)

#define LINPUPPI_etaBins 2
#define LINPUPPI_etaCut 0
// assuming the region spans [1.5,2.5] (or 1.25,2.75 with the overlaps),
// the cut is exactly at the region center (2.0), so it's at 0 in integer coordinates
#define LINPUPPI_invertEta 0  // 0 if we're building the FW for the positive eta endcap.

#define LINPUPPI_DR2MAX 4727  // 0.3 cone
#define LINPUPPI_DR2MIN 84    // 0.04 cone
#define LINPUPPI_dzCut 40
#define LINPUPPI_ptMax 200  // 50.0/LINPUPPI_ptLSB

#define LINPUPPI_ptSlopeNe 0.3
#define LINPUPPI_ptSlopePh 0.4
#define LINPUPPI_ptZeroNe 5.0
#define LINPUPPI_ptZeroPh 3.0
#define LINPUPPI_alphaSlope 1.5
#define LINPUPPI_alphaZero 6.0
#define LINPUPPI_alphaCrop 3.0
#define LINPUPPI_priorNe 5.0
#define LINPUPPI_priorPh 1.5

#define LINPUPPI_ptSlopeNe_1 0.3
#define LINPUPPI_ptSlopePh_1 0.4
#define LINPUPPI_ptZeroNe_1 7.0
#define LINPUPPI_ptZeroPh_1 4.0
#define LINPUPPI_alphaSlope_1 1.5
#define LINPUPPI_alphaZero_1 6.0
#define LINPUPPI_alphaCrop_1 3.0
#define LINPUPPI_priorNe_1 5.0
#define LINPUPPI_priorPh_1 1.5

#define LINPUPPI_ptCut 4    // 1.0/LINPUPPI_ptLSB
#define LINPUPPI_ptCut_1 8  // 2.0/LINPUPPI_ptLSB

//=================================================
#elif defined(REG_HGCalNoTK)

#define LINPUPPI_DR2MAX 4727  // 0.3 cone
#define LINPUPPI_DR2MIN 84    // 0.04 cone
#define LINPUPPI_dzCut 40     // unused
#define LINPUPPI_ptMax 200    // 50.0/LINPUPPI_ptLSB

#define LINPUPPI_ptSlopeNe 0.3
#define LINPUPPI_ptSlopePh 0.4
#define LINPUPPI_ptZeroNe 9.0
#define LINPUPPI_ptZeroPh 5.0
#define LINPUPPI_alphaSlope 2.2
#define LINPUPPI_alphaZero 9.0
#define LINPUPPI_alphaCrop 4.0
#define LINPUPPI_priorNe 7.0
#define LINPUPPI_priorPh 5.0

#define LINPUPPI_ptCut 16  // 4.0/LINPUPPI_ptLSB

//=================================================
#elif defined(REG_HF)

#define LINPUPPI_DR2MAX 4727  // 0.3 cone
#define LINPUPPI_DR2MIN 525   // 0.1 cone
#define LINPUPPI_dzCut 40     // unused

#define LINPUPPI_ptMax 400  // 100.0/LINPUPPI_ptLSB

#define LINPUPPI_ptSlopeNe 0.25
#define LINPUPPI_ptSlopePh 0.25
#define LINPUPPI_ptZeroNe 14.
#define LINPUPPI_ptZeroPh 14.
#define LINPUPPI_alphaSlope 0.6
#define LINPUPPI_alphaZero 9.0
#define LINPUPPI_alphaCrop 4.0
#define LINPUPPI_priorNe 6.0
#define LINPUPPI_priorPh 6.0

#define LINPUPPI_ptCut 40  // 10.0/LINPUPPI_ptLSB

#endif

#endif
