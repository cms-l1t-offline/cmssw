#ifndef FIRMWARE_TkJetWord_h
#define FIRMWARE_TkJetWord_h

#include <vector>
#include <ap_int.h>
#include <cassert>
#include <cmath>

namespace l1t {

  typedef ap_ufixed<14, 12, AP_TRN, AP_SAT> pt_t;
  typedef ap_int<10> eta_t;
  typedef ap_int<10> phi_t;
  typedef ap_int<12> glbeta_t;
  typedef ap_int<11> glbphi_t;
  typedef ap_int<10> z0_t;         // 40cm / 0.1
  typedef ap_uint<5> nt_t; //number of tracks
  typedef ap_uint<4> nx_t; //number of tracks with xbit = 1

  namespace Scales {
    const int INTPHI_PI = 720;
    const int INTPHI_TWOPI = 2 * INTPHI_PI;
    constexpr float INTPT_LSB = 0.25;
    constexpr float ETAPHI_LSB = M_PI / INTPHI_PI;
    constexpr float Z0_LSB = 0.05;
    inline float floatPt(pt_t pt) { return pt.to_float(); }
    inline int intPt(pt_t pt) { return (ap_ufixed<16, 14>(pt)).to_int(); }
    inline int intNt(nt_t nt) { return (ap_ufixed<7, 5>(nt)).to_int(); }
    inline int intXt(nx_t xt) { return (ap_ufixed<6, 4>(xt)).to_int(); }
    inline float floatEta(eta_t eta) { return eta.to_float() * ETAPHI_LSB; }
    inline float floatPhi(phi_t phi) { return phi.to_float() * ETAPHI_LSB; }
    inline float floatEta(glbeta_t eta) { return eta.to_float() * ETAPHI_LSB; }
    inline float floatPhi(glbphi_t phi) { return phi.to_float() * ETAPHI_LSB; }
    inline float floatZ0(z0_t z0) { return z0.to_float() * Z0_LSB; }

    inline pt_t makePt(int pt) { return ap_ufixed<16, 14>(pt); }
    inline nt_t makeNt(int nt) { return ap_ufixed<7, 5>(nt); }
    inline nx_t makeXt(int xt) { return ap_ufixed<6, 4>(xt); }
    inline pt_t makePtFromFloat(float pt) { return pt_t(0.25 * round(pt * 4)); }
    inline z0_t makeZ0(float z0) { return z0_t(round(z0 / Z0_LSB)); }

    inline ap_uint<pt_t::width> ptToInt(pt_t pt) {
      // note: this can be synthethized, e.g. when pT is used as intex in a LUT
      ap_uint<pt_t::width> ret = 0;
      ret(pt_t::width - 1, 0) = pt(pt_t::width - 1, 0);
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

    inline float maxAbsEta() { return ((1 << (eta_t::width - 1)) - 1) * ETAPHI_LSB; }
    inline float maxAbsPhi() { return ((1 << (phi_t::width - 1)) - 1) * ETAPHI_LSB; }
    inline float maxAbsGlbEta() { return ((1 << (glbeta_t::width - 1)) - 1) * ETAPHI_LSB; }
    inline float maxAbsGlbPhi() { return ((1 << (glbphi_t::width - 1)) - 1) * ETAPHI_LSB; }
  };  // namespace Scales


  struct TkJetWord {
    pt_t hwPt;
    glbeta_t hwEta;
    glbphi_t hwPhi;
    z0_t hwZ0;
    nt_t hwNt; //number of tracks
    nx_t hwXt; //number of tracks with xbit=1

    inline void clear() {
      hwPt = 0;
      hwEta = 0;
      hwPhi = 0;
      hwZ0 = 0;
      hwNt = 0;
      hwXt = 0;
    }

    int intPt() const { return Scales::intPt(hwPt); }
    int intNTracks() const { return Scales::intNt(hwNt); }
    int intNXTracks() const { return Scales::intXt(hwXt); }
    float floatPt() const { return Scales::floatPt(hwPt); }
    float floatEta() const { return Scales::floatEta(hwEta); }
    float floatPhi() const { return Scales::floatPhi(hwPhi); }
    float floatZ0() const { return Scales::floatZ0(hwZ0); }

  };
 

  typedef std::vector<l1t::TkJetWord> TkJetWordCollection;
  
}  // namespace l1ct

#endif
