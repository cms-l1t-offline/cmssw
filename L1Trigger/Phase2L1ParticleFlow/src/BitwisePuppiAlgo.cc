#include "L1Trigger/Phase2L1ParticleFlow/interface/BitwisePuppiAlgo.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "L1Trigger/Phase2L1ParticleFlow/src/dbgPrintf.h"

#include "ref/linpuppi_ref.h"
int dr2_int(etaphi_t eta1, etaphi_t phi1, etaphi_t eta2, etaphi_t phi2) {
  ap_int<etaphi_t::width + 1> deta = (eta1 - eta2);
  ap_int<etaphi_t::width + 1> dphi = (phi1 - phi2);
  return deta * deta + dphi * dphi;
}

#include "utils/DiscretePF2Firmware.h"
#include "utils/Firmware2DiscretePF.h"

namespace {
  std::vector<float> getvf(const edm::ParameterSet &iConfig, const std::string &name) {
    const std::vector<double> &vd = iConfig.getParameter<std::vector<double>>(name);
    std::vector<float> ret;
    ret.insert(ret.end(), vd.begin(), vd.end());
    return ret;
  }
  std::vector<int> getvi(const edm::ParameterSet &iConfig, const std::string &name) {
    const std::vector<int32_t> &vd = iConfig.getParameter<std::vector<int32_t>>(name);
    std::vector<int> ret;
    ret.insert(ret.end(), vd.begin(), vd.end());
    return ret;
  }
  std::vector<unsigned int> getvu(const edm::ParameterSet &iConfig, const std::string &name) {
    const std::vector<uint32_t> &vd = iConfig.getParameter<std::vector<uint32_t>>(name);
    std::vector<unsigned int> ret;
    ret.insert(ret.end(), vd.begin(), vd.end());
    return ret;
  }

}  // namespace

using namespace l1tpf_impl;

BitwisePuppiAlgo::BitwisePuppiAlgo(const edm::ParameterSet &iConfig)
    : PUAlgoBase(iConfig), configs_(), debug_(iConfig.getUntrackedParameter<int>("puppiDebug", 0)) {
  const std::string &algo = iConfig.getParameter<std::string>("bitwisePUAlgo");
  if (algo == "linpuppi")
    algo_ = AlgoChoice::linpuppi;
  else if (algo == "linpuppi_flt")
    algo_ = AlgoChoice::linpuppi_flt;
  else if (algo == "fwdlinpuppi")
    algo_ = AlgoChoice::fwdlinpuppi;
  else if (algo == "fwdlinpuppi_flt")
    algo_ = AlgoChoice::fwdlinpuppi_flt;
  else
    throw cms::Exception("Configuration", "Unsupported bitwisePUAlgo " + algo);
  const edm::ParameterSet &pset = iConfig.getParameter<edm::ParameterSet>("bitwisePUConfig");
  for (int inv = 0; inv <= 1; ++inv) {
    configs_.push_back(std::make_shared<linpuppi_config>(pset.getParameter<uint32_t>("nTrack"),
                                                         pset.getParameter<uint32_t>("nIn"),
                                                         pset.getParameter<uint32_t>("nOut"),
                                                         pset.getParameter<uint32_t>("dR2Min"),
                                                         pset.getParameter<uint32_t>("dR2Max"),
                                                         pset.getParameter<uint32_t>("ptMax"),
                                                         pset.getParameter<uint32_t>("dzCut"),
                                                         getvi(pset, "absEtaBins"),
                                                         inv,
                                                         getvf(pset, "ptSlopeNe"),
                                                         getvf(pset, "ptSlopePh"),
                                                         getvf(pset, "ptZeroNe"),
                                                         getvf(pset, "ptZeroPh"),
                                                         getvf(pset, "alphaSlope"),
                                                         getvf(pset, "alphaZero"),
                                                         getvf(pset, "alphaCrop"),
                                                         getvf(pset, "priorNe"),
                                                         getvf(pset, "priorPh"),
                                                         getvu(pset, "ptCut")));
    const auto cfg = configs_.back();
    unsigned int neta = cfg->absEtaBins.size() + 1;
    if (cfg->ptSlopeNe.size() != neta || cfg->ptSlopePh.size() != neta || cfg->ptZeroNe.size() != neta ||
        cfg->ptZeroPh.size() != neta || cfg->alphaSlope.size() != neta || cfg->alphaZero.size() != neta ||
        cfg->alphaCrop.size() != neta || cfg->priorNe.size() != neta || cfg->priorPh.size() != neta ||
        cfg->ptCut.size() != neta) {
      throw cms::Exception("Configuration", "Mismatch in parameter vector sizes");
    }
    if (configs_.back()->absEtaBins.empty())
      break;  // no bins, no need to worry about inverting eta
  }
}

BitwisePuppiAlgo::~BitwisePuppiAlgo() {}

const std::vector<std::string> &BitwisePuppiAlgo::puGlobalNames() const {
  static const std::vector<std::string> names_;
  return names_;
}
void BitwisePuppiAlgo::doPUGlobals(const std::vector<Region> &rs,
                                   float z0,
                                   float npu,
                                   std::vector<float> &globals) const {}

void BitwisePuppiAlgo::runChargedPV(Region &r, float z0) const {
  if (algo_ == AlgoChoice::fwdlinpuppi || algo_ == AlgoChoice::fwdlinpuppi_flt) {
    return;
  }

  const linpuppi_config *cfg = configs_[0].get();
  if (!cfg->absEtaBins.empty() && r.etaCenter < 0)
    cfg = configs_[1].get();

  std::unique_ptr<PFChargedObj[]> in(new PFChargedObj[cfg->nTrack]), out(new PFChargedObj[cfg->nTrack]);
  std::vector<unsigned int> indices;
  for (unsigned int i = 0, n = r.pf.size(), nin = 0; i < n; ++i) {
    if (r.pf[i].hwId <= 1 || r.pf[i].hwId == 4) {
      r.pf[i].hwPuppiWeight = 0;  // init
      if (nin < cfg->nTrack) {
        indices.push_back(i);
        dpf2fw::convert(r.pf[i], in[nin]);
        nin++;
      }
    }
  }
  for (unsigned int i = indices.size(); i < cfg->nTrack; ++i) {
    clear(in[i]);
  }

  z0_t hwZ0 = int(std::round(z0 * l1tpf_impl::InputTrack::Z0_SCALE));

  if (debug_ && !r.pf.empty()) {
    dbgPrintf(
        "BitwisePuppiCharged\nBitwisePuppiCharged region eta [ %+5.2f , %+5.2f ], phi [ %+5.2f , %+5.2f ], algo = %d\n",
        r.etaMin - r.etaExtra,
        r.etaMax + r.etaExtra,
        r.phiCenter - r.phiHalfWidth - r.phiExtra,
        r.phiCenter + r.phiHalfWidth + r.phiExtra,
        int(algo_));
    dbgPrintf("BitwisePuppiCharged \t N(pfch) %3lu [max %2u]\n", indices.size(), cfg->nTrack);
    for (int ipf = 0, npf = indices.size(); ipf < npf; ++ipf) {
      const auto &pf = r.pf[indices[ipf]];
      dbgPrintf(
          "BitwisePuppiCharged \t pfch  %3d: pt %7.2f vtx eta %+5.2f  vtx phi %+5.2f  calo eta %+5.2f  calo phi %+5.2f "
          " pid %d    vz %+6.3f  dz %+6.3f \n",
          ipf,
          pf.floatPt(),
          pf.floatVtxEta(),
          pf.floatVtxPhi(),
          pf.floatEta(),
          pf.floatPhi(),
          int(pf.hwId),
          pf.floatDZ(),
          pf.floatDZ() - z0);
    }
  }

  linpuppi_chs_ref(*cfg, hwZ0, in.get(), out.get(), debug_);

  for (unsigned int i = 0, n = indices.size(); i < n; ++i) {
    unsigned int ipf = indices[i];
    if (out[i].hwPt > 0) {
      r.pf[ipf].setPuppiW(1.0f);
      r.puppi.push_back(r.pf[ipf]);
      r.puppi.back().hwPt = int(out[i].hwPt);
      if (debug_)
        dbgPrintf("BitwisePuppiCharged \t charged pf %3u (index %3u) pt %7.2f accepted by Puppi\n",
                  ipf,
                  i,
                  r.pf[ipf].floatPt());
    } else if (debug_)
      dbgPrintf("BitwisePuppiCharged \t charged pf %3u (index %3u) pt %7.2f discared by Puppi\n",
                ipf,
                i,
                r.pf[ipf].floatPt());
  }
}

void BitwisePuppiAlgo::runNeutralsPU(Region &r, float z0, float npu, const std::vector<float> &globals) const {
  const linpuppi_config *cfg = configs_[0].get();
  //if (!cfg->absEtaBins.empty() && r.etaCenter < 0) std::cout << "Using config with inverted eta for region with eta center " << r.etaCenter << std::endl;
  if (!cfg->absEtaBins.empty() && r.etaCenter < 0)
    cfg = configs_[1].get();

  std::unique_ptr<PFNeutralObj[]> outallne_nocut(new PFNeutralObj[cfg->nIn]);
  std::unique_ptr<PFNeutralObj[]> outallne(new PFNeutralObj[cfg->nIn]);
  std::unique_ptr<PFNeutralObj[]> outselne(new PFNeutralObj[cfg->nOut]);
  if (algo_ == AlgoChoice::linpuppi || algo_ == AlgoChoice::linpuppi_flt) {
    std::unique_ptr<TkObj[]> track(new TkObj[cfg->nTrack]);
    dpf2fw::convert(cfg->nTrack, r.track, track.get());

    z0_t hwZ0 = int(std::round(z0 * l1tpf_impl::InputTrack::Z0_SCALE));
    std::unique_ptr<PFNeutralObj[]> in(new PFNeutralObj[cfg->nIn]);

    std::vector<unsigned int> indices;
    for (unsigned int i = 0, n = r.pf.size(), nin = 0; i < n; ++i) {
      if (r.pf[i].hwId == 2 || r.pf[i].hwId == 3) {
        r.pf[i].hwPuppiWeight = 0;  // init
        if (nin < cfg->nIn) {
          indices.push_back(i);
          dpf2fw::convert(r.pf[i], in[nin]);
          nin++;
        }
      }
    }
    for (unsigned int i = indices.size(); i < cfg->nIn; ++i) {
      clear(in[i]);
    }

    if (debug_ && !r.pf.empty()) {
      dbgPrintf("BitwisePuppi\nBitwisePuppi region eta [ %+5.2f , %+5.2f ], phi [ %+5.2f , %+5.2f ], algo = %d\n",
                r.etaMin - r.etaExtra,
                r.etaMax + r.etaExtra,
                r.phiCenter - r.phiHalfWidth - r.phiExtra,
                r.phiCenter + r.phiHalfWidth + r.phiExtra,
                int(algo_));
      dbgPrintf("BitwisePuppi \t N(track) %3lu [max %2u]   N(pfne) %2lu [max %2u]    Nout %3u\n",
                r.track.size(),
                cfg->nTrack,
                indices.size(),
                cfg->nIn,
                cfg->nOut);
      for (int itk = 0, ntk = r.track.size(); itk < ntk; ++itk) {
        const auto &tk = r.track[itk];
        dbgPrintf(
            "BitwisePuppi \t track %3d: pt %7.2f vtx eta %+5.2f  vtx phi %+5.2f  calo eta %+5.2f  calo phi %+5.2f  vz "
            "%+6.3f  dz %+6.3f\n",
            itk,
            tk.floatPt(),
            tk.floatVtxEta(),
            tk.floatVtxPhi(),
            tk.floatEta(),
            tk.floatPhi(),
            tk.floatDZ(),
            tk.floatDZ() - z0);
      }
      for (int ipf = 0, npf = indices.size(); ipf < npf; ++ipf) {
        const auto &pf = r.pf[indices[ipf]];
        dbgPrintf(
            "BitwisePuppi \t pfne  %3d: pt %7.2f                               calo eta %+5.2f  calo phi %+5.2f  pid "
            "%d\n",
            ipf,
            pf.floatPt(),
            pf.floatEta(),
            pf.floatPhi(),
            int(pf.hwId));
      }
    }

    if (algo_ == AlgoChoice::linpuppi)
      linpuppi_ref(*cfg, track.get(), hwZ0, in.get(), outallne_nocut.get(), outallne.get(), outselne.get(), debug_);
    else
      linpuppi_flt(*cfg, track.get(), hwZ0, in.get(), outallne_nocut.get(), outallne.get(), outselne.get(), debug_);

    for (unsigned int i = 0, n = indices.size(); i < n; ++i) {
      unsigned int ipf = indices[i];
      if (outallne[i].hwPtPuppi > 0) {
        r.pf[ipf].setPuppiW(int(outallne[i].hwPtPuppi) / float(r.pf[ipf].hwPt));
        r.puppi.push_back(r.pf[ipf]);
        r.puppi.back().hwPt = int(outallne[i].hwPtPuppi);
        if (debug_)
          dbgPrintf("BitwisePuppi neutral %2u pt %7.2f [ %6d ] eta %+5.2f  phi %+5.2f --> pt %7.2f [ %6d, %6d ] \n",
                    i,
                    r.pf[ipf].floatPt(),
                    r.pf[ipf].hwPt,
                    r.pf[ipf].floatEta(),
                    r.pf[ipf].floatPhi(),
                    r.puppi.back().floatPt(),
                    r.puppi.back().hwPt,
                    int(outallne[i].hwPtPuppi));
      }
    }

  } else {
    std::unique_ptr<HadCaloObj[]> calo(new HadCaloObj[cfg->nIn]);
    dpf2fw::convert(cfg->nIn, r.calo, calo.get());

    if (debug_ && !r.calo.empty()) {
      dbgPrintf("BitwisePuppi\nBitwisePuppi region eta [ %+5.2f , %+5.2f ], phi [ %+5.2f , %+5.2f ], algo = %d\n",
                r.etaMin - r.etaExtra,
                r.etaMax + r.etaExtra,
                r.phiCenter - r.phiHalfWidth - r.phiExtra,
                r.phiCenter + r.phiHalfWidth + r.phiExtra,
                int(algo_));
      dbgPrintf("BitwisePuppi \t N(calo) %3lu [max %2u]    Nout %3u\n", r.calo.size(), cfg->nIn, cfg->nOut);
      for (int ic = 0, nc = r.calo.size(); ic < nc; ++ic) {
        const auto &c = r.calo[ic];
        dbgPrintf("BitwisePuppi \t calo  %3d: pt %7.2f eta %+5.2f  phi %+5.2f  isEM %1d\n",
                  ic,
                  c.floatPt(),
                  c.floatEta(),
                  c.floatPhi(),
                  int(c.isEM));
      }
    }

    if (algo_ == AlgoChoice::fwdlinpuppi)
      fwdlinpuppi_ref(*cfg, calo.get(), outallne_nocut.get(), outallne.get(), outselne.get(), debug_);
    else
      fwdlinpuppi_flt(*cfg, calo.get(), outallne_nocut.get(), outallne.get(), outselne.get(), debug_);

    fw2dpf::convert_puppi(cfg->nOut, outselne.get(), r.puppi);
  }

  if (debug_ &&
      !((algo_ == AlgoChoice::linpuppi || algo_ == AlgoChoice::linpuppi_flt) ? r.pf.empty() : r.calo.empty())) {
    dbgPrintf("BitwisePuppi \t Output N(ch) %3u/%3u   N(nh) %3u/%3u   N(ph) %3u/%u   [all/fiducial]\n",
              r.nOutput(l1tpf_impl::Region::charged_type, true, false),
              r.nOutput(l1tpf_impl::Region::charged_type, true, true),
              r.nOutput(l1tpf_impl::Region::neutral_hadron_type, true, false),
              r.nOutput(l1tpf_impl::Region::neutral_hadron_type, true, true),
              r.nOutput(l1tpf_impl::Region::photon_type, true, false),
              r.nOutput(l1tpf_impl::Region::photon_type, true, true));
    for (int ipf = 0, npf = r.puppi.size(); ipf < npf; ++ipf) {
      const auto &pf = r.puppi[ipf];
      dbgPrintf(
          "BitwisePuppi \t pf    %3d: pt %7.2f pid %d   vtx eta %+5.2f  vtx phi %+5.2f  calo eta %+5.2f  calo phi "
          "%+5.2f\n",
          ipf,
          pf.floatPt(),
          int(pf.hwId),
          pf.floatVtxEta(),
          pf.floatVtxPhi(),
          pf.floatEta(),
          pf.floatPhi());
    }
  }
}
