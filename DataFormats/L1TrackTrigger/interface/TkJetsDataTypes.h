#ifndef FIRMWARE_dataformats_datatypes_h
#define FIRMWARE_dataformats_datatypes_h

#if (!defined(__CLANG__)) && defined(__GNUC__) && defined(CMSSW_GIT_HASH)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wint-in-bool-context"
#pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
#pragma GCC diagnostic ignored "-Wuninitialized"
#endif
#include <ap_int.h>
#if (!defined(__CLANG__)) && defined(__GNUC__) && defined(CMSSW_GIT_HASH)
#pragma GCC diagnostic pop
#endif

#include <ap_int.h>
#include <cassert>
#include <cmath>

namespace l1t {

  typedef ap_ufixed<14, 12, AP_TRN, AP_SAT> pt_t;
  typedef ap_ufixed<10, 8, AP_TRN, AP_SAT> pt10_t;
  typedef ap_fixed<16, 14, AP_TRN, AP_SAT> dpt_t;
  typedef ap_ufixed<28, 24, AP_TRN, AP_SAT> pt2_t;
  typedef ap_int<10> eta_t;
  typedef ap_int<10> phi_t;
  typedef ap_int<6> tkdeta_t;
  typedef ap_uint<7> tkdphi_t;
  typedef ap_int<12> glbeta_t;
  typedef ap_int<11> glbphi_t;
  typedef ap_int<5> vtx_t;
  typedef ap_int<10> z0_t;         // 40cm / 0.1
  typedef ap_int<8> dxy_t;         // tbd
  typedef ap_uint<3> tkquality_t;  // tbd
  typedef ap_uint<9> puppiWgt_t;   // 256 = 1.0
  typedef ap_uint<14> tk2em_dr_t;
  typedef ap_uint<14> tk2calo_dr_t;
  typedef ap_uint<10> em2calo_dr_t;
  typedef ap_uint<13> tk2calo_dq_t;
  typedef ap_uint<4> egquality_t;
  typedef ap_uint<5> nt_t; //number of tracks
  typedef ap_uint<4> nx_t; //number of tracks with xbit = 1
  // FIXME: adjust range 10-11bits -> 1/4 - 1/2TeV is probably more than enough for all reasonable use cases
  typedef ap_ufixed<11, 9, AP_TRN, AP_SAT> iso_t;

  namespace Scales {
    const int INTPHI_PI = 720;
    const int INTPHI_TWOPI = 2 * INTPHI_PI;
    constexpr float INTPT_LSB = 0.25;
    constexpr float ETAPHI_LSB = M_PI / INTPHI_PI;
    constexpr float Z0_LSB = 0.05;
    inline float floatPt(pt_t pt) { return pt.to_float(); }
    inline float floatPt(dpt_t pt) { return pt.to_float(); }
    inline float floatPt(pt2_t pt2) { return pt2.to_float(); }
    inline int intPt(pt_t pt) { return (ap_ufixed<16, 14>(pt) << 2).to_int(); }
    inline int intPt(dpt_t pt) { return (ap_fixed<18, 16>(pt) << 2).to_int(); }
    inline int intNt(nt_t nt) { return (ap_ufixed<7, 5>(nt) << 2).to_int(); }
    inline int intXt(nx_t xt) { return (ap_ufixed<6, 4>(xt) << 2).to_int(); }
    inline float floatEta(eta_t eta) { return eta.to_float() * ETAPHI_LSB; }
    inline float floatPhi(phi_t phi) { return phi.to_float() * ETAPHI_LSB; }
    inline float floatEta(tkdeta_t eta) { return eta.to_float() * ETAPHI_LSB; }
    inline float floatPhi(tkdphi_t phi) { return phi.to_float() * ETAPHI_LSB; }
    inline float floatEta(glbeta_t eta) { return eta.to_float() * ETAPHI_LSB; }
    inline float floatPhi(glbphi_t phi) { return phi.to_float() * ETAPHI_LSB; }
    inline float floatZ0(z0_t z0) { return z0.to_float() * Z0_LSB; }

    inline pt_t makePt(int pt) { return ap_ufixed<16, 14>(pt) >> 2; }
    inline dpt_t makeDPt(int dpt) { return ap_fixed<18, 16>(dpt) >> 2; }
    inline nt_t makeNt(int nt) { return ap_ufixed<7, 5>(nt) >> 2; }
    inline nx_t makeXt(int xt) { return ap_ufixed<6, 4>(xt) >> 2; }
    inline pt_t makePtFromFloat(float pt) { return pt_t(0.25 * round(pt * 4)); }
    inline dpt_t makeDPtFromFloat(float dpt) { return dpt_t(dpt); }
    inline z0_t makeZ0(float z0) { return z0_t(round(z0 / Z0_LSB)); }

    inline ap_uint<pt_t::width> ptToInt(pt_t pt) {
      // note: this can be synthethized, e.g. when pT is used as intex in a LUT
      ap_uint<pt_t::width> ret = 0;
      ret(pt_t::width - 1, 0) = pt(pt_t::width - 1, 0);
      return ret;
    }

    inline ap_int<dpt_t::width> ptToInt(dpt_t pt) {
      // note: this can be synthethized, e.g. when pT is used as intex in a LUT
      ap_uint<dpt_t::width> ret = 0;
      ret(dpt_t::width - 1, 0) = pt(dpt_t::width - 1, 0);
      return ret;
    }

    inline phi_t makePhi(float phi) { return round(phi / ETAPHI_LSB); }
    inline eta_t makeEta(float eta) { return round(eta / ETAPHI_LSB); }
    inline glbeta_t makeGlbEta(float eta) { return round(eta / ETAPHI_LSB); }
    inline glbeta_t makeGlbEtaRoundEven(float eta) {
      glbeta_t ghweta = round(eta / ETAPHI_LSB);
      return (ghweta % 2) ? glbeta_t(ghweta + 1) : ghweta;
    }

    inline glbphi_t makeGlbPhi(float phi) { return round(phi / ETAPHI_LSB); }
    inline iso_t makeIso(float iso) { return iso_t(0.25 * round(iso * 4)); }

    inline int makeDR2FromFloatDR(float dr) { return ceil(dr * dr / ETAPHI_LSB / ETAPHI_LSB); }

    inline float maxAbsEta() { return ((1 << (eta_t::width - 1)) - 1) * ETAPHI_LSB; }
    inline float maxAbsPhi() { return ((1 << (phi_t::width - 1)) - 1) * ETAPHI_LSB; }
    inline float maxAbsGlbEta() { return ((1 << (glbeta_t::width - 1)) - 1) * ETAPHI_LSB; }
    inline float maxAbsGlbPhi() { return ((1 << (glbphi_t::width - 1)) - 1) * ETAPHI_LSB; }
  };  // namespace Scales

  inline int dr2_int(eta_t eta1, phi_t phi1, eta_t eta2, phi_t phi2) {
    ap_int<eta_t::width + 1> deta = (eta1 - eta2);
    ap_int<phi_t::width + 1> dphi = (phi1 - phi2);
    return deta * deta + dphi * dphi;
  }

}  // namespace l1ct

#endif
