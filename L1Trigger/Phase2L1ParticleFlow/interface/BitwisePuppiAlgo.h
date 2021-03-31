#ifndef L1Trigger_Phase2L1ParticleFlow_BitwisePuppiAlgo_h
#define L1Trigger_Phase2L1ParticleFlow_BitwisePuppiAlgo_h

#include "L1Trigger/Phase2L1ParticleFlow/interface/PUAlgoBase.h"

struct linpuppi_config;

namespace l1tpf_impl {

  class BitwisePuppiAlgo : public PUAlgoBase {
  public:
    BitwisePuppiAlgo(const edm::ParameterSet &);
    ~BitwisePuppiAlgo() override;

    const std::vector<std::string> &puGlobalNames() const override;
    void doPUGlobals(const std::vector<Region> &rs, float z0, float npu, std::vector<float> &globals) const override;
    void runChargedPV(Region &r, float z0) const override;
    void runNeutralsPU(Region &r, float z0, float npu, const std::vector<float> &globals) const override;

  private:
    enum class AlgoChoice { linpuppi, linpuppi_flt, fwdlinpuppi, fwdlinpuppi_flt } algo_;
    std::vector<std::shared_ptr<linpuppi_config>> configs_;
    bool debug_;
  };

}  // namespace l1tpf_impl

#endif
