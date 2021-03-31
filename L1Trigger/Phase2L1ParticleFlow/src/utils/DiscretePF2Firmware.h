#ifndef L1Trigger_Phase2L1ParticleFlow_DISCRETEPF2FIRMWARE_H
#define L1Trigger_Phase2L1ParticleFlow_DISCRETEPF2FIRMWARE_H

/// NOTE: this include is not standalone, since the path to DiscretePFInputs is different in CMSSW & Vivado_HLS

#include "../firmware/data.h"
#include <vector>

namespace dpf2fw {

  // convert inputs from discrete to firmware
  inline void convert(const l1tpf_impl::PropagatedTrack &in, TkObj &out) {
    out.hwPt = in.hwPt;
    out.hwPtErr = in.hwCaloPtErr;
    out.hwEta = in.hwEta;  // @calo
    out.hwPhi = in.hwPhi;  // @calo
    out.hwZ0 = in.hwZ0;
    out.hwTightQuality = (in.hwStubs >= 6 && in.hwChi2 < 500);
  }

  inline TkObj transformConvert(const l1tpf_impl::PropagatedTrack &in) {
    TkObj out;
    convert(in, out);
    return out;
  }

  inline void convert(const l1tpf_impl::CaloCluster &in, HadCaloObj &out) {
    out.hwPt = in.hwPt;
    out.hwEmPt = in.hwEmPt;
    out.hwEta = in.hwEta;
    out.hwPhi = in.hwPhi;
    out.hwIsEM = in.isEM;
  }
  inline void convert(const l1tpf_impl::CaloCluster &in, EmCaloObj &out) {
    out.hwPt = in.hwPt;
    out.hwPtErr = in.hwPtErr;
    out.hwEta = in.hwEta;
    out.hwPhi = in.hwPhi;
  }
  inline void convert(const l1tpf_impl::Muon &in, MuObj &out) {
    out.hwPt = in.hwPt;
    out.hwPtErr = 0;       // does not exist in input
    out.hwEta = in.hwEta;  // @calo
    out.hwPhi = in.hwPhi;  // @calo
  }

  inline void convert(const l1tpf_impl::PFParticle &src, PFChargedObj &pf) {
    pf.hwPt = src.hwPt;
    pf.hwEta = src.hwEta;
    pf.hwPhi = src.hwPhi;
    switch (src.hwId) {
      case 0:
        pf.hwId = PID_Charged;
        break;
      case 1:
        pf.hwId = PID_Electron;
        break;
      case 2:
        pf.hwId = PID_Neutral;
        break;
      case 3:
        pf.hwId = PID_Photon;
        break;
      case 4:
        pf.hwId = PID_Muon;
        break;
    }
    pf.hwZ0 = src.track.hwZ0;
  }
  inline void convert(const l1tpf_impl::PFParticle &src, PFNeutralObj &pf, bool isPuppi = false) {
    pf.hwPt = src.hwPt;
    pf.hwEta = src.hwEta;
    pf.hwPhi = src.hwPhi;
    switch (src.hwId) {
      case 0:
        pf.hwId = PID_Charged;
        break;
      case 1:
        pf.hwId = PID_Electron;
        break;
      case 2:
        pf.hwId = PID_Neutral;
        break;
      case 3:
        pf.hwId = PID_Photon;
        break;
      case 4:
        pf.hwId = PID_Muon;
        break;
    }
    pf.hwPtPuppi = isPuppi ? pt_t(src.hwPt) : pt_t(0);
  }

  template <unsigned int NMAX, typename In, typename Out>
  void convert(const std::vector<In> &in, Out out[NMAX]) {
    for (unsigned int i = 0, n = std::min<unsigned int>(NMAX, in.size()); i < n; ++i) {
      convert(in[i], out[i]);
    }
    for (unsigned int i = in.size(); i < NMAX; ++i) {
      clear(out[i]);
    }
  }

  template <typename In, typename Out>
  void convert(unsigned int NMAX, const std::vector<In> &in, Out out[]) {
    for (unsigned int i = 0, n = std::min<unsigned int>(NMAX, in.size()); i < n; ++i) {
      convert(in[i], out[i]);
    }
    for (unsigned int i = in.size(); i < NMAX; ++i) {
      clear(out[i]);
    }
  }

}  // namespace dpf2fw

#endif
